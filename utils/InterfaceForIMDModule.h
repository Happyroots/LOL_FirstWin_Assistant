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
	int m_iThrusterSpdDown1;//+5��
	int m_iThrusterSpdDown2;//0��
	int m_iThrusterSpdDown3;//-5��

	int m_iThrusterSpdUp1;//+5��
	int m_iThrusterSpdUp2;//0��
	int m_iThrusterSpdUp3;//-5��

	int m_iTelegraphBellL1;//+5��
	int m_iTelegraphBellL2;//0��
	int m_iTelegraphBellL3;//-5��

	int m_iTelegraphBellR1;//+5��
	int m_iTelegraphBellR2;//0��
	int m_iTelegraphBellR3;//-5��

	int m_iRudderAngle1;//���
	int m_iRudderAngle2;//0��
	int m_iRudderAngle3;//�������

	int m_iAzimuthingPortCos1;//0�㵵
	int m_iAzimuthingPortCos2;//180�㵵

	int m_iAzimuthingPortSpd1;//���
	int m_iAzimuthingPortSpd2;//0��
	int m_iAzimuthingPortSpd3;//��С��

	int m_iAzimuthingStbdCos1;//0�㵵
	int m_iAzimuthingStbdCos2;//180�㵵

	int m_iAzimuthingStbdSpd1;//���
	int m_iAzimuthingStbdSpd2;//0��
	int m_iAzimuthingStbdSpd3;//��С��

	_tagIMDDEVICECONFIG()
	{
		m_iThrusterSpdDown1 = 0;//+5��
		m_iThrusterSpdDown2 = 0;//0��
		m_iThrusterSpdDown3 = 0;//-5��

		m_iThrusterSpdUp1 = 0;//+5��
		m_iThrusterSpdUp2 = 0;//0��
		m_iThrusterSpdUp3 = 0;//-5��

		m_iTelegraphBellL1 = 0;//+5��
		m_iTelegraphBellL2 = 0;//0��
		m_iTelegraphBellL3 = 0;//-5��

		m_iTelegraphBellR1 = 0;//+5��
		m_iTelegraphBellR2 = 0;//0��
		m_iTelegraphBellR3 = 0;//-5��

		m_iRudderAngle1 = 0;//���
		m_iRudderAngle2 = 0;//0��
		m_iRudderAngle3 = 0;//�������

		m_iAzimuthingPortCos1 = 0;//0�㵵
		m_iAzimuthingPortCos2 = 0;//180�㵵

		m_iAzimuthingPortSpd1 = 0;//���
		m_iAzimuthingPortSpd2 = 0;//0��
		m_iAzimuthingPortSpd3 = 0;//��С��

		m_iAzimuthingStbdCos1 = 0;//0�㵵
		m_iAzimuthingStbdCos2 = 0;//180�㵵

		m_iAzimuthingStbdSpd1 = 0;//���
		m_iAzimuthingStbdSpd2 = 0;//0��
		m_iAzimuthingStbdSpd3 = 0;//��С��
	}
}IMDDEVICECONFIG;


class CAllDataForConsole;
class CnComm;


//ͨ��IMDģ��ɼ����ӡ����֡����ơ�ȫ��ת���豸��Ϣ�Ľӿ���
class CInterfaceForIMDModule
{
public:	
	CInterfaceForIMDModule(CAllDataForConsole* pAllDataTempSub, BOOL bAutoStandbyCluchin);
	~CInterfaceForIMDModule();

public:	//��������
	CAllDataForConsole* m_pAllDataTempSub;			//���߳�ʹ�õ���ʱ����
	BOOL m_bAutoStandbyCluchin;

public:
	//��ȡ�����ļ�
	IMDDEVICECONFIG m_IMDDeviceConfig;
	void ReadIMDDeviceConig();	//��ȡIMD���豸�����ļ�

	//�ɼ����Ӷ��ֲ���ȫ��ת���豸��IMD�ɼ�ģ��
	CnComm* m_pCnComm;			//��������
	BOOL m_bConsoleOpen;		//�ڲ�״̬������̨�����Ƿ��Ѵ�
	BOOL m_bCloseConsole;		//�ⲿ���ƣ��Ƿ���Ҫ�رտ���̨	
	BOOL m_bIsFirstOpenCom;		//�Ƿ��һ�δ򿪴��ڣ���λ

	void DecodeAndEncodeIMDConsoleCardThread();				//Ϊ��Ӳ������̨�ṩ�̵߳��õĹ����ӿ�
	unsigned short AI[16];										//�����޷��Ŷ�����
	bool DecodeNewIMDControlCardThread(int iAddress);			//���߳��е���,����---IMD���豸	
	void DecodeChangeNewIMDToTrueValue();						//����IMD���豸������ת��
	int ChangeSteerToTrueAngle(int iValue1, int iValue2, int iValue3, int iChangeValue);
	int  ChangeTelegraphToTrueBell(int iValue1, int iValue2, int iValue3, int iChangeValue);	//�����豸---ת��Ϊ��ʵ��λ	//��ǰ����5��
	int  ChangeTelegraphToTrueBell_zzx(int iValue1, int iValue2, int iValue3, int iChangeValue);	//�����豸---ת��Ϊ��ʵ��λ	//��ǰ����10����
	int  ChangeAzimuthingToTrueCourse(int iValue1, int iValue2, int iChangeValue);				//ȫ��ת�豸����---ת��Ϊ��ʵ��λ
	float ChangeAzimuthingToTrueSpeed(int iValue1, int iValue2, int iValue3, int iChangeValue);	//ȫ��ת�豸�ٶ�---ת��Ϊ��ʵ��λ�İٷֱ�
																								//��Ϣ�����߳�
	HANDLE m_hThread_IMDDevice;
	DWORD  m_dwThreadID_IMDDevice;
	BOOL CreateThread_IMDDevice(int nPortID, CString sSetting);
	static void ThreadProc_IMDDevice(LPVOID pParam);
};

#endif //AFX_INTERFACEFORIMDMODULE_H__5B6DSWAD9_60DE_4FSC_BTY0_9DDDH18F2E6D__INCLUDED_
