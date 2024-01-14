#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QThread>

class UDPSocket : public QObject
{
    Q_OBJECT
public:
    explicit UDPSocket(QObject *parent = nullptr);

    UDPSocket(QString ip, quint16 port, QObject *parent = nullptr);
    ~UDPSocket();

    //绑定本机的ip和端口号信息
    bool bind(QString ip, ushort port);

    //设置目标主机的ip和端口号
    void setTargetInfo(QString ip, quint16 port);

signals:
    //通过该信号传递接收到的数据
    void recvDataSignal(QString data);

public slots:
    //读取数据的槽函数
    void on_readyReadData();
    //通过该函数发送数据
    void sendData(QString data);
    void start();

private:
    QUdpSocket      *m_sock;        //UDP套接字指针
    QHostAddress    m_hostAddr;     //保存目标的地址对象
    quint16         m_port;         //保存目标的端口号(类型一致)

protected:
    QThread *m_thread;

};

#endif // UDPSOCKET_H
