#ifndef BRIDGE_ZL_H
#define BRIDGE_ZL_H
#include <QObject>
#include <QByteArray>
#include <QJsonObject>

class Bridge_ZL : public QObject
{
    Q_OBJECT
public:
    explicit Bridge_ZL(QObject *parent = nullptr);

    enum Address {
        ADDRESS_BellLeft = 0x05,
        ADDRESS_BellRight = 0x06,
        ADDRESS_Rudder = 0x07,
    };
    struct Values_Bridge {
        int valueBellRight;
        int valueBellLeft;
        int valueRudder;
    };
    Values_Bridge m_sValuesBridge = {0, 0, 0}; // 使用大括号初始化列表将所有成员初始化为0
    int m_iOriginValBridge[3] = {};
    unsigned short m_registernumber = 0X0001;  //寄存器数量
    int m_Address = ADDRESS_BellLeft;
    QByteArray generateQueryMessage(int Address);
    Values_Bridge parseData(QByteArray);
    QJsonObject m_jConfigBridge_ZL ;


    int ChangeTelegraphToTrueBell(int iValue1, int iValue2, int iValue3, int iChangeValue);//转换为真实档位
    // 三个值可能是最大值，最小值，中值
    int ChangeAzimuthingToTrueCourse(int iValue1, int iValue2, int value3, int iChangeValue); //转换为真实档位
    int ChangeSteerToTrueAngle(int iValue1, int iValue2, int iValue3, int iChangeValue);

};



#endif // BRIDGE_ZL_H
