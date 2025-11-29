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
	TCHAR	_lpFunc[MAX_LOADSTRING]{};
	TCHAR	lpText[(MAX_LOADSTRING * 2)]{};

	try {
		throw LoadString(Resource->hLoad(), uID, lpText, MAX_LOADSTRING);
	}
	catch (int i) {
		if (i != 0) {
			if (lpFunc && (dwLine != 0)) {
				MultiByteToWideChar (CP_ACP, 0, lpFunc, -1, _lpFunc, MAX_LOADSTRING);
				_sntprintf_s(lpText, (MAX_LOADSTRING * 2), _TRUNCATE, L"%s\n\n (%s : %d : 0x%08X)", lpText, _lpFunc, dwLine, GetLastError());
			}
			try {
				throw MessageBox(hWnd, lpText, g_szWindowTitle, uType);
			}
			catch (int) {
				return;
			}
			catch (...) {
				return;
			}
		}
	}
	catch (...) {
		return;
	}
}

//
// vSettingDialog()
//
VOID		vSettingDialog(HWND hWnd, INT32 iSelectedPane)
{
	UNREFERENCED_PARAMETER(hWnd);

	HWND	_hWnd = FindWindow(CLASS_FLUSHMOUSESETTINGS, NULL);
	if (_hWnd != NULL) {
		SetForegroundWindow(_hWnd);
		SendMessage(_hWnd, WM_SETTINGSEX, SETTINGSEX_SETTINGS_CHANGE_PANE, iSelectedPane);
	}
	else {
		if (Cursor)	Cursor->vStopIMECursorChangeThread();
		TCHAR	CommandLine[2]{};
		if (_itow_s(iSelectedPane, CommandLine, (sizeof(CommandLine) - sizeof(TCHAR)), 10) != 0)	return;
		if (!bCreateProcess(RELATIVE_FLUSHMOUSESETTINGS_EXE, CommandLine)) {
			return;
		}
	}
}



/* = EOF = */
