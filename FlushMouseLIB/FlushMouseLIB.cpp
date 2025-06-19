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
CTaskTray	*TaskTray = NULL;
CSynTP		*SynTP = NULL;

BOOL		bIMEInConverting = FALSE;

//
// Local Data
//
constexpr auto FOCUSINITTIMERVALUE = 200;
constexpr auto CHECKFOCUSTIMERID = 1;
static UINT     nCheckFocusTimerTickValue = FOCUSINITTIMERVALUE;
static UINT_PTR nCheckFocusTimerID = CHECKFOCUSTIMERID;
static UINT_PTR	uCheckFocusTimer = NULL;

constexpr auto PROCINITTIMERVALUE = 2000;
constexpr auto CHECKPROCTIMERID = 2;
static UINT     nCheckProcTimerTickValue = PROCINITTIMERVALUE;
static UINT_PTR nCheckProcTimerID = CHECKPROCTIMERID;
static UINT_PTR	uCheckProcTimer = NULL;

static CFlushMouseHook		*FlushMouseHook = NULL;
static CEventHook			*EventHook = NULL;
static CPowerNotification	*PowerNotification = NULL;
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
static LRESULT	Cls_OnPowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting);
static void		Cls_OnInput(HWND hWnd, DWORD dwFlags, HRAWINPUT hRawInput);
static void		Cls_OnWTSSessionChange(HWND hWnd, DWORD dwSessionEvent, DWORD dwSessionID);

// EX Message Handler
static BOOL		Cls_OnSettingsEx(HWND hWnd, int iCode, int iSubCode);
static void		Cls_OnInputLangChangeEx(HWND hWnd, UINT CodePage, HKL hkl);
static void		Cls_OnEventForegroundEx(HWND hWnd, DWORD dwEvent, HWND hForeWnd);
static void		Cls_OnCheckIMEStartConvertingEx(HWND hWnd, BOOL bStartConverting, DWORD vkCode);
static void		Cls_OnCheckExistingJPIMEEx(HWND hWnd, BOOL bEPHelper);
static void		Cls_OnSysKeyDownUpEx(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

// Sub
static VOID CALLBACK		vCheckFocusTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static VOID CALLBACK		vCheckProcTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
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
	if (LoadString(Resource->hLoad(), IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) {
		delete	Resource;
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
	if ((hWnd = InitInstance(hInstance, nShowCmd)) == NULL) {	// Window作成
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
			Sleep(1000);
			if ((hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL)) != NULL) {
				SetFocus(GetLastActivePopup(hWnd));
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);
				if (i == 1) {
					bReportEvent(MSG_FLUSHMOUSE_ALREADY_RUN, APPLICATION_CATEGORY);
					return (-1);
				}
			}
		}
	}
	if ((*lpCmdLine != _T('\0')) && (CompareStringOrdinal(lpCmdLine, -1, L"/Quit", -1, TRUE) == CSTR_EQUAL)) {
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
#define HANDLE_WM_POWERBROADCAST(hWnd, wParam, lParam, fn) (LRESULT)(DWORD)(BOOL)((fn)((hWnd), (ULONG)(wParam), (POWERBROADCAST_SETTING *)(lParam)))
#define HANDLE_WM_INPUT(hWnd, wParam, lParam, fn) ((fn)((hWnd), (DWORD)(GET_RAWINPUT_CODE_WPARAM(wParam)), (HRAWINPUT)(lParam)), 0L)
#define HANDLE_WM_WTSSESSION_CHANGE(hWnd, wParam, lParam, fn) ((fn)((hWnd), (DWORD)(wParam), (DWORD)(lParam)), 0L)

	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_POWERBROADCAST, Cls_OnPowerBroadcast);
		HANDLE_MSG(hWnd, WM_INPUT, Cls_OnInput);
		HANDLE_MSG(hWnd, WM_WTSSESSION_CHANGE, Cls_OnWTSSessionChange);

		HANDLE_MSG(hWnd, WM_SETTINGSEX, Cls_OnSettingsEx);
		HANDLE_MSG(hWnd, WM_INPUTLANGCHANGEEX, Cls_OnInputLangChangeEx);
		HANDLE_MSG(hWnd, WM_EVENT_SYSTEM_FOREGROUNDEX, Cls_OnEventForegroundEx);
		HANDLE_MSG(hWnd, WM_CHECKIMESTARTCONVEX, Cls_OnCheckIMEStartConvertingEx);
		HANDLE_MSG(hWnd, WM_CHECKEXISTINGJPIMEEX, Cls_OnCheckExistingJPIMEEx);
		HANDLE_MSG(hWnd, WM_SYSKEYDOWNUPEX, Cls_OnSysKeyDownUpEx);
			break;

		default:
			if (TaskTray && (TaskTray->iCheckTaskTrayMessage(hWnd, message, wParam, lParam) == (-1))) {
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_TOPMOST)
				vMessageBox(hWnd, IDS_NOTREGISTERTT, MessageBoxTYPE, __func__, __LINE__);
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

	hMainWnd = hWnd;

	CHANGEFILTERSTRUCT	cf{};
	cf.cbSize = sizeof(CHANGEFILTERSTRUCT);
	if (ChangeWindowMessageFilterEx(hWnd, WM_SETTINGSEX, MSGFLT_ALLOW, &cf)) {
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

#pragma warning(push)
#pragma warning(disable : 101)
	constexpr auto uiNumDevices = 1;
	RAWINPUTDEVICE	RawInputDevice[uiNumDevices]{};
	RawInputDevice[0].hwndTarget = hWnd;	RawInputDevice[0].dwFlags = (RIDEV_INPUTSINK | RIDEV_DEVNOTIFY);
	RawInputDevice[0].usUsagePage =  HID_USAGE_PAGE_GENERIC;	RawInputDevice[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	MouseRawInput = new CMouseRawInput();
	if (MouseRawInput) {
		if (!MouseRawInput->bRegisterRawInputDevices(RawInputDevice, uiNumDevices)) {
			vMessageBox(hWnd, IDS_NOTREGISTERMS, MessageBoxTYPE, __func__, __LINE__);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}
#pragma warning(pop)
	
	if (!bStartThreadHookTimer(hWnd)) {
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	if ((bCheckExistingJPIME() && Profile->lpstAppRegData->bEnableEPHelper)) {
		bForExplorerPatcherSWS(GetForegroundWindow(), TRUE, TRUE, NULL, NULL);
	}

	TaskTray = new CTaskTray(hWnd);
	if (TaskTray) {
		HICON	hIcon = NULL;
		if ((hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_FLUSHMOUSE))) != NULL) {
			BOOL	bRet = FALSE;
			for (int i = 0; i < 30; i++) {
				if ((bRet = TaskTray->bCreateTaskTrayWindow(hWnd, hIcon, szTitle)))	break;
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
			bStartSynTPHelper(hMainWnd, Profile->lpstAppRegData->dwSynTPHelper1, TRUE);
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
	if (MouseRawInput != NULL) {
		delete MouseRawInput;
		MouseRawInput = NULL;
	}
	if (uCheckProcTimer != NULL) {
		if (KillTimer(hWnd, nCheckProcTimerID)) {
			uCheckProcTimer = NULL;
		}
	}
	vStopThreadHookTimer(hWnd);
	if (FlushMouseHook != NULL) {
		FlushMouseHook->bHookUnset64();
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
	if (MouseRawInput)	MouseRawInput->vRawInputDevicesHandler(hWnd, dwFlags, hRawInput);
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
// WM_LBUTTONDOWNEX
// Cls_OnLButtonDownEx()
//
void Cls_OnLButtonDownEx(HWND hWnd, int x, int y, HWND hForeground)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(hForeground);
	if (!Profile)	return;
	if (bCheckExistingJPIME() && (Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
		if (Profile->lpstAppRegData->bEnableEPHelper) {
			bForExplorerPatcherSWS(hForeground, TRUE, FALSE, NULL, NULL);
		}
		if (Profile->lpstAppRegData->bIMEModeForced) {
			if (!bChromium_Helper(hForeground))	return;
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
	if (!Profile || !Cursor)	return;
	if (Profile->lpstAppRegData->bDoModeDispByMouseBttnUp) {
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
	UNREFERENCED_PARAMETER(dwEvent);

	if (!Profile || !Cursor || !EventHook)	return;
	if (EventHook->hFormerWnd != hForeWnd) {
		EventHook->hFormerWnd = hForeWnd;
		if (hWnd != hForeWnd) {
			HWND	hWndObserved = NULL;
			CURSORINFO	CursorInfo{ CursorInfo.cbSize = sizeof(CURSORINFO) };
			if (Profile->lpstAppRegData->bOffChangedFocus) {
				Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
			}
			if ((Profile) && Profile->lpstAppRegData->bDoModeDispByMouseBttnUp && GetCursorInfo(&CursorInfo)) {
				if (CursorInfo.flags != CURSOR_SHOWING)	return;
				if (Profile->lpstAppRegData->bDisplayFocusWindowIME) {
					hWndObserved = hForeWnd;
				}
				else {
					if ((hWndObserved = WindowFromPoint(CursorInfo.ptScreenPos)) == NULL)	return;
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
// WM_SYSKEYDOWNUPEX
// Cls_OnSysKeyDownUpEx()
//
static void Cls_OnSysKeyDownUpEx(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(flags);

	if (!Profile || !Cursor)	return;
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
			if (Profile->lpstAppRegData->bEnableEPHelper)	bForExplorerPatcherSWS(hForeWnd, FALSE, FALSE, NULL, NULL);
			if (Profile->lpstAppRegData->bDoModeDispByCtrlUp) {
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
			if (Profile->lpstAppRegData->bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			}
			if (bIMEInConverting)	return;
			break;
		case KEY_KANJI:				// JP(IME/ENG) Alt + 漢字	(0x19)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
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
			if (Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			break;
		case KEY_IME_OFF:			// IME OFF					(0x1a)
			if (Profile->lpstAppRegData->bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
			}
			if (bIMEInConverting)	return;
			break;
		case KEY_OEM_PA1:			// US(ENG) 無変換			(0xeb)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
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
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				Sleep(50);
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					Sleep(300);
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
			if (Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			Sleep(50);
			break;
		case KEY_OEM_IME_ON:		// JP(IME/ENG) IME ON  (0xf4)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				Sleep(50);
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					Sleep(300);
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
			if (Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			Sleep(50);
			break;
		case KEY_OEM_BACKTAB:		// OEM Alt+カタカナ/ひらがな	(0xf5)
		case KEY_FF:				// US(ENG) 変換/ひら/カタ	(0xff)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
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
			if (Profile->lpstAppRegData->bForceHiragana)	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
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
			if (bIMEInConverting && (Profile->lpstAppRegData->bIMEModeForced == FALSE)) {
				return;
			}
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
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
			if (Profile->lpstAppRegData->bForceHiragana && !(dwBeforeIMEMode != IMEOFF))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			break;
		case KEY_NONCONVERT:		// JP(IME/ENG) 無変換		(0x1d)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
				if (dwBeforeIMEMode != IMEOFF) {
					if (bForExplorerPatcherSWS(hForeWnd, TRUE, Profile->lpstAppRegData->bEnableEPHelper, &hNewHKL, &hPreviousHKL)) {
						if (Profile->lpstAppRegData->bIMEModeForced) {
							if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
								if (((UINT64)hPreviousHKL & LANG_MASK) != LANG_IME) {
									PostMessage(hWnd, WM_SYSKEYDOWNUPEX, KEY_OEM_PA1, (0x80000000));
									return;
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
			if (Profile->lpstAppRegData->bForceHiragana && (dwBeforeIMEMode != IMEOFF) && (GetAsyncKeyState(VK_SHIFT) & 0x8000))	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			break;
		case KEY_OEM_ATTN:			// JP(IME/ENG) 英数/CapsLock(0xf0)
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
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
			if ((Profile->lpstAppRegData->bIMEModeForced == FALSE)) {
				return;
			}
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
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
			if (Profile->lpstAppRegData->bForceHiragana)	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			break;
		case KEY_OEM_COPY:			// JP(IME/ENG) OEM ひらがな	(0xf2)
			if ((Profile->lpstAppRegData->bIMEModeForced == FALSE)) {
				return;
			}
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
			if ((Profile->lpstAppRegData->bEnableEPHelper || Profile->lpstAppRegData->bIMEModeForced)) {
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
			if (Profile->lpstAppRegData->bForceHiragana)	Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			if (bIMEInConverting)	return;
			break;
		default:
			return;
		}
	}
	HWND	hWndObserved = NULL;
	if (Profile->lpstAppRegData->bDisplayFocusWindowIME) {
		if ((hWndObserved = GetForegroundWindow()) == NULL)	return;
	}
	else {
		POINT	pt{};
		if (GetCursorPos(&pt)) {
			if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
		}
	}
	if (Cursor->bStartIMECursorChangeThread(hWndObserved)) {
		if (Profile->lpstAppRegData->bDoModeDispByIMEKeyDown) {
			if (!bIMEInConverting) {
				if (!Cursor->bStartDrawIMEModeThread(hWndObserved))	return;
			}
		}
	}
	return;
}

//
// bStartThreadHookTimer()
//
BOOL		bStartThreadHookTimer(HWND hWnd)
{
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_TOPMOST)
	
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
	if (!Profile || !bSetEnableIMEModeForcedLL64(Profile->lpstAppRegData->bIMEModeForced)) {
		vMessageBox(hWnd, IDS_NOTREGISTERHOOK, MessageBoxTYPE, __func__, __LINE__);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}
	if (EventHook == NULL) {
		EventHook = new CEventHook;
		if (!EventHook || !EventHook->bEventSet()) {
			vMessageBox(hWnd, IDS_NOTRREGISTEVH, MessageBoxTYPE, __func__, __LINE__);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}
	HWND	hFindWnd = FindWindow(CLASS_FLUSHMOUSE, NULL);
	if ((hFindWnd) && Profile) {
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
				vMessageBox(hWnd, IDS_NOTIMERESOUCE, MessageBoxTYPE, __func__, __LINE__);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				return FALSE;
			}
		}
		if (uCheckProcTimer == NULL) {
			if ((uCheckProcTimer = SetTimer(hWnd, nCheckProcTimerID, nCheckProcTimerTickValue, (TIMERPROC)&vCheckProcTimerProc)) == 0) {
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
	if (Cursor)	Cursor->vStopDrawIMEModeMouseByWndThread();

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
		HWND hWndObserved = NULL;
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