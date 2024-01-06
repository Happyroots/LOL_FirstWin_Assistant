// ConsoleInterfaceImpl.cpp: implementation of the CConsoleInterfaceImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "windows.h"
#include "iostream"
#include <fstream>
#include <stdio.h>
using namespace std;

#include "ConsoleInterfaceImpl.h"
#include "InterfaceForCompassRudder.h"
#include "InterfaceForConsole2005.h"
#include "InterfaceForIMDModule.h"
#include "InterfaceForConsole2020.h"
#include "InterfaceForGPSAIS.h"
#include "InterfaceForTouchPnlAnchorSound.h"
#include "InterfaceForTouchPnlEngine.h"
#include "InterfaceForTouchPnlEngineVert.h"
#include "InterfaceForTouchPnlPitch.h"
#include "InterfaceForTouchPnlQTDraft.h"
#include "InterfaceForTouchPnlSteer.h"
#include "InterfaceForTouchPnlThruster.h"
#include "InterfaceForDevSimUnit.h"
#include "InterfaceForIOSControl.h"
#include "InterfaceForECDISControl.h"
#include "InterfaceForConsoleRudder2022.h"
#include "InterfaceForConsoleEngine2022.h"
#include "InterfaceForConsoleJoyStick.h"
#include "InterfaceForConsoleSZYHZMeter.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//CConsoleInterfaceImpl* pConsoleInterfaceImpl;

//主程序开始,析构函数
CConsoleInterfaceImpl::CConsoleInterfaceImpl()
{
	m_pDataAllConsole = new CAllDataForConsole();	//中间变量
	m_pAllDataTempMain = new CAllDataForConsole();	//主线程临时变量
	m_pAllDataTempSub = new CAllDataForConsole();	//子线程临时变量
	
	//中英文切换		
	m_eLastLanguage = SYSLANG::SYSLANG_ENGLISH;

	m_bAutoStandbyCluchin = false;

	//
	{
		m_pInterfaceForConsole2005 = new CInterfaceForConsole2005(this, m_pDataAllConsole, m_pAllDataTempMain, m_pAllDataTempSub);
		m_pInterfaceForConsole2005->CreateThread_Console2005PCB(DeviceCommIndex::PCBCTRLCOMM, _T("57600,N,8,1"));	//COM1

		m_pInterfaceForIMDModule = new CInterfaceForIMDModule(m_pAllDataTempSub, m_bAutoStandbyCluchin);	//IMD采集模块
		m_pInterfaceForIMDModule->CreateThread_IMDDevice(DeviceCommIndex::IMDCTRLCOMM, _T("115200, N, 8, 1"));	//COM2
		m_pInterfaceForConsoleRudder2022 = new CInterfaceForConsoleRudder2022(this, m_pDataAllConsole, m_pAllDataTempMain, m_pAllDataTempSub);//舵机触摸板
		m_pInterfaceForConsoleRudder2022->CreateThread_Rudder(DeviceCommIndex::TAORUDDERCOMM, _T("115200, N, 8, 1"));	//COM36
		m_pInterfaceForConsoleEngine2022 = new CInterfaceForConsoleEngine2022(this, m_pDataAllConsole, m_pAllDataTempMain, m_pAllDataTempSub);//主机触摸板
		m_pInterfaceForConsoleEngine2022->CreateThread_Engine(DeviceCommIndex::TAOENGINECOMM, _T("115200, N, 8, 1"));	//COM38

		m_pInterfaceForConsoleJoyStick = new InterfaceForConsoleJoyStick(this, m_pDataAllConsole, m_pAllDataTempMain, m_pAllDataTempSub);//主机触摸板
		m_pInterfaceForConsoleJoyStick->CreateThread_JoyStick();

			
		m_pInterfaceForConsole2020 = new CInterfaceForConsole2020(this,m_pAllDataTempSub, m_bAutoStandbyCluchin);
		m_pInterfaceForConsole2020->OpenPortForGetData_SelfCard();

		m_pInterfaceForIOSControl = new CInterfaceForIOSControl(m_pAllDataTempSub, m_bAutoStandbyCluchin);
		m_pInterfaceForIOSControl->CreateThreadForDevice_IOS();
		//ECDIS
		m_pInterfaceForECDISControl = new CInterfaceForECDISControl(m_pAllDataTempSub, m_bAutoStandbyCluchin);
		m_pInterfaceForECDISControl->CreateThreadForDevice_ECDIS();

		m_pInterfaceForDevSimUnit = new CInterfaceForDevSimUnit(m_pAllDataTempSub, m_bAutoStandbyCluchin);
		m_pInterfaceForDevSimUnit->CreateThreadForDevice_TouchScr();

		m_pInterfaceForTouchPnlAnchorSound = new CInterfaceForTouchPnlAnchorSound(this, m_pDataAllConsole, m_pAllDataTempMain, m_pAllDataTempSub);
		m_pInterfaceForTouchPnlAnchorSound->CreateThread_TouchPnl_AnchorSound(DeviceCommIndex::ANCHORSOUNDCOMM, _T("115200, N, 8, 1"));

		m_pInterfaceForTouchPnlEngine = new CInterfaceForTouchPnlEngine(this, m_pDataAllConsole, m_pAllDataTempMain, m_pAllDataTempSub);
		m_pInterfaceForTouchPnlEngine->CreateThread_TouchPnl_Engine(DeviceCommIndex::ENGINETOUCHCOMM, _T("115200, N, 8, 1"));

		m_pInterfaceForTouchPnlEngineVert = new CInterfaceForTouchPnlEngineVert(this, m_pDataAllConsole, m_pAllDataTempMain, m_pAllDataTempSub);
		m_pInterfaceForTouchPnlEngineVert->CreateThread_TouchPnl_VertEngine(DeviceCommIndex::VERTENGINECOMM, _T("115200, N, 8, 1"));

		m_pInterfaceForTouchPnlPitch = new CInterfaceForTouchPnlPitch(this, m_pDataAllConsole, m_pAllDataTempMain, m_pAllDataTempSub);
		m_pInterfaceForTouchPnlPitch->CreateThread_TouchPnl_Pitch(DeviceCommIndex::PITCHTOUCHCOMM, _T("115200, N, 8, 1"));

		/*m_pInterfaceForTouchPnlQTDraft = new CInterfaceForTouchPnlQTDraft(this, m_pDataAllConsole, m_pAllDataTempMain, m_pAllDataTempSub);
		m_pInterfaceForTouchPnlQTDraft->CreateThread_TouchPnl_QTDraft(DeviceCommIndex::QTDRAFTCOMM, _T("115200, N, 8, 1"));*/

		m_pInterfaceForTouchPnlSteer = new CInterfaceForTouchPnlSteer(this, m_pDataAllConsole, m_pAllDataTempMain, m_pAllDataTempSub);
		m_pInterfaceForTouchPnlSteer->CreateThread_TouchPnl_Steer(DeviceCommIndex::STEERTOUCHCOMM, _T("115200, N, 8, 1"));

		m_pInterfaceForTouchPnlThruster = new CInterfaceForTouchPnlThruster(this, m_pDataAllConsole, m_pAllDataTempMain, m_pAllDataTempSub);
		m_pInterfaceForTouchPnlThruster->CreateThread_TouchPnl_Thruster(DeviceCommIndex::THRUSTERTOUCHCOMM, _T("115200, N, 8, 1"));

		
		//第1路发送罗经三面舵信息的接口对象
		m_pInterfaceForCompassRudder1 = new CInterfaceForCompassRudder(m_pAllDataTempSub);
		m_pInterfaceForCompassRudder1->CreateThreadForDevice_Compass(DeviceCommIndex::COMPASSCOMM_1st, _T("4800, N, 8, 1"));//3
		//第2路发送罗经三面舵信息的接口对象
		m_pInterfaceForCompassRudder2 = new CInterfaceForCompassRudder(m_pAllDataTempSub);
		m_pInterfaceForCompassRudder2->CreateThreadForDevice_Compass(DeviceCommIndex::AUXCOMPASSCOMM_2nd, _T("4800, N, 8, 1"));//5
		//第3路发送罗经三面舵信息的接口对象
		m_pInterfaceForCompassRudder3 = new CInterfaceForCompassRudder(m_pAllDataTempSub);
		m_pInterfaceForCompassRudder3->CreateThreadForDevice_Compass(DeviceCommIndex::AUXCOMPASSCOMM_3rd, _T("4800, N, 8, 1"));//6

		m_pInterfaceForConsoleSZYHZMeter = new CInterfaceForConsoleSZYHZMeter(m_pAllDataTempMain);		//深圳引航站 表头更新信息																												   
		m_pInterfaceForConsoleSZYHZMeter->CreateThreadForDevice_Meter(DeviceCommIndex::SZYHZMETERCOM, DeviceCommIndex::SZYHZMETERCOM2, _T("115200, N, 8, 1"));//6


		m_pInterfaceForGPSAIS = new CInterfaceForGPSAIS();
		m_pInterfaceForGPSAIS->CreateThread_GPS();
		m_pInterfaceForGPSAIS->CreateThread_AIS();
		m_pInterfaceForGPSAIS->CreateThread_GPS2();
	}
}

CConsoleInterfaceImpl::~CConsoleInterfaceImpl()
{
	if (m_pInterfaceForDevSimUnit)
		delete m_pInterfaceForDevSimUnit;

	if (m_pInterfaceForConsole2020)
		delete m_pInterfaceForConsole2020;

	if (m_pInterfaceForIMDModule)
		delete m_pInterfaceForIMDModule;

	if (m_pInterfaceForCompassRudder1)
		delete m_pInterfaceForCompassRudder1;

	if (m_pInterfaceForCompassRudder2)
		delete m_pInterfaceForCompassRudder2;

	if (m_pInterfaceForTouchPnlAnchorSound)
		delete m_pInterfaceForTouchPnlAnchorSound;

	if (m_pInterfaceForTouchPnlEngine)
		delete m_pInterfaceForTouchPnlEngine;

	if (m_pInterfaceForTouchPnlEngineVert)
		delete m_pInterfaceForTouchPnlEngineVert;

	if (m_pInterfaceForTouchPnlPitch)
		delete m_pInterfaceForTouchPnlPitch;

	if (m_pInterfaceForTouchPnlQTDraft)
		delete m_pInterfaceForTouchPnlQTDraft;

	if (m_pInterfaceForTouchPnlSteer)
		delete m_pInterfaceForTouchPnlSteer;

	if (m_pInterfaceForTouchPnlThruster)
		delete m_pInterfaceForTouchPnlThruster;


	delete m_pDataAllConsole;
	delete m_pAllDataTempMain;
	delete m_pAllDataTempSub;
}