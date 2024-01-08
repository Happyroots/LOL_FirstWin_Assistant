#include "serialworker.h"
#include <QDebug>
#include <QThread>


SerialWorker::SerialWorker(QObject *parent)
    : QObject(parent)
{
    m_thread = new QThread();

    m_serialPort = new QSerialPort();
//    init_port();
    this->moveToThread(m_thread);
    m_serialPort->moveToThread(m_thread);
    m_thread->start();  //启动线程
}

SerialWorker::SerialWorker(
                           const QString portName,
                           int baudRate ,
                           QSerialPort::Parity parity ,
                           QSerialPort::DataBits dataBits ,
                           QSerialPort::StopBits stopbits,
                           QSerialPort::FlowControl flowControl,
                            int ReadIntervalTimeout,
                           QObject *parent
                           )
    : QObject(parent)
{

    m_serialPort = new QSerialPort();
    init(portName,
         baudRate ,
         parity ,
         dataBits ,
         stopbits ,
         flowControl,
         ReadIntervalTimeout
         );
    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_serialPort->moveToThread(m_thread);

}

SerialWorker::~SerialWorker()
{
    delete m_serialPort; m_serialPort = nullptr;
    delete m_thread; m_thread = nullptr;

}

//void SerialWorker::init_port()
//{
//        m_serialPort->setPortName("/dev/ttyS1");                   //串口名 windows下写作COM1
//        m_serialPort->setBaudRate(38400);                           //波特率
//        m_serialPort->setDataBits(QSerialPort::Data8);             //数据位
//        m_serialPort->setStopBits(QSerialPort::OneStop);           //停止位
//        m_serialPort->setParity(QSerialPort::NoParity);            //奇偶校验
//        m_serialPort->setFlowControl(QSerialPort::NoFlowControl);  //流控制
//        if (m_serialPort->open(QIODevice::ReadWrite))
//            {
//                qDebug() << "Port have been opened";
//            }
//        else
//            {
//                qDebug() << "open it failed";
//            }
//        connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(doDataReciveWork()), Qt::QueuedConnection); //Qt::DirectConnection
//}

void SerialWorker::start()
{

    m_thread->start();  //启动线程
}

void SerialWorker::init(const QString portName,
     int baudRate /*= itas109::BaudRate::BaudRate9600*/,
     QSerialPort::Parity parity /*= itas109::Parity::ParityNone*/,
     QSerialPort::DataBits dataBits /*= itas109::DataBits::DataBits8*/,
     QSerialPort::StopBits stopbits /*= itas109::StopBits::StopOne*/,
     QSerialPort::FlowControl flowControl /*= itas109::FlowControl::FlowNone*/,
     int ReadIntervalTimeout

//     unsigned int readBufferSize /*= 4096*/
                        )
{
    m_serialPort->setPortName(QString(portName)); //串口名 windows下写作COM1
    m_serialPort->setBaudRate(baudRate);          //波特率
    m_serialPort->setDataBits(dataBits);          //数据位
    m_serialPort->setStopBits(stopbits);          //停止位
    m_serialPort->setParity(parity);              //奇偶校验
    m_serialPort->setFlowControl(flowControl);    //流控制
    m_serialPort->setReadBufferSize(300);
//    m_serialPort->setDataTerminalReady(true);
    m_iTimeout = ReadIntervalTimeout;
//    m_serialPort->setReadIntervalTimeout(timeout);
    if (m_serialPort->open(QIODevice::ReadWrite))
    {
        m_bIsOpen = true;
        qDebug() << "Port have been opened";
    }
    else
    {
        m_bIsOpen = false;
        qDebug() << "open it failed";
    }
    connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(doDataReciveWork()), Qt::QueuedConnection); //Qt::DirectConnection

}


//只读
bool SerialWorker::is_open()
{
    return m_bIsOpen;
}

void SerialWorker::close()
{
    m_serialPort->close();
    m_bIsOpen = false;
}

int SerialWorker::getLastError() const
{
    return QSerialPort::OpenError;
}

const char *SerialWorker::getLastErrorMsg() const
{
    return "";
}

void SerialWorker::doDataSendWork(const QByteArray data)
{
    if(m_serialPort->isWritable()){
        //qDebug() <<  "子线程槽函数发送数据：" << data << "线程ID：" << QThread::currentThreadId();
        // 发送数据
        m_serialPort->write(data);
        m_serialPort->flush();
        m_serialPort->waitForBytesWritten(100);
    }else{
        qDebug() << "Couldn't write to serial!";
    }

}

void SerialWorker::doDataReciveWork()
{

        // 1.收到数据
        m_thread->msleep(10);
        QByteArray buffer = m_serialPort->readAll();
        qDebug() <<  "Tread received" << buffer << "ThreadID:" << QThread::currentThreadId();

        // 2.进行数据处理

        // 3.将结果发送到主线程
        emit sendResultToGui(buffer);
}
