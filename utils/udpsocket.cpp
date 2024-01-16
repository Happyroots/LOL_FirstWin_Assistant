#include "udpsocket.h"

UDPSocket::UDPSocket(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_sock = new QUdpSocket;
    // 连接数据读取信号槽
    connect(m_sock, &QUdpSocket::readyRead, this, &UDPSocket::on_readyReadData);

    m_sock->moveToThread(m_thread);
    m_thread->start();  //启动线程

}

UDPSocket::UDPSocket(QHostAddress ip, quint16 port, QObject *parent): QObject(parent),
    // 存储传入的IP和端口号
    m_hostAddr( QHostAddress(ip)),
    m_port(port)
{
    m_thread = new QThread();
    this->moveToThread(m_thread);
    m_sock = new QUdpSocket;
    connect(m_sock, &QUdpSocket::readyRead, this, &UDPSocket::on_readyReadData);
    if (!m_sock->bind(QHostAddress(ip), port)){
        qDebug() << "端口" << port << "不可用";
    }
    m_sock->moveToThread(m_thread);
    m_thread->start();  //启动线程
}

UDPSocket::~UDPSocket()
{

}


void UDPSocket::sendData(QString data)
{
    // 发送传入的数据到指定的信息的位置
    m_sock->writeDatagram(data.toUtf8(), m_hostAddr, m_port); //稳定
//    m_sock->write(data.toUtf8());
}

void UDPSocket::start()
{
    m_thread->start();  //启动线程
}

void UDPSocket::on_readyReadData()
{
    // 通过函数判断当前是否有等待读取的数据并循环获取
    while(m_sock->hasPendingDatagrams())
    {
        //创建数据存储容器，并设置长度为将接收的数据长度
        QByteArray data;
        data.resize(m_sock->pendingDatagramSize());
        //读取数据并保存信息发送者的地址和ip(方便发送时指定发送位置)
//        m_sock->readDatagram(data.data(), data.size(), &m_hostAddr, &m_port);8
//        QHostAddress sender(QHostAddress::AnyIPv4);
        m_sock->readDatagram(data.data(), data.size(), &m_hostAddr, &m_port);
//        qDebug() << data;
        //发送接收数据的信号
        emit recvDataSignal(data);
    }
}
