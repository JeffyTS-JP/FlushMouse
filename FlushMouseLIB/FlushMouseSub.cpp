// FlushMouseSub.cpp
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
#include "FlushMouseSub.h"
#include "FlushMouseLIB.h"
#include "TaskTray.h"
#include "Eventlog.h"
#include "CommonDef.h"
#include "..\FlushMouseDLL\ShellHookDll.h"
#include "..\FlushMouseDLL\GlobalHookDll.h"
#include "..\FlushMouseDLL\MouseHookDll.h"
#include "..\FlushMouseDLL\KeyboardHookDll.h"
#include "..\FlushMouseDLL32\FlushMouseDll32.h"
#include "..\MiscLIB\CRegistry.h"

//
// Define
//

//
// Global Data
//

//
// Local Data
//

//
// Local Prototype Define
//

//
// bCheckDrawIMEModeArea()
//
BOOL		bCheckDrawIMEModeArea(HWND hWndObserved)
{
	if (hMainWnd == hWndObserved)	return FALSE;
	if (FindWindow(L"Shell_TrayWnd", NULL) == hWndObserved)	return FALSE;
	POINT	pt{};
	if (GetCursorPos(&pt)) {
		if (WindowFromPoint(pt) != NULL) {
			RECT	rc{};
			if (bGetTaskTrayWindowRect(hMainWnd, &rc) != FALSE) {
				if (((pt.x >= rc.left) && (pt.x <= rc.right)) || ((pt.y <= rc.top) && (pt.y >= rc.bottom)))	return FALSE;	// Clicked on Notify Icon
			}
		}
		return TRUE;
	}
	return FALSE;
}

//
// class CRawInput
//
CRawInput::CRawInput(HWND hwndTarget)
{
	_hwndTarget = hwndTarget;
	lpRawInputDevice = NULL;

	if (_hwndTarget == NULL)	return;
	if (lpRawInputDevice == NULL)	lpRawInputDevice = new RAWINPUTDEVICE[sizeof(RAWINPUTDEVICE)];
}

CRawInput::~CRawInput()
{
	if (lpRawInputDevice != NULL)	delete [] lpRawInputDevice;
	lpRawInputDevice = NULL;
	_hwndTarget = NULL;
}

//
// bRegisterRawInputDevices()
//
BOOL		CRawInput::bRegisterRawInputDevices(USHORT usUsagePage, USHORT usUsage, DWORD  dwFlags)
{
	if ((_hwndTarget == NULL) || (lpRawInputDevice == NULL))	return FALSE;
	lpRawInputDevice->hwndTarget = _hwndTarget;	lpRawInputDevice->usUsagePage = usUsagePage;
	lpRawInputDevice->usUsage = usUsage;		lpRawInputDevice->dwFlags = dwFlags;
#define	uiNumDevices	1
	if (!RegisterRawInputDevices(lpRawInputDevice, uiNumDevices, sizeof(RAWINPUTDEVICE))) {
		return FALSE;
	}
#undef uiNumDevices
	return TRUE;
}

//
// vRawInputDevicesHandler()
//
void		CRawInput::vRawInputDevicesHandler(HWND hWnd, DWORD dwFlags, HRAWINPUT hRawInput)
{
	UINT	uBufferSize = 0;
	UINT	cbSize = 0;
	if ((uBufferSize = GetRawInputData(hRawInput, RID_INPUT, NULL, &cbSize, sizeof(RAWINPUTHEADER))) != (-1)) {
		LPUINT	lpuBuffer = new UINT[cbSize];
		if (lpuBuffer != NULL) {
			if ((uBufferSize = GetRawInputData(hRawInput, RID_INPUT, lpuBuffer, &cbSize, sizeof(RAWINPUTHEADER))) != (-1)) {
				LPRAWINPUT	lpRawInput = (LPRAWINPUT)lpuBuffer;
				switch (lpRawInput->header.dwType) {
				case RIM_TYPEMOUSE:
					vRawInputMouseHandler(hWnd, dwFlags, lpRawInput);
					break;
				default:
					break;
				}

			}
			if (lpuBuffer != NULL) delete [] lpuBuffer;
		}
	}
}

//
// vRawInputMouseHandler()
//
void	CRawInput::vRawInputMouseHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput)
{
	UNREFERENCED_PARAMETER(dwFlags);

	RAWMOUSE RawMouse = (RAWMOUSE)(lpRawInput->data.mouse);
	POINT	pt{};
	if (RawMouse.ulButtons & RI_MOUSE_LEFT_BUTTON_DOWN) {
		GetCursorPos(&pt);
		Cls_OnLButtonDownEx(hWnd, pt.x, pt.y, GetForegroundWindow());
	}
	else if (RawMouse.ulButtons & RI_MOUSE_LEFT_BUTTON_UP) {
		GetCursorPos(&pt);
		Cls_OnLButtonUpEx(hWnd, pt.x, pt.y, GetForegroundWindow());
	}
}

//
// class CPowerNotification
// CPowerNotification()
//
CPowerNotification::CPowerNotification(HWND hWnd)
{
	guidPowerSettingNotification = GUID_NULL;
	if ((hSuspendResumeNotification = RegisterSuspendResumeNotification(hWnd, DEVICE_NOTIFY_WINDOW_HANDLE)) == NULL) {
	}
	if ((hPowerSettingNotification = RegisterPowerSettingNotification(hWnd, &guidPowerSettingNotification, DEVICE_NOTIFY_WINDOW_HANDLE)) == NULL) {
	}
}

//
// ~CPowerNotification()
//
CPowerNotification::~CPowerNotification()
{
	if (hPowerSettingNotification != NULL) {
		if (UnregisterPowerSettingNotification(hPowerSettingNotification) == 0) {
		}
	}
	hPowerSettingNotification = NULL;
	if (hSuspendResumeNotification != NULL) {
		if (UnregisterSuspendResumeNotification(hSuspendResumeNotification) == 0) {
		}
	}
	hSuspendResumeNotification = NULL;
}

//
// PowerBroadcast()
//
BOOL		CPowerNotification::PowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(Type);
	UNREFERENCED_PARAMETER(lpSetting);
	switch (Type) {
	case PBT_APMSUSPEND:
		break;
	case PBT_APMRESUMEAUTOMATIC:
		break;
	case PBT_APMRESUMESUSPEND:
		bDestroyTaskTrayWindow(hWnd);
		break;
	case PBT_POWERSETTINGCHANGE:
		break;
	case PBT_APMPOWERSTATUSCHANGE:
			SYSTEM_POWER_STATUS	PowerStatus{};
			if (GetSystemPowerStatus(&PowerStatus)) {
				switch (PowerStatus.ACLineStatus) {
				case 0:
					bDestroyTaskTrayWindow(hWnd);
					break;
				case 1:
					bDestroyTaskTrayWindow(hWnd);
					break;
				default:
					break;
				}
			}
		if (lpSetting != NULL) {
			PPOWERBROADCAST_SETTING	lpPwrSetting = (POWERBROADCAST_SETTING*)lpSetting;
			if ((lpPwrSetting->PowerSetting == GUID_CONSOLE_DISPLAY_STATE)
				|| (lpPwrSetting->PowerSetting == GUID_MONITOR_POWER_ON)
				|| (lpPwrSetting->PowerSetting == GUID_SESSION_DISPLAY_STATUS)) {
				if (lpPwrSetting->Data[0] == 0) {
				}
				else {
				}
			}
		}
	}
	return TRUE;
}

//
// class CFocusEvent
//
//
CEventHook::CEventHook()
{
	hEventHook = NULL;
	hFormerWnd = NULL;
	hEventHookIME = NULL;
}

CEventHook::~CEventHook()
{
	if (bEventUnset()) {
		hEventHook = NULL;
		hFormerWnd = NULL;
		hEventHookIME = NULL;
	}
}

//
// bEventSet()
//
BOOL		CEventHook::bEventSet()
{
#define	EVENT_FLAGS		(WINEVENT_OUTOFCONTEXT)
	hEventHook = SetWinEventHook(
		EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
		NULL,
		&vHandleEvent,
		0, 0,
		EVENT_FLAGS);
	if (hEventHook == NULL)	return FALSE;

	hEventHookIME = SetWinEventHook(
		EVENT_OBJECT_TEXTSELECTIONCHANGED, EVENT_OBJECT_TEXTEDIT_CONVERSIONTARGETCHANGED,
		NULL,
		&vHandleEventIME,
		0, 0,
		EVENT_FLAGS);
	if (hEventHookIME == NULL)	return FALSE;

	return TRUE;
}

//
// bEventUnset()
//
BOOL		CEventHook::bEventUnset()
{
	if (hEventHookIME) {
		if (UnhookWinEvent(hEventHookIME)) {
			hEventHookIME = NULL;
		}
	}
	if (hEventHook) {
		if (UnhookWinEvent(hEventHook)) {
			hEventHook = NULL;
			//dwIMEEvent = 0;
		}
	}
	if ((hEventHookIME == NULL) && (hEventHook == NULL))	return TRUE;
	return FALSE;
}

//
// vHandleEvent()
//
void CALLBACK CEventHook::vHandleEvent(HWINEVENTHOOK hook, DWORD dwEvent, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	UNREFERENCED_PARAMETER(hook);
	UNREFERENCED_PARAMETER(idObject);
	UNREFERENCED_PARAMETER(idChild);
	UNREFERENCED_PARAMETER(dwEventThread);
	UNREFERENCED_PARAMETER(dwmsEventTime);

	if (hWnd == NULL)	return;
	if (dwEvent == EVENT_SYSTEM_FOREGROUND) {
		bIMEInConverting = FALSE;
		HWND	hFindWnd = FindWindow(CLASS_FLUSHMOUSE, NULL);
		if (hFindWnd != NULL) {
			PostMessage(hFindWnd, WM_EVENT_SYSTEM_FOREGROUNDEX, (WPARAM)dwEvent, (LPARAM)hWnd);
		}
	}
}

//
// vHandleEventIME()
//
void CALLBACK CEventHook::vHandleEventIME(HWINEVENTHOOK hook, DWORD dwEvent, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	UNREFERENCED_PARAMETER(hook);
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(idObject);
	UNREFERENCED_PARAMETER(idChild);
	UNREFERENCED_PARAMETER(dwEventThread);
	UNREFERENCED_PARAMETER(dwmsEventTime);

	switch (dwEvent) {
	case EVENT_OBJECT_CLOAKED:
		bIMEInConverting = FALSE;
		break;
	case EVENT_OBJECT_UNCLOAKED:
	case EVENT_OBJECT_IME_SHOW:
	case EVENT_OBJECT_IME_CHANGE:
		bIMEInConverting = TRUE;
		break;
	default:
		return;
	}
}

//
// class CFlushMouseHook 
//
CFlushMouseHook::CFlushMouseHook()
{
	hHook64Dll = NULL;
	bShellHook64 = FALSE;
	bGlobalHook64 = FALSE;
	bKeyboardHookLL64 = FALSE;
	bMouseHook64 = FALSE;
	bHook32Dll = FALSE;
	lpstProcessInfomation = new PROCESS_INFORMATION[sizeof(PROCESS_INFORMATION)];
	if (lpstProcessInfomation)	ZeroMemory(lpstProcessInfomation, sizeof(PROCESS_INFORMATION));
}

CFlushMouseHook::~CFlushMouseHook()
{
	bHookUnset();
	if (lpstProcessInfomation) {
		delete[]	lpstProcessInfomation;
		lpstProcessInfomation = NULL;
	}
}
//
// bHookSet()
//
BOOL			CFlushMouseHook::bHookSet(HWND hWnd, LPCTSTR lpszDll64Name, LPCTSTR lpszExec32Name)
{
	UNREFERENCED_PARAMETER(lpszDll64Name);
	if ((bGlobalHook64 = bGlobalHookSet(hWnd)) != FALSE) {
		if ((bKeyboardHookLL64 = bKeyboardHookLLSet(hWnd)) != FALSE) {
			if ((bShellHook64 = bShellHookSet(hWnd)) != FALSE) {
				if ((bHook32Dll = bHook32DllStart(hWnd, lpszExec32Name)) != FALSE) {
					return TRUE;
				}
			}
		}
	}
	bHookUnset();
	return FALSE;
}

//
// bHookUnset()
//
BOOL		CFlushMouseHook::bHookUnset()
{
	if (bHook32Dll)			bHook32DllStop();
	if (bShellHook64)		bShellHookUnset();
	if (bKeyboardHookLL64)	bKeyboardHookLLUnset();
	if (bGlobalHook64)		bGlobalHookUnset();
	return TRUE;
}

//
// bHook32DllStart()
//
BOOL	 	CFlushMouseHook::bHook32DllStart(HWND hWnd, LPCTSTR lpszExec32Name)
{
#define	COMAMANDLINESIZE (sizeof(_T(" ")) * (sizeof(unsigned long long) + 1))
	BOOL	bRet = FALSE;
	DWORD	dwSize = 0;
	dwSize = ExpandEnvironmentStrings(lpszExec32Name, NULL, 0);
	LPTSTR	lpszBuffer = new TCHAR[dwSize];
	if (lpszBuffer) {
		ZeroMemory(lpszBuffer, dwSize);
		dwSize = ExpandEnvironmentStrings(lpszExec32Name, lpszBuffer, dwSize);
		LPTSTR	lpszCommandLine = NULL;
		lpszCommandLine = new TCHAR[dwSize + COMAMANDLINESIZE];
		if (lpszCommandLine) {
			ZeroMemory(lpszCommandLine, (dwSize + sizeof(COMAMANDLINESIZE)));
			_sntprintf_s(lpszCommandLine, (dwSize + COMAMANDLINESIZE), _TRUNCATE, _T("%s %llu"), lpszBuffer, (unsigned long long)hWnd);
			if (lpstProcessInfomation) {
				STARTUPINFO	stStartupInfo{};	stStartupInfo.cb = sizeof(STARTUPINFO);
				if ((bRet = CreateProcess(NULL, lpszCommandLine, NULL, NULL, TRUE,
					NORMAL_PRIORITY_CLASS, NULL, NULL, &stStartupInfo, lpstProcessInfomation)) != FALSE) {
					for (int i = 3; i > 0; i--) {
						Sleep(500);
						if (FindWindow(CLASS_FLUSHMOUSE32, NULL) != NULL) {
							bHook32Dll = TRUE;
							bRet = TRUE;
							break;
						}
						else {
							bHook32Dll = FALSE;
							bRet = FALSE;
						}
					}
				}
			}
			delete[]	lpszCommandLine;
		}
		delete[]	lpszBuffer;
	}
	return bRet;
}

//
// bKeyboardHookUnset32()
//
BOOL 	CFlushMouseHook::bHook32DllStop()
{
#define	TIMEOUT	1000 
	if (!bHook32Dll)		return TRUE;
	BOOL		bRet = FALSE;
	if (lpstProcessInfomation != NULL) {
		if (lpstProcessInfomation->hProcess != NULL) {
			if (!EnumWindows((WNDENUMPROC)&CFlushMouseHook::bEnumWindowsProcHookStop, (LPARAM)lpstProcessInfomation)) {
				if (GetLastError() == ERROR_SUCCESS) {
					DWORD dwRet = WaitForSingleObject(lpstProcessInfomation->hProcess, TIMEOUT);
					switch (dwRet) {
					case WAIT_OBJECT_0:
						bRet = TRUE;
						break;
					case WAIT_FAILED:
					case WAIT_ABANDONED:
					case WAIT_TIMEOUT:
					default:
						if (TerminateProcess(lpstProcessInfomation->hProcess, 0)) {
							bRet = TRUE;
						}
						bRet = TRUE;
					}
					DWORD dwExitCode;
					if (!GetExitCodeProcess(lpstProcessInfomation->hProcess, &dwExitCode)) {
						bRet = FALSE;
					}
					if (lpstProcessInfomation->hProcess != NULL) {
						CloseHandle(lpstProcessInfomation->hProcess);
					}
				}
				if (lpstProcessInfomation->hThread != NULL) {
					CloseHandle(lpstProcessInfomation->hThread);
				}
			}
		}
	}
	return bRet;
}

//
// bEnumWindowsProc()
//
BOOL CALLBACK CFlushMouseHook::bEnumWindowsProcHookStop(HWND hWnd, LPARAM lParam)
{
	LPPROCESS_INFORMATION pi = (LPPROCESS_INFORMATION)lParam;

	DWORD lpdwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &lpdwProcessId);

	if (pi->dwProcessId == lpdwProcessId)
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
		SetLastError(ERROR_SUCCESS);
		return FALSE;
	}
	return TRUE;
}

/* = EOF = */