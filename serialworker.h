#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QSerialPort>

class SerialWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialWorker(QObject *parent = nullptr);
    explicit SerialWorker(
                           const QString portName,
                           int baudRate ,
                           QSerialPort::Parity parity ,
                           QSerialPort::DataBits dataBits ,
                           QSerialPort::StopBits stopbits,
                           QSerialPort::FlowControl flowControl ,
                           int ReadIntervalTimeout,
                           QObject *parent = nullptr
//                           unsigned int readBufferSize
                           );
    ~SerialWorker();
//    void init_port();  //初始化串口
    void start();
    bool setSerialPortParams(QSerialPort &serial, qint32 baudRate);
    void init(const QString portName,
              int baudRate /*= itas109::BaudRate::BaudRate9600*/,
              QSerialPort::Parity parity /*= itas109::Parity::ParityNone*/,
              QSerialPort::DataBits dataBits /*= itas109::DataBits::DataBits8*/,
              QSerialPort::StopBits stopbits /*= itas109::StopBits::StopOne*/,
              QSerialPort::FlowControl flowControl /*= itas109::FlowControl::FlowNone*/,
              int ReadIntervalTimeout = 50
//              unsigned int readBufferSize /*= 4096*/
              );
    bool is_open();
    int getLastError() const;
    const char *getLastErrorMsg() const;

private:
    bool m_bIsOpen = false;
    int m_iTimeout = 0;

signals:
    virtual void sendResultToGui(QByteArray result);

    //接收数据
    virtual void receive_data(QByteArray tmp);



protected:
    QThread *m_thread;
    QSerialPort *m_serialPort;

public slots:
    virtual void doDataSendWork(const QByteArray data);
    virtual void doDataReciveWork();
    void close();
};

#endif // SERIALWORKER_H
