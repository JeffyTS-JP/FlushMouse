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
#include <powrprof.h>
#pragma comment(lib, "powrprof.lib")

//
// Define
// 
// Application-defined message identifiers.
#define WM_TASKTRAY			(WM_APP + 1)

//
// Struct Define
//

//
// Global Data
//
extern HWND		hAboutDlg;
extern HWND		hSettingDlg;

//
// Global Prototype Define
//
extern BOOL 	bCreateTaskTrayWindow(HWND hWnd, UINT uID);
extern BOOL		bDestroyTaskTrayWindow(HWND hWnd);
extern void		Cls_OnTaskTray(HWND hWnd, UINT id, UINT uMsg);
extern void		vMessageBox(HWND hWnd, UINT uID, UINT uType);
extern VOID		vAboutDialog(HWND hWnd);
extern VOID		vSettingDialog(HWND hWnd);
extern VOID		vGetSetProfileData();

//
// class CFocusEvent
//
class CFocusEvent
{
public:
	CFocusEvent();
	~CFocusEvent();
	BOOL		bEventSet();
	BOOL		bEventUnset();

private:
	static void CALLBACK vHandleEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

private:
	HWINEVENTHOOK	hEventHook;
};

//
// class CPowerNotification
//
class CPowerNotification
{
public:
	CPowerNotification();
	~CPowerNotification();

private:
	static DEVICE_NOTIFY_CALLBACK_ROUTINE DeviceNotifyCallbackRoutine;

private:
	DEVICE_NOTIFY_SUBSCRIBE_PARAMETERS	Recipient;
	HPOWERNOTIFY	RegistrationHandle;
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
	BOOL		bHook64DllLoad(LPCTSTR lpszDll64Name);
	BOOL		bHook64DllUnload();
	BOOL		bHook32DllStart(HWND hWnd, LPCTSTR lpszExec32Name);
	BOOL		bHook32DllStop();
	static BOOL CALLBACK bEnumWindowsProc(HWND hWnd, LPARAM lParam);

private:
	HMODULE		hHook64Dll;
	BOOL		bGlobalHook64;
	BOOL		bKeyboardHook64;
	BOOL		bMouseHook64;
	BOOL		bHook32Dll;

private:
	LPPROCESS_INFORMATION	lpstProcessInfomation;
};

