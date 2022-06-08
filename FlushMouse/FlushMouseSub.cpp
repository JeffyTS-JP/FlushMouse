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
#include "FlushMouseLIB.h"
#include "FlushMouseSub.h"
#include "Profile.h"
#include "Resource.h"

#include "..\FlushMouseDLL32\FlushMouseDll32.h"
#include "..\MiscLIB\CRegistry.h"

//
// Define
//

//
// Global Data
//
HWND		hAboutDlg = NULL;
HWND		hSettingDlg = NULL;

//
// Local Data
//
static BOOL		bOffChangedFocus = FALSE;

//
// Local Prototype Define
//
static INT_PTR CALLBACK SettingDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static BOOL		bInitDlgCenter(HWND hDlg, HICON hIcon, DWORD dwICC);
static BOOL		bSetCheckDlgButton(HWND hDlg, int iIDButton, BOOL bCheck);
static VOID		vSetTrackbar(HWND hDlg, int iIDTrackbar, int iRangeMin, int iRangeMax, int iTickFreq, int iPos, int iPageSize);
static BOOL		bGetDlgButtonChecked(HWND hDlg, int iIDButton);
static DWORD	dwGetTrackbarPos(HWND hDlg, int iIDTrackbar);

//
// bCreateTaskTrayWindow()
//
BOOL		bCreateTaskTrayWindow(HWND hWnd, UINT uID)
{
	NOTIFYICONDATA   nIco{};
	nIco.cbSize = sizeof(NOTIFYICONDATA);
	nIco.hWnd = hWnd;
	nIco.uID = uID;
	nIco.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nIco.uCallbackMessage = WM_TASKTRAY;
	if ((nIco.hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(uID))) == NULL) {
		return FALSE;
	}
	_tcsncpy_s(nIco.szTip, ARRAYSIZE(nIco.szTip), szTitle, _TRUNCATE);
	if (Shell_NotifyIcon(NIM_ADD, &nIco) == FALSE) {			// Add TaskTray Icon
		if (GetLastError() == ERROR_TIMEOUT) {
			Sleep(1000);
			if (Shell_NotifyIcon(NIM_ADD, &nIco) == FALSE) {	// Retry once
				return FALSE;
			}
		}
		else {
			return FALSE;
		}
	}
	return TRUE;
}

//
// bDestroyTaskTrayWindow()
// 
BOOL		bDestroyTaskTrayWindow(HWND hWnd)
{
	NOTIFYICONDATA nIco{};
	nIco.cbSize = sizeof(NOTIFYICONDATA);
	nIco.hWnd = hWnd;
	nIco.uID = IDI_FLUSHMOUSE;
	nIco.uFlags = 0;
	return Shell_NotifyIcon(NIM_DELETE, &nIco);
}

//
// WM_TASKTRAY
// Cls_OnTaskTray()
//
void		Cls_OnTaskTray(HWND hWnd, UINT id, UINT uMsg)
{
	if (id != IDI_FLUSHMOUSE) {
		return;
	}
	switch (uMsg) {
		case WM_LBUTTONDOWN:
			if (bOffChangedFocus) {
				Cursor->vIMEOpenCloseForced(hWnd, IMECLOSE);
			}
			[[fallthrough]];						// fallthrough is explicit
			//break;
		case WM_RBUTTONDOWN:
			bDisplayIMEModeOnCursor = FALSE;
			bDoModeDispByMouseBttnUp = FALSE;
			bDrawNearCaret = FALSE;

			// Display menu when left & right button is clicked on task tray icon.
			POINT pt{};
			GetCursorPos(&pt);
			HMENU hMenu = LoadMenu(Resource->hLoad(), MAKEINTRESOURCE(IDR_TT_MENU));
			HMENU hSubMenu = GetSubMenu(hMenu, 0);
			SetForegroundWindow(hWnd);
			APPBARDATA stAppBarData{};
			stAppBarData.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &stAppBarData);
			UINT		uFlags = 0;
			switch (stAppBarData.uEdge) {
				case ABE_TOP:
					uFlags = TPM_RIGHTALIGN | TPM_TOPALIGN;
					if (pt.y < stAppBarData.rc.bottom)	pt.y = stAppBarData.rc.bottom;
					break;
				case ABE_BOTTOM:
					uFlags = TPM_RIGHTALIGN | TPM_BOTTOMALIGN;
					if (pt.y > stAppBarData.rc.top)	pt.y = stAppBarData.rc.top;
					break;
				case ABE_LEFT:
					uFlags = TPM_LEFTALIGN | TPM_BOTTOMALIGN;
					if (pt.x < stAppBarData.rc.right)	pt.x = stAppBarData.rc.right;
					break;
				case ABE_RIGHT:
					uFlags = TPM_RIGHTALIGN | TPM_BOTTOMALIGN;
					if (pt.x > stAppBarData.rc.left)		pt.x = stAppBarData.rc.left;
					break;
			}
			if (hAboutDlg != NULL) {
				//EnableMenuItem(hSubMenu, IDR_TT_ABOUT, MF_BYCOMMAND | MF_DISABLED);
			}
			if (hSettingDlg != NULL) {
				//EnableMenuItem(hSubMenu, IDR_TT_SETTING, MF_BYCOMMAND | MF_DISABLED);
			}
			TrackPopupMenu(hSubMenu, uFlags, pt.x, pt.y, 0, hWnd, NULL);
			DestroyMenu(hMenu);
			PostMessage(hWnd, WM_NULL, 0, 0);
			break;
	}
}

//
// vSettingDialog()
//
VOID		vSettingDialog(HWND hWnd)
{
	if (hSettingDlg != NULL) {
		SetForegroundWindow(hSettingDlg);
		return;
	}
	DialogBox(Resource->hLoad(), MAKEINTRESOURCE(IDD_SETTING), hWnd, SettingDlg);
}

//
// SettingDlg()
//
static INT_PTR CALLBACK SettingDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
		case WM_INITDIALOG:
			hSettingDlg = hDlg;
			{	// Initialize
				HICON	hIcon = (HICON)LoadImage(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 16, 16, 0);
				bInitDlgCenter(hDlg, hIcon, (ICC_BAR_CLASSES | ICC_HOTKEY_CLASS));
			}
			{	// Combo Box
				TCHAR       lpText[MAX_LOADSTRING];
				if (LoadString(Resource->hLoad(), IDS_CMBX_LIST01, lpText, MAX_LOADSTRING) == 0)		return (INT_PTR)FALSE;
				SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)lpText);
				if (LoadString(Resource->hLoad(), IDS_CMBX_LIST02, lpText, MAX_LOADSTRING) == 0)		return (INT_PTR)FALSE;
				SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)lpText);
			}
			{	// Check Box
				bSetCheckDlgButton(hDlg, IDC_CHECK1, Profile->stAppRegData.bDisplayIMEModeOnCursor);
				bSetCheckDlgButton(hDlg, IDC_CHECK2, Profile->stAppRegData.bOffChangedFocus);
				bSetCheckDlgButton(hDlg, IDC_CHECK3, Profile->stAppRegData.bForceHiragana);
				bSetCheckDlgButton(hDlg, IDC_CHECK4, Profile->stAppRegData.bDoModeDispByIMEKeyDown);
				bSetCheckDlgButton(hDlg, IDC_CHECK5, Profile->stAppRegData.bDoModeDispByMouseBttnUp);
				bSetCheckDlgButton(hDlg, IDC_CHECK6, Profile->stAppRegData.bDoModeDispByCtrlUp);
				bSetCheckDlgButton(hDlg, IDC_CHECK7, Profile->stAppRegData.bDrawNearCaret);
				bSetCheckDlgButton(hDlg, IDC_CHECK8, Profile->stAppRegData.bEnableEPHelper);
			}
			{	// Track Bar
				vSetTrackbar(hDlg, IDC_SLIDER1, 16, 64, 8, Profile->stAppRegData.iCursorSize, 8);
				vSetTrackbar(hDlg, IDC_SLIDER2, 8, 64, 8, Profile->stAppRegData.iModeSize, 8);
				vSetTrackbar(hDlg, IDC_SLIDER3, 100, 800, 70, Profile->stAppRegData.dwDisplayModeTime, 70);
				vSetTrackbar(hDlg, IDC_SLIDER4, 0, 1000, 100, Profile->stAppRegData.dwWaitWaveTime, 100);
				if (IsDlgButtonChecked(hDlg, IDC_CHECK3) == 0 && IsDlgButtonChecked(hDlg, IDC_CHECK4) == 0) {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), FALSE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), FALSE);
				}
				else {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), TRUE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), TRUE);
				}
				if (IsDlgButtonChecked(hDlg, IDC_CHECK4) != 0)	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), TRUE);
				else EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), FALSE);
			}
			{	// 既定の動作
				if (Profile->stAppRegData.bDisplayFocusWindowIME) {
					SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETCURSEL, 0, 0);
					//EnableWindow(GetDlgItem(hDlg, IDC_CHECK2), TRUE);
				}
				else {
					SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETCURSEL, 1, 0);
					//CheckDlgButton(hDlg, IDC_CHECK2, BST_UNCHECKED);
					//EnableWindow(GetDlgItem(hDlg, IDC_CHECK2), FALSE);
				}
			}
			SetFocus(GetDlgItem(hDlg, IDC_CHECK1));
			return (INT_PTR)TRUE;
		case WM_HSCROLL:
			{	// Track Bar
				if ((GetDlgItem(hDlg, IDC_SLIDER1) == (HWND)lParam)
					|| (GetDlgItem(hDlg, IDC_SLIDER2) == (HWND)lParam)
					|| (GetDlgItem(hDlg, IDC_SLIDER3) == (HWND)lParam)
					|| (GetDlgItem(hDlg, IDC_SLIDER4) == (HWND)lParam)) {
					EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				}
			}
			break;
		case WM_CTLCOLORDLG:
			return (INT_PTR)GetStockObject(WHITE_BRUSH);							// 背景の色
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT:
			SetBkMode(((HDC)wParam), TRANSPARENT);
			SetTextColor(((HDC)wParam), RGB(0, 0, 0));								// 文字の色
			return (INT_PTR)GetStockObject(WHITE_BRUSH);							// 背景の色
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDC_CHECK4:
			case IDC_CHECK5:
			case IDC_CHECK6:
				if (IsDlgButtonChecked(hDlg, IDC_CHECK4) == 0 && IsDlgButtonChecked(hDlg, IDC_CHECK5) == 0 && IsDlgButtonChecked(hDlg, IDC_CHECK6) == 0) {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), FALSE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), FALSE);
				}
				else {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), TRUE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), TRUE);
				}
				if (IsDlgButtonChecked(hDlg, IDC_CHECK6) != 0)	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), TRUE);
				else EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), FALSE);
				[[fallthrough]];						// fallthrough is explicit
			case IDC_CHECK1:
			case IDC_CHECK2:
			case IDC_CHECK3:
			case IDC_CHECK7:
			case IDC_CHECK8:
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				break;
			case IDC_COMBO1:
				if (HIWORD(wParam) == CBN_SELCHANGE) {
					switch (SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, 0, 0)) {
					case 0:
						//EnableWindow(GetDlgItem(hDlg, IDC_CHECK2), TRUE);
						break;
					case 1:
						if (IsDlgButtonChecked(hDlg, IDC_CHECK2) != 0) {
							//CheckDlgButton(hDlg, IDC_CHECK2, BST_UNCHECKED);
						}
						//EnableWindow(GetDlgItem(hDlg, IDC_CHECK2), FALSE);
						break;
					default:
						break;
					}
					EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				}
				break;
			case IDC_BUTTON1:
				SetFocus(GetDlgItem(hDlg, IDC_CHECK1));
				if (IsDlgButtonChecked(hDlg, IDC_CHECK4) == 0 && IsDlgButtonChecked(hDlg, IDC_CHECK5) == 0 && IsDlgButtonChecked(hDlg, IDC_CHECK6) == 0) {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), FALSE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), FALSE);
				}
				else {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), TRUE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), TRUE);
				}
				if (IsDlgButtonChecked(hDlg, IDC_CHECK6) != 0)	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), TRUE);
				else EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), FALSE);
				[[fallthrough]];						// fallthrough is explicit
			case IDOK:
				Profile->stAppRegData.bDisplayIMEModeOnCursor = bGetDlgButtonChecked(hDlg, IDC_CHECK1);
				Profile->stAppRegData.bOffChangedFocus = bGetDlgButtonChecked(hDlg, IDC_CHECK2);
				Profile->stAppRegData.bForceHiragana = bGetDlgButtonChecked(hDlg, IDC_CHECK3);
				Profile->stAppRegData.bDoModeDispByIMEKeyDown = bGetDlgButtonChecked(hDlg, IDC_CHECK4);
				Profile->stAppRegData.bDoModeDispByMouseBttnUp = bGetDlgButtonChecked(hDlg, IDC_CHECK5);
				Profile->stAppRegData.bDoModeDispByCtrlUp = bGetDlgButtonChecked(hDlg, IDC_CHECK6);
				Profile->stAppRegData.bDrawNearCaret = bGetDlgButtonChecked(hDlg, IDC_CHECK7);
				Profile->stAppRegData.bEnableEPHelper = bGetDlgButtonChecked(hDlg, IDC_CHECK8);
				Profile->stAppRegData.iCursorSize = (int)dwGetTrackbarPos(hDlg, IDC_SLIDER1);
				Profile->stAppRegData.iModeSize = (int)dwGetTrackbarPos(hDlg, IDC_SLIDER2);
				Profile->stAppRegData.dwDisplayModeTime = dwGetTrackbarPos(hDlg, IDC_SLIDER3);
				Profile->stAppRegData.dwWaitWaveTime = dwGetTrackbarPos(hDlg, IDC_SLIDER4);
				switch (SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, 0, 0)) {
					case 0:
						Profile->stAppRegData.bDisplayFocusWindowIME = TRUE;
						break;
					case 1:
						Profile->stAppRegData.bDisplayFocusWindowIME = FALSE;
						break;
					default:
						break;
				}
				if (Profile != NULL) {
					Profile->bSetProfileData();								// Set Registry data
				}
				vStopThredHookTimer(hMainWnd);
				vGetSetProfileData();
				if (!bStartThredHookTimer(hMainWnd)) {
					PostMessage(hMainWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
					return (INT_PTR)FALSE;
				}
				if (LOWORD(wParam) == IDC_BUTTON1) {
					EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);
					return (INT_PTR)FALSE;
				}
				[[fallthrough]];						// fallthrough is explicit
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))	hSettingDlg = NULL;
				return (INT_PTR)TRUE;
			}
	}
	return (INT_PTR)FALSE;
}

//
// bInitDlgCenter()
//
static BOOL		bInitDlgCenter(HWND hDlg, HICON hIcon, DWORD dwICC)
{
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int	cy = GetSystemMetrics(SM_CYSCREEN);
	RECT		rc{};
	GetWindowRect(hDlg, (LPRECT)&rc);
	rc.right = rc.right - rc.left + 1;	rc.bottom = rc.bottom - rc.top + 1;
	rc.left = (cx - rc.right) / 2;		rc.top = (cy - rc.bottom) / 2;
	MoveWindow(hDlg, rc.left, rc.top, rc.right, rc.bottom, TRUE);
	SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	// Initialize
	INITCOMMONCONTROLSEX stICC{};
	stICC.dwSize = sizeof(INITCOMMONCONTROLSEX);
	stICC.dwICC = dwICC;
	return InitCommonControlsEx(&stICC);
}

//
// bSetCheckDlgButton()
//
static BOOL		bSetCheckDlgButton(HWND hDlg, int iIDButton, BOOL bCheck)
{
	UINT		uCheck = 0;
	if (bCheck) uCheck = BST_CHECKED;
	else			uCheck = BST_UNCHECKED;
	if (!CheckDlgButton(hDlg, iIDButton, uCheck)) {
		return FALSE;
	}
	return TRUE;
}

//
// vSetTrackbar()
//
static VOID		vSetTrackbar(HWND hDlg, int iIDTrackbar, int iRangeMin, int iRangeMax, int iTickFreq, int iPos, int iPageSize)
{
	SendDlgItemMessage(hDlg, iIDTrackbar, TBM_SETRANGE, TRUE, MAKELPARAM(iRangeMin, iRangeMax));
	SendDlgItemMessage(hDlg, iIDTrackbar, TBM_SETTICFREQ, iTickFreq, 0);
	SendDlgItemMessage(hDlg, iIDTrackbar, TBM_SETPOS, TRUE, iPos);
	SendDlgItemMessage(hDlg, iIDTrackbar, TBM_SETPAGESIZE, 0, iPageSize);
}

//
// bGetDlgButtonChecked()
//
static BOOL		bGetDlgButtonChecked(HWND hDlg, int iIDButton)
{
	if (IsDlgButtonChecked(hDlg, iIDButton) == 0)	return FALSE;
	else return TRUE;
}

//
// dwGetTrackbarPos()
//
static DWORD		dwGetTrackbarPos(HWND hDlg, int iIDTrackbar)
{
	return (DWORD)SendDlgItemMessage(hDlg, iIDTrackbar, TBM_GETPOS, NULL, NULL);
}

//
// vGetSetProfileData()
//
VOID			vGetSetProfileData()
{
	bDisplayIMEModeOnCursor = Profile->stAppRegData.bDisplayIMEModeOnCursor;
	bDisplayFocusWindowIME = Profile->stAppRegData.bDisplayFocusWindowIME;
	bDoModeDispByIMEKeyDown = Profile->stAppRegData.bDoModeDispByIMEKeyDown;
	bDoModeDispByMouseBttnUp = Profile->stAppRegData.bDoModeDispByMouseBttnUp;
	bDoModeDispByCtrlUp = Profile->stAppRegData.bDoModeDispByCtrlUp;
	bOffChangedFocus = Profile->stAppRegData.bOffChangedFocus;
	bDrawNearCaret = Profile->stAppRegData.bDrawNearCaret;
	bEnableEPHelper = Profile->stAppRegData.bEnableEPHelper;
}

//
// class CFocusEvent
//
//
CFocusEvent::CFocusEvent()
{
	hEventHook = NULL;
}

CFocusEvent::~CFocusEvent()
{
	bEventUnset();
}

//
// bEventSet()
//
BOOL		CFocusEvent::bEventSet()
{
#define	EVENT_FLAGS		(WINEVENT_OUTOFCONTEXT)
	hEventHook = SetWinEventHook(
		EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,	// Range of events
		NULL,												// Handle to DLL.
		&vHandleEvent,										// The callback.
		0, 0,												// Process and thread IDs of interest (0 = all)
		EVENT_FLAGS);										// Flags.
	if (hEventHook == NULL)	return FALSE;
	return TRUE;
}

//
// bEventUnset()
//
BOOL		CFocusEvent::bEventUnset()
{
	BOOL		bRet = FALSE;
	bRet = UnhookWinEvent(hEventHook);
	return bRet;
}

//
// vHandleEvent()
//
void CALLBACK CFocusEvent::vHandleEvent(HWINEVENTHOOK hook, DWORD dwEvent, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	UNREFERENCED_PARAMETER(hook);
	UNREFERENCED_PARAMETER(dwEvent);
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(idObject);
	UNREFERENCED_PARAMETER(idChild);
	UNREFERENCED_PARAMETER(dwEventThread);
	UNREFERENCED_PARAMETER(dwmsEventTime);

	if (hWnd == NULL)	return;
	if (dwEvent == EVENT_SYSTEM_FOREGROUND) {
		if (bCheckExistingJPIME() && bEnableEPHelper)	bForExplorerPatcherSWS(hWnd, FALSE, NULL, NULL);
		if (hMainWnd != hWnd) {
			if (bOffChangedFocus) {
				Cursor->vIMEOpenCloseForced(hWnd, IMECLOSE);
				if (!Cursor->bStartIMECursorChangeThread(hWnd))		return;		// error
			}
			HWND	hWndObserved = NULL;
			if (bDisplayFocusWindowIME) {
				hWndObserved = hWnd;
			}
			else {
				POINT	pt{};
				if (GetCursorPos(&pt)) {
					RECT	rc{};
					NOTIFYICONIDENTIFIER	nii{};
					nii.cbSize = sizeof(NOTIFYICONIDENTIFIER);
					nii.hWnd = hMainWnd;	nii.uID = IDI_FLUSHMOUSE;	nii.guidItem = GUID_NULL;
					if (Shell_NotifyIconGetRect(&nii, &rc) != S_OK) {
						return;
					}
				}
			}
			if (!Cursor->bStartDrawIMEModeThread(hWndObserved))	return;			// error
		}
	}
}

//
// class CFlushMouseHook 
//
CFlushMouseHook::CFlushMouseHook()
{
	hHook64Dll = NULL;
	bGlobalHook64 = FALSE;
	bKeyboardHook64 = FALSE;
	bMouseHook64 = FALSE;
	bHook32Dll = FALSE;
	lpstProcessInfomation = NULL;
}

CFlushMouseHook::~CFlushMouseHook()
{
	bHookUnset();
}

#define		_KEYBOARDHOKK32
//
// bHookSet()
//
BOOL			CFlushMouseHook::bHookSet(HWND hWnd, LPCTSTR lpszDll64Name, LPCTSTR lpszExec32Name)
{
	UNREFERENCED_PARAMETER(lpszDll64Name);
	if ((bHook32Dll = bHook32DllStart(hWnd, lpszExec32Name)) != FALSE) {
		return TRUE;
	}
	bHook32DllStop();
	return FALSE;
}

//
// bHookUnset()
//
BOOL		CFlushMouseHook::bHookUnset()
{
	if (bHook32Dll)		bHook32DllStop();
	return TRUE;
}

//
// bHook64DllLoad()
//
BOOL			CFlushMouseHook::bHook64DllLoad(LPCTSTR lpszDll64Name)
{
	DWORD	dwSize = 0;
	dwSize = ExpandEnvironmentStrings(lpszDll64Name, NULL, 0);
	LPTSTR	lpszBuffer = new TCHAR[dwSize];
	ZeroMemory(lpszBuffer, dwSize);
	dwSize = ExpandEnvironmentStrings(lpszDll64Name, lpszBuffer, dwSize);
	BOOL		bRet = FALSE;
	if (hHook64Dll == NULL) {
		if ((hHook64Dll = LoadLibraryEx(lpszBuffer, NULL, 0)) != NULL) {
			bRet = TRUE;
		}
	}
	delete[]	lpszBuffer;
	return bRet;
}

//
// bHook64DllUnload()
//
BOOL			CFlushMouseHook::bHook64DllUnload()
{
	if (hHook64Dll != NULL) {
		if (!FreeLibrary(hHook64Dll)) {
			return FALSE;
		}
	}
	hHook64Dll = NULL;
	return TRUE;
}

//
// bHook32DllStart()
//
BOOL	 	CFlushMouseHook::bHook32DllStart(HWND hWnd, LPCTSTR lpszExec32Name)
{
#define	COMAMANDLINESIZE (sizeof(_T(" ")) * (sizeof(unsigned long long) + 1))
	DWORD	dwSize = 0;
	dwSize = ExpandEnvironmentStrings(lpszExec32Name, NULL, 0);
	LPTSTR	lpszBuffer = new TCHAR[dwSize];
	ZeroMemory(lpszBuffer, dwSize);
	dwSize = ExpandEnvironmentStrings(lpszExec32Name, lpszBuffer, dwSize);

	LPTSTR	lpszCommandLine = NULL;
	lpszCommandLine = new TCHAR[dwSize + COMAMANDLINESIZE];
	ZeroMemory(lpszCommandLine, (dwSize + sizeof(COMAMANDLINESIZE)));
	_sntprintf_s(lpszCommandLine, (dwSize + COMAMANDLINESIZE), _TRUNCATE, _T("%s %llu"), lpszBuffer, (unsigned long long)hWnd);
	if (lpszBuffer != NULL)	delete[]		lpszBuffer;

	lpstProcessInfomation = new PROCESS_INFORMATION[sizeof(PROCESS_INFORMATION)];
	ZeroMemory(lpstProcessInfomation, sizeof(PROCESS_INFORMATION));
	STARTUPINFO	stStartupInfo{};		stStartupInfo.cb = sizeof(STARTUPINFO);
	BOOL		bRet = FALSE;
	if ((bRet = CreateProcess(NULL, lpszCommandLine, NULL, NULL, TRUE,
		NORMAL_PRIORITY_CLASS, NULL, NULL, &stStartupInfo, lpstProcessInfomation)) == FALSE) {
			delete[] lpstProcessInfomation;
	}
	else {
		bHook32Dll = TRUE;
	}
	if (lpszCommandLine != NULL)		delete[]	lpszCommandLine;
	return bRet;
}

//
// bKeyboardHookUnset32()
//
BOOL 	CFlushMouseHook::bHook32DllStop()
{
#define	TIMEOUT	1000 
	if (!bHook32Dll)	return TRUE;
	BOOL		bRet = FALSE;
	if (lpstProcessInfomation->hProcess != NULL) {
		if (!EnumWindows((WNDENUMPROC)&CFlushMouseHook::bEnumWindowsProc, (LPARAM)lpstProcessInfomation)) {
			return FALSE;
		}
		DWORD dwRet = WaitForSingleObject(lpstProcessInfomation->hProcess, TIMEOUT);
		switch (dwRet) {
			case WAIT_OBJECT_0:
				bRet = TRUE;
				break;
			case WAIT_FAILED:
			case WAIT_ABANDONED:
			case WAIT_TIMEOUT:
			default:
				if (TerminateProcess(lpstProcessInfomation->hProcess, 0)) {
					bRet = TRUE;
				}
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
	delete[] lpstProcessInfomation;
	return bRet;
}

//
// bEnumWindowsProc()
//
BOOL CALLBACK CFlushMouseHook::bEnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	LPPROCESS_INFORMATION pi = (LPPROCESS_INFORMATION)lParam;

	DWORD lpdwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &lpdwProcessId);

	if (pi->dwProcessId == lpdwProcessId)
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}
	return TRUE;
}

