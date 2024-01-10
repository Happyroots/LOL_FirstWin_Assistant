#include "dtu.h"
#include <QString>
#include <QRegularExpression>
#include <QDateTime>
#include <QTextStream>

DTU::DTU()
{
    QString logFileName = QDateTime::currentDateTime().toString("yyyy-MM-dd--hh:mm:ss");
    // 创建并打开日志文件，如果文件已经存在，会被清空
    logFile = new QFile(logFileName);

}


void DTU::parseData(QByteArray newData)
{

    int endIdx = newData.indexOf("E");
    if (endIdx != -1) {
        newData.remove(endIdx+1, newData.size()); // 删除已处理的数据包，包括'E\n'
        m_QSreceivedData += newData; // 追加新接收到的数据
//        qDebug() << "m_QSreceivedData:" << m_QSreceivedData;
        int startIdx = m_QSreceivedData.indexOf("X"); // 查找起始位置
        if (startIdx != -1) {   //一句完整的
            newData.remove(0, startIdx-1);
            m_QSreceivedData.remove(0, startIdx-1); // 删除已处理的数据包，包括'E\n'
            processData(m_QSreceivedData);
        }
        m_QSreceivedData = "";
        newData = "";
    }
    int startIdx = newData.indexOf("X");
    if (startIdx != -1) {
        newData.remove(0, startIdx-1); // 删除已处理的数据包，包括'E\n'
        m_QSreceivedData += newData; // 追加新接收到的数据
        endIdx = newData.indexOf("E"); // 查找结束位置
        if (endIdx != -1) {
            newData.remove(endIdx+1, newData.size()); // 删除已处理的数据包，包括'E\n'
            m_QSreceivedData = newData; // 追加新接收到的数据
            processData(m_QSreceivedData);
//            qDebug() << "m_QSreceivedData:" << m_QSreceivedData;
            m_QSreceivedData = "";
            newData = "";
        }
    }
//    qDebug() << "after process" << newData;
//    分成4条发送怎么办？/
//    m_QSreceivedData += newData;
    // 去除空串，并把含有信息但没有标识符的部分拼接到最后一个包中
    if (newData.isEmpty()) {
        newData = "";
    }
}

DTU::DataDTU DTU::processData(const QByteArray& qbytearray){

//    m_Client->write(QString(qbytearray).toUtf8().data());

    QString str = QString::fromUtf8(qbytearray);
    // 获取当前时间
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QRegularExpression regex("SX(-*[0-9.]+)L(-*[0-9.]+)A(-*[0-9.]+)E");
    QRegularExpressionMatch match = regex.match(str);

    if (match.hasMatch()) {
        qreal sxNumber = match.captured(1).toDouble();
        qreal lNumber =  match.captured(2).toDouble();
        qreal aNumber =  match.captured(3).toDouble();
        m_sDataDTU.course = sxNumber;
        m_sDataDTU.latitude = lNumber;
        m_sDataDTU.longtitude = aNumber;
//        qDebug() << "SX: " << sxNumber << " Long: " << lNumber << " Lat: " << aNumber;
        // 将输出语句修改为保存sxNumber、lNumber和aNumber的值
//        ui->lineEdit_longtitude->setText(QString::number(lNumber, 'f', 6));
//        ui->lineEdit_latitude->setText(QString::number(aNumber, 'f', 6));
//        qreal velocity_abs = caculate_velocity_abs(lNumber, aNumber);
//        ui->lineEdit_abV_m->setText(QString::number( velocity_abs, 'f', 2));
//        ui->lineEdit_abV_kn->setText(QString::number( velocity_abs*1.94384, 'f', 1));
//        if (sxNumber < 0) sxNumber += 360;
//        emit update_DashboardCourseSpeed(sxNumber); //
        if (logFile->open(QIODevice::Append | QIODevice::Text))
        {
            QTextStream stream(logFile);
//            stream << currentTime << "," << sxNumber << "," << lNumber << "," << aNumber << "\n";
            stream << currentTime << "," << QString::number(sxNumber, 'f', 6) << ","
                   << QString::number(lNumber, 'f', 6) << "," << QString::number(aNumber, 'f', 6) << "\n";
            logFile->close();
        }

        {
            //期望点
            //添加期望点test m_cAlgorithm_control.AddPoint(lat,lon);
//            m_cAlgorithm_control.AddPoints(121.534996, 38.865784);
//            m_cAlgorithm_control.AddPoints(121.534683, 38.865903);
//            m_cAlgorithm_control.AddPoints(121.534531, 38.865887);
//            m_cAlgorithm_control.AddPoints(121.534760, 38.865757);

//            ZLControl::GPSPoint GPS1(lNumber,aNumber);
//            //GPS1.lon=lNumber;
//            //GPS1.lat=aNumber;
//            ZLControl::ControlInfo CIF;
//            CIF =  m_cAlgorithm_control.CalMainTest(GPS1, sxNumber);
//            m_iCmdRudder = CIF.DeltaE + m_iBias_cmd_rudder;
//            m_iCmdPropeller = CIF.PropE + m_iBias_cmd_prop - 30;
//            qDebug()<<"Rud:"<<CIF.DeltaE;
//            qDebug()<<"Prop:"<<CIF.PropE;
//            sendCmdToShip();
        }

    }
}
