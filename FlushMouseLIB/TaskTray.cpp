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
HWND			hAboutDlg = NULL;
HWND			hSettingDlg = NULL;
HWND			hSynTPHelperDlg = NULL;

//
// Local Data
//

//
// Local Prototype Define
//

//
// iCheckTaskTrayMessage()
//

//
// Class CTaskTray
// 
CTaskTray::CTaskTray(HWND hWnd)
	: uTaskTrayID(NOTIFYICONDATA_ID), uTaskbarCreatedMessage(0), uTaskbarCreated(0)
{
	if ((uTaskbarCreatedMessage = RegisterWindowMessage(_T("FlushMouseTaskTray-{CA959312-1F82-45E8-AC7B-6F1F6CDD19C4}"))) == 0) {
		uTaskbarCreatedMessage = 0;
		return;
	}
	if ((uTaskbarCreated = RegisterWindowMessage(_T("TaskbarCreated"))) == 0) {
		uTaskbarCreated = 0;
		return;
	}
	CHANGEFILTERSTRUCT	cf{};
	cf.cbSize = sizeof(CHANGEFILTERSTRUCT);
	if (!ChangeWindowMessageFilterEx(hWnd, uTaskbarCreatedMessage, MSGFLT_ALLOW, &cf)) {
		uTaskbarCreatedMessage = 0;
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
	if ((uTaskTrayID == 0) || (uTaskbarCreatedMessage == 0))	return FALSE;

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
	try {
		throw Shell_NotifyIcon(NIM_ADD, &nIco);
	}
	catch (BOOL bRet) {
		if (!bRet) {
			_Post_equals_last_error_ DWORD err = GetLastError();
			if (err == ERROR_TIMEOUT) {
				Sleep(2000);
				try {
					throw Shell_NotifyIcon(NIM_MODIFY, &nIco);
				}
				catch (BOOL bRet) {
					if (!bRet) {
						_Post_equals_last_error_ err = GetLastError();
						if (err == ERROR_TIMEOUT) {
							Sleep(2000);
							try {
								throw Shell_NotifyIcon(NIM_ADD, &nIco);
							}
							catch (BOOL bRet) {
								if (!bRet) {
									bReportEvent(MSG_TASKTRAY_REGISTER_FAILD, APPLICATION_CATEGORY);
									return FALSE;
								}
							}
							catch (...) {
								return FALSE;
							}
						}
						else {
							bReportEvent(MSG_TASKTRAY_REGISTER_FAILD, APPLICATION_CATEGORY);
							return FALSE;
						}
					}
				}
				catch (...) {
					return FALSE;
				}
			}
			else {
				bReportEvent(MSG_TASKTRAY_REGISTER_FAILD, APPLICATION_CATEGORY);
				return FALSE;
			}
		}
	}
	catch (...) {
		return FALSE;
	}
	return TRUE;
}

//
// bReCreateTaskTrayWindow()
//
BOOL		CTaskTray::bReCreateTaskTrayWindow(HWND hWnd) const
{
	if (bDestroyTaskTrayWindow(hWnd)) {
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
			if (!bDestroyTaskTrayWindow(hWnd)) {
			}
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
	try {
		throw Shell_NotifyIcon(NIM_DELETE, &nIco);
	}
	catch (BOOL bRet) {
		if (bRet) {
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

//
// iCheckTaskTrayMessage()
//
int		CTaskTray::iCheckTaskTrayMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		HANDLE_MSG(hWnd, WM_COMMAND, Cls_OnCommand);
		HANDLE_MSG(hWnd, WM_TASKTRAYEX, Cls_OnTaskTrayEx);

	default:
		if (((TaskTray->uTaskbarCreatedMessage != 0) && (message == TaskTray->uTaskbarCreatedMessage)) 
				|| ((TaskTray->uTaskbarCreated != 0) && (message == TaskTray->uTaskbarCreated))) {
			if (TaskTray->bReCreateTaskTrayWindow(hWnd)) {
				bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
				PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
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
BOOL		CTaskTray::bDisplayBalloon(HWND hWnd, DWORD dwInfoFlags, LPCTSTR szInfoTitle, LPCTSTR szInfo)
{
	std::future<BOOL> _future;
	std::promise<BOOL> _promise;
	_future = _promise.get_future();
	try {
		std::thread([&](std::promise<BOOL> _promise) { 
			(void)_promise;
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

			try {
				throw Shell_NotifyIcon(NIM_MODIFY, &nIco);
			}
			catch (BOOL bRet) {
				if (bRet) {
					return TRUE;
				}
				else {
				}
			}
			catch (...) {
			}
			return TRUE;
			}, std::move(_promise)).detach();
#define TIMEOUT		100
		if(std::future_status::ready == _future.wait_until(std::chrono::system_clock::now() + std::chrono::milliseconds(TIMEOUT))) {
			return TRUE;
		}
#undef TIMEOUT
	}
	catch (...) {
	}
	return FALSE;
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
		try {
			throw Shell_NotifyIcon(NIM_MODIFY, &nIco);
		}
		catch (BOOL bRet) {
			if (bRet) {
				return TRUE;
			}
			else {
			}
		}
		catch (...) {
		}
		Sleep(1000);
	}
	return FALSE;
}


/* = EOF = */
