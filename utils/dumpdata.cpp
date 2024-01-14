#include "dumpdata.h"
#include "QDebug"
#include "QMessageBox"

//DUMPData::DUMPData(QObject *parent) : QObject(parent)
//{
//}

DUMPData::DUMPData(QString filePath, QObject *parent)
{
    if(!filePath.isEmpty()) m_sFilePath = filePath;
    m_pRecordedData = new QFile(m_sFilePath);
    if (!m_pRecordedData->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open CSV file: " << m_pRecordedData->errorString();
//        return false;  // 无法打开文件
    }
    // 使用 QTextStream 读取文件
    m_pfileIn  = new QTextStream(m_pRecordedData);
    m_thread = new QThread;
    this->moveToThread(m_thread);
    m_pRecordedData->moveToThread(m_thread);
    m_updateTimer = new QTimer();
    m_updateTimer->setInterval(5);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(dumpData()));
    m_updateTimer->moveToThread(m_thread);
    connect(m_thread, SIGNAL(started()), this, SLOT(start_timer()));
    m_thread->start();
}


void DUMPData::start_timer()
{
    m_updateTimer->start();
}

DUMPData::~DUMPData()
{
    m_pRecordedData->close();
}

void DUMPData::dumpData()
{
    double lon = 0, lat = 0, heading = 0;
    // 逐行读取文件内容 只适用CSV文件
    if(!m_pfileIn->atEnd()) {
        QString line = m_pfileIn->readLine();
        // 使用逗号分隔符拆分 CSV 行
        QStringList fields = line.split(',');
        // 确保行中有足够的字段
        if (fields.size() >= 4) {
            bool conversionOk = false;
            double dx = -0.000469, dy = +0.000065;
            lon = fields[2].toDouble(&conversionOk) + dx;
            if (!conversionOk) {
                qDebug() << "Failed to convert longitude to double: " << fields[2];
                return;  // 转换失败，跳过当前行
            }

            lat = fields[3].toDouble(&conversionOk) + dy;
            if (!conversionOk) {
                qDebug() << "Failed to convert latitude to double: " << fields[3];
                return;
            }

            heading = fields[1].toDouble(&conversionOk) + dy;
            if (!conversionOk) {
                qDebug() << "Failed to convert latitude to double: " << fields[3];
                return;
            }
        }
    }
    else{
        m_updateTimer->stop();
        emit endRecordedDataToGUI();
    }

    QString result;
    QTextStream resultStream(&result);
    resultStream << "SX" << heading << "L" << QString::number(lon, 'f', 6) << "A" << QString::number(lat, 'f', 6) << "E";
    QByteArray byteArray = result.toUtf8();
    emit sendRecordedDataToGUI(byteArray);

}
