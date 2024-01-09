#pragma once
#include <cmath>
#define pi 3.14159265358979323846
#define EarthR 6371000.0 //����뾶
#include <vector>
#include <iostream>



class ZLControl
{
public:
	ZLControl();
	~ZLControl();

	struct GPSPoint
	{
		double lat;
		double lon;
		GPSPoint() {}
		GPSPoint(double lat1,double lon1)
		{
			this->lat = lat1;
			this->lon = lon1;
		}
		
	}GPSxy;

	size_t CurPointID;

	// ---------------------- ��γ���¼��� ----------------------------//
	//��ʼ��
	GPSPoint Initialpoint;

	// ��ǰ��λ
	GPSPoint CurShipPoint;

	//�Ƕ�ת����
	double ToRadians(double degree);
	
	//����ת�Ƕ�
	double ToDegree(double Radian);

	//����������γ��֮��ľ���
	double CalDistance(double lat1, double lon1, double lat2, double lon2);

	//����������γ��֮��ļн�
	double CalBearing(double lat1, double lon1, double lat2, double lon2);

	// �����������������ĵ��
	double dotProduct(const GPSPoint& A, const GPSPoint& B, const GPSPoint& P);

	// ���㾭γ�ȵ� P ����ھ����ߵļнǣ������ƣ�
	double angleWithLongitude(const GPSPoint& A, const GPSPoint& B, const GPSPoint& P);
	
	//������
	std::vector<GPSPoint> ExPoints;

	//�Լ��趨������

	//��ȡ�����������
	int NumPoints();

	//�ж��Ƿ���������
	bool IsHavePoint();

	//���������
	void AddPoints(GPSPoint P);
	void AddPoints(double lat,double lon);

	//ָ��λ�����������
	void AddPoints(size_t num,GPSPoint P);

	//ɾ��������
	void DelPoints();

	//ָ��λ��ɾ��������
	void DelPoints(size_t num);

	//�޸�ָ��λ�õ�������
	void VerPoints(size_t num,GPSPoint P);

	//��ӡ������
	void PrintPoints();

	//����Ԥ�ڵ��봬���ߵļн�
	double CalDegreOfPoint(GPSPoint P, double Psi);

	//����Ƕ�
	double OutputDeg(double Bearing);

	//�������
	int OutPutPeller(double distance, double R);

	// ---------------------- xy���¼��� ----------------------------//
	// 
	struct XYPoint
	{
		double x;
		double y;
		XYPoint() {}
		XYPoint(double lat1, double lon1)
		{
			this->x = lat1;
			this->y = lon1;
		}
	}Pxy;
	//����xy��
	std::vector<XYPoint> ExPoints_xy;

	//����γ��תΪxy��m�Ƶ�λ
	XYPoint gpsToXY(const GPSPoint& gpsPoint, const GPSPoint& referenceGPS);

	//����γ��תΪUTM m�Ƶ�λ
	XYPoint GpsToUtm(const GPSPoint& gpsPoint);

	// UTM �ο���
	XYPoint GPS2UTM_REF(const GPSPoint& gpsPoint, const GPSPoint& referenceGPS);

	//��ȡxyϵ�е�
	void CoverG2XY();

	//��㵽�ߵĽǶ�
	double CalAngle_Ex_ship(const GPSPoint& P, double Psi);
	double CalAngle_Ex_ship(const XYPoint& P, double Psi);

	// �������������ĵ��
	double dotProduct(const XYPoint& A, const XYPoint& B, const XYPoint& P);

	//���㺯��
	struct ControlInfo
	{
		double DeltaE;
		double PropE;
		ControlInfo() {}
		ControlInfo(double a, double b)
		{
			this->DeltaE = a;
			this->PropE = b;
		}
	};
	ControlInfo CalMainTest(const GPSPoint& ship, double Psi);
	ControlInfo CalMainTest(double ship_lat, double ship_lon, double Psi);

};

