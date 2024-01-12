#include "contriols.h"
#include "ui_contriols.h"
#include <QStandardItem>
#include <QListView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QHostAddress>
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QSerialPortInfo>

Contriols::Contriols(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Contriols)
{
    ui->setupUi(this);

//    初始化IP输入框
//    QRegularExpression rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
//    QRegularExpressionValidator* ipValidator = new QRegularExpressionValidator(rx, this);
//    ui->lineEdit_ip->setValidator(ipValidator);

//    ui->lineEdit_longtitude->setText(QString("121.456132")); ui->lineEdit_latitude->setText(QString("未收到"));

//    初始化端口名字
    QList<QSerialPortInfo> portNameList = QSerialPortInfo::availablePorts();
    for(int i = 0; i < portNameList.size(); i++){
        ui->comboBox_port->insertItem(i, portNameList[i].portName());
    }

    // 初始化定时器
    updateTimer = new QTimer(this);
    //connect(updateTimer, &QTimer::timeout, this, &Contriols::onUpdateTimerTimeout);
    updateTimer->start(2000); // 每100毫秒触发一次定时器

    QString logFileName = QDateTime::currentDateTime().toString("yyyy-MM-dd--hh_mm_ss");
    logFileName += ".csv";
    // 创建并打开日志文件，如果文件已经存在，会被清空
    logFile = new QFile(logFileName);
    if (logFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(logFile);
        stream << "Time,SXNumber,LNumber,ANumber\n";

    }

    connect_clicked();
    timeDuration.start();
    connect(this, SIGNAL(update_DashboardCourseSpeed(double )), ui->widget_DashboardCourseSpeed, SLOT(UpdateAngle(double)));


#ifdef TESTING_MODE
#else
    ui->pushButton_openPortArduino->setEnabled(false);
    ui->pushButton_openPortArduino->setVisible(false);
#endif

    ui->pushButton_switchControl->rl_flag = m_bIsControl;
    ui->pushButton_switchControl->update();
    connect(ui->pushButton_switchControl, SIGNAL(syncStatusControl(bool)), this, SLOT(On_receive_pushButton_switchControl(bool)));
}

Contriols::~Contriols()
{
    delete ui; ui = nullptr;
    if(!m_SerialWorker_DTU) delete m_SerialWorker_DTU; m_SerialWorker_DTU = nullptr;
    if(logFile->isOpen())logFile->close(); if(logFile) delete logFile; logFile = nullptr;
}

void Contriols::On_receive_DTU(QByteArray tmpdata)
{
    parseData(tmpdata);
}

void Contriols::On_receive_RudderBell(Bridge_ZL::Values_Bridge data_RudderBell)
{
    m_iCmdPropeller = -data_RudderBell.valueBellLeft;
    m_iCmdPropeller *= 10;
    m_iCmdPropeller += m_iBias_cmd_prop;
    m_iCmdPropeller = (m_iCmdPropeller + 5) / 10 * 10;

    m_iCmdRudder = -data_RudderBell.valueRudder;
    m_iCmdRudder += m_iBias_cmd_rudder;
    m_iCmdRudder = (m_iCmdRudder + 2) / 5 * 5;
    sendCmdToShip();

#ifdef TESTING_MODE

#else
#endif

}



void Contriols::sendCmdToShip(){
    QString formattedString_cmd = QString("S%1,%2,%3E").arg(m_iCmdRudder).arg(m_iCmdPropeller).arg(m_iCmdSideThuster);
    QByteArray byteArray_cmd = formattedString_cmd.toUtf8();
    ui->lineEdit_rudder->setText(QString::number(m_iCmdRudder - m_iBias_cmd_rudder));
    ui->lineEdit_propeller->setText(QString::number(m_iCmdPropeller - m_iBias_cmd_prop));
    ui->lineEdit_sideThruster->setText(QString::number(m_iCmdSideThuster - m_iBias_cmd_prop));

    qreal velocity_reV_m = (m_iCmdPropeller - m_iBias_cmd_prop) * cmd2velocity;
    ui->lineEdit_reV_m->setText(QString::number(velocity_reV_m, 'f', 2));
    ui->lineEdit_reV_kn->setText(QString::number( velocity_reV_m * m_s2kn, 'f', 1));

    if(m_SerialWorker_DTU){
        emit m_signalSendCmdToShip(byteArray_cmd);
    }

#ifdef TESTING_MODE
    QByteArray msgArduino;
    msgArduino.resize(10);
    int written = qsnprintf(msgArduino.data(), msgArduino.size(), "S%dM%dE", m_iCmdPropeller, m_iCmdRudder);
    if (written > -1) {
        msgArduino.resize(written);
//        qDebug() << "Formatted message:" << msgArduino;
    } else {
//        /qDebug() << "Error while formatting message.";
    }
    emit m_signalSendCmdToArduino(msgArduino);
#else
#endif

    m_Client->write(QString("test").toUtf8().data());
//    ui->lineEdit_reV_m =
}

void Contriols::parseData(QByteArray newData)
{

    int endIdx = newData.indexOf("E");
    if (endIdx != -1) {
        newData.remove(endIdx+1, newData.size()); // 删除已处理的数据包，包括'E\n'
        m_QSreceivedData += newData; // 追加新接收到的数据
//        qDebug() << "m_QSreceivedData:" << m_QSreceivedData;
        int startIdx = m_QSreceivedData.indexOf("X"); // 查找起始位置
        if (startIdx != -1) {   //一句完整的
            newData.remove(0, startIdx-1);
            m_QSreceivedData.remove(0, startIdx-1); // 删除已处理的数据包，包括'E\n'
            processData(m_QSreceivedData);
        }
        m_QSreceivedData = "";
        newData = "";
    }
    int startIdx = newData.indexOf("X");
    if (startIdx != -1) {
        newData.remove(0, startIdx-1); // 删除已处理的数据包，包括'E\n'
        m_QSreceivedData += newData; // 追加新接收到的数据
        endIdx = newData.indexOf("E"); // 查找结束位置
        if (endIdx != -1) {
            newData.remove(endIdx+1, newData.size()); // 删除已处理的数据包，包括'E\n'
            m_QSreceivedData = newData; // 追加新接收到的数据
            processData(m_QSreceivedData);
//            qDebug() << "m_QSreceivedData:" << m_QSreceivedData;
            m_QSreceivedData = "";
            newData = "";
        }
    }
//    qDebug() << "after process" << newData;
//    分成4条发送怎么办？/
//    m_QSreceivedData += newData;
    // 去除空串，并把含有信息但没有标识符的部分拼接到最后一个包中
    if (newData.isEmpty()) {
        newData = "";
    }
}

void Contriols::processData(const QByteArray& qbytearray){

    m_Client->write(QString(qbytearray).toUtf8().data());

    QString str = QString::fromUtf8(qbytearray);
    // 获取当前时间
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QRegularExpression regex("SX(-*[0-9.]+)L(-*[0-9.]+)A(-*[0-9.]+)E");
    QRegularExpressionMatch match = regex.match(str);

    if (match.hasMatch()) {
        qreal sxNumber = match.captured(1).toDouble();
        qreal lNumber =  match.captured(2).toDouble();
        qreal aNumber =  match.captured(3).toDouble();

//        qDebug() << "SX: " << sxNumber << " Long: " << lNumber << " Lat: " << aNumber;
        // 将输出语句修改为保存sxNumber、lNumber和aNumber的值
        ui->lineEdit_longtitude->setText(QString::number(lNumber, 'f', 6));
        ui->lineEdit_latitude->setText(QString::number(aNumber, 'f', 6));
        qreal velocity_abs = caculate_velocity_abs(lNumber, aNumber);
        ui->lineEdit_abV_m->setText(QString::number( velocity_abs, 'f', 2));
        ui->lineEdit_abV_kn->setText(QString::number( velocity_abs*1.94384, 'f', 1));
        if (sxNumber < 0) sxNumber += 360;
        emit update_DashboardCourseSpeed(sxNumber); //
//        if (logFile->open(QIODevice::Append | QIODevice::Text))
        if(logFile->isOpen())
        {
            QTextStream stream(logFile);
//            stream << currentTime << "," << sxNumber << "," << lNumber << "," << aNumber << "\n";
            stream << currentTime << "," << QString::number(sxNumber, 'f', 6) << ","
                   << QString::number(lNumber, 'f', 6) << "," << QString::number(aNumber, 'f', 6) << "\n";
//            logFile->close();
        }

        {
            //期望点
            //添加期望点test m_cAlgorithm_control.AddPoint(lat,lon);
            if(m_bIsControl) {
                m_cAlgorithm_control.AddPoints(121.534996, 38.865784);
                m_cAlgorithm_control.AddPoints(121.534683, 38.865903);
                m_cAlgorithm_control.AddPoints(121.534531, 38.865887);
                m_cAlgorithm_control.AddPoints(121.534760, 38.865757);

                ZLControl::GPSPoint GPS1(lNumber,aNumber);
                //GPS1.lon=lNumber;
                //GPS1.lat=aNumber;
                ZLControl::ControlInfo CIF;
                CIF =  m_cAlgorithm_control.CalMainTest(GPS1, sxNumber);
                m_iCmdRudder = CIF.DeltaE + m_iBias_cmd_rudder;
                m_iCmdPropeller = CIF.PropE + m_iBias_cmd_prop - 30;
                qDebug()<<"Rud:"<<CIF.DeltaE;
                qDebug()<<"Prop:"<<CIF.PropE;
                sendCmdToShip();

            }
        }

    }
}

//void Contriols::on_pushButton_connect_clicked()
void Contriols::connect_clicked()
{
//    if(ui->pushButton_connect->text() == "连接" && !m_Client) {
        //获取服务器IP和端口
        QString ip = "127.0.0.1"; //ui->lineEdit_ip->text();192.168.1.100
        qint16 port = 8888; //ui->spinBoxPortNetwork->text().toInt();
        //主动与服务器建立连接
        m_Client = new QTcpSocket(this);
        m_Client->connectToHost(QHostAddress(ip),port);
//        connect(m_Client,&QTcpSocket::connected, this,
//                [=]()
//                {
//                    ui->pushButton_connect->setText("断连");
//                }
//                );
        //获取编辑框内容
//        QString str = ui->pushButton_connect->text();
        //发送数据
//        m_Client->write(str.toUtf8().data());
//    }
//    else if(ui->pushButton_connect->text() == "断连") {
//        m_Client->close();
//        delete m_Client; m_Client = nullptr;
//        ui->pushButton_connect->setText("连接");
//    }

}


qreal Contriols::caculate_velocity_abs(qreal longtitude, qreal latitude)
{
    qreal velocity_abs = 0.0;
    Gps gps;
    qreal distance = gps.calculateDistance(longtitude_last, latitude_last, longtitude, latitude);
    timeDuration.stop();
    double duration = timeDuration.get_duration();
    timeDuration.start();
    velocity_abs = distance / duration;
    return velocity_abs;
}


void Contriols::on_pushButton_cmdForward_clicked()
{
    m_iCmdPropeller += m_iStepPropellar;
    if(m_iCmdPropeller > 100 + m_iBias_cmd_prop) m_iCmdPropeller -= m_iStepPropellar;
    //通信：发布命令到船端
    sendCmdToShip();
}


void Contriols::on_pushButton_CmdBack_clicked()
{
    m_iCmdPropeller -= m_iStepPropellar;
    if(m_iCmdPropeller < -100 + m_iBias_cmd_prop) m_iCmdPropeller += m_iStepPropellar;
    sendCmdToShip();
}


void Contriols::on_pushButton_CmdLeft_clicked()
{
    m_iCmdRudder -= m_iStepRudder;
    if(m_iCmdRudder < -35 + m_iBias_cmd_rudder) m_iCmdRudder += m_iStepRudder;
    sendCmdToShip();

}


void Contriols::on_pushButton_CmdRight_clicked()
{
    m_iCmdRudder += m_iStepRudder;
    if(m_iCmdRudder > 35 + m_iBias_cmd_rudder) m_iCmdRudder -= m_iStepRudder;
    sendCmdToShip();
}

void Contriols::on_pushButton_CmdLeftThruster_clicked()
{
    m_iCmdSideThuster -= m_iStepPropellar;
    if(m_iCmdSideThuster < -100 + m_iBias_cmd_prop) m_iCmdSideThuster += m_iStepPropellar;
    sendCmdToShip();
}


void Contriols::on_pushButton_CmdRightThruster_clicked()
{
    m_iCmdSideThuster += m_iStepPropellar;
    if(m_iCmdSideThuster > 100 + m_iBias_cmd_prop) m_iCmdSideThuster -= m_iStepPropellar;
    sendCmdToShip();
}



void Contriols::on_pushButton_openPortDTU_clicked()
{
    if(ui->pushButton_openPortDTU->text() == tr("连接网络"))
    {

        if (ui->comboBox_port->count() > 0)///当前列表的内容个数
        {///打开串口操作
            int SelBaudRate=9600;
            m_SerialWorker_DTU = new SerialWorker(ui->comboBox_port->currentText().toStdString().c_str(),
                                                  SelBaudRate,
                                                  QSerialPort::NoParity,
                                                  QSerialPort::Data8,
                                                  QSerialPort::OneStop,
                                                  QSerialPort::NoFlowControl,
                                                  500);

            connect(m_SerialWorker_DTU, SIGNAL(sendResultToGui(QByteArray)), this, SLOT(On_receive_DTU(QByteArray)), Qt::QueuedConnection);
            connect(this, SIGNAL(m_signalSendCmdToShip(QByteArray)), m_SerialWorker_DTU, SLOT(doDataSendWork(const QByteArray)), Qt::QueuedConnection);
            connect(this, SIGNAL(closeSerialPort_DTU()), m_SerialWorker_DTU, SLOT(close()), Qt::QueuedConnection);
            m_SerialWorker_DTU->start();
            if(m_SerialWorker_DTU->is_open())
            {
                ui->pushButton_openPortDTU->setText(tr("关闭网络"));        }
            else
            {
            QMessageBox::information(NULL,tr("information"),tr("open port error") + QString("\n\ncode: %1\nmessage: %2").arg(m_SerialWorker_DTU->getLastError()).arg(m_SerialWorker_DTU->getLastErrorMsg()));
                ui->pushButton_openPortDTU->setText(tr("连接网络"));
                qDebug()<< m_SerialWorker_DTU->getLastError();
            }
        }
        else
        {
            QMessageBox::information(NULL,tr("information"),tr("This Computer no avaiable port"));
            qDebug()<< "This Computer no avaiable port";
        }
    }
    else
    {
        emit closeSerialPort_DTU();
//        m_SerialWorker_DTU->close(); //不能跨线程操作
        ui->pushButton_openPortDTU->setText(tr("连接网络"));
    }
}


void Contriols::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        on_pushButton_cmdForward_clicked();
    }
    else if(event->key() == Qt::Key_A)
    {
        on_pushButton_CmdLeft_clicked();
    }
    else if(event->key() == Qt::Key_S)
    {
        on_pushButton_CmdBack_clicked();
    }
    else if(event->key() == Qt::Key_D)
    {
        on_pushButton_CmdRight_clicked();
    }
    else if(event->key() == Qt::Key_Q)
    {
        on_pushButton_CmdLeftThruster_clicked();
    }
    else if(event->key() == Qt::Key_E)
    {
        on_pushButton_CmdRightThruster_clicked();
    }
}

void Contriols::on_pushButton_openPortRudderBell_clicked()
{
//    CInterfaceForIMDModule* m_pInterfaceForIMDModule;		//通过IMD模块采集车钟、侧推、舵轮等信息接口对象
    if(ui->pushButton_openPortRudderBell->text() == tr("连接驾驶台"))
    {

        if (ui->comboBox_port->count() > 0)///当前列表的内容个数
        {///打开串口操作
            int SelBaudRate=115200;
            m_SerialWorker_RudderBell = new SerialWorkerP(ui->comboBox_port->currentText().toStdString().c_str(),
                                                  SelBaudRate,
                                                  QSerialPort::NoParity,
                                                  QSerialPort::Data8,
                                                  QSerialPort::OneStop,
                                                  QSerialPort::NoFlowControl,
                                                  500);
            qRegisterMetaType<Bridge_ZL::Values_Bridge>("Bridge_ZL::Values_Bridge");
            connect(m_SerialWorker_RudderBell, SIGNAL(sendBridgeDataToGui(Bridge_ZL::Values_Bridge)), this, SLOT(On_receive_RudderBell(Bridge_ZL::Values_Bridge)), Qt::QueuedConnection);
            connect(this, SIGNAL(closeSerialPort_RudderBell()), m_SerialWorker_RudderBell, SLOT(close()), Qt::QueuedConnection);
            m_SerialWorker_RudderBell->start();
            if(m_SerialWorker_RudderBell->is_open())
            {
                ui->pushButton_openPortRudderBell->setText(tr("关闭驾驶台"));        }
            else
            {
            QMessageBox::information(NULL,tr("information"),tr("open port error") + QString("\n\ncode: %1\nmessage: %2").arg(m_SerialWorker_RudderBell->getLastError()).arg(m_SerialWorker_RudderBell->getLastErrorMsg()));
                ui->pushButton_openPortRudderBell->setText(tr("连接驾驶台"));
                qDebug()<< m_SerialWorker_RudderBell->getLastError();
            }
        }
        else
        {
            QMessageBox::information(NULL,tr("information"),tr("This Computer no avaiable port"));
            qDebug()<< "This Computer no avaiable port";
        }
    }
    else
    {
        emit closeSerialPort_RudderBell();
        ui->pushButton_openPortRudderBell->setText(tr("连接驾驶台"));
    }

}

#ifdef TESTING_MODE

void Contriols::on_pushButton_openPortArduino_clicked()
{
    if(ui->pushButton_openPortArduino->text() == tr("打开Arduino"))
    {

        if (ui->comboBox_port->count() > 0)///当前列表的内容个数
        {///打开串口操作
            int SelBaudRate=38400;
            m_cSerialArduino = new SerialWorker(ui->comboBox_port->currentText().toStdString().c_str(),
                                                  SelBaudRate,
                                                  QSerialPort::NoParity,
                                                  QSerialPort::Data8,
                                                  QSerialPort::OneStop,
                                                  QSerialPort::NoFlowControl,
                                                  500);
            connect(this, SIGNAL(m_signalSendCmdToArduino(QByteArray)), m_cSerialArduino, SLOT(doDataSendWork(const QByteArray)), Qt::QueuedConnection);
            connect(this, SIGNAL(closeSerialPort_Arduino()), m_cSerialArduino, SLOT(close()), Qt::QueuedConnection);
            m_cSerialArduino->start();
            if(m_cSerialArduino->is_open())
            {
                ui->pushButton_openPortArduino->setText(tr("关闭Arduino"));        }
            else
            {
            QMessageBox::information(NULL,tr("information"),tr("open port error") + QString("\n\ncode: %1\nmessage: %2").arg(m_cSerialArduino->getLastError()).arg(m_cSerialArduino->getLastErrorMsg()));
                ui->pushButton_openPortArduino->setText(tr("打开Arduino"));
                qDebug()<< m_cSerialArduino->getLastError();
            }
        }
        else
        {
            QMessageBox::information(NULL,tr("information"),tr("This Computer no avaiable port"));
            qDebug()<< "This Computer no avaiable port";
        }
    }
    else
    {
        emit closeSerialPort_Arduino();
        ui->pushButton_openPortArduino->setText(tr("打开Arduino"));
    }
}

#endif


void Contriols::On_receive_pushButton_switchControl(bool rl_flag)
{
    m_bIsControl = rl_flag;
}

