#ifndef DTU_H
#define DTU_H

#include <QByteArray>
#include <QFile>

class DTU
{
public:
    DTU();
    struct DataDTU{
        qreal course ;
        qreal longtitude;
        qreal latitude;
    }m_sDataDTU;

    DataDTU processData(const QByteArray& qbytearray);
    void parseData(QByteArray newData);
    QByteArray m_QSreceivedData;
    QFile *logFile;



};

#endif // DTU_H
