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
#include "pch.h"

//
// Define
// 

//
// Struct Define
//

//
// Global Data
//
extern HWND		hAboutDlg;
extern HWND		hSettingDlg;
extern BOOL		bIMEInConverting;

//
// Global Prototype Define
//
extern BOOL 	bCreateTaskTrayWindow(HWND hWnd, HICON hIcon, LPCTSTR lpszTitile);
extern BOOL		bReCreateTaskTrayWindow(HWND hWnd, UINT message);
extern BOOL		bDestroyTaskTrayWindow(HWND hWnd);
extern BOOL		bGetTaskTrayWindowRect(HWND hWnd, LPRECT lpRect);
extern void		Cls_OnTaskTrayEx(HWND hWnd, UINT id, UINT uMsg);
extern void		vMessageBox(HWND hWnd, UINT uID, UINT uType);
extern VOID		vAboutDialog(HWND hWnd);
extern VOID		vSettingDialog(HWND hWnd);
extern VOID		vGetSetProfileData();

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
	BOOL		bHook64DllLoad(LPCTSTR lpszDll64Name);
	BOOL		bHook64DllUnload();
	BOOL		bHook32DllStart(HWND hWnd, LPCTSTR lpszExec32Name);
	BOOL		bHook32DllStop();
	static BOOL CALLBACK bEnumWindowsProcHookStop(HWND hWnd, LPARAM lParam);

private:
	HMODULE		hHook64Dll;
	BOOL		bGlobalHook64;
	BOOL		bKeyboardHook64;
	BOOL		bMouseHook64;
	BOOL		bHook32Dll;

private:
	LPPROCESS_INFORMATION	lpstProcessInfomation;
};

/* = EOF = */