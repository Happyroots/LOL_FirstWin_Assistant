// ConsoleInterfaceImpl.h: interface for the CConsoleInterfaceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSOLEINTERFACEIMPL_H__5B6DEREAD9_6DD03_4DS9C_B1DD0_9EDSDDD1282E6D__INCLUDED_)
#define AFX_CONSOLEINTERFACEIMPL_H__5B6DEREAD9_6DD03_4DS9C_B1DD0_9EDSDDD1282E6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "resource.h"
#include "ConsoleInterface.h"

#include "Exercise.h"

/////////////////////////////////////////////////////////////////////
#define RIGHTANCHOR		0	//右锚标识号
#define LEFTANCHOR		1	//左锚标识号
#define ASTERNANCHOR	2	//后锚标识号
#define MAXANCHORNUM	3	//锚总数量
#define MAXLINENUM		20
#define MAXTUGNUM		14
#define MAXANCHORNUM	3
#define MAXTHRUSTERNUM	4

enum{ ENGINE_FINISH, ENGINE_STANDBY, ENGINE_STOP };
enum{ NO_ENGINE_EMERG, ENGINE_EMERG_RUN, ENGINE_EMERG_STOP };
enum{ FIXED_PITCH_MODE, COMBINATOR_MODE, VAR_PITCH_MODE };
enum{ CPP_FOLLOW, CPP_NONFOLLOW, CPP_COMBINATOR };
enum{ CONTROL_BYENGINEROOM, CONTROL_BYFORE_WHEELHOUSE, CONTROL_BYAFT_WHEELHOUSE };

#define AUTO_COURSECONTROL	0
#define AUTO_TRACKCONTROL	1
#define AUTO_GYRO_SYNC		0
#define AUTO_MAGNETIC_SYNC	1
#define RUDDER_AD_ZERO		100
#define RUDDER_LIMITATION	40

enum enum_Device
{
	eRudder = 0,
	eEngine,
	ePitch,
	eThrusterCard,
	eLine,
	eTug,
	eAnchor,
	eSounder,
	eNaviLight,
	eDeckLight,
	eLog,
	eECHO,
	eFlagSignal,
	eSteer,
	eThrottle,
	eAzimuthing,
	eThrusterDevice
};

enum enum_Control
{
	eNull = 0,
	ePCBControl = 1,
	eSmallTouchControl,
	eBigTouchControl,
	eSoftwareControl
};

enum DeviceCommIndex	//各设备的固定串口号，将不同功能的设备串口号固定，有利于程序简单配置
{
	PCBCTRLCOMM = 1,	//原来2005年版本电路板
	IMDCTRLCOMM = 2,	//719所设备，采用电阻、电流等采集方式
	///COMPASSCOMM = 3,	//第1路罗经，三面舵
	COMPASSCOMM_1st = 3,	//第1路罗经，三面舵
	SELFCARDCOMM = 4,	//自制采集电路板，采集车钟、舵轮、侧推、全回转、围壳内的舵轮等设备信息
	///AUXCOMPASSCOMM = 5,	//第2路罗经，三面舵
	AUXCOMPASSCOMM_2nd = 5,	//第2路罗经，三面舵
	AUXCOMPASSCOMM_3rd = 6,	//第3路罗经，三面舵
	SZYHZMETERCOM = 61,	//深圳引航站 表头信息端口（收发风速 时间表头信息端口）
	SZYHZMETERCOM2 = 62,	//深圳引航站 表头信息端口（收发锦州表头信息端口）
	ENGINETOUCHCOMM = 30,	//中显触摸屏，主机板
	STEERTOUCHCOMM = 31,	//中显触摸屏，舵机板
	PITCHTOUCHCOMM = 32,	//中显触摸屏，螺距板
	THRUSTERTOUCHCOMM = 33,	//中显触摸屏，侧推器板
	ANCHORSOUNDCOMM = 34,	//中显触摸屏，锚声号板（竖版）
	VERTENGINECOMM = 35,	//中显触摸屏，主机板（竖版）
	TAORUDDERCOMM = 36,	//陶晶驰触摸屏，舵机板
	QTDRAFTCOMM = 37,	//中显触摸屏，QT吃水调节面板
	TAOENGINECOMM = 38,	//陶晶驰触摸屏，主机板
	AISCTRLCOMM = 50,	//Lowrance型号，AIS
	GPSCTRLCOMM = 51,	//波特率38400
	GPS2CTRLCOMM = 52,	//Navman版本的GPS，波特率4800
};
enum CONSOLE2022OPERTION//谁的申请硬件控制按钮生效
{
	Eneine = 0,
	Rudder = 1
};
enum CourseButtonIndex
{
	YAWINGOn, RudderOn, CounterRudderOn, RudderLimitOn, OffCourseOn, SetCourseOn, OffTrackOn, RadiusOn
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _tagIMDDeviceInConsole	//719所连接的设备是否存在
{
	bool m_bRudderDeviceInConsole : 1;			//舵轮独立设备
	bool m_bTelegraphDeviceInConsole : 1;		//车钟独立设备
	bool m_bThrusterDeviceInCosole : 1;			//侧推器独立设备
	bool m_bAzimuthingPortDeviceInConsole : 1;	//全回转独立设备左
	bool m_bAzimuthingStbdDeviceInConsole : 1;	//全回转独立设备右
	_tagIMDDeviceInConsole()
	{
		m_bRudderDeviceInConsole = false;
		m_bTelegraphDeviceInConsole = false;
		m_bThrusterDeviceInCosole = false;
		m_bAzimuthingPortDeviceInConsole = false;
		m_bAzimuthingStbdDeviceInConsole = false;
	}
}IMDDeviceInConsole;

typedef struct _tagSelfCardDeviceInConsole	//自制采集电路板线上的设备是否存在
{
	bool m_bRudderDevice : 1;			//舵轮独立设备
	bool m_bTelegraphDevice : 1;		//车钟独立设备
	bool m_bThrusterDevice : 1;			//侧推器独立设备
	bool m_bAzimuthingPortDevice : 1;	//全回转独立设备左
	bool m_bAzimuthingStbdDevice : 1;	//全回转独立设备右
	bool m_bQTRudderDevice : 1;			//潜艇围壳内舵轮板
	bool m_bCDYRudderDevice : 1;   //add by zzx 2021.04.14 for 操舵仪
	_tagSelfCardDeviceInConsole()
	{
		m_bRudderDevice = false;
		m_bTelegraphDevice = false;
		m_bThrusterDevice = false;
		m_bAzimuthingPortDevice = false;
		m_bAzimuthingStbdDevice = false;
		m_bQTRudderDevice = false;
		m_bCDYRudderDevice = false;   //
	}
}SelfCardDeviceInConsole;

typedef struct _tagPCBCardInConsole //各硬件面板是否存在
{
	bool m_bTugCardInConsole : 1;
	bool m_bThrusterCardInConsole : 1;
	bool m_bPilotCardInConsole : 1;		//舵系统：自动舵
	bool m_bRudderCardInConsole : 1;	//2018.5.22
	bool m_bEngineCardInConsole : 1;
	bool m_bLineCardInConsole : 1;
	bool m_bPitchCardInConsole : 1;
	bool m_bDepthECHOCardInConsole : 1;	
	bool m_bSpeedLOGCardInConsole : 1;	
	bool m_bNaviLightCardInConsole : 1;
	bool m_bDeckLightCardInConsole : 1;
	bool m_bAnchorCardInConsole : 1;
	bool m_bSoundCardInConsole : 1;
	_tagPCBCardInConsole()
	{
		m_bTugCardInConsole = false;
		m_bThrusterCardInConsole = false;
		m_bPilotCardInConsole = false;
		m_bRudderCardInConsole = false;
		m_bEngineCardInConsole = false;
		m_bLineCardInConsole = false;
		m_bPitchCardInConsole = false;
		m_bDepthECHOCardInConsole = false;
		m_bSpeedLOGCardInConsole = false;
		m_bNaviLightCardInConsole = false;
		m_bDeckLightCardInConsole = false;
		m_bAnchorCardInConsole = false;
		m_bSoundCardInConsole = false;
	}
}PCBCardInConsole;

typedef struct _tagTouchCardInConsole //各小触摸板是否存在
{
	bool m_bEngineCardInConsole : 1;	//主机
	bool m_bPilotCardInConsole : 1;		//舵系统：自动舵
	bool m_bRudderCardInConsole : 1;	//舵机板
	bool m_bThrusterCardInConsole : 1;	//侧推
	bool m_bPitchCardInConsole : 1;		//螺距
	bool m_bAnchorCardInConsole : 1;	//锚
	bool m_bSoundCardInConsole : 1;		//声号
	_tagTouchCardInConsole()
	{
		m_bEngineCardInConsole = false;
		m_bPilotCardInConsole = false;
		m_bRudderCardInConsole = false;
		m_bThrusterCardInConsole = false;
		m_bPitchCardInConsole = false;
		m_bAnchorCardInConsole = false;
		m_bSoundCardInConsole = false;
	}
}TouchPanelInConsole;

typedef struct _tagTouchScreenInConsole //各触摸屏设备是否存在
{
	bool m_bNaviLightPanelInConsole;	//航行灯控制面板
	bool m_bDeckLightPanelInConsole;	//甲板灯控制面板
	bool m_bSoundPanelInConsole;		//声号控制面板
	bool m_bSpeedLOGPanelInConsole;		//速度计程仪控制面板
	bool m_bDepthECHOPanelInConsole;	//测深仪控制面板
	bool m_bAnchorPanelInConsole;		//锚控制面板
	bool m_bTugPanelInConsole;			//矢量拖轮控制面板
	bool m_bLinePanelInConsole;			//缆绳控制面板
	bool m_bFlagPanelInConsole;			//号旗控制面板
	_tagTouchScreenInConsole()
	{
		m_bNaviLightPanelInConsole = false;
		m_bDeckLightPanelInConsole = false;
		m_bSoundPanelInConsole = false;
		m_bSpeedLOGPanelInConsole = false;
		m_bDepthECHOPanelInConsole = false;
		m_bAnchorPanelInConsole = false;
		m_bTugPanelInConsole = false;
		m_bLinePanelInConsole = false;
		m_bFlagPanelInConsole = false;
	}
}TouchScreenInConsole;

typedef struct _tagECDISPanelInConsole //各海图设备是否存在
{
	bool m_bPilotPanelInConsole;	//自动舵系统
	bool m_bRudderPanelInConsole;	//舵机板
	bool m_bNaviLightPanelInConsole;//航行灯
	bool m_bDeckLightPanelInConsole;//甲板灯
	bool m_bSoundPanelInConsole;	//声号
	bool m_bEnginePanelInConsole;	//主机
	bool m_bThrusterPanelInConsole;	//侧推
	bool m_bPitchPanelInConsole;	//螺距
	bool m_bAnchorPanelInConsole;	//锚
	bool m_bTugPanelInConsole;		//矢量拖轮
	bool m_bLinePanelInConsole;		//缆绳
	bool m_bFlagPanelInConsole;		//号旗
	bool m_bAlarmDepthInConsole;	//报警水深、测深仪
	_tagECDISPanelInConsole()
	{
		m_bPilotPanelInConsole = false;
		m_bRudderPanelInConsole = false;
		m_bNaviLightPanelInConsole = false;
		m_bDeckLightPanelInConsole = false;
		m_bSoundPanelInConsole = false;
		m_bEnginePanelInConsole = false;
		m_bThrusterPanelInConsole = false;
		m_bPitchPanelInConsole = false;
		m_bAnchorPanelInConsole = false;
		m_bTugPanelInConsole = false;
		m_bLinePanelInConsole = false;
		m_bFlagPanelInConsole = false;
		m_bAlarmDepthInConsole = false;
	}
	//...............................	
}ECDISPanelInConsole;

class COSCtrlDeviceTemp	//先暂时加上独立设备
{
public:
	OSCTRLSRC m_nEngineCtrl;		//主机面板控制
	OSCTRLSRC m_nPitchCtrl;			//螺距面板控制
	OSCTRLSRC m_nRudderCtrl;		//舵机面板控制
	//
	OSCTRLSRC m_nAnchorCtrl;		//锚面板控制
	OSCTRLSRC m_nLineCtrl;			//缆面板控制
	OSCTRLSRC m_nTugCtrl;			//矢量拖轮面板控制
	OSCTRLSRC m_nThrusterCtrl;		//侧推器面板控制
	OSCTRLSRC m_nNaviLightCtrl;		//航行灯面板控制
	OSCTRLSRC m_nDeckLightCtrl;		//甲板灯面板控制
	OSCTRLSRC m_nFlagCtrl;			//号旗面板控制
	OSCTRLSRC m_nSoundCtrl;			//声号面板控制
	OSCTRLSRC m_nLogCtrl;			//计程仪面板
	OSCTRLSRC m_nSounderCtrl;		//测深仪面板控制
	OSCTRLSRC m_nPilotCtrl;			//舵系统面板控制控制 
	//
	OSCTRLSRC m_nTelegraphIndCtrl;		//车钟独立设备控制
	OSCTRLSRC m_nRudderIndCtrl;			//舵轮独立设备控制 
	OSCTRLSRC m_nThrusterIndCtrl;		//侧推器独立设备控制 
	OSCTRLSRC m_nAzimuthingPortIndCtrl;	//全回转左独立设备控制 
	OSCTRLSRC m_nAzimuthingStbdIndCtrl;	//全回转右独立设备控制 
};

class COSCtrlInfoByIOS //主要为来自于教练员站的拖轮控制信息
{
public:
	COSCtrlInfoByIOS();
	~COSCtrlInfoByIOS();

	int TugHP[MAXTUGNUM];				//马力,档位
	int TugDir[MAXTUGNUM];				//矢量拖轮命令航向,档位	
	TUGOPSTATE TugOPState[MAXTUGNUM];	//拖轮状态														
};

class CAllDataForConsole
{
public:
	CAllDataForConsole();
	~CAllDataForConsole();

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CONSOLE2022OPERTION m_Console2022Opertion_take = Rudder;//默认为舵触摸屏控制硬件生效 当有主机触摸屏时主机生效
	bool m_Console2022Opertion = false;//是不是用当前硬件控制本船
	SYSLANG m_eLanguage;	//系统语言，由教练员设置整个系统运行的语言界面
	string m_sOwnShipMMSI;	//本船的MMIS码
	int  m_nOwnShipCode;	//当前船舶Code码	
	bool m_bTugCtrlByIOS;	//拖轮由教练员还是本船控制：false->由本船自身控制; true->由教练员控制，默认false
	bool m_bLineTowInfo;	//？？？
	PROPELLERTYPE m_PropellerType;      //推进器类型

	PCBCardInConsole m_PCBCardInConsole;			//2005版本PCB相关硬件面板设备是否存在
	TouchPanelInConsole m_TouchPanelInConsole;		//各小触摸板是否存在
	IMDDeviceInConsole m_IMDDeviceInConsole;		//通过IMD采集模块采集如719所各独立设备是否存在
	SelfCardDeviceInConsole m_SelfCardDeviceInConsole;	//通过自制采集板采集如车钟、舵轮、侧推、全回转、潜艇围壳内舵板邓设备是否存在
	TouchScreenInConsole m_TouchScreenInConsole;	//各触摸屏设备是否存在
	ECDISPanelInConsole m_ECDISPanelInConsole;		//各海图控制设备是否存在


	//本船故障设置及报警确认--------------------------------------------------------------------------------	
	CFailureInfo m_FailureInfo;			//本船所有故障及确认信息	
	bool m_bEngineFailureTemp;			//主机故障信息的接收变量
	bool m_bSteerFailureTemp;			//主机故障信息的接收变量
	//---------------------------------------------------------------------------------------
	
	//舵控制，包括111舵板和141舵轮
	//舵个数
	STEERINGMODE eRudderAutoHandNFU;			//0:Auto,1:FollowUp,2:NFU,3:NFUSplit	
	int  iRudderCount;							//舵的个数
	int  iStbdOrderRudderAng;					//命令舵角[0-200]
	int  iPortOrderRudderAng;					//命令舵角for Double Rudder by YYG_2005-4-21	
	int	 iStbdTrueOrderAng;						//转化以后的真实命令舵角_added by liujingjing_2017.10.10---[0,200]转为实际舵角
	int  iPortTrueOrderAng;						//转化以后的真实命令舵角_added by liujingjing_2017.10.10---[0,200]转为实际舵角
	int  iStbdMaxRudderAng;						//最大舵角
	int  iPortMaxRudderAng;						//最大舵角
	float fStbdRealRudderAng;					//实际右舵角值，单位"度" //2017_	
	float fPortRealRudderAng;					//实际左舵角值，单位"度"//
	int  iSteeringWheelRudder;					//added by YYG_2005-9-28,[-60,60]//141SteeringWheel

	//自动舵控制相关参数
	AUTOPILOTTYPE eAutoCourseOrTrack;			//0: AutoCourse, 1: AutoTrack	
	char  cAutoAlgorithm;						//自动舵算法选项 航向-1 航迹-2
	int   nAutoGyroOrMagn;						//0: Gyro (default: red),1: magn (green)
	float fYawingCoff;							//to 0-1.0  ---->K3:  积分系数
	float fRudderCoff;							//to 0-1.0---->K1:比例系数
	float fCountRudderCoff;						//to -10-10  ---> K2： 微      分系数
	float fRudderLimit;							//autopilot rudder limit inital data: -35-35
	float fOffCourseAlarm;						//autopilot off course alarm initial data: -20--20
	float fRadius;								//to 0-2000.0
	float fOffTrackAlarm;						//to 0-2000.0
	float fCourseSetting;						//设置航向,初值从主线程获取，即电罗经指示航向
	float fROT;									//转首角速度，单位"deg/min" //2017_修改单位
	float fHeading;								//送到控制台的航向，与电罗经航向会有些误差，单位"deg" //2017_
	float fMagneticHeading;						//磁罗经航向，与电罗经航向会有些误差，单位"deg"  //2017_
	float fAngP;
	float fAngR;
	
	//水深及速度测试相关：431测深及Log控制板	
	int   iWaterTrack;							//对地还是对水：0: Ground Track,1: Water Track
	int   iDopplerLogUnit;						//测速仪单位：0: Knots,1:m/s, 2: feets
	int   iEchoSounderUnit;						//水深单位：1:Fathom, 0: meter,2:feet	
	float fDepthLimitSetting;					//报警水深，单位"m"
	float fDepth;								//船位所在海域水深（是否考虑潮高，没有确定），单位"m"	
	float fOwnshipAlarmDepth;					//船舶报警水深设置值，单位"m"
	float fForeSpdV;							//获取船舶在船艏处的横向速度(Vec2f.y，单位"m/s)（属于对地速度）
	float fAftSpdV;								//获取船舶在船艉处的横向速度(Vec2f.y，单位"m/s)（属于对地速度）
	float fSpd;									//获取船舶合速度，单位"m/s"
	float fForeDepth;							//船艏龙骨下水深
	float fAftDepth;							//船艉龙骨下水深
	float fDistance;
	float fSpdU;//2022.11.20 gcc
	
	//航行灯控制/甲板灯控制/探照灯控制
	//bool  NavigationLamp[14];	//???			//0:White1,1:Red1,2:Port,3:Stbd,4:ForeMastHead,5:ForeAnchor,6:Red2,//7:White2,8:Red3,9:White3,10:MainMasterHead,11:SternLight,12:SternAnchor,13:Morse
	//bool  DeckLamp[8];						//0,1: ForeDeck Port, 2,3: ForeDeck Stbd, 4,5: AftDeck Port, 6,7: AftDeck Stbd

	//航行灯
	bool m_bForeAnchorLight : 1;//前锚灯
	bool m_bForeMastLight1 : 1;	//前桅灯1
	bool m_bForeMastLight2 : 1;	//前桅灯2
	bool m_bForeMastLight3 : 1;	//前桅灯3
	bool m_bPortSideLight : 1;	//左舷灯
	bool m_bStbdSideLight : 1;	//右舷灯
	bool m_bAftMastLight : 1;	//主桅灯
	bool m_bTowLight : 1;		//拖带灯
	bool m_bSternLight : 1;		//尾灯
	bool m_bAftAnchorLight : 1;	//后锚灯
	////zt add
	//bool m_bForeAnchorLight_back : 1;//备用前锚灯
	//bool m_bPortSideLight_back : 1;	 //备用左舷灯
	//bool m_bStbdSideLight_back : 1;	 //备用右舷灯
	//bool m_bAftMastLight_back : 1;	 //备用主桅灯
	//bool m_bSternLight_back : 1;	 //备用尾灯
	//bool m_bAftAnchorLight_back : 1; //备用后锚灯
	//bool m_bTopTrailLight : 1;		 //上航迹灯
	//bool m_bTopTrailLight_back : 1;	 //备用上航迹灯
	//bool m_bDownTrailLight : 1;		 //下航迹灯
	//bool m_bDownTrailLight_back : 1; //备用下航迹灯
	////zt add

	////环照灯
	////zt add
	//bool m_bPRALight3 : 1;			 //左舷最低端处红色环照灯
	//bool m_bSRALight3 : 1;			 //右舷最低端处红色环照灯
	//zt add
	bool m_bMRFLight0 : 1;		//中间红色闪光灯，在后桅顶端
	bool m_bMYFLight0 : 1;		//中间黄色闪光灯，在后桅顶端
	bool m_bMGALight1 : 1;		//中间绿色环照灯，在信号灯中间桅顶端
	bool m_bMRALight1 : 1;		//中间红色环照灯，在信号灯中间桅顶端
	bool m_bMRALight2 : 1;		//中间红色环照灯，在信号灯中间桅中间
	bool m_bMWALight2 : 1;		//中间白色环照灯，在信号灯中间桅中间
	bool m_bMYFLight2 : 1;		//中间黄色闪光灯，在信号灯中间桅中间
	bool m_bMRALight3 : 1;		//中间红色环照灯，在信号灯中间桅底端
	bool m_bMYFLight3 : 1;		//中间黄色闪光灯，在信号灯中间桅底端
	bool m_bMGALight3 : 1;		//中间绿色环照灯，在信号灯中间桅底端
	bool m_bPRALight1 : 1;		//左侧红色环照灯，在信号灯左侧桅顶端
	bool m_bPWALight1 : 1;		//左侧白色环照灯，在信号灯左侧桅顶端
	bool m_bPGALight1 : 1;		//左侧绿色环照灯，在信号灯左侧桅顶端
	bool m_bPRALight2 : 1;		//左侧红色环照灯，在信号灯左侧桅底端
	bool m_bPWALight2 : 1;		//左侧白色环照灯，在信号灯左侧桅底端
	bool m_bPGALight2 : 1;		//左侧绿色环照灯，在信号灯左侧桅底端
	bool m_bSRALight1 : 1;		//右侧红色环照灯，在信号灯右侧桅顶端
	bool m_bSWALight1 : 1;		//右侧白色环照灯，在信号灯右侧桅顶端
	bool m_bSGALight1 : 1;		//右侧绿色环照灯，在信号灯右侧桅顶端
	bool m_bSRALight2 : 1;		//右侧红色环照灯，在信号灯右侧桅底端
	bool m_bSWALight2 : 1;		//右侧白色环照灯，在信号灯右侧桅底端
	bool m_bSGALight2 : 1;		//右侧绿色环照灯，在信号灯右侧桅底端

	//甲板灯
	bool m_bForeCastlePort : 1;	//前左甲板灯
	bool m_bForeCastleStbd : 1;	//前右甲板灯
	bool m_bMainDeck1 : 1;		//中前部左甲板灯
	bool m_bMainDeck2 : 1;		//中前部右甲板灯
	bool m_bMainDeck3 : 1;		//中后部左甲板灯
	bool m_bMainDeck4 : 1;		//中后部右甲板灯
	bool m_bLunchAreaPort : 1;	//后左甲板灯
	bool m_bLunchAreaStbd : 1;	//后右甲板灯

	//探照灯
	bool m_bPortSearchLight : 1;				//左舷探照灯，也可用作莫尔斯灯
	bool m_bStbdSearchLight : 1;				//右舷探照灯，也可用作莫尔斯灯
	float m_fPortSLDirection;					//左舷探照灯水平方向角度，向北为0，顺时针为正，逆时针为负，单位"deg"
	float m_fPortSLPitchAng;					//左舷探照灯上下俯仰角度，水平为0，向上仰为正，向下俯为负，单位"deg"
	float m_fPortSLBeamAng;						//左舷探照灯光束范围角度，单位"deg"
	float m_fStbdSLDirection;					//右舷探照灯水平方向角度，向北为0，顺时针为正，逆时针为负，单位"deg"
	float m_fStbdSLPitchAng;					//右舷探照灯上下俯仰角度，水平为0，向上仰为正，向下俯为负，单位"deg"
	float m_fStbdSLBeamAng;						//右舷探照灯光束范围角度，单位"deg"

	//号旗信息---2018.5.22
	CFlagSignal m_FlagSignal;

	//号型信息
	bool m_bNormalAnchorShape : 1;				//普通锚泊，一个球体。
	bool m_bAgroundShape : 1;					//搁浅，垂直三个球体。
	bool m_bConstrainShape : 1;					//吃水受限，一个圆柱体。
	bool m_bNotUnderCmdShape : 1;				//失控，垂直两个球体。
	bool m_bRestrictUnderwayShape : 1;			//操限船在航，球菱球。
	bool m_bRestrictAnchorShape : 1;			//操限船锚泊，球菱球加锚球。
	bool m_bTowingUnderwayShape : 1;			//拖带船在航，一个菱形体。
	bool m_bFishTrawlingShape : 1;				//拖网渔船,尖端对接两个圆锥。
	bool m_bFishUnTrawlingShape : 1;			//非拖网渔船，尖端对接两个圆锥加一个向上的圆锥体。
	bool m_bDredgingShape : 1;					//疏浚船，球菱球加两个球体和两个菱形体。
	bool m_bMineClearUnderwayShape : 1;			//清除水雷作业在航，三个球体。
	bool m_bMineClearAnchorShape : 1;			//清除水雷作业锚泊，三个球体加锚球。
	
	//声号控制
	int  nFogAutoOrHand;						//0: auto, 1: hand!!!,默认手动
	bool bBowBell : 1;							//0表示没有，1表示正在操作，每次控制台发送数据来设置进行，一个数学模型解算周期判断完成操作并清除(在一个周期以后由控制台程序清除)，默认为0
	bool bSternGong : 1;						//0表示没有，1表示正在操作，每次控制台发送数据来设置进行，一个数学模型解算周期判断完成操作并清除(在一个周期以后由控制台程序清除)，默认为0	
	bool bHandSendFog : 1;						//是否手动按下雾笛（在SignalID为0的情况下）
	bool bAlarmSiren : 1;						//是否手动按下警笛
	char iAutoFogPeriod;						//60s,90s,120s
	char nAutoFogType;							//1->1短声; 2->2短声; 3->3短声;4->4短声;5->5短声;6->短长短;7->短短长;
												//8->一长声;9->2长声;10->长短短;11->长短短短;12->长长短;13->长长短短;14->长短长短;15->长短短短短;16->短长短短短长	

	//211主机控制板	
	int   iEngineControlMode;					//0: Manual 1: throttle,2005-4-21 added for singapore demo
	//主机个数
	int EngineSize;
	//右主机信息
	int   nRightBell;							//revised by YYG_2005-9-3, =[10,-10]-->Full Ahead->Full Astern ,old is[1-13]
	int   nRightBell2;							//revised by YYG_2005-9-3, =[10,-10]-->Full Ahead->Full Astern ,old is[1-13]
	int   nStbdEngineEmcyRunOrStop;				//1: EmcyRun, 2: EmcyStop, 0: Normal
	int   nStbdEngineReadyOrStandby;			//2: Ready, 1: Standy, 0: Finish
	bool  bStbdClutchInOut : 1;					//0: Out, 1:In 	
	float fStbdAirPressure;						//右主机的空气压力值	
	float fStbdOrderRPMPercent;					//主机命令转速,百分比
	float fStbdRealRPMPercent;					//主机实际转速,百分比
	float fStbdRealRPM;							//实际转速,实际值_revised by liujingjing_2017.11.08	
	ENGINERUNSTATE eStbdEngineRunState;			//主机运行状态:备车和完车		
	//左主机信息
	int   nLeftBell;							//revised by YYG_2005-9-3, =[10,-10]-->Full Ahead->Full Astern ,old is[1-13]
	int   nLeftBell2;							//revised by YYG_2005-9-3, =[10,-10]-->Full Ahead->Full Astern ,old is[1-13]
	int   nPortEngineEmcyRunOrStop;				//1: EmcyRun, 2: EmcyStop, 0: Normal
	int   nPortEngineReadyOrStandby;			//2: Ready, 1: Standy, 0: Finish
	bool  bPortClutchInOut : 1;					//0: Out, 1:In 		
	float fPortAirPressure;						//左主机的空气压力值		
	float fPortOrderRPMPercent;					//命令转速,百分比
	float fPortRealRPMPercent;					//实际转速,百分比
	float fPortRealRPM;							//实际转速,实际值_revised by liujingjing_2017.11.08
	ENGINERUNSTATE ePortEngineRunState;			//主机运行状态:备车和完车
		
	//Pitch控制或推进器控制		
	float fStbdAheadMaxPitchAng;				//正向最大Pitch角
	float fStbdSternMaxPitchAng;				//反向最大Pitch角
	float fStbdAheadMaxShaftRPM;				//正向最大轴转速
	float fStbdSternMaxShaftRPM;				//反向最大轴转速	
	float fPortAheadMaxPitchAng;				//正向最大Pitch角	
	float fPortSternMaxPitchAng;				//反向最大Pitch角
	float fPortAheadMaxShaftRPM;				//正向最大轴转速
	float fPortSternMaxShaftRPM;				//反向最大轴转速

	PITCHMODE ePitchCombinatorMode;				//螺距控制模式,0:固定螺距模式,1:组合螺距模式,2:变螺距模式		
	float fStbdOrderPitchAng;					//-30~30 for CPP Ship,命令螺距角实际值,单车钟以右为准,原来的fBladeAngle变量???		
	float fStbdOrderPitchAngPercent;			//-1.0~1.0 for CPP Ship,命令螺距百分比,单车钟以右为准
	float fStbdOrderEnginePitch;				//右主推进器螺距命令，单位""
	float fStbdRealPitchAng;					//右主推进器螺距实际值，单位""
	float fStbdRealPitchAngPer;					//螺距角特殊百分比，值-100.0f~+100.0f
	float fStbdRealShaftRPM;					//轴转速实际值
	float fStbdRealShaftRPMPer;					//轴转速特殊百分比，值-100.0f~+100.0f
	float fPortOrderPitchAng;					//-30~30 for CPP Ship???	
	float fPortOrderPitchAngPercent;			//-1.0~1.0 for CPP Ship,
	float fPortOrderEnginePitch;				//左主推进器螺距命令
	float fPortRealPitchAng;					//左主推进器螺距实际值
	float fPortRealPitchAngPer;					//螺距特殊百分比，单位“百分比”，值-100.0f~+100.0f
	float fPortRealShaftRPM;					//轴转速实际值
	float fPortRealShaftRPMPer;					//轴转速特殊百分比，值-100.0f~+100.0f

	float fStbdOrderPitchDir;
	float fPortOrderPitchDir;

	//螺距控制
	int iFishPitchPower;						//0:Power Off, 1:Power On,added by SXF_2009-5-25	
	int iCPPEmergStop;							//1: Stop, 0: Run
	int iCPPCombinCont;							//reserved for expanding	
	int iCPPControlMode;						//enum{CPP_FOLLOW,CPP_NONFOLLOW,CPP_COMBINATOR};
	int iCPPEngineControlPos;					//0:CONTROL_BYENGINEROOM,1:CONTROL_BYFORE_WHEELHOUSE,2:CONTROL_BYAFT_WHEELHOUSE

	//221侧推板控制板，221侧推板控制板_2，海事大学王思源模拟器用
	//目前假设最多有4个侧推器，原来模拟器中有两个侧推器：0表示首侧推，1表示尾侧推；DP模拟器中有三个侧推器：0->表示首侧推1; 1->表示首侧推2; 2->表示尾侧推
	int   iThrusterNum;								//侧推器的数量
	int   iThrusterPower[MAXTHRUSTERNUM];			//是否上电(艏侧推器)	
	float fThrusterMaxOrderPitch[MAXTHRUSTERNUM];	//侧推器对应的最大命令Pitch
	float fThrusterOrderPitchPercent[MAXTHRUSTERNUM];//艏侧推器上电后的控制，对应命令Pitch百分比
	float fThrusterRealPitchPercent[MAXTHRUSTERNUM]; //0-16，对应的实际Pitch百分比
	
	//锚控制	//331锚及声号控制板	
	BOOL bSternAnchor;							//前后锚：true->stern(后锚只有一个); false->port and stbd(前锚分为左右两个)
	ANCHOROPSTATE eStbdAnchorState;				//ANCHOROPSTATE,0: hold,1:letgo,2: heave,3:slack
	ANCHOROPSTATE ePortAnchorState;				//ANCHOROPSTATE,0: hold,1:letgo,2: heave,3:slack	
	ANCHOROPSTATE eSternAnchorState;			//ANCHOROPSTATE,0: hold,1:letgo,2: heave,3:slack
	//静态变量由本船类型决定（先决定锚类型，再由锚类型决定相关静态变量），动态变量由数学模型解算得到	
	float m_fAnchorForce[MAXANCHORNUM];			//锚力，由数学模型解算得出的动态数据，单位"t"	
	float m_fAnchorLength[MAXANCHORNUM];		//锚链长度，由数学模型解算得出的动态数据，单位"sh"	
	float m_fAnchorDirection[MAXANCHORNUM];		//锚方向
	float m_fAnchorSpeed[MAXANCHORNUM];			//锚速度

	//矢量Tugs相关信息，有些数据需要硬件面板和软件面板进行综合：321拖轮控制板；321拖轮控制板_2，在教练台中，与控制台中的双车钟只能有一个有效
	int iActiveTug;								//当前活动的拖轮序号
	int TugHP[MAXTUGNUM];						//马力,档位
	int TugDir[MAXTUGNUM];						//矢量拖轮命令航向,档位
	float TugForces[MAXTUGNUM];					//14个拖轮力信息	
	TUGOPSTATE TugOPState[MAXTUGNUM];			//拖轮状态		
	int TugON[MAXTUGNUM];						//矢量拖轮是否使用,Tug For Inter
	
	//Lines相关信息；311缆绳控制板	
	int   iActiveLine;							//当前活动的缆绳序号		
	int   LineCableSpeed[MAXLINENUM];			//20条缆绳速度控制: 
	float LineForces[MAXLINENUM];				//20个缆绳力信息
	float LineLengths[MAXLINENUM];				//20条缆绳长度信息	
	LINEOPSTATE LineOPState[MAXLINENUM];		//20条缆绳状态:0:Null,1:PayOut,2:PayOutStop;3:Slack3;4:Slack2;5:Slack1;6:Stop,7:Heave1,8:Heave2,9:Heave3

	//Azimuthing---全回转设备(水翼船)---new added by liujingjing_2018.3.15
	float fStbdOrderBucketAng;					//水翼船命令喷水方向---真实角度，单位"deg"
	float fStbdRealBucketAng;					//水翼船实际喷水方向，单位"deg"
	float fStbdOrderBucketSize;					//设置命令水翼船喷水设备开合程度---百分比
	float fStbdRealBucketSize;					//设置命令水翼船喷水设备开合程度---百分比
	float fPortOrderBucketAng;					//水翼船命令喷水方向---真实角度，单位"deg"
	float fPortRealBucketAng;					//水翼船实际喷水方向，单位"deg"
	float fPortOrderBucketSize;					//设置命令水翼船喷水设备开合程度---百分比
	float fPortRealBucketSize;					//设置命令水翼船喷水设备开合程度---百分比
		
	COSCtrlDeviceTemp m_OSCtrlDevice;			//设备控制信息
	COSCtrlInfoByTouchScr struDataForBigTouchCard;//大触摸屏信息
	COSCtrlInfoByECDIS    struDataForEcdisCard;	  //来自电子海图的控制信息
	COSCtrlInfoByIOS      struDataForIOSCtrl;	  //来自于教练员站的控制信息  
	bool m_bClearECDISCtrl;
	bool bTouchControlFog = false;//陶晶池触摸屏按钮控制雾笛


	//602FishGear控制(渔船控制)板_1
	BOOL bFishGearAlarmAck1;					//added by SXF_2009-5-25
	int iPortFishCablePower1;					//Rightest on console:0-> off, 1-> on 
	int iPortFishCableDrumCluth1;				//Rightest on console:0-> out, 1-> in 
	int iPortFishCableDrumBrake1;				//Rightest on console:0-> BrakeOff, 1-> on 
	int iPortFishOtterOperate1;					//0: Normal(Stop); 1: PayOut,2:WindUp,added by YYG_2009-5-24
	int iStbdFishCablePower1;					//Second from Rightest on console:0-> off, 1-> on 
	int iStbdFishCableDrumCluth1;				//Second from Rightest on console:0-> out, 1-> in 
	int iStbdFishCableDrumBrake1;				//Second from Rightest on console:0-> BrakeOff, 1-> on
	int iStbdFishOtterOperate1;					//0: Normal(Stop); 1: PayOut,2:WindUp,added by YYG_2009-5-24
	int iTrawlNetOperate1;						//0: Normal(Stop); 1: PayOut,2:WindUp,added by YYG_2009-5-24	
	int iPortFishCableControl;					//[-10.0,10.0],Stbd Fish Winch Control Board,manual operation winch position
	int iStbdFishCableControl;					//[-10.0,10.0],manual operation winch position
	int iFishWinchControlMode1;					//0: manual, 1: winch throttle control,added by YYG_2009-5-24
	float fPortFishWinchThrottle1;				//[-10.0,10.0],added by YYG_2009-5-24
	float fStbdFishWinchThrottle1;				//[-10.0,10.0],added by YYG_2009-5-24

	//602FishGear控制(渔船控制)板_2
	BOOL bFishGearAlarmAck2;					//added by SXF_2009-5-25
	int iPortFishCablePower2;					//Third From Rightest on console:0-> off, 1-> on 
	int iPortFishCableDrumCluth2;				//Third From Rightest on console:0-> out, 1-> in 
	int iPortFishCableDrumBrake2;				//Third From Rightest on console:0-> BrakeOff, 1-> on 
	int iPortFishOtterOperate2;					//0: Normal(Stop); 1: PayOut,2:WindUp,added by YYG_2009-5-24
	int iStbdFishCablePower2;					//Fourth from Rightest on console:0-> off, 1-> on 
	int iStbdFishCableDrumCluth2;				//Fourth from Rightest on console:0-> out, 1-> in 
	int iStbdFishCableDrumBrake2;				//Fourth from Rightest on console:0-> BrakeOff, 1-> on 
	int iStbdFishOtterOperate2;					//0: Normal(Stop); 1: PayOut,2:WindUp,added by YYG_2009-5-24
	int iTrawlNetOperate2;						//0: Normal(Stop); 1: PayOut,2:WindUp,added by YYG_2009-5-24	
	int iPortFishCableControl2;					//[-10.0,10.0],Port Fish Winch Control Board,added by YYG_2009-5-24
	int iStbdFishCableControl2;					//[-10.0,10.0],added by YYG_2009-5-24
	int iFishWinchControlMode2;					//0: manual, 1: winch throttle control,added by YYG_2009-5-24
	float fPortFishWinchThrottle2;				//[-10.0,10.0],added by YYG_2009-5-24
	float fStbdFishWinchThrottle2;				//[-10.0,10.0],added by YYG_2009-5-24
	
	//231左边水翼船控制
	BOOL hasWaterJetLeftControlDevice;
	float fLeftBucketOrder;						//[-1.0,1.0]
	int iLeftWJSpeedControl;					//1: +, 0: Stop, -1: -
	char cLeftWJStartOrStop;					//0: Stop,1:Start
	char cLeftWJSemiOn;							//0: off,1:on
	char cLeftWJMasterInq;						//0: normal,1: Master
	int iLeftWJBucketChangeDir;					//-1,0,1 
	int iLeftWJRudderChangeDir;					//-1,0,1 	
	float fLeftWJRudderAngle;
	float fLeftWJPitch;
	
	//water jet
	char cLeftWJAlarmAck;
	char cLeftWJAzimuthButtonStatus;			//0: normal,1: on
	char cLeftWJPitchButtonStatus;				//0: normal,1: on	
	float fLeftJoystickAzimuth;	
	float fLeftJoystickUpDown;

	char cRightWJAlarmAck;
	char cRightWJAzimuthButtonStatus;			//0: normal,1: on
	char cRightWJPitchButtonStatus;				//0: normal,1: on	
	float fRightJoystickAzimuth;
	float fRightJoystickUpDown;

	//241右边水翼船控制
	BOOL hasWaterJetRightControlDevice;
	float fRightBucketOrder;					//[-1.0,1.0]
	int iRightWJSpeedControl;					//1: +, 0: Stop, -1: -
	char cRightWJStartOrStop;					//0: Stop,1:Start
	bool bRightWJSemiOn : 1;					//0: off,1:on
	char cRightWJMasterInq;						//0: normal,1: Master
	float fRightWJRudderAngle;	
	float fRightWJPitch;
	int iRightWJBucketChangeDir;				//-1,0,1 
	int iRightWJRudderChangeDir;				//-1,0,1 

	//251RPM控制板；281双车钟控制板；281双车钟控制板_2，拖轮控制台用；281双车钟控制板_3，渔船用；281双车钟控制板_4，渔船用
	int bIsVSPTugRefresh;
	int iVSPTugStbdTelegraph;					//[10,-10]: full ahead-- full astern  
	int iVSPTugPortTelegraph;					//[10,-10]: full ahead-- full astern
	int iVSPTugRudder;							//[0-200]
	int iVSPTugNFU;								//1: left, 0: middle, 2: right
	int iVSPTugEmergControlActive;				//0: not active, 1: active
	int iVSPTugPortEngineStart;					//1: start,0: stop
	int iVSPTugStbdEngineStart;					//1: start,0: stop	
	int iVSPTugEmergPortEngineSpeed;
	int iVSPTugEmergStbdEngineSpeed;
	//川大潜艇馆上浮下潜变量
	int iCDQTRowComeUpDive = 7;//船艏水平舵
	int iCDQTSternComeUpDive = 7;//船艉水平舵

public:
	//位置姿态速度加速度等相关
	CGeoPoint ship_lon_lat;						//本船经纬度结构对象
	int OwnshipID;
	float fSpeed;								//船舶速度，单位"kn/s"，一般为>=0.0f，要和速度的方向配合使用	
	float fCourse;								//船舶航向
	/////////////////////////////////////////////////////////////////////////////////	
	float fWindDir;								//绝对风向，单位"deg"
	float fWindSpeed;							//绝对风速，单位"m/s"
	float fRelativeWindSpeed;					//相对风速，单位"m/s"
	float fRelativeWindDir;						//相对风向，单位"deg"
	float fDepthUnderKeel;						//船舶龙骨下水深（考虑到潮高），单位"m"
	float fTideHeight;							//潮高
	/////////////////////////////////////////////////////////////////////////////////
	
	int iSimuLocalYear;				//获取年份
	int iSimuLocalMonth;				//获取月份
	int iSimuLocalDate;				//获取日期
	int iSimuLocalDay;				//获取星期，0=星期日，1=星期一，...
	int iSimuLocalHour;
	int iSimuLocalMinute;
	int iSimuLocalSecond;				//获取秒
	float fDeltaSimulationTime;

	int MeterUTCYear;
	int MeterUTCMonth;
	int MeterUTCDate;
	int MeterUTCDay;
	int MeterUTCHour;
	int MeterUTCMinute;
	int MeterUTCSecond;

	int nSubmarineState = 0;
	int nSubmarineEngineType = 1;
	int nInHaubour = 1;
};

class CInterfaceForCompassRudder;
class CInterfaceForIMDModule;
class CInterfaceForConsole2020;
class CInterfaceForGPSAIS;
class CInterfaceForTouchPnlAnchorSound;
class CInterfaceForTouchPnlEngine;
class CInterfaceForTouchPnlEngineVert;
class CInterfaceForTouchPnlPitch;
class CInterfaceForTouchPnlQTDraft;
class CInterfaceForTouchPnlSteer;
class CInterfaceForTouchPnlThruster;
class CInterfaceForDevSimUnit;
class CInterfaceForConsole2005;
class CInterfaceForIOSControl;
class CInterfaceForECDISControl;
class CInterfaceForConsoleRudder2022;
class CInterfaceForConsoleEngine2022;
class InterfaceForConsoleJoyStick;
class CInterfaceForConsoleSZYHZMeter;
class CConsoleInterfaceImpl
{
	//外接接口
public:
	CWnd* m_pWnd; 	CConsoleInterfaceImpl();
	~CConsoleInterfaceImpl();

	void EncodeDataToConsole(COwnShip* pOwnShip, CEnviroment* pEnviroment, vector<CTargetShip>& vecpTargetShip);		//将本船信息编码到控制台状态信息,将目标船信息传入
	void DecodeDataFromConsole(COwnShip* pOwnShip, CEnviroment* pEnviroment);	//主线程调用，先将中间变量数据(需要数据保护)写入到主线程临时变量，再由主线程临时变量赋值到pOwnShip和pEnviroment对象中

	void DecodeAllDataFromAllConsoleCard();		//子线程中调用，解码	
	void DecodeAllControlTypeInfoInConsole();	//子线程中调用，由控制台中所有信息的存在信息，解析为设备的当前控制类型信息

	void SetAllOwnShipDataToAllConsoleCard(COwnShip* pOwnShip, CEnviroment* pEnviroment);	//主线程调用，将主线程中的数据写入到中间变量
	void EncodeAllOwnShipDataToAllConsoleCard();				//子线程中调用，将中间变量的数据写入到子线程的数据中，并数据编码到电路板的对应的输出字节中	

	void SetImplPrt(CConsoleInterfaceImpl* pSetImpl) { pConsoleInterfaceImpl = pSetImpl; }

public:
	//公共接口数据
	CCriticalSection m_CSForData;				//MFC临界区类对象,用来保护数据（主线程、子线程数据保护）
	CAllDataForConsole* m_pDataAllConsole;		//线程间的中间变量
	CAllDataForConsole* m_pAllDataTempMain;		//主线程使用的临时变量：主线程将
	CAllDataForConsole* m_pAllDataTempSub;		//子线程使用的临时变量
	CConsoleInterfaceImpl* pConsoleInterfaceImpl;

	bool m_bAutoStandbyCluchin;					//将车钟直接准备好
	DataComm::SYSLANG m_eLastLanguage;			//中文名切换变量

public:
	CInterfaceForIOSControl* m_pInterfaceForIOSControl;
	CInterfaceForECDISControl* m_pInterfaceForECDISControl;
	CInterfaceForConsole2005* m_pInterfaceForConsole2005;	//通过Console2005控制台电路系统采集车钟、侧推、舵轮等信息接口对象
	CInterfaceForDevSimUnit* m_pInterfaceForDevSimUnit;
	CInterfaceForConsole2020* m_pInterfaceForConsole2020;	//通过Console2020控制台电路系统采集车钟、侧推、舵轮等信息接口对象
	CInterfaceForIMDModule* m_pInterfaceForIMDModule;		//通过IMD模块采集车钟、侧推、舵轮等信息接口对象
	CInterfaceForCompassRudder* m_pInterfaceForCompassRudder1;	//第1路发送罗经三面舵信息的接口对象
	CInterfaceForCompassRudder* m_pInterfaceForCompassRudder2;	//第1路发送罗经三面舵信息的接口对象
	CInterfaceForCompassRudder* m_pInterfaceForCompassRudder3;	//第3路发送罗经三面舵信息的接口对象
	CInterfaceForTouchPnlAnchorSound* m_pInterfaceForTouchPnlAnchorSound;
	CInterfaceForTouchPnlEngine* m_pInterfaceForTouchPnlEngine;
	CInterfaceForTouchPnlEngineVert* m_pInterfaceForTouchPnlEngineVert;
	CInterfaceForTouchPnlPitch* m_pInterfaceForTouchPnlPitch;
	CInterfaceForTouchPnlQTDraft* m_pInterfaceForTouchPnlQTDraft;
	CInterfaceForTouchPnlSteer* m_pInterfaceForTouchPnlSteer;
	CInterfaceForTouchPnlThruster* m_pInterfaceForTouchPnlThruster;
	CInterfaceForGPSAIS* m_pInterfaceForGPSAIS;
	CInterfaceForConsoleRudder2022* m_pInterfaceForConsoleRudder2022;//触摸屏舵控制
	CInterfaceForConsoleEngine2022* m_pInterfaceForConsoleEngine2022;//触摸屏主机控制;
	InterfaceForConsoleJoyStick* m_pInterfaceForConsoleJoyStick;//摇杆控制
	CInterfaceForConsoleSZYHZMeter* m_pInterfaceForConsoleSZYHZMeter;//深圳引航站 表头信息更新
};
#endif 
