//
// FlushMouse32.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/04/06  JeffyTS  	New edit.
//

//
// Include
//
#pragma once

//
// Define
//

//
// Global Prototype Define
//

//
// Global Data
//

//
// class CPowerNotification
//
class CPowerNotification32
{
	public:
		explicit CPowerNotification32(HWND hWnd);
		~CPowerNotification32();

		BOOL		PowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting);

	private:
		HPOWERNOTIFY	hSuspendResumeNotification;
		HPOWERNOTIFY	hPowerSettingNotification;
		GUID			guidPowerSettingNotification;
};



/* = EOF = */