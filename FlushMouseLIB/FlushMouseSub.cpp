//
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
#include "KeyboardHook.h"
#include "Resource.h"
#include "CommonDef.h"
#include "..\FlushMouseDLL\ShellHookDll.h"
#include "..\FlushMouseDLL\GlobalHookDll.h"
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
static BOOL		bChangeHKLbySendInput(HKL hNewHKL, HKL hPreviousHKL, BOOL bForce);
static BOOL CALLBACK		bEnumChildProcChangeHKL(HWND hWnd, LPARAM lParam);
static BOOL		bSendInputSub(UINT cInputs, LPINPUT pInputs);

//
// bSettingsEx()
//
BOOL		bSettingsEx(HWND hWnd, int iCode, int iSubCode)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(iSubCode);

	if (!Profile || !Cursor)	return FALSE;

	switch(iCode) {
		case SETTINGSEX_OK:
			Cursor->vStopDrawIMEModeMouseByWndThread();
			vSettingDialogApply();
			if (!Profile->bSetProfileData())	return FALSE;
			if (Profile->lpstAppRegData->bDisplayIMEModeOnCursor && (Profile->lpstAppRegData->dwDisplayIMEModeMethod != DisplayIMEModeMethod_RESOURCE)) {
				Cursor->bStartDrawIMEModeMouseByWndThread();
			}
			else {
				Cursor->vStopDrawIMEModeMouseByWndThread();
			}
			vSettingDialogClose();
			return TRUE;
		case SETTINGSEX_CANCEL:
			vSettingDialogClose();
			if (Profile->lpstAppRegData->bDisplayIMEModeOnCursor && (Profile->lpstAppRegData->dwDisplayIMEModeMethod != DisplayIMEModeMethod_RESOURCE)) {
				Cursor->bStartDrawIMEModeMouseByWndThread();
			}
			else {
				Cursor->vStopDrawIMEModeMouseByWndThread();
			}
			return TRUE;
		case SETTINGSEX_APPLY:
			vSettingDialogApply();
			if (!Profile->bSetProfileData())	return FALSE;
			Cursor->vSetParamFromRegistry();
			return TRUE;
		case SETTINGSEX_RELOAD_REGISTRY:
			if (!Profile->bGetProfileData())	return FALSE;
			Cursor->vSetParamFromRegistry();
			return TRUE;
		case SETTINGSEX_RELOAD_MOUSECURSOR:
			vSettingDialogApply();
			if (!Profile->bGetProfileData4Mouse())	return FALSE;
			if (!Profile->bGetProfileData4IMEMode())	return FALSE;
			Cursor->vSetParamFromRegistry();
			if (!Cursor->bReloadCursor()) {
				vSettingDialogClose();
				bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
				return FALSE;
			}
			if (Profile->lpstAppRegData->bDisplayIMEModeOnCursor && (Profile->lpstAppRegData->dwDisplayIMEModeMethod != DisplayIMEModeMethod_RESOURCE)) {
				Cursor->bStartDrawIMEModeMouseByWndThread();
			}
			else {
				Cursor->vStopDrawIMEModeMouseByWndThread();
			}
			return TRUE;
		case SETTINGSEX_SETTINGS_CHANGE_PANE:
			return TRUE;
		case SETTINGSEX_SETTINGS_STARTED:
			if (Profile->lpstAppRegData->bDisplayIMEModeOnCursor &&(Profile->lpstAppRegData->dwDisplayIMEModeMethod != DisplayIMEModeMethod_RESOURCE)) {
				if (Cursor->bStartDrawIMEModeMouseByWndThread()) {
					return TRUE;
				}
				vSettingDialogClose();
				bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
				return FALSE;
			}
			else {
				Cursor->vStopDrawIMEModeMouseByWndThread();
			}
			return TRUE;
		case SETTINGSEX_SETTINGS_SETREGISTRY:
			vSettingDialogApply();
			return Profile->bSetProfileData4Settings();
		case SETTINGSEX_SETTINGS_GENERAL_SETREGISTRY: 
			vSettingDialogApply();
			if (!Profile->bSetProfileData4Mouse())	return FALSE;
			return TRUE;
		case SETTINGSEX_SETTINGS_IMEMODE_SETREGISTRY:
			vSettingDialogApply();
			if (!Profile->bSetProfileData4IMEMode())	return FALSE;
			Cursor->vSetParamFromRegistry();
			return TRUE;
		case SETTINGSEX_SETTINGS_CLOSE:
			if (!FindWindow(CLASS_FLUSHMOUSESETTINGS, NULL)) {
				vSettingDialogClose();
			}
			return TRUE;
		case SETTINGSEX_SYNTP_START:
			vSettingDialogApply();
			if (!Profile->bGetProfileData4SynTPHelper())	return FALSE;
			return bStartSynTPHelper(hWnd, Profile->lpstAppRegData->dwSynTPHelper1, TRUE);
		case SETTINGSEX_SYNTP_STOP:
			vSettingDialogApply();
			if (!Profile->bGetProfileData4SynTPHelper())	return FALSE;
			return bStopSynTPHelper(hWnd);
		case SETTINGSEX_SYNTP_IS_STARTED:
			if (!Profile->bGetProfileData4SynTPHelper())	return FALSE;
			if (SynTP)	return TRUE;
			else return FALSE;
		case SETTINGSEX_SYNTP_SETREGISRY:
			vSettingDialogApply();
			if (!Profile->bSetProfileData4SynTPHelper())	return FALSE;
			return TRUE;
	}
	return TRUE;
}

//
// bStartSynTPHelper()
//
BOOL		bStartSynTPHelper(HWND hWnd, DWORD dwSynTPHelper, BOOL bShowMessage)
{
	if (!Profile || !TaskTray)	return FALSE;
	Profile->lpstAppRegData->bSynTPStarted1 = FALSE;
	if (SynTP == NULL)	SynTP = new CSynTP(Profile->lpstAppRegData->dwSynTPPadX, Profile->lpstAppRegData->dwSynTPPadY, Profile->lpstAppRegData->dwSynTPEdgeX, Profile->lpstAppRegData->dwSynTPEdgeY);
	if (SynTP) {
		TCHAR	szInfo[MAX_LOADSTRING];
		switch (dwSynTPHelper) {
		case SYNTPH_SENDERIPV4:
		case SYNTPH_SENDERIPV4_START:
			if (bIsPrivateAddress(Profile->lpstAppRegData->szSynTPSendIPAddr1)) {
				SynTP->bStopSender();
				if (SynTP->bStartSender(hMainWnd, Profile->lpstAppRegData->szSynTPSendIPAddr1, Profile->lpstAppRegData->dwSynTPPortNo1)) {
					Profile->lpstAppRegData->bSynTPStarted1 = TRUE;
					break;
				}
				else {
					if (bShowMessage) {
						LoadString(Resource->hLoad(), IDS_CANTSYTPHELPER, szInfo, MAX_LOADSTRING);
						TaskTray->bDisplayBalloon(hWnd, NIIF_WARNING, NULL, szInfo);
					}
				}
			}
			else {
				if (bShowMessage) {
					LoadString(Resource->hLoad(), IDS_NOTPRIVATEADDR, szInfo, MAX_LOADSTRING);
					TaskTray->bDisplayBalloon(hWnd, NIIF_WARNING, NULL, szInfo);
				}
			}
			delete SynTP;
			SynTP = NULL;
			break;
		case SYNTPH_SENDERHOSNAMEIPV4:
		case SYNTPH_SENDERHOSNAMEIPV4_START:
		case SYNTPH_SENDERHOSNAMEIPV6:
		case SYNTPH_SENDERHOSNAMEIPV6_START:
			if (bCheckExistHostnameIPv4(Profile->lpstAppRegData->szSynTPSendHostname1, Profile->lpstAppRegData->dwSynTPTimeOut)) {
				SynTP->bStopSender();
				if (SynTP->bStartSender(hMainWnd, Profile->lpstAppRegData->szSynTPSendHostname1, Profile->lpstAppRegData->dwSynTPPortNo1)) {
					Profile->lpstAppRegData->bSynTPStarted1 = TRUE;
					break;
				}
				else {
					if (bShowMessage) {
						LoadString(Resource->hLoad(), IDS_CANTSYTPHELPER, szInfo, MAX_LOADSTRING);
						TaskTray->bDisplayBalloon(hWnd, NIIF_WARNING, NULL, szInfo);
					}
				}
			}
			else {
				if (bShowMessage) {
					LoadString(Resource->hLoad(), IDS_NOTHOSTNAME, szInfo, MAX_LOADSTRING);
					TaskTray->bDisplayBalloon(hWnd, NIIF_WARNING, NULL, szInfo);
				}
			}
			delete SynTP;
			SynTP = NULL;
			break;
		case SYNTPH_RECEIVERIPV4:
		case SYNTPH_RECEIVERIPV4_START:
		case SYNTPH_RECEIVERIPV6:
		case SYNTPH_RECEIVERIPV6_START:
			SynTP->vStopReceiver();
			if (SynTP->bStartReceiver(hMainWnd, Profile->lpstAppRegData->dwSynTPPortNo1)) {
				Profile->lpstAppRegData->bSynTPStarted1 = TRUE;
				break;
			}
			else {
				if (bShowMessage) {
					LoadString(Resource->hLoad(), IDS_CANTSYTPHELPER, szInfo, MAX_LOADSTRING);
					TaskTray->bDisplayBalloon(hWnd, NIIF_WARNING, NULL, szInfo);
				}
			}
			delete SynTP;
			SynTP = NULL;
			break;
		}
	}
	Profile->bSetProfileData();
	if (Profile->lpstAppRegData->bSynTPStarted1) {
		LPTSTR	lpszToolHints = new TCHAR[MAX_LOADSTRING];
		if (lpszToolHints) {
			ZeroMemory(lpszToolHints, MAX_LOADSTRING);
			_tcsncpy_s(lpszToolHints, MAX_LOADSTRING, szTitle, _TRUNCATE);
			LPTSTR	lpszBuff = new TCHAR[MAX_LOADSTRING];
			if (lpszBuff) {
				ZeroMemory(lpszBuff, MAX_LOADSTRING);
				LoadString(Resource->hLoad(), IDS_SYNTPRUNNING, lpszBuff, MAX_LOADSTRING);
				_tcsncat_s(lpszToolHints, MAX_LOADSTRING, lpszBuff, _TRUNCATE);
				TaskTray->bModifyToolHints(hWnd,lpszToolHints);
				delete []	lpszBuff;
			}
			delete []	lpszToolHints;
		}
	}
	return Profile->lpstAppRegData->bSynTPStarted1;
}

//
// bStopSynTPHelper()
//
BOOL		bStopSynTPHelper(HWND hWnd)
{
	if (SynTP) {
		SynTP->vStopReceiver();
		SynTP->bStopSender();
		delete SynTP;
		SynTP = NULL;
		if (Profile) {
			Profile->lpstAppRegData->bSynTPStarted1 = FALSE;
			Profile->bSetProfileData();
			if (!Profile->lpstAppRegData->bSynTPStarted1)	TaskTray->bModifyToolHints(hWnd, szTitle);
			return TRUE;
		}
		return TRUE;
	}
	return FALSE;
}

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
			if (TaskTray) {
				if (TaskTray->bGetTaskTrayWindowRect(hMainWnd, &rc)) {
					if (((pt.x >= rc.left) && (pt.x <= rc.right)) || ((pt.y <= rc.top) && (pt.y >= rc.bottom)))	return FALSE;
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

//
// class CMouseRawInput
//
//
// vRawInputMouseHandler()
//
void	CMouseRawInput::vRawInputMouseHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput)
{
	UNREFERENCED_PARAMETER(hWnd);
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
//
CPowerNotification::CPowerNotification(HWND hWnd)
	: hSuspendResumeNotification(NULL), hPowerSettingNotification(NULL), guidPowerSettingNotification(GUID_NULL)
{
	if ((hSuspendResumeNotification = RegisterSuspendResumeNotification(hWnd, DEVICE_NOTIFY_WINDOW_HANDLE)) == NULL) {
	}
	if ((hPowerSettingNotification = RegisterPowerSettingNotification(hWnd, &guidPowerSettingNotification, DEVICE_NOTIFY_WINDOW_HANDLE)) == NULL) {
	}
}

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
	
	TCHAR	CommandLine[] = L"/Start";
	switch (Type) {
	case PBT_APMSUSPEND:
		break;
	case PBT_APMRESUMEAUTOMATIC:
		break;
	case PBT_APMRESUMESUSPEND:
		bCreateProcess(FLUSHMOUSE_EXE, CommandLine);
		break;
	case PBT_POWERSETTINGCHANGE:
		break;
	case PBT_APMPOWERSTATUSCHANGE:
		SYSTEM_POWER_STATUS	PowerStatus{};
		if (GetSystemPowerStatus(&PowerStatus)) {
			switch (PowerStatus.ACLineStatus) {
			case 0:
				bCreateProcess(FLUSHMOUSE_EXE, CommandLine);
				break;
			case 1:
				bCreateProcess(FLUSHMOUSE_EXE, CommandLine);
				break;
			default:
				break;
			}
		}
		if (lpSetting != NULL) {
			PPOWERBROADCAST_SETTING	lpPwrSetting = reinterpret_cast<POWERBROADCAST_SETTING*>(lpSetting);
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
// class CEventHook
//
//
CEventHook::CEventHook()
	: hFormerWnd(NULL), hEventHook(NULL), hEventHookIME(NULL)
{
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
	: hHook64Dll(NULL), bShellHook64(FALSE), bGlobalHook64(FALSE), bKeyboardHookLL64(FALSE), bHook32Dll(FALSE),
	lpstProcessInfomation(new PROCESS_INFORMATION[sizeof(PROCESS_INFORMATION)])
{
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

	if ((bKeyboardHookLL64 = bKeyboardHookLLSet(hWnd)) != FALSE) {
		if ((bGlobalHook64 = bGlobalHookSet(hWnd)) != FALSE) {
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
	if (bKeyboardHookLL64)	vKeyboardHookLLUnset();
	if (bGlobalHook64)		bGlobalHookUnset();
	return TRUE;
}

//
// bHookUnset64()
//
BOOL		CFlushMouseHook::bHookUnset64() const
{
	if (bShellHook64)		bShellHookUnset();
	if (bKeyboardHookLL64)	vKeyboardHookLLUnset();
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
			ZeroMemory(lpszCommandLine, (size_t)(dwSize + COMAMANDLINESIZE));
			_sntprintf_s(lpszCommandLine, (dwSize + COMAMANDLINESIZE), _TRUNCATE, _T("%s %llu"), lpszBuffer, (unsigned long long)hWnd);
			if (lpstProcessInfomation) {
				STARTUPINFO	stStartupInfo{};	stStartupInfo.cb = sizeof(STARTUPINFO);
				if ((bRet = CreateProcess(NULL, lpszCommandLine, NULL, NULL, TRUE,
					NORMAL_PRIORITY_CLASS, NULL, NULL, &stStartupInfo, lpstProcessInfomation)) != FALSE) {
					for (int i = 5; i > 0; i--) {
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
BOOL 	CFlushMouseHook::bHook32DllStop() const
{
#define	TIMEOUT	3000 
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
						TerminateProcess(lpstProcessInfomation->hProcess, 0);
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

//
// bKBisEP()
//
BOOL		bKBisEP()
{
	if (GetKeyboardType(1) == 0) {
		_Post_equals_last_error_ DWORD err = GetLastError();
		if (err != 0) {
		}
		else {
			return TRUE;
		}
	}

	DWORD	dProcessID = 0;
	DWORD	dThreadID = GetWindowThreadProcessId(GetForegroundWindow(), &dProcessID);
	if (dThreadID != 0) {
		HKL	hkl = GetKeyboardLayout(dThreadID);
		if (hkl == US_ENG) {
			return TRUE;
		}
	}

#define SUBKEY	_T("SOFTWARE\\Microsoft\\Input\\Locales")
#define VALUE	_T("InputLocale")
	DWORD		dwInputLocale = 0;
	CRegistry	*CReg = new CRegistry;
	if (CReg) {
		if (!CReg->bReadSystemRegValueDWORD(HKEY_LOCAL_MACHINE, SUBKEY, VALUE, &dwInputLocale)) {
		}
		delete	CReg;
	}
	if ((dwInputLocale & 0x0000ffff) == LANG_IME)	return FALSE;
	return TRUE;
#undef SUBKEY
#undef VALUE
}

//
// bForExplorerPatcherSWS()
//
BOOL		bForExplorerPatcherSWS(HWND hForeWnd, BOOL bChangeToIME, BOOL bIMEModeForcedChange, LPHKL lpNewHKL, LPHKL lpPreviousHKL)
{
	if (hForeWnd == NULL)	return FALSE;
	BOOL	bRet = FALSE;
	HKL		hkl = NULL;
	HKL		hPreviousHKL = NULL;
	DWORD	dwProcessID = 0;
	DWORD	dwThreadID = 0;
	DWORD	dwInputLocale = 0;
	if ((dwThreadID = GetWindowThreadProcessId(hForeWnd, &dwProcessID)) != 0) {
		if ((hkl = GetKeyboardLayout(dwThreadID)) != NULL) {
			if ((dwInputLocale = Cime->dwGetInputLocale()) != 0) {
				hkl = (HKL)(((UINT64)hkl & KB_MASK) | (dwInputLocale & LANG_MASK));
				int	iKeyboardType = GetKeyboardType(1);
				if (iKeyboardType == 2)	hPreviousHKL = hkl;
				else hPreviousHKL = US_ENG;
				if ((bChangeToIME && (hkl != JP_IME)) || (hkl == US_ENG) || bIMEModeForcedChange) {
					hkl = JP_IME;
					if (ActivateKeyboardLayout(hkl, (KLF_SETFORPROCESS | KLF_REORDER)) != 0) {
						if ((hkl = GetKeyboardLayout(dwThreadID)) != NULL) {
							if (hkl != JP_IME) {
								if (bChangeHKLbySendInput(JP_IME, hkl, FALSE)) {
									hkl = JP_IME;
									if (ActivateKeyboardLayout(hkl, (KLF_SETFORPROCESS | KLF_REORDER)) != 0) {
										bRet = TRUE;
									}
								}
							}
							else {
								bRet = TRUE;
							}
						}
					}
				}
				else {
					if (ActivateKeyboardLayout(hkl, (KLF_SETFORPROCESS | KLF_REORDER)) != 0) {
						bRet = TRUE;
					}
				}
			}
		}
	}
	if (bRet) {
		EnumChildWindows(hForeWnd, &bEnumChildProcChangeHKL, (LPARAM)hkl);
		Cime->vActivateIME(hForeWnd);
	}
	HWND	hWnd = FindWindow(_T("Hidemaru32Class"), NULL);
	if ((hWnd != NULL) && (hForeWnd == hWnd)) {
		if (!bChangeHKLbySendInput(hkl, hPreviousHKL, FALSE)) {
		}
	}

	if (lpNewHKL != NULL)		*lpNewHKL = hkl;
	if (lpPreviousHKL != NULL)	*lpPreviousHKL = hPreviousHKL;
	return bRet;
}

//
// bChangeHKLbySendInput();
//
static BOOL	bChangeHKLbySendInput(HKL hNewHKL, HKL hPreviousHKL, BOOL bForce)
{
	BOOL	bRet = FALSE;
	int		iKBList = 0;
	if ((iKBList = GetKeyboardLayoutList(0, NULL)) != 0) {
		LPHKL	lpHKL = NULL;
		if ((lpHKL = new HKL[sizeof(HKL) * iKBList]) != NULL) {
			ZeroMemory(lpHKL, (sizeof(HKL) * iKBList));
			if (GetKeyboardLayoutList(iKBList, lpHKL) != 0) {
				int	iPreviousKB = 0, iNewKB = 0;
				for (int i = 0; i < iKBList; i++) {
					if (lpHKL[i] == hPreviousHKL)	iPreviousKB = i;
					if (lpHKL[i] == hNewHKL)		iNewKB = i;
				}
				int iKB = iKBList - iPreviousKB + iNewKB;
				if ((GetAsyncKeyState(VK_SHIFT) & 0x8000)) iKB = iKBList - iKB;
				if ((iNewKB != iPreviousKB) || bForce) {
					LPINPUT lpInputs = NULL;
					if ((lpInputs = new INPUT[sizeof(INPUT) * (iKB * 2 + 2)]) != NULL) {
						ZeroMemory(lpInputs, sizeof(INPUT) * (iKB * 2 + 2));
						lpInputs[0].type = INPUT_KEYBOARD;	lpInputs[0].ki.wVk = VK_LWIN;
						int i = 0;
						for (; i < ((iKB * 2)); i = i + 2) {
							lpInputs[i + 1].type = INPUT_KEYBOARD;	lpInputs[i + 1].ki.wVk = VK_SPACE;
							lpInputs[i + 2].type = INPUT_KEYBOARD;	lpInputs[i + 2].ki.wVk = VK_SPACE;	lpInputs[i + 2].ki.dwFlags = KEYEVENTF_KEYUP;
						}
						lpInputs[iKB * 2 + 1].type = INPUT_KEYBOARD;	lpInputs[iKB * 2 + 1].ki.wVk = VK_LWIN;	lpInputs[iKB * 2 + 1].ki.dwFlags = KEYEVENTF_KEYUP;
						if (bSendInputSub((UINT)(iKB * 2 + 2), lpInputs)) {
							bRet = TRUE;
						}
						if (lpInputs)	delete[]	lpInputs;
					}
				}
				else bRet = TRUE;

			}
			if (lpHKL)	delete[]	lpHKL;
		}
	}
	return bRet;
}

//
// bEnumChildProcChangeHKL()
//
static BOOL CALLBACK bEnumChildProcChangeHKL(HWND hWnd, LPARAM lParam)
{
	HWND	hIMWnd = NULL;
	if (ActivateKeyboardLayout((HKL)lParam, KLF_SETFORPROCESS) != 0) {
		if ((hIMWnd = ImmGetDefaultIMEWnd(hWnd)) != NULL) {
			SendMessage(hIMWnd, WM_INPUTLANGCHANGEREQUEST, (WPARAM)INPUTLANGCHANGE_SYSCHARSET, lParam);
		}
	}
	return TRUE;
}

//
// bCheckExistingJPIME()
//
BOOL		bCheckExistingJPIME()
{
	BOOL	bRet = FALSE;
	LPHKL	lpHKL = NULL;
	int		iKBList = 0;
	if ((iKBList = GetKeyboardLayoutList(0, NULL)) != 0) {
		if ((lpHKL = new HKL[sizeof(HKL) * iKBList]) != NULL) {
			ZeroMemory(lpHKL, (sizeof(HKL) * iKBList));
			if ((iKBList = GetKeyboardLayoutList(iKBList, lpHKL)) != 0) {
				for (int i = 0; i < iKBList; i++) {
					if (lpHKL[i] == JP_IME) {
						bRet = TRUE;
						break;
					}
				}
			}
			if (lpHKL)	delete[]	lpHKL;
		}
	}
	HWND	hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL);
	if (hWnd != NULL) {
		if (Profile != NULL) {
			if (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced) {
				SendMessage(hWnd, WM_CHECKEXISTINGJPIMEEX, (WPARAM)bRet, (LPARAM)NULL);
			}
			else {
				SendMessage(hWnd, WM_CHECKEXISTINGJPIMEEX, (WPARAM)FALSE, (LPARAM)NULL);
			}
		}
	}
	return bRet;
}

//
// bSendInputSub()
//
static BOOL		bSendInputSub(UINT cInputs, LPINPUT pInputs)
{
	if (SendInput(cInputs, pInputs, sizeof(INPUT)) == cInputs) {
		Sleep(10);
		return TRUE;
	}
	return FALSE;
}

//
// bChromium_Helper()
//
BOOL	bChromium_Helper(HWND hForeWnd)
{
	TCHAR	szBuffer[_MAX_PATH]{};
	if (GetClassName(hForeWnd, szBuffer, _MAX_PATH) != 0) {
		if ((szBuffer[0] != _T('\0')) && ((CompareStringOrdinal(szBuffer, -1, L"Chrome_RenderWidgetHostHWND", -1, TRUE) == CSTR_EQUAL)
			|| (CompareStringOrdinal(szBuffer, -1, L"Chrome_WidgetWin_1", -1, TRUE) == CSTR_EQUAL))) {
			DWORD	dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if (dwBeforeIMEMode != IMEOFF) {
				HWND    hIMWnd = NULL;
				if ((hIMWnd = ImmGetDefaultIMEWnd(hForeWnd)) != NULL) {
					DWORD	dwSentenceMode = 0;
					if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETOPENSTATUS, NULL) != 0) {
						if ((dwSentenceMode = (DWORD)SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETSENTENCEMODE, NULL)) == 0) {
							if (!SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETSENTENCEMODE, (LPARAM)IME_SMODE_PHRASEPREDICT) != 0) {
								return TRUE;
							}
							else return FALSE;
						}
						else return FALSE;
					}
					else return FALSE;
				}
				else return FALSE;
			}
			else return TRUE;
		}
		return TRUE;
	}
	return FALSE;
}

//
// bCreateProcess()
//
BOOL	 	bCreateProcess(LPCTSTR lpszExecName, LPTSTR lpCommandLine)
{
	BOOL	bRet = FALSE;
	DWORD	dwSize = 0;
	dwSize = ExpandEnvironmentStrings(lpszExecName, NULL, 0);
	LPTSTR	lpszBuffer = new TCHAR[dwSize];
	if (lpszBuffer) {
		ZeroMemory(lpszBuffer, dwSize);
		dwSize = ExpandEnvironmentStrings(lpszExecName, lpszBuffer, dwSize);
		if (dwSize < MAX_PATH) {
			size_t	size = 0x7fff;
			size = wcsnlen_s(lpCommandLine, size);
			if ((0 < size) && (size < (size_t)((size_t)0x7fff - (size_t)dwSize - sizeof(TCHAR)))) {
				size = (size_t)dwSize + size + 2;
				LPTSTR	_lpCommandLine = new TCHAR[size];
				if (_lpCommandLine) {
					ZeroMemory(_lpCommandLine, size);
					_sntprintf_s(_lpCommandLine, size, _TRUNCATE, _T("%s %s"), lpszBuffer, lpCommandLine);
					PROCESS_INFORMATION	ProcessInfomation{};	
					STARTUPINFO	StartupInfo{};		StartupInfo.cb = sizeof(STARTUPINFO);
#define	CREATIONFLAGS	0
					if (CreateProcess(NULL, _lpCommandLine, NULL, NULL, FALSE, CREATIONFLAGS, NULL, NULL, &StartupInfo, &ProcessInfomation) != FALSE) {
						CloseHandle(ProcessInfomation.hProcess);
						CloseHandle(ProcessInfomation.hThread);
						bRet = TRUE;
					}
					delete[] _lpCommandLine;
				}
			}
		}
		delete[] lpszBuffer;
	}
	return bRet;
}


/* = EOF = */