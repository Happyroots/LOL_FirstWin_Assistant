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

	//���豸��ȫ��ת���棻���ӣ����Ƶ�
	m_bConsoleOpen = FALSE;
	m_bCloseConsole = FALSE;
	m_pCnComm = new CnComm;
	//m_bAlarmTimer = FALSE;
	//m_bAlarmOn = FALSE;
	//m_iDeviceNum = 0;
	//m_iLastDeviceNum = -1;//Ĭ��ֵ
	m_bIsFirstOpenCom = FALSE;

	//719�����豸
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

	ReadIMDDeviceConig();//��ȡ��IMD�豸�������ļ�
}

CInterfaceForIMDModule::~CInterfaceForIMDModule()
{
	//IMDӲ�����豸
	if (m_pCnComm != NULL)
	{
		m_pCnComm->Close();
		m_bConsoleOpen = FALSE;
		delete m_pCnComm;
	}

	if (m_hThread_IMDDevice != NULL)
	{
		DWORD ExitCode_Device_IMD;//�ر�GPS�߳�
		GetExitCodeThread(m_hThread_IMDDevice, &ExitCode_Device_IMD);
		TerminateThread(m_hThread_IMDDevice, ExitCode_Device_IMD);
	}
}

//Decode
bool CInterfaceForIMDModule::DecodeNewIMDControlCardThread(int iAddress)
{
	unsigned short registernumber;	//�Ĵ�������
	unsigned short crcvalue;
	unsigned char cTempData[1000];
	unsigned char cTempDataForRX[1000];
	unsigned short RXDatalength;

	registernumber = 0X0001;	//�Ĵ�������
	cTempData[0] = iAddress;	//��ַ��
	cTempData[1] = 0X03;		//������
	cTempData[2] = (unsigned char)(0X0000 >> 8);	//�Ĵ�����ʼ��ַ���ֽ�
	cTempData[3] = (unsigned char)(0X0000);			//�Ĵ�����ʼ��ַ���ֽ�

	cTempData[4] = (unsigned char)(registernumber >> 8);//�Ĵ����������ֽ�
	cTempData[5] = (unsigned char)(registernumber);		//�Ĵ����������ֽ�

	crcvalue = CalcCRC((BYTE*)cTempData, 6);		//����CRCУ����
	cTempData[6] = (unsigned char)(crcvalue);		//�Ĵ����������ֽ�
	cTempData[7] = (unsigned char)(crcvalue >> 8);	//�Ĵ����������ֽ�

	m_pCnComm->Write((void*)cTempData, 8);		//������д����
																		//m_pCnComm->Purge();		//2018.6.29
	RXDatalength = m_pCnComm->Read(cTempDataForRX, 7, 100); 	//�Ӵ��ڶ�����(��ʱ����Ϊ100ms��ԭ��Ϊ50ms��

																						//���ݰ������Լ���ַ�빦�����Լ��Ĵ�����������ȷ
	if ((RXDatalength == (5 + 2 * registernumber)) &&	//���ݳ���У��
		(cTempDataForRX[0] == iAddress) &&	//��ַУ��
		(cTempDataForRX[1] == 0X03) &&	//������У��
		(cTempDataForRX[2] == (2 * registernumber)))
	{
		crcvalue = CalcCRC((BYTE*)cTempDataForRX, (3 + 2 * registernumber)); 	//����CRCУ����

																				//CRCУ���ж��Ƿ���ȷ
		if ((cTempDataForRX[3 + 2 * registernumber] == (unsigned char)(crcvalue)) && (cTempDataForRX[4 + 2 * registernumber] == (unsigned char)(crcvalue >> 8)))
		{
			for (int i = 0; i < registernumber; i++)
			{
				AI[i + iAddress - 3] = cTempDataForRX[3 + 2 * i + 0] * 16 * 16 + cTempDataForRX[3 + 2 * i + 1];	//�������ݴ����AI������			
			}
		}
		/*else
		{
		AfxMessageBox(_T("111"));
		}*/
		return true;
	}
	else//�ж϶����豸�Ƿ����
	{
		//AfxMessageBox(_T("123"));
		return false;
	}
}

//IMD���豸����ת��
void CInterfaceForIMDModule::DecodeChangeNewIMDToTrueValue()
{
}

//���豸------------------------------------------------------------------------------
void CInterfaceForIMDModule::DecodeAndEncodeIMDConsoleCardThread()
{
	//20210622-LiuXiuwen-���޸ģ��ȷ�����ϰ������̨���ϵ磬�����������Ҫ��֤�ܲɼ�����Ӧ�����ݣ�
	//20210622-LiuXiuwen-���޸ģ��ɼ�������·�У����ǵ��кܶ��·ģ��ʵ�������п���û�У����У�


	//������---�����豸
	if ((DecodeNewIMDControlCardThread(0X03)) &&//������---��	
		(DecodeNewIMDControlCardThread(0X04)))	//������---��)
	{
		m_pAllDataTempSub->m_IMDDeviceInConsole.m_bThrusterDeviceInCosole = true;
		//�����������豸����ֵת������ֵ
		if (m_pAllDataTempSub->m_OSCtrlDevice.m_nThrusterIndCtrl == OSCTRLSRC::OSCTRLSRC_HARDWARE)
		{
			m_pAllDataTempSub->fThrusterOrderPitchPercent[0] = (float)ChangeTelegraphToTrueBell(m_IMDDeviceConfig.m_iThrusterSpdUp1, m_IMDDeviceConfig.m_iThrusterSpdUp2, m_IMDDeviceConfig.m_iThrusterSpdUp3, AI[0]) / 10.0f;//�������ϣ�ת��Ϊ��ʵ��λ�İٷֱ�---liujingjing_2018.3.19
			m_pAllDataTempSub->fThrusterOrderPitchPercent[1] = (float)ChangeTelegraphToTrueBell(m_IMDDeviceConfig.m_iThrusterSpdDown1, m_IMDDeviceConfig.m_iThrusterSpdDown2, m_IMDDeviceConfig.m_iThrusterSpdDown3, AI[1]) / 10.0f;//�������£�ת��Ϊ��ʵ��λ�İٷֱ�---liujingjing_2018.3.19
		}
	}

	//����---�����豸
	if (m_pAllDataTempSub->iEngineControlMode == 1) //0: Manual 1: throttle,2005-4-21 added for singapore demo
	{
		if ((DecodeNewIMDControlCardThread(0X05)) &&	//����---��
			(DecodeNewIMDControlCardThread(0X06)))	//����---��
		{
			m_pAllDataTempSub->m_IMDDeviceInConsole.m_bTelegraphDeviceInConsole = true;

			//���Ӷ����豸����ֵת������ֵ
			if (m_pAllDataTempSub->m_OSCtrlDevice.m_nTelegraphIndCtrl == OSCTRLSRC::OSCTRLSRC_HARDWARE)
			{
				if (m_pAllDataTempSub->iEngineControlMode == 1) //0: Manual 1: throttle,2005-4-21 added for singapore demo
				{
					if (m_pAllDataTempSub->m_nOwnShipCode == 65 || m_pAllDataTempSub->m_nOwnShipCode == 289
						|| m_pAllDataTempSub->m_nOwnShipCode == 290 || m_pAllDataTempSub->m_nOwnShipCode == 291)
					{
						m_pAllDataTempSub->nLeftBell = ChangeTelegraphToTrueBell_zzx(m_IMDDeviceConfig.m_iTelegraphBellL1, m_IMDDeviceConfig.m_iTelegraphBellL2, m_IMDDeviceConfig.m_iTelegraphBellL3, AI[2]);//������ת��Ϊ��ʵ��λ
						m_pAllDataTempSub->nRightBell = ChangeTelegraphToTrueBell_zzx(m_IMDDeviceConfig.m_iTelegraphBellR1, m_IMDDeviceConfig.m_iTelegraphBellR2, m_IMDDeviceConfig.m_iTelegraphBellR3, AI[3]);//�����ң�ת��Ϊ��ʵ��λ
					}
					else
					{
						m_pAllDataTempSub->nLeftBell = ChangeTelegraphToTrueBell(m_IMDDeviceConfig.m_iTelegraphBellL1, m_IMDDeviceConfig.m_iTelegraphBellL2, m_IMDDeviceConfig.m_iTelegraphBellL3, AI[2]);//������ת��Ϊ��ʵ��λ
						m_pAllDataTempSub->nRightBell = ChangeTelegraphToTrueBell(m_IMDDeviceConfig.m_iTelegraphBellR1, m_IMDDeviceConfig.m_iTelegraphBellR2, m_IMDDeviceConfig.m_iTelegraphBellR3, AI[3]);//�����ң�ת��Ϊ��ʵ��λ
					}

					if (m_pAllDataTempSub->ePitchCombinatorMode == PITCHMODE::PITCHMODE_COMBINATOR)//����ģʽ
					{
						if (m_pAllDataTempSub->nRightBell >= 0)
						{
							m_pAllDataTempSub->fStbdOrderPitchAng = m_pAllDataTempSub->nRightBell * m_pAllDataTempSub->fStbdAheadMaxPitchAng / 10.0f;//ʵ��ֵ
						}
						else
						{
							m_pAllDataTempSub->fStbdOrderPitchAng = m_pAllDataTempSub->nRightBell * m_pAllDataTempSub->fStbdSternMaxPitchAng / 10.0f;//ʵ��ֵ
						}
						if (m_pAllDataTempSub->nLeftBell >= 0)
						{
							m_pAllDataTempSub->fPortOrderPitchAng = m_pAllDataTempSub->nLeftBell * m_pAllDataTempSub->fPortAheadMaxPitchAng / 10.0f;//ʵ��ֵ
						}
						else
						{
							m_pAllDataTempSub->fPortOrderPitchAng = m_pAllDataTempSub->nLeftBell * m_pAllDataTempSub->fPortSternMaxPitchAng / 10.0f;//ʵ��ֵ
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
						else//�����������������£�ֱ��ͨ��ҡ�˳��ӿ���---added by liujingjing_2018.3.29
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

	//��---�����豸
	//if (m_pAllDataTempSub->eRudderAutoHandNFU == STEERINGMODE::STEERINGMODE_HAND)
	{
		if ((DecodeNewIMDControlCardThread(0X07)))
		{
			m_pAllDataTempSub->m_IMDDeviceInConsole.m_bRudderDeviceInConsole = true;

			//������豸����ֵת������ֵ
			if ((m_pAllDataTempSub->m_OSCtrlDevice.m_nRudderIndCtrl == OSCTRLSRC::OSCTRLSRC_HARDWARE) /*&& (!m_pAllDataTempSub->m_SelfCardDeviceInConsole.m_bQTRudderDevice)*/)
			{
				if (m_pAllDataTempSub->eRudderAutoHandNFU == STEERINGMODE::STEERINGMODE_HAND)//�涯��
				{
					m_pAllDataTempSub->iStbdOrderRudderAng = ChangeSteerToTrueAngle(m_IMDDeviceConfig.m_iRudderAngle1, m_IMDDeviceConfig.m_iRudderAngle2, m_IMDDeviceConfig.m_iRudderAngle3, AI[4]);//���ֶ�ǣ�ת��Ϊ��ʵ��λ
					m_pAllDataTempSub->iPortOrderRudderAng = m_pAllDataTempSub->iStbdOrderRudderAng;
					m_pAllDataTempSub->iStbdTrueOrderAng = m_pAllDataTempSub->iStbdOrderRudderAng;
					m_pAllDataTempSub->iPortTrueOrderAng = m_pAllDataTempSub->iStbdTrueOrderAng;
				}

				////Сƽ��IMDΪӦ����
				//if (m_pAllDataTempSub->eRudderAutoHandNFU == STEERINGMODE::STEERINGMODE_NFU)//Ӧ����
				//{
				//	int val1 = m_IMDDeviceConfig.m_iRudderAngle1;
				//	int val2 = m_IMDDeviceConfig.m_iRudderAngle2;
				//	int val3 = m_IMDDeviceConfig.m_iRudderAngle3;
				//	m_pAllDataTempSub->iStbdOrderRudderAng = ChangeSteerToTrueAngle(val1, val2, val3, AI[4]);//���ֶ�ǣ�ת��Ϊ��ʵ��λ
				//	m_pAllDataTempSub->iPortOrderRudderAng = m_pAllDataTempSub->iStbdOrderRudderAng;
				//	m_pAllDataTempSub->iStbdTrueOrderAng = m_pAllDataTempSub->iStbdOrderRudderAng;
				//	m_pAllDataTempSub->iPortTrueOrderAng = m_pAllDataTempSub->iStbdTrueOrderAng;
				//}

			}
		}
	}

	//ȫ��ת---�����豸
	if (m_pAllDataTempSub->m_PropellerType == PROPELLERTYPE::PROPELLER_AZI)//WaterJet
	{
		if ((DecodeNewIMDControlCardThread(0X08)) &&	//ȫ��ת�豸��---����			
			(DecodeNewIMDControlCardThread(0X09)))//ȫ��ת�豸��---�ٶ�
		{
			m_pAllDataTempSub->m_IMDDeviceInConsole.m_bAzimuthingPortDeviceInConsole = true;

			//��ȫ��ת�����豸����ֵת������ֵ
			if (m_pAllDataTempSub->m_OSCtrlDevice.m_nAzimuthingPortIndCtrl == OSCTRLSRC::OSCTRLSRC_HARDWARE)
			{
				if (m_pAllDataTempSub->m_PropellerType == PROPELLERTYPE::PROPELLER_AZI)//WaterJet---ˮ��
				{
					if (m_pAllDataTempSub->m_IMDDeviceInConsole.m_bAzimuthingPortDeviceInConsole == true)
					{
						m_pAllDataTempSub->fPortOrderBucketAng = ChangeAzimuthingToTrueCourse(m_IMDDeviceConfig.m_iAzimuthingPortCos1, m_IMDDeviceConfig.m_iAzimuthingPortCos2, AI[5]);//ת��Ϊ��ʵ��λ
						m_pAllDataTempSub->fPortOrderBucketSize = ChangeAzimuthingToTrueSpeed(m_IMDDeviceConfig.m_iAzimuthingPortSpd1, m_IMDDeviceConfig.m_iAzimuthingPortSpd2, m_IMDDeviceConfig.m_iAzimuthingPortSpd3, AI[6]);//ת��Ϊ��ʵ��λ�İٷֱ�
					}
				}
			}
		}

		if ((DecodeNewIMDControlCardThread(0X0A)) &&	//ȫ��ת�豸��---����			
			(DecodeNewIMDControlCardThread(0X0B)))//ȫ��ת�豸��---�ٶ�
		{
			m_pAllDataTempSub->m_IMDDeviceInConsole.m_bAzimuthingStbdDeviceInConsole = true;

			//��ȫ��ת�����豸����ֵת������ֵ
			if (m_pAllDataTempSub->m_OSCtrlDevice.m_nAzimuthingStbdIndCtrl == OSCTRLSRC::OSCTRLSRC_HARDWARE)
			{
				if (m_pAllDataTempSub->m_PropellerType == PROPELLERTYPE::PROPELLER_AZI)//WaterJet---ˮ��
				{
					if (m_pAllDataTempSub->m_IMDDeviceInConsole.m_bAzimuthingStbdDeviceInConsole == true)
					{
						m_pAllDataTempSub->fStbdOrderBucketAng = ChangeAzimuthingToTrueCourse(m_IMDDeviceConfig.m_iAzimuthingStbdCos1, m_IMDDeviceConfig.m_iAzimuthingStbdCos2, AI[7]);//ת��Ϊ��ʵ��λ
						m_pAllDataTempSub->fStbdOrderBucketSize = ChangeAzimuthingToTrueSpeed(m_IMDDeviceConfig.m_iAzimuthingStbdSpd1, m_IMDDeviceConfig.m_iAzimuthingStbdSpd2, m_IMDDeviceConfig.m_iAzimuthingStbdSpd3, AI[8]);//ת��Ϊ��ʵ��λ�İٷֱ�
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

int CInterfaceForIMDModule::ChangeTelegraphToTrueBell_zzx(int iValue1, int iValue2, int iValue3, int iChangeValue)//ת��Ϊ��ʵ��λ
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

int CInterfaceForIMDModule::ChangeTelegraphToTrueBell(int iValue1, int iValue2, int iValue3, int iChangeValue)//ת��Ϊ��ʵ��λ
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

int CInterfaceForIMDModule::ChangeAzimuthingToTrueCourse(int iValue1, int iValue2, int iChangeValue)//ת��Ϊ��ʵ��λ
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

float CInterfaceForIMDModule::ChangeAzimuthingToTrueSpeed(int iValue1, int iValue2, int iValue3, int iChangeValue)//ȫ��ת�豸�ٶ�---ת��Ϊ��ʵ��λ�İٷֱ�
{
	float fLightSpdLevel = 0.0f;//iValue1:��󵵣�iValue2:0����iValue3:��С��

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

void CInterfaceForIMDModule::ReadIMDDeviceConig()//��ȡ��IMD�豸�������ļ�
{
	std::string outFile;
	BaseData::GetModulePath(outFile);
	outFile += "\\IMDDeviceInfo.dat";

	std::ifstream fin(outFile, std::ios::in);
	if (fin.is_open())
	{
		fin >> m_IMDDeviceConfig.m_iThrusterSpdUp1;//+5��
		fin >> m_IMDDeviceConfig.m_iThrusterSpdUp2;//0��
		fin >> m_IMDDeviceConfig.m_iThrusterSpdUp3;//-5��

		fin >> m_IMDDeviceConfig.m_iThrusterSpdDown1;//+5��
		fin >> m_IMDDeviceConfig.m_iThrusterSpdDown2;//0��
		fin >> m_IMDDeviceConfig.m_iThrusterSpdDown3;//-5��

		fin >> m_IMDDeviceConfig.m_iTelegraphBellR1;//+5��
		fin >> m_IMDDeviceConfig.m_iTelegraphBellR2;//0��
		fin >> m_IMDDeviceConfig.m_iTelegraphBellR3;//-5��

		fin >> m_IMDDeviceConfig.m_iTelegraphBellL1;//+5��
		fin >> m_IMDDeviceConfig.m_iTelegraphBellL2;//0��
		fin >> m_IMDDeviceConfig.m_iTelegraphBellL3;//-5��

		fin >> m_IMDDeviceConfig.m_iRudderAngle1;//���
		fin >> m_IMDDeviceConfig.m_iRudderAngle2;//0��
		fin >> m_IMDDeviceConfig.m_iRudderAngle3;//�������

		fin >> m_IMDDeviceConfig.m_iAzimuthingPortCos1;//0�㵵
		fin >> m_IMDDeviceConfig.m_iAzimuthingPortCos2;//180�㵵

		fin >> m_IMDDeviceConfig.m_iAzimuthingPortSpd1;//���
		fin >> m_IMDDeviceConfig.m_iAzimuthingPortSpd2;//0��
		fin >> m_IMDDeviceConfig.m_iAzimuthingPortSpd3;//��С��

		fin >> m_IMDDeviceConfig.m_iAzimuthingStbdCos1;//0�㵵
		fin >> m_IMDDeviceConfig.m_iAzimuthingStbdCos2;//180�㵵

		fin >> m_IMDDeviceConfig.m_iAzimuthingStbdSpd1;//���
		fin >> m_IMDDeviceConfig.m_iAzimuthingStbdSpd2;//0��
		fin >> m_IMDDeviceConfig.m_iAzimuthingStbdSpd3;//��С��

		fin.close();
	}
}

BOOL CInterfaceForIMDModule::CreateThread_IMDDevice(int nPortID, CString sSetting)//Ϊ���豸���ڿ���һ���߳�
{
	if (m_bConsoleOpen == FALSE)
	{
		m_bConsoleOpen = m_pCnComm->Open(nPortID, sSetting);//�򿪴���		
	}

	if (m_bConsoleOpen == TRUE)
	{
		//����һ�߳����ڷ��ͽ��տ���̨����//��һ���߳��д������еĴ������ݣ���Sir˵�����ʡ�һ���̴߳�����̫�����ݡ�
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
				AfxMessageBox(_T("û�п����߳���Դ��"));
				m_pCnComm->Close();
				return FALSE;
			}
		}
	}
}

void CInterfaceForIMDModule::ThreadProc_IMDDevice(LPVOID pParam)//���豸ͬһ�߳�
{
	CInterfaceForIMDModule* pInterfaceForIMDModule = (CInterfaceForIMDModule*)pParam;

	static LARGE_INTEGER litmp_IMD;
	static LONGLONG qt1_IMD = 0;
	static LONGLONG qt2_IMD = 0;
	LARGE_INTEGER litmp1;
	QueryPerformanceFrequency(&litmp1);//���ʱ��Ƶ��  
	double dff = (double)litmp1.QuadPart;
	while (true)
	{
		QueryPerformanceCounter(&litmp_IMD);
		qt1_IMD = litmp_IMD.QuadPart;
		
		if (pInterfaceForIMDModule->m_bConsoleOpen == TRUE)
			pInterfaceForIMDModule->DecodeAndEncodeIMDConsoleCardThread();

		QueryPerformanceCounter(&litmp_IMD);
		qt2_IMD = litmp_IMD.QuadPart;
		float m_fOwnShipReceiveTime = 1000.0f*(qt2_IMD - qt1_IMD) / dff;	//����---�߳�����ÿ�ε�ʱ����
		qt2_IMD = qt1_IMD;
		float fSleep = 100 - m_fOwnShipReceiveTime;//100����
		if (fSleep >= 0)
			Sleep(fSleep);
	}
	if (pInterfaceForIMDModule->m_bConsoleOpen == FALSE)
		pInterfaceForIMDModule->m_pCnComm->Close();
}
