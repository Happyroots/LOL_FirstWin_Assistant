#ifndef CONTRIOLS_H
#define CONTRIOLS_H

#include <QWidget>
#include <QTcpSocket>
#include "dtu_arduino_driver.h"
#include<chrono>
#include <thread>
#include <QStandardItemModel>
#include <QImage>  // 添加这一行
#include <QLabel>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QKeyEvent>
#include "gps.h"
#include "timeduration.h"

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

    // 新添加的 QLabel 成员变量
    QLabel *mapLabel;
    SerialWorker *m_SerialWorker_DTU;
    QByteArray m_QSreceivedData;
    QStandardItemModel* db_table_model_ = nullptr;

signals:
    void m_signalSendCmdToShip(const QByteArray data);
    void closeSerialPort_DTU();
    void update_DashboardCourseSpeed(double value);

private slots:
//   / void on_pushButton_connect_clicked();

    void on_pushButton_cmdForward_clicked();

    void on_pushButton_CmdBack_clicked();

    void on_pushButton_CmdLeft_clicked();

    void on_pushButton_CmdRight_clicked();

    void on_pushButton_openPortDTU_clicked();

    void on_receive(QByteArray tmpdata);
    void sendCmdToShip();
    void parseData(QByteArray newData);
    void processData(const QByteArray& qbytearray);
    // 新添加的成员函数，用于绘制轨迹点
    //void drawTrackPoint(qreal lNumber, qreal aNumber);
    //void onUpdateTimerTimeout();
    //void simulateData();
    void connect_clicked();
    qreal caculate_velocity_abs(qreal , qreal);

private:
    Ui::Contriols *ui;
    // 添加一个用于存储轨迹点的容器
    QVector<QPointF> trackPoints;
    QTimer *updateTimer;
    // 在 Contriols 类中添加一个用于模拟数据的定时器
    //QTimer *simulatedDataTimer;

protected:
    // 重写绘图事件
    void paintEvent(QPaintEvent *event) override;
    void updateMapWithTrack();

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

};

#endif // CONTRIOLS_H
