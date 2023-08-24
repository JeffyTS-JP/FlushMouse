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
TCHAR		szTitle[MAX_LOADSTRING]{};
HWND		hMainWnd = NULL;

CProfile	*Profile = NULL;
CCursor		*Cursor = NULL;	
CResource	*Resource = NULL;
CIME		*Cime = NULL;

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
static UINT     nCheckFocusTimerTickValue = FOCUSINITTIMERVALUE;
static UINT_PTR nCheckFocusTimerID = CHECKFOCUSTIMERID;
static UINT_PTR	uCheckFocusTimer = NULL;
// Timer for Poc
#define PROCINITTIMERVALUE		1000
#define CHECKPROCTIMERID		2
static UINT     nCheckProcTimerTickValue = PROCINITTIMERVALUE;
static UINT_PTR nCheckProcTimerID = CHECKPROCTIMERID;
static UINT_PTR	uCheckProcTimer = NULL;

// Hook
static CFlushMouseHook		*FlushMouseHook = NULL;

// Event Handler	
static CEventHook			*EventHook = NULL;

// for PowerNotification
static CPowerNotification	*PowerNotification = NULL;

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
static void		Cls_OnCheckIMEStartConvertingEx(HWND hWnd, BOOL bStartConverting, DWORD vkCode);
static void		Cls_OnCheckExistingJPIMEEx(HWND hWnd, BOOL bEPHelper);
static void		Cls_OnInputLangChangeEx(HWND hWnd, UINT CodePage, HKL hkl);

// Sub
static BOOL		bSendInputSub(UINT cInputs, LPINPUT pInputs);
static BOOL		bKBisEP();
static BOOL		bChangeHKLbySendInput(HKL hNewHKL, HKL hPreviousHKL);
static VOID CALLBACK		vCheckFocusTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static VOID CALLBACK		vCheckProcTimerProc(HWND hWnd, UINT uMsg, UINT uTimerID, DWORD dwTime);
static BOOL CALLBACK		bEnumChildProcChangeHKL(HWND hWnd, LPARAM lParam);

//
//  bWinMain()
//
BOOL		bWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, int nCmdShow)
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
	HWND	hWnd = NULL;
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

	if ((hWnd = InitInstance(hInstance, nCmdShow)) == NULL) {
		return FALSE;
	}
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
	hMainWnd = hWnd;
	return TRUE;
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
//#define HANDLE_WM_INPUTLANGCHANGE(hWnd, wParam, lParam, fn) ((fn)((hWnd), (UINT)(wParam), (HKL)(lParam)), 0L)

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
		HANDLE_MSG(hWnd, WM_CHECKEXISTINGJPIMEEX, Cls_OnCheckExistingJPIMEEx);
		HANDLE_MSG(hWnd, WM_INPUTLANGCHANGEEX, Cls_OnInputLangChangeEx);
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

	hMainWnd = hWnd;

	Cime = new CIME;
	
	Profile = new CProfile;
	if (!Profile->bGetProfileData()) {
		vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}

	vGetSetProfileData();

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

	if ((bCheckExistingJPIME() && bEnableEPHelper)) {
		bForExplorerPatcherSWS(GetForegroundWindow(), TRUE, TRUE, NULL, NULL);
	}

	bReportEvent(MSG_START_SUCCEED, APPLICATION_CATEGORY);

	return TRUE;
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
static void Cls_OnDestroy(HWND hWnd)
{
	vDestroyWindow(hWnd);
}

//
// vDestroyWindow()
//
void		vDestroyWindow(HWND hWnd)
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

	if (Cime != NULL) {
		delete Cime;
		Cime = NULL;
	}

	bReportEvent(MSG_QUIT_FLUSHMOUSE, APPLICATION_CATEGORY);

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
}

//
// WM_INPUTLANGCHANGEEX
// Cls_OnInputLangChangeEx
//
static void		Cls_OnInputLangChangeEx(HWND hWnd, UINT CodePage, HKL hkl)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(CodePage);
	if ((bCheckExistingJPIME() && bEnableEPHelper)) {
		if (hkl == US_ENG) {
			HWND	hForeWnd = GetForegroundWindow();
			if (hForeWnd != NULL) {
				HKL	hNewHKL = NULL;
				HKL	hPreviousHKL = NULL;
				bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL);
			}
		}
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
	if ((bCheckExistingJPIME() && bEnableEPHelper)) {
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
			POINT	pt{};
			if (bDisplayFocusWindowIME) {
				hWndObserved = hForeWnd;
			}
			else {
				if (GetCursorPos(&pt)) {
					if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
				}
				else {
					return;
				}
			}
			if (bOffChangedFocus) {
				Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
			}
			if (!Cursor->bStartIMECursorChangeThread(hWndObserved))	return;
			if (GetCursorPos(&pt)) {
				RECT		rc{};
				if (FindWindow(L"Shell_TrayWnd", NULL) == hForeWnd) {
					if (bGetTaskTrayWindowRect(hWnd, &rc) == FALSE)	return;
					if (((pt.x >= rc.left) && (pt.x <= rc.right)) || ((pt.y <= rc.top) && (pt.y >= rc.bottom)))	return;
				}
				if (!Cursor->bStartDrawIMEModeThreadWaitEventForeGround(hForeWnd))	return;
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
	if ((fDown == FALSE)) {											// Key up
		switch (vk) {
		case KEY_TAB:
		case KEY_RETURN:
			if (!bIMEInConverting) {
				HWND	hWndObserved = NULL;
				if (bDisplayFocusWindowIME) {
					if ((hWndObserved = GetForegroundWindow()) == NULL)	return;
				}
				else {
					POINT	pt{};
					if (GetCursorPos(&pt)) {
						if ((hWndObserved = WindowFromPoint(pt)) == NULL)	return;
					}
					else 	return; // error
				}
				if (Cursor->bStartIMECursorChangeThread(hWndObserved)) {
					if (bDoModeDispByIMEKeyDown) {
						if (!bIMEInConverting) {
							if (!Cursor->bStartDrawIMEModeThreadWait(hWndObserved))	return;
						}
					}
				}
				return;
			}
			bIMEInConverting = FALSE;
			return;
		case KEY_ONLY_CTRLUP:
			if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
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
		case KEY_IME_ON:			// IME ON			(0x16)
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
			}
			break;
		case KEY_KANJI:				// JP(IME/ENG) Alt + 漢字	(0x19)
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				DWORD	dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL)) {
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
			break;
		case KEY_IME_OFF:			// IME OFF					(0x1a)
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
			}
			break;
		case KEY_CONVERT:			// JP(IME/ENG) 変換			(0x1c)
			if (bIMEInConverting) {
				return;
			}
			if ((bEnableEPHelper || bIMEModeForced)) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				DWORD	dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
				if (dwBeforeIMEMode == IMEOFF) {
					if (bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL)) {
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
			break;
		case KEY_NONCONVERT:		// JP(IME/ENG) 無変換		(0x1d)
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				DWORD	dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
				if (dwBeforeIMEMode != IMEOFF) {
					if (bForExplorerPatcherSWS(hForeWnd, TRUE, FALSE, &hNewHKL, &hPreviousHKL)) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							if (((UINT64)hPreviousHKL & LANG_MASK) != LANG_IME) {
								PostMessage(hWnd, WM_SYSKEYDOWNUPEX, KEY_OEM_PA1, (0x80000000));
							}
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
					}
				}
			}
			break;
		case KEY_OEM_3:				// JP(IME/ENG) [@] / US(ENG) IME ON (0xc0) = ['] ALT + 半角/全角 or 漢字
		case KEY_OEM_8:				// JP(IME/ENG) [`] / UK(ENG) IME ON (0xdf) = ['] ALT + 半角/全角 or 漢字
			if (!bKBisEP())	return;
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL)) {
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
			break;
		case KEY_OEM_PA1:			// US(ENG) 無変換			(0xeb)
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				DWORD	dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
				if (dwBeforeIMEMode != IMEOFF) {
					if (bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL)) {
						Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
					}
				}
			}
			break;
		case KEY_OEM_ATTN:			// JP(IME/ENG) 英数/CapsLock(0xf0)
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				DWORD	dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
				if (dwBeforeIMEMode != IMEOFF) {
					if (bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL)) {
						if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
					}
				}
			}
			break;
		case KEY_OEM_FINISH:		// JP(IME/ENG) OEM カタカナ	(0xf1)
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						Sleep(50);
						Cime->vIMEConvertModeChangeForced(hForeWnd, ZENKANA_IMEON);
					}
					else {
						Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						Sleep(50);
						Cime->vIMEConvertModeChangeForced(hForeWnd, ZENKANA_IMEON);
					}
				}
			}
			break;
		case KEY_OEM_COPY:			// JP(IME/ENG) OEM ひらがな	(0xf2)
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						Sleep(50);
						Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
					}
					else {
						Cime->vIMEConvertModeChangeForced(hForeWnd, ZENHIRA_IMEON);
					}
				}
			}
			break;
		case KEY_OEM_IME_OFF:		// JP(IME/ENG) IME OFF (0xf3)
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					break;
				}
				DWORD	dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						if (dwBeforeIMEMode != IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							Sleep(50);
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							Sleep(50);
						}
					}
					else {
						if (dwBeforeIMEMode != IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							Sleep(100);
						}
					}
				}
			}
			break;
		case KEY_OEM_IME_ON:		// JP(IME/ENG) IME ON  (0xf4)
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				HWND	hWndHidemaru = FindWindow(_T("Hidemaru32Class"), NULL);
				if ((hWndHidemaru != NULL) && (hForeWnd == hWndHidemaru)) {
					break;
				}
				DWORD	dwBeforeIMEMode = Cime->dwIMEMode(hForeWnd, FALSE);
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL)) {
					if ((hPreviousHKL != JP_IME) && (hNewHKL == JP_IME)) {
						if (dwBeforeIMEMode != IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
							Sleep(50);
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							Sleep(50);
						}
					}
					else {
						if (dwBeforeIMEMode != IMEOFF) {
							Cime->vIMEOpenCloseForced(hForeWnd, IMECLOSE);
						}
						else {
							Cime->vIMEOpenCloseForced(hForeWnd, IMEOPEN);
							Sleep(100);
						}
					}
				}
				Sleep(10);
			}
			break;
		case KEY_OEM_BACKTAB:		// OEM Alt+カタカナ/ひらがな	(0xf5)
		case KEY_FF:				// US(ENG) 変換/ひら/カタ	(0xff)
			if (bIMEInConverting) {
				return;
			}
			if (bEnableEPHelper || bIMEModeForced) {
				if ((hForeWnd = GetForegroundWindow()) == NULL)	return;
				SetFocus(hForeWnd);
				if (bForExplorerPatcherSWS(hForeWnd, TRUE, TRUE, &hNewHKL, &hPreviousHKL)) {
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
		if ((hWndObserved = GetForegroundWindow()) == NULL)	return;
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
// WM_CHECKEXISTINGJPIMEEX
// Cls_OnCheckExistingJPIMEEx()
//
void		Cls_OnCheckExistingJPIMEEx(HWND hWnd, BOOL bEPHelper)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(bEPHelper);
	if (!bSetEnableEPHelperLL64(bEPHelper)) {
		PostMessage(hWnd, WM_DESTROY, (WPARAM)0, (LPARAM)0);
	}
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
						if ((hkl = GetKeyboardLayout(dwThreadID)) != NULL) {
							if (hkl != JP_IME) {
								if (bChangeHKLbySendInput(JP_IME, hkl)) {
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
				int iKB = iKBList - iPreviousKB + iNewKB;
				if ((GetAsyncKeyState(VK_SHIFT) & 0x8000)) iKB = iKBList - iKB;
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
	BOOL		bRet = FALSE;
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
	HWND		hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL);
	if (hWnd != NULL) {
		if (bEnableEPHelper) {
			PostMessage(hWnd, WM_CHECKEXISTINGJPIMEEX, (WPARAM)bRet, (LPARAM)NULL);
		}
		else {
			PostMessage(hWnd, WM_CHECKEXISTINGJPIMEEX, (WPARAM)FALSE, (LPARAM)NULL);
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
// bStartThreadHookTimer()
//
BOOL		bStartThreadHookTimer(HWND hWnd)
{
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)
	
	vGetSetProfileData();

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
		if (!FlushMouseHook->bHookSet(hWnd, szFlushMouseDLL, szFlushMouse32)) {
			vMessageBox(hWnd, IDS_NOTREGISTERHOOK, MessageBoxTYPE);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}

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

	if (EventHook == NULL) {
		EventHook = new CEventHook;
		if (!EventHook->bEventSet()) {
			vMessageBox(hWnd, IDS_NOTRREGISTEVH, MessageBoxTYPE);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}

	HWND	hFindWnd = FindWindow(CLASS_FLUSHMOUSE, NULL);
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
			vDestroyWindow(hWnd);
			bReportEvent(MSG_DETECT_FLUSHMOUSE_STOP, APPLICATION_CATEGORY);
			bCreateProcess(FLUSHMOUSE_EXE);
			bReportEvent(MSG_RESTART_EVENT, APPLICATION_CATEGORY);
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