#ifndef DUMPDATA_H
#define DUMPDATA_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QTimer>
#include <QTextStream>

class DUMPData : public QObject
{
    Q_OBJECT
public:
//    explicit DUMPData(QObject *parent = nullptr); // explicit  防止意外的类型转换
    DUMPData(QString filePath = "", QObject *parent = nullptr);
    ~DUMPData();

    QFile *m_pRecordedData;
    QString m_sFilePath = "C:\\Users\\26282\\Desktop\\release_landterminal\\11.csv";
    QTextStream *m_pfileIn;

    QThread *m_thread;
    QTimer *m_updateTimer;

signals:
    void sendRecordedDataToGUI(QByteArray);
    void endRecordedDataToGUI();

public slots:
    void dumpData();
    void start_timer();
};

#endif // DUMPDATA_H
