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


Contriols::Contriols(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Contriols)
{
    ui->setupUi(this);
    QRegularExpression rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    //QRegularExpressionValidator* ipValidator = new QRegularExpressionValidator(rx, this);
    //ui->lineEdit_ip->setValidator(ipValidator);

    db_table_model_ = new QStandardItemModel();
    ui->tableView_data->setModel(db_table_model_);
    ui->tableView_data->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QStringList table_h_headers;
    table_h_headers << "GPSIMU信息回传" ;
    db_table_model_->setHorizontalHeaderLabels(table_h_headers);

    QStringList table_v_headers;

    table_v_headers << "经纬度"  << "航向" ;
    db_table_model_->setVerticalHeaderLabels(table_v_headers);

    // 设置第一行第一列的数据为 "100"（数据位置从0开始计数）
    QStandardItem *item = new QStandardItem("34.3323124, 121.25421311");
    db_table_model_->setItem(0, 0, item);
//    delete  item;
    // 设置第二行第二列的数据为 "200"
    item = new QStandardItem("200");
    db_table_model_->setItem(1, 0, item);
//    delete  item;
    // 设置第三行第三列的数据为 "300"
   // item = new QStandardItem("300");
   // db_table_model_->setItem(2, 0, item);
//    delete  item;

    QList<QSerialPortInfo> portNameList = QSerialPortInfo::availablePorts();
    for(int i = 0; i < portNameList.size(); i++){
        ui->comboBox_portDTU->insertItem(i, portNameList[i].portName());
    }

    // 初始化定时器
    updateTimer = new QTimer(this);
    //connect(updateTimer, &QTimer::timeout, this, &Contriols::onUpdateTimerTimeout);
    updateTimer->start(2000); // 每100毫秒触发一次定时器


    // 创建并打开日志文件，如果文件已经存在，会被清空
    QFile logFile(logFileName);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&logFile);
        stream << "Time,SXNumber,LNumber,ANumber\n";
    }

    connect_clicked();

}

Contriols::~Contriols()
{
    delete ui;
    if(!m_SerialWorker_DTU) {
        delete m_SerialWorker_DTU; m_SerialWorker_DTU = nullptr;
    }
}

void Contriols::on_receive(QByteArray tmpdata)
{
    parseData(tmpdata);
    //drawTrackPoint(lNumber, aNumber);
}



void Contriols::sendCmdToShip(){
    if(m_SerialWorker_DTU){
        QString formattedString_cmd = QString("S%1,%2E").arg(m_iCmdRudder).arg(m_iCmdPropeller);
        QByteArray byteArray_cmd = formattedString_cmd.toUtf8();
        ui->lineEdit_rudder_2->setText(QString::number(m_iCmdRudder - m_iBias_cmd_rudder));
        ui->lineEdit_propeller_2->setText(QString::number(m_iCmdPropeller - m_iBias_cmd_prop));


        emit m_signalSendCmdToShip(byteArray_cmd);
    }
    m_Client->write(QString("test").toUtf8().data());

}

void Contriols::parseData(QByteArray newData)
{
//    /qDebug() << "before process" << newData;
//    int startIdx = newData.indexOf("X"); // 查找起始位置
//    if (startIdx != -1) {
//        newData.remove(0, startIdx-1); // 删除已处理的数据包，包括'E\n'
//        m_QSreceivedData += newData; // 追加新接收到的数据
//    }
    int endIdx = newData.indexOf("E"); // 查找结束位置
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
    int startIdx = newData.indexOf("X"); // 查找起始位置
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



        // 设置第一行第一列的数据为 "100"（数据位置从0开始计数）
        QStandardItem *item = new QStandardItem(match.captured(3)+", "+match.captured(2));
        db_table_model_->setItem(0, 0, item);
        // 设置第二行第二列的数据为 "200"
        item = new QStandardItem(match.captured(1));
        db_table_model_->setItem(1, 0, item);

//        qDebug() << "SX: " << sxNumber << " L: " << lNumber << " A: " << aNumber;
        // 将输出语句修改为保存sxNumber、lNumber和aNumber的值

        // 在文件中写入时间和数值
        QFile logFile(logFileName);
        if (logFile.open(QIODevice::Append | QIODevice::Text))
        {
            QTextStream stream(&logFile);
//            stream << currentTime << "," << sxNumber << "," << lNumber << "," << aNumber << "\n";
            stream << currentTime << "," << QString::number(sxNumber, 'f', 6) << ","
                   << QString::number(lNumber, 'f', 6) << "," << QString::number(aNumber, 'f', 6) << "\n";

//            std::ostringstream ss;
//            ss << std::fixed << std::setprecision(3) << sxNumber;
//            QString sxNumberStr = QString::fromStdString(ss.str());
//            ss.str(std::string());
//            ss.clear();
//            ss << std::fixed << std::setprecision(6) << lNumber;
//            QString lNumberStr = QString::fromStdString(ss.str());
//            ss.str(std::string());
//            ss.clear();
//            ss << std::fixed << std::setprecision(6) << aNumber;
//            QString aNumberStr = QString::fromStdString(ss.str());
//            stream << currentTime << "," << sxNumberStr << "," << lNumberStr << "," << aNumberStr << "\n";
        }

        // 调用 drawTrackPoint 添加新的轨迹点
        //drawTrackPoint(lNumber, aNumber);

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





void Contriols::on_pushButton_openPortDTU_clicked()
{
    if(ui->pushButton_openPortDTU->text() == tr("打开DTU串口"))
    {

        if (ui->comboBox_portDTU->count() > 0)///当前列表的内容个数
        {///打开串口操作
            int SelBaudRate=9600;
            m_SerialWorker_DTU = new SerialWorker(ui->comboBox_portDTU->currentText().toStdString().c_str(),
                                                  SelBaudRate,
                                                  QSerialPort::NoParity,
                                                  QSerialPort::Data8,
                                                  QSerialPort::OneStop,
                                                  QSerialPort::NoFlowControl,
                                                  500);

            connect(m_SerialWorker_DTU, SIGNAL(sendResultToGui(QByteArray)), this, SLOT(on_receive(QByteArray)), Qt::QueuedConnection);
            connect(this, SIGNAL(m_signalSendCmdToShip(QByteArray)), m_SerialWorker_DTU, SLOT(doDataSendWork(const QByteArray)), Qt::QueuedConnection);
            connect(this, SIGNAL(closeSerialPort_DTU()), m_SerialWorker_DTU, SLOT(close()), Qt::QueuedConnection);
            if(m_SerialWorker_DTU->is_open())
            {
                ui->pushButton_openPortDTU->setText(tr("关闭DTU串口"));        }
            else
            {
            QMessageBox::information(NULL,tr("information"),tr("open port error") + QString("\n\ncode: %1\nmessage: %2").arg(m_SerialWorker_DTU->getLastError()).arg(m_SerialWorker_DTU->getLastErrorMsg()));
                ui->pushButton_openPortDTU->setText(tr("打开DTU串口"));
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
        ui->pushButton_openPortDTU->setText(tr("打开DTU串口"));
    }
}



// 重写绘图事件
void Contriols::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

//    // 创建绘图对象
//    QPainter painter(this);

//    // 绘制轨迹点
//    painter.setPen(Qt::blue); // 设置轨迹点颜色
//    painter.setBrush(Qt::blue);

//    for (const QPointF &point : trackPoints)
//    {
//        // 在地图上绘制每个轨迹点
//        painter.drawEllipse(point, 5, 5);
//        // 在每个轨迹点的旁边打印坐标值
//        painter.drawText(point + QPointF(10, -10), QString("(%1, %2)").arg(point.x()).arg(point.y()));
//    }


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
}
