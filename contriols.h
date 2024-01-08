#ifndef CONTRIOLS_H
#define CONTRIOLS_H

#include <QWidget>
#include <QTcpSocket>
//#include "dtu_arduino_driver.h"
#include <chrono>
#include <thread>
#include <QStandardItemModel>
#include <QImage>  // 添加这一行
#include <QLabel>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QKeyEvent>
#include "utils/gps.h"
#include "utils/timeduration.h"
#include "drivers/serialworkerp.h"
#include "drivers/bridge_zl.h"
#include "drivers/serialworker.h"

namespace Ui {
class Contriols;
}

class Contriols : public QWidget
{
    Q_OBJECT

public:
    explicit Contriols(QWidget *parent = nullptr);
    ~Contriols();

    QTcpSocket *m_Client = nullptr;

    int m_iBias_cmd_rudder = 35;
    int m_iBias_cmd_prop = 100;
    int m_iCmdRudder = m_iBias_cmd_rudder;
    int m_iCmdPropeller = m_iBias_cmd_prop;
    int m_iStepPropellar = 5;
    int m_iStepRudder = 5;

    SerialWorker *m_SerialWorker_DTU;
    QByteArray m_QSreceivedData;
    SerialWorkerP *m_SerialWorker_RudderBell;
    Bridge_ZL::Values_Bridge m_sValues_Bridge;

signals:
    void m_signalSendCmdToShip(const QByteArray data);
    void closeSerialPort_DTU();
    void update_DashboardCourseSpeed(double value);
    void closeSerialPort_RudderBell();
#ifdef TESTING_MODE
    void m_signalSendCmdToArduino(const QByteArray data);
    void closeSerialPort_Arduino();
#endif

private slots:
//   / void on_pushButton_connect_clicked();

    void on_pushButton_cmdForward_clicked();

    void on_pushButton_CmdBack_clicked();

    void on_pushButton_CmdLeft_clicked();

    void on_pushButton_CmdRight_clicked();

    void on_pushButton_openPortDTU_clicked();

    void On_receive_DTU(QByteArray tmpdata);
    void On_receive_RudderBell(Bridge_ZL::Values_Bridge data_RudderBell);
    void sendCmdToShip();
    void parseData(QByteArray newData);
    void processData(const QByteArray& qbytearray);
    void connect_clicked();
    qreal caculate_velocity_abs(qreal , qreal);
    void on_pushButton_openPortRudderBell_clicked();
#ifdef TESTING_MODE
    void on_pushButton_openPortArduino_clicked();
#endif

private:
    Ui::Contriols *ui;
    // 添加一个用于存储轨迹点的容器
    QVector<QPointF> trackPoints;
    QTimer *updateTimer;

public:
    qreal sxNumber;
    qreal lNumber;
    qreal aNumber;

    qreal sxNumber_last = 0.0;
    qreal longtitude_last = 0.0;
    qreal latitude_last = 0.0;

    qreal m_s2kn = 1.9438445;
    qreal cmd2velocity = 0.0255;
    QFile *logFile;
    TimeDuration timeDuration;
    void keyPressEvent(QKeyEvent *event) override;
#ifdef TESTING_MODE
    SerialWorker *m_cSerialArduino;
#endif
};

#endif // CONTRIOLS_H
