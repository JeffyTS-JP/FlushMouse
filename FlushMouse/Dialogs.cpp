//
// Dialogs.cpp
//		Copyright (C) 2022 JeffyTS
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
#include "..\version.h"
#include "..\FlushMouseLIB\Resource.h"
#include "..\FlushMouseLIB\CommonDef.h"
#include "..\FlushMouseLIB\FlushMouseLIB.h"
#include "..\FlushMouseLIB\TaskTray.h"

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
void vMessageBox(HWND hWnd, UINT uID, UINT uType)
{
	TCHAR	lpText[MAX_LOADSTRING];
	try {
		throw LoadString(Resource->hLoad(), uID, lpText, MAX_LOADSTRING);
	}
	catch (int i) {
		if (i != 0) {
			try {
				throw MessageBox(hWnd, lpText, szTitle, uType);
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
// vAboutDialog()
//
VOID		vAboutDialog(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);

	HWND	_hWnd = FindWindow(CLASS_FLUSHMOUSESETTINGS, NULL);
	if (_hWnd != NULL) {
		SetForegroundWindow(_hWnd);
		SendMessage(_hWnd, WM_SETTINGSEX, SETTINGSEX_CHANGE_PANE, SETTINGSEX_SELECTEDPANE_ABOUT);
	}
	else {
		TCHAR	CommandLine[1]{};
		if (_itow_s(SETTINGSEX_SELECTEDPANE_ABOUT, CommandLine, sizeof(CommandLine), 10) != 0)	return;

		if (!bCreateProcess(RELATIVE_FLUSHMOUSESETTINGS_EXE, CommandLine)) {
			return;
		}
	}
}

//
// vSettingDialog()
//
VOID		vSettingDialog(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);

	HWND	_hWnd = FindWindow(CLASS_FLUSHMOUSESETTINGS, NULL);
	if (_hWnd != NULL) {
		SetForegroundWindow(_hWnd);
		SendMessage(_hWnd, WM_SETTINGSEX, SETTINGSEX_CHANGE_PANE, SETTINGSEX_SELECTEDPANE_GENERAL);
	}
	else {
		TCHAR	CommandLine[1]{};
		if (_itow_s(SETTINGSEX_SELECTEDPANE_GENERAL, CommandLine, sizeof(CommandLine), 10) != 0)	return;

		if (!bCreateProcess(RELATIVE_FLUSHMOUSESETTINGS_EXE, CommandLine)) {
			return;
		}
	}
}



/* = EOF = */
