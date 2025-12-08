//
// FlushMouse32.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/04/06  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "FlushMouse32.h"
#include "Resource.h"
#include "..\FlushMouseLIB\CommonDef.h"
#include "..\FlushMouseDLL\EventlogData.h"
#include "..\MiscLIB\CWindow.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#define _CRTDBG_MAP_ALLOC
#define new DEBUG_CLIENTBLOCK
#endif

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

//
// Define
//
// Timer
constexpr UINT PROCINITTIMERVALUE = 2000;
constexpr UINT_PTR CHECKPROCTIMERID = 2;
static UINT     nCheckProcTimerTickValue = PROCINITTIMERVALUE;
static UINT_PTR nCheckProcTimerID = CHECKPROCTIMERID;
static UINT_PTR	uCheckProcTimer = NULL;

//
// Struct Define
//

//
// Global Data
//

//
// Local Data
//
static TCHAR			szTitle[MAX_LOADSTRING] = FLUSHMOUSE32;
static HINSTANCE		hInst = NULL;
static HWND				hParentWnd = NULL;
static UINT				uTaskbarCreated = 0;

static CPowerNotification32	*PowerNotification = NULL;

//
// Global Prototype Define
//

//
// Local Prototype Define
//
static ATOM             MyRegisterClass(HINSTANCE);
static BOOL             InitInstance(HINSTANCE, int);
static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// Handler
static BOOL				Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
static void				Cls_OnDestroy(HWND hWnd);
static LRESULT			Cls_OnPowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting);

// Sub
static VOID CALLBACK	vCheckProcTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static BOOL				bReportEvent(DWORD dwEventID, WORD wCategory);
static BOOL				bDestroyTaskTrayWindow(HWND hWnd);
static void				vMessageBox(HWND hWnd, UINT uID, UINT uType, LPCSTR lpFunc, DWORD dwLine);
static DWORD WINAPI		CheckProcWorker(LPVOID lpParameter);

//
// wWinMain()
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_TOPMOST)
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_ASSERTE(_CrtCheckMemory());
#endif

	HANDLE	hHandle = GetCurrentProcess();
	if (hHandle != NULL) {
		(void)SetPriorityClass(hHandle, NORMAL_PRIORITY_CLASS);
	}

	(void)bInitializeWindowCache();

	HWND	hWnd = NULL;
	if ((hWnd = FindWindow(CLASS_FLUSHMOUSE32, NULL)) != NULL) {
		SetFocus(GetLastActivePopup(hWnd));
		PostMessage(hWnd, WM_DESTROY, NULL, NULL);
		for (int i = 10; i > 0; i--) {
			Sleep(500);
			if ((hWnd = FindWindow(CLASS_FLUSHMOUSE32, NULL)) != NULL) {
				SetFocus(GetLastActivePopup(hWnd));
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);
				if (i == 1) {
					vMessageBox(NULL, IDS_ALREADYRUN, MessageBoxTYPE, NULL, 0);
					vUninitializeWindowCache();
					return (-1);
				}
			}
			else break;
		}
	}

	if (*lpCmdLine == _T('\0')) {
		if ((hParentWnd = FindWindow(CLASS_FLUSHMOUSE, NULL)) == NULL) {
			vMessageBox(NULL, IDS_NOTFORKBY64, MessageBoxTYPE, NULL, 0);
			vUninitializeWindowCache();
			return (-1);
		}
	}
	else {
		unsigned long long ll = 0;
		if ((ll = _tstoll(lpCmdLine)) == 0) {
			vUninitializeWindowCache();
			return (-1);
		}
		hParentWnd = (HWND)ll;
		if (hParentWnd != FindWindow(CLASS_FLUSHMOUSE, NULL)) {
			hParentWnd = NULL;
			bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION32_CATEGORY);
			vUninitializeWindowCache();
			return(-1);
		}
	}

	if (LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) {
		vUninitializeWindowCache();
		return (-1);
	}

	if (!hPrevInstance) {
		if (!MyRegisterClass(hInstance)) {
			vUninitializeWindowCache();
			return (-1);
		}
	}

	if (!InitInstance(hInstance, nCmdShow)) {
		vUninitializeWindowCache();
		return (-1);
	}

	MSG		msg{};
	BOOL	bRet = NULL;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == (-1)) {
			break;
		}
		else {
			HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLUSHMOUSE32));
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

#if defined _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return (int)msg.wParam;
}

//
// MyRegisterClass()
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FLUSHMOUSE32));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_FLUSHMOUSE32);
	wcex.lpszClassName = CLASS_FLUSHMOUSE32;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
// InitInstance(HINSTANCE, int)
//
static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	UNREFERENCED_PARAMETER(nCmdShow);
#define		WINDOWSTYLE		WS_OVERLAPPEDWINDOW  | WS_HSCROLL | WS_VSCROLL
	hInst = hInstance;
	HWND	hWnd = NULL;
	hWnd = CreateWindowEx(
					WS_DISABLED,
					CLASS_FLUSHMOUSE32,
					szTitle,
					WINDOWSTYLE,
					0, 0,
					0, 0,
					NULL,
					NULL,
					hInstance,
					NULL);
	if (!hWnd) {
		return FALSE;
	}
	return TRUE;
}

//
// WndProc(HWND, UINT, WPARAM, LPARAM)
//
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#define HANDLE_WM_POWERBROADCAST(hWnd, wParam, lParam, fn) (LRESULT)(DWORD)(BOOL)((fn)((hWnd), (ULONG)(wParam), (POWERBROADCAST_SETTING *)(lParam)))

	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_POWERBROADCAST, Cls_OnPowerBroadcast);
	default:
		if ((uTaskbarCreated != 0) && (message == uTaskbarCreated)) {
			if (uCheckProcTimer != NULL) {
				if (KillTimer(hWnd, nCheckProcTimerID)) {
					uCheckProcTimer = NULL;
				}
			}
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

	if ((uTaskbarCreated = RegisterWindowMessage(_T("TaskbarCreated"))) == 0) {
		vMessageBox(hWnd, IDS_NOTREGISTEHOOK, MessageBoxTYPE, __func__, __LINE__);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}
	PowerNotification = new CPowerNotification32(hWnd);
	if (PowerNotification == NULL) {
		vMessageBox(hWnd, IDS_NOTREGISTEHOOK, MessageBoxTYPE, __func__, __LINE__);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	BOOL	bBool = FALSE;
	if (SetUserObjectInformation(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bBool, sizeof(BOOL)) != FALSE) {
		if (uCheckProcTimer == NULL) {
			if ((uCheckProcTimer = SetTimer(hWnd, nCheckProcTimerID, nCheckProcTimerTickValue, (TIMERPROC)&vCheckProcTimerProc)) == 0) {
				vMessageBox(hWnd, IDS_NOTREGISTEHOOK, MessageBoxTYPE, __func__, __LINE__);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				return FALSE;
			}
		}
	}
	else {
		vMessageBox(hWnd, IDS_NOTREGISTEHOOK, MessageBoxTYPE, __func__, __LINE__);
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
	if (uCheckProcTimer != NULL) {
		if (KillTimer(hWnd, nCheckProcTimerID)) {
			uCheckProcTimer = NULL;
		}
	}

	if (PowerNotification != NULL) {
		delete PowerNotification;
		PowerNotification = NULL;
	}
	vInvalidateAllWindowCache();
	vUninitializeWindowCache();
	PostQuitMessage(0);
}

//
// WM_POWERBROADCAST
// Cls_OnPowerBroadcastEx()
//
static LRESULT		Cls_OnPowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting)
{
	return PowerNotification->PowerBroadcast(hWnd, Type, lpSetting);
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
		if (!QueueUserWorkItem(CheckProcWorker, (LPVOID)hWnd, WT_EXECUTEDEFAULT)) {
			bReportEvent(MSG_WORKER_QUEUE_FAIL, APPLICATION32_CATEGORY);
		}
	}
	return;
}

//
// CheckProcWorker()
//
static DWORD WINAPI CheckProcWorker(LPVOID lpParameter)
{
	HWND	_hWnd = reinterpret_cast<HWND>(lpParameter);
	HWND	_hWnd64 = hGetCachedWindowByClassName(CLASS_FLUSHMOUSE);
	if (!_hWnd64 || (_hWnd64 != hParentWnd)) {
		bReportEvent(MSG_DETECT_FLUSHMOUSE_STOP, APPLICATION32_CATEGORY);
		if (hParentWnd)	bDestroyTaskTrayWindow(hParentWnd);
		bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION32_CATEGORY);
		if (IsWindow(_hWnd)) {
			PostMessage(_hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		}
	}
	return 0;
}

//
// bDestroyTaskTrayWindow()
//
BOOL		bDestroyTaskTrayWindow(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);

	NOTIFYICONDATA nIco{};
	nIco.cbSize = sizeof(NOTIFYICONDATA);
	nIco.hWnd = hParentWnd;
	nIco.uID = NOTIFYICONDATA_ID;
	nIco.guidItem = GUID_NULL;
	nIco.uFlags = 0;

	BOOL bRet = Shell_NotifyIcon(NIM_DELETE, &nIco) != FALSE;
	return bRet ? TRUE : FALSE;
}

//
// bReportEvent()
//
static BOOL	bReportEvent(DWORD dwEventID, WORD wCategory)
{
	BOOL	bRet = FALSE;
	HANDLE	hEvent = RegisterEventSource(NULL, FLUSHMOUSE);
	if (!hEvent)	return FALSE;

	DWORD	severity = (dwEventID >> 30) & 0x3;
	WORD	wType = EVENTLOG_SUCCESS;
	switch (severity) {
	case STATUS_SEVERITY_INFORMATIONAL:
		wType = EVENTLOG_INFORMATION_TYPE;
		break;
	case STATUS_SEVERITY_WARNING:
		wType = EVENTLOG_WARNING_TYPE;
		break;
	case STATUS_SEVERITY_ERROR:
		wType = EVENTLOG_ERROR_TYPE;
		break;
	default:
			break;
	}
	if (ReportEvent(hEvent, wType, wCategory, dwEventID, NULL, 0, 0, NULL, NULL) != 0) {
		bRet = TRUE;
	}
	if (DeregisterEventSource(hEvent) == 0) {
		bRet = FALSE;
	}
	return bRet;
}
//
// vMessageBox()
//
static void vMessageBox(HWND hWnd, UINT uID, UINT uType, LPCSTR lpFunc, DWORD dwLine)
{
	TCHAR	szRes[MAX_LOADSTRING]{};
	TCHAR	szFuncT[MAX_LOADSTRING]{};
	TCHAR	lpText[(MAX_LOADSTRING * 2)]{};

	int	nLoaded = 0;
	if (hInst) {
		nLoaded = LoadString(hInst, uID, szRes, MAX_LOADSTRING);
	}
	if (nLoaded == 0) {
		return;
	}

	if (lpFunc && (dwLine != 0)) {
#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, lpFunc, -1, szFuncT, MAX_LOADSTRING);
#else
		_strncpy_s(szFuncT, MAX_LOADSTRING, lpFunc, _TRUNCATE);
#endif
		_sntprintf_s(lpText, (MAX_LOADSTRING * 2), _TRUNCATE, _T("%s\r\n\r\n (%s : %d : 0x%08X)"), szRes, szFuncT, dwLine, GetLastError());
	}
	else {
		_tcsncpy_s(lpText, (MAX_LOADSTRING * 2), szRes, _TRUNCATE);
	}

	(void)MessageBox(hWnd, lpText, szTitle, uType);
}

//
// class CPowerNotification
// CPowerNotification()
//
CPowerNotification32::CPowerNotification32(HWND hWnd)
	: hSuspendResumeNotification(NULL), hPowerSettingNotification(NULL), guidPowerSettingNotification(GUID_NULL)
{
	if ((hSuspendResumeNotification = RegisterSuspendResumeNotification(hWnd, DEVICE_NOTIFY_WINDOW_HANDLE)) == NULL) {
	}
	if ((hPowerSettingNotification = RegisterPowerSettingNotification(hWnd, &guidPowerSettingNotification, DEVICE_NOTIFY_WINDOW_HANDLE)) == NULL) {
	}
}

//
// ~CPowerNotification()
//
CPowerNotification32::~CPowerNotification32()
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
BOOL	CPowerNotification32::PowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(Type);
	UNREFERENCED_PARAMETER(lpSetting);

	HWND	_hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL);
	switch (Type) {
	case PBT_APMSUSPEND:
		if (hParentWnd)	bDestroyTaskTrayWindow(hParentWnd);
		break;
	case PBT_APMRESUMEAUTOMATIC:
		if (!_hWnd || (_hWnd != hParentWnd)) {
			if (hParentWnd)	bDestroyTaskTrayWindow(hParentWnd);
		}
		break;
	case PBT_APMRESUMESUSPEND:
		break;
	case PBT_POWERSETTINGCHANGE:
		break;
	case PBT_APMPOWERSTATUSCHANGE:
		SYSTEM_POWER_STATUS	PowerStatus{};
		if (GetSystemPowerStatus(&PowerStatus)) {
			switch (PowerStatus.ACLineStatus) {
			case 0:
				if (hParentWnd)	bDestroyTaskTrayWindow(hParentWnd);
				break;
			case 1:
				if (!_hWnd || (_hWnd != hParentWnd)) {
					if (hParentWnd)	bDestroyTaskTrayWindow(hParentWnd);
				}
				break;
			default:
				break;
			}
		}
	}
	return TRUE;
}


/* = EOF = */
