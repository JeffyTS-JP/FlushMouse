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
#include "CommonDef.h"
#include "..\FlushMouseDLL\EventlogDll.h"
#include "..\FlushMouseDLL\GlobalHookDll.h"
#include "..\FlushMouseDLL\KeyboardHookDll.h"
#include "..\FlushMouseDLL32\MouseHookDll32.h"
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
TCHAR		szTitle[MAX_LOADSTRING]{};					// タイトル バーのテキスト
HWND		hMainWnd = NULL;

CProfile	* Profile = NULL;							// Profile class
CCursor		* Cursor = NULL;							// Cursor class
CResource	* Resource = NULL;							// Resource class
CIME		* Cime = NULL;								// IME class

// Use in FlushMouse from Registry
BOOL		bDisplayIMEModeOnCursor = TRUE;
BOOL		bOffChangedFocus = FALSE;
BOOL		bDoModeDispByIMEKeyDown = FALSE;
BOOL		bDoModeDispByCtrlUp = FALSE;
BOOL		bDisplayFocusWindowIME = FALSE;
BOOL		bDoModeDispByMouseBttnUp = FALSE;
BOOL		bDrawNearCaret = FALSE;
BOOL		bEnableEPHelper = FALSE;
BOOL		bMoveIMEToolbar = FALSE;
BOOL		bIMEModeForced = FALSE;

//
// Local Data
//
// Timer for Cursor
#define FOCUSINITTIMERVALUE		200
#define CHECKFOCUSTIMERID		1
static UINT     nCheckFocusTimerTickValue = FOCUSINITTIMERVALUE;	// Timer tick
static UINT_PTR nCheckFocusTimerID = CHECKFOCUSTIMERID;				// Timer ID
static UINT_PTR	uCheckFocusTimer = NULL;
// Timer for Poc
#define PROCINITTIMERVALUE		1000
#define CHECKPROCTIMERID		2
static UINT     nCheckProcTimerTickValue = PROCINITTIMERVALUE;		// Timer tick
static UINT_PTR nCheckProcTimerID = CHECKPROCTIMERID;				// Timer ID
static UINT_PTR	uCheckProcTimer = NULL;

// Hook
static CFlushMouseHook* FlushMouseHook = NULL;

// Event Handler
static CEventHook* EventHook = NULL;

// for PowerNotification
static CPowerNotification* PowerNotification = NULL;

//
// Global Prototype Define
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

//
// Local Prototype Define
//
static ATOM		MyRegisterClass(HINSTANCE hInstance);
static HWND		InitInstance(HINSTANCE hInstance, int nCmdShow);
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK AboutDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Handler
static BOOL		Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
static void		Cls_OnDestroy(HWND hWnd);
static void		Cls_OnKillFocus(HWND hWnd, HWND hWndOldFocus);
static void		Cls_OnSetFocus(HWND hWnd, HWND hWndNewFocus);
static void		Cls_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);
static void		Cls_OnDisplayChange(HWND hWnd, UINT bitsPerPixel, UINT cxScreen, UINT cyScreen);
static LRESULT	Cls_OnPowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting);

// EX Message Handler
static void		Cls_OnLButtonDownEx(HWND hWnd, int x, int y, HWND hForeground);
static void		Cls_OnLButtonUpEx(HWND hWnd, int x, int y, HWND hForeground);
static void		Cls_OnSysKeyDownUpEx(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
static void		Cls_OnEventForegroundEx(HWND hWnd, DWORD dwEvent, HWND hForeWnd);
static void		Cls_OnCheckIMEStartConvertingEx(HWND hWnd, BOOL bStartConversioning, DWORD vkCode);

// Sub
static BOOL		bSendInputSub(UINT cInputs, LPINPUT pInputs);
static BOOL		bKBisEP();
static BOOL		bChangeHKLbySendInput(HKL hNewHKL, HKL hPreviousHKL);
static VOID CALLBACK	vCheckFocusTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static VOID CALLBACK	vCheckProcTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static BOOL CALLBACK	bEnumChildProcChangeHKL(HWND hWnd, LPARAM lParam);

//
//  bWinMain()
//
BOOL		bWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance)
{
	if (LoadString(Resource->hLoad(), IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) return FALSE;

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	if (!hPrevInstance) {
		if (!MyRegisterClass(hInstance)) {
			return FALSE;
		}
	}

#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)
	HWND	    hWnd = NULL;
	if ((hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL)) != NULL) {
		SetFocus(GetLastActivePopup(hWnd));
		PostMessage(hWnd, WM_DESTROY, NULL, NULL);
		for (int i = 3; i > 0; i--) {
			Sleep(500);	
			if ((hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL)) != NULL) {
				SetFocus(GetLastActivePopup(hWnd));
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);
				if (i == 1) {
					vMessageBox(NULL, IDS_ALREADYRUN, MessageBoxTYPE);
					if (Resource != NULL) {
						Resource->bUnload();
						delete Resource;
						Resource = NULL;
					}
					return FALSE;
				}
			}
			else break;
		}
	}

	if ((hWnd = InitInstance(hInstance, SW_HIDE)) == NULL) {
		return FALSE;
	}
	hMainWnd = hWnd;
	return TRUE;
}

//
//  MyRegisterClass()
//
static ATOM MyRegisterClass(HINSTANCE hInstance)
{
#define CLASSSTYLE CS_HREDRAW | CS_VREDRAW // クラススタイル

	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CLASSSTYLE;                                                // クラススタイル
	wcex.lpfnWndProc = WndProc;												// Window Proc.
	wcex.cbClsExtra = 0;                                                    // クラスの補足データなし
	wcex.cbWndExtra = 0;                                                    // ウィンドウの補足データなし
	wcex.hInstance = hInstance;                                             // クラスのウィンドウハンドル
	wcex.hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_FLUSHMOUSE));	// アイコンハンドル
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);								// マウスカーソルハンドル
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);               // ウィンドウ背景色
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_FLUSHMOUSE);                    // デフォルトメニュー名
	wcex.lpszClassName = CLASS_FLUSHMOUSE;                                  // このウインドウクラスにつける名前
	wcex.hIconSm = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL));	// 16×16の小さいサイズのアイコン

	return RegisterClassEx(&wcex);
}

//
// InitInstance(HINSTANCE, int)
//
static HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	UNREFERENCED_PARAMETER(nCmdShow);
#define		WINDOWSTYLE		WS_DISABLED
	// for TaskTray
	HWND		hWnd = NULL;						// メインウィンドウのハンドル
	hWnd = CreateWindowEx(
					WS_EX_TOOLWINDOW,				// Tool Bar Window
					CLASS_FLUSHMOUSE,				// RegisterClass()呼び出しを参照
					szTitle,						// Title barのテキスト
					WINDOWSTYLE,					// Window style
					0, 0,							// 水平・垂直位置
					0, 0,							// 幅・高さ
					NULL,							// 親オーバーラップウィンドウ
					NULL,							// ウィンドウクラスのメニューを使う
					hInstance,						// 所有インスタンス
					NULL);							// ポインタは不要
	if (!hWnd) {
		return NULL;								// ウィンドウを作成できなかったときはNULLを返す
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
#define HANDLE_WM_INPUTLANGCHANGE(hWnd, wParam, lParam, fn) ((fn)((hWnd), (UINT)(wParam), (HKL)(lParam)), 0L)

	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_SETFOCUS, Cls_OnSetFocus);
		HANDLE_MSG(hWnd, WM_KILLFOCUS, Cls_OnKillFocus);
		HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
		HANDLE_MSG(hWnd, WM_DISPLAYCHANGE, Cls_OnDisplayChange);

		HANDLE_MSG(hWnd, WM_LBUTTONDOWNEX, Cls_OnLButtonDownEx);
		HANDLE_MSG(hWnd, WM_LBUTTONUPEX, Cls_OnLButtonUpEx);
		HANDLE_MSG(hWnd, WM_SYSKEYDOWNUPEX, Cls_OnSysKeyDownUpEx);
		HANDLE_MSG(hWnd, WM_TASKTRAYEX, Cls_OnTaskTrayEx);
		HANDLE_MSG(hWnd, WM_POWERBROADCAST, Cls_OnPowerBroadcast);
		HANDLE_MSG(hWnd, WM_EVENT_SYSTEM_FOREGROUNDEX, Cls_OnEventForegroundEx);
		HANDLE_MSG(hWnd, WM_CHECKIMESTARTCONVEX, Cls_OnCheckIMEStartConvertingEx);
		break;

	default:
		if (!bReCreateTaskTrayWindow(hWnd, message)) {
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
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)
	Cime = new CIME;
	
	Profile = new CProfile;
	if (!Profile->bGetProfileData()) {
		vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	vGetSetProfileData();

	// Create Task Tray
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

	// for PowerNotification
	PowerNotification = new CPowerNotification(hWnd);
	if (!PowerNotification) {
		vMessageBox(hWnd, IDS_NOTRREGISTEVH, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	if (!bStartThreadHookTimer(hWnd)) {
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	if (bCheckExistingJPIME() && bEnableEPHelper)	bForExplorerPatcherSWS(GetForegroundWindow(), FALSE, FALSE, NULL, NULL);
	
	return TRUE;
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
static void Cls_OnDestroy(HWND hWnd)
{

	vStopThreadHookTimer(hWnd);

	SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);

	if (Profile != NULL) {
		Profile->bSetProfileData();
		delete	Profile;
		Profile = NULL;
	}

	if (!bDestroyTaskTrayWindow(hWnd)) {
	}

	if (PowerNotification != NULL) {
		delete PowerNotification;
		PowerNotification = NULL;
	}

	if (Resource != NULL) {
		Resource->bUnload();
		delete Resource;
		Resource = NULL;
	}

	if (Cime != NULL) {
		delete Cime;
		Cime = NULL;
	}

	PostQuitMessage(0);
}

//
// WM_SETFOCUS
// Cls_OnSetFocus()
//
static void Cls_OnSetFocus(HWND hWnd, HWND hWndOldFocus)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(hWndOldFocus);
	bDisplayIMEModeOnCursor = FALSE;
	bDoModeDispByMouseBttnUp = FALSE;
	bDrawNearCaret = FALSE;
}

//
// WM_KILLFOCUS
// Cls_OnKillFocus()
//
static void Cls_OnKillFocus(HWND hWnd, HWND hWndNewFocus)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(hWndNewFocus);
	vGetSetProfileData();
}

//
// WM_COMMAND
// Cls_OnCommand()
//
static void Cls_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify)
{
	UNREFERENCED_PARAMETER(hWndCtl);
	UNREFERENCED_PARAMETER(codeNotify);
	switch (id) {
	case IDM_ABOUT:
	case IDR_TT_ABOUT:
		vAboutDialog(hWnd);
		break;
	case IDR_TT_SETTING:
		vSettingDialog(hWnd);
		break;
	case IDM_EXIT:
	case IDR_TT_QUIT:
		PostMessage(hWnd, WM_DESTROY, (WPARAM)0, (LPARAM)0);
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
	if (!Cime->bGetVirtualDesktopSize()) {
	}
}

//
// WM_LBUTTONDOWNEX
// Cls_OnLButtonDownEx()
//
static void Cls_OnLButtonDownEx(HWND hWnd, int x, int y, HWND hForeground)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(hForeground);
	if (bEnableEPHelper) {
		bForExplorerPatcherSWS(hForeground, FALSE, FALSE, NULL, NULL);
	}
	return;
}

//
// WM_LBUTTONUPEX
// Cls_OnLButtonUpEx()
//
static void Cls_OnLButtonUpEx(HWND hWnd, int x, int y, HWND hForeground)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(hForeground);
	if (bMoveIMEToolbar && (GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
		if (!Cime->bMoveIMEToolbar()) {
		}
	}
	if (bDoModeDispByMouseBttnUp) {
		HWND	hWndObserved = NULL;
		POINT	pt{};
		if (GetCursorPos(&pt)) {
			RECT	rc{};
			if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;	
			if (FindWindow(L"Shell_TrayWnd", NULL) == GetForegroundWindow()) {
				if (bGetTaskTrayWindowRect(hWnd, &rc) == FALSE)	return;
				if (((pt.x >= rc.left) && (pt.x <= rc.right)) || ((pt.y <= rc.top) && (pt.y >= rc.bottom)))	return;
			}
			if (!Cursor->bStartDrawIMEModeThread(hWndObserved))	return;
		}
	}
	return;
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
// WM_EVENT_SYSTEM_FOREGROUNDEX
// Cls_OnEventForegroundEx()
//
static void		Cls_OnEventForegroundEx(HWND hWnd, DWORD dwEvent, HWND hForeWnd)
{
	UNREFERENCED_PARAMETER(dwEvent);

	if (EventHook->hFormerWnd != hForeWnd) {
		EventHook->hFormerWnd = hForeWnd;
		if (hWnd != hForeWnd) {
			HWND	hWndObserved = NULL;
			if (bDisplayFocusWindowIME) {
				hWndObserved = hWnd;
			}
			else {
				POINT	pt{};
				if (GetCursorPos(&pt)) {
					if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
				}
				else {
					return;
				}
			}
			if (bCheckExistingJPIME() && bEnableEPHelper)	bForExplorerPatcherSWS(hWnd, FALSE, FALSE, NULL, NULL);	// @@@ for Explorer Patcher Simple Window Switcher
			if (bOffChangedFocus) {
				Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
			}
			if (!Cursor->bStartIMECursorChangeThread(hWndObserved))	return;		// error
			POINT	pt{};
			if (GetCursorPos(&pt)) {
				RECT	rc{};
				if (FindWindow(L"Shell_TrayWnd", NULL) == GetForegroundWindow()) {
					if (bGetTaskTrayWindowRect(hWnd, &rc) == FALSE)	return;	// error
					if (((pt.x >= rc.left) && (pt.x <= rc.right)) || ((pt.y <= rc.top) && (pt.y >= rc.bottom)))	return;	// Clicked on Notify Icon
				}
				if (!Cursor->bStartDrawIMEModeThread(hForeWnd))	return;		// error
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

	HWND	hWndObserved = GetForegroundWindow();
	POINT	pt{};
	if (Cursor->bGetCaretPos(hWndObserved, &pt)) {
		if ((pt.x != 0) && (pt.y != 0)) {
			hWndObserved = WindowFromPoint(pt);
		}
	}
	if (Cime->dwIMEMode(hWndObserved, FALSE) != IMEOFF)	bIMEInConverting = bStartConverting;
	else												bIMEInConverting = FALSE;
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
	HWND	hForeWnd = GetForegroundWindow();
	if ((fDown == FALSE)) {											// Key up
		switch (vk) {
		case KEY_ONLY_CTRLUP:
			if (bEnableEPHelper)	bForExplorerPatcherSWS(hForeWnd, FALSE, FALSE, NULL, NULL);
			if (bDoModeDispByCtrlUp) {
				POINT	pt{};
				if (GetCursorPos(&pt)) {
					if (!Cursor->bStartDrawIMEModeThreadWait(WindowFromPoint(pt)))	return;
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
		case KEY_F6:				// F6				(0x75)
		case KEY_F7:				// F7				(0x76)
		case KEY_F8:				// F8				(0x77)
		case KEY_F9:				// F9				(0x78)
		case KEY_F10:				// F10				(0x79)
			break;
		case KEY_IME_ON:			// IME ON					(0x16)
			bIMEInConverting = FALSE;
			if (bEnableEPHelper || bIMEModeForced) {
				Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			}
			break;
		case KEY_KANJI:				// JP(IME/ENG) Alt + 漢字	(0x19)
			bIMEInConverting = FALSE;
			if (bEnableEPHelper || bIMEModeForced) {
				DWORD	dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, bIMEModeForced, &hNewHKL, &hPreviousHKL)) {
					if ((bIMEModeForced != FALSE) && (!Cime->bIsNewIME())) {
						if (dwBeforeIMEMode != IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
						}
					}
				}
			}
			break;
		case KEY_IME_OFF:			// IME OFF					(0x1a)
			if (bIMEInConverting)	return;
			if (bEnableEPHelper || bIMEModeForced) {
				if ((bIMEModeForced != FALSE) && (!Cime->bIsNewIME())) {
					Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
				}
			}
			break;
		case KEY_CONVERT:			// JP(IME/ENG) 変換			(0x1c)
			bIMEInConverting = FALSE;
			if (bEnableEPHelper || bIMEModeForced) {
				SetFocus(hForeWnd);
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, bIMEModeForced, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
					}
					else {
						if ((bIMEModeForced != FALSE) && (!Cime->bIsNewIME())) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
						}
					}
				}
			}
			break;
		case KEY_NONCONVERT:		// JP(IME/ENG) 無変換		(0x1d)
			bIMEInConverting = FALSE;
			if (bEnableEPHelper || bIMEModeForced) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, bIMEModeForced, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						if (((UINT64)hPreviousHKL & LANG_MASK) != LANG_IME) {
							PostMessage(hWnd, WM_SYSKEYDOWNUPEX, KEY_OEM_PA1, (0x80000000));
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
					}
					else {
						if ((bIMEModeForced != FALSE) && (!Cime->bIsNewIME())) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
					}
				}
			}
			break;
		case KEY_OEM_3:				// JP(IME/ENG) [@] / US(ENG) IME ON (0xc0) = ['] ALT + 半角/全角 or 漢字
		case KEY_OEM_8:				// JP(IME/ENG) [`] / UK(ENG) IME ON (0xdf) = ['] ALT + 半角/全角 or 漢字
			if (!bKBisEP())	return;
			if (bIMEInConverting)	return;
			if (bEnableEPHelper || bIMEModeForced) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, bIMEModeForced, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
					}
					else {
						return;
					}
				}
			}
			break;
			return;
		case KEY_OEM_PA1:			// US(ENG) 無変換			(0xeb)
			bIMEInConverting = FALSE;
			if (bEnableEPHelper || bIMEModeForced) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, bIMEModeForced, &hNewHKL, &hPreviousHKL)) {
					Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
				}
			}
			break;
		case KEY_OEM_ATTN:			// JP(IME/ENG) 英数/CapsLock(0xf0)
			if (bIMEInConverting)	return;
			if (bEnableEPHelper || bIMEModeForced) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, bIMEModeForced, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
					}
					else {
						if ((bIMEModeForced != FALSE) && (!Cime->bIsNewIME())) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
					}
				}
			}
			break;
		case KEY_OEM_FINISH:		// JP(IME/ENG) OEM カタカナ	(0xf1)
			if (bIMEInConverting)	return;
			if (bEnableEPHelper || bIMEModeForced) {
				if (bForExplorerPatcherSWS(hForeWnd, bIMEModeForced, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cime->vIMEConvertModeChangeForced(hForeWnd, ZENKANA_IMEON);
					}
					else {
						if ((bIMEModeForced != FALSE) && (!Cime->bIsNewIME())) {
							Cime->vIMEConvertModeChangeForced(hForeWnd, ZENKANA_IMEON);
						}
					}
				}
			}
			break;
		case KEY_OEM_COPY:			// JP(IME/ENG) OEM ひらがな	(0xf2)
			if (bIMEInConverting)	return;
			if (bEnableEPHelper || bIMEModeForced) {
				if (bForExplorerPatcherSWS(hForeWnd, bIMEModeForced, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
					}
					else {
						if ((bIMEModeForced != FALSE) && (!Cime->bIsNewIME())) {
							Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
						}
					}
				}
			}
			break;
		case KEY_OEM_IME_OFF:		// JP(IME/ENG) IME OFF
			bIMEInConverting = FALSE;
			if (bEnableEPHelper || bIMEModeForced) {
				DWORD	dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
				if (bForExplorerPatcherSWS(hForeWnd, bIMEModeForced, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						if (dwBeforeIMEMode == IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
						}
					}
					else {
						if ((bIMEModeForced != FALSE) && (!Cime->bIsNewIME())) {
							if (dwBeforeIMEMode != IMEOFF) {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							}
						}
					}
				}
			}
			break;
		case KEY_OEM_IME_ON:		// JP(IME/ENG) IME ON
			bIMEInConverting = FALSE;
			if (bEnableEPHelper || bIMEModeForced) {
				DWORD	dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
				if (bForExplorerPatcherSWS(GetForegroundWindow(), bIMEModeForced, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						if ((bIMEModeForced != FALSE) && (!Cime->bIsNewIME())) {
							if (dwBeforeIMEMode == IMEOFF) {
								Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							}
						}
					}
					else {
						if ((bIMEModeForced != FALSE) && (!Cime->bIsNewIME())) {
							if (dwBeforeIMEMode != IMEOFF) {
								Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							}
						}
					}
				}
			}
			break;
		case KEY_OEM_BACKTAB:		// OEM Alt+カタカナ/ひらがな	(0xf5)
		case KEY_FF:				// US(ENG) 変換/ひら/カタ	(0xff)
			bIMEInConverting = FALSE;
			if (bEnableEPHelper || bIMEModeForced) {
				SetFocus(hForeWnd);
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, bIMEModeForced, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
					}
					else {
						Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
					}
				}
			}
			break;
		default:
			return;
		}
	}
	else {															// Key down
		switch (vk) {
		case KEY_IME_ON:			// IME ON					(0x16)
		case KEY_OEM_ATTN:			// JP(IME/ENG) 英数/CapsLock(0xf0)
		case KEY_OEM_FINISH:		// JP(IME/ENG) OEM カタカナ	(0xf1)
		case KEY_OEM_COPY:			// JP(IME/ENG) OEM ひらがな	(0xf2)
			if (bEnableEPHelper) {	
				PostMessage(hWnd, WM_SYSKEYDOWNUPEX, vk, (0x80000000));
			}
			return;
		default:
			return;
		}
	}
	HWND	hWndObserved = NULL;
	if (bDisplayFocusWindowIME) {
		hWndObserved = hForeWnd;
	}
	else {
		POINT	pt{};
		if (GetCursorPos(&pt)) {
			if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
		}
	}
	if (Cursor->bStartIMECursorChangeThread(hWndObserved)) {
		if (bDoModeDispByIMEKeyDown) {
			if (!bIMEInConverting) {
				if (!Cursor->bStartDrawIMEModeThread(hWndObserved))	return;
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
	if (GetKeyboardType(1) == 0) {		// EP Keyboard
		_Post_equals_last_error_ DWORD err = GetLastError();
		if (err != 0) {
		}
		else {
			return TRUE;
		}
	}

	// 2nd Check
	DWORD	dProcessID = 0;
	DWORD	dThreadID = GetWindowThreadProcessId(GetForegroundWindow(), &dProcessID);
	if (dThreadID != 0) {
		HKL	hkl = GetKeyboardLayout(dThreadID);
		if (hkl == US_ENG) {			// EP Keyboard
			return TRUE;
		}
	}

	// 3rd Check
#define SUBKEY	_T("SOFTWARE\\Microsoft\\Input\\Locales")
#define VALUE	_T("InputLocale")
	DWORD	dwInputLocale = 0;
	CRegistry* CReg = new CRegistry;
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
// for Explorer Patcher Simple Window Switcher
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
						//EnumChildWindows(hForeWnd, &bEnumChildProcChangeHKL, (LPARAM)hkl);
						//Cime->vActivateIME(hForeWnd);
						if ((hkl = GetKeyboardLayout(dwThreadID)) != NULL) {				// Re Check
							if (hkl != JP_IME) {
								if (bChangeHKLbySendInput(JP_IME, hkl)) {
									hkl = JP_IME;
									bRet = TRUE;
								}
							}
							else {
								bRet = TRUE;
							}
						}
					}
				}
			}
		}
	}
	if (bRet) {
		EnumChildWindows(hForeWnd, &bEnumChildProcChangeHKL, (LPARAM)hkl);
		Cime->vActivateIME(hForeWnd);
	}
	if (lpNewHKL != NULL)		*lpNewHKL = hkl;
	if (lpPreviousHKL != NULL)	*lpPreviousHKL = hPreviousHKL;
	return bRet;
}

//
// bChangeHKLbySendInput();
//
BOOL	bChangeHKLbySendInput(HKL hNewHKL, HKL hPreviousHKL)
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
				//if ((GetAsyncKeyState(VK_SHIFT) & 0x8000)) iKB = iKBList - iKB;
				while (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
					Sleep(1);
				}
				int iKB = iKBList - iPreviousKB + iNewKB;
				if (iNewKB != iPreviousKB) {
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
	HWND	hWnd = FindWindow(CLASS_FLUSHMOUSE32, NULL);
	if (hWnd != NULL)	PostMessage(hWnd, WM_CHECKEXISTINGJPIMEEX, (WPARAM)bRet, (LPARAM)NULL);
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
// bStartThreadHookTimer()
//
BOOL		bStartThreadHookTimer(HWND hWnd)
{
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)
	
	vGetSetProfileData();

	// Load Cursor
	if (Cursor == NULL) {
		Cursor = new CCursor;
		if (!Cursor->bInitialize(hWnd)) {
			vMessageBox(hWnd, IDS_CANTLOADCURSOR, MessageBoxTYPE);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}

	// Register Hook
	if (FlushMouseHook == NULL) {
		FlushMouseHook = new CFlushMouseHook;
		if (!FlushMouseHook->bHookSet(hWnd, szFlushMouse64, szFlushMouse32)) {
			vMessageBox(hWnd, IDS_NOTREGISTERHOOK, MessageBoxTYPE);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}

	// Set Timer
	BOOL	bBool = FALSE;
	if (SetUserObjectInformation(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bBool, sizeof(BOOL)) != FALSE) {
		// Set Timer for Cursor
		if (uCheckFocusTimer == NULL) {
			if ((uCheckFocusTimer = SetTimer(hWnd, nCheckFocusTimerID, nCheckFocusTimerTickValue, (TIMERPROC)&vCheckFocusTimerProc)) == 0) {
				vMessageBox(hWnd, IDS_NOTIMERESOUCE, MessageBoxTYPE);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				return FALSE;
			}
		}

		// Set Timer for Proc
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

	// Set Event Handler
	if (EventHook == NULL) {
		EventHook = new CEventHook;
		if (!EventHook->bEventSet()) {
			vMessageBox(hWnd, IDS_NOTRREGISTEVH, MessageBoxTYPE);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}

	HWND	hFindWnd = FindWindow(CLASS_FLUSHMOUSE32, NULL);
	if (hFindWnd != NULL) {
		if (bEnableEPHelper) {
			PostMessage(hFindWnd, WM_CHECKEXISTINGJPIMEEX, (WPARAM)bEnableEPHelper, (LPARAM)NULL);
		}
		else {
			PostMessage(hFindWnd, WM_CHECKEXISTINGJPIMEEX, (WPARAM)FALSE, (LPARAM)NULL);
		}
	}

	return TRUE;
}

//
// vStopThreadHookTimer()
//
VOID		vStopThreadHookTimer(HWND hWnd)
{
	bDisplayIMEModeOnCursor = FALSE;
	bDoModeDispByMouseBttnUp = FALSE;
	bDrawNearCaret = FALSE;

	if (EventHook != NULL) {
		delete EventHook;
		EventHook = NULL;
	}

	if (uCheckProcTimer != NULL) {
		if (KillTimer(hWnd, nCheckProcTimerID)) {
			uCheckProcTimer = NULL;
		}
	}

	if (uCheckFocusTimer != NULL) {
		if (KillTimer(hWnd, nCheckFocusTimerID)) {
			uCheckFocusTimer = NULL;
		}
	}

	if (FlushMouseHook != NULL) {
		delete	FlushMouseHook;
		FlushMouseHook = NULL;
	}

	if (Cursor != NULL) {
		delete Cursor;
		Cursor = NULL;
	}
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
		if (bDisplayFocusWindowIME) {
			if ((hWndObserved = GetForegroundWindow()) == NULL)	return; // error
		}
		else {
			POINT	pt{};
			if (!GetCursorPos(&pt))								return;	// error
			if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return; // error
		}
		if (!Cursor->bStartIMECursorChangeThread(hWndObserved))	return;	// error
		return;
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
			bReportEvent(MSG_RESTART_EVENT, APPLICATION_CATEGORY);			// 再起動する
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);		// Quit
		}
	}
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