//
// SynTPDlg.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/03/21  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "Resource.h"
#include "FlushMouseLIB.h"
#include "TaskTray.h"
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#include "..\MiscLIB\CRegistry.h"

//
// Define
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
// vSynTPHelperDialog()
//
VOID		vSynTPHelperDialog(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);

	HWND	_hWnd = FindWindow(CLASS_FLUSHMOUSESETTINGS, NULL);
	if (_hWnd != NULL) {
		SetForegroundWindow(_hWnd);
		SendMessage(_hWnd, WM_SETTINGSEX, SETTINGSEX_SETTINGS_CHANGE_PANE, SETTINGSEX_SELECTEDPANE_SYNTPHELPER);
	}
	else {
		if (Cursor)	Cursor->vStopIMECursorChangeThread();
		TCHAR	CommandLine[2]{};
		if (_itow_s(SETTINGSEX_SELECTEDPANE_SYNTPHELPER, CommandLine, (sizeof(CommandLine) - sizeof(TCHAR)), 10) != 0)	return;

		if (!bCreateProcess(RELATIVE_FLUSHMOUSESETTINGS_EXE, CommandLine)) {
			return;
		}
	}
}


/* = EOF = */