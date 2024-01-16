#include "bridge_zl.h"
extern "C" {
#include "utils/crcLib.h"
}
#include <QFile>
#include <QJsonDocument>
#include <QDebug>


Bridge_ZL::Bridge_ZL(QObject *parent) : QObject(parent)
{
     QFile file(":/config/config_bridge_ZL.json"); // 替换为你的 JSON 文件路径

     if (!file.open(QIODevice::ReadOnly)) {
         qDebug() << "Failed to open file.";
     }
     QByteArray jsonData = file.readAll();
     file.close();

     QJsonParseError error;
     QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &error);

     if (error.error != QJsonParseError::NoError) {
         qDebug() << "Failed to parse JSON:" << error.errorString();
     }

     if (!jsonDoc.isObject()) {
         qDebug() << "JSON is not an object.";
     }

     m_jConfigBridge_ZL = jsonDoc.object();
}

QByteArray Bridge_ZL::generateQueryMessage(int iAddress)
{
    m_Address = iAddress; //发送了，不一定能收到，所以不会同步
    unsigned short crcvalue;
    unsigned char cTempData[1000] = {};

    cTempData[0] = iAddress;	//地址码
    cTempData[1] = 0X03;		//功能码
    cTempData[2] = (unsigned char)(0X0000 >> 8);	//寄存器初始地址高字节
    cTempData[3] = (unsigned char)(0X0000);			//寄存器初始地址低字节

    cTempData[4] = (unsigned char)(m_registernumber >> 8);//寄存器数量高字节
    cTempData[5] = (unsigned char)(m_registernumber);		//寄存器数量低字节

    crcvalue = crc16_modbus(cTempData, 6);

    cTempData[6] = (unsigned char)(crcvalue);		//寄存器数量低字节
    cTempData[7] = (unsigned char)(crcvalue >> 8);	//寄存器数量高字节

//    qDebug().noquote() << qSetFieldWidth(2) << hex << crcvalue;
//    for (int i = 0; i < 6; ++i) {
//        qDebug("%02x ", cTempData[i]);
//    }
//    qDebug() << ""; // 换行
    QByteArray byteArray(reinterpret_cast<const char*>(cTempData), 8);
    return byteArray;
}

Bridge_ZL::Values_Bridge Bridge_ZL::parseData(QByteArray requestedData)
{
    const char * data = requestedData.constData();;
    unsigned short RXDatalength = requestedData.length();
    unsigned char cTempDataForRX[1000];

    // 复制数据，将 const char * 转换为 unsigned char *
    for (int i = 0; i < requestedData.length(); ++i) {
        cTempDataForRX[i] = static_cast<unsigned char>(data[i]);
    }
      //m_pCnComm->Purge();		//2018.6.29
//    RXDatalength = m_pCnComm->Read(cTempDataForRX, 7, 100); 	//从串口读数据(超时设置为100ms，原来为50ms）

    m_Address  = cTempDataForRX[0];                                                                                  //数据包长度以及地址码功能码以及寄存器数量都正确
    if ((RXDatalength == (5 + 2 * m_registernumber)) &&	//数据长度校验
//        (cTempDataForRX[0] == m_Address) &&	//地址校验
        (cTempDataForRX[1] == 0X03) &&	//功能码校验
        (cTempDataForRX[2] == (2 * m_registernumber)))
    {
        int crcvalue = crc16_modbus(cTempDataForRX, (3 + 2 * m_registernumber)); 	//计算CRC校验码

                                                                                //CRC校验判断是否正确
        if ((cTempDataForRX[3 + 2 * m_registernumber] == (unsigned char)(crcvalue)) && (cTempDataForRX[4 + 2 * m_registernumber] == (unsigned char)(crcvalue >> 8)))
        {
            for (int i = 0; i < m_registernumber; i++)
            {
                m_iOriginValBridge[i + m_Address - ADDRESS_BellLeft] = cTempDataForRX[3 + 2 * i + 0] * 16 * 16 + cTempDataForRX[3 + 2 * i + 1];	//读出数据存放在AI数组中
            }

        }
    }
//    else//判断独立设备是否存在
//    {
//        return false;
//    }
    if(m_Address == ADDRESS_BellLeft){
        m_sValuesBridge.valueBellLeft = ChangeTelegraphToTrueBell(m_jConfigBridge_ZL["iTelegraphBellL1"].toInt(), m_jConfigBridge_ZL["iTelegraphBellL2"].toInt(), m_jConfigBridge_ZL["iTelegraphBellL3"].toInt(), m_iOriginValBridge[m_Address - ADDRESS_BellLeft]);
    }
    if(m_Address == ADDRESS_BellRight){
        m_sValuesBridge.valueBellRight = ChangeTelegraphToTrueBell(m_jConfigBridge_ZL["iTelegraphBellR1"].toInt(), m_jConfigBridge_ZL["iTelegraphBellR2"].toInt(), m_jConfigBridge_ZL["iTelegraphBellR3"].toInt(),  m_iOriginValBridge[m_Address - ADDRESS_BellLeft]);

    }
    if(m_Address == ADDRESS_Rudder){
        m_sValuesBridge.valueRudder = ChangeSteerToTrueAngle(m_jConfigBridge_ZL["iRudderAngle1"].toInt(), m_jConfigBridge_ZL["iRudderAngle2"].toInt(), m_jConfigBridge_ZL["iRudderAngle3"].toInt(), m_iOriginValBridge[m_Address - ADDRESS_BellLeft]);
    }

    //通过判断ADRRES,使用相应的函数，获取车舵命令，一次循环全部搞定？
}

int Bridge_ZL::ChangeTelegraphToTrueBell(int iValue1, int iValue2, int iValue3, int iChangeValue)
{
    float fLightThrottleLevel = 0.0f;
    int iBell = 0;

    if (iValue1 >= iValue3)
    {
        if (iChangeValue >= iValue2)
        {
            fLightThrottleLevel = 10.0*abs(iValue1 - iChangeValue) / (iValue1 - iValue2);
            iBell = 10.0 - 2.0 * ((int)(fLightThrottleLevel / 2.0 + 0.5));
        }
        else
        {
            fLightThrottleLevel = 10.0*(iValue3 - iChangeValue) / abs(iValue3 - iValue2);
            iBell = -10.0 - 2.0 * ((int)(fLightThrottleLevel / 2.0 - 0.5));
        }
    }
    else
    {
        if (iChangeValue <= iValue2)
        {
            fLightThrottleLevel = 10.0*abs(iValue1 - iChangeValue) / abs(iValue1 - iValue2);
            iBell = 10.0 - 2.0 * ((int)(fLightThrottleLevel / 2.0 + 0.5));
        }
        else
        {
            fLightThrottleLevel = 10.0*(iValue3 - iChangeValue) / abs(iValue3 - iValue2);
            iBell = -10.0 + 2.0 * ((int)(fLightThrottleLevel / 2.0 + 0.5));
        }
    }

    return iBell;

}

int Bridge_ZL::ChangeAzimuthingToTrueCourse(int iValue1, int iValue2, int value3, int iChangeValue)
{
    float fLightThrottleLevel = 0.0f;
    int iCourse = 0;

    if (iValue1 > iValue2)
    {
        if ((iChangeValue >= iValue2) && (iChangeValue <= iValue1))
        {
            fLightThrottleLevel = 180.0 * abs(iValue1 - iChangeValue) / abs(iValue1 - iValue2);
            iCourse = (int)(fLightThrottleLevel);
        }
        else if (iChangeValue < iValue2)
        {
            fLightThrottleLevel = 180.0 * abs(iValue1 - iChangeValue) / abs(iValue1 - iValue2);
            iCourse = (int)(fLightThrottleLevel);
            iCourse = iCourse - 360;
        }
        else if (iChangeValue > iValue1)
        {
            fLightThrottleLevel = 180.0 * abs(iValue1 - iChangeValue) / abs(iValue1 - iValue2);
            iCourse = (int)(fLightThrottleLevel);
            iCourse = -iCourse;
        }
    }
    else if (iValue1 < iValue2)
    {
        if ((iChangeValue >= iValue1) && (iChangeValue <= iValue2))
        {
            fLightThrottleLevel = 180.0 * abs(iValue1 - iChangeValue) / abs(iValue1 - iValue2);
            iCourse = (int)(fLightThrottleLevel);
            iCourse = iCourse - 360;
        }
        else if (iChangeValue < iValue1)
        {
            fLightThrottleLevel = 180.0 * abs(iValue1 - iChangeValue) / abs(iValue1 - iValue2);
            iCourse = (int)(fLightThrottleLevel);
        }
        else if (iChangeValue > iValue2)
        {
            fLightThrottleLevel = 180.0 * abs(iValue1 - iChangeValue) / abs(iValue1 - iValue2);
            iCourse = (int)(fLightThrottleLevel);
            iCourse = 360 - iCourse;
        }
    }
    return iCourse;

}

int Bridge_ZL::ChangeSteerToTrueAngle(int iValue1, int iValue2, int iValue3, int iChangeValue)
{
    float fLightThrottleLevel = 0.0f;
    float fCos = 0;
    int iCos = 0;
    float fMax = m_jConfigBridge_ZL["iStbdMaxRudderAng"].toDouble();

    if (iValue1 >= iValue3)
    {
        if (iChangeValue >= iValue2)
        {
            if (iChangeValue > iValue1)
            {
                iChangeValue = iValue1;
            }
            fLightThrottleLevel = fMax*abs((float)(iValue1 - iChangeValue)) / (float)(iValue1 - iValue2);
            fCos = fMax - fLightThrottleLevel;
        }
        else
        {
            if (iChangeValue < iValue3)
            {
                iChangeValue = iValue3;
            }
            fLightThrottleLevel = fMax*(float)(iValue3 - iChangeValue) / abs((float)(iValue3 - iValue2));
            fCos = -fMax - fLightThrottleLevel;
        }
    }
    else
    {
        if (iChangeValue <= iValue2)
        {
            if (iChangeValue < iValue1)
            {
                iChangeValue = iValue1;
            }
            fLightThrottleLevel = fMax*abs((float)(iValue1 - iChangeValue)) / abs((float)(iValue1 - iValue2));
            fCos = fMax - fLightThrottleLevel;
        }
        else
        {
            if (iChangeValue > iValue3)
            {
                iChangeValue = iValue3;
            }
            fLightThrottleLevel = fMax*(float)(iValue3 - iChangeValue) / abs((float)(iValue3 - iValue2));
            fCos = -fMax + fLightThrottleLevel;
        }
    }

    if (fCos >= 0)
    {
        iCos = fCos + 0.5f;
    }
    else
    {
        iCos = fCos - 0.5f;
    }
    return iCos;

}

#include <sstream>
Bridge_ZL::Values_Bridge Bridge_ZL::parseVHW(const QString& vhwMessage) {
    std::istringstream stream(vhwMessage.toStdString());
    std::string field;

    std::vector<std::string> fields;
    while (std::getline(stream, field, ',')) {
        fields.push_back(field);
    }
    Bridge_ZL::Values_Bridge values_bridge;
    // 确保字段数量足够，并且第三个字段是"A"（数据有效） 注意空格，第三个字段是空格+A
//    qDebug() <<  fields[2].c_str();
    if (fields.size() >= 6 &&
            fields[2] == std::string("A")) {
        // 获取数字字段
        std::string value_RudderStr = fields[3];
        std::string value_BellStr = fields[4];

        try {
            // 解析数字
            int value_Rudder = std::stoi(value_RudderStr);
            int value_Bell = std::stoi(value_BellStr);

            // 输出或使用这些数字
//            qDebug() << "舵：" << value_Rudder-35;
//            qDebug() << "车：" << value_Bell-100;

            // 如果有需要，可以将这些值存储到类成员变量中
            values_bridge.valueRudder = value_Rudder;
            values_bridge.valueBellLeft = values_bridge.valueBellRight = value_Bell;
        }
        catch (const std::invalid_argument& e) {
            qDebug()  << "无效的数据：" << e.what();
        }
        catch (const std::out_of_range& e) {
            qDebug()  << "数据超出范围：" << e.what();
        }
    }
    else {
        qDebug() << "无效的VHW消息";
        values_bridge.valueBellLeft = values_bridge.valueBellRight = 100;
        values_bridge.valueRudder = 35;
    }
    return values_bridge;
}
