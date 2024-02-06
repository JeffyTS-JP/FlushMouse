//
// FlushMouse32.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/04/06  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "FlushMouse32.h"
#include "Resource.h"
#include "..\FlushMouseDLL32\MouseHookDLL32.h"
#include "..\FlushMouseLIB\CommonDef.h"
#include "..\FlushMouseDLL\EventlogData.h"

#ifdef _DEBUG
#include <stdlib.h>
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
#define PROCINITTIMERVALUE		5000
#define CHECKPROCTIMERID		2
static UINT     nCheckProcTimerTickValue = PROCINITTIMERVALUE;		// Timer tick
static UINT_PTR nCheckProcTimerID = CHECKPROCTIMERID;				// Timer ID
static UINT_PTR	uCheckProcTimer = NULL;

#define HOOKINITTIMERVALUE		300000
#define CHECKHOOKIMERID		3
static UINT     nCheckHookTimerTickValue = HOOKINITTIMERVALUE;		// Timer tick
static UINT_PTR nCheckHookTimerID = CHECKHOOKIMERID;				// Timer ID
static UINT_PTR	uCheckHookTimer = NULL;

//
// Struct Define
//

//
// Global Data
//

//
// Local Data
//
static TCHAR			szTitle[MAX_LOADSTRING]{};
static HINSTANCE		hInst = NULL;
static HWND				hParentWnd = NULL;

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

// Sub
static VOID CALLBACK	vCheckProcTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static VOID CALLBACK	vCheckHookTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static BOOL				bReportEvent(DWORD dwEventID, WORD wCategory);
static void				vMessageBox(HWND hWnd, UINT uID, UINT uType);

//
// wWinMain()
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)					// MessageBox style
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif	

	HANDLE	hHandle = GetCurrentProcess();
	if (hHandle != NULL) {
		if (!SetPriorityClass(hHandle, NORMAL_PRIORITY_CLASS)) {
			CloseHandle(hHandle);
			return (-1);
		}
		CloseHandle(hHandle);
	}

	bReportEvent(MSG_STARTING_FLUSHMOUSE, APPLICATION32_CATEGORY);

	if (*lpCmdLine == _T('\0')) {
		if ((hParentWnd = FindWindow(CLASS_FLUSHMOUSE, NULL)) == NULL) {
			vMessageBox(NULL, IDS_NOTFORKBY64, MessageBoxTYPE);
			return (-1);
		}
	}
	else {
		unsigned long long ll = 0;
		if ((ll = _tstoll(lpCmdLine)) == 0) return (-1);
		hParentWnd = (HWND)ll;
		if (hParentWnd != FindWindow(CLASS_FLUSHMOUSE, NULL)) {
			hParentWnd = NULL;
			bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION32_CATEGORY);
			return(-1);
		}
	}

	if (LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) return (-1);

	if (!hPrevInstance) {
		if (!MyRegisterClass(hInstance)) {
			return (-1);
		}
	}

	HWND	hWnd = NULL;
	if ((hWnd = FindWindow(CLASS_FLUSHMOUSE32, NULL)) != NULL) {
		SetFocus(GetLastActivePopup(hWnd));
		PostMessage(hWnd, WM_DESTROY, NULL, NULL);
		for (int i = 3; i > 0; i--) {
			Sleep(500);
			if ((hWnd = FindWindow(CLASS_FLUSHMOUSE32, NULL)) != NULL) {
				SetFocus(GetLastActivePopup(hWnd));
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);
				if (i == 1) {
					vMessageBox(NULL, IDS_ALREADYRUN, MessageBoxTYPE);
					return (-1);
				}
			}
			else break;
		}
	}

	if (!InitInstance(hInstance, nCmdShow)) {
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

	bReportEvent(MSG_STOPPED_FLUSHMOUSE, APPLICATION32_CATEGORY);
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
	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
	default:
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
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)
	
	if (!bMouseHookSet32(hParentWnd)) {
		vMessageBox(hWnd, IDS_NOTREGISTEHOOK, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	BOOL	bBool = FALSE;
	if (SetUserObjectInformation(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bBool, sizeof(BOOL)) != FALSE) {
		if (uCheckProcTimer == NULL) {
			if ((uCheckProcTimer = SetTimer(hWnd, nCheckProcTimerID, nCheckProcTimerTickValue, (TIMERPROC)&vCheckProcTimerProc)) == 0) {
				vMessageBox(hWnd, IDS_NOTREGISTEHOOK, MessageBoxTYPE);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				return FALSE;
			}
		}
		if (uCheckHookTimer == NULL) {
			if ((uCheckHookTimer = SetTimer(hWnd, nCheckHookTimerID, nCheckHookTimerTickValue, (TIMERPROC)&vCheckHookTimerProc)) == 0) {
				vMessageBox(hWnd, IDS_NOTREGISTEHOOK, MessageBoxTYPE);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				return FALSE;
			}
		}
	}
	else {
		vMessageBox(hWnd, IDS_NOTREGISTEHOOK, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	bReportEvent(MSG_STARTED_FLUSHMOUSE, APPLICATION32_CATEGORY);
	return TRUE;
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
static void Cls_OnDestroy(HWND hWnd)
{
	bReportEvent(MSG_QUIT_FLUSHMOUSE, APPLICATION32_CATEGORY);

	if (uCheckHookTimer != NULL) {
		if (KillTimer(hWnd, nCheckHookTimerID)) {
			uCheckHookTimer = NULL;
		}
	}
	
	if (uCheckProcTimer != NULL) {
		if (KillTimer(hWnd, nCheckProcTimerID)) {
			uCheckProcTimer = NULL;
		}
	}

	bMouseHookUnset32();

	PostQuitMessage(0);
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
		if (FindWindow(CLASS_FLUSHMOUSE, NULL) == NULL) {
			bReportEvent(MSG_DETECT_FLUSHMOUSE_STOP, APPLICATION32_CATEGORY);
			if (uCheckProcTimer != NULL) {
				if (KillTimer(hWnd, nCheckProcTimerID)) {
					uCheckProcTimer = NULL;
				}
			}
			NOTIFYICONDATA nIco{};
			nIco.cbSize = sizeof(NOTIFYICONDATA);
			nIco.hWnd = hParentWnd;
			nIco.uID = NOTIFYICONDATA_ID;
			nIco.guidItem = GUID_NULL;
			nIco.uFlags = 0;
			try {
				throw Shell_NotifyIcon(NIM_DELETE, &nIco);
			}
			catch (BOOL bRet) {
				if (!bRet) {
				}
			}
			catch (...) {
			}
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION32_CATEGORY);
		}
	}
	return;
}

//
// vCheckHookTimerProc()
//
static VOID CALLBACK vCheckHookTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(dwTime);

	if (uTimerID == nCheckHookTimerID) {
		bMouseHookUnset32();
		Sleep(100);
		bMouseHookSet32(hParentWnd);
	}
	return;
}

//
// bReportEvent()
//
static BOOL	bReportEvent(DWORD dwEventID, WORD wCategory)
{
	BOOL	bRet = FALSE;
	HANDLE	hEvent = RegisterEventSource(NULL, FLUSHMOUSE);
	if (hEvent != NULL) {
		if (ReportEvent(hEvent, (0x0000000c & (dwEventID >> 28)), wCategory, dwEventID, NULL, 0, 0, NULL, NULL) != 0) {
			bRet = TRUE;
		}
		else {
		}
		if (DeregisterEventSource(hEvent) == 0) {
			bRet = FALSE;
		}
	}
	return bRet;
}

//
// vMessageBox()
//
static void vMessageBox(HWND hWnd, UINT uID, UINT uType)
{
	TCHAR       lpText[MAX_LOADSTRING];
	try {
		throw LoadString(hInst, uID, lpText, MAX_LOADSTRING);
	}
	catch (int i) {
		if (i != 0) {
			try {
				throw MessageBox(hWnd, lpText, szTitle, uType);
			}
			catch (int) {
				return;
			}
			catch (...) {
				return;
			}
		}
	}
	catch (...) {
		return;
	}
}

/* = EOF = */