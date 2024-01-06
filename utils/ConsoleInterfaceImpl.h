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
#define RIGHTANCHOR		0	//��ê��ʶ��
#define LEFTANCHOR		1	//��ê��ʶ��
#define ASTERNANCHOR	2	//��ê��ʶ��
#define MAXANCHORNUM	3	//ê������
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

enum DeviceCommIndex	//���豸�Ĺ̶����ںţ�����ͬ���ܵ��豸���ںŹ̶��������ڳ��������
{
	PCBCTRLCOMM = 1,	//ԭ��2005��汾��·��
	IMDCTRLCOMM = 2,	//719���豸�����õ��衢�����Ȳɼ���ʽ
	///COMPASSCOMM = 3,	//��1·�޾��������
	COMPASSCOMM_1st = 3,	//��1·�޾��������
	SELFCARDCOMM = 4,	//���Ʋɼ���·�壬�ɼ����ӡ����֡����ơ�ȫ��ת��Χ���ڵĶ��ֵ��豸��Ϣ
	///AUXCOMPASSCOMM = 5,	//��2·�޾��������
	AUXCOMPASSCOMM_2nd = 5,	//��2·�޾��������
	AUXCOMPASSCOMM_3rd = 6,	//��3·�޾��������
	SZYHZMETERCOM = 61,	//��������վ ��ͷ��Ϣ�˿ڣ��շ����� ʱ���ͷ��Ϣ�˿ڣ�
	SZYHZMETERCOM2 = 62,	//��������վ ��ͷ��Ϣ�˿ڣ��շ����ݱ�ͷ��Ϣ�˿ڣ�
	ENGINETOUCHCOMM = 30,	//���Դ�������������
	STEERTOUCHCOMM = 31,	//���Դ������������
	PITCHTOUCHCOMM = 32,	//���Դ��������ݾ��
	THRUSTERTOUCHCOMM = 33,	//���Դ���������������
	ANCHORSOUNDCOMM = 34,	//���Դ�������ê���Ű壨���棩
	VERTENGINECOMM = 35,	//���Դ������������壨���棩
	TAORUDDERCOMM = 36,	//�վ��۴������������
	QTDRAFTCOMM = 37,	//���Դ�������QT��ˮ�������
	TAOENGINECOMM = 38,	//�վ��۴�������������
	AISCTRLCOMM = 50,	//Lowrance�ͺţ�AIS
	GPSCTRLCOMM = 51,	//������38400
	GPS2CTRLCOMM = 52,	//Navman�汾��GPS��������4800
};
enum CONSOLE2022OPERTION//˭������Ӳ�����ư�ť��Ч
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
typedef struct _tagIMDDeviceInConsole	//719�����ӵ��豸�Ƿ����
{
	bool m_bRudderDeviceInConsole : 1;			//���ֶ����豸
	bool m_bTelegraphDeviceInConsole : 1;		//���Ӷ����豸
	bool m_bThrusterDeviceInCosole : 1;			//�����������豸
	bool m_bAzimuthingPortDeviceInConsole : 1;	//ȫ��ת�����豸��
	bool m_bAzimuthingStbdDeviceInConsole : 1;	//ȫ��ת�����豸��
	_tagIMDDeviceInConsole()
	{
		m_bRudderDeviceInConsole = false;
		m_bTelegraphDeviceInConsole = false;
		m_bThrusterDeviceInCosole = false;
		m_bAzimuthingPortDeviceInConsole = false;
		m_bAzimuthingStbdDeviceInConsole = false;
	}
}IMDDeviceInConsole;

typedef struct _tagSelfCardDeviceInConsole	//���Ʋɼ���·�����ϵ��豸�Ƿ����
{
	bool m_bRudderDevice : 1;			//���ֶ����豸
	bool m_bTelegraphDevice : 1;		//���Ӷ����豸
	bool m_bThrusterDevice : 1;			//�����������豸
	bool m_bAzimuthingPortDevice : 1;	//ȫ��ת�����豸��
	bool m_bAzimuthingStbdDevice : 1;	//ȫ��ת�����豸��
	bool m_bQTRudderDevice : 1;			//ǱͧΧ���ڶ��ְ�
	bool m_bCDYRudderDevice : 1;   //add by zzx 2021.04.14 for �ٶ���
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

typedef struct _tagPCBCardInConsole //��Ӳ������Ƿ����
{
	bool m_bTugCardInConsole : 1;
	bool m_bThrusterCardInConsole : 1;
	bool m_bPilotCardInConsole : 1;		//��ϵͳ���Զ���
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

typedef struct _tagTouchCardInConsole //��С�������Ƿ����
{
	bool m_bEngineCardInConsole : 1;	//����
	bool m_bPilotCardInConsole : 1;		//��ϵͳ���Զ���
	bool m_bRudderCardInConsole : 1;	//�����
	bool m_bThrusterCardInConsole : 1;	//����
	bool m_bPitchCardInConsole : 1;		//�ݾ�
	bool m_bAnchorCardInConsole : 1;	//ê
	bool m_bSoundCardInConsole : 1;		//����
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

typedef struct _tagTouchScreenInConsole //���������豸�Ƿ����
{
	bool m_bNaviLightPanelInConsole;	//���еƿ������
	bool m_bDeckLightPanelInConsole;	//�װ�ƿ������
	bool m_bSoundPanelInConsole;		//���ſ������
	bool m_bSpeedLOGPanelInConsole;		//�ٶȼƳ��ǿ������
	bool m_bDepthECHOPanelInConsole;	//�����ǿ������
	bool m_bAnchorPanelInConsole;		//ê�������
	bool m_bTugPanelInConsole;			//ʸ�����ֿ������
	bool m_bLinePanelInConsole;			//�����������
	bool m_bFlagPanelInConsole;			//����������
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

typedef struct _tagECDISPanelInConsole //����ͼ�豸�Ƿ����
{
	bool m_bPilotPanelInConsole;	//�Զ���ϵͳ
	bool m_bRudderPanelInConsole;	//�����
	bool m_bNaviLightPanelInConsole;//���е�
	bool m_bDeckLightPanelInConsole;//�װ��
	bool m_bSoundPanelInConsole;	//����
	bool m_bEnginePanelInConsole;	//����
	bool m_bThrusterPanelInConsole;	//����
	bool m_bPitchPanelInConsole;	//�ݾ�
	bool m_bAnchorPanelInConsole;	//ê
	bool m_bTugPanelInConsole;		//ʸ������
	bool m_bLinePanelInConsole;		//����
	bool m_bFlagPanelInConsole;		//����
	bool m_bAlarmDepthInConsole;	//����ˮ�������
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

class COSCtrlDeviceTemp	//����ʱ���϶����豸
{
public:
	OSCTRLSRC m_nEngineCtrl;		//����������
	OSCTRLSRC m_nPitchCtrl;			//�ݾ�������
	OSCTRLSRC m_nRudderCtrl;		//���������
	//
	OSCTRLSRC m_nAnchorCtrl;		//ê������
	OSCTRLSRC m_nLineCtrl;			//��������
	OSCTRLSRC m_nTugCtrl;			//ʸ������������
	OSCTRLSRC m_nThrusterCtrl;		//������������
	OSCTRLSRC m_nNaviLightCtrl;		//���е�������
	OSCTRLSRC m_nDeckLightCtrl;		//�װ��������
	OSCTRLSRC m_nFlagCtrl;			//����������
	OSCTRLSRC m_nSoundCtrl;			//����������
	OSCTRLSRC m_nLogCtrl;			//�Ƴ������
	OSCTRLSRC m_nSounderCtrl;		//������������
	OSCTRLSRC m_nPilotCtrl;			//��ϵͳ�����ƿ��� 
	//
	OSCTRLSRC m_nTelegraphIndCtrl;		//���Ӷ����豸����
	OSCTRLSRC m_nRudderIndCtrl;			//���ֶ����豸���� 
	OSCTRLSRC m_nThrusterIndCtrl;		//�����������豸���� 
	OSCTRLSRC m_nAzimuthingPortIndCtrl;	//ȫ��ת������豸���� 
	OSCTRLSRC m_nAzimuthingStbdIndCtrl;	//ȫ��ת�Ҷ����豸���� 
};

class COSCtrlInfoByIOS //��ҪΪ�����ڽ���Ավ�����ֿ�����Ϣ
{
public:
	COSCtrlInfoByIOS();
	~COSCtrlInfoByIOS();

	int TugHP[MAXTUGNUM];				//����,��λ
	int TugDir[MAXTUGNUM];				//ʸ�����������,��λ	
	TUGOPSTATE TugOPState[MAXTUGNUM];	//����״̬														
};

class CAllDataForConsole
{
public:
	CAllDataForConsole();
	~CAllDataForConsole();

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CONSOLE2022OPERTION m_Console2022Opertion_take = Rudder;//Ĭ��Ϊ�津��������Ӳ����Ч ��������������ʱ������Ч
	bool m_Console2022Opertion = false;//�ǲ����õ�ǰӲ�����Ʊ���
	SYSLANG m_eLanguage;	//ϵͳ���ԣ��ɽ���Ա��������ϵͳ���е����Խ���
	string m_sOwnShipMMSI;	//������MMIS��
	int  m_nOwnShipCode;	//��ǰ����Code��	
	bool m_bTugCtrlByIOS;	//�����ɽ���Ա���Ǳ������ƣ�false->�ɱ����������; true->�ɽ���Ա���ƣ�Ĭ��false
	bool m_bLineTowInfo;	//������
	PROPELLERTYPE m_PropellerType;      //�ƽ�������

	PCBCardInConsole m_PCBCardInConsole;			//2005�汾PCB���Ӳ������豸�Ƿ����
	TouchPanelInConsole m_TouchPanelInConsole;		//��С�������Ƿ����
	IMDDeviceInConsole m_IMDDeviceInConsole;		//ͨ��IMD�ɼ�ģ��ɼ���719���������豸�Ƿ����
	SelfCardDeviceInConsole m_SelfCardDeviceInConsole;	//ͨ�����Ʋɼ���ɼ��糵�ӡ����֡����ơ�ȫ��ת��ǱͧΧ���ڶ����豸�Ƿ����
	TouchScreenInConsole m_TouchScreenInConsole;	//���������豸�Ƿ����
	ECDISPanelInConsole m_ECDISPanelInConsole;		//����ͼ�����豸�Ƿ����


	//�����������ü�����ȷ��--------------------------------------------------------------------------------	
	CFailureInfo m_FailureInfo;			//�������й��ϼ�ȷ����Ϣ	
	bool m_bEngineFailureTemp;			//����������Ϣ�Ľ��ձ���
	bool m_bSteerFailureTemp;			//����������Ϣ�Ľ��ձ���
	//---------------------------------------------------------------------------------------
	
	//����ƣ�����111����141����
	//�����
	STEERINGMODE eRudderAutoHandNFU;			//0:Auto,1:FollowUp,2:NFU,3:NFUSplit	
	int  iRudderCount;							//��ĸ���
	int  iStbdOrderRudderAng;					//������[0-200]
	int  iPortOrderRudderAng;					//������for Double Rudder by YYG_2005-4-21	
	int	 iStbdTrueOrderAng;						//ת���Ժ����ʵ������_added by liujingjing_2017.10.10---[0,200]תΪʵ�ʶ��
	int  iPortTrueOrderAng;						//ת���Ժ����ʵ������_added by liujingjing_2017.10.10---[0,200]תΪʵ�ʶ��
	int  iStbdMaxRudderAng;						//�����
	int  iPortMaxRudderAng;						//�����
	float fStbdRealRudderAng;					//ʵ���Ҷ��ֵ����λ"��" //2017_	
	float fPortRealRudderAng;					//ʵ������ֵ����λ"��"//
	int  iSteeringWheelRudder;					//added by YYG_2005-9-28,[-60,60]//141SteeringWheel

	//�Զ��������ز���
	AUTOPILOTTYPE eAutoCourseOrTrack;			//0: AutoCourse, 1: AutoTrack	
	char  cAutoAlgorithm;						//�Զ����㷨ѡ�� ����-1 ����-2
	int   nAutoGyroOrMagn;						//0: Gyro (default: red),1: magn (green)
	float fYawingCoff;							//to 0-1.0  ---->K3:  ����ϵ��
	float fRudderCoff;							//to 0-1.0---->K1:����ϵ��
	float fCountRudderCoff;						//to -10-10  ---> K2�� ΢      ��ϵ��
	float fRudderLimit;							//autopilot rudder limit inital data: -35-35
	float fOffCourseAlarm;						//autopilot off course alarm initial data: -20--20
	float fRadius;								//to 0-2000.0
	float fOffTrackAlarm;						//to 0-2000.0
	float fCourseSetting;						//���ú���,��ֵ�����̻߳�ȡ�������޾�ָʾ����
	float fROT;									//ת�׽��ٶȣ���λ"deg/min" //2017_�޸ĵ�λ
	float fHeading;								//�͵�����̨�ĺ�������޾��������Щ����λ"deg" //2017_
	float fMagneticHeading;						//���޾���������޾��������Щ����λ"deg"  //2017_
	float fAngP;
	float fAngR;
	
	//ˮ��ٶȲ�����أ�431���Log���ư�	
	int   iWaterTrack;							//�Եػ��Ƕ�ˮ��0: Ground Track,1: Water Track
	int   iDopplerLogUnit;						//�����ǵ�λ��0: Knots,1:m/s, 2: feets
	int   iEchoSounderUnit;						//ˮ�λ��1:Fathom, 0: meter,2:feet	
	float fDepthLimitSetting;					//����ˮ���λ"m"
	float fDepth;								//��λ���ں���ˮ��Ƿ��ǳ��ߣ�û��ȷ��������λ"m"	
	float fOwnshipAlarmDepth;					//��������ˮ������ֵ����λ"m"
	float fForeSpdV;							//��ȡ�����ڴ������ĺ����ٶ�(Vec2f.y����λ"m/s)�����ڶԵ��ٶȣ�
	float fAftSpdV;								//��ȡ�����ڴ������ĺ����ٶ�(Vec2f.y����λ"m/s)�����ڶԵ��ٶȣ�
	float fSpd;									//��ȡ�������ٶȣ���λ"m/s"
	float fForeDepth;							//����������ˮ��
	float fAftDepth;							//����������ˮ��
	float fDistance;
	float fSpdU;//2022.11.20 gcc
	
	//���еƿ���/�װ�ƿ���/̽�յƿ���
	//bool  NavigationLamp[14];	//???			//0:White1,1:Red1,2:Port,3:Stbd,4:ForeMastHead,5:ForeAnchor,6:Red2,//7:White2,8:Red3,9:White3,10:MainMasterHead,11:SternLight,12:SternAnchor,13:Morse
	//bool  DeckLamp[8];						//0,1: ForeDeck Port, 2,3: ForeDeck Stbd, 4,5: AftDeck Port, 6,7: AftDeck Stbd

	//���е�
	bool m_bForeAnchorLight : 1;//ǰê��
	bool m_bForeMastLight1 : 1;	//ǰΦ��1
	bool m_bForeMastLight2 : 1;	//ǰΦ��2
	bool m_bForeMastLight3 : 1;	//ǰΦ��3
	bool m_bPortSideLight : 1;	//���ϵ�
	bool m_bStbdSideLight : 1;	//���ϵ�
	bool m_bAftMastLight : 1;	//��Φ��
	bool m_bTowLight : 1;		//�ϴ���
	bool m_bSternLight : 1;		//β��
	bool m_bAftAnchorLight : 1;	//��ê��
	////zt add
	//bool m_bForeAnchorLight_back : 1;//����ǰê��
	//bool m_bPortSideLight_back : 1;	 //�������ϵ�
	//bool m_bStbdSideLight_back : 1;	 //�������ϵ�
	//bool m_bAftMastLight_back : 1;	 //������Φ��
	//bool m_bSternLight_back : 1;	 //����β��
	//bool m_bAftAnchorLight_back : 1; //���ú�ê��
	//bool m_bTopTrailLight : 1;		 //�Ϻ�����
	//bool m_bTopTrailLight_back : 1;	 //�����Ϻ�����
	//bool m_bDownTrailLight : 1;		 //�º�����
	//bool m_bDownTrailLight_back : 1; //�����º�����
	////zt add

	////���յ�
	////zt add
	//bool m_bPRALight3 : 1;			 //������Ͷ˴���ɫ���յ�
	//bool m_bSRALight3 : 1;			 //������Ͷ˴���ɫ���յ�
	//zt add
	bool m_bMRFLight0 : 1;		//�м��ɫ����ƣ��ں�Φ����
	bool m_bMYFLight0 : 1;		//�м��ɫ����ƣ��ں�Φ����
	bool m_bMGALight1 : 1;		//�м���ɫ���յƣ����źŵ��м�Φ����
	bool m_bMRALight1 : 1;		//�м��ɫ���յƣ����źŵ��м�Φ����
	bool m_bMRALight2 : 1;		//�м��ɫ���յƣ����źŵ��м�Φ�м�
	bool m_bMWALight2 : 1;		//�м��ɫ���յƣ����źŵ��м�Φ�м�
	bool m_bMYFLight2 : 1;		//�м��ɫ����ƣ����źŵ��м�Φ�м�
	bool m_bMRALight3 : 1;		//�м��ɫ���յƣ����źŵ��м�Φ�׶�
	bool m_bMYFLight3 : 1;		//�м��ɫ����ƣ����źŵ��м�Φ�׶�
	bool m_bMGALight3 : 1;		//�м���ɫ���յƣ����źŵ��м�Φ�׶�
	bool m_bPRALight1 : 1;		//����ɫ���յƣ����źŵ����Φ����
	bool m_bPWALight1 : 1;		//����ɫ���յƣ����źŵ����Φ����
	bool m_bPGALight1 : 1;		//�����ɫ���յƣ����źŵ����Φ����
	bool m_bPRALight2 : 1;		//����ɫ���յƣ����źŵ����Φ�׶�
	bool m_bPWALight2 : 1;		//����ɫ���յƣ����źŵ����Φ�׶�
	bool m_bPGALight2 : 1;		//�����ɫ���յƣ����źŵ����Φ�׶�
	bool m_bSRALight1 : 1;		//�Ҳ��ɫ���յƣ����źŵ��Ҳ�Φ����
	bool m_bSWALight1 : 1;		//�Ҳ��ɫ���յƣ����źŵ��Ҳ�Φ����
	bool m_bSGALight1 : 1;		//�Ҳ���ɫ���յƣ����źŵ��Ҳ�Φ����
	bool m_bSRALight2 : 1;		//�Ҳ��ɫ���յƣ����źŵ��Ҳ�Φ�׶�
	bool m_bSWALight2 : 1;		//�Ҳ��ɫ���յƣ����źŵ��Ҳ�Φ�׶�
	bool m_bSGALight2 : 1;		//�Ҳ���ɫ���յƣ����źŵ��Ҳ�Φ�׶�

	//�װ��
	bool m_bForeCastlePort : 1;	//ǰ��װ��
	bool m_bForeCastleStbd : 1;	//ǰ�Ҽװ��
	bool m_bMainDeck1 : 1;		//��ǰ����װ��
	bool m_bMainDeck2 : 1;		//��ǰ���Ҽװ��
	bool m_bMainDeck3 : 1;		//�к���װ��
	bool m_bMainDeck4 : 1;		//�к��Ҽװ��
	bool m_bLunchAreaPort : 1;	//����װ��
	bool m_bLunchAreaStbd : 1;	//���Ҽװ��

	//̽�յ�
	bool m_bPortSearchLight : 1;				//����̽�յƣ�Ҳ������Ī��˹��
	bool m_bStbdSearchLight : 1;				//����̽�յƣ�Ҳ������Ī��˹��
	float m_fPortSLDirection;					//����̽�յ�ˮƽ����Ƕȣ���Ϊ0��˳ʱ��Ϊ������ʱ��Ϊ������λ"deg"
	float m_fPortSLPitchAng;					//����̽�յ����¸����Ƕȣ�ˮƽΪ0��������Ϊ�������¸�Ϊ������λ"deg"
	float m_fPortSLBeamAng;						//����̽�յƹ�����Χ�Ƕȣ���λ"deg"
	float m_fStbdSLDirection;					//����̽�յ�ˮƽ����Ƕȣ���Ϊ0��˳ʱ��Ϊ������ʱ��Ϊ������λ"deg"
	float m_fStbdSLPitchAng;					//����̽�յ����¸����Ƕȣ�ˮƽΪ0��������Ϊ�������¸�Ϊ������λ"deg"
	float m_fStbdSLBeamAng;						//����̽�յƹ�����Χ�Ƕȣ���λ"deg"

	//������Ϣ---2018.5.22
	CFlagSignal m_FlagSignal;

	//������Ϣ
	bool m_bNormalAnchorShape : 1;				//��ͨê����һ�����塣
	bool m_bAgroundShape : 1;					//��ǳ����ֱ�������塣
	bool m_bConstrainShape : 1;					//��ˮ���ޣ�һ��Բ���塣
	bool m_bNotUnderCmdShape : 1;				//ʧ�أ���ֱ�������塣
	bool m_bRestrictUnderwayShape : 1;			//���޴��ں���������
	bool m_bRestrictAnchorShape : 1;			//���޴�ê�����������ê��
	bool m_bTowingUnderwayShape : 1;			//�ϴ����ں���һ�������塣
	bool m_bFishTrawlingShape : 1;				//�����洬,��˶Խ�����Բ׶��
	bool m_bFishUnTrawlingShape : 1;			//�������洬����˶Խ�����Բ׶��һ�����ϵ�Բ׶�塣
	bool m_bDredgingShape : 1;					//�迣�����������������������������塣
	bool m_bMineClearUnderwayShape : 1;			//���ˮ����ҵ�ں����������塣
	bool m_bMineClearAnchorShape : 1;			//���ˮ����ҵê�������������ê��
	
	//���ſ���
	int  nFogAutoOrHand;						//0: auto, 1: hand!!!,Ĭ���ֶ�
	bool bBowBell : 1;							//0��ʾû�У�1��ʾ���ڲ�����ÿ�ο���̨�������������ý��У�һ����ѧģ�ͽ��������ж���ɲ��������(��һ�������Ժ��ɿ���̨�������)��Ĭ��Ϊ0
	bool bSternGong : 1;						//0��ʾû�У�1��ʾ���ڲ�����ÿ�ο���̨�������������ý��У�һ����ѧģ�ͽ��������ж���ɲ��������(��һ�������Ժ��ɿ���̨�������)��Ĭ��Ϊ0	
	bool bHandSendFog : 1;						//�Ƿ��ֶ�������ѣ���SignalIDΪ0������£�
	bool bAlarmSiren : 1;						//�Ƿ��ֶ����¾���
	char iAutoFogPeriod;						//60s,90s,120s
	char nAutoFogType;							//1->1����; 2->2����; 3->3����;4->4����;5->5����;6->�̳���;7->�̶̳�;
												//8->һ����;9->2����;10->���̶�;11->���̶̶�;12->������;13->�����̶�;14->���̳���;15->���̶̶̶�;16->�̳��̶̶̳�	

	//211�������ư�	
	int   iEngineControlMode;					//0: Manual 1: throttle,2005-4-21 added for singapore demo
	//��������
	int EngineSize;
	//��������Ϣ
	int   nRightBell;							//revised by YYG_2005-9-3, =[10,-10]-->Full Ahead->Full Astern ,old is[1-13]
	int   nRightBell2;							//revised by YYG_2005-9-3, =[10,-10]-->Full Ahead->Full Astern ,old is[1-13]
	int   nStbdEngineEmcyRunOrStop;				//1: EmcyRun, 2: EmcyStop, 0: Normal
	int   nStbdEngineReadyOrStandby;			//2: Ready, 1: Standy, 0: Finish
	bool  bStbdClutchInOut : 1;					//0: Out, 1:In 	
	float fStbdAirPressure;						//�������Ŀ���ѹ��ֵ	
	float fStbdOrderRPMPercent;					//��������ת��,�ٷֱ�
	float fStbdRealRPMPercent;					//����ʵ��ת��,�ٷֱ�
	float fStbdRealRPM;							//ʵ��ת��,ʵ��ֵ_revised by liujingjing_2017.11.08	
	ENGINERUNSTATE eStbdEngineRunState;			//��������״̬:�������공		
	//��������Ϣ
	int   nLeftBell;							//revised by YYG_2005-9-3, =[10,-10]-->Full Ahead->Full Astern ,old is[1-13]
	int   nLeftBell2;							//revised by YYG_2005-9-3, =[10,-10]-->Full Ahead->Full Astern ,old is[1-13]
	int   nPortEngineEmcyRunOrStop;				//1: EmcyRun, 2: EmcyStop, 0: Normal
	int   nPortEngineReadyOrStandby;			//2: Ready, 1: Standy, 0: Finish
	bool  bPortClutchInOut : 1;					//0: Out, 1:In 		
	float fPortAirPressure;						//�������Ŀ���ѹ��ֵ		
	float fPortOrderRPMPercent;					//����ת��,�ٷֱ�
	float fPortRealRPMPercent;					//ʵ��ת��,�ٷֱ�
	float fPortRealRPM;							//ʵ��ת��,ʵ��ֵ_revised by liujingjing_2017.11.08
	ENGINERUNSTATE ePortEngineRunState;			//��������״̬:�������공
		
	//Pitch���ƻ��ƽ�������		
	float fStbdAheadMaxPitchAng;				//�������Pitch��
	float fStbdSternMaxPitchAng;				//�������Pitch��
	float fStbdAheadMaxShaftRPM;				//���������ת��
	float fStbdSternMaxShaftRPM;				//���������ת��	
	float fPortAheadMaxPitchAng;				//�������Pitch��	
	float fPortSternMaxPitchAng;				//�������Pitch��
	float fPortAheadMaxShaftRPM;				//���������ת��
	float fPortSternMaxShaftRPM;				//���������ת��

	PITCHMODE ePitchCombinatorMode;				//�ݾ����ģʽ,0:�̶��ݾ�ģʽ,1:����ݾ�ģʽ,2:���ݾ�ģʽ		
	float fStbdOrderPitchAng;					//-30~30 for CPP Ship,�����ݾ��ʵ��ֵ,����������Ϊ׼,ԭ����fBladeAngle����???		
	float fStbdOrderPitchAngPercent;			//-1.0~1.0 for CPP Ship,�����ݾ�ٷֱ�,����������Ϊ׼
	float fStbdOrderEnginePitch;				//�����ƽ����ݾ������λ""
	float fStbdRealPitchAng;					//�����ƽ����ݾ�ʵ��ֵ����λ""
	float fStbdRealPitchAngPer;					//�ݾ������ٷֱȣ�ֵ-100.0f~+100.0f
	float fStbdRealShaftRPM;					//��ת��ʵ��ֵ
	float fStbdRealShaftRPMPer;					//��ת������ٷֱȣ�ֵ-100.0f~+100.0f
	float fPortOrderPitchAng;					//-30~30 for CPP Ship???	
	float fPortOrderPitchAngPercent;			//-1.0~1.0 for CPP Ship,
	float fPortOrderEnginePitch;				//�����ƽ����ݾ�����
	float fPortRealPitchAng;					//�����ƽ����ݾ�ʵ��ֵ
	float fPortRealPitchAngPer;					//�ݾ�����ٷֱȣ���λ���ٷֱȡ���ֵ-100.0f~+100.0f
	float fPortRealShaftRPM;					//��ת��ʵ��ֵ
	float fPortRealShaftRPMPer;					//��ת������ٷֱȣ�ֵ-100.0f~+100.0f

	float fStbdOrderPitchDir;
	float fPortOrderPitchDir;

	//�ݾ����
	int iFishPitchPower;						//0:Power Off, 1:Power On,added by SXF_2009-5-25	
	int iCPPEmergStop;							//1: Stop, 0: Run
	int iCPPCombinCont;							//reserved for expanding	
	int iCPPControlMode;						//enum{CPP_FOLLOW,CPP_NONFOLLOW,CPP_COMBINATOR};
	int iCPPEngineControlPos;					//0:CONTROL_BYENGINEROOM,1:CONTROL_BYFORE_WHEELHOUSE,2:CONTROL_BYAFT_WHEELHOUSE

	//221���ư���ư壬221���ư���ư�_2�����´�ѧ��˼Դģ������
	//Ŀǰ���������4����������ԭ��ģ��������������������0��ʾ�ײ��ƣ�1��ʾβ���ƣ�DPģ��������������������0->��ʾ�ײ���1; 1->��ʾ�ײ���2; 2->��ʾβ����
	int   iThrusterNum;								//������������
	int   iThrusterPower[MAXTHRUSTERNUM];			//�Ƿ��ϵ�(��������)	
	float fThrusterMaxOrderPitch[MAXTHRUSTERNUM];	//��������Ӧ���������Pitch
	float fThrusterOrderPitchPercent[MAXTHRUSTERNUM];//���������ϵ��Ŀ��ƣ���Ӧ����Pitch�ٷֱ�
	float fThrusterRealPitchPercent[MAXTHRUSTERNUM]; //0-16����Ӧ��ʵ��Pitch�ٷֱ�
	
	//ê����	//331ê�����ſ��ư�	
	BOOL bSternAnchor;							//ǰ��ê��true->stern(��êֻ��һ��); false->port and stbd(ǰê��Ϊ��������)
	ANCHOROPSTATE eStbdAnchorState;				//ANCHOROPSTATE,0: hold,1:letgo,2: heave,3:slack
	ANCHOROPSTATE ePortAnchorState;				//ANCHOROPSTATE,0: hold,1:letgo,2: heave,3:slack	
	ANCHOROPSTATE eSternAnchorState;			//ANCHOROPSTATE,0: hold,1:letgo,2: heave,3:slack
	//��̬�����ɱ������;������Ⱦ���ê���ͣ�����ê���;�����ؾ�̬����������̬��������ѧģ�ͽ���õ�	
	float m_fAnchorForce[MAXANCHORNUM];			//ê��������ѧģ�ͽ���ó��Ķ�̬���ݣ���λ"t"	
	float m_fAnchorLength[MAXANCHORNUM];		//ê�����ȣ�����ѧģ�ͽ���ó��Ķ�̬���ݣ���λ"sh"	
	float m_fAnchorDirection[MAXANCHORNUM];		//ê����
	float m_fAnchorSpeed[MAXANCHORNUM];			//ê�ٶ�

	//ʸ��Tugs�����Ϣ����Щ������ҪӲ����������������ۺϣ�321���ֿ��ư壻321���ֿ��ư�_2���ڽ���̨�У������̨�е�˫����ֻ����һ����Ч
	int iActiveTug;								//��ǰ����������
	int TugHP[MAXTUGNUM];						//����,��λ
	int TugDir[MAXTUGNUM];						//ʸ�����������,��λ
	float TugForces[MAXTUGNUM];					//14����������Ϣ	
	TUGOPSTATE TugOPState[MAXTUGNUM];			//����״̬		
	int TugON[MAXTUGNUM];						//ʸ�������Ƿ�ʹ��,Tug For Inter
	
	//Lines�����Ϣ��311�������ư�	
	int   iActiveLine;							//��ǰ����������		
	int   LineCableSpeed[MAXLINENUM];			//20�������ٶȿ���: 
	float LineForces[MAXLINENUM];				//20����������Ϣ
	float LineLengths[MAXLINENUM];				//20������������Ϣ	
	LINEOPSTATE LineOPState[MAXLINENUM];		//20������״̬:0:Null,1:PayOut,2:PayOutStop;3:Slack3;4:Slack2;5:Slack1;6:Stop,7:Heave1,8:Heave2,9:Heave3

	//Azimuthing---ȫ��ת�豸(ˮ��)---new added by liujingjing_2018.3.15
	float fStbdOrderBucketAng;					//ˮ��������ˮ����---��ʵ�Ƕȣ���λ"deg"
	float fStbdRealBucketAng;					//ˮ��ʵ����ˮ���򣬵�λ"deg"
	float fStbdOrderBucketSize;					//��������ˮ����ˮ�豸���ϳ̶�---�ٷֱ�
	float fStbdRealBucketSize;					//��������ˮ����ˮ�豸���ϳ̶�---�ٷֱ�
	float fPortOrderBucketAng;					//ˮ��������ˮ����---��ʵ�Ƕȣ���λ"deg"
	float fPortRealBucketAng;					//ˮ��ʵ����ˮ���򣬵�λ"deg"
	float fPortOrderBucketSize;					//��������ˮ����ˮ�豸���ϳ̶�---�ٷֱ�
	float fPortRealBucketSize;					//��������ˮ����ˮ�豸���ϳ̶�---�ٷֱ�
		
	COSCtrlDeviceTemp m_OSCtrlDevice;			//�豸������Ϣ
	COSCtrlInfoByTouchScr struDataForBigTouchCard;//��������Ϣ
	COSCtrlInfoByECDIS    struDataForEcdisCard;	  //���Ե��Ӻ�ͼ�Ŀ�����Ϣ
	COSCtrlInfoByIOS      struDataForIOSCtrl;	  //�����ڽ���Ավ�Ŀ�����Ϣ  
	bool m_bClearECDISCtrl;
	bool bTouchControlFog = false;//�վ��ش�������ť�������


	//602FishGear����(�洬����)��_1
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

	//602FishGear����(�洬����)��_2
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
	
	//231���ˮ������
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

	//241�ұ�ˮ������
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

	//251RPM���ư壻281˫���ӿ��ư壻281˫���ӿ��ư�_2�����ֿ���̨�ã�281˫���ӿ��ư�_3���洬�ã�281˫���ӿ��ư�_4���洬��
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
	//����Ǳͧ���ϸ���Ǳ����
	int iCDQTRowComeUpDive = 7;//����ˮƽ��
	int iCDQTSternComeUpDive = 7;//����ˮƽ��

public:
	//λ����̬�ٶȼ��ٶȵ����
	CGeoPoint ship_lon_lat;						//������γ�Ƚṹ����
	int OwnshipID;
	float fSpeed;								//�����ٶȣ���λ"kn/s"��һ��Ϊ>=0.0f��Ҫ���ٶȵķ������ʹ��	
	float fCourse;								//��������
	/////////////////////////////////////////////////////////////////////////////////	
	float fWindDir;								//���Է��򣬵�λ"deg"
	float fWindSpeed;							//���Է��٣���λ"m/s"
	float fRelativeWindSpeed;					//��Է��٣���λ"m/s"
	float fRelativeWindDir;						//��Է��򣬵�λ"deg"
	float fDepthUnderKeel;						//����������ˮ����ǵ����ߣ�����λ"m"
	float fTideHeight;							//����
	/////////////////////////////////////////////////////////////////////////////////
	
	int iSimuLocalYear;				//��ȡ���
	int iSimuLocalMonth;				//��ȡ�·�
	int iSimuLocalDate;				//��ȡ����
	int iSimuLocalDay;				//��ȡ���ڣ�0=�����գ�1=����һ��...
	int iSimuLocalHour;
	int iSimuLocalMinute;
	int iSimuLocalSecond;				//��ȡ��
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
	//��ӽӿ�
public:
	CWnd* m_pWnd; 	CConsoleInterfaceImpl();
	~CConsoleInterfaceImpl();

	void EncodeDataToConsole(COwnShip* pOwnShip, CEnviroment* pEnviroment, vector<CTargetShip>& vecpTargetShip);		//��������Ϣ���뵽����̨״̬��Ϣ,��Ŀ�괬��Ϣ����
	void DecodeDataFromConsole(COwnShip* pOwnShip, CEnviroment* pEnviroment);	//���̵߳��ã��Ƚ��м��������(��Ҫ���ݱ���)д�뵽���߳���ʱ�������������߳���ʱ������ֵ��pOwnShip��pEnviroment������

	void DecodeAllDataFromAllConsoleCard();		//���߳��е��ã�����	
	void DecodeAllControlTypeInfoInConsole();	//���߳��е��ã��ɿ���̨��������Ϣ�Ĵ�����Ϣ������Ϊ�豸�ĵ�ǰ����������Ϣ

	void SetAllOwnShipDataToAllConsoleCard(COwnShip* pOwnShip, CEnviroment* pEnviroment);	//���̵߳��ã������߳��е�����д�뵽�м����
	void EncodeAllOwnShipDataToAllConsoleCard();				//���߳��е��ã����м����������д�뵽���̵߳������У������ݱ��뵽��·��Ķ�Ӧ������ֽ���	

	void SetImplPrt(CConsoleInterfaceImpl* pSetImpl) { pConsoleInterfaceImpl = pSetImpl; }

public:
	//�����ӿ�����
	CCriticalSection m_CSForData;				//MFC�ٽ��������,�����������ݣ����̡߳����߳����ݱ�����
	CAllDataForConsole* m_pDataAllConsole;		//�̼߳���м����
	CAllDataForConsole* m_pAllDataTempMain;		//���߳�ʹ�õ���ʱ���������߳̽�
	CAllDataForConsole* m_pAllDataTempSub;		//���߳�ʹ�õ���ʱ����
	CConsoleInterfaceImpl* pConsoleInterfaceImpl;

	bool m_bAutoStandbyCluchin;					//������ֱ��׼����
	DataComm::SYSLANG m_eLastLanguage;			//�������л�����

public:
	CInterfaceForIOSControl* m_pInterfaceForIOSControl;
	CInterfaceForECDISControl* m_pInterfaceForECDISControl;
	CInterfaceForConsole2005* m_pInterfaceForConsole2005;	//ͨ��Console2005����̨��·ϵͳ�ɼ����ӡ����ơ����ֵ���Ϣ�ӿڶ���
	CInterfaceForDevSimUnit* m_pInterfaceForDevSimUnit;
	CInterfaceForConsole2020* m_pInterfaceForConsole2020;	//ͨ��Console2020����̨��·ϵͳ�ɼ����ӡ����ơ����ֵ���Ϣ�ӿڶ���
	CInterfaceForIMDModule* m_pInterfaceForIMDModule;		//ͨ��IMDģ��ɼ����ӡ����ơ����ֵ���Ϣ�ӿڶ���
	CInterfaceForCompassRudder* m_pInterfaceForCompassRudder1;	//��1·�����޾��������Ϣ�Ľӿڶ���
	CInterfaceForCompassRudder* m_pInterfaceForCompassRudder2;	//��1·�����޾��������Ϣ�Ľӿڶ���
	CInterfaceForCompassRudder* m_pInterfaceForCompassRudder3;	//��3·�����޾��������Ϣ�Ľӿڶ���
	CInterfaceForTouchPnlAnchorSound* m_pInterfaceForTouchPnlAnchorSound;
	CInterfaceForTouchPnlEngine* m_pInterfaceForTouchPnlEngine;
	CInterfaceForTouchPnlEngineVert* m_pInterfaceForTouchPnlEngineVert;
	CInterfaceForTouchPnlPitch* m_pInterfaceForTouchPnlPitch;
	CInterfaceForTouchPnlQTDraft* m_pInterfaceForTouchPnlQTDraft;
	CInterfaceForTouchPnlSteer* m_pInterfaceForTouchPnlSteer;
	CInterfaceForTouchPnlThruster* m_pInterfaceForTouchPnlThruster;
	CInterfaceForGPSAIS* m_pInterfaceForGPSAIS;
	CInterfaceForConsoleRudder2022* m_pInterfaceForConsoleRudder2022;//�����������
	CInterfaceForConsoleEngine2022* m_pInterfaceForConsoleEngine2022;//��������������;
	InterfaceForConsoleJoyStick* m_pInterfaceForConsoleJoyStick;//ҡ�˿���
	CInterfaceForConsoleSZYHZMeter* m_pInterfaceForConsoleSZYHZMeter;//��������վ ��ͷ��Ϣ����
};
#endif 
