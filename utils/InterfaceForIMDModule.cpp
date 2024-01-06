// InterfaceForIMDModule.cpp: implementation of the CInterfaceForIMDModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "iostream"
#include <fstream>
#include <stdio.h>
using namespace std;

#include "cnComm.h"
#include "InterfaceForIMDModule.h"
#include "ConsoleInterfaceImpl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CInterfaceForIMDModule::CInterfaceForIMDModule(CAllDataForConsole* pAllDataTempSub, BOOL bAutoStandbyCluchin)
{
	m_pAllDataTempSub = pAllDataTempSub;
	m_bAutoStandbyCluchin = bAutoStandbyCluchin;

	//新设备：全回转；舵；车钟；侧推等
	m_bConsoleOpen = FALSE;
	m_bCloseConsole = FALSE;
	m_pCnComm = new CnComm;
	//m_bAlarmTimer = FALSE;
	//m_bAlarmOn = FALSE;
	//m_iDeviceNum = 0;
	//m_iLastDeviceNum = -1;//默认值
	m_bIsFirstOpenCom = FALSE;

	//719所新设备
	for (int i = 0; i < 50; i++)//50-???
	{
		//m_ArrayLastAO[i] = 0;
		//m_ArrayAOSubTemp[i] = 0X0000;
		//m_ArrayAISubTemp[i] = 0X0000;
	}
	for (int i = 0; i < 16; i++)
	{
		AI[i] = 0;
	}

	ReadIMDDeviceConig();//读取新IMD设备的配置文件
}

CInterfaceForIMDModule::~CInterfaceForIMDModule()
{
	//IMD硬件新设备
	if (m_pCnComm != NULL)
	{
		m_pCnComm->Close();
		m_bConsoleOpen = FALSE;
		delete m_pCnComm;
	}

	if (m_hThread_IMDDevice != NULL)
	{
		DWORD ExitCode_Device_IMD;//关闭GPS线程
		GetExitCodeThread(m_hThread_IMDDevice, &ExitCode_Device_IMD);
		TerminateThread(m_hThread_IMDDevice, ExitCode_Device_IMD);
	}
}

//Decode
bool CInterfaceForIMDModule::DecodeNewIMDControlCardThread(int iAddress)
{
	unsigned short registernumber;	//寄存器数量
	unsigned short crcvalue;
	unsigned char cTempData[1000];
	unsigned char cTempDataForRX[1000];
	unsigned short RXDatalength;

	registernumber = 0X0001;	//寄存器数量
	cTempData[0] = iAddress;	//地址码
	cTempData[1] = 0X03;		//功能码
	cTempData[2] = (unsigned char)(0X0000 >> 8);	//寄存器初始地址高字节
	cTempData[3] = (unsigned char)(0X0000);			//寄存器初始地址低字节

	cTempData[4] = (unsigned char)(registernumber >> 8);//寄存器数量高字节
	cTempData[5] = (unsigned char)(registernumber);		//寄存器数量低字节

	crcvalue = CalcCRC((BYTE*)cTempData, 6);		//计算CRC校验码
	cTempData[6] = (unsigned char)(crcvalue);		//寄存器数量低字节
	cTempData[7] = (unsigned char)(crcvalue >> 8);	//寄存器数量高字节

	m_pCnComm->Write((void*)cTempData, 8);		//往串口写数据
																		//m_pCnComm->Purge();		//2018.6.29
	RXDatalength = m_pCnComm->Read(cTempDataForRX, 7, 100); 	//从串口读数据(超时设置为100ms，原来为50ms）

																						//数据包长度以及地址码功能码以及寄存器数量都正确
	if ((RXDatalength == (5 + 2 * registernumber)) &&	//数据长度校验
		(cTempDataForRX[0] == iAddress) &&	//地址校验
		(cTempDataForRX[1] == 0X03) &&	//功能码校验
		(cTempDataForRX[2] == (2 * registernumber)))
	{
		crcvalue = CalcCRC((BYTE*)cTempDataForRX, (3 + 2 * registernumber)); 	//计算CRC校验码

																				//CRC校验判断是否正确
		if ((cTempDataForRX[3 + 2 * registernumber] == (unsigned char)(crcvalue)) && (cTempDataForRX[4 + 2 * registernumber] == (unsigned char)(crcvalue >> 8)))
		{
			for (int i = 0; i < registernumber; i++)
			{
				AI[i + iAddress - 3] = cTempDataForRX[3 + 2 * i + 0] * 16 * 16 + cTempDataForRX[3 + 2 * i + 1];	//读出数据存放在AI数组中			
			}
		}
		/*else
		{
		AfxMessageBox(_T("111"));
		}*/
		return true;
	}
	else//判断独立设备是否存在
	{
		//AfxMessageBox(_T("123"));
		return false;
	}
}

//IMD新设备数据转换
void CInterfaceForIMDModule::DecodeChangeNewIMDToTrueValue()
{
}

//新设备------------------------------------------------------------------------------
void CInterfaceForIMDModule::DecodeAndEncodeIMDConsoleCardThread()
{
	//20210622-LiuXiuwen-待修改：先发送练习，控制台再上电，这种情况下需要保证能采集到相应的数据；
	//20210622-LiuXiuwen-待修改：采集数据线路中，考虑到有很多电路模块实际配置中可能没有，，有；


	//侧推器---独立设备
	if ((DecodeNewIMDControlCardThread(0X03)) &&//侧推器---上	
		(DecodeNewIMDControlCardThread(0X04)))	//侧推器---下)
	{
		m_pAllDataTempSub->m_IMDDeviceInConsole.m_bThrusterDeviceInCosole = true;
		//侧推器独立设备：数值转换及赋值
		if (m_pAllDataTempSub->m_OSCtrlDevice.m_nThrusterIndCtrl == OSCTRLSRC::OSCTRLSRC_HARDWARE)
		{
			m_pAllDataTempSub->fThrusterOrderPitchPercent[0] = (float)ChangeTelegraphToTrueBell(m_IMDDeviceConfig.m_iThrusterSpdUp1, m_IMDDeviceConfig.m_iThrusterSpdUp2, m_IMDDeviceConfig.m_iThrusterSpdUp3, AI[0]) / 10.0f;//侧推器上：转换为真实档位的百分比---liujingjing_2018.3.19
			m_pAllDataTempSub->fThrusterOrderPitchPercent[1] = (float)ChangeTelegraphToTrueBell(m_IMDDeviceConfig.m_iThrusterSpdDown1, m_IMDDeviceConfig.m_iThrusterSpdDown2, m_IMDDeviceConfig.m_iThrusterSpdDown3, AI[1]) / 10.0f;//侧推器下：转换为真实档位的百分比---liujingjing_2018.3.19
		}
	}

	//车钟---独立设备
	if (m_pAllDataTempSub->iEngineControlMode == 1) //0: Manual 1: throttle,2005-4-21 added for singapore demo
	{
		if ((DecodeNewIMDControlCardThread(0X05)) &&	//车钟---左
			(DecodeNewIMDControlCardThread(0X06)))	//车钟---右
		{
			m_pAllDataTempSub->m_IMDDeviceInConsole.m_bTelegraphDeviceInConsole = true;

			//车钟独立设备：数值转换及赋值
			if (m_pAllDataTempSub->m_OSCtrlDevice.m_nTelegraphIndCtrl == OSCTRLSRC::OSCTRLSRC_HARDWARE)
			{
				if (m_pAllDataTempSub->iEngineControlMode == 1) //0: Manual 1: throttle,2005-4-21 added for singapore demo
				{
					if (m_pAllDataTempSub->m_nOwnShipCode == 65 || m_pAllDataTempSub->m_nOwnShipCode == 289
						|| m_pAllDataTempSub->m_nOwnShipCode == 290 || m_pAllDataTempSub->m_nOwnShipCode == 291)
					{
						m_pAllDataTempSub->nLeftBell = ChangeTelegraphToTrueBell_zzx(m_IMDDeviceConfig.m_iTelegraphBellL1, m_IMDDeviceConfig.m_iTelegraphBellL2, m_IMDDeviceConfig.m_iTelegraphBellL3, AI[2]);//车钟左：转换为真实档位
						m_pAllDataTempSub->nRightBell = ChangeTelegraphToTrueBell_zzx(m_IMDDeviceConfig.m_iTelegraphBellR1, m_IMDDeviceConfig.m_iTelegraphBellR2, m_IMDDeviceConfig.m_iTelegraphBellR3, AI[3]);//车钟右：转换为真实档位
					}
					else
					{
						m_pAllDataTempSub->nLeftBell = ChangeTelegraphToTrueBell(m_IMDDeviceConfig.m_iTelegraphBellL1, m_IMDDeviceConfig.m_iTelegraphBellL2, m_IMDDeviceConfig.m_iTelegraphBellL3, AI[2]);//车钟左：转换为真实档位
						m_pAllDataTempSub->nRightBell = ChangeTelegraphToTrueBell(m_IMDDeviceConfig.m_iTelegraphBellR1, m_IMDDeviceConfig.m_iTelegraphBellR2, m_IMDDeviceConfig.m_iTelegraphBellR3, AI[3]);//车钟右：转换为真实档位
					}

					if (m_pAllDataTempSub->ePitchCombinatorMode == PITCHMODE::PITCHMODE_COMBINATOR)//联动模式
					{
						if (m_pAllDataTempSub->nRightBell >= 0)
						{
							m_pAllDataTempSub->fStbdOrderPitchAng = m_pAllDataTempSub->nRightBell * m_pAllDataTempSub->fStbdAheadMaxPitchAng / 10.0f;//实际值
						}
						else
						{
							m_pAllDataTempSub->fStbdOrderPitchAng = m_pAllDataTempSub->nRightBell * m_pAllDataTempSub->fStbdSternMaxPitchAng / 10.0f;//实际值
						}
						if (m_pAllDataTempSub->nLeftBell >= 0)
						{
							m_pAllDataTempSub->fPortOrderPitchAng = m_pAllDataTempSub->nLeftBell * m_pAllDataTempSub->fPortAheadMaxPitchAng / 10.0f;//实际值
						}
						else
						{
							m_pAllDataTempSub->fPortOrderPitchAng = m_pAllDataTempSub->nLeftBell * m_pAllDataTempSub->fPortSternMaxPitchAng / 10.0f;//实际值
						}
					}

					//added by liujingjing_2018.3.22--------------------------------------------------------------------------------------------------------
					if ((m_pAllDataTempSub->m_nOwnShipCode >= 400) && (m_pAllDataTempSub->m_nOwnShipCode < 450))   //VSP Ship    //added by YYG_2005-8-20,
					{
						m_pAllDataTempSub->nLeftBell = m_pAllDataTempSub->nLeftBell;
						m_pAllDataTempSub->nRightBell = m_pAllDataTempSub->nRightBell;
					}
					else//Other ships: FPP, CPP
					{
						if ((m_pAllDataTempSub->m_PCBCardInConsole.m_bEngineCardInConsole == true) || (m_pAllDataTempSub->m_TouchPanelInConsole.m_bEngineCardInConsole == true))//added by liujingjing_2018.3.29
						{
							//Stbd
							if (m_pAllDataTempSub->nStbdEngineReadyOrStandby == ENGINE_STANDBY || m_pAllDataTempSub->nStbdEngineEmcyRunOrStop == ENGINE_EMERG_RUN)
							{
								m_pAllDataTempSub->nRightBell = m_pAllDataTempSub->nRightBell;
								//==================================================================
								if (m_pAllDataTempSub->nStbdEngineEmcyRunOrStop == ENGINE_EMERG_STOP)
								{
									m_pAllDataTempSub->nRightBell = 0;//Throttle Stop
								}
							}
							else if (m_pAllDataTempSub->nStbdEngineReadyOrStandby == ENGINE_FINISH || m_pAllDataTempSub->nStbdEngineEmcyRunOrStop == ENGINE_EMERG_STOP)
							{
								m_pAllDataTempSub->nRightBell = 0; //Throttle Stop
							}
							//Port
							if (m_pAllDataTempSub->nPortEngineReadyOrStandby == ENGINE_STANDBY || m_pAllDataTempSub->nPortEngineEmcyRunOrStop == ENGINE_EMERG_RUN)
							{
								m_pAllDataTempSub->nLeftBell = m_pAllDataTempSub->nLeftBell;
								//==================================================================
								if (m_pAllDataTempSub->nPortEngineEmcyRunOrStop == ENGINE_EMERG_STOP)
								{
									m_pAllDataTempSub->nLeftBell = 0;//Throttle Stop
								}
							}
							else if (m_pAllDataTempSub->nPortEngineReadyOrStandby == ENGINE_FINISH || m_pAllDataTempSub->nPortEngineEmcyRunOrStop == ENGINE_EMERG_STOP)
							{
								m_pAllDataTempSub->nLeftBell = 0;//Throttle Stop
							}
						}
						else//不存在主机板的情况下，直接通过摇杆车钟控制---added by liujingjing_2018.3.29
						{
							m_pAllDataTempSub->nLeftBell = m_pAllDataTempSub->nLeftBell;
							m_pAllDataTempSub->nRightBell = m_pAllDataTempSub->nRightBell;

							m_pAllDataTempSub->nStbdEngineReadyOrStandby = ENGINE_STANDBY;			//2: Ready, 1: Standy, 0: Finish
							m_pAllDataTempSub->bStbdClutchInOut = true;								//0: Out, 1:In
							m_pAllDataTempSub->nPortEngineReadyOrStandby = ENGINE_STANDBY;			//2: Ready, 1: Standy, 0: Finish
							m_pAllDataTempSub->bPortClutchInOut = true;								//0: Out, 1:In
							m_pAllDataTempSub->ePortEngineRunState = ENGINERUNSTATE::ENGINERUNSTATE_READY;
							m_pAllDataTempSub->eStbdEngineRunState = ENGINERUNSTATE::ENGINERUNSTATE_READY;
						}
						if (m_bAutoStandbyCluchin == true)
						{
							m_pAllDataTempSub->nLeftBell = m_pAllDataTempSub->nLeftBell;
							m_pAllDataTempSub->nRightBell = m_pAllDataTempSub->nRightBell;

							m_pAllDataTempSub->nStbdEngineReadyOrStandby = ENGINE_STANDBY;			//2: Ready, 1: Standy, 0: Finish
							m_pAllDataTempSub->bStbdClutchInOut = true;								//0: Out, 1:In
							m_pAllDataTempSub->nPortEngineReadyOrStandby = ENGINE_STANDBY;			//2: Ready, 1: Standy, 0: Finish
							m_pAllDataTempSub->bPortClutchInOut = true;								//0: Out, 1:In
							m_pAllDataTempSub->ePortEngineRunState = ENGINERUNSTATE::ENGINERUNSTATE_READY;
							m_pAllDataTempSub->eStbdEngineRunState = ENGINERUNSTATE::ENGINERUNSTATE_READY;
						}
					}

					m_pAllDataTempSub->nLeftBell2 = m_pAllDataTempSub->nLeftBell;
					m_pAllDataTempSub->nRightBell2 = m_pAllDataTempSub->nRightBell;
					////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				}
			}
		}
	}

	//舵---独立设备
	//if (m_pAllDataTempSub->eRudderAutoHandNFU == STEERINGMODE::STEERINGMODE_HAND)
	{
		if ((DecodeNewIMDControlCardThread(0X07)))
		{
			m_pAllDataTempSub->m_IMDDeviceInConsole.m_bRudderDeviceInConsole = true;

			//舵独立设备：数值转换及赋值
			if ((m_pAllDataTempSub->m_OSCtrlDevice.m_nRudderIndCtrl == OSCTRLSRC::OSCTRLSRC_HARDWARE) /*&& (!m_pAllDataTempSub->m_SelfCardDeviceInConsole.m_bQTRudderDevice)*/)
			{
				if (m_pAllDataTempSub->eRudderAutoHandNFU == STEERINGMODE::STEERINGMODE_HAND)//随动舵
				{
					m_pAllDataTempSub->iStbdOrderRudderAng = ChangeSteerToTrueAngle(m_IMDDeviceConfig.m_iRudderAngle1, m_IMDDeviceConfig.m_iRudderAngle2, m_IMDDeviceConfig.m_iRudderAngle3, AI[4]);//舵轮舵角：转换为真实档位
					m_pAllDataTempSub->iPortOrderRudderAng = m_pAllDataTempSub->iStbdOrderRudderAng;
					m_pAllDataTempSub->iStbdTrueOrderAng = m_pAllDataTempSub->iStbdOrderRudderAng;
					m_pAllDataTempSub->iPortTrueOrderAng = m_pAllDataTempSub->iStbdTrueOrderAng;
				}

				////小平岛IMD为应急舵
				//if (m_pAllDataTempSub->eRudderAutoHandNFU == STEERINGMODE::STEERINGMODE_NFU)//应急舵
				//{
				//	int val1 = m_IMDDeviceConfig.m_iRudderAngle1;
				//	int val2 = m_IMDDeviceConfig.m_iRudderAngle2;
				//	int val3 = m_IMDDeviceConfig.m_iRudderAngle3;
				//	m_pAllDataTempSub->iStbdOrderRudderAng = ChangeSteerToTrueAngle(val1, val2, val3, AI[4]);//舵轮舵角：转换为真实档位
				//	m_pAllDataTempSub->iPortOrderRudderAng = m_pAllDataTempSub->iStbdOrderRudderAng;
				//	m_pAllDataTempSub->iStbdTrueOrderAng = m_pAllDataTempSub->iStbdOrderRudderAng;
				//	m_pAllDataTempSub->iPortTrueOrderAng = m_pAllDataTempSub->iStbdTrueOrderAng;
				//}

			}
		}
	}

	//全回转---独立设备
	if (m_pAllDataTempSub->m_PropellerType == PROPELLERTYPE::PROPELLER_AZI)//WaterJet
	{
		if ((DecodeNewIMDControlCardThread(0X08)) &&	//全回转设备左---航向			
			(DecodeNewIMDControlCardThread(0X09)))//全回转设备左---速度
		{
			m_pAllDataTempSub->m_IMDDeviceInConsole.m_bAzimuthingPortDeviceInConsole = true;

			//左全回转独立设备：数值转换及赋值
			if (m_pAllDataTempSub->m_OSCtrlDevice.m_nAzimuthingPortIndCtrl == OSCTRLSRC::OSCTRLSRC_HARDWARE)
			{
				if (m_pAllDataTempSub->m_PropellerType == PROPELLERTYPE::PROPELLER_AZI)//WaterJet---水翼船
				{
					if (m_pAllDataTempSub->m_IMDDeviceInConsole.m_bAzimuthingPortDeviceInConsole == true)
					{
						m_pAllDataTempSub->fPortOrderBucketAng = ChangeAzimuthingToTrueCourse(m_IMDDeviceConfig.m_iAzimuthingPortCos1, m_IMDDeviceConfig.m_iAzimuthingPortCos2, AI[5]);//转换为真实档位
						m_pAllDataTempSub->fPortOrderBucketSize = ChangeAzimuthingToTrueSpeed(m_IMDDeviceConfig.m_iAzimuthingPortSpd1, m_IMDDeviceConfig.m_iAzimuthingPortSpd2, m_IMDDeviceConfig.m_iAzimuthingPortSpd3, AI[6]);//转换为真实档位的百分比
					}
				}
			}
		}

		if ((DecodeNewIMDControlCardThread(0X0A)) &&	//全回转设备右---航向			
			(DecodeNewIMDControlCardThread(0X0B)))//全回转设备右---速度
		{
			m_pAllDataTempSub->m_IMDDeviceInConsole.m_bAzimuthingStbdDeviceInConsole = true;

			//右全回转独立设备：数值转换及赋值
			if (m_pAllDataTempSub->m_OSCtrlDevice.m_nAzimuthingStbdIndCtrl == OSCTRLSRC::OSCTRLSRC_HARDWARE)
			{
				if (m_pAllDataTempSub->m_PropellerType == PROPELLERTYPE::PROPELLER_AZI)//WaterJet---水翼船
				{
					if (m_pAllDataTempSub->m_IMDDeviceInConsole.m_bAzimuthingStbdDeviceInConsole == true)
					{
						m_pAllDataTempSub->fStbdOrderBucketAng = ChangeAzimuthingToTrueCourse(m_IMDDeviceConfig.m_iAzimuthingStbdCos1, m_IMDDeviceConfig.m_iAzimuthingStbdCos2, AI[7]);//转换为真实档位
						m_pAllDataTempSub->fStbdOrderBucketSize = ChangeAzimuthingToTrueSpeed(m_IMDDeviceConfig.m_iAzimuthingStbdSpd1, m_IMDDeviceConfig.m_iAzimuthingStbdSpd2, m_IMDDeviceConfig.m_iAzimuthingStbdSpd3, AI[8]);//转换为真实档位的百分比
					}
				}
			}
		}
	}
}

int CInterfaceForIMDModule::ChangeSteerToTrueAngle(int iValue1, int iValue2, int iValue3, int iChangeValue)
{
	float fLightThrottleLevel = 0.0f;
	float fCos = 0;
	int iCos = 0;
	float fMax = m_pAllDataTempSub->iStbdMaxRudderAng;

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

int CInterfaceForIMDModule::ChangeTelegraphToTrueBell_zzx(int iValue1, int iValue2, int iValue3, int iChangeValue)//转换为真实档位
{
	float fLightThrottleLevel = 0.0f;
	int iBell = 0;

	if (iValue1 >= iValue3)
	{
		if (iChangeValue >= iValue2)
		{
			fLightThrottleLevel = 10.0*abs(iValue1 - iChangeValue) / (iValue1 - iValue2);
			iBell = 10.0 - 1.0 * ((int)(fLightThrottleLevel / 1.0 + 0.5));
		}
		else
		{
			fLightThrottleLevel = 10.0*(iValue3 - iChangeValue) / abs(iValue3 - iValue2);
			iBell = -10.0 - 1.0 * ((int)(fLightThrottleLevel / 1.0 - 0.5));
		}
	}
	else
	{
		if (iChangeValue <= iValue2)
		{
			fLightThrottleLevel = 10.0*abs(iValue1 - iChangeValue) / abs(iValue1 - iValue2);
			iBell = 10.0 - 1.0 * ((int)(fLightThrottleLevel / 1.0 + 0.5));
		}
		else
		{
			fLightThrottleLevel = 10.0*(iValue3 - iChangeValue) / abs(iValue3 - iValue2);
			iBell = -10.0 + 1.0 * ((int)(fLightThrottleLevel / 1.0 + 0.5));
		}
	}

	return iBell;
}

int CInterfaceForIMDModule::ChangeTelegraphToTrueBell(int iValue1, int iValue2, int iValue3, int iChangeValue)//转换为真实档位
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

int CInterfaceForIMDModule::ChangeAzimuthingToTrueCourse(int iValue1, int iValue2, int iChangeValue)//转换为真实档位
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

float CInterfaceForIMDModule::ChangeAzimuthingToTrueSpeed(int iValue1, int iValue2, int iValue3, int iChangeValue)//全回转设备速度---转换为真实档位的百分比
{
	float fLightSpdLevel = 0.0f;//iValue1:最大档，iValue2:0档，iValue3:最小档

	if (iValue1 >= iValue2)
	{
		fLightSpdLevel = 100.0*abs(iValue2 - iChangeValue) / (iValue1 - iValue2);
		if (abs(iChangeValue - iValue1) <= 10)
		{
			fLightSpdLevel = 100.0f;
		}
		else if (abs(iChangeValue - iValue2) <= 10)
		{
			fLightSpdLevel = 0.0f;
		}
		if (iChangeValue > iValue1)
		{
			fLightSpdLevel = 100.0f;
		}
		else if (iChangeValue < iValue2)
		{
			fLightSpdLevel = 0.0f;
		}
	}
	else
	{
		fLightSpdLevel = 100.0*abs(iValue2 - iChangeValue) / abs(iValue1 - iValue2);
		if (abs(iChangeValue - iValue1) <= 10)
		{
			fLightSpdLevel = 100.0f;
		}
		else if (abs(iChangeValue - iValue2) <= 10)
		{
			fLightSpdLevel = 0.0f;
		}
		if (iChangeValue < iValue1)
		{
			fLightSpdLevel = 100.0f;
		}
		else if (iChangeValue > iValue2)
		{
			fLightSpdLevel = 0.0f;
		}
	}
	fLightSpdLevel = fLightSpdLevel / 100.0f;
	/*if (iChangeValue <= iValue2)
	{
	fLightSpdLevel = abs(iChangeValue - iValue2) * 100.0f / abs(iValue1 - iValue2);

	if (abs(iChangeValue - iValue1) <= 10)
	{
	fLightSpdLevel = 100.0f;
	}
	}
	else if (iChangeValue > iValue2)
	{
	fLightSpdLevel = abs(iChangeValue - iValue3) * 100.0f / abs(iValue3 - iValue2);

	if (abs(iChangeValue - iValue3) <= 10)
	{
	fLightSpdLevel = 100.0f;
	}
	fLightSpdLevel = -fLightSpdLevel;
	}*/

	return fLightSpdLevel;
}

void CInterfaceForIMDModule::ReadIMDDeviceConig()//读取新IMD设备的配置文件
{
	std::string outFile;
	BaseData::GetModulePath(outFile);
	outFile += "\\IMDDeviceInfo.dat";

	std::ifstream fin(outFile, std::ios::in);
	if (fin.is_open())
	{
		fin >> m_IMDDeviceConfig.m_iThrusterSpdUp1;//+5档
		fin >> m_IMDDeviceConfig.m_iThrusterSpdUp2;//0档
		fin >> m_IMDDeviceConfig.m_iThrusterSpdUp3;//-5档

		fin >> m_IMDDeviceConfig.m_iThrusterSpdDown1;//+5档
		fin >> m_IMDDeviceConfig.m_iThrusterSpdDown2;//0档
		fin >> m_IMDDeviceConfig.m_iThrusterSpdDown3;//-5档

		fin >> m_IMDDeviceConfig.m_iTelegraphBellR1;//+5档
		fin >> m_IMDDeviceConfig.m_iTelegraphBellR2;//0档
		fin >> m_IMDDeviceConfig.m_iTelegraphBellR3;//-5档

		fin >> m_IMDDeviceConfig.m_iTelegraphBellL1;//+5档
		fin >> m_IMDDeviceConfig.m_iTelegraphBellL2;//0档
		fin >> m_IMDDeviceConfig.m_iTelegraphBellL3;//-5档

		fin >> m_IMDDeviceConfig.m_iRudderAngle1;//最大档
		fin >> m_IMDDeviceConfig.m_iRudderAngle2;//0档
		fin >> m_IMDDeviceConfig.m_iRudderAngle3;//反向最大档

		fin >> m_IMDDeviceConfig.m_iAzimuthingPortCos1;//0°档
		fin >> m_IMDDeviceConfig.m_iAzimuthingPortCos2;//180°档

		fin >> m_IMDDeviceConfig.m_iAzimuthingPortSpd1;//最大档
		fin >> m_IMDDeviceConfig.m_iAzimuthingPortSpd2;//0档
		fin >> m_IMDDeviceConfig.m_iAzimuthingPortSpd3;//最小档

		fin >> m_IMDDeviceConfig.m_iAzimuthingStbdCos1;//0°档
		fin >> m_IMDDeviceConfig.m_iAzimuthingStbdCos2;//180°档

		fin >> m_IMDDeviceConfig.m_iAzimuthingStbdSpd1;//最大档
		fin >> m_IMDDeviceConfig.m_iAzimuthingStbdSpd2;//0档
		fin >> m_IMDDeviceConfig.m_iAzimuthingStbdSpd3;//最小档

		fin.close();
	}
}

BOOL CInterfaceForIMDModule::CreateThread_IMDDevice(int nPortID, CString sSetting)//为各设备串口开启一新线程
{
	if (m_bConsoleOpen == FALSE)
	{
		m_bConsoleOpen = m_pCnComm->Open(nPortID, sSetting);//打开串口		
	}

	if (m_bConsoleOpen == TRUE)
	{
		//开启一线程用于发送接收控制台数据//在一个线程中处理所有的串口数据，刘Sir说不合适。一个线程处理了太多内容。
		if (m_hThread_IMDDevice == NULL)
		{
			if (m_hThread_IMDDevice = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0,
				(LPTHREAD_START_ROUTINE)ThreadProc_IMDDevice,
				this, 0, &m_dwThreadID_IMDDevice))
			{
				SetThreadPriority(m_hThread_IMDDevice, THREAD_PRIORITY_NORMAL);
				return TRUE;
			}
			else
			{
				AfxMessageBox(_T("没有可用线程资源！"));
				m_pCnComm->Close();
				return FALSE;
			}
		}
	}
}

void CInterfaceForIMDModule::ThreadProc_IMDDevice(LPVOID pParam)//各设备同一线程
{
	CInterfaceForIMDModule* pInterfaceForIMDModule = (CInterfaceForIMDModule*)pParam;

	static LARGE_INTEGER litmp_IMD;
	static LONGLONG qt1_IMD = 0;
	static LONGLONG qt2_IMD = 0;
	LARGE_INTEGER litmp1;
	QueryPerformanceFrequency(&litmp1);//获得时钟频率  
	double dff = (double)litmp1.QuadPart;
	while (true)
	{
		QueryPerformanceCounter(&litmp_IMD);
		qt1_IMD = litmp_IMD.QuadPart;
		
		if (pInterfaceForIMDModule->m_bConsoleOpen == TRUE)
			pInterfaceForIMDModule->DecodeAndEncodeIMDConsoleCardThread();

		QueryPerformanceCounter(&litmp_IMD);
		qt2_IMD = litmp_IMD.QuadPart;
		float m_fOwnShipReceiveTime = 1000.0f*(qt2_IMD - qt1_IMD) / dff;	//毫秒---线程运行每次的时间间隔
		qt2_IMD = qt1_IMD;
		float fSleep = 100 - m_fOwnShipReceiveTime;//100毫秒
		if (fSleep >= 0)
			Sleep(fSleep);
	}
	if (pInterfaceForIMDModule->m_bConsoleOpen == FALSE)
		pInterfaceForIMDModule->m_pCnComm->Close();
}
