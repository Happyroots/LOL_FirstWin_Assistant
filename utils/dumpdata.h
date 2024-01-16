#ifndef DUMPDATA_H
#define DUMPDATA_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QTimer>
#include <QTextStream>
#include <QSettings>

class DUMPData : public QObject
{
    Q_OBJECT
public:
//    explicit DUMPData(QObject *parent = nullptr); // explicit  防止意外的类型转换
    DUMPData(QString filePath = "", QObject *parent = nullptr);
    ~DUMPData();

    QFile *m_pRecordedData;
    QString m_sFilePath;
//    QString m_sFilePath = "C:\\Users\\93211\\Desktop\\11.csv";

    QTextStream *m_pfileIn;

    QThread *m_thread;
    QTimer *m_updateTimer;

    // 创建QSettings对象
    QSettings *m_pSettings = nullptr;

signals:
    void sendRecordedDataToGUI(QByteArray);
    void endRecordedDataToGUI();

public slots:
    void dumpData();
    void start_timer();
};

#endif // DUMPDATA_H
