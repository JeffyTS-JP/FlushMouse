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
#include "..\FlushMouseLIB\CommonDef.h"
#include "..\FlushMouseDLL\EventlogData.h"
#include "..\FlushMouseDLL32\FlushMouseDll32.h"
#include "..\FlushMouseDLL32\MouseHookDll32.h"

#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#include <crtdbg.h>
#define new DEBUG_CLIENTBLOCK
#endif // _DEBUG

//
// Define
//
// Timer
#define PROCINITTIMERVALUE		1000
#define CHECKPROCTIMERID		2
static UINT     nCheckProcTimerTickValue = PROCINITTIMERVALUE;		// Timer tick
static UINT_PTR nCheckProcTimerID = CHECKPROCTIMERID;				// Timer ID
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
static TCHAR			szTitle[MAX_LOADSTRING]{};
static HINSTANCE		hInst = NULL;
static HWND			hParentWnd = NULL;

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
static BOOL				bReportEvent(DWORD dwEventID, WORD wCategory);
static BOOL	 			bCreateProcess(LPCTSTR lpszExecName);
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
	if (!SetPriorityClass(hHandle, NORMAL_PRIORITY_CLASS)) {
		return (-1);
	}

	if (*lpCmdLine == _T('\0')) {
		hParentWnd = NULL;
		vMessageBox(NULL, IDS_NOTFORKBY64, MessageBoxTYPE);
		return (-1);
	}
	else {
		unsigned long long ll = 0;
		if ((ll = _tstoll(lpCmdLine)) == 0) return (-1);
		hParentWnd = (HWND)ll;
		if (hParentWnd != FindWindow(CLASS_FLUSHMOUSE, NULL)) {
			vMessageBox(NULL, IDS_NOTFORKBY64, MessageBoxTYPE);
			return (-1);
		}
	}

	if (LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) return (-1);

	if (!hPrevInstance) {
		if (!MyRegisterClass(hInstance)) {
			return (-1);
		}
	}

	HWND	    hWnd = NULL;
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
	
	MSG		msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLUSHMOUSE32));
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

//
// MyRegisterClass()
//
static ATOM MyRegisterClass(HINSTANCE hInstance)
{
#define CLASSSTYLE CS_HREDRAW | CS_VREDRAW // クラススタイル

	WNDCLASSEX wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CLASSSTYLE;                                                // クラススタイル
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;                                                    // クラスの補足データなし
	wcex.cbWndExtra = 0;                                                    // ウィンドウの補足データなし
	wcex.hInstance = hInstance;                                             // クラスのウィンドウハンドル
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FLUSHMOUSE32));    // アイコンハンドル
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);								// マウスカーソルハンドル
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);               // ウィンドウ背景色
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_FLUSHMOUSE32);                  // デフォルトメニュー名
	wcex.lpszClassName = CLASS_FLUSHMOUSE32;								// このウインドウクラスにつける名前
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));    // 16×16の小さいサイズのアイコン

	return RegisterClassEx(&wcex);
}

//
// InitInstance(HINSTANCE, int)
//
static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	UNREFERENCED_PARAMETER(nCmdShow);
#define		WINDOWSTYLE		WS_OVERLAPPEDWINDOW  | WS_HSCROLL | WS_VSCROLL // | WS_VISIBLE // WS_MINIMIZE | WS_SYSMENU
	hInst = hInstance;							// グローバル変数にインスタンス ハンドルを格納する
	HWND		hWnd = NULL;						// メインウィンドウのハンドル
	hWnd = CreateWindowEx(
							WS_DISABLED,		// Disabled Window
							CLASS_FLUSHMOUSE32,	// RegisterClass()呼び出しを参照
							szTitle,            // Title barのテキスト
							WINDOWSTYLE,        // Window style
							0, 0,               // 水平・垂直位置
							0, 0,               // 幅・高さ
							NULL,				// 親オーバーラップウィンドウ
							NULL,				// ウィンドウクラスのメニューを使う
							hInstance,			// 所有インスタンス
							NULL);				// ポインタは不要
	if (!hWnd) {
		return FALSE;							// ウィンドウを作成できなかったときはFALSEを返す
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
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)						// MessageBox style
	if (!bMouseHookSet32(hParentWnd)) {	
		vMessageBox(hWnd, IDS_NOTREGISTEHOOK, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	BOOL		bBool = FALSE;
	if (SetUserObjectInformation(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bBool, sizeof(BOOL)) != FALSE) {
		// Set Timer for Proc
		if (uCheckProcTimer == NULL) {
			if ((uCheckProcTimer = SetTimer(hWnd, nCheckProcTimerID, nCheckProcTimerTickValue, (TIMERPROC)&vCheckProcTimerProc)) == 0) {
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

	return TRUE;
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
static void Cls_OnDestroy(HWND hWnd)
{
	bMouseHookUnset32();

	if (uCheckProcTimer != NULL) {
		if (KillTimer(hWnd, nCheckProcTimerID)) {
			uCheckProcTimer = NULL;
		}
	}
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
			bReportEvent(MSG_DETECT_FLUSHMOUSE_STOP, APPLICATION32_CATEGORY);
			bCreateProcess(FLUSHMOUSE_EXE);
			bReportEvent(MSG_RESTART_EVENT, APPLICATION32_CATEGORY);	
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		}
	}
	return;
}

//
// bReportEvent()
//
static BOOL	bReportEvent(DWORD dwEventID, WORD wCategory)
{
	BOOL		bRet = FALSE;
	HANDLE	hEvent = RegisterEventSource(NULL, _T("FlushMouse"));
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
// bCreateProcess()
//
static BOOL	 	bCreateProcess(LPCTSTR lpszExecName)
{
	BOOL		bRet = FALSE;
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