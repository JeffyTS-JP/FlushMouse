//
// Dialogs.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/03  JeffyTS  	New edit.
// #0001	2023/11/27	JeffyTS		Divided from TaskTray.cpp
//

// Include
//
#pragma once
#include "pch.h"
#include "Dialogs.h"
#include "..\FlushMouseLIB\CommonDef.h"
#include "..\FlushMouseLIB\FlushMouseLIB.h"
#include "..\FlushMouseLIB\FlushMouseSub.h"

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

//
// Local Data
//

//
// Local Prototype Define
//

//
// vMessageBox()
//
void vMessageBox(HWND hWnd, UINT uID, UINT uType, LPCSTR lpFunc, DWORD dwLine)
{
	TCHAR	szRes[MAX_LOADSTRING]{};
	TCHAR	szFuncT[MAX_LOADSTRING]{};
	TCHAR	lpText[(MAX_LOADSTRING * 2)]{};

	int	nLoaded = 0;
	if (Resource && Resource->hLoad()) {
		nLoaded = LoadString(Resource->hLoad(), uID, szRes, MAX_LOADSTRING);
	}
	if (nLoaded == 0) {
		return;
	}

	if (lpFunc && (dwLine != 0)) {
#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, lpFunc, -1, szFuncT, MAX_LOADSTRING);
#else
		_strncpy_s(szFuncT, MAX_LOADSTRING, lpFunc, _TRUNCATE);
#endif
		_sntprintf_s(lpText, (MAX_LOADSTRING * 2), _TRUNCATE, _T("%s\r\n\r\n (%s : %d : 0x%08X)"), szRes, szFuncT, dwLine, GetLastError());
	}
	else {
		_tcsncpy_s(lpText, (MAX_LOADSTRING * 2), szRes, _TRUNCATE);
	}

	(void)MessageBox(hWnd, lpText, g_szWindowTitle, uType);
}

//
// vSettingDialog()
//
VOID vSettingDialog(HWND hWnd, INT32 iSelectedPane)
{
	UNREFERENCED_PARAMETER(hWnd);

	HWND	_hWnd = FindWindow(CLASS_FLUSHMOUSESETTINGS, NULL);
	if (_hWnd != NULL) {
		SetForegroundWindow(_hWnd);
		SendMessage(_hWnd, WM_SETTINGSEX, SETTINGSEX_SETTINGS_CHANGE_PANE, iSelectedPane);
	}
	else {
		if (Cursor) Cursor->vStopIMECursorChangeThread();
		TCHAR	CommandLine[16]{};
		errno_t err = _itow_s(iSelectedPane, CommandLine, ARRAYSIZE(CommandLine), 10);
		if (err != 0) return;
		if (!bCreateProcess(RELATIVE_FLUSHMOUSESETTINGS_EXE, CommandLine)) {
			return;
		}
	}
}


/* = EOF = */
