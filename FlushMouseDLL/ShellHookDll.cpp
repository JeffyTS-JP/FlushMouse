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

	HINSTANCE hInst = hGetInstance();
	if (hInst == NULL) {
		hWndShellParent = NULL;
		return FALSE;
	}

	CHANGEFILTERSTRUCT cf{};
	cf.cbSize = sizeof(CHANGEFILTERSTRUCT);
	(void)ChangeWindowMessageFilterEx(hWnd, WM_INPUTLANGCHANGEEX, MSGFLT_ALLOW, &cf);

	hHookShell = SetWindowsHookEx(WH_SHELL, (HOOKPROC)lpShellHookProc, hInst, 0);
	if (hHookShell == NULL) {
		hWndShellParent = NULL;
		return FALSE;
	}

	return TRUE;
}

//
// bShellHookUnset()
//
DLLEXPORT BOOL __stdcall bShellHookUnset()
{
	BOOL bRet = FALSE;
	if (hHookShell != NULL) {
		if (UnhookWindowsHookEx(hHookShell) != FALSE) {
			bRet = TRUE;
		}
		hHookShell = NULL;
	}

	hWndShellParent = NULL;
	return bRet;
}

//
// lpShellHookProc()
//
static LRESULT CALLBACK lpShellHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {
		return CallNextHookEx(hHookShell, nCode, wParam, lParam);
	}
	switch (nCode) {
		case HSHELL_LANGUAGE:
			if (hWndShellParent != NULL) {
				PostMessage(hWndShellParent, WM_INPUTLANGCHANGEEX, wParam, lParam);
			}
			break;
	}

	return CallNextHookEx(hHookShell, nCode, wParam, lParam);
}


/* = EOF = */
