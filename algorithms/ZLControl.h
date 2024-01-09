#pragma once
#include <cmath>
#define pi 3.14159265358979323846
#define EarthR 6371000.0 //地球半径
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

	// ---------------------- 经纬度下计算 ----------------------------//
	//初始点
	GPSPoint Initialpoint;

	// 当前船位
	GPSPoint CurShipPoint;

	//角度转弧度
	double ToRadians(double degree);
	
	//弧度转角度
	double ToDegree(double Radian);

	//计算两个经纬度之间的距离
	double CalDistance(double lat1, double lon1, double lat2, double lon2);

	//计算两个经纬度之间的夹角
	double CalBearing(double lat1, double lon1, double lat2, double lon2);

	// 计算两个地理坐标点的点积
	double dotProduct(const GPSPoint& A, const GPSPoint& B, const GPSPoint& P);

	// 计算经纬度点 P 相对于经度线的夹角（弧度制）
	double angleWithLongitude(const GPSPoint& A, const GPSPoint& B, const GPSPoint& P);
	
	//期望点
	std::vector<GPSPoint> ExPoints;

	//自己设定期望点

	//获取期望点的数量
	int NumPoints();

	//判断是否有期望点
	bool IsHavePoint();

	//添加期望点
	void AddPoints(GPSPoint P);
	void AddPoints(double lat,double lon);

	//指定位置添加期望点
	void AddPoints(size_t num,GPSPoint P);

	//删除期望点
	void DelPoints();

	//指定位置删除期望点
	void DelPoints(size_t num);

	//修改指定位置的期望点
	void VerPoints(size_t num,GPSPoint P);

	//打印期望点
	void PrintPoints();

	//计算预期点与船中线的夹角
	double CalDegreOfPoint(GPSPoint P, double Psi);

	//输出角度
	double OutputDeg(double Bearing);

	//输出车令
	int OutPutPeller(double distance, double R);

	// ---------------------- xy轴下计算 ----------------------------//
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
	//期望xy点
	std::vector<XYPoint> ExPoints_xy;

	//将经纬度转为xy轴m制单位
	XYPoint gpsToXY(const GPSPoint& gpsPoint, const GPSPoint& referenceGPS);

	//将经纬度转为UTM m制单位
	XYPoint GpsToUtm(const GPSPoint& gpsPoint);

	// UTM 参考点
	XYPoint GPS2UTM_REF(const GPSPoint& gpsPoint, const GPSPoint& referenceGPS);

	//获取xy系列点
	void CoverG2XY();

	//求点到线的角度
	double CalAngle_Ex_ship(const GPSPoint& P, double Psi);
	double CalAngle_Ex_ship(const XYPoint& P, double Psi);

	// 计算两个坐标点的点积
	double dotProduct(const XYPoint& A, const XYPoint& B, const XYPoint& P);

	//计算函数
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

