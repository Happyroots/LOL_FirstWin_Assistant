// InterfaceForIMDModule.h: interface for the CInterfaceForIMDModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACEFORIMDMODULE_H__5B6DSWAD9_60DE_4FSC_BTY0_9DDDH18F2E6D__INCLUDED_)
#define AFX_INTERFACEFORIMDMODULE_H__5B6DSWAD9_60DE_4FSC_BTY0_9DDDH18F2E6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>

#include "Exercise.h"

typedef struct _tagIMDDEVICECONFIG
{
	int m_iThrusterSpdDown1;//+5档
	int m_iThrusterSpdDown2;//0档
	int m_iThrusterSpdDown3;//-5档

	int m_iThrusterSpdUp1;//+5档
	int m_iThrusterSpdUp2;//0档
	int m_iThrusterSpdUp3;//-5档

	int m_iTelegraphBellL1;//+5档
	int m_iTelegraphBellL2;//0档
	int m_iTelegraphBellL3;//-5档

	int m_iTelegraphBellR1;//+5档
	int m_iTelegraphBellR2;//0档
	int m_iTelegraphBellR3;//-5档

	int m_iRudderAngle1;//最大档
	int m_iRudderAngle2;//0档
	int m_iRudderAngle3;//反向最大档

	int m_iAzimuthingPortCos1;//0°档
	int m_iAzimuthingPortCos2;//180°档

	int m_iAzimuthingPortSpd1;//最大档
	int m_iAzimuthingPortSpd2;//0档
	int m_iAzimuthingPortSpd3;//最小档

	int m_iAzimuthingStbdCos1;//0°档
	int m_iAzimuthingStbdCos2;//180°档

	int m_iAzimuthingStbdSpd1;//最大档
	int m_iAzimuthingStbdSpd2;//0档
	int m_iAzimuthingStbdSpd3;//最小档

	_tagIMDDEVICECONFIG()
	{
		m_iThrusterSpdDown1 = 0;//+5档
		m_iThrusterSpdDown2 = 0;//0档
		m_iThrusterSpdDown3 = 0;//-5档

		m_iThrusterSpdUp1 = 0;//+5档
		m_iThrusterSpdUp2 = 0;//0档
		m_iThrusterSpdUp3 = 0;//-5档

		m_iTelegraphBellL1 = 0;//+5档
		m_iTelegraphBellL2 = 0;//0档
		m_iTelegraphBellL3 = 0;//-5档

		m_iTelegraphBellR1 = 0;//+5档
		m_iTelegraphBellR2 = 0;//0档
		m_iTelegraphBellR3 = 0;//-5档

		m_iRudderAngle1 = 0;//最大档
		m_iRudderAngle2 = 0;//0档
		m_iRudderAngle3 = 0;//反向最大档

		m_iAzimuthingPortCos1 = 0;//0°档
		m_iAzimuthingPortCos2 = 0;//180°档

		m_iAzimuthingPortSpd1 = 0;//最大档
		m_iAzimuthingPortSpd2 = 0;//0档
		m_iAzimuthingPortSpd3 = 0;//最小档

		m_iAzimuthingStbdCos1 = 0;//0°档
		m_iAzimuthingStbdCos2 = 0;//180°档

		m_iAzimuthingStbdSpd1 = 0;//最大档
		m_iAzimuthingStbdSpd2 = 0;//0档
		m_iAzimuthingStbdSpd3 = 0;//最小档
	}
}IMDDEVICECONFIG;


class CAllDataForConsole;
class CnComm;


//通过IMD模块采集车钟、舵轮、侧推、全回转等设备信息的接口类
class CInterfaceForIMDModule
{
public:	
	CInterfaceForIMDModule(CAllDataForConsole* pAllDataTempSub, BOOL bAutoStandbyCluchin);
	~CInterfaceForIMDModule();

public:	//公共数据
	CAllDataForConsole* m_pAllDataTempSub;			//子线程使用的临时变量
	BOOL m_bAutoStandbyCluchin;

public:
	//读取配置文件
	IMDDEVICECONFIG m_IMDDeviceConfig;
	void ReadIMDDeviceConig();	//读取IMD新设备配置文件

	//采集车钟舵轮侧推全回转等设备的IMD采集模块
	CnComm* m_pCnComm;			//串口数据
	BOOL m_bConsoleOpen;		//内部状态：控制台连接是否已打开
	BOOL m_bCloseConsole;		//外部控制：是否需要关闭控制台	
	BOOL m_bIsFirstOpenCom;		//是否第一次打开串口，复位

	void DecodeAndEncodeIMDConsoleCardThread();				//为新硬件控制台提供线程调用的公共接口
	unsigned short AI[16];										//定义无符号短整型
	bool DecodeNewIMDControlCardThread(int iAddress);			//子线程中调用,解码---IMD新设备	
	void DecodeChangeNewIMDToTrueValue();						//所有IMD新设备的数据转换
	int ChangeSteerToTrueAngle(int iValue1, int iValue2, int iValue3, int iChangeValue);
	int  ChangeTelegraphToTrueBell(int iValue1, int iValue2, int iValue3, int iChangeValue);	//车钟设备---转换为真实档位	//向前向后各5档
	int  ChangeTelegraphToTrueBell_zzx(int iValue1, int iValue2, int iValue3, int iChangeValue);	//车钟设备---转换为真实档位	//向前向后各10档档
	int  ChangeAzimuthingToTrueCourse(int iValue1, int iValue2, int iChangeValue);				//全回转设备航向---转换为真实档位
	float ChangeAzimuthingToTrueSpeed(int iValue1, int iValue2, int iValue3, int iChangeValue);	//全回转设备速度---转换为真实档位的百分比
																								//信息处理线程
	HANDLE m_hThread_IMDDevice;
	DWORD  m_dwThreadID_IMDDevice;
	BOOL CreateThread_IMDDevice(int nPortID, CString sSetting);
	static void ThreadProc_IMDDevice(LPVOID pParam);
};

#endif //AFX_INTERFACEFORIMDMODULE_H__5B6DSWAD9_60DE_4FSC_BTY0_9DDDH18F2E6D__INCLUDED_
