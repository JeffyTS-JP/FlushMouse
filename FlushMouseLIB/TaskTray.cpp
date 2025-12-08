//
// TaskTray.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
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
#include "..\FlushMouseDLL\EventlogData.h"

//
// Library
//

//
// Define
//

//
// Struct Define
//
typedef struct tagBALLOON_PARAM {
	HWND	hWnd;
	DWORD	dwInfoFlags;
	TCHAR	szInfoTitle[ARRAYSIZE(((NOTIFYICONDATA*)0)->szInfoTitle)];
	TCHAR	szInfo[ARRAYSIZE(((NOTIFYICONDATA*)0)->szInfo)];
} BALLOON_PARAM, *PBALLOON_PARAM, *LPBALLOON_PARAM;

//
// Global Data
//

//
// Local Data
//

//
// Local Prototype Define
//
static DWORD WINAPI	BalloonThreadProc(LPVOID lpParam);

//
// iCheckTaskTrayMessage()
//

//
// Class CTaskTray
//
CTaskTray::CTaskTray()
	: uTaskTrayID(NOTIFYICONDATA_ID), uTaskbarCreated(0)
{
	if ((uTaskbarCreated = RegisterWindowMessage(_T("TaskbarCreated"))) == 0) {
		uTaskbarCreated = 0;
		return;
	}
}

CTaskTray::~CTaskTray()
{

}

//
// bCreateTaskTrayWindow()
//
BOOL		CTaskTray::bCreateTaskTrayWindow(HWND hWnd, HICON hIcon, LPCTSTR lpszTitle) const
{
	if ((uTaskTrayID == 0) || (uTaskbarCreated == 0))	return FALSE;

	NOTIFYICONDATA   nIco{};
	nIco.cbSize = sizeof(NOTIFYICONDATA);
	nIco.hWnd = hWnd;
	nIco.uID = uTaskTrayID;
	nIco.guidItem = GUID_NULL;
	nIco.uCallbackMessage = WM_TASKTRAYEX;
	nIco.dwState = NIS_HIDDEN | NIS_SHAREDICON;
	nIco.dwInfoFlags = NIIF_USER | NIIF_LARGE_ICON | NIIF_NOSOUND;
	nIco.uVersion = NOTIFYICON_VERSION_4;
	nIco.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nIco.hIcon = hIcon;
	_tcsncpy_s(nIco.szTip, ARRAYSIZE(nIco.szTip), lpszTitle, _TRUNCATE);

	BOOL bRet = Shell_NotifyIcon(NIM_ADD, &nIco);
	if (bRet) return TRUE;

	DWORD err = GetLastError();
	if (err == ERROR_TIMEOUT) {
		Sleep(2000);
		if (Shell_NotifyIcon(NIM_MODIFY, &nIco)) return TRUE;
		err = GetLastError();
		if (err == ERROR_TIMEOUT) {
			Sleep(2000);
			if (Shell_NotifyIcon(NIM_ADD, &nIco)) return TRUE;
		}
	}
	bReportEvent(MSG_TASKTRAY_REGISTER_FAILD, APPLICATION_CATEGORY);
	return FALSE;
}

//
// bReCreateTaskTrayWindow()
//
BOOL		CTaskTray::bReCreateTaskTrayWindow(HWND hWnd) const
{
	if (!Resource || !Cursor)	return FALSE;
	if (!bDestroyTaskTrayWindow(hWnd)) {
		bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)0, (LPARAM)0);
	}
	HICON	hIcon = NULL;
	if ((hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_FLUSHMOUSE))) != NULL) {
		BOOL	bRet = FALSE;
		for (int i = 0; i < 3; i++) {
			if ((bRet = bCreateTaskTrayWindow(hWnd, hIcon, g_szWindowTitle)))	break;
			Sleep(1000);
		}
		if (bRet) {
			if (!Cursor->bReloadCursor()) {
				bReportEvent(MSG_THREAD_HOOK_TIMER_RESTART_FAILED, APPLICATION_CATEGORY);
			}
		}
		else {
			bDestroyTaskTrayWindow(hWnd);
			bReportEvent(MSG_TASKTRAY_REGISTER_FAILD, APPLICATION_CATEGORY);
			bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)0, (LPARAM)0);
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
BOOL		CTaskTray::bDestroyTaskTrayWindow(HWND hWnd) const
{
	NOTIFYICONDATA nIco{};
	nIco.cbSize = sizeof(NOTIFYICONDATA);
	nIco.hWnd = hWnd;
	nIco.uID = uTaskTrayID;
	nIco.guidItem = GUID_NULL;

	BOOL bRet = Shell_NotifyIcon(NIM_DELETE, &nIco);
	return bRet ? TRUE : FALSE;
}

//
// bGetTaskTrayWindowRect()
//
BOOL		CTaskTray::bGetTaskTrayWindowRect(HWND hWnd, LPRECT lpRect) const
{
	NOTIFYICONIDENTIFIER	nii{};
	nii.cbSize = sizeof(NOTIFYICONIDENTIFIER);
	nii.hWnd = hWnd;
	nii.uID = uTaskTrayID;
	nii.guidItem = GUID_NULL;

	HRESULT hResult = Shell_NotifyIconGetRect(&nii, lpRect);
	if (hResult == S_OK) return TRUE;
	if (hResult == HRESULT_FROM_WIN32(ERROR_TIMEOUT) || hResult == HRESULT_FROM_WIN32(ERROR_PROC_NOT_FOUND)) {
		return FALSE;
	}
	PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
	return FALSE;
}

//
// iCheckTaskTrayMessage()
//
int		CTaskTray::iCheckTaskTrayMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
		HANDLE_MSG(hWnd, WM_TASKTRAYEX, Cls_OnTaskTrayEx);

	default:
		if ((TaskTray->uTaskbarCreated != 0) && (message == TaskTray->uTaskbarCreated)) {
			if (TaskTray->bReCreateTaskTrayWindow(hWnd)) {
				return 0;
			}
			else return (-1);
		}
	}
	return 0;
}

//
// WM_COMMAND
// Cls_OnCommand()
//
void		CTaskTray::Cls_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify)
{
	UNREFERENCED_PARAMETER(hWndCtl);
	UNREFERENCED_PARAMETER(codeNotify);

	if (!Profile || !Cursor)	return;
	Cursor->vStopIMECursorChangeThread();
	switch (id) {
	case IDR_TT_MENU:
		break;
	case IDR_TT_SETTING:
		Profile->bGetProfileData();
		vSettingDialog(hWnd,SETTINGSEX_SELECTEDPANE_GENERAL);
		return;
	case IDR_TT_IMEMODE:
		Profile->bGetProfileData();
		vSettingDialog(hWnd,SETTINGSEX_SELECTEDPANE_IMEMODE);
		return;
	case IDR_TT_SYNTPHELPER:
		Profile->bGetProfileData();
		vSettingDialog(hWnd,SETTINGSEX_SELECTEDPANE_SYNTPHELPER);
		return;
	case IDM_ABOUT:
	case IDR_TT_ABOUT:
		vSettingDialog(hWnd,SETTINGSEX_SELECTEDPANE_ABOUT);
		return;
	case IDM_EXIT:
	case IDR_TT_QUIT:
		PostMessage(hWnd, WM_DESTROY, (WPARAM)0, (LPARAM)0);
		return;
	}
	if (!Cursor->bReloadCursor()) {
		vSettingDialogClose();
		bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)0, (LPARAM)0);
	}
}

//
// WM_TASKTRAYEX
// Cls_OnTaskTrayEx()
//
void		CTaskTray::Cls_OnTaskTrayEx(HWND hWnd, UINT id, UINT uMsg)
{
	if (id != NOTIFYICONDATA_ID) {
		return;
	}
	if (!Profile || !Cime || !Resource)	return;
	switch (uMsg) {
	case WM_LBUTTONDOWN:
		if (Profile->lpstAppRegData->bOffChangedFocus) {
			Cime->vIMEOpenCloseForced(hWnd, IMECLOSE);
		}
		[[fallthrough]];
	case WM_RBUTTONDOWN:
		POINT pt{};
		GetCursorPos(&pt);
		HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_TT_MENU));
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
		PostMessage(hWnd, WM_NULL, 0, 0);
		break;
	}
	return;
}

//
// bDisplayBalloon()
//
BOOL		CTaskTray::bDisplayBalloon(HWND hWnd, DWORD dwInfoFlags, LPCTSTR szInfoTitle, LPCTSTR szInfo) const
{
	BALLOON_PARAM *BalloonParam = new BALLOON_PARAM();
	if (!BalloonParam) return FALSE;
	ZeroMemory(BalloonParam, sizeof(BALLOON_PARAM));
	BalloonParam->hWnd = hWnd;
	BalloonParam->dwInfoFlags = dwInfoFlags;
	if (szInfoTitle) _tcsncpy_s(BalloonParam->szInfoTitle, ARRAYSIZE(BalloonParam->szInfoTitle), szInfoTitle, _TRUNCATE);
	if (szInfo) _tcsncpy_s(BalloonParam->szInfo, ARRAYSIZE(BalloonParam->szInfo), szInfo, _TRUNCATE);

	HANDLE hThread = CreateThread(NULL, 0, BalloonThreadProc, BalloonParam, 0, NULL);
	if (hThread) {
		CloseHandle(hThread);
		return TRUE;
	}
	NOTIFYICONDATA	nIco{};
	nIco.cbSize = sizeof(NOTIFYICONDATA);
	nIco.hWnd = hWnd;
	nIco.uID = uTaskTrayID;
	nIco.guidItem = GUID_NULL;
	nIco.dwState = NIS_HIDDEN | NIS_SHAREDICON;
	nIco.dwInfoFlags = dwInfoFlags;
	nIco.hIcon = NULL;
	nIco.uVersion = NOTIFYICON_VERSION_4;
	nIco.uFlags = NIF_INFO;
	if (szInfoTitle)	_tcsncpy_s(nIco.szInfoTitle, ARRAYSIZE(nIco.szInfoTitle), szInfoTitle, _TRUNCATE);
	if (szInfo)			_tcsncpy_s(nIco.szInfo, ARRAYSIZE(nIco.szInfo), szInfo, _TRUNCATE);
	BOOL bRet = Shell_NotifyIcon(NIM_MODIFY, &nIco);
	return bRet ? TRUE : FALSE;
}

//
// BalloonThreadProc()
//
static DWORD WINAPI	BalloonThreadProc(LPVOID lpParam)
{
	BALLOON_PARAM *BalloonParam = reinterpret_cast<BALLOON_PARAM*>(lpParam);
	if (!BalloonParam) return 1;

	NOTIFYICONDATA	nIco{};
	nIco.cbSize = sizeof(NOTIFYICONDATA);
	nIco.hWnd = BalloonParam->hWnd;
	nIco.uID = NOTIFYICONDATA_ID;
	nIco.guidItem = GUID_NULL;
	nIco.dwState = NIS_HIDDEN | NIS_SHAREDICON;
	nIco.dwInfoFlags = BalloonParam->dwInfoFlags;
	nIco.hIcon = NULL;
	nIco.uVersion = NOTIFYICON_VERSION_4;
	nIco.uFlags = NIF_INFO;
	_tcsncpy_s(nIco.szInfoTitle, ARRAYSIZE(nIco.szInfoTitle), BalloonParam->szInfoTitle, _TRUNCATE);
	_tcsncpy_s(nIco.szInfo, ARRAYSIZE(nIco.szInfo), BalloonParam->szInfo, _TRUNCATE);

	(void)Shell_NotifyIcon(NIM_MODIFY, &nIco);

	delete BalloonParam;
	return 0;
}

//
// bModifyToolHints()
//
BOOL		CTaskTray::bModifyToolHints(HWND hWnd, LPCTSTR lpszToolHints) const
{
	NOTIFYICONDATA	nIco{};
	nIco.cbSize = sizeof(NOTIFYICONDATA);
	nIco.hWnd = hWnd;
	nIco.uID = uTaskTrayID;
	nIco.guidItem = GUID_NULL;
	nIco.dwState = NIS_HIDDEN | NIS_SHAREDICON;
	nIco.hIcon = NULL;
	nIco.uVersion = NOTIFYICON_VERSION_4;
	nIco.uFlags = NIF_TIP;

	if (lpszToolHints)	_tcsncpy_s(nIco.szTip, ARRAYSIZE(nIco.szTip), lpszToolHints, _TRUNCATE);

	for (int i = 0; i < 10; i++) {
		if (Shell_NotifyIcon(NIM_MODIFY, &nIco)) return TRUE;
		Sleep(1000);
	}
	return FALSE;
}


/* = EOF = */
