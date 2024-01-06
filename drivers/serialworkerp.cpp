#include "serialworkerp.h"



SerialWorkerP::SerialWorkerP(const QString portName, int baudRate, QSerialPort::Parity parity, QSerialPort::DataBits dataBits, QSerialPort::StopBits stopbits, QSerialPort::FlowControl flowControl, int ReadIntervalTimeout, QObject *parent)
    : SerialWorker(portName,
                   baudRate ,
                   parity ,
                   dataBits ,
                   stopbits ,
                   flowControl,
                   ReadIntervalTimeout)
{
//    disconnect(m_serialPort, SIGNAL(readyRead()), this, SLOT(doDataReciveWork()));
}

#include <QThread>
void SerialWorkerP::start()
{
//    串口类必须在 moveToThread 之前初始化，否则程序会 crash（崩溃
//    new时不要传入”this“,否则会传入警告
//    m_thread = new QThread();
//    this->moveToThread(m_thread);
//    m_serialPort->moveToThread(m_thread);

    m_cBridge_ZL = new Bridge_ZL();
    m_cBridge_ZL->moveToThread(m_thread);

    m_updateTimer = new QTimer();
    m_updateTimer->moveToThread(m_thread);

    m_updateTimer->setInterval(1000);//间隔，
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(requestData()));
    connect(m_thread, SIGNAL(started()), this, SLOT(start_timer()));

    m_thread->start();

}

void SerialWorkerP::start_timer()
{
    m_updateTimer->start();//启动定时器
}

#include <QDebug>
void SerialWorkerP::requestData(int iAddress)
{
    QByteArray byteArray = m_cBridge_ZL->generateQueryMessage(iAddress);
    if(is_open())
        doDataSendWork( byteArray );

//    m_thread->msleep(200); //数据容易没，效果也不好
//    m_serialPort->flush();
//    //半双工
//    if(m_serialPort->bytesAvailable() <= 0) {
//           return;
//     }
//    QByteArray requestedData;
//    if(is_open()){
//            requestedData =  m_serialPort->readAll();
//            qDebug() << requestedData; //一次读了好几条数据
//    }
//    if (requestedData.isEmpty()) return;
//    m_cBridge_ZL->parseData(requestedData);
}

#include <QDebug>
void SerialWorkerP::requestData()
{
    requestData(0x05);
    m_thread->msleep(200);
//    requestData(0x06);
//    m_thread->msleep(200);
    requestData(0x07);
    m_thread->msleep(200);


//    改进：降低延迟，全收到
//        requestData(0x05);
//        m_thread->msleep(500);
//        requestData(0x06);
//        m_thread->msleep(500);
//        requestData(0x07);
//        m_thread->msleep(500);

    //Todo:循环只能2次，看看什么问题？
//    for(int iAddress = m_cBridge_ZL->ADDRESS_BellLeft; iAddress <= m_cBridge_ZL->ADDRESS_Rudder; iAddress ++){
//        QByteArray byteArray = m_cBridge_ZL->generateQueryMessage(iAddress);
//        if(is_open())
//            doDataSendWork( byteArray );
//        //等待一会，看看能收全不
//        m_thread->usleep(1000);
//        //半双工
//        if(m_serialPort->bytesAvailable() <= 0) {
//               return;
//         }
//        QByteArray requestedData;
//        if(is_open()){
//                requestedData =  m_serialPort->readAll();
//        }
//        if (requestedData.isEmpty()) return;
//        m_cBridge_ZL->parseData(requestedData);
//    }

    emit sendBridgeDataToGui(m_cBridge_ZL->m_sValuesBridge);
}


void SerialWorkerP::doDataReciveWork()
{
        // 1.收到数据
//    QByteArray buffer = m_serialPort->readAll();
//    m_thread->usleep(100); //数据容易没，效果也不好
    QByteArray buffer;
    buffer = m_serialPort->read(7); // 读取 7 个字节的数据
    qDebug() <<  "Tread received" << buffer << "ThreadID:" << QThread::currentThreadId();
    // 2.进行数据处理
    //1.等待一会，看看能收全不,
    //2.可能是线松了，没信号
    //3.先上电，等几分钟？再插串口，还是反着？
    //4.是不是设备性能不好？
    m_serialPort->flush();
    //半双工
//        if(m_serialPort->bytesAvailable() <= 0) {
//               return;
//         }
    QByteArray requestedData = buffer;

    if (requestedData.isEmpty()) return;
    m_cBridge_ZL->parseData(requestedData);

    // 3.将结果发送到主线程
//        emit sendResultToGui(buffer);
}

