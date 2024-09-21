//
// FlushMouseSub.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/03  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "..\MiscLIB\CRawInput.h"
//
// Define
// 

//
// Struct Define
//

//
// Global Data
//

//
// Global Prototype Define
//
extern BOOL		bSettingsEx(HWND hWnd, int iCode, int iSubCode);
extern BOOL		bStartSynTPHelper(HWND hWnd, DWORD dwSynTPHelper, BOOL bShowMessage);
extern BOOL		bStopSynTPHelper();
extern BOOL		bCheckDrawIMEModeArea(HWND hWndObserved);

//
// class CMouseRawInput
//
class CMouseRawInput	:	public CRawInput
{
public:
	CMouseRawInput();
	~CMouseRawInput();

private:
	void	vRawInputMouseHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput);
};

//
// class CEventHook
//
class CEventHook
{
public:
	CEventHook();
	~CEventHook();

	BOOL		bEventSet();
	BOOL		bEventUnset();

private:
	static void CALLBACK vHandleEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
	static void CALLBACK vHandleEventIME(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

public:
	HWND			hFormerWnd;

private:
	HWINEVENTHOOK	hEventHook;
	HWINEVENTHOOK	hEventHookIME;
};

//
// class CPowerNotification
//
class CPowerNotification
{
public:
	CPowerNotification(HWND hWnd);
	~CPowerNotification();

	BOOL		PowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting);

private:
	HPOWERNOTIFY	hSuspendResumeNotification;
	HPOWERNOTIFY	hPowerSettingNotification;
	GUID			guidPowerSettingNotification;
};

//
// class CFlushMouseHook
//
class CFlushMouseHook
{
public:
	CFlushMouseHook();
	~CFlushMouseHook();
	BOOL		bHookSet(HWND hWnd, LPCTSTR lpszDll64Name, LPCTSTR lpszExec32Name);
	BOOL		bHookUnset();

private:
	BOOL		bHook32DllStart(HWND hWnd, LPCTSTR lpszExec32Name);
	BOOL		bHook32DllStop() const;
	static BOOL CALLBACK bEnumWindowsProcHookStop(HWND hWnd, LPARAM lParam);

private:
	HMODULE		hHook64Dll;
	BOOL		bShellHook64;
	BOOL		bGlobalHook64;
	BOOL		bKeyboardHookLL64;
	BOOL		bHook32Dll;

private:
	LPPROCESS_INFORMATION	lpstProcessInfomation;
};

/* = EOF = */