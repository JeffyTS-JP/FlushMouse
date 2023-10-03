//
// TaskTray.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/03  JeffyTS  	New edit.
// #0001	2023/06/30	JeffyTS		Divided from FlushMouseSub.cpp
//

// Include
//
#pragma once
#include "pch.h"
#include "TaskTray.h"
#include "CommonDef.h"
#include "Resource.h"
#include "FlushMouseLIB.h"
#include "..\FlushMouseDLL\EventlogDll.h"

//
// Library
//

//
// Define
//

//
// Struct Define
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
	_tcsncpy_s(nIco.szTip, ARRAYSIZE(nIco.szTip), lpszTitle, _TRUNCATE);
	try {
		throw Shell_NotifyIcon(NIM_ADD, &nIco);
	}
	catch (BOOL bRet) {
		if (!bRet) {
			_Post_equals_last_error_ DWORD err = GetLastError();
			if (err == ERROR_TIMEOUT) {
				Sleep(1000);
				try {
					throw Shell_NotifyIcon(NIM_ADD, &nIco);
				}
				catch (BOOL bRet) {
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
			if (bCreateTaskTrayWindow(hWnd, hIcon, szTitle)) {
				bTaskTray = TRUE;
				vStopThreadHookTimer(hWnd);
				if (!bStartThreadHookTimer(hWnd)) {
					bReportEvent(MSG_THREAD_HOOK_TIMER_RESTART_FAILED, APPLICATION_CATEGORY);
					PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
				}
				bReportEvent(MSG_THREAD_HOOK_TIMER_RESTARTED, APPLICATION_CATEGORY);
				return TRUE;
			}
			else {
				if (bDestroyTaskTrayWindow(hWnd)) {
					bTaskTray = FALSE;
				}
				bReportEvent(MSG_TASKTRAY_REGISTER_FAILD, APPLICATION_CATEGORY);
				vStopThreadHookTimer(hWnd);
				bReportEvent(MSG_START_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
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
			else {
				return FALSE;
			}
		}
		catch (...) {
			return FALSE;
		}
	}
	return TRUE;
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
#ifdef _DEBUG
		HRESULT	hResult = E_FAIL;
		if ((hResult = Shell_NotifyIconGetRect(&nii, lpRect)) != S_OK) {
			return FALSE;
		}
		return TRUE;
	}
#else
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
#endif // _DEBUG
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
		[[fallthrough]];	
	case WM_RBUTTONDOWN:
		bDisplayIMEModeOnCursor = FALSE;
		bDoModeDispByMouseBttnUp = FALSE;
		bDrawNearCaret = FALSE;

		POINT pt{};
		GetCursorPos(&pt);
		HMENU hMenu = LoadMenu(Resource->hLoad(), MAKEINTRESOURCE(IDR_TT_MENU));
		HMENU hSubMenu = GetSubMenu(hMenu, 0);
		SetForegroundWindow(hWnd);
		APPBARDATA stAppBarData{};
		stAppBarData.cbSize = sizeof(APPBARDATA);
		stAppBarData.hWnd = hWnd;
		SHAppBarMessage(ABM_GETTASKBARPOS, &stAppBarData);
		UINT	uFlags = 0;
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
		TrackPopupMenu(hSubMenu, uFlags, pt.x, pt.y, 0, hWnd, NULL);
		break;
	}
	return;
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
			{
				HICON	hIcon = (HICON)LoadImage(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 16, 16, 0);
				bInitDlgCenter(hDlg, hIcon, (ICC_BAR_CLASSES | ICC_HOTKEY_CLASS));
			}
			{
				TCHAR	lpText[MAX_LOADSTRING];
				if (LoadString(Resource->hLoad(), IDS_CMBX_LIST01, lpText, MAX_LOADSTRING) == 0)		return (INT_PTR)FALSE;
				SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)lpText);
				if (LoadString(Resource->hLoad(), IDS_CMBX_LIST02, lpText, MAX_LOADSTRING) == 0)		return (INT_PTR)FALSE;
				SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)lpText);
			}
			{
				bSetCheckDlgButton(hDlg, IDC_CHECK1, Profile->lpstAppRegData->bDisplayIMEModeOnCursor);
				bSetCheckDlgButton(hDlg, IDC_CHECK2, Profile->lpstAppRegData->bOffChangedFocus);
				bSetCheckDlgButton(hDlg, IDC_CHECK3, Profile->lpstAppRegData->bForceHiragana);
				bSetCheckDlgButton(hDlg, IDC_CHECK4, Profile->lpstAppRegData->bDoModeDispByIMEKeyDown);
				bSetCheckDlgButton(hDlg, IDC_CHECK5, Profile->lpstAppRegData->bDoModeDispByMouseBttnUp);
				bSetCheckDlgButton(hDlg, IDC_CHECK6, Profile->lpstAppRegData->bDoModeDispByCtrlUp);
				bSetCheckDlgButton(hDlg, IDC_CHECK7, Profile->lpstAppRegData->bDrawNearCaret);
				bSetCheckDlgButton(hDlg, IDC_CHECK8, Profile->lpstAppRegData->bEnableEPHelper);
			}
			{
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
			{
				if (Profile->lpstAppRegData->bDisplayFocusWindowIME) {
					SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETCURSEL, 0, 0);
				}
				else {
					SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETCURSEL, 1, 0);
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
			return (INT_PTR)GetStockObject(WHITE_BRUSH);
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT:
			SetBkMode(((HDC)wParam), TRANSPARENT);
			SetTextColor(((HDC)wParam), RGB(0, 0, 0));
			return (INT_PTR)GetStockObject(WHITE_BRUSH);	
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
				[[fallthrough]];	
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
						break;
					case 1:
						if (IsDlgButtonChecked(hDlg, IDC_CHECK2) != 0) {
						}
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
					Profile->bSetProfileData();
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
				[[fallthrough]];	
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

/* = EOF = */
