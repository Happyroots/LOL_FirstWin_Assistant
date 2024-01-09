#include "ZLControl.h"

ZLControl::ZLControl() 
{
    //��ʼ��
    CurPointID = 0;
}

ZLControl::~ZLControl() {}

//�Ƕ�ת����
double ZLControl::ToRadians(double degree)
{
	return degree * (pi / 180.0);
}

//����ת�Ƕ�
double ZLControl::ToDegree(double Radian)
{
	return Radian * (180.0 / pi);
}

//����������γ��֮��ľ���
double ZLControl::CalDistance(double lat1, double lon1, double lat2, double lon2)
{
    // ����γ��ת��Ϊ����
    lat1 = ToRadians(lat1);
    lon1 = ToRadians(lon1);
    lat2 = ToRadians(lat2);
    lon2 = ToRadians(lon2);

    // �����ֵ
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    // ʹ�� Haversine ��ʽ�������
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    // �������
    double distance = EarthR * c;

    return distance;
}

//����������γ��֮��ļн�
double ZLControl::CalBearing(double lat1, double lon1, double lat2, double lon2)
{
    // ����γ��ת��Ϊ����
    lat1 = ToRadians(lat1);
    lon1 = ToRadians(lon1);
    lat2 = ToRadians(lat2);
    lon2 = ToRadians(lon2);

    // �����ֵ
    double dLon = lon2 - lon1;

    // ���㷽λ��
    double x = sin(dLon) * cos(lat2);
    double y = cos(lat1) * sin(lat2) - (sin(lat1) * cos(lat2) * cos(dLon));
    double bearing = atan2(x, y);

    // ת��Ϊ�ǶȲ�����Ϊ��ֵ
    bearing = ToDegree(bearing);
    bearing = fmod((bearing + 360.0), 360.0);

    return bearing;
}


// �����������������ĵ��
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

// ���㾭γ�ȵ� P ����ھ����ߵļн�
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

//��ȡ�����������
int ZLControl::NumPoints()
{
    return this->ExPoints.size();
}

//�ж��Ƿ���������
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

//���������
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

//ָ��λ�����������
void ZLControl::AddPoints(size_t num, GPSPoint P)
{
    if (IsHavePoint())
    {
        if (this->ExPoints.size() > num)
        {
            //������Ч
            this->ExPoints.insert(this->ExPoints.begin() + num, P);
        }
        else
        {
            std::cout << "����̫С��������Ч��" << std::endl;
        }

    }
    else
    {
        std::cout << "û�������㣬�����" << std::endl;
        return;
    }
}

//ɾ��������
void ZLControl::DelPoints()
{
    this->ExPoints.pop_back();
}

//ָ��λ��ɾ��������
void ZLControl::DelPoints(size_t num)
{
    if (ZLControl::IsHavePoint())
    {
        if (this->ExPoints.size() > num)
        {
            //������Ч
            this->ExPoints.erase(this->ExPoints.begin() + num);
        }
        else
        {
            std::cout << "����̫С��������Ч��" << std::endl;
        }

    }
    else
    {
        std::cout << "û�������㣬�����" << std::endl;
        return;
    }
}

//�޸�ָ��λ�õ�������
void ZLControl::VerPoints(size_t num, GPSPoint P)
{
    // ��������Ƿ���Ч
    if (num < this->ExPoints.size()) 
    {
        // ֱ��ͨ����������Ԫ�أ����޸���ֵ
        this->ExPoints[num] = P;
    }
    else {
        std::cout << "��Ч������" << std::endl;
    }
}

//��ӡ������
void ZLControl::PrintPoints()
{
    std::cout << "������: ";
    for (const auto& element : this->ExPoints) 
    {
        std::cout << element.lat << "\t" << element.lon << "\n";
    }
    std::cout << std::endl;
}

//����Ԥ�ڵ��봬����ļн�
double ZLControl::CalDegreOfPoint(GPSPoint P,double Psi)
{
    GPSPoint P1;
    double latA = ToRadians(P.lat);
    double lonA = ToRadians(P.lon);
    double Bearing = ToRadians(Psi);
    double distance = 80;

    // �����µ�γ��
    double latB = asin(sin(latA) * cos(distance / EarthR) + cos(latA) * sin(distance / EarthR) * cos(Bearing));

    // �����µľ���
    double lonB = lonA + atan2(sin(Bearing) * sin(distance / EarthR) * cos(latA), cos(distance / EarthR) - sin(latA) * sin(latB));

    // �����ת��Ϊ����
    P1.lat = ToDegree(latB);
    P1.lon = ToDegree(lonB);

    GPSPoint P2 = this->ExPoints.at(CurPointID);
    //���������Ƕȵľ��� P�� P1���� P2������
    double Deg = angleWithLongitude(P, P1, P2);

    return Deg;
}

//����Ƕ�
double ZLControl::OutputDeg(double Bearing)
{
    //��������Χ��-35-35
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

//�������
int ZLControl::OutPutPeller(double distance,double R)
{
    //����ٷֱȣ�distance������GPS�����������ľ��룬R����Χ
    if (distance < R)
    {
        return 40;
        if (IsHavePoint())
        {
            CurPointID += 1;//����һ��
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

//����γ��תΪxy��m�Ƶ�λ
ZLControl::XYPoint ZLControl::gpsToXY(const GPSPoint& gpsPoint, const GPSPoint& referenceGPS)
{

    // ת��Ϊ������
    double latRad = ToRadians(gpsPoint.lat);// gpsPoint.latitude* (M_PI / 180.0);
    double lonRad = ToRadians(gpsPoint.lon);//gpsPoint.longitude * (M_PI / 180.0);
    double refLatRad = ToRadians(referenceGPS.lat);//referenceGPS.latitude * (M_PI / 180.0);
    double refLonRad = ToRadians(referenceGPS.lon);//referenceGPS.longitude * (M_PI / 180.0);

    // ��������ڳ�ʼ�ο���ľ�γ�Ȳ�ֵ
    double deltaLon = lonRad - refLonRad;
    double deltaLat = latRad - refLatRad;

    XYPoint xyPoint;
    // ʹ��������Ʒ����� xy ���꣬����������ڳ�ʼ�ο���Ĳ�ֵ
    xyPoint.y = EarthR * deltaLon;
    xyPoint.x = EarthR * log(tan(pi / 4.0 + latRad / 2.0) / tan(pi / 4.0 + refLatRad / 2.0));
    return xyPoint;
}

//��ȡxyϵ�е�
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

//��㵽�ߵĽǶ�
double ZLControl::CalAngle_Ex_ship(const GPSPoint& P, double Psi)
{
    //P �Ǵ�λ
    double psi = ToRadians(Psi);
    XYPoint SP = gpsToXY(P, this->Initialpoint); // ����

    //���ݴ������غ���30m�Ĵ��׵�
    XYPoint AP;
    AP.x = (SP.x + 0.0 * cos(-psi) - 30.0 * sin(-psi));
    AP.y = (SP.y + 0.0 * sin(-Psi) + 30.0 * cos(-Psi));

    //��ȡ������
    XYPoint BP;
    BP = gpsToXY(this->ExPoints.at(CurPointID), this->Initialpoint);

    // ����
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
    //P �Ǵ�λ
    double psi = ToRadians(Psi);

    //���ݴ������غ���30m�Ĵ��׵�
    XYPoint AP;
    AP.x = (P.x + 0.0 * cos(-psi) - 30.0 * sin(-psi));
    AP.y = (P.y + 0.0 * sin(-Psi) + 30.0 * cos(-Psi));

    //��ȡ������
    XYPoint BP;
    BP = gpsToXY(this->ExPoints.at(CurPointID), this->Initialpoint);

    // ����
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

// �����������������ĵ��
double ZLControl::dotProduct(const XYPoint& A, const XYPoint& B, const XYPoint& P)
{
    double vectorABx = B.x - A.x;
    double vectorABy = B.y - A.y;
    double vectorAPx = P.x - A.x;
    double vectorAPy = P.y - A.y;

    return vectorABx * vectorAPx + vectorABy * vectorAPy;
}

//����γ��תΪUTM m�Ƶ�λ
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

// UTM �ο���
ZLControl::XYPoint ZLControl::GPS2UTM_REF(const GPSPoint& gpsPoint, const GPSPoint& referenceGPS)
{
    ZLControl::XYPoint A = GpsToUtm(gpsPoint);
    ZLControl::XYPoint RA = GpsToUtm(referenceGPS);
    ZLControl::XYPoint Temp;
    Temp.x = A.x - RA.x;
    Temp.y = A.y - RA.y;
    return Temp;
}

//����
ZLControl::ControlInfo ZLControl::CalMainTest(const GPSPoint& ship, double Psi)
{

    //���������㡢��λ������Ǽ���н�
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
    //���������㡢��λ������Ǽ���н�
    double Angle = CalAngle_Ex_ship(ship, Psi);
    double dist = CalBearing(ship.lat, ship.lon, this->ExPoints.at(CurPointID).lat, this->ExPoints.at(CurPointID).lon);
    ControlInfo CInfo;
    CInfo.DeltaE = OutputDeg(Angle);
    CInfo.PropE = OutPutPeller(dist, 5);
    return CInfo;
}