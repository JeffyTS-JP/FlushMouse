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
#include "Profile.h"
#include "Resource.h"
#include "Eventlog.h"
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
TCHAR		g_szWindowTitle[MAX_LOADSTRING]{ FLUSHMOUSE };
HWND		g_hMainWnd = NULL;

CProfile	*Profile = NULL;
CCursor		*Cursor = NULL;
CResource	*Resource = NULL;
CIME		*Cime = NULL;
CTaskTray	*TaskTray = NULL;
CSynTP		*SynTP = NULL;

BOOL		bIMEInConverting = FALSE;

//
// Local Data
//
constexpr UINT_PTR CHECKFOCUSTIMERID = 1;
static UINT     nCheckFocusTimerTickValue = 0;
static UINT_PTR nCheckFocusTimerID = CHECKFOCUSTIMERID;
static UINT_PTR	uCheckFocusTimer = NULL;

constexpr UINT_PTR CHECKPROCTIMERID = 2;
static UINT     nCheckProcTimerTickValue = 0;
static UINT_PTR nCheckProcTimerID = CHECKPROCTIMERID;
static UINT_PTR	uCheckProcTimer = NULL;

static CFlushMouseHook		*FlushMouseHook = NULL;
static CEventHook			*EventHook = NULL;
static CPowerNotification	*PowerNotification = NULL;
static CRawInputDevice		*RawInput = NULL;

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
static LRESULT	Cls_OnPowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting);
static void		Cls_OnInput(HWND hWnd, DWORD dwFlags, HRAWINPUT hRawInput);
static void		Cls_OnWTSSessionChange(HWND hWnd, DWORD dwSessionEvent, DWORD dwSessionID);
static void		Cls_OnDisplayChange(HWND hWnd, UINT bitsPerPixel, UINT cxScreen, UINT cyScreen);

// EX Message Handler
static BOOL		Cls_OnSettingsEx(HWND hWnd, int iCode, int iSubCode);
static void		Cls_OnInputLangChangeEx(HWND hWnd, UINT CodePage, HKL hkl);
static void		Cls_OnEventForegroundEx(HWND hWnd, DWORD dwEvent, HWND hForeWnd);
static void		Cls_OnCheckIMEStartConvertingEx(HWND hWnd, BOOL bStartConverting, DWORD vkCode);

// Sub
static VOID CALLBACK	vCheckFocusTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static VOID CALLBACK	vCheckProcTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static BOOL		bNotInTaskBar(LPCTSTR lpClassName);

//
//  bWinMain()
//
BOOL		bWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_opt_ LPWSTR lpCmdLine, _In_ int nShowCmd)
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
	if (!Resource || (LoadString(Resource->hLoad(), IDS_APP_TITLE, g_szWindowTitle, MAX_LOADSTRING) == 0)) {
		delete	Resource;
		Resource = NULL;
		return FALSE;
	}

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	if (!hPrevInstance) {
		if (!MyRegisterClass(hInstance)) {
			if (Resource)	delete	Resource;
			Resource = NULL;
			return FALSE;
		}
	}

	HWND	hWnd = NULL;
	if ((hWnd = InitInstance(hInstance, nShowCmd)) == NULL) {
		if (Resource)	delete	Resource;
		Resource = NULL;
		return FALSE;
	}
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
	g_hMainWnd = hWnd;

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
	Resource = NULL;
	return TRUE;
}

//
// iCheckCmdLine()
//
int			iCheckCmdLine(LPCTSTR lpCmdLine)
{
	HWND	hWnd = NULL;
	DWORD	dwProcessId = GetCurrentProcessId();
	if ((hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL)) != NULL) {
		SetFocus(GetLastActivePopup(hWnd));
		if (dwProcessId != GetWindowThreadProcessId(hWnd, NULL))	PostMessage(hWnd, WM_DESTROY, NULL, NULL);
		for (int i = 10; i > 0; i--) {
			Sleep(500);
			if ((hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL)) != NULL) {
				SetFocus(GetLastActivePopup(hWnd));
				if (dwProcessId != GetWindowThreadProcessId(hWnd, NULL))	PostMessage(hWnd, WM_DESTROY, NULL, NULL);
				else break;
				if (i == 1) {
					bReportEvent(MSG_FLUSHMOUSE_ALREADY_RUN, APPLICATION_CATEGORY);
					return (-1);
				}
			}
			else break;
		}
	}
	if ((*lpCmdLine != _T('\0')) && (_tcscmp(lpCmdLine, L"/Start") == 0)) {
		bReportEvent(MSG_START_FLUSHMOUSE_EVENT, Shortcut_CATEGORY);
		return (0);
	}
	if ((*lpCmdLine != _T('\0')) && (_tcscmp(lpCmdLine, L"/Quit") == 0)) {
		return (0);
	}
	return (1);
}

//
// bSetHeapInformation()
//
static BOOL		bSetHeapInformation()
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
#define		WINDOWSTYLE		WS_DISABLED
	HWND	hWnd = NULL;
	hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		CLASS_FLUSHMOUSE,
		g_szWindowTitle,
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
#define HANDLE_WM_POWERBROADCAST(hWnd, wParam, lParam, fn) (LRESULT)(DWORD)(BOOL)((fn)((hWnd), (ULONG)(wParam), (POWERBROADCAST_SETTING *)(lParam)))
#define HANDLE_WM_INPUT(hWnd, wParam, lParam, fn) ((fn)((hWnd), (DWORD)(GET_RAWINPUT_CODE_WPARAM(wParam)), (HRAWINPUT)(lParam)), 0L)
#define HANDLE_WM_WTSSESSION_CHANGE(hWnd, wParam, lParam, fn) ((fn)((hWnd), (DWORD)(wParam), (DWORD)(lParam)), 0L)

	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_POWERBROADCAST, Cls_OnPowerBroadcast);
		HANDLE_MSG(hWnd, WM_INPUT, Cls_OnInput);
		HANDLE_MSG(hWnd, WM_WTSSESSION_CHANGE, Cls_OnWTSSessionChange);
		HANDLE_MSG(hWnd, WM_DISPLAYCHANGE, Cls_OnDisplayChange);

		HANDLE_MSG(hWnd, WM_SETTINGSEX, Cls_OnSettingsEx);
		HANDLE_MSG(hWnd, WM_INPUTLANGCHANGEEX, Cls_OnInputLangChangeEx);
		HANDLE_MSG(hWnd, WM_EVENT_SYSTEM_FOREGROUNDEX, Cls_OnEventForegroundEx);
		HANDLE_MSG(hWnd, WM_CHECKIMESTARTCONVEX, Cls_OnCheckIMEStartConvertingEx);
		HANDLE_MSG(hWnd, WM_SYSKEYDOWNUPEX, Cls_OnSysKeyDownUpEx);
		break;

	default:
		if (TaskTray && (TaskTray->iCheckTaskTrayMessage(hWnd, message, wParam, lParam) == (-1))) {
			bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
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

#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_TOPMOST)

	g_hMainWnd = hWnd;

	CHANGEFILTERSTRUCT	cf{};
	cf.cbSize = sizeof(CHANGEFILTERSTRUCT);
	if (!ChangeWindowMessageFilterEx(hWnd, WM_SETTINGSEX, MSGFLT_ALLOW, &cf)) {
		vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE, __func__, __LINE__);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	if (!WTSRegisterSessionNotification(hWnd, NOTIFY_FOR_THIS_SESSION)) {
		vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE, __func__, __LINE__);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	Cime = new CIME;
	if (!Cime) {
		vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE, __func__, __LINE__);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	Profile = new CProfile;
	if (Profile) {
		if (!Profile->bFixChangedProfileData()) {
			vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE, __func__, __LINE__);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
		if (!Profile->bGetProfileData()) {
			vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE, __func__, __LINE__);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}

	constexpr UINT uiNumDevices = 2;
	RAWINPUTDEVICE	RawInputDevice[uiNumDevices]{};
	RawInputDevice[0].hwndTarget = hWnd;	RawInputDevice[0].dwFlags = (RIDEV_INPUTSINK | RIDEV_DEVNOTIFY);
	RawInputDevice[0].usUsagePage =  HID_USAGE_PAGE_GENERIC;	RawInputDevice[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	RawInputDevice[1].hwndTarget = hWnd;	RawInputDevice[1].dwFlags = (RIDEV_INPUTSINK | RIDEV_DEVNOTIFY);
	RawInputDevice[1].usUsagePage =  HID_USAGE_PAGE_GENERIC;	RawInputDevice[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
	RawInput = new CRawInputDevice();
	if (RawInput) {
		if (!RawInput->bRegisterRawInputDevices(RawInputDevice, uiNumDevices)) {
			vMessageBox(hWnd, IDS_NOTREGISTERMS, MessageBoxTYPE, __func__, __LINE__);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}

	if (!bStartThreadHookTimer(hWnd)) {
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);	
		return FALSE;
	}

	if ((bCheckExistingJPIME() && Profile->lpstAppRegData->bEnableEPHelper)) {
		bForExplorerPatcherSWS(GetForegroundWindow(), TRUE, TRUE, NULL, NULL);
	}

	TaskTray = new CTaskTray();
	if (TaskTray) {
		HICON	hIcon = NULL;
		if ((hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_FLUSHMOUSE))) != NULL) {
			BOOL	bRet = FALSE;
			for (int i = 0; i < 30; i++) {
				if ((bRet = TaskTray->bCreateTaskTrayWindow(hWnd, hIcon, g_szWindowTitle)))	break;
				Sleep(1000);
			}
			if (!bRet) {
				try {
					vMessageBox(hWnd, IDS_NOTREGISTERTT, MessageBoxTYPE, __func__, __LINE__);
				}
				catch (...) {
				}
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				return FALSE;
			}
		}
		else {
			vMessageBox(hWnd, IDS_NOTREGISTERTT, MessageBoxTYPE, __func__, __LINE__);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);	
			return FALSE;
		}
	}

	if (Profile != NULL) {
		if ((Profile->lpstAppRegData->dwSynTPHelper1 == SYNTPH_SENDERIPV4_START)
			|| (Profile->lpstAppRegData->dwSynTPHelper1 == SYNTPH_RECEIVERIPV4_START)
			|| (Profile->lpstAppRegData->dwSynTPHelper1 == SYNTPH_SENDERHOSNAMEIPV4_START)) {
			bStartSynTPHelper(g_hMainWnd, Profile->lpstAppRegData->dwSynTPHelper1, TRUE);
		}
	}

	PowerNotification = new CPowerNotification(hWnd);
	if (PowerNotification == NULL) {
		vMessageBox(hWnd, IDS_NOTRREGISTEVH, MessageBoxTYPE, __func__, __LINE__);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);	
		return FALSE;
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

	WTSUnRegisterSessionNotification(hWnd);

	PostQuitMessage(0);
}

//
// vDestroyWindow()
//
void		vDestroyWindow(HWND hWnd)
{
	if (Profile != NULL) {
		Profile->lpstAppRegData->bDisplayIMEModeOnCursor = FALSE;
		Profile->lpstAppRegData->dwDisplayIMEModeMethod = DisplayIMEModeMethod_RESOURCE;
		Profile->lpstAppRegData->bDoModeDispByMouseBttnUp = FALSE;
		Profile->lpstAppRegData->bDrawNearCaret = FALSE;
	}
	HWND	_hWnd = FindWindow(CLASS_FLUSHMOUSESETTINGS, NULL);
	if (_hWnd != NULL) {
		SendMessage(_hWnd, WM_DESTROY, 0, 0);
	}
	if (SynTP) {
		delete SynTP;
		SynTP = NULL;
	}
	if (RawInput != NULL) {
		delete RawInput;
		RawInput = NULL;
	}
	if (uCheckProcTimer != NULL) {
		if (KillTimer(hWnd, nCheckProcTimerID)) {
			uCheckProcTimer = NULL;
		}
	}
	vStopThreadHookTimer(hWnd);
	if (FlushMouseHook != NULL) {
		delete	FlushMouseHook;
		FlushMouseHook = NULL;
	}
	if (Cime != NULL) {
		delete Cime;
		Cime = NULL;
	}
	SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
	if (TaskTray != NULL) {
		if (!TaskTray->bDestroyTaskTrayWindow(hWnd)) {
		}
		delete TaskTray;
		TaskTray = NULL;
	}
	if (PowerNotification != NULL) {
		delete PowerNotification;
		PowerNotification = NULL;
	}
}

//
// WM_POWERBROADCAST
// Cls_OnPowerBroadcast()
//
static LRESULT	Cls_OnPowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting)
{
	return PowerNotification->PowerBroadcast(hWnd, Type, lpSetting);
}

// 
// WM_INPUT
// Cls_OnInput()
//
static void		Cls_OnInput(HWND hWnd, DWORD dwFlags, HRAWINPUT hRawInput)
{
	if (RawInput)	RawInput->vRawInputDevicesHandler(hWnd, dwFlags, hRawInput);
}

//
// WM_WTSSESSION_CHANGE
// Cls_OnWTSSessionChange()
//
static void		Cls_OnWTSSessionChange(HWND hWnd, DWORD dwSessionEvent, DWORD dwSessionID)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(dwSessionID);

	switch (dwSessionEvent) {
	case WTS_SESSION_LOCK:
		vStopThreadHookTimer(hWnd);
		break;
	case WTS_SESSION_UNLOCK:
		bStartThreadHookTimer(hWnd);
		TCHAR	CommandLine[] = L"/Start";
		bCreateProcess(FLUSHMOUSE_EXE, CommandLine);
		break;
	}
}

//
// WM_DISPLAYCHANGE
// Cls_OnDisplayChange()
//
static void	Cls_OnDisplayChange(HWND hWnd, UINT bitsPerPixel, UINT cxScreen, UINT cyScreen)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(bitsPerPixel);
	UNREFERENCED_PARAMETER(cxScreen);
	UNREFERENCED_PARAMETER(cyScreen);
	if (bCheckExistingJPIME() && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
		if (Profile->lpstAppRegData->bEnableEPHelper) {
			bForExplorerPatcherSWS(GetForegroundWindow(), TRUE, TRUE, NULL, NULL);
		}
		if (Profile->lpstAppRegData->bIMEModeForced) {
			bChromium_Helper(GetForegroundWindow());
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

	if (!Profile || !Cursor)	return;

	TCHAR	szBuffer[_MAX_PATH]{};
	if (Profile->lpstAppRegData->bSupportVirtualDesktop) {
		if (GetClassName(hForeground, szBuffer, _MAX_PATH) != 0) {
			if ((szBuffer[0] != _T('\0')) && (_tcscmp(szBuffer, L"TscShellContainerClass") == 0)) {
				return;
			}
		}
	}
	if (bCheckExistingJPIME() && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
		if (Profile->lpstAppRegData->bEnableEPHelper) {
			HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
			if ((!hWndHidemaru) || (hForeground != hWndHidemaru)) {
				bForExplorerPatcherSWS(hForeground, TRUE, FALSE, NULL, NULL);
			}
		}
		if (Profile->lpstAppRegData->bIMEModeForced) {
			bChromium_Helper(hForeground);
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
	if (!Profile || !Cursor || !EventHook)	return;
	if (Profile->lpstAppRegData->bSupportVirtualDesktop) {
		TCHAR	szBuffer[_MAX_PATH]{};
		if (GetClassName(hForeground, szBuffer, _MAX_PATH) != 0) {
			if ((szBuffer[0] != _T('\0')) && (_tcscmp(szBuffer, L"TscShellContainerClass") == 0)) {
				return;
			}
		}
	}
	if (Profile->lpstAppRegData->bDoModeDispByMouseBttnUp) {
		HWND	hWndObserved = NULL;
		POINT	pt{};
		if (GetCursorPos(&pt)) {
			if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
			if (!Cursor->bStartIMECursorChangeThread(hWndObserved))	return;
			if (!bCheckDrawIMEModeArea(hWndObserved))	return;
			if (!Cursor->bStartDrawIMEModeThread(hWndObserved))	return;
		}
	}
	return;
}

//
// WM_SETTINGSEX (WM_USER + 0xfe)
// Cls_OnSettingsEx()
//
static BOOL		Cls_OnSettingsEx(HWND hWnd, int iCode, int iSubCode)
{
	return bSettingsEx(hWnd, iCode, iSubCode);
}

//
// WM_INPUTLANGCHANGEEX
// Cls_OnInputLangChangeEx
//
static void		Cls_OnInputLangChangeEx(HWND hWnd, UINT CodePage, HKL hkl)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(CodePage);
	if (!Profile || !Cursor)	return;
	HWND	hForeWnd = NULL;
	HKL		hNewHKL = NULL;
	HKL		hPreviousHKL = NULL;
	if ((hForeWnd = GetForegroundWindow()) != NULL) {
		if ((bCheckExistingJPIME() && Profile->lpstAppRegData->bEnableEPHelper)) {
			if (hkl == US_ENG) {
				bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL);
			}
		}
		HWND	hWndObserved = NULL;
		POINT	pt{};
		if (Profile->lpstAppRegData->bDoModeDispByMouseBttnUp && GetCursorPos(&pt)) {
			if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
			if (Profile->lpstAppRegData->bDisplayFocusWindowIME) {
				hWndObserved = hForeWnd;
			}
			if (Cursor->bStartIMECursorChangeThread(hWndObserved)) {
				if (Profile->lpstAppRegData->bDoModeDispByIMEKeyDown) {
					if (!bCheckDrawIMEModeArea(hWndObserved))	return;
					if (!Cursor->bStartDrawIMEModeThread(hWndObserved))	return;
				}
			}
		}
	}
}

//
// WM_EVENT_SYSTEM_FOREGROUNDEX
// Cls_OnEventForegroundEx()
//
static void		Cls_OnEventForegroundEx(HWND hWnd, DWORD dwEvent, HWND hForeWnd)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(dwEvent);

	if (!Profile || !Cursor || !EventHook)	return;
	if (Cursor->bIsDisplayWindow(hForeWnd))	{
		return;
	}
	if (Profile->lpstAppRegData->bSupportVirtualDesktop) {
		TCHAR	szBuffer[_MAX_PATH]{};
		if (GetClassName(hForeWnd, szBuffer, _MAX_PATH) != 0) {
			if ((szBuffer[0] != _T('\0')) && (_tcscmp(szBuffer, L"TscShellContainerClass") == 0)) {
				return;
			}
		}
	}
	CURSORINFO	CursorInfo{ CursorInfo.cbSize = sizeof(CURSORINFO) };
	if (!GetCursorInfo(&CursorInfo)) {
		_Post_equals_last_error_ DWORD err = GetLastError();
		if (err == ERROR_ACCESS_DENIED) {
			return;
		}
		return;
	}
	if (Profile->lpstAppRegData->bOffChangedFocus) {
		if (Cime)	Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
	}
	if (CursorInfo.flags != CURSOR_SHOWING)	return;
	HWND hWndObserved = hGetObservedWnd();
	if (!hWndObserved)	return;
	if (!Cursor->bStartIMECursorChangeThreadWait(hWndObserved, 100))	return;
	if (Profile->lpstAppRegData->bDoModeDispByMouseBttnUp) {
		if (!bCheckDrawIMEModeArea(hWndObserved))	return;
#define	DELAYTIME	300
		if (!Cursor->bStartDrawIMEModeThreadWait(hForeWnd,(Profile->lpstAppRegData->dwAdditionalWaitTime + DELAYTIME)))	return;
#undef	DELAYTIME
	}
	return;
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
// WM_SYSKEYDOWNUPEX
// Cls_OnSysKeyDownUpEx()
//
void Cls_OnSysKeyDownUpEx(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(flags);
	
	if (!Profile || !Cursor)	return;
	if (((cRepeat & 0xfffe) != 0) || ((flags & 0x4000) != 0))	return;

	HKL		hNewHKL = NULL;
	HKL		hPreviousHKL = NULL;
	HWND	hWndObserved = NULL, hForeWnd = NULL;
	DWORD	dwBeforeIMEMode = IMEOFF;
	POINT	pt{};

	if ((fDown == FALSE)) {											// Key up
		switch (vk) {
		case KEY_TAB:					// Tab (0x09)
			if (bIMEInConverting)	return;
			if ((GetKeyState(VK_MENU) & 0x8000) || (GetKeyState(VK_LMENU) & 0x8000) || (GetKeyState(VK_RMENU) & 0x8000)
				|| (GetKeyState(VK_LWIN) & 0x8000) || (GetKeyState(VK_RWIN) & 0x8000)) {
				return;
			}
			break;
		case KEY_RETURN:				// Enter (0x0d)
			bIMEInConverting = FALSE;
			break;
		case KEY_ONLY_CTRLUP:			// VK_ONLY_CTRLUP	(0xe8)
			if (bIMEInConverting)	return;
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if (Profile->lpstAppRegData->bEnableEPHelper)	bForExplorerPatcherSWS(hForeWnd, FALSE, FALSE, NULL, NULL);
			if (GetCursorPos(&pt)) {
				if (Profile->lpstAppRegData->bDoModeDispByCtrlUp) {
					if (!Cursor->bStartDrawIMEModeThreadWait(WindowFromPoint(pt), Profile->lpstAppRegData->dwWaitWaveTime))	return;
				}
			}
			return;
		case KEY_ALT:					// ALT    (0x12)
		case KEY_LALT:					// ALT L  (0xa4)
		case KEY_RALT:					// ALT R  (0xa5)
			return;
		case KEY_LWIN:					// L Win  (0x5b)
		case KEY_RWIN:					// R Win  (0x5c)
		case KEY_CTRL:					// Ctrl   (0x11)
		case KEY_LCTRL:					// Ctrl L (0xa2)
		case KEY_RCTRL:					// Ctrl R (0xa3)
		case KEY_SHIFT:					// Shift  (0x10)
		case KEY_LSHIFT:				// Shift L(0xa0)
		case KEY_RSHIFT:				// Shift R(0xa1)
			if (bIMEInConverting)	return;
			hWndObserved = hGetObservedWnd();
			if (!hWndObserved)	return;
			if (!Cursor->bStartIMECursorChangeThread(hWndObserved))	return;
			return;
		case KEY_F5:					// F5				(0x74)
			if (bIMEInConverting)	return;
			break;
		case KEY_F6:					// F6				(0x75)
		case KEY_F7:					// F7				(0x76)
		case KEY_F8:					// F8				(0x77)
		case KEY_F9:					// F9				(0x78)
			if (bIMEInConverting)	return;
			if ((GetKeyState(VK_CONTROL) & 0x8000) || (GetKeyState(VK_LCONTROL) & 0x8000) || (GetKeyState(VK_RCONTROL) & 0x8000)) {
				break;
			}
			return;
		case KEY_IME_ON:				// IME ON			(0x16)
			if (bIMEInConverting)	return;
			if (Profile->lpstAppRegData->bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			}
			break;
		case KEY_KANJI:					// JP(IME/ENG) Alt + 漢字	(0x19)
			if (bIMEInConverting)	return;
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					SleepEx(50, FALSE);
				}
				else {
					if (!bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL))	return;

				}
				if (Profile->lpstAppRegData->bIMEModeForced) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						if (dwBeforeIMEMode != IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							bIMEInConverting = FALSE;
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							bChromium_Helper(hForeWnd);
						}
					}
					else {
						if (dwBeforeIMEMode != IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							bIMEInConverting = FALSE;
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							bChromium_Helper(hForeWnd);
						}
					}
				}
			}
			if (Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			break;
		case KEY_IME_OFF:				// IME OFF					(0x1a)
			if (bIMEInConverting)	return;
			if (Profile->lpstAppRegData->bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
				bIMEInConverting = FALSE;
			}
			break;
		case KEY_OEM_3:					// JP(IME/ENG) [@] / US(ENG) IME ON (0xc0) = ['] ALT + 半角/全角 or 漢字
		case KEY_OEM_8:					// JP(IME/ENG) [`] / UK(ENG) IME ON (0xdf) = ['] ALT + 半角/全角 or 漢字
		case KEY_OEM_PA1:				// US(ENG) 無変換			(0xeb)
			if (bIMEInConverting)	return;
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (dwBeforeIMEMode != IMEOFF) {
					if (Profile->lpstAppRegData->bIMEModeForced) {
						HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
						if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
							SleepEx(50, FALSE);
						}
						else {
							if (!bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL))	return;

						}
						Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						bIMEInConverting = FALSE;
					}
				}
			}
			break;
		case KEY_OEM_IME_OFF:				// JP(IME/ENG) IME OFF		(0xf3)
			if (bIMEInConverting)	return;
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					SleepEx(50, FALSE);
				}
				else {
					if (!bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL))	return;

				}
				if (Profile->lpstAppRegData->bIMEModeForced) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						if (dwBeforeIMEMode != IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							SleepEx(10, FALSE);
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							bIMEInConverting = FALSE;
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							SleepEx(10, FALSE);
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							bChromium_Helper(hForeWnd);
						}
					}
					else {
						if (dwBeforeIMEMode != IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							SleepEx(10, FALSE);
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							bIMEInConverting = FALSE;
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							bChromium_Helper(hForeWnd);
						}
					}
				}
			}
			if (Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			break;
		case KEY_OEM_IME_ON:				// JP(IME/ENG) IME ON		(0xf4)
			if (bIMEInConverting)	return;
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					SleepEx(50, FALSE);
				}
				else {
					if (!bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL))	return;

				}
				if (Profile->lpstAppRegData->bIMEModeForced) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						if (dwBeforeIMEMode != IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							SleepEx(10, FALSE);
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							bIMEInConverting = FALSE;
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							SleepEx(10, FALSE);
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							bChromium_Helper(hForeWnd);
						}
					}
					else {
						if (dwBeforeIMEMode != IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							SleepEx(10, FALSE);
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							bIMEInConverting = FALSE;
						}
						else {
							bChromium_Helper(hForeWnd);
						}
					}
				}
			}
			if (Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			break;
		case KEY_OEM_BACKTAB:				// OEM Alt+カタカナ/ひらがな	(0xf5)
		case KEY_FF:						// US(ENG) 変換/ひら/カタ	(0xff)
			if (bIMEInConverting)	return;
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				SetFocus(hForeWnd);
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					SleepEx(50, FALSE);
				}
				else {
					if (!bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL))	return;

				}
				if (Profile->lpstAppRegData->bIMEModeForced) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
						bChromium_Helper(hForeWnd);
					}
					else {
						Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						bIMEInConverting = FALSE;
					}
				}
			}
			if (Profile->lpstAppRegData->bForceHiragana)	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			break;
		default:
			return;
		}
	}
	else {															// Key down
		switch (vk) {
		case KEY_IME_ON:				// IME ON					(0x16)
		case KEY_CONVERT:				// JP(IME/ENG) 変換			(0x1c)
			if (bIMEInConverting && (Profile->lpstAppRegData->bIMEModeForced == FALSE)) {
				return;
			}
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (dwBeforeIMEMode == IMEOFF) {
					HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
					if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
						SleepEx(50, FALSE);
					}
					else {
						if (!bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL))	return;

					}
					if (Profile->lpstAppRegData->bIMEModeForced) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							SleepEx(10, FALSE);
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							bChromium_Helper(hForeWnd);
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							SleepEx(10, FALSE);
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							bChromium_Helper(hForeWnd);
						}
					}
				}
			}
			else {
				if (bIMEInConverting)	return;
			}
			if (Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			break;
		case KEY_NONCONVERT:			// JP(IME/ENG) 無変換		(0x1d)
			if (bIMEInConverting)	return;
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (dwBeforeIMEMode != IMEOFF) {
					HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
					if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
						SleepEx(50, FALSE);
					}
					else {
						if (!bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL))	return;

					}
					if (Profile->lpstAppRegData->bIMEModeForced) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							if (((UINT64)hPreviousHKL & LANG_MASK) != LANG_IME) {
								PostMessage(hWnd, WM_SYSKEYDOWNUPEX, KEY_OEM_PA1, (0x80000000));
								return;
							}
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							SleepEx(10, FALSE);
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							bIMEInConverting = FALSE;
						}
					}
				}
			}
			else {
				Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
				SleepEx(10, FALSE);
				Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
				bIMEInConverting = FALSE;
			}
			if ((GetKeyState(VK_SHIFT) & 0x8000) || (GetKeyState(VK_LSHIFT) & 0x8000) || (GetKeyState(VK_RSHIFT) & 0x8000)) {
				if (Profile->lpstAppRegData->bForceHiragana && (dwBeforeIMEMode != IMEOFF))
					Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
					bChromium_Helper(hForeWnd);
			}
			break;
		case KEY_OEM_ATTN:				// JP(IME/ENG) 英数/CapsLock(0xf0)
			if (bIMEInConverting)	return;
			if ((GetKeyState(VK_SHIFT) & 0x8000) || (GetKeyState(VK_LSHIFT) & 0x8000) || (GetKeyState(VK_RSHIFT) & 0x8000))	break;
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (dwBeforeIMEMode != IMEOFF) {
					HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
					if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
						SleepEx(50, FALSE);
					}
					else {
						if (!bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL))	return;

					}
					if (Profile->lpstAppRegData->bIMEModeForced) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							bIMEInConverting = FALSE;
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							bIMEInConverting = FALSE;
						}
					}
				}
				else {
					Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
					SleepEx(10, FALSE);
					Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
					bIMEInConverting = FALSE;
				}
			}
			break;
		case KEY_OEM_FINISH:			// JP(IME/ENG) OEM カタカナ	(0xf1)
			if (bIMEInConverting)	return;
			if ((Profile->lpstAppRegData->bIMEModeForced == FALSE)) {
				return;
			}
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					SleepEx(50, FALSE);
				}
				else {
					if (!bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL))	return;

				}
				if (Profile->lpstAppRegData->bIMEModeForced) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						SleepEx(10, FALSE);
						Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
						SleepEx(10, FALSE);
						Cime->vIMEConvertModeChangeForced(hForeWnd, ZENKANA_IMEON);
						bChromium_Helper(hForeWnd);
					}
					else {
						Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
						SleepEx(10, FALSE);
						Cime->vIMEConvertModeChangeForced(hForeWnd, ZENKANA_IMEON);
						bChromium_Helper(hForeWnd);
					}
				}
			}
			if (Profile->lpstAppRegData->bForceHiragana)	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			break;
		case KEY_OEM_COPY:				// JP(IME/ENG) OEM ひらがな	(0xf2)
			if (bIMEInConverting)	return;
			if ((Profile->lpstAppRegData->bIMEModeForced == FALSE)) {
				return;
			}
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					SleepEx(50, FALSE);
				}
				else {
					if (!bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL))	return;

				}
				if (Profile->lpstAppRegData->bIMEModeForced) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						SleepEx(10, FALSE);
						Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
						SleepEx(10, FALSE);
						Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
						bChromium_Helper(hForeWnd);
					}
					else {
						Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
						SleepEx(10, FALSE);
						Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
						bChromium_Helper(hForeWnd);
					}
				}
			}
			if (Profile->lpstAppRegData->bForceHiragana)	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			break;
		case KEY_OEM_IME_OFF:			// JP(IME/ENG) IME OFF		(0xf3)
		case KEY_OEM_IME_ON:			// JP(IME/ENG) IME ON		(0xf4)
			if (bIMEInConverting)	return;
			break;
		default:
			return;
		}
		}
	hWndObserved = hGetObservedWnd();
	if (!hWndObserved)	return;
	if (Profile->lpstAppRegData->bDoModeDispByIMEKeyDown) {
		if (!bIMEInConverting) {
			if (!Cursor->bStartDrawIMEModeThreadWait(hWndObserved, Profile->lpstAppRegData->dwAdditionalWaitTime))	return;	// error
		}
	}
	if (!Cursor->bStartIMECursorChangeThread(hWndObserved))	return;
	return;
}

//
// bStartThreadHookTimer()
//
BOOL		bStartThreadHookTimer(HWND hWnd)
{
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_TOPMOST)

	if (!Profile)	return FALSE;
	if (Cursor == NULL) {
		Cursor = new CCursor;
		if (!Cursor || !Cursor->bInitialize(hWnd)) {
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);	
			return FALSE;
		}
	}
	if (FlushMouseHook == NULL) {
		FlushMouseHook = new CFlushMouseHook;
		if (!FlushMouseHook || !FlushMouseHook->bHookSet(hWnd, FLUSHMOUSE_DLL, FLUSHMOUSE32_EXE)) {
			DWORD	dwErr = GetLastError();
			if ((dwErr == ERROR_MOD_NOT_FOUND) || (dwErr == ERROR_SHARING_VIOLATION) || (dwErr == ERROR_ENVVAR_NOT_FOUND)) {
				Sleep(3000);
				if (!FlushMouseHook || !FlushMouseHook->bHookSet(hWnd, FLUSHMOUSE_DLL, FLUSHMOUSE32_EXE)) {
					bReportEvent(MSG_THREAD_HOOK_TIMER_START_FAILED, APPLICATION_CATEGORY);
					vMessageBox(hWnd, IDS_NOTREGISTERHOOK, MessageBoxTYPE, __func__, __LINE__);
					PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
					return FALSE;
				}
			}
		}
	}
	if (EventHook == NULL) {
		EventHook = new CEventHook;
		if (!EventHook || !EventHook->bEventSet(hWnd)) {
			vMessageBox(hWnd, IDS_NOTRREGISTEVH, MessageBoxTYPE, __func__, __LINE__);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);	
			return FALSE;
		}
	}
	BOOL	bBool = FALSE;
	if (SetUserObjectInformation(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bBool, sizeof(BOOL)) != FALSE) {
		if (uCheckFocusTimer == NULL) {
			if ((uCheckFocusTimer = SetTimer(hWnd, nCheckFocusTimerID, Profile->lpstAppRegData->nCheckFocusTimerTickValue, (TIMERPROC)&vCheckFocusTimerProc)) == 0) {
				vMessageBox(hWnd, IDS_NOTIMERESOUCE, MessageBoxTYPE, __func__, __LINE__);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				return FALSE;
			}
		}
		if (uCheckProcTimer == NULL) {
			if ((uCheckProcTimer = SetTimer(hWnd, nCheckProcTimerID, Profile->lpstAppRegData->nCheckProcTimerTickValue, (TIMERPROC)&vCheckProcTimerProc)) == 0) {
				vMessageBox(hWnd, IDS_NOTIMERESOUCE, MessageBoxTYPE, __func__, __LINE__);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				return FALSE;
			}
		}
	}
	else {
		vMessageBox(hWnd, IDS_NOTIMERESOUCE, MessageBoxTYPE, __func__, __LINE__);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);		
		return FALSE;
	}
	return TRUE;
}

//
// vStopThreadHookTimer()
//
VOID	vStopThreadHookTimer(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);

	if (Cursor)	Cursor->vStopIMECursorChangeThread();

	if (EventHook != NULL) {
		delete EventHook;
		EventHook = NULL;
	}
	if (FlushMouseHook != NULL) {
		delete FlushMouseHook;
		FlushMouseHook = NULL;
	}
	if (Cursor != NULL) {
		delete Cursor;
		Cursor = NULL;
	}
	SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
}

//
// vCheckFocusTimerProc()
//
static VOID CALLBACK vCheckFocusTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(dwTime);

	if (!Profile || !Cursor)	return;
	if (uTimerID == nCheckFocusTimerID) {
		HWND hWndObserved = hGetObservedWnd();
		if (!hWndObserved)	return;
		if (!Cursor->bStartIMECursorChangeThreadTimer(hWndObserved))	return;
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
			bReportEvent(MSG_DETECT_FLUSHMOUSE_STOP, APPLICATION_CATEGORY);
			SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
			if (FlushMouseHook != NULL) {
				delete	FlushMouseHook;
				FlushMouseHook = NULL;
			}
			Sleep(1000);
			if (FlushMouseHook == NULL) {
				FlushMouseHook = new CFlushMouseHook;
				if (!FlushMouseHook || !FlushMouseHook->bHookSet(hWnd, FLUSHMOUSE_DLL, FLUSHMOUSE32_EXE)) {
					if (GetLastError() == ERROR_MOD_NOT_FOUND) {
						Sleep(3000);
						if (!FlushMouseHook || !FlushMouseHook->bHookSet(hWnd, FLUSHMOUSE_DLL, FLUSHMOUSE32_EXE)) {
							bReportEvent(MSG_THREAD_HOOK_TIMER_RESTART_FAILED, APPLICATION_CATEGORY);
							bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
							PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
							return;
						}
					}
				}
			}
		}
		if (bNotInTaskBar(CLASS_FLUSHMOUSE)) {
			if (bNotInTaskBar(CLASS_CURSORWINDOW)) {
				if (bNotInTaskBar(CLASS_CARETWINDOW)) {
					if (bNotInTaskBar(CLASS_MOUSEWINDOW)) {
						return;
					}

				}
			}
		}
		bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
	}
}

//
// bNotInTaskBar()
//
static BOOL bNotInTaskBar(LPCTSTR lpClassName)
{
	HWND	_hWnd = NULL;
	LONG	ExStyle = 0;
	if ((_hWnd = FindWindow(lpClassName, NULL)) != NULL) {
		if (!(GetWindowLong(_hWnd, GWL_STYLE) & WS_VISIBLE))	return TRUE;
		if (((ExStyle = GetWindowLong(_hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)))	return TRUE;
		if (ExStyle & WS_EX_NOREDIRECTIONBITMAP)	return TRUE;
		if (!(GetWindow(_hWnd, GW_OWNER) && (!(ExStyle & WS_EX_APPWINDOW) || IsIconic(_hWnd))))	return TRUE;
		return FALSE;
	}
	return TRUE;
}



/* = EOF = */