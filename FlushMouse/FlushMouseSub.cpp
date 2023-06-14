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
#include "CommonDef.h"
#include "Profile.h"
#include "Resource.h"
#include "..\FlushMouseDLL\GlobalHookDll.h"
#include "..\FlushMouseDLL\KeyboardHookDll.h"
#include "..\FlushMouseDLL\EventlogDll.h"
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
BOOL		bIMEInConverting = FALSE;

//
// Local Data
//
static BOOL		bTaskTray = FALSE;
static UINT		uTaskbarCreatedMessage = 0;

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
BOOL		bCreateTaskTrayWindow(HWND hWnd, HICON hIcon, LPCTSTR lpszTitle)
{
	NOTIFYICONDATA   nIco{};
	nIco.cbSize = sizeof(NOTIFYICONDATA);
	nIco.hWnd = hWnd;
	nIco.uID = NOTIFYICONDATA_ID;
	nIco.guidItem = GUID_NULL;
	nIco.uFlags = NIF_INFO | NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nIco.uCallbackMessage = WM_TASKTRAYEX;
	nIco.dwState = NIS_HIDDEN | NIS_SHAREDICON;
	nIco.dwInfoFlags = NIIF_USER | NIIF_LARGE_ICON | NIIF_NOSOUND;
	nIco.uVersion = NOTIFYICON_VERSION_4;
	nIco.hIcon = hIcon;
	if ((nIco.hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_FLUSHMOUSE))) == NULL) {
		return FALSE;
	}
	_tcsncpy_s(nIco.szTip, ARRAYSIZE(nIco.szTip), lpszTitle, _TRUNCATE);
	BOOL	bRet = FALSE;
	try {
		throw bRet = Shell_NotifyIcon(NIM_ADD, &nIco);
	}
	catch (BOOL) {
		if (!bRet) {
			_Post_equals_last_error_ DWORD err = GetLastError();
			if (err == ERROR_TIMEOUT) {
				Sleep(1000);
				try {
					throw bRet = Shell_NotifyIcon(NIM_ADD, &nIco);
				}
				catch (BOOL) {
					if (!bRet) {
						return FALSE;
					}
				}
				catch (...) {
					return FALSE;
				}
			}
			else {
				return FALSE;
			}
		}
	}
	catch (...) {
		return FALSE;
	}

	if ((uTaskbarCreatedMessage = RegisterWindowMessage(_T("TaskbarCreated"))) == 0) {
		bDestroyTaskTrayWindow(hWnd);
		return FALSE;
	}
	bTaskTray = TRUE;
	return TRUE;
}

//
// bReCreateTaskTrayWindow()
//
BOOL		bReCreateTaskTrayWindow(HWND hWnd, UINT message)
{
	if (message == uTaskbarCreatedMessage) {
		if (bDestroyTaskTrayWindow(hWnd)) {
			bTaskTray = FALSE;
		}
		HICON	hIcon = NULL;
		if ((hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_FLUSHMOUSE))) != NULL) {
			if (bCreateTaskTrayWindow(hWnd, hIcon, szTitle) == FALSE) {
				if (bDestroyTaskTrayWindow(hWnd)) {
					bTaskTray = FALSE;
				}
				bReportEvent(MSG_RESTART_EVENT, APPLICATION_CATEGORY);
				Sleep(100);
				return TRUE;
			}
			else {
				bTaskTray = TRUE;
				vStopThreadHookTimer(hWnd);
				if (!bStartThreadHookTimer(hWnd)) {
					PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				}
				bReportEvent(MSG_THREAD_HOOK_TIMER_RESTARTED, APPLICATION_CATEGORY);
				return TRUE;
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
	if (bTaskTray != FALSE) {
		NOTIFYICONDATA nIco{};
		nIco.cbSize = sizeof(NOTIFYICONDATA);
		nIco.hWnd = hWnd;
		nIco.uID = NOTIFYICONDATA_ID;
		nIco.guidItem = GUID_NULL;
		nIco.uFlags = 0;
		try {
			throw Shell_NotifyIcon(NIM_DELETE, &nIco);
		}
		catch (BOOL bRet) {
			if (bRet) {
				bTaskTray = FALSE;
				return TRUE;
			}
		}
		catch (...) {
		}
	}
	return FALSE;
}

//
// bGetTaskTrayWindowRect()
// 
BOOL		bGetTaskTrayWindowRect(HWND hWnd, LPRECT lpRect)
{
	if (bTaskTray != FALSE) {
		NOTIFYICONIDENTIFIER	nii{};
		nii.cbSize = sizeof(NOTIFYICONIDENTIFIER);
		nii.hWnd = hWnd;
		nii.uID = NOTIFYICONDATA_ID;
		nii.guidItem = GUID_NULL;
		try {
			throw Shell_NotifyIconGetRect(&nii, lpRect);
		}
		catch (HRESULT	hResult) {
			if (hResult != S_OK) {
				return FALSE;
			}
			return TRUE;
		}
		catch (...) {
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}
	return FALSE;
}

//
// WM_TASKTRAYEX
// Cls_OnTaskTrayEx()
//
void		Cls_OnTaskTrayEx(HWND hWnd, UINT id, UINT uMsg)
{
	if (id != NOTIFYICONDATA_ID) {
		return;
	}
	switch (uMsg) {
		case WM_LBUTTONDOWN:
			if (bOffChangedFocus) {
				Cime->vIMEOpenCloseForced(hWnd, IMECLOSE);
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
			stAppBarData.hWnd = hWnd;
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
				bSetCheckDlgButton(hDlg, IDC_CHECK1, Profile->lpstAppRegData->bDisplayIMEModeOnCursor);
				bSetCheckDlgButton(hDlg, IDC_CHECK2, Profile->lpstAppRegData->bOffChangedFocus);
				bSetCheckDlgButton(hDlg, IDC_CHECK3, Profile->lpstAppRegData->bForceHiragana);
				bSetCheckDlgButton(hDlg, IDC_CHECK4, Profile->lpstAppRegData->bDoModeDispByIMEKeyDown);
				bSetCheckDlgButton(hDlg, IDC_CHECK5, Profile->lpstAppRegData->bDoModeDispByMouseBttnUp);
				bSetCheckDlgButton(hDlg, IDC_CHECK6, Profile->lpstAppRegData->bDoModeDispByCtrlUp);
				bSetCheckDlgButton(hDlg, IDC_CHECK7, Profile->lpstAppRegData->bDrawNearCaret);
				bSetCheckDlgButton(hDlg, IDC_CHECK8, Profile->lpstAppRegData->bEnableEPHelper);
			}
			{	// Track Bar
				vSetTrackbar(hDlg, IDC_SLIDER1, 16, 64, 8, Profile->lpstAppRegData->iCursorSize, 8);
				vSetTrackbar(hDlg, IDC_SLIDER2, 8, 64, 8, Profile->lpstAppRegData->iModeSize, 8);
				vSetTrackbar(hDlg, IDC_SLIDER3, 100, 800, 70, Profile->lpstAppRegData->dwDisplayModeTime, 70);
				vSetTrackbar(hDlg, IDC_SLIDER4, 0, 1000, 100, Profile->lpstAppRegData->dwWaitWaveTime, 100);
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
				if (Profile->lpstAppRegData->bDisplayFocusWindowIME) {
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
				Profile->lpstAppRegData->bDisplayIMEModeOnCursor = bGetDlgButtonChecked(hDlg, IDC_CHECK1);
				Profile->lpstAppRegData->bOffChangedFocus = bGetDlgButtonChecked(hDlg, IDC_CHECK2);
				Profile->lpstAppRegData->bForceHiragana = bGetDlgButtonChecked(hDlg, IDC_CHECK3);
				Profile->lpstAppRegData->bDoModeDispByIMEKeyDown = bGetDlgButtonChecked(hDlg, IDC_CHECK4);
				Profile->lpstAppRegData->bDoModeDispByMouseBttnUp = bGetDlgButtonChecked(hDlg, IDC_CHECK5);
				Profile->lpstAppRegData->bDoModeDispByCtrlUp = bGetDlgButtonChecked(hDlg, IDC_CHECK6);
				Profile->lpstAppRegData->bDrawNearCaret = bGetDlgButtonChecked(hDlg, IDC_CHECK7);
				Profile->lpstAppRegData->bEnableEPHelper = bGetDlgButtonChecked(hDlg, IDC_CHECK8);
				Profile->lpstAppRegData->iCursorSize = (int)dwGetTrackbarPos(hDlg, IDC_SLIDER1);
				Profile->lpstAppRegData->iModeSize = (int)dwGetTrackbarPos(hDlg, IDC_SLIDER2);
				Profile->lpstAppRegData->dwDisplayModeTime = dwGetTrackbarPos(hDlg, IDC_SLIDER3);
				Profile->lpstAppRegData->dwWaitWaveTime = dwGetTrackbarPos(hDlg, IDC_SLIDER4);
				switch (SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, 0, 0)) {
					case 0:
						Profile->lpstAppRegData->bDisplayFocusWindowIME = TRUE;
						break;
					case 1:
						Profile->lpstAppRegData->bDisplayFocusWindowIME = FALSE;
						break;
					default:
						break;
				}
				if (Profile != NULL) {
					Profile->bSetProfileData();								// Set Registry data
				}
				vStopThreadHookTimer(hMainWnd);
				if (!bStartThreadHookTimer(hMainWnd)) {
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
	bDisplayIMEModeOnCursor = Profile->lpstAppRegData->bDisplayIMEModeOnCursor;
	bOffChangedFocus = Profile->lpstAppRegData->bOffChangedFocus;
	bDisplayFocusWindowIME = Profile->lpstAppRegData->bDisplayFocusWindowIME;
	bDoModeDispByIMEKeyDown = Profile->lpstAppRegData->bDoModeDispByIMEKeyDown;
	bDoModeDispByMouseBttnUp = Profile->lpstAppRegData->bDoModeDispByMouseBttnUp;
	bDoModeDispByCtrlUp = Profile->lpstAppRegData->bDoModeDispByCtrlUp;
	bDrawNearCaret = Profile->lpstAppRegData->bDrawNearCaret;
	bEnableEPHelper = Profile->lpstAppRegData->bEnableEPHelper;
	bMoveIMEToolbar = Profile->lpstAppRegData->bMoveIMEToolbar;
	bIMEModeForced = Profile->lpstAppRegData->bIMEModeForced;
}

//
// class CPowerNotification
// CPowerNotification()
//
CPowerNotification::CPowerNotification(HWND hWnd)
{
	if ((hSuspendResumeNotification = RegisterSuspendResumeNotification(hWnd, DEVICE_NOTIFY_WINDOW_HANDLE)) == NULL) {
	}
	if ((hPowerSettingNotification = RegisterPowerSettingNotification(hWnd, &guidPowerSettingNotification, DEVICE_NOTIFY_WINDOW_HANDLE)) == NULL) {
	}
}

//
// ~CPowerNotification()
//
CPowerNotification::~CPowerNotification()
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
BOOL		CPowerNotification::PowerBroadcast(HWND hWnd, ULONG Type, POWERBROADCAST_SETTING* lpSetting)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(Type);
	UNREFERENCED_PARAMETER(lpSetting);
	switch (Type) {
	case PBT_APMSUSPEND:
		vStopThreadHookTimer(hWnd);
		bReportEvent(MSG_PBT_APMSUSPEND, POWERNOTIFICATION_CATEGORY);
		bReportEvent(MSG_THREAD_HOOK_TIMER_STOPPED, POWERNOTIFICATION_CATEGORY);
		break;
	case PBT_APMRESUMEAUTOMATIC:
		bReportEvent(MSG_PBT_APMRESUMEAUTOMATIC, POWERNOTIFICATION_CATEGORY);
		break;
	case PBT_APMRESUMESUSPEND:
		vGetSetProfileData();
		if (!bStartThreadHookTimer(hWnd)) {
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		}
		bReportEvent(MSG_THREAD_HOOK_TIMER_STARTED, POWERNOTIFICATION_CATEGORY);
		bReportEvent(MSG_PBT_APMRESUMESUSPEND, POWERNOTIFICATION_CATEGORY);
		break;
	case PBT_POWERSETTINGCHANGE:
		bReportEvent(MSG_PBT_POWERSETTINGCHANGE, POWERNOTIFICATION_CATEGORY);
		break;
	case PBT_APMPOWERSTATUSCHANGE:
		bReportEvent(MSG_PBT_APMPOWERSTATUSCHANGE, POWERNOTIFICATION_CATEGORY);	
		SYSTEM_POWER_STATUS	PowerStatus{};
		if (GetSystemPowerStatus(&PowerStatus)) {
			switch (PowerStatus.ACLineStatus) {
			case 0:
				bReportEvent(MSG_PBT_APMPOWERSTATUSCHANGE_AC_OFF, POWERNOTIFICATION_CATEGORY);
				bDestroyTaskTrayWindow(hWnd);
				bReportEvent(MSG_RESTART_EVENT, POWERNOTIFICATION_CATEGORY);
				break;
			case 1:
				bReportEvent(MSG_PBT_APMPOWERSTATUSCHANGE_AC_ON, POWERNOTIFICATION_CATEGORY);
				bReportEvent(MSG_RESTART_EVENT, POWERNOTIFICATION_CATEGORY);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				break;
			default:
				break;
			}
		}
		if (lpSetting != NULL) {
			PPOWERBROADCAST_SETTING	lpPwrSetting = (POWERBROADCAST_SETTING*)lpSetting;
			if ((lpPwrSetting->PowerSetting == GUID_CONSOLE_DISPLAY_STATE)
				|| (lpPwrSetting->PowerSetting == GUID_MONITOR_POWER_ON)
				|| (lpPwrSetting->PowerSetting == GUID_SESSION_DISPLAY_STATUS)) {
				if (lpPwrSetting->Data[0] == 0) {
					bReportEvent(MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_OFF, POWERNOTIFICATION_CATEGORY);
				}
				else {
					bReportEvent(MSG_PBT_APMPOWERSTATUSCHANGE_DISPLAY_ON, POWERNOTIFICATION_CATEGORY);
				}
			}
		}
	}
	return TRUE;
}

//
// class CFocusEvent
//
//
CEventHook::CEventHook()
{
	hEventHook = NULL;
	hFormerWnd = NULL;
	hEventHookIME = NULL;
}

CEventHook::~CEventHook()
{
	if (bEventUnset()) {
		hEventHook = NULL;
		hFormerWnd = NULL;
		hEventHookIME = NULL;
	}
}

//
// bEventSet()
//
BOOL		CEventHook::bEventSet()
{
#define	EVENT_FLAGS		(WINEVENT_OUTOFCONTEXT)
	hEventHook = SetWinEventHook(
		EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,	// Range of events
		NULL,												// Handle to DLL.
		&vHandleEvent,										// The callback.
		0, 0,												// Process and thread IDs of interest (0 = all)
		EVENT_FLAGS);										// Flags.
	if (hEventHook == NULL)	return FALSE;

	hEventHookIME = SetWinEventHook(
		EVENT_OBJECT_TEXTSELECTIONCHANGED, EVENT_OBJECT_TEXTEDIT_CONVERSIONTARGETCHANGED,	// Range of events
		NULL,												// Handle to DLL.
		&vHandleEventIME,									// The callback.
		0, 0,												// Process and thread IDs of interest (0 = all)
		EVENT_FLAGS);										// Flags.
	if (hEventHookIME == NULL)	return FALSE;

	return TRUE;
}

//
// bEventUnset()
//
BOOL		CEventHook::bEventUnset()
{
	if (hEventHookIME) {
		if (UnhookWinEvent(hEventHookIME)) {
			hEventHookIME = NULL;
		}
	}
	if (hEventHook) {
		if (UnhookWinEvent(hEventHook)) {
			hEventHook = NULL;
			//dwIMEEvent = 0;
		}
	}
	if ((hEventHookIME == NULL) && (hEventHook == NULL))	return TRUE;
	return FALSE;
}

//
// vHandleEvent()
//
void CALLBACK CEventHook::vHandleEvent(HWINEVENTHOOK hook, DWORD dwEvent, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
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
		bIMEInConverting = FALSE;
		HWND	hFindWnd = FindWindow(CLASS_FLUSHMOUSE, NULL);
		if (hFindWnd != NULL) {
			PostMessage(hFindWnd, WM_EVENT_SYSTEM_FOREGROUNDEX, (WPARAM)dwEvent, (LPARAM)hWnd);
		}
	}
}

//
// vHandleEventIME()
//
void CALLBACK CEventHook::vHandleEventIME(HWINEVENTHOOK hook, DWORD dwEvent, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	UNREFERENCED_PARAMETER(hook);
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(idObject);
	UNREFERENCED_PARAMETER(idChild);
	UNREFERENCED_PARAMETER(dwEventThread);
	UNREFERENCED_PARAMETER(dwmsEventTime);

	switch (dwEvent) {
	case EVENT_OBJECT_CLOAKED:
		bIMEInConverting = FALSE;
		break;
	case EVENT_OBJECT_UNCLOAKED:
	case EVENT_OBJECT_IME_SHOW:
	case EVENT_OBJECT_IME_CHANGE:
		bIMEInConverting = TRUE;
		break;
	default:
		return;
	}
}

//
// class CFlushMouseHook 
//
CFlushMouseHook::CFlushMouseHook()
{
	hHook64Dll = NULL;
	bGlobalHook64 = FALSE;
	bKeyboardHookLL64 = FALSE;
	bMouseHook64 = FALSE;
	bHook32Dll = FALSE;
	lpstProcessInfomation = new PROCESS_INFORMATION[sizeof(PROCESS_INFORMATION)];
	if (lpstProcessInfomation)	ZeroMemory(lpstProcessInfomation, sizeof(PROCESS_INFORMATION));
}

CFlushMouseHook::~CFlushMouseHook()
{
	bHookUnset();
	if (lpstProcessInfomation)	delete[]	lpstProcessInfomation;
	lpstProcessInfomation = NULL;
}
//
// bHookSet()
//
BOOL			CFlushMouseHook::bHookSet(HWND hWnd, LPCTSTR lpszDll64Name, LPCTSTR lpszExec32Name)
{
	UNREFERENCED_PARAMETER(lpszDll64Name);
	if ((bGlobalHook64 = bGlobalHookSet(hWnd)) != FALSE) {
		if ((bKeyboardHookLL64 = bKeyboardHookLLSet(hWnd)) != FALSE) {
			if ((bHook32Dll = bHook32DllStart(hWnd, lpszExec32Name)) != FALSE) {
				return TRUE;
			}
		}
	}
	bHookUnset();
	return FALSE;
}

//
// bHookUnset()
//
BOOL		CFlushMouseHook::bHookUnset()
{
	if (bHook32Dll)			bHook32DllStop();
	if (bKeyboardHookLL64)	bKeyboardHookLLUnset();
	if (bGlobalHook64)		bGlobalHookUnset();
	return TRUE;
}

//
// bHook32DllStart()
//
BOOL	 	CFlushMouseHook::bHook32DllStart(HWND hWnd, LPCTSTR lpszExec32Name)
{
#define	COMAMANDLINESIZE (sizeof(_T(" ")) * (sizeof(unsigned long long) + 1))
	BOOL	bRet = FALSE;
	DWORD	dwSize = 0;
	dwSize = ExpandEnvironmentStrings(lpszExec32Name, NULL, 0);
	LPTSTR	lpszBuffer = new TCHAR[dwSize];
	if (lpszBuffer) {
		ZeroMemory(lpszBuffer, dwSize);
		dwSize = ExpandEnvironmentStrings(lpszExec32Name, lpszBuffer, dwSize);

		LPTSTR	lpszCommandLine = NULL;
		lpszCommandLine = new TCHAR[dwSize + COMAMANDLINESIZE];
		if (lpszCommandLine) {
			ZeroMemory(lpszCommandLine, (dwSize + sizeof(COMAMANDLINESIZE)));
			_sntprintf_s(lpszCommandLine, (dwSize + COMAMANDLINESIZE), _TRUNCATE, _T("%s %llu"), lpszBuffer, (unsigned long long)hWnd);
			if (lpstProcessInfomation) {
				STARTUPINFO	stStartupInfo{};		stStartupInfo.cb = sizeof(STARTUPINFO);
				if ((bRet = CreateProcess(NULL, lpszCommandLine, NULL, NULL, TRUE,
					NORMAL_PRIORITY_CLASS, NULL, NULL, &stStartupInfo, lpstProcessInfomation)) == FALSE) {
				}
				else {
					bHook32Dll = TRUE;
				}
			}
			delete[]	lpszCommandLine;
		}
		delete[]		lpszBuffer;
	}
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
	if (lpstProcessInfomation != NULL) {
		if (lpstProcessInfomation->hProcess != NULL) {
			if (!EnumWindows((WNDENUMPROC)&CFlushMouseHook::bEnumWindowsProcHookStop, (LPARAM)lpstProcessInfomation)) {
				if (GetLastError() == ERROR_SUCCESS) {
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
						bRet = TRUE;
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
			}
		}
	}
	return bRet;
}

//
// bEnumWindowsProc()
//
BOOL CALLBACK CFlushMouseHook::bEnumWindowsProcHookStop(HWND hWnd, LPARAM lParam)
{
	LPPROCESS_INFORMATION pi = (LPPROCESS_INFORMATION)lParam;

	DWORD lpdwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &lpdwProcessId);

	if (pi->dwProcessId == lpdwProcessId)
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
		SetLastError(ERROR_SUCCESS);
		return FALSE;
	}
	return TRUE;
}

/* = EOF = */