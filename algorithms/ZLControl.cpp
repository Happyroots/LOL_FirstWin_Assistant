#include "ZLControl.h"

ZLControl::ZLControl() 
{
    //初始化
    CurPointID = 0;
}

ZLControl::~ZLControl() {}

//角度转弧度
double ZLControl::ToRadians(double degree)
{
	return degree * (pi / 180.0);
}

//弧度转角度
double ZLControl::ToDegree(double Radian)
{
	return Radian * (180.0 / pi);
}

//计算两个经纬度之间的距离
double ZLControl::CalDistance(double lat1, double lon1, double lat2, double lon2)
{
    // 将经纬度转换为弧度
    lat1 = ToRadians(lat1);
    lon1 = ToRadians(lon1);
    lat2 = ToRadians(lat2);
    lon2 = ToRadians(lon2);

    // 计算差值
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    // 使用 Haversine 公式计算距离
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    // 计算距离
    double distance = EarthR * c;

    return distance;
}

//计算两个经纬度之间的夹角
double ZLControl::CalBearing(double lat1, double lon1, double lat2, double lon2)
{
    // 将经纬度转换为弧度
    lat1 = ToRadians(lat1);
    lon1 = ToRadians(lon1);
    lat2 = ToRadians(lat2);
    lon2 = ToRadians(lon2);

    // 计算差值
    double dLon = lon2 - lon1;

    // 计算方位角
    double x = sin(dLon) * cos(lat2);
    double y = cos(lat1) * sin(lat2) - (sin(lat1) * cos(lat2) * cos(dLon));
    double bearing = atan2(x, y);

    // 转换为角度并调整为正值
    bearing = ToDegree(bearing);
    bearing = fmod((bearing + 360.0), 360.0);

    return bearing;
}


// 计算两个地理坐标点的点积
double ZLControl::dotProduct(const GPSPoint& A, const GPSPoint& B, const GPSPoint& P)
{
    double latA_rad = ToRadians(A.lat);
    double lonA_rad = ToRadians(A.lon);
    double latB_rad = ToRadians(B.lat);
    double lonB_rad = ToRadians(B.lon);
    double latP_rad = ToRadians(P.lat);
    double lonP_rad = ToRadians(P.lon);

    double vectorABx = cos(latA_rad) * cos(lonA_rad) - cos(latB_rad) * cos(lonB_rad);
    double vectorABy = cos(latA_rad) * sin(lonA_rad) - cos(latB_rad) * sin(lonB_rad);
    double vectorAPx = cos(latA_rad) * cos(lonP_rad) - cos(latP_rad) * cos(lonP_rad);
    double vectorAPy = cos(latA_rad) * sin(lonP_rad) - cos(latP_rad) * sin(lonP_rad);

    return vectorABx * vectorAPx + vectorABy * vectorAPy;
}

// 计算经纬度点 P 相对于经度线的夹角
double ZLControl::angleWithLongitude(const GPSPoint& A, const GPSPoint& B, const GPSPoint& P)
{
    double dot = dotProduct(A, B, P);

    double magnitudeAB = sqrt(pow(cos(ToRadians(A.lat)) * cos(ToRadians(A.lon)) -
        cos(ToRadians(B.lat)) * cos(ToRadians(B.lon)), 2) +
        pow(cos(ToRadians(A.lat)) * sin(ToRadians(A.lon)) -
            cos(ToRadians(B.lat)) * sin(ToRadians(B.lon)), 2));

    double magnitudeAP = sqrt(pow(cos(ToRadians(A.lat)) * cos(ToRadians(A.lon)) -
        cos(ToRadians(P.lat)) * cos(ToRadians(P.lon)), 2) +
        pow(cos(ToRadians(A.lat)) * sin(ToRadians(A.lon)) -
            cos(ToRadians(P.lat)) * sin(ToRadians(P.lon)), 2));

    return ToDegree(acos(dot / (magnitudeAB * magnitudeAP)));
}

//获取期望点的数量
int ZLControl::NumPoints()
{
    return this->ExPoints.size();
}

//判断是否有期望点
bool ZLControl::IsHavePoint()
{
    if (this->ExPoints.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

//添加期望点
void ZLControl::AddPoints(GPSPoint P)
{
    if (!IsHavePoint())
    {
        this->Initialpoint = P;
    }
    this->ExPoints.push_back(P);
}

void ZLControl::AddPoints(double lat, double lon)
{
    GPSPoint P;
    P.lat = lat;
    P.lon = lon;
    if (!IsHavePoint())
    {
        this->Initialpoint = P;
    }
    this->ExPoints.push_back(P);
}

//指定位置添加期望点
void ZLControl::AddPoints(size_t num, GPSPoint P)
{
    if (IsHavePoint())
    {
        if (this->ExPoints.size() > num)
        {
            //索引有效
            this->ExPoints.insert(this->ExPoints.begin() + num, P);
        }
        else
        {
            std::cout << "索引太小，索引无效！" << std::endl;
        }

    }
    else
    {
        std::cout << "没有期望点，请添加" << std::endl;
        return;
    }
}

//删除期望点
void ZLControl::DelPoints()
{
    this->ExPoints.pop_back();
}

//指定位置删除期望点
void ZLControl::DelPoints(size_t num)
{
    if (ZLControl::IsHavePoint())
    {
        if (this->ExPoints.size() > num)
        {
            //索引有效
            this->ExPoints.erase(this->ExPoints.begin() + num);
        }
        else
        {
            std::cout << "索引太小，索引无效！" << std::endl;
        }

    }
    else
    {
        std::cout << "没有期望点，请添加" << std::endl;
        return;
    }
}

//修改指定位置的期望点
void ZLControl::VerPoints(size_t num, GPSPoint P)
{
    // 检查索引是否有效
    if (num < this->ExPoints.size()) 
    {
        // 直接通过索引访问元素，并修改其值
        this->ExPoints[num] = P;
    }
    else {
        std::cout << "无效的索引" << std::endl;
    }
}

//打印期望点
void ZLControl::PrintPoints()
{
    std::cout << "期望点: ";
    for (const auto& element : this->ExPoints) 
    {
        std::cout << element.lat << "\t" << element.lon << "\n";
    }
    std::cout << std::endl;
}

//计算预期点与船艏向的夹角
double ZLControl::CalDegreOfPoint(GPSPoint P,double Psi)
{
    GPSPoint P1;
    double latA = ToRadians(P.lat);
    double lonA = ToRadians(P.lon);
    double Bearing = ToRadians(Psi);
    double distance = 80;

    // 计算新的纬度
    double latB = asin(sin(latA) * cos(distance / EarthR) + cos(latA) * sin(distance / EarthR) * cos(Bearing));

    // 计算新的经度
    double lonB = lonA + atan2(sin(Bearing) * sin(distance / EarthR) * cos(latA), cos(distance / EarthR) - sin(latA) * sin(latB));

    // 将结果转换为度数
    P1.lat = ToDegree(latB);
    P1.lon = ToDegree(lonB);

    GPSPoint P2 = this->ExPoints.at(CurPointID);
    //计算三个角度的距离 P船 P1首向 P2期望点
    double Deg = angleWithLongitude(P, P1, P2);

    return Deg;
}

//输出角度
double ZLControl::OutputDeg(double Bearing)
{
    //输出舵令，范围在-35-35
    double Delta;
    if (fabs(Bearing) > 90)
    {
        Delta =  Bearing * 0.2;
    }
    else
    {
        Delta = Bearing * 0.1;
    }

    if (Delta > 35)
    {
        Delta = 35;
    }
    else if (Delta < -35)
    {
        Delta = -35;
    }
    else
    {
        Delta = Delta;
    }

    return Delta;
}

//输出车令
int ZLControl::OutPutPeller(double distance,double R)
{
    //输出百分比，distance代表船上GPS点相对期望点的距离，R代表范围
    if (distance < R)
    {
        return 40;
        if (IsHavePoint())
        {
            CurPointID += 1;//到下一个
            if (CurPointID >= this->ExPoints.size())
            {
                CurPointID = 0;
            }
        }
        

        PrintPoints();
    }
    else
    {
        return 70;
    }
}

//将经纬度转为xy轴m制单位
ZLControl::XYPoint ZLControl::gpsToXY(const GPSPoint& gpsPoint, const GPSPoint& referenceGPS)
{

    // 转换为弧度制
    double latRad = ToRadians(gpsPoint.lat);// gpsPoint.latitude* (M_PI / 180.0);
    double lonRad = ToRadians(gpsPoint.lon);//gpsPoint.longitude * (M_PI / 180.0);
    double refLatRad = ToRadians(referenceGPS.lat);//referenceGPS.latitude * (M_PI / 180.0);
    double refLonRad = ToRadians(referenceGPS.lon);//referenceGPS.longitude * (M_PI / 180.0);

    // 计算相对于初始参考点的经纬度差值
    double deltaLon = lonRad - refLonRad;
    double deltaLat = latRad - refLatRad;

    XYPoint xyPoint;
    // 使用球面近似法计算 xy 坐标，并调整相对于初始参考点的差值
    xyPoint.y = EarthR * deltaLon;
    xyPoint.x = EarthR * log(tan(pi / 4.0 + latRad / 2.0) / tan(pi / 4.0 + refLatRad / 2.0));
    return xyPoint;
}

//获取xy系列点
void ZLControl::CoverG2XY()
{
    if (IsHavePoint())
    {
        for (const auto& element : this->ExPoints)
        {
            //std::cout << element.lat << "\t" << element.lon << "\n";
            XYPoint xy = gpsToXY(element, Initialpoint);
            this->ExPoints_xy.push_back(xy);
        }
    }
    else
    {
        return;
    }
}

//求点到线的角度
double ZLControl::CalAngle_Ex_ship(const GPSPoint& P, double Psi)
{
    //P 是船位
    double psi = ToRadians(Psi);
    XYPoint SP = gpsToXY(P, this->Initialpoint); // 船点

    //根据船点做沿航向30m的船首点
    XYPoint AP;
    AP.x = (SP.x + 0.0 * cos(-psi) - 30.0 * sin(-psi));
    AP.y = (SP.y + 0.0 * sin(-Psi) + 30.0 * cos(-Psi));

    //获取期望点
    XYPoint BP;
    BP = gpsToXY(this->ExPoints.at(CurPointID), this->Initialpoint);

    // 计算
    double dot = dotProduct(SP, AP, BP);
    double magnitudeAB = sqrt(pow(AP.x - SP.x, 2) + pow(AP.y - SP.y, 2));
    double magnitudeAP = sqrt(pow(BP.x - SP.x, 2) + pow(BP.y - SP.y, 2));

    double Angle = ToDegree(acos(dot / (magnitudeAB * magnitudeAP)));
    if ((AP.x - SP.x) * (BP.x - SP.x) + (AP.y - SP.y) * (BP.y - SP.y) < 0)
    {
        Angle = -Angle;
    }

    return Angle;
}

double ZLControl::CalAngle_Ex_ship(const XYPoint& P, double Psi)
{
    //P 是船位
    double psi = ToRadians(Psi);

    //根据船点做沿航向30m的船首点
    XYPoint AP;
    AP.x = (P.x + 0.0 * cos(-psi) - 30.0 * sin(-psi));
    AP.y = (P.y + 0.0 * sin(-Psi) + 30.0 * cos(-Psi));

    //获取期望点
    XYPoint BP;
    BP = gpsToXY(this->ExPoints.at(CurPointID), this->Initialpoint);

    // 计算
    double dot = dotProduct(P, AP, BP);
    double magnitudeAB = sqrt(pow(AP.x - P.x, 2) + pow(AP.y - P.y, 2));
    double magnitudeAP = sqrt(pow(BP.x - P.x, 2) + pow(BP.y - P.y, 2));

    double Angle = ToDegree(acos(dot / (magnitudeAB * magnitudeAP)));
    if ((AP.x - P.x) * (BP.x - P.x) + (AP.y - P.y) * (BP.y - P.y) < 0)
    {
        Angle = -Angle;
    }

    return Angle;
}

// 计算两个地理坐标点的点积
double ZLControl::dotProduct(const XYPoint& A, const XYPoint& B, const XYPoint& P)
{
    double vectorABx = B.x - A.x;
    double vectorABy = B.y - A.y;
    double vectorAPx = P.x - A.x;
    double vectorAPy = P.y - A.y;

    return vectorABx * vectorAPx + vectorABy * vectorAPy;
}

//将经纬度转为UTM m制单位
ZLControl::XYPoint ZLControl::GpsToUtm(const GPSPoint& gpsPoint)
{
    double a = 6378.137;
    double e = 0.0818192;
    double k0 = 0.9996;
    double E0 = 500;
    double N0 = 0;

    double Zonenum = (int)(gpsPoint.lon / 6.0) + 31;
    double lamda0 = (Zonenum - 1) * 6.0 - 180 + 3;
    lamda0 = ToRadians(lamda0);
    double phi = ToRadians(gpsPoint.lat);
    double lamda = ToRadians(gpsPoint.lon);

    double v = 1.0 / sqrt(1 - pow(e, 2.0) * pow(sin(phi), 2.0));
    double A = (lamda - lamda0) * cos(phi);
    double T = tan(phi) * tan(phi);
    double C = pow(e, 2) * cos(phi) * cos(phi) / (1 - pow(e, 2));
    double s = (1 - pow(e, 2) / 4.0 - 3 * pow(e, 3) / 64.0 - 5 * pow(e, 6) / 256.0) * phi -
               (3 * pow(e, 2) / 8.0 + 3 * pow(e, 4) / 32.0 + 45 * pow(e, 6.0) / 1024.0) * sin(2 * phi) +
               (15 * pow(e, 4) / 256.0 + 45 * pow(e, 6) / 1024.0) * sin(4 * phi) - 35 * pow(e, 6.0) / 3072.0 * sin(6 * phi);
    double UTME = E0 + k0 * a * v * (A + (1 - T + C) * pow(A, 3) / 6.0 + (5 - 18 * T + T * T) * pow(A, 5) / 120.0);
    double UTMN = N0 + k0 * a * (s + v * tan(phi) * (A * A / 2.0 + (5 - T + 9 * C + 4 * C * C) * pow(A, 4) / 24.0 + (61 - 58 * T + T * T) * pow(A, 6) / 720.0));
    ZLControl::XYPoint xypoint;
    xypoint.x = UTME * 1000;
    xypoint.y = UTMN * 1000;
    return xypoint;
}

// UTM 参考点
ZLControl::XYPoint ZLControl::GPS2UTM_REF(const GPSPoint& gpsPoint, const GPSPoint& referenceGPS)
{
    ZLControl::XYPoint A = GpsToUtm(gpsPoint);
    ZLControl::XYPoint RA = GpsToUtm(referenceGPS);
    ZLControl::XYPoint Temp;
    Temp.x = A.x - RA.x;
    Temp.y = A.y - RA.y;
    return Temp;
}

//计算
ZLControl::ControlInfo ZLControl::CalMainTest(const GPSPoint& ship, double Psi)
{

    //根据期望点、船位、航向角计算夹角
    double Angle = CalAngle_Ex_ship(ship, Psi);
    double dist = CalBearing(ship.lat, ship.lon, this->ExPoints.at(CurPointID).lat, this->ExPoints.at(CurPointID).lon);
    ControlInfo CInfo;
    CInfo.DeltaE = OutputDeg(Angle);
    CInfo.PropE = OutPutPeller(dist, 5);
    return CInfo;
}

ZLControl::ControlInfo ZLControl::CalMainTest(double ship_lat, double ship_lon, double Psi)
{
    GPSPoint ship(ship_lat, ship_lon);
    //根据期望点、船位、航向角计算夹角
    double Angle = CalAngle_Ex_ship(ship, Psi);
    double dist = CalBearing(ship.lat, ship.lon, this->ExPoints.at(CurPointID).lat, this->ExPoints.at(CurPointID).lon);
    ControlInfo CInfo;
    CInfo.DeltaE = OutputDeg(Angle);
    CInfo.PropE = OutPutPeller(dist, 5);
    return CInfo;
}