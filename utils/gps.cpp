#include "gps.h"

Gps::Gps()
{

}


double Gps::toRadians(double degree) {
    return degree * PI / 180.0;
}

#include <QDateTime>
QString Gps::makeGPRMC(double longitude, double latitude, double heading)
{
    // 创建当前时间的字符串
    QString currentTime = QDateTime::currentDateTime().toString("hhmmss");


    // 创建GPRMC字符串
    QString GPRMC = QString("$GPRMC,%1,A,").arg(currentTime);

    // 添加纬度信息
    QString latitudeString = QString("%1,%2").arg(latitude, 0, 'f', 6).arg(latitude >= 0 ? "N" : "S");
    GPRMC += latitudeString + ",";

    // 添加经度信息
    QString longitudeString = QString("%1,%2").arg(longitude, 0, 'f', 6).arg(longitude >= 0 ? "E" : "W");
    GPRMC += longitudeString + ",";

    // 添加速度信息（这里设为0，你可以根据需要更改）
    GPRMC += "0.0,";

    // 添加航向信息
    QString headingString = QString("%1").arg(heading, 0, 'f', 6);
    GPRMC += headingString;

    return GPRMC;
}

#include <cmath>
double Gps::calculateDistance(double lat1, double lon1, double lat2, double lon2)
{
    if (lat1 <= 0) return 1e-12;
    // 将角度转换为弧度
//    lat1 = toRadians(lat1);
//    lon1 = toRadians(lon1);
//    lat2 = toRadians(lat2);
//    lon2 = toRadians(lon2);

//    // 应用球面余弦定理计算距离
//    double dlon = lon2 - lon1;
//    double dlat = lat2 - lat1;
//    double a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
//    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

//    return EARTH_RADIUS * c;

    return sqrt(pow(lat2 - lat1, 2) + pow(lon2 - lon1, 2)) * 111120;
}
