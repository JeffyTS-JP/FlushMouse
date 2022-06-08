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
#include "..\FlushMouseDLL32\KeyboardHookDll32.h"
#include "..\FlushMouseDLL32\MouseHookDll32.h"
#include "..\MiscLIB\CThread.h"

#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")

//
// Define
//
#define	KB_MASK		0xffff0000
#define	KB_JP		0x04110000
#define	KB_US		0x04090000
#define	LANG_MASK	0x0000ffff
#define LANG_IME	0x00000411
#define	LANG_ENG	0x00000409
#define JP_IME		(HKL)(KB_JP | LANG_IME)
#define JP_ENG		(HKL)(KB_JP | LANG_ENG)
#define US_IME		(HKL)(KB_US | LANG_IME)
#define US_ENG		(HKL)(KB_US | LANG_ENG)

#define WM_SYSINUPUTLANGCHANGEEX	(WM_USER + WM_INPUTLANGCHANGE)

//
// Struct Define
//

//
// Global Data
//
// Hook
TCHAR		szTitle[MAX_LOADSTRING]{};					// タイトル バーのテキスト
TCHAR		szWindowClass[MAX_LOADSTRING]{};			// メイン ウィンドウ クラス名
HWND		hMainWnd = NULL;

CProfile	*Profile = NULL;							// Profile class
CCursor		*Cursor = NULL;								// CCursor class

CResource	*Resource = NULL;							// Resource

// Use in FlushMouse from Registry
BOOL		bDisplayIMEModeOnCursor = TRUE;
BOOL		bDoModeDispByIMEKeyDown = FALSE;
BOOL		bDoModeDispByCtrlUp = FALSE;
BOOL		bDisplayFocusWindowIME = FALSE;
BOOL		bDoModeDispByMouseBttnUp = FALSE;
BOOL		bDrawNearCaret = FALSE;
BOOL		bEnableEPHelper = FALSE;

//
// Local Data
//
// Timer
#define INITTIMERVALUE		200
#define CHECKFOCUSTIMERID	1
static UINT     nCheckFocusTimerTickValue = INITTIMERVALUE;	// Timer tick
static UINT_PTR nCheckFocusTimerID = CHECKFOCUSTIMERID;		// Timer ID
static BOOL		bCheckFocusTimer = FALSE;

// Hook
static CFlushMouseHook* FlushMouseHook = NULL;

// Event Handller for Focus
static CFocusEvent* FocusEvent = NULL;

// TaskTray
static BOOL		bTaskTray = FALSE;
static UINT		uTaskbarCreatedMessage = 0;

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


// Hooked Message Handler
static void		Cls_OnLButtonDownEx(HWND hWnd, int x, int y, HWND hForeground);
static void		Cls_OnLButtonUpEx(HWND hWnd, int x, int y, HWND hForeground);
static void		Cls_OnSysKeyDownUpEx(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

// Sub
static VOID CALLBACK	vCheckFocusTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static LONG WINAPI		lExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
static BOOL CALLBACK	bEnumChildProcActivateIME(HWND hWnd, LPARAM lParam);
static BOOL CALLBACK	bEnumChildProcChangeHKL(HWND hWnd, LPARAM lParam);
static BOOL		bKBisEP();

//
// Local Prototype Define
//

//
//  bWinMain()
//
BOOL		bWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance)
{
	SetUnhandledExceptionFilter(&lExceptionFilter);

	if (LoadString(Resource->hLoad(), IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) return FALSE;
	if (LoadString(Resource->hLoad(), IDC_FLUSHMOUSE, szWindowClass, MAX_LOADSTRING) == 0) return FALSE;

	// Set Thread DPI Awareness V2 for High DPI
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	if (!hPrevInstance) {
		if (!MyRegisterClass(hInstance)) {
			return FALSE;
		}
	}

#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)
	HWND	    hWnd = NULL;
	if ((hWnd = FindWindow(szWindowClass, NULL)) != NULL) {
		SetFocus(GetLastActivePopup(hWnd));
		PostMessage(hWnd, WM_DESTROY, NULL, NULL);
		for (int i = 3; i > 0; i--) {
			Sleep(500);	
			if (FindWindow(szWindowClass, NULL) != NULL) {
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
	wcex.lpszClassName = szWindowClass;                                     // このウインドウクラスにつける名前
	wcex.hIconSm = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL));	// 16×16の小さいサイズのアイコン

	return RegisterClassEx(&wcex);
}

//
// InitInstance(HINSTANCE, int)
//
static HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	UNREFERENCED_PARAMETER(nCmdShow);
#define		WINDOWSTYLE		(WS_OVERLAPPEDWINDOW  | WS_HSCROLL | WS_VSCROLL)
	// for TaskTray
	HWND		hWnd = NULL;						// メインウィンドウのハンドル
	hWnd = CreateWindowEx(
					WS_EX_TOOLWINDOW,				// Tool Bar Window
					szWindowClass,					// RegisterClass()呼び出しを参照
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
// void Cls_OnTaskTray(HWND hWnd, UINT id, UINT uMsg)
#define HANDLE_WM_TASKTRAY(hWnd, wParam, lParam, fn) ((fn)((hWnd), (UINT)(wParam), (UINT)(lParam)), 0L)

	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_SETFOCUS, Cls_OnSetFocus);
		HANDLE_MSG(hWnd, WM_KILLFOCUS, Cls_OnKillFocus);
		HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWNEX, Cls_OnLButtonDownEx);
		HANDLE_MSG(hWnd, WM_LBUTTONUPEX, Cls_OnLButtonUpEx);
		HANDLE_MSG(hWnd, WM_SYSKEYDOWNUPEX, Cls_OnSysKeyDownUpEx);
		HANDLE_MSG(hWnd, WM_TASKTRAY, Cls_OnTaskTray);

	default:
		if (message == uTaskbarCreatedMessage) {
			if ((bTaskTray = bCreateTaskTrayWindow(hWnd, IDI_FLUSHMOUSE)) == FALSE) {
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)        // MessageBox style
				vMessageBox(hWnd, IDS_NOTREGISTERTT, MessageBoxTYPE);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				return FALSE;
			}
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
	Profile = new CProfile;
	if (!Profile->bGetProfileData()) {
		vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}
	vGetSetProfileData();												// Profile dataを変数にセット

	// Register Re Create Task Tray Message
	if ((uTaskbarCreatedMessage = RegisterWindowMessage(_T("TaskbarCreated"))) == 0) {
		vMessageBox(hWnd, IDS_NOTREGISTERTT, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}
	// Create Task Tray
	if ((bTaskTray = bCreateTaskTrayWindow(hWnd, IDI_FLUSHMOUSE)) == FALSE) {
		vMessageBox(hWnd, IDS_NOTREGISTERTT, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	if (!bStartThredHookTimer(hWnd)) {
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	if (bCheckExistingJPIME() && bEnableEPHelper)	bForExplorerPatcherSWS(GetForegroundWindow(), FALSE, NULL, NULL);

	return TRUE;
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
static void Cls_OnDestroy(HWND hWnd)
{

	vStopThredHookTimer(hWnd);

	EXCEPTION_POINTERS ExceptionInfo{};
	lExceptionFilter(&ExceptionInfo);

	if (Profile != NULL) {
		Profile->bSetProfileData();
		delete	Profile;
	}

	if (bTaskTray) {
		if (bDestroyTaskTrayWindow(hWnd)) {
			bTaskTray = FALSE;
		}
	}

	if (Resource != NULL) {
		Resource->bUnload();
		delete Resource;
		Resource = NULL;
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
		DestroyWindow(hWnd);
		break;
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
		POINT	pt{};
		if (GetCursorPos(&pt)) {
			RECT	rc{};
			NOTIFYICONIDENTIFIER	nii{};
			nii.cbSize = sizeof(NOTIFYICONIDENTIFIER);
			nii.hWnd = hWnd;	nii.uID = IDI_FLUSHMOUSE;	nii.guidItem = GUID_NULL;
			if (Shell_NotifyIconGetRect(&nii, &rc) != S_OK) {
				return;
			}
			if (((pt.x >= rc.left) && (pt.x <= rc.right)) || ((pt.y <= rc.top) && (pt.y >= rc.bottom)))	return;
		}
		bForExplorerPatcherSWS(hForeground, FALSE, NULL, NULL);
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
	if (bDoModeDispByMouseBttnUp) {
		HWND	hWndObserved = NULL;
		POINT	pt{};
		if (GetCursorPos(&pt)) {
			RECT	rc{};
			NOTIFYICONIDENTIFIER	nii{};
			nii.cbSize = sizeof(NOTIFYICONIDENTIFIER);
			nii.hWnd = hWnd;	nii.uID = IDI_FLUSHMOUSE;	nii.guidItem = GUID_NULL;
			if (Shell_NotifyIconGetRect(&nii, &rc) != S_OK) {
				return;
			}
			if (((pt.x >= rc.left) && (pt.x <= rc.right)) || ((pt.y <= rc.top) && (pt.y >= rc.bottom)))	return;	// Clicked on Notify Icon
			if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;			// error
		}
		if (!Cursor->bStartDrawIMEModeThread(hWndObserved))	return;		// error
	}
	return;
}

//
// WM_SYSKEYDOWNEX
// Cls_OnSyskeDownUpEx()
//
static void Cls_OnSysKeyDownUpEx(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(flags);
#define KEY_ONLY_CTRLUP		(WM_USER + VK_ONLY_CTRLUP)				// Only Ctrl key up
#define KEY_CTRL			(WM_USER + VK_CONTROL)					// Ctrl   (0x11)
#define KEY_LCTRL			(WM_USER + VK_LCONTROL)					// Ctrl L (0xa2)
#define KEY_RCTRL			(WM_USER + VK_RCONTROL)					// Ctrl R (0xa3)
#define	KEY_ALT				(WM_USER + VK_MENU)						// ALT    (0x12
#define	KEY_LALT			(WM_USER + VK_LMENU)					// ALT L  (0xa4)
#define	KEY_RALT			(WM_USER + VK_RMENU)					// ALT R  (0xa5)
#define KEY_SHIFT			(WM_USER + VK_SHIFT)					// Shift  (0x10)
#define KEY_LSHIFT			(WM_USER + VK_LSHIFT)					// Shift L(0xa0)
#define KEY_RSHIFT			(WM_USER + VK_RSHIFT)					// Shift R(0xa1)
#define KEY_LWIN			(WM_USER + VK_LWIN)						// L Win  (0x5b)
#define KEY_RWIN			(WM_USER + VK_RWIN)						// R Win  (0x5c)
#define KEY_CAPITAL			(WM_USER + VK_CAPITAL)					// Caps Lock (0x14)
#define KEY_KANA			(WM_USER + VK_KANA)						// カナ   (0x15)
#define KEY_IME_ON			(WM_USER + VK_IME_ON)					// IME ON (0x16)
#define KEY_KANJI			(WM_USER + VK_KANJI)					// 漢字   (0x19)
#define KEY_IME_OFF			(WM_USER + VK_IME_OFF)					// IME OFF(0x1a)
#define KEY_CONVERT			(WM_USER + VK_CONVERT)					// IME convert    (0x1c)
#define KEY_NONCONVERT		(WM_USER + VK_NONCONVERT)				// IME nonconvert (0x1d)
#define KEY_CTRL_F6			(WM_USER + VK_F6)						// Ctrl + F6 (0x75) 
#define KEY_CTRL_F7			(WM_USER + VK_F7)						// Ctrl + F7 (0x76)
#define KEY_CTRL_F8			(WM_USER + VK_F8)						// Ctrl + F8 (0x77)
#define KEY_CTRL_F9			(WM_USER + VK_F9)						// Ctrl + F9 (0x78)
#define KEY_OEM_3			(WM_USER + VK_OEM_3)					// OEM US(ENG) IME ON (0xc0) [`] for US
#define KEY_OEM_8			(WM_USER + VK_OEM_8)					// OEM UK(ENG) IME ON (0xc0) [`] for UK
#define KEY_OEM_ATTN		(WM_USER + VK_OEM_ATTN)					// 英数/CapsLock (0xf0)
#define KEY_OEM_FINISH		(WM_USER + VK_OEM_FINISH)				// OEM カタカナ (0xf1)
#define KEY_OEM_COPY		(WM_USER + VK_OEM_COPY)					// OEM ひらがな (0xf2)
#define KEY_OEM_IME_OFF		(WM_USER + VK_OEM_IME_OFF)				// OEM IME OFF (0xf3) But It's IME ON when key up
#define KEY_OEM_IME_ON		(WM_USER + VK_OEM_IME_ON)				// OEM IME ON  (0xf4) But It's IME OFF when key up
#define KEY_OEM_BACKTAB		(WM_USER + VK_OEM_BACKTAB)				// OEM Alt+カタカナ/ひらがな (0xf5)
#define KEY_OEM_PA1			(WM_USER + VK_OEM_PA1)					// US(ENG) KB 無変換	 (0xeb)
#define KEY_FF				(WM_USER + 0xff)						// US(ENG) KB 変換/ひら/カタ	(0xff)

	if ((cRepeat & 0xfffe) != 0) {
		return;
	}
	HKL		hNewHKL = NULL;
	HKL		hPreviousHKL = NULL;
	HWND	hForeWnd = GetForegroundWindow();
	if ((fDown == FALSE)) {											// Key up
		switch (vk) {
		case KEY_ONLY_CTRLUP:
			if (bEnableEPHelper)	bForExplorerPatcherSWS(hForeWnd, FALSE, NULL, NULL);
			if (bDoModeDispByCtrlUp) {
				POINT	pt{};
				if (GetCursorPos(&pt)) {
					if (!Cursor->bStartDrawIMEModeThreadWait(WindowFromPoint(pt)))	return;	// error
				}
			}
			return;
		case KEY_CTRL:
		case KEY_LCTRL:
		case KEY_RCTRL:
		case KEY_ALT:
		case KEY_LALT:
		case KEY_RALT:
			if (bEnableEPHelper)	bForExplorerPatcherSWS(hForeWnd, TRUE, NULL, NULL);
			return;
		case KEY_CTRL_F6:			// Ctrl + F6				(0x75)
		case KEY_CTRL_F7:			// Ctrl + F7				(0x76)
		case KEY_CTRL_F8:			// Ctrl + F8				(0x77)
		case KEY_CTRL_F9:			// Ctrl + F9				(0x78)
			break;
		case KEY_IME_ON:			// IME ON					(0x16)
			if (bEnableEPHelper) {
				Cursor->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			}
			break;
		case KEY_KANJI:				// JP(IME/ENG) Alt + 漢字	(0x19)
			if (bEnableEPHelper) {
				while (GetAsyncKeyState(VK_MENU) & 0x8000)
				{
					Sleep(50);
				}
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, &hNewHKL, &hPreviousHKL)) {
				}
			}
			break;
		case KEY_IME_OFF:			// IME OFF					(0x1a)
			break;
		case KEY_CONVERT:			// JP(IME/ENG) 変換			(0x1c)
			if (bEnableEPHelper) {
				SetFocus(hForeWnd);
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cursor->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
					}
				}
			}
			break;
		case KEY_NONCONVERT:		// JP(IME/ENG) 無変換		(0x1d)
			if (bEnableEPHelper) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						if (((UINT64)hPreviousHKL & LANG_MASK) != LANG_IME) {
							PostMessage(hWnd, WM_SYSKEYDOWNUPEX, KEY_OEM_PA1, (0x80000000));
						}
						else {
							Cursor->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
					}
				}
			}
			break;
		case KEY_OEM_3:				// JP(IME/ENG) [@] / US(ENG) IME ON (0xc0) = ['] ALT + 半角/全角 or 漢字
		case KEY_OEM_8:				// JP(IME/ENG) [`] / UK(ENG) IME ON (0xdf) = ['] ALT + 半角/全角 or 漢字
			if (bEnableEPHelper) {
				while (GetAsyncKeyState(VK_MENU) & 0x8000)
				{
					Sleep(50);
				}
				PostMessage(hWnd, WM_SYSKEYDOWNUPEX, KEY_OEM_IME_ON, (0x80000000));
				Sleep(10);
			}
			return;
		case KEY_OEM_PA1:			// US(ENG) 無変換			(0xeb)
			if (bEnableEPHelper) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cursor->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
					}
					else {		// KEY_OEM_PA1 is from US KB
						Cursor->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
					}
				}
			}
			break;
		case KEY_OEM_ATTN:			// JP(IME/ENG) 英数/CapsLock(0xf0)
			if (bEnableEPHelper) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cursor->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
					}
				}
			}
			break;
		case KEY_OEM_FINISH:		// JP(IME/ENG) OEM カタカナ	(0xf1)
			if (bEnableEPHelper) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cursor->vIMEConvertModeChangeForced(hForeWnd, ZENKANA_IMEON);
					}
				}
			}
			break;
		case KEY_OEM_COPY:			// JP(IME/ENG) OEM ひらがな	(0xf2)
			if (bEnableEPHelper) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cursor->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
					}
				}
			}
			break;
		case KEY_OEM_IME_OFF:		// JP(IME/ENG) IME OFF		(0xf3) But It's IME ON when key up
			if (bEnableEPHelper) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cursor->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
					}
					else {
						Cursor->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
					}
				}
			}
			break;
		case KEY_OEM_IME_ON:		// JP(IME/ENG) IME ON		(0xf4) But It's IME OFF when key up
			if (bEnableEPHelper) {
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cursor->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
					}
					else {
						Cursor->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
					}
				}
			}
			break;
		case KEY_OEM_BACKTAB:		// OEM Alt+カタカナ/ひらがな	(0xf5)
		case KEY_FF:				// US(ENG) 変換/ひら/カタ	(0xff)
			if (bEnableEPHelper) {
				SetFocus(hForeWnd);
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cursor->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
					}
					else {
						Cursor->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
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
			// case KEY_CAPITAL:		// US(ENG)     英数/CapsLock	(0x14) CapsLockされてしまうので処理しない
		case KEY_IME_ON:			// IME ON					(0x16)
		case KEY_OEM_ATTN:			// JP(IME/ENG) 英数/CapsLock(0xf0)
		case KEY_OEM_FINISH:		// JP(IME/ENG) OEM カタカナ	(0xf1)
		case KEY_OEM_COPY:			// JP(IME/ENG) OEM ひらがな	(0xf2)
			if (bEnableEPHelper) {
				PostMessage(hWnd, WM_SYSKEYDOWNUPEX, vk, (0x80000000));
				return;
			}
			break;
		default:
			return;
		}
	}
	//===
	if (Cursor->bStartIMECursorChangeThread(hForeWnd)) {
		if (bDoModeDispByIMEKeyDown) {
			HWND	hWndObserved = NULL;
			if (bDisplayFocusWindowIME) {
				if ((hWndObserved = GetForegroundWindow()) == NULL)		return; // error
				//hWndObserved = hForeWnd;
			}
			else {
				POINT	pt{};
				if (GetCursorPos(&pt)) {
					if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return; // error
				}
			}
			if (!Cursor->bStartDrawIMEModeThread(hWndObserved))	return;			// error
		}
	}
	return;
}

// 
// for Explorer Patcher Simple Window Switcher
// bForExplorerPatcherSWS()
//
BOOL		bForExplorerPatcherSWS(HWND hForeWnd, BOOL bChangeToIME, LPHKL lpNewHKL, LPHKL lpPreviousHKL)
{
	if (hForeWnd == NULL)	return FALSE;
	BOOL	bRet = FALSE;
	HKL		hkl = NULL;
	HKL		hPreviousHKL = NULL;
	DWORD	dwProcessID = 0;
	DWORD	dwThreadID = 0;

	dwThreadID = GetWindowThreadProcessId(hForeWnd, &dwProcessID);
	if ((hkl = GetKeyboardLayout(dwThreadID)) != NULL) {
		int	iKeyboardType = GetKeyboardType(1);
		if (iKeyboardType == 2)	hPreviousHKL = hkl;
		else hPreviousHKL = 0x00000000;
		if ((bChangeToIME && (hkl != JP_IME)) || (hkl == US_ENG)) {
			hkl = JP_IME;
			if (ActivateKeyboardLayout(hkl, KLF_SETFORPROCESS) != 0) {
				EnumChildWindows(hForeWnd, &bEnumChildProcChangeHKL, (LPARAM)hkl);
				bRet = TRUE;
			}
		}
		else {
			if (hPreviousHKL != JP_IME)	EnumChildWindows(hForeWnd, &bEnumChildProcChangeHKL, (LPARAM)hkl);
			bRet = TRUE;
		}
	}
	EnumChildWindows(hForeWnd, &bEnumChildProcActivateIME, NULL);
	if (lpNewHKL != NULL)		*lpNewHKL = hkl;
	if (lpPreviousHKL != NULL)	*lpPreviousHKL = hPreviousHKL;
	return bRet;
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
					if (lpHKL[0] == JP_IME) {
						bRet = TRUE;
						break;
					}
				}
			}
			delete[]	lpHKL;
		}
	}
	HWND	hWnd = FindWindow(_T("FLUSHMOUSE32"), NULL);
	if (hWnd != NULL)	PostMessage(hWnd, WM_SYSINUPUTLANGCHANGEEX, (WPARAM)bRet, (LPARAM)NULL);
	return bRet;
}

// 
// bEnumChildProcActivateIME()
// 
BOOL CALLBACK bEnumChildProcActivateIME(HWND hWnd, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	HWND	hIMWnd = ImmGetDefaultIMEWnd(hWnd);
	if (hIMWnd != NULL) {
		SendMessage(hIMWnd, WM_ACTIVATE, (WPARAM)WA_ACTIVE, (LPARAM)NULL);
		LPTSTR	lpszBuffer = new TCHAR[_MAX_PATH];
		ZeroMemory(lpszBuffer, _MAX_PATH);
		if (GetClassName(hIMWnd, lpszBuffer, _MAX_PATH) != 0) {
			HWND	hIMEWnd = FindWindow(lpszBuffer, NULL);
			if (hIMEWnd != NULL) {
				SendMessage(hIMEWnd, WM_ACTIVATE, (WPARAM)WA_ACTIVE, (LPARAM)NULL);
			}
		}
		if (lpszBuffer != NULL)	delete[] lpszBuffer;
	}
	return TRUE;
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
// lExceptionFilter()
//
static LONG WINAPI lExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	UNREFERENCED_PARAMETER(pExceptionInfo);

	SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);

	return EXCEPTION_CONTINUE_SEARCH;
}

//
// bStartThredHookTimer()
//
BOOL		bStartThredHookTimer(HWND hWnd)
{
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)
	Cursor = new CCursor;
	if (!Cursor->bInitialize(hWnd)) {
		vMessageBox(hWnd, IDS_CANTLOADCURSOR, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	FocusEvent = new CFocusEvent;
	if (!FocusEvent->bEventSet()) {
		vMessageBox(hWnd, IDS_NOTRREGISTEVH, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	if ((bCheckFocusTimer = (BOOL)SetTimer(hWnd, nCheckFocusTimerID, nCheckFocusTimerTickValue, (TIMERPROC)&vCheckFocusTimerProc)) == 0) {
		vMessageBox(hWnd, IDS_NOTIMERESOUCE, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	FlushMouseHook = new CFlushMouseHook;
	if (!FlushMouseHook->bHookSet(hWnd, szFlushMouse64, szFlushMouse32)) {
		vMessageBox(hWnd, IDS_NOTREGISTERHOOK, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}
	return TRUE;
}

//
// vStopThredHookTimer()
//
VOID		vStopThredHookTimer(HWND hWnd)
{
	if (bCheckFocusTimer) {
		if (KillTimer(hWnd, nCheckFocusTimerID)) {
			bCheckFocusTimer = FALSE;
		}
	}

	if (FocusEvent != NULL) {
		delete FocusEvent;
		FocusEvent = NULL;
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
	ZeroMemory(szResFile, _MAX_PATH);
	_tcsncpy_s(szResFile, _MAX_PATH, lpszResFile, _TRUNCATE);
}

CResource::~CResource()
{
	hModRes = NULL;
	iResourceLoadCount = 0;
	delete[]	szResFile;
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

