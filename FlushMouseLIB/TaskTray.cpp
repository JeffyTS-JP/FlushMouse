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
#include "Eventlog.h"
#include "FlushMouseLIB.h"

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

//
// Local Data
//
static BOOL		bTaskTray = FALSE;
static UINT		uTaskbarCreatedMessage = 0;

//
// Local Prototype Define
//

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
// bCheckTaskTrayMessage()
//
BOOL		bCheckTaskTrayMessage(HWND hWnd, UINT message)
{
	if (message == uTaskbarCreatedMessage) {	
		if (bDestroyTaskTrayWindow(hWnd)) {
			bTaskTray = FALSE;
		}
		if (bReCreateTaskTrayWindow(hWnd)) {
			return TRUE;
		}
		else return FALSE;
	}
	return TRUE;
}

//
// bReCreateTaskTrayWindow()
//
BOOL		bReCreateTaskTrayWindow(HWND hWnd)
{
	HICON	hIcon = NULL;
	if ((hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_FLUSHMOUSE))) != NULL) {
		if (bCreateTaskTrayWindow(hWnd, hIcon, szTitle)) {
			bTaskTray = TRUE;
			if (!Cursor->bReloadCursor()) {
				bReportEvent(MSG_THREAD_HOOK_TIMER_RESTART_FAILED, APPLICATION_CATEGORY);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			}
			bReportEvent(MSG_THREAD_HOOK_TIMER_RESTARTED, APPLICATION_CATEGORY);
		}
		else {
			if (bDestroyTaskTrayWindow(hWnd)) {
				bTaskTray = FALSE;
			}
			bReportEvent(MSG_TASKTRAY_REGISTER_FAILD, APPLICATION_CATEGORY);
			bReportEvent(MSG_START_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		}
	}
	else {
		return FALSE;
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
		if ((Profile != NULL) && Profile->lpstAppRegData->bOffChangedFocus) {
			Cime->vIMEOpenCloseForced(hWnd, IMECLOSE);
		}
		[[fallthrough]];	
	case WM_RBUTTONDOWN:
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


/* = EOF = */
