//
// ShellHookDll.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/07/14  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "ShellHookDll.h"
#include "..\FlushMouseLIB\CommonDef.h"

//
// Define
//

//
// Local Prototype Define
//
static LRESULT CALLBACK lpShellHookProc(int, WPARAM, LPARAM);

//
// Local Data
//
#pragma comment(linker, "/SECTION:FLUSHMOUSEDLL_SEG,RWS")
#pragma data_seg("FLUSHMOUSEDLL_SEG")
static HWND				hWndShellParent = NULL;
static HHOOK			hHookShell = NULL;

#pragma data_seg()

//
// bShellHookSet()
//
DLLEXPORT BOOL  __stdcall bShellHookSet(HWND hWnd)
{
	hWndShellParent = hWnd;
	CHANGEFILTERSTRUCT	cf{};	cf.cbSize = sizeof(CHANGEFILTERSTRUCT);
	if (ChangeWindowMessageFilterEx(hWnd, WM_INPUTLANGCHANGEEX, MSGFLT_ALLOW, &cf)) {
		MINIMIZEDMETRICS	mm{};	mm.cbSize = sizeof(MINIMIZEDMETRICS);	mm.iArrange = ARW_HIDE;
		if (SystemParametersInfo(SPI_SETMINIMIZEDMETRICS, sizeof(MINIMIZEDMETRICS), (PVOID)&mm, SPIF_SENDCHANGE)) {
			hHookShell = SetWindowsHookEx(WH_SHELL, (HOOKPROC)lpShellHookProc, hGetInstance(), 0);
			if (hHookShell) {
				return TRUE;
			}
			hWndShellParent = NULL;
			hHookShell = NULL;
		}
	}
	return FALSE;
}

//
// bShellHookUnset()
//
DLLEXPORT BOOL __stdcall bShellHookUnset()
{
	BOOL	bRet = FALSE;
	if (hHookShell) {
		if (UnhookWindowsHookEx(hHookShell)) {
			bRet = TRUE;
		}
	}
	hWndShellParent = NULL;
	hHookShell = NULL;
	return bRet;
}

//
// lpShellHookProc()
//		フックの処理
//
static LRESULT CALLBACK lpShellHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	switch (nCode) {
		case HSHELL_LANGUAGE:
			PostMessage(hWndShellParent, WM_INPUTLANGCHANGEEX, wParam, lParam);
			break;
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


/* = EOF = */
