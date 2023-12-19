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
    connect(updateTimer, &QTimer::timeout, this, &Contriols::onUpdateTimerTimeout);
    updateTimer->start(2000); // 每100毫秒触发一次定时器

    // 在 Contriols 类的构造函数中初始化定时器
    //simulatedDataTimer = new QTimer(this);
    //connect(simulatedDataTimer, &QTimer::timeout, this, &Contriols::simulateData);
    //simulatedDataTimer->start(2000); // 每秒模拟一次数据更新

    // 初始化 mapLabel
    mapLabel = new QLabel(this);
    mapLabel->setGeometry(0, 150, 1200, 800); // 你需要根据实际情况调整这里的参数
    // 设置初始地图图片
    QPixmap initialMap(":/new/Images/HaiwanBridge.jpg"); // 替换为你的图片路径
    // 获取图片的大小
    QSize imageSize = initialMap.size();

    // 设置 QLabel 的大小为图片的大小
    mapLabel->setFixedSize(imageSize);

    mapLabel->setScaledContents(true); // 允许图片缩放
    mapLabel->setPixmap(initialMap);
    mapLabel->show();

    // 创建并打开日志文件，如果文件已经存在，会被清空
    QFile logFile(logFileName);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&logFile);
        stream << "Time,SXNumber,LNumber,ANumber\n";
    }

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

// 添加一个槽函数，用于模拟位置数据的变化
void Contriols::simulateData()
{
//    qreal lNumber0 = 121.0+31.261/60.0;//零点的GPS坐标
//    qreal aNumber0 = 38.0+52.190/60.0;

//    static qreal lNumber = 121.0+31.261/60.0;//零点的GPS坐标
//    static qreal aNumber = 38.0+52.190/60.0;

//    double Lon_1 = 121.0+32.096/60.0; //参照点的GPS坐标
//    double Lat_1 = 38.0+51.945/60.0 ;

//    // 模拟位置数据的变化
//    lNumber += 0.0005;
//    aNumber -= 0.0005;

//   qreal lNumber1 = (lNumber-lNumber0)*1293.0/(Lon_1-lNumber0);//实时GPS坐标转为像素的坐标
//   qreal aNumber1 = (aNumber-aNumber0)*493.0/(Lat_1-aNumber0);



//    // 调用 drawTrackPoint 添加模拟数据的轨迹点
//    drawTrackPoint(lNumber1, aNumber1);
}

void Contriols::onUpdateTimerTimeout()
{
    // 根据最新的轨迹点绘制轨迹
    updateMapWithTrack();
}

void Contriols::updateMapWithTrack()
{
    // 获取当前地图图片
    QPixmap currentMap = mapLabel->pixmap()->copy();

    // 创建 QPainter 用于在图片上绘制
    QPainter painter(&currentMap);
    painter.setPen(Qt::red); // 设置轨迹线的颜色

    // 绘制轨迹线
    for (int i = 1; i < trackPoints.size(); ++i) {
        painter.drawLine(trackPoints[i - 1], trackPoints[i]);
    }

    // 在地图上绘制轨迹点
    painter.setPen(Qt::red); // 设置轨迹点颜色
    painter.setBrush(Qt::blue);
    for (const QPointF &point : trackPoints) {
        painter.drawEllipse(point, 2, 2);
        // 在每个轨迹点的旁边打印坐标值
        //painter.drawText(point + QPointF(10, -10), QString("(%1, %2)").arg(point.x()).arg(point.y()));
    }

    // 更新地图图片
    mapLabel->setPixmap(currentMap);


}

// 新添加的成员函数，用于绘制轨迹点
void Contriols::drawTrackPoint(qreal lNumber, qreal aNumber)
{
    // 获取当前时间
    //QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    // 添加轨迹点到容器
    qreal lNumber0 = 121.0+31.226/60.0;//零点的GPS坐标
    qreal aNumber0 = 38.0+52.208/60.0;
    //qreal lNumber0 = 121.312;//零点的GPS坐标
    //qreal aNumber0 = 38.522;
    qreal l1=lNumber-(int)lNumber;//小数点后的
    qreal a1=aNumber-(int)aNumber;
    //qreal l2=l1*100-(int)l1*100;//小数点后三位后的
    lNumber = int(lNumber)+(l1*100)/60.0;
    aNumber = int(aNumber)+(a1*100)/60.0;
     //lNumber = 121.0+31.261/60.0;//零点的GPS坐标
     //aNumber = 38.0+52.190/60.0;

   // lNumber = lNumber+31.261/60.0;//零点的GPS坐标
   // aNumber = aNumber+52.190/60.0;

    double Lon_1 = 121.0+32.096/60.0; //参照点的GPS坐标
    double Lat_1 = 38.0+51.9450/60.0 ;
    qDebug() << "Original Track Point: (" << lNumber << ", " << aNumber<<")";


     qreal lNumber1 = (lNumber-lNumber0)*738.0/(Lon_1-lNumber0);//实时GPS坐标转为像素的坐标
     qreal aNumber1 = (aNumber-aNumber0)*288.0/(Lat_1-aNumber0);

    trackPoints.append(QPointF(lNumber1, aNumber1));
    //trackPoints.append(QPoint(240, 196));

    // 在控制台输出轨迹点的数值
    qDebug() << "Track Point: (" << lNumber1 << ", " << aNumber1  <<")";

//    // 在文件中写入时间和数值
//    QFile logFile(logFileName);
//    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
//        QTextStream stream(&logFile);
//        stream << currentTime << "," << sxNumber << "," << lNumber << "," << aNumber << "\n";
//    }

    // 重绘窗口
    update();
}

void Contriols::sendCmdToShip(){
    if(m_SerialWorker_DTU){
        QString formattedString_cmd = QString("S%1,%2E").arg(m_iCmdRudder).arg(m_iCmdPropeller);
        QByteArray byteArray_cmd = formattedString_cmd.toUtf8();
        ui->lineEdit_rudder->setText(QString::number(m_iCmdRudder - m_iBias_cmd_rudder));
        ui->lineEdit_propeller->setText(QString::number(m_iCmdPropeller - m_iBias_cmd_prop));


        emit m_signalSendCmdToShip(byteArray_cmd);
    }

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
    QString str = QString::fromUtf8(qbytearray);
    // 获取当前时间
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QRegularExpression regex("SX(-*[0-9.]+)L(-*[0-9.]+)A(-*[0-9.]+)E");
    QRegularExpressionMatch match = regex.match(str);

    if (match.hasMatch()) {
        qreal sxNumber = match.captured(1).toDouble();
        qreal lNumber = match.captured(2).toDouble();
        qreal aNumber = match.captured(3).toDouble();



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
        if (logFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream stream(&logFile);
            stream << currentTime << "," << sxNumber << "," << lNumber << "," << aNumber << "\n";
        }

        // 调用 drawTrackPoint 添加新的轨迹点
        drawTrackPoint(lNumber, aNumber);

    }
}

/*void Contriols::on_pushButton_connect_clicked()
{
    if(ui->pushButton_connect->text() == "连接" && !m_Client) {
        //获取服务器IP和端口
        QString ip = ui->lineEdit_ip->text();
        qint16 port = ui->spinBoxPortNetwork->text().toInt();
        //主动与服务器建立连接
        m_Client = new QTcpSocket(this);
        m_Client->connectToHost(QHostAddress(ip),port);
        connect(m_Client,&QTcpSocket::connected, this,
                [=]()
                {
                    ui->pushButton_connect->setText("断连");
                }
                );
        //获取编辑框内容
        QString str = ui->pushButton_connect->text();
        //发送数据
        m_Client->write(str.toUtf8().data());
    }
    else if(ui->pushButton_connect->text() == "断连") {
        m_Client->close();
        delete m_Client; m_Client = nullptr;
        ui->pushButton_connect->setText("连接");
    }

}
*/

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
        qDebug() << "按下了W键";
    }
    else if(event->key() == Qt::Key_A)
    {
        qDebug() << "按下了A键";
    }
    else if(event->key() == Qt::Key_S)
    {
        qDebug() << "按下了S键";
    }
    else if(event->key() == Qt::Key_D)
    {
        qDebug() << "按下了D键";
    }
}
