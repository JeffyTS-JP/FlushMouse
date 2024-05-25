//
// FlushMouseLIB.cpp
//		Copyright (C) 1993 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/07/09  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "FlushMouseLIB.h"
#include "FlushMouseSub.h"
#include "Cursor.h"
#include "TaskTray.h"
#include "Profile.h"
#include "Resource.h"
#include "Eventlog.h"
#include "KeyboardHook.h"
#include "CommonDef.h"
#include "..\FlushMouseDLL\GlobalHookDll.h"
#include "..\MiscLIB\CThread.h"
#include "..\MiscLIB\CRegistry.h"

//
// Define
//

//
// Struct Define
//

//
// Global Data
//
// Hook
TCHAR		szTitle[MAX_LOADSTRING] = FLUSHMOUSE;
HWND		hMainWnd = NULL;

CProfile	*Profile = NULL;
CCursor		*Cursor = NULL;	
CResource	*Resource = NULL;
CIME		*Cime = NULL;
CSynTP		*SynTP = NULL;

BOOL		bIMEInConverting = FALSE;

//
// Local Data
//
#define FOCUSINITTIMERVALUE		200
#define CHECKFOCUSTIMERID		1
static UINT     nCheckFocusTimerTickValue = FOCUSINITTIMERVALUE;
static UINT_PTR nCheckFocusTimerID = CHECKFOCUSTIMERID;
static UINT_PTR	uCheckFocusTimer = NULL;

#define PROCINITTIMERVALUE		2000
#define CHECKPROCTIMERID		2
static UINT     nCheckProcTimerTickValue = PROCINITTIMERVALUE;
static UINT_PTR nCheckProcTimerID = CHECKPROCTIMERID;
static UINT_PTR	uCheckProcTimer = NULL;

static CFlushMouseHook		*FlushMouseHook = NULL;
static CEventHook			*EventHook = NULL;
static CMouseRawInput		*MouseRawInput = NULL;

//
// Global Prototype Define
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

//
// Local Prototype Define
//
static BOOL		bSetHeapInformation();
static ATOM		MyRegisterClass(HINSTANCE hInstance);
static HWND		InitInstance(HINSTANCE hInstance, int nCmdShow);
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Handler
static BOOL		Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
static void		Cls_OnDestroy(HWND hWnd);
static void		Cls_OnInput(HWND hWnd, DWORD dwFlags, HRAWINPUT hRawInput);

// EX Message Handler
static void		Cls_OnSettingsEx(HWND hWnd, WPARAM wParam, LPARAM lParam);
static void		Cls_OnInputLangChangeEx(HWND hWnd, UINT CodePage, HKL hkl);
static void		Cls_OnEventForegroundEx(HWND hWnd, DWORD dwEvent, HWND hForeWnd);
static void		Cls_OnCheckIMEStartConvertingEx(HWND hWnd, BOOL bStartConverting, DWORD vkCode);
static void		Cls_OnCheckExistingJPIMEEx(HWND hWnd, BOOL bEPHelper);
static void		Cls_OnSysKeyDownUpEx(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

// Sub
static BOOL		bSendInputSub(UINT cInputs, LPINPUT pInputs);
static BOOL		bKBisEP();
static BOOL		bChangeHKLbySendInput(HKL hNewHKL, HKL hPreviousHKL, BOOL bForce);
static BOOL		bChromium_Helper(HWND hForeWnd);
static VOID CALLBACK		vCheckFocusTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static VOID CALLBACK		vCheckProcTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static BOOL CALLBACK		bEnumChildProcChangeHKL(HWND hWnd, LPARAM lParam);

//
//  bWinMain()
//
BOOL		bWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_opt_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	HANDLE	hHandle = GetCurrentProcess();
	if (hHandle != NULL) {
		if (!SetPriorityClass(hHandle, NORMAL_PRIORITY_CLASS)) {
			CloseHandle(hHandle);
			return FALSE;
		}
		CloseHandle(hHandle);
	}

	if (!bSetHeapInformation())	return FALSE;
	
	Resource = new CResource(FLUSHMOUSE_EXE);
	if (Resource->hLoad() == NULL) {
		if (Resource)	delete	Resource;
		return FALSE;
	}
	
	if (LoadString(Resource->hLoad(), IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) {
		if (Resource)	delete	Resource;
		return FALSE;
	}

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	if (!hPrevInstance) {
		if (!MyRegisterClass(hInstance)) {
			if (Resource)	delete	Resource;
			return FALSE;
		}
	}

	HWND	hWnd = NULL;
	if ((hWnd = InitInstance(hInstance, nCmdShow)) == NULL) {
		if (Resource)	delete	Resource;
		return FALSE;
	}
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
	hMainWnd = hWnd;

	MSG		msg{};
	BOOL	bRet = FALSE;
	while (TRUE) {
		try {
			bRet = GetMessage(&msg, NULL, 0, 0);
		}
		catch (...) {
			throw;
		}
		if ((bRet == 0) || (bRet == (-1))) {
			break;
		}
		else {
			HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLUSHMOUSE));
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	if (Resource)	delete	Resource;
	return TRUE;
}

//
// iCheckCmdLine()
//
int			iCheckCmdLine(LPCTSTR lpCmdLine)
{
	if ((*lpCmdLine != _T('\0')) && (CompareStringOrdinal(lpCmdLine, -1, L"/Start", -1, TRUE) == CSTR_EQUAL)) {
		bReportEvent(MSG_START_FLUSHMOUSE_EVENT, Shortcut_CATEGORY);
		return (0);
	}
	HWND	hWnd = NULL;
	if ((hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL)) != NULL) {
		SetFocus(GetLastActivePopup(hWnd));
		PostMessage(hWnd, WM_DESTROY, NULL, NULL);
		for (int i = 10; i > 0; i--) {
			Sleep(500);
			if ((hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL)) != NULL) {
				SetFocus(GetLastActivePopup(hWnd));
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);
				if (i == 1) {
					if ((*lpCmdLine != _T('\0')) && (CompareStringOrdinal(lpCmdLine, -1, L"/Quit", -1, TRUE) == CSTR_EQUAL)) {
						bReportEvent(MSG_QUIT_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
						return (0);
					}
					Resource = new CResource(FLUSHMOUSE_EXE);
					if (Resource->hLoad() != NULL) {
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL)
						vMessageBox(NULL, IDS_ALREADYRUN, MessageBoxTYPE);
						if (Resource)	delete	Resource;
					}
					return (-1);
				}
			}
		}
		return (0);
	}
	if ((*lpCmdLine != _T('\0')) && (CompareStringOrdinal(lpCmdLine, -1, L"/Quit", -1, TRUE) == CSTR_EQUAL)) {
		bReportEvent(MSG_QUIT_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
		return (0);
	}
	return (1);
}

//
// bSetHeapInformation()
//
BOOL	bSetHeapInformation()
{
	HANDLE	hHeap = NULL;
	if ((hHeap = GetProcessHeap()) != NULL) {
		ULONG	HeapInformation = 2;
		if (!HeapSetInformation(hHeap, HeapCompatibilityInformation, &HeapInformation, sizeof(ULONG))) {
			_Post_equals_last_error_ DWORD err = GetLastError();
			if (err != ERROR_INVALID_PARAMETER) {
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}

//
//  MyRegisterClass()
//
static ATOM MyRegisterClass(HINSTANCE hInstance)
{
#define CLASSSTYLE CS_HREDRAW | CS_VREDRAW

	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CLASSSTYLE;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_FLUSHMOUSE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_FLUSHMOUSE);
	wcex.lpszClassName = CLASS_FLUSHMOUSE;
	wcex.hIconSm = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
// InitInstance(HINSTANCE, int)
//
static HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	UNREFERENCED_PARAMETER(nCmdShow);
#define		WINDOWSTYLE		WS_DISABLED
	HWND	hWnd = NULL;
	hWnd = CreateWindowEx(
					WS_EX_TOOLWINDOW,
					CLASS_FLUSHMOUSE,
					szTitle,
					WINDOWSTYLE,
					0, 0,
					0, 0,
					NULL,
					NULL,
					hInstance,
					NULL);
	if (!hWnd) {
		return NULL;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return hWnd;
}

//
// WndProc(HWND, UINT, WPARAM, LPARAM)
//
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#define HANDLE_WM_INPUT(hWnd, wParam, lParam, fn) ((fn)((hWnd), (DWORD)(GET_RAWINPUT_CODE_WPARAM(wParam)), (HRAWINPUT)(lParam)), 0L)

	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
		HANDLE_MSG(hWnd, WM_INPUT, Cls_OnInput);

		HANDLE_MSG(hWnd, WM_SETTINGSEX, Cls_OnSettingsEx);
		HANDLE_MSG(hWnd, WM_INPUTLANGCHANGEEX, Cls_OnInputLangChangeEx);
		HANDLE_MSG(hWnd, WM_EVENT_SYSTEM_FOREGROUNDEX, Cls_OnEventForegroundEx);
		HANDLE_MSG(hWnd, WM_CHECKIMESTARTCONVEX, Cls_OnCheckIMEStartConvertingEx);
		HANDLE_MSG(hWnd, WM_CHECKEXISTINGJPIMEEX, Cls_OnCheckExistingJPIMEEx);
		HANDLE_MSG(hWnd, WM_SYSKEYDOWNUPEX, Cls_OnSysKeyDownUpEx);

		HANDLE_MSG(hWnd, WM_TASKTRAYEX, Cls_OnTaskTrayEx);
			break;

		default:
			if (!bCheckTaskTrayMessage(hWnd, message)) {
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL)
				vMessageBox(hWnd, IDS_NOTREGISTERTT, MessageBoxTYPE);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				break;
			}
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// WM_CREATE
// Cls_OnCreate()
//
static BOOL Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	UNREFERENCED_PARAMETER(lpCreateStruct);

#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL)

	hMainWnd = hWnd;

	Cime = new CIME;
	if (Cime == NULL) {
		vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}
	
	Profile = new CProfile;
	if (Profile != NULL) {
		if (!Profile->bGetProfileData()) {
			vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}

	HICON	hIcon = NULL;
	if ((hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_FLUSHMOUSE))) != NULL) {
		if (bCreateTaskTrayWindow(hWnd, hIcon, szTitle) == FALSE) {
			try {
				vMessageBox(hWnd, IDS_NOTREGISTERTT, MessageBoxTYPE);
			}
			catch (...) {
			}
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}
	else {
		vMessageBox(hWnd, IDS_NOTREGISTERTT, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

#define	uiNumDevices	1
	RAWINPUTDEVICE	RawInputDevice[uiNumDevices]{};
	RawInputDevice[0].hwndTarget = hWnd;	RawInputDevice[0].dwFlags = (RIDEV_INPUTSINK | RIDEV_DEVNOTIFY);
	RawInputDevice[0].usUsagePage =  HID_USAGE_PAGE_GENERIC;	RawInputDevice[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	MouseRawInput = new CMouseRawInput();
	if (MouseRawInput != NULL) {
		if (!MouseRawInput->bRegisterRawInputDevices(RawInputDevice, uiNumDevices)) {
			vMessageBox(hWnd, IDS_NOTREGISTERMS, MessageBoxTYPE);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);	// Quit
			return FALSE;
		}
	}
#undef uiNumDevices

	if (!bStartThreadHookTimer(hWnd)) {
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	if (((Profile != NULL) && bCheckExistingJPIME() && Profile->lpstAppRegData->bEnableEPHelper)) {
		bForExplorerPatcherSWS(GetForegroundWindow(), TRUE, TRUE, NULL, NULL);
	}

	if (Profile->lpstAppRegData->dwSynTPHelper1 == 0x02) {
		SynTP = new CSynTP(Profile->lpstAppRegData->dwSynTPPadX, Profile->lpstAppRegData->dwSynTPPadY, Profile->lpstAppRegData->dwSynTPEdgeX, Profile->lpstAppRegData->dwSynTPEdgeY);
		if (SynTP) {
			if (!SynTP->bStartSender(hMainWnd, Profile->lpstAppRegData->szSynTPSendIPAddr1, Profile->lpstAppRegData->dwSynTPPortNo1)) {
				vMessageBox(hWnd, IDS_CANTSYTPHELPER, MessageBoxTYPE);
				delete SynTP;
				SynTP = NULL;
			}
		}
	}
	else if (Profile->lpstAppRegData->dwSynTPHelper1 == 0x12) {
		if (bCheckExistHostnameIPv4(Profile->lpstAppRegData->szSynTPSendHostname1)) {
			SynTP = new CSynTP(Profile->lpstAppRegData->dwSynTPPadX, Profile->lpstAppRegData->dwSynTPPadY, Profile->lpstAppRegData->dwSynTPEdgeX, Profile->lpstAppRegData->dwSynTPEdgeY);
			if (SynTP) {
				if (!SynTP->bStartSender(hMainWnd, Profile->lpstAppRegData->szSynTPSendHostname1, Profile->lpstAppRegData->dwSynTPPortNo1)) {
					vMessageBox(hWnd, IDS_CANTSYTPHELPER, MessageBoxTYPE);
					delete SynTP;
					SynTP = NULL;
				}
			}
		}
	}
	else if (Profile->lpstAppRegData->dwSynTPHelper1 == 0x04) {
		SynTP = new CSynTP(Profile->lpstAppRegData->dwSynTPPadX, Profile->lpstAppRegData->dwSynTPPadY, Profile->lpstAppRegData->dwSynTPEdgeX, Profile->lpstAppRegData->dwSynTPEdgeY);
		if (SynTP) {
			if (!SynTP->bStartReceiver(hMainWnd, Profile->lpstAppRegData->dwSynTPPortNo1)) {
				vMessageBox(hWnd, IDS_CANTSYTPHELPER, MessageBoxTYPE);
				delete SynTP;
				SynTP = NULL;
			}
		}
	}

	return TRUE;
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
static void Cls_OnDestroy(HWND hWnd)
{
	vDestroyWindow(hWnd);

	if (Profile != NULL) {
		delete	Profile;
		Profile = NULL;
	}
	
	PostQuitMessage(0);
}

//
// vDestroyWindow()
//
void		vDestroyWindow(HWND hWnd)
{
	if (Profile != NULL) {
		Profile->lpstAppRegData->bDisplayIMEModeOnCursor = FALSE;
		Profile->lpstAppRegData->bDisplayIMEModeByWindow = FALSE;
		Profile->lpstAppRegData->bDoModeDispByMouseBttnUp = FALSE;
		Profile->lpstAppRegData->bDrawNearCaret = FALSE;
	}
	if (uCheckProcTimer != NULL) {
		if (KillTimer(hWnd, nCheckProcTimerID)) {
			uCheckProcTimer = NULL;
		}
	}
	if (SynTP) {
		delete SynTP;
		SynTP = NULL;
	}
	if (MouseRawInput != NULL) {
		delete MouseRawInput;
		MouseRawInput = NULL;
	}
	if (uCheckFocusTimer != NULL) {
		if (KillTimer(hWnd, nCheckFocusTimerID)) {
			uCheckFocusTimer = NULL;
		}
	}
	if (EventHook != NULL) {
		delete EventHook;
		EventHook = NULL;
	}
	if (FlushMouseHook != NULL) {
		delete	FlushMouseHook;
		FlushMouseHook = NULL;
	}
	if (Cursor != NULL) {
		delete Cursor;	
		Cursor = NULL;
	}
	if (Cime != NULL) {
		delete Cime;
		Cime = NULL;
	}
	SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
	if (!bDestroyTaskTrayWindow(hWnd)) {
	}
}

// 
// WM_INPUT
// Cls_OnInput()
//
static void		Cls_OnInput(HWND hWnd, DWORD dwFlags, HRAWINPUT hRawInput)
{
	if (MouseRawInput)	MouseRawInput->vRawInputDevicesHandler(hWnd, dwFlags, hRawInput);
}

//
// WM_INPUTLANGCHANGEEX
// Cls_OnInputLangChangeEx
//
static void		Cls_OnInputLangChangeEx(HWND hWnd, UINT CodePage, HKL hkl)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(CodePage);
	HWND	hForeWnd = NULL;
	HKL		hNewHKL = NULL;
	HKL		hPreviousHKL = NULL;
	if ((hForeWnd = GetForegroundWindow()) != NULL) {
		if (((Profile != NULL) && bCheckExistingJPIME() && Profile->lpstAppRegData->bEnableEPHelper)) {
			if (hkl == US_ENG) {
				bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL);
			}
		}
		HWND	hWndObserved = NULL;
		POINT	pt{};
		if (GetCursorPos(&pt)) {
			if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
			if ((Profile != NULL) && Profile->lpstAppRegData->bDisplayFocusWindowIME) {
				hWndObserved = hForeWnd;
			}
			if (Cursor->bStartIMECursorChangeThread(hWndObserved)) {
				if ((Profile != NULL) && Profile->lpstAppRegData->bDoModeDispByIMEKeyDown) {
					if (!bCheckDrawIMEModeArea(hWndObserved))	return;
					if (!Cursor->bStartDrawIMEModeThread(hWndObserved))	return;	
				}
			}
		}
	}
}

//
// WM_LBUTTONDOWNEX
// Cls_OnLButtonDownEx()
//
void Cls_OnLButtonDownEx(HWND hWnd, int x, int y, HWND hForeground)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(hForeground);
	TCHAR	szBuffer[_MAX_PATH]{};
	if (GetClassName(hForeground, szBuffer, _MAX_PATH) != 0) {
		if ((Profile != NULL) && bCheckExistingJPIME() && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
			if (Profile->lpstAppRegData->bEnableEPHelper) {
				bForExplorerPatcherSWS(hForeground, FALSE, FALSE, NULL, NULL);
			}
			if (Profile->lpstAppRegData->bIMEModeForced) {
				if (!bChromium_Helper(hForeground))	return;
			}
		}
	}
	return;
}

//
// WM_LBUTTONUPEX
// Cls_OnLButtonUpEx()
//
void Cls_OnLButtonUpEx(HWND hWnd, int x, int y, HWND hForeground)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(hForeground);
	if ((Profile != NULL) && Profile->lpstAppRegData->bDoModeDispByMouseBttnUp) {
		HWND	hWndObserved = NULL;
		POINT	pt{};
		if (GetCursorPos(&pt)) {
			if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
			if (!bCheckDrawIMEModeArea(hWndObserved))	return;
			if (!Cursor->bStartDrawIMEModeThreadWaitDblClk(hWndObserved))	return;
		}
	}
	return;
}

//
// WM_SETTINGSEX (WM_USER + 0xfe)
// Cls_OnSettingsEx()
//
static void		Cls_OnSettingsEx(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lParam);
	if ((wParam == 0) || (wParam == 2) || (wParam == 3)) {
		vSettingDialogApply();

		if (Profile != NULL) {
			Profile->bSetProfileData();
		}
		if ((wParam == 0) || (wParam == 2)) {
			if (!Cursor->bReloadCursor()) {
				vSettingDialogClose();
				bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
				return;
			}
		}
		else if (wParam == 3) {
			Cursor->vSetParamFromRegistry();
		}
	}
	if ((wParam == 0) || (wParam == 1)) {
		vSettingDialogClose();
	}
}

//
// WM_EVENT_SYSTEM_FOREGROUNDEX
// Cls_OnEventForegroundEx()
//
static void		Cls_OnEventForegroundEx(HWND hWnd, DWORD dwEvent, HWND hForeWnd)
{
	UNREFERENCED_PARAMETER(dwEvent);

	if(!EventHook)	return;
	if (EventHook->hFormerWnd != hForeWnd) {
		EventHook->hFormerWnd = hForeWnd;
		if (hWnd != hForeWnd) {
			HWND	hWndObserved = NULL;
			POINT	pt{};
			if ((Profile != NULL) && Profile->lpstAppRegData->bOffChangedFocus) {
				Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
			}
			if (GetCursorPos(&pt)) {
				if ((Profile != NULL) && Profile->lpstAppRegData->bDisplayFocusWindowIME) {
					hWndObserved = hForeWnd;
				}
				else {
					if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
				}
				if (!Cursor->bStartIMECursorChangeThread(hWndObserved))	return;
				if (!bCheckDrawIMEModeArea(hWndObserved))	return;
				if (!Cursor->bStartDrawIMEModeThreadWaitEventForeGround(hWndObserved))	return;
			}
		}
	}
}

//
// WM_CHECKIMESTARTCONVEX
// Cls_OnCheckIMEStartConvertingEx()
//
static void		Cls_OnCheckIMEStartConvertingEx(HWND hWnd, BOOL bStartConverting, DWORD vkCode)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(vkCode);

	bIMEInConverting = bStartConverting;
}

//
// WM_CHECKEXISTINGJPIMEEX
// Cls_OnCheckExistingJPIMEEx()
//
void		Cls_OnCheckExistingJPIMEEx(HWND hWnd, BOOL bEPHelper)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(bEPHelper);
	if (!bSetEnableEPHelperLL64(bEPHelper)) {
		bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
	}
}

//
// WM_SYSKEYDOWNEX
// Cls_OnSysKeyDownUpEx()
//
static void Cls_OnSysKeyDownUpEx(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(flags);

	if ((cRepeat & 0xfffe) != 0) {
		return;	
	}
	HKL		hNewHKL = NULL;
	HKL		hPreviousHKL = NULL;
	HWND	hForeWnd = NULL;
	DWORD	dwBeforeIMEMode = IMEOFF;
	if ((fDown == FALSE)) {											// Key up
		switch (vk) {
		case KEY_TAB:
			if (bIMEInConverting)	return;
			break;
		case KEY_RETURN:
			bIMEInConverting = FALSE;
			break;
		case KEY_ONLY_CTRLUP:
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if ((Profile != NULL) && Profile->lpstAppRegData->bEnableEPHelper)	bForExplorerPatcherSWS(hForeWnd, FALSE, FALSE, NULL, NULL);
			if ((Profile != NULL) && Profile->lpstAppRegData->bDoModeDispByCtrlUp) {
				POINT	pt{};
				if (GetCursorPos(&pt)) {
					if (!Cursor->bStartDrawIMEModeThreadWaitWave(WindowFromPoint(pt)))	return;
				}
			}
			return;
		case KEY_CTRL:
		case KEY_LCTRL:
		case KEY_RCTRL:
		case KEY_ALT:
		case KEY_LALT:
		case KEY_RALT:
			return;
		case KEY_F5:				// F5				(0x74)
		case KEY_F6:				// F6				(0x75)
		case KEY_F7:				// F7				(0x76)
		case KEY_F8:				// F8				(0x77)
		case KEY_F9:				// F9				(0x78)
		case KEY_F10:				// F10				(0x79)
			break;
		case KEY_IME_ON:			// IME ON			(0x16)
			if ((Profile != NULL) && Profile->lpstAppRegData->bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			}
			if (bIMEInConverting)	return;
			break;
		case KEY_KANJI:				// JP(IME/ENG) Alt + 漢字	(0x19)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile != NULL) && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
					if (Profile->lpstAppRegData->bIMEModeForced) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							if (dwBeforeIMEMode != IMEOFF) {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							}
							else {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
								Sleep(50);
								Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							}
						}
						else {
							if (dwBeforeIMEMode != IMEOFF) {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							}
							else {
								Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							}
						}
					}
				}
			}
			if ((Profile != NULL) && Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			break;
		case KEY_IME_OFF:			// IME OFF					(0x1a)
			if ((Profile != NULL) && Profile->lpstAppRegData->bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
			}
			if (bIMEInConverting)	return;
			break;
		case KEY_OEM_3:				// JP(IME/ENG) [@] / US(ENG) IME ON (0xc0) = ['] ALT + 半角/全角 or 漢字
		case KEY_OEM_8:				// JP(IME/ENG) [`] / UK(ENG) IME ON (0xdf) = ['] ALT + 半角/全角 or 漢字
			if (!bKBisEP())	return;
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if ((Profile != NULL) && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
					if (Profile->lpstAppRegData->bIMEModeForced) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							Sleep(50);
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
						}
						else {
							return;
						}
					}
				}
			}
			if (bIMEInConverting)	return;
			break;
		case KEY_OEM_PA1:			// US(ENG) 無変換			(0xeb)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile != NULL) && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (dwBeforeIMEMode != IMEOFF) {
					if (Profile->lpstAppRegData->bIMEModeForced) {
						if (bForExplorerPatcherSWS(hForeWnd, TRUE,Profile->lpstAppRegData-> bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
					}
				}
			}
			if (bIMEInConverting)	return;
			break;
		case KEY_OEM_IME_OFF:		// JP(IME/ENG) IME OFF (0xf3)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile != NULL) && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				Sleep(50);
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					Sleep(50);
					break;
				}
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
					if (Profile->lpstAppRegData->bIMEModeForced) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							if (dwBeforeIMEMode != IMEOFF) {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							}
							else {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
								Sleep(50);
								Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
								if (!bChromium_Helper(hForeWnd))	return;
							}
						}
						else {
							if (dwBeforeIMEMode != IMEOFF) {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							}
							else {
								Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
								Sleep(50);
								if (!bChromium_Helper(hForeWnd))	return;
							}
						}
					}
				}
			}
			if ((Profile != NULL) && Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			Sleep(50);
			break;
		case KEY_OEM_IME_ON:		// JP(IME/ENG) IME ON  (0xf4)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile != NULL) && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				Sleep(50);
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					Sleep(200);
					break;
				}
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
					if (Profile->lpstAppRegData->bIMEModeForced) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							if (dwBeforeIMEMode != IMEOFF) {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							}
							else {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
								Sleep(50);
								Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
								Sleep(50);
								if (!bChromium_Helper(hForeWnd))	return;
							}
						}
						else {
							if (dwBeforeIMEMode != IMEOFF) {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							}
							else {
								if (!bChromium_Helper(hForeWnd))	return;
							}
						}
					}
				}
			}
			if ((Profile != NULL) && Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			Sleep(50);
			break;
		case KEY_OEM_BACKTAB:		// OEM Alt+カタカナ/ひらがな	(0xf5)
		case KEY_FF:				// US(ENG) 変換/ひら/カタ	(0xff)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if ((Profile != NULL) && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				SetFocus(hForeWnd);
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
					if (Profile->lpstAppRegData->bIMEModeForced) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							Sleep(50);
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
					}
				}
			}
			if ((Profile != NULL) && Profile->lpstAppRegData->bForceHiragana)	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			break;
		default:
			return;
		}
	}
	else {															// Key down
		switch (vk) {
		case KEY_IME_ON:			// IME ON					(0x16)
		case KEY_CONVERT:			// JP(IME/ENG) 変換			(0x1c)
			if ((Profile != NULL) && bIMEInConverting && (Profile->lpstAppRegData->bIMEModeForced == FALSE)) {
				return;
			}
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile != NULL) && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (dwBeforeIMEMode == IMEOFF) {
					if (bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
						if (Profile->lpstAppRegData->bIMEModeForced) {
							if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
								Sleep(50);
								Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							}
							else {
								Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							}
						}
					}
				}
			}
			if ((Profile != NULL) && Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			break;
		case KEY_NONCONVERT:		// JP(IME/ENG) 無変換		(0x1d)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile != NULL) && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (dwBeforeIMEMode != IMEOFF) {
					if (bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
						if (Profile->lpstAppRegData->bIMEModeForced) {
							if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
								if (((UINT64)hPreviousHKL & LANG_MASK) != LANG_IME) {
									PostMessage(hWnd, WM_SYSKEYDOWNUPEX, KEY_OEM_PA1, (0x80000000));
								}
							}
							else {
								Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
								Sleep(50);
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							}
						}
					}
				}
				else {
					Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
					Sleep(50);
					Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
				}
			}
			if ((Profile != NULL) && Profile->lpstAppRegData->bForceHiragana && (dwBeforeIMEMode != IMEOFF) && (GetAsyncKeyState(VK_SHIFT) & 0x8000))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			break;
		case KEY_OEM_ATTN:			// JP(IME/ENG) 英数/CapsLock(0xf0)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile != NULL) && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (dwBeforeIMEMode != IMEOFF) {
					if (bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
						if (Profile->lpstAppRegData->bIMEModeForced) {
							if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							}
							else {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							}
						}
					}
				}
			}
			if (bIMEInConverting)	return;
			break;
		case KEY_OEM_FINISH:		// JP(IME/ENG) OEM カタカナ	(0xf1)
			if ((Profile != NULL) && (Profile->lpstAppRegData->bIMEModeForced == FALSE)) {
				return;
			}
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if ((Profile != NULL) && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
					if (Profile->lpstAppRegData->bIMEModeForced) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							Sleep(50);
							Cime->vIMEConvertModeChangeForced(hForeWnd, ZENKANA_IMEON);
							if (!bChromium_Helper(hForeWnd))	return;
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							Sleep(50);
							Cime->vIMEConvertModeChangeForced(hForeWnd, ZENKANA_IMEON);
							if (!bChromium_Helper(hForeWnd))	return;
						}
					}
				}
			}
			if ((Profile != NULL) && Profile->lpstAppRegData->bForceHiragana)	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			break;
		case KEY_OEM_COPY:			// JP(IME/ENG) OEM ひらがな	(0xf2)
			if ((Profile != NULL) && (Profile->lpstAppRegData->bIMEModeForced == FALSE)) {
				return;
			}
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if ((Profile != NULL) && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
					if (Profile->lpstAppRegData->bIMEModeForced) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							Sleep(50);
							Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
							if (!bChromium_Helper(hForeWnd))	return;
						}
						else {
							Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
							if (!bChromium_Helper(hForeWnd))	return;
						}
					}
				}
			}
			if ((Profile != NULL) && Profile->lpstAppRegData->bForceHiragana)	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			break;
		default:
			return;
		}
	}
	HWND	hWndObserved = NULL;
	if ((Profile != NULL) && Profile->lpstAppRegData->bDisplayFocusWindowIME) {
		if ((hWndObserved = GetForegroundWindow()) == NULL)	return;
	}
	else {
		POINT	pt{};
		if (GetCursorPos(&pt)) {
			if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
		}
	}
	if (Cursor != NULL) {
		if (Cursor->bStartIMECursorChangeThread(hWndObserved)) {
			if ((Profile != NULL) && Profile->lpstAppRegData->bDoModeDispByIMEKeyDown) {
				if (!bIMEInConverting) {
					if (!Cursor->bStartDrawIMEModeThread(hWndObserved))	return;
				}
			}
		}
	}
	return;
}

//
// bKBisEP()
//
static BOOL		bKBisEP()
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
BOOL	bChangeHKLbySendInput(HKL hNewHKL, HKL hPreviousHKL, BOOL bForce)
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
BOOL CALLBACK bEnumChildProcChangeHKL(HWND hWnd, LPARAM lParam)
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
static BOOL	bChromium_Helper(HWND hForeWnd)
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
// bStartThreadHookTimer()
//
BOOL		bStartThreadHookTimer(HWND hWnd)
{
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL)
	
	if (Cursor == NULL) {
		Cursor = new CCursor;
		if (!Cursor->bInitialize(hWnd)) {
			vMessageBox(hWnd, IDS_CANTLOADCURSOR, MessageBoxTYPE);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}

	if (FlushMouseHook == NULL) {
		FlushMouseHook = new CFlushMouseHook;
		if (!FlushMouseHook->bHookSet(hWnd, FLUSHMOUSE_DLL, FLUSHMOUSE32_EXE)) {
			vMessageBox(hWnd, IDS_NOTREGISTERHOOK, MessageBoxTYPE);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}

	if ((Profile == NULL) || !bSetEnableIMEModeForcedLL64(Profile->lpstAppRegData->bIMEModeForced)) {
		vMessageBox(hWnd, IDS_NOTREGISTERHOOK, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}


	if (EventHook == NULL) {
		EventHook = new CEventHook;
		if (!EventHook->bEventSet()) {
			vMessageBox(hWnd, IDS_NOTRREGISTEVH, MessageBoxTYPE);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}

	HWND	hFindWnd = FindWindow(CLASS_FLUSHMOUSE, NULL);
	if ((hFindWnd != NULL) && Profile != NULL) {
		if (Profile->lpstAppRegData->bEnableEPHelper) {
			SendMessage(hFindWnd, WM_CHECKEXISTINGJPIMEEX, (WPARAM)Profile->lpstAppRegData->bEnableEPHelper, (LPARAM)NULL);
		}
		else {
			SendMessage(hFindWnd, WM_CHECKEXISTINGJPIMEEX, (WPARAM)FALSE, (LPARAM)NULL);
		}
	}

	BOOL	bBool = FALSE;
	if (SetUserObjectInformation(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bBool, sizeof(BOOL)) != FALSE) {
		if (uCheckFocusTimer == NULL) {
			if ((uCheckFocusTimer = SetTimer(hWnd, nCheckFocusTimerID, nCheckFocusTimerTickValue, (TIMERPROC)&vCheckFocusTimerProc)) == 0) {
				vMessageBox(hWnd, IDS_NOTIMERESOUCE, MessageBoxTYPE);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				return FALSE;
			}
		}

		if (uCheckProcTimer == NULL) {
			if ((uCheckProcTimer = SetTimer(hWnd, nCheckProcTimerID, nCheckProcTimerTickValue, (TIMERPROC)&vCheckProcTimerProc)) == 0) {
				vMessageBox(hWnd, IDS_NOTIMERESOUCE, MessageBoxTYPE);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				return FALSE;
			}
		}
	}
	else {
		vMessageBox(hWnd, IDS_NOTIMERESOUCE, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	return TRUE;
}

//
// vCheckFocusTimerProc()
//
static VOID CALLBACK vCheckFocusTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(dwTime);
	if (uTimerID == nCheckFocusTimerID) {
		HWND hWndObserved = NULL;
		if (Profile != NULL) {
			if (Profile->lpstAppRegData->bDisplayFocusWindowIME) {
				if ((hWndObserved = GetForegroundWindow()) == NULL)	return;
			}
			else {
				POINT	pt{};
				if (!GetCursorPos(&pt))	return;
				if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
			}
			if (!Cursor->bStartIMECursorChangeThread(hWndObserved))	return;
		}
	}
	return;
}

//
// vCheckProcTimerProc()
//
static VOID CALLBACK vCheckProcTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(dwTime);

	if (uTimerID == nCheckProcTimerID) {
		if (FindWindow(CLASS_FLUSHMOUSE32, NULL) == NULL) {
			if (FlushMouseHook != NULL) {
				delete	FlushMouseHook;
				FlushMouseHook = NULL;
			}
			Sleep(1000);
			if (FlushMouseHook == NULL) {
				FlushMouseHook = new CFlushMouseHook;
				if (!FlushMouseHook->bHookSet(hWnd, FLUSHMOUSE_DLL, FLUSHMOUSE32_EXE)) {
					PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				}
			}
		}

	}
}

//
// bCreateProcess()
//
BOOL	 	bCreateProcess(LPCTSTR lpszExecName)
{
	BOOL	bRet = FALSE;
	DWORD	dwSize = 0;
	dwSize = ExpandEnvironmentStrings(lpszExecName, NULL, 0);
	LPTSTR	lpszBuffer = new TCHAR[dwSize];
	if (lpszBuffer) {
		ZeroMemory(lpszBuffer, dwSize);
		dwSize = ExpandEnvironmentStrings(lpszExecName, lpszBuffer, dwSize);
		PROCESS_INFORMATION	ProcessInfomation{};
		STARTUPINFO	StartupInfo{};		StartupInfo.cb = sizeof(STARTUPINFO);
		if (CreateProcess(lpszBuffer, NULL, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &StartupInfo, &ProcessInfomation) != FALSE) {
			CloseHandle(ProcessInfomation.hProcess);
			CloseHandle(ProcessInfomation.hThread);
			bRet = TRUE;
		}
		delete[]		lpszBuffer;
	}
	return bRet;
}

//
// Class CResource
//
CResource::CResource()
{
	hModRes = NULL;
	iResourceLoadCount = 0;
	szResFile = NULL;
}

CResource::CResource(LPCTSTR lpszResFile)
{
	hModRes = NULL;
	iResourceLoadCount = 0;
	szResFile = new TCHAR[_MAX_PATH];
	if (szResFile) {
		ZeroMemory(szResFile, sizeof(TCHAR) * _MAX_PATH);
		_tcsncpy_s(szResFile, _MAX_PATH, lpszResFile, _TRUNCATE);
	}
}

CResource::~CResource()
{
	if (Resource != NULL) {
		Resource->bUnload();
		Resource = NULL;
	}
	hModRes = NULL;
	iResourceLoadCount = 0;
	if (szResFile)	delete[]	szResFile;
	szResFile = NULL;
}

//
// hLoad()
//
HMODULE		CResource::hLoad()
{
	if ((hModRes == NULL) && (iResourceLoadCount == 0)) {
		if ((hModRes = LoadLibraryEx(szResFile, NULL, (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE))) == NULL) {
			return NULL;
		}
	}
	++iResourceLoadCount;
	return hModRes;
}

//
// bUnload()
//
BOOL		CResource::bUnload()
{
	if ((hModRes != NULL) && (iResourceLoadCount == 1)) {
		if (!FreeLibrary(hModRes)) {
			return FALSE;
		}
	}
	--iResourceLoadCount;
	return TRUE;
}

/* = EOF = */