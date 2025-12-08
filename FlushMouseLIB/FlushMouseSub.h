//
// FlushMouseSub.h
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
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
extern BOOL		bStopSynTPHelper(HWND hWnd);
extern BOOL		bCheckDrawIMEModeArea(HWND hWndObserved);

extern BOOL		bKeyboardTypeIsEP();
extern BOOL		bForExplorerPatcherSWS(HWND hForeWnd, BOOL bChangeToIME, BOOL bIMEModeForcedChange, LPHKL lpNewHKL, LPHKL lpPreviousHKL);
extern BOOL		bCheckExistingJPIME();
extern BOOL		bChromium_Helper(HWND hForeWnd);

extern HWND	 	hGetObservedWnd();
extern BOOL	 	bCreateProcess(LPCTSTR lpszExecName, LPTSTR lpCommandLine);

//
// class CRawInputDevice
//
class CRawInputDevice	:	public CRawInput
{
	public:
		CRawInputDevice()
			:	uuMouseWindowTick(GetTickCount64()), bOnlyCtrlLL(FALSE), dwPreviousVKLL(0),
				bStartConvertingLL(FALSE), uuKeyRepeatTickLL(GetTickCount64())
		{};
		~CRawInputDevice() override {};

private:
	virtual void	vRawInputMouseHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput) override;
	virtual void	vRawInputKeyboardHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput) override;

private:
	ULONGLONG		uuMouseWindowTick;

	BOOL			bOnlyCtrlLL;
	DWORD			dwPreviousVKLL;
	BOOL			bStartConvertingLL;
	ULONGLONG		uuKeyRepeatTickLL;
};

//
// class CPowerNotification
//
class CPowerNotification
{
	public:
		explicit CPowerNotification(HWND hWnd);
		~CPowerNotification();

		BOOL		PowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting);

	private:
		HPOWERNOTIFY	hSuspendResumeNotification;
		HPOWERNOTIFY	hPowerSettingNotification;
		GUID			guidPowerSettingNotification;
};

//
// class CEventHook
//
class CEventHook
{
	public:
		CEventHook();
		~CEventHook();

		BOOL	bEventSet(HWND hWnd);
		BOOL	bEventUnset();

	private:
		static void CALLBACK vHandleEvent(HWINEVENTHOOK hook, DWORD dwEvent, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
		static void CALLBACK vHandleEventIME(HWINEVENTHOOK hook, DWORD dwEvent, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

	public:

	private:
		HWND			hMainWnd;
		HWINEVENTHOOK	hEventHook;
		HWINEVENTHOOK	hEventHookIME;
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
		BOOL		bHook32Dll;

	private:
		LPPROCESS_INFORMATION	lpstProcessInformation;
};


/* = EOF = */
