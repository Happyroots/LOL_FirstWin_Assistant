#ifndef SERIALWORKERP_H
#define SERIALWORKERP_H

#include <serialworker.h>
#include <QTimer>
#include "drivers/bridge_zl.h"


class SerialWorkerP : public SerialWorker
{
    Q_OBJECT
public:

    explicit SerialWorkerP(
                           const QString portName,
                           int baudRate ,
                           QSerialPort::Parity parity ,
                           QSerialPort::DataBits dataBits ,
                           QSerialPort::StopBits stopbits,
                           QSerialPort::FlowControl flowControl ,
                           int ReadIntervalTimeout,
                           QObject *parent = nullptr
                           );
    QTimer *m_updateTimer;
    Bridge_ZL *m_cBridge_ZL;

public slots:
    void requestData();
    void start();
    void start_timer();
    void requestData(int iAddrres);
    void doDataReciveWork();

signals:
//    void sendResultToGui(Bridge_ZL::Values_Bridge);
    void sendBridgeDataToGui(Bridge_ZL::Values_Bridge);


};

#endif // SERIALWORKERP_H
