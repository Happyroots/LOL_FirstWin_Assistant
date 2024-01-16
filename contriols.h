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
#include "algorithms/ZLControl.h"
#include "utils/udpsocket.h"

#ifdef REPLAY_MODE
#include  "utils/dumpdata.h"
#endif

namespace Ui {
class Contriols;
}

class Contriols : public QWidget
{
    Q_OBJECT

public:
    explicit Contriols(QWidget *parent = nullptr);
    ~Contriols();


    int m_iBias_cmd_rudder = 35;
    int m_iBias_cmd_prop = 100;
    int m_iCmdRudder = m_iBias_cmd_rudder;
    int m_iCmdPropeller = m_iBias_cmd_prop;
    int m_iCmdSideThuster = m_iBias_cmd_prop;
    int m_iStepPropellar = 20;
    int m_iStepRudder = 5;
    int m_iStepSideThruster = 50;

    SerialWorker *m_SerialWorker_DTU = nullptr;
    QByteArray m_QSreceivedData;
    SerialWorkerP *m_SerialWorker_RudderBell = nullptr;
    Bridge_ZL m_cBridge_ZL;
    Bridge_ZL::Values_Bridge m_sValues_Bridge;
    ZLControl m_cAlgorithm_control;
    QTcpSocket *m_Client = nullptr;
    UDPSocket *m_cUDPSocket_send = nullptr;
    UDPSocket *m_cUDPSocket_receive = nullptr;

#ifdef REPLAY_MODE
    DUMPData *m_pDumpData;
#endif

signals:
    void m_signalSendCmdToShip(const QByteArray data);
    void m_signalSendGPSToSimulator(const QString data);

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
    void On_receive_Simulator(QString);

#ifdef TESTING_MODE
    void on_pushButton_openPortArduino_clicked();
    void UpdateGPS();
#endif
#ifdef REPLAY_MODE
    void On_receive_RecordedData(QByteArray tmpdata);
    void On_receive_RecordedData_end();
#endif
    void on_pushButton_CmdLeftThruster_clicked();

    void on_pushButton_CmdRightThruster_clicked();

    void On_receive_pushButton_switchControl(bool);
private:
    Ui::Contriols *ui = nullptr;
    // 添加一个用于存储轨迹点的容器
//    QVector<QPointF> trackPoints;
//    QTimer *updateTimer = nullptr;

public:
    qreal sxNumber;
    qreal lNumber;
    qreal aNumber;

    qreal sxNumber_last = 0.0;
    qreal longtitude_last = 0.0;
    qreal latitude_last = 0.0;

    qreal m_s2kn = 1.9438445;
    qreal cmd2velocity = 0.0255;
    QFile *logFile = nullptr;
    Gps m_cGps;
    TimeDuration timeDuration;
    bool m_bIsControl = false;
    void keyPressEvent(QKeyEvent *event) override;

#ifdef TESTING_MODE
    //Arduino串口类
    SerialWorker *m_cSerialArduino = nullptr;
    //发送测试数据
    QTimer *m_updateTimer = nullptr;
//     float INumber0 = 121.0 + 31.261/60.0;
//     float ANumber0 = 38.0 + 52.190/60.0;
     double m_dLongtitude = 121.0 + 32.040 / 60.0;
     double m_dLatitude = 38.0 + 51.945/60.0;
     float m_fHeading = 0;
#endif
};

#endif // CONTRIOLS_H
