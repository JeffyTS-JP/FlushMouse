//
// GlobalHookDll.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/04  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "GlobalHookDll.h"
#include "..\FlushMouseLIB\CommonDef.h"

//
// Define
//
#define	INJECT_HOOK		((LPARAM)1)
#define	REMOVE_HOOK		((LPARAM)0)

//
// Local Prototype Define
//
static LRESULT CALLBACK lpGlobalHookProc(int, WPARAM, LPARAM);

//
// Local Data
//
#pragma comment(linker, "/SECTION:FLUSHMOUSEDLL_SEG,RWS")
#pragma data_seg("FLUSHMOUSEDLL_SEG")
static UINT					WM_HOOKEX = 0;
static HINSTANCE			hGLInstance = NULL;
static HWND					hWndGLParent = NULL;
static HHOOK				hHookGL = NULL;
static HWND					hPrevWnd = NULL;
static BOOL					bSubclassed = FALSE;
static HMODULE				hModule = NULL;
#pragma data_seg()

//
// bGlobalHookSet()
//
DLLEXPORT BOOL  __stdcall bGlobalHookSet(HWND hWnd)
{
	hWndGLParent = hWnd;
	if ((hGLInstance = hGetInstance()) == NULL)	return FALSE;
	if (WM_HOOKEX == 0)	WM_HOOKEX = RegisterWindowMessage(L"FlushMouseDLL_GlobalHook");
	if (WM_HOOKEX == 0) return FALSE;
	HHOOK	hHook = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)lpGlobalHookProc, hGLInstance, 0);
	if (hHook) {
		hHookGL = hHook;
		if (hWnd) {
			SendMessage(hWnd, WM_HOOKEX, 0, INJECT_HOOK);
			return TRUE;
		}
		UnhookWindowsHookEx(hHook);
	}
	hGLInstance = NULL;
	hWndGLParent = NULL;
	hHookGL = NULL;
	hPrevWnd = NULL;
	bSubclassed = FALSE;
	return FALSE;
}

//
// bGlobalHookUnset()
//
DLLEXPORT BOOL __stdcall bGlobalHookUnset()
{
	BOOL		bRet = FALSE;
	hHookGL = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)lpGlobalHookProc, hGLInstance, GetWindowThreadProcessId(hWndGLParent, NULL));
	if (hHookGL) {
		SendMessage(hWndGLParent, WM_HOOKEX, 0, REMOVE_HOOK);
		if ((bSubclassed == FALSE))	bRet = TRUE;
	}
	hGLInstance = NULL;
	hWndGLParent = NULL;
	hHookGL = NULL;
	hPrevWnd = NULL;
	bSubclassed = FALSE;
	return bRet;
}

//
//フックの処理
//
template<typename T1, typename T2>
constexpr DWORD WORD2DWORD(T1 h, T2  l) { return (DWORD)(((((DWORD)(l)) << 16) & 0xffff0000) | ((DWORD)(h) & 0xffff)); }

static LRESULT CALLBACK lpGlobalHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	LPCWPRETSTRUCT	lpCW = (LPCWPRETSTRUCT)lParam;
	if (nCode == HC_ACTION) {
		switch (lpCW->message) {
		case WM_IME_STARTCOMPOSITION:
			if (lpCW->lResult != 0) {
				SendMessage(hWndGLParent, WM_CHECKIMESTARTCONVEX, (WPARAM)TRUE, (LPARAM)0);
			}
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		case WM_IME_ENDCOMPOSITION:
			if (lpCW->lResult != 0) {
				SendMessage(hWndGLParent, WM_CHECKIMESTARTCONVEX, (WPARAM)FALSE, (LPARAM)0);
			}
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		default:
			if (lpCW->message == WM_HOOKEX) {
				if (lpCW->lParam == INJECT_HOOK) {
					if (!bSubclassed) {
						if (hHookGL == NULL)	break;
						if (UnhookWindowsHookEx(hHookGL) != FALSE) {
							if ((hModule = LoadLibraryEx(FLUSHMOUSE_DLL, NULL, 0))) {
								bSubclassed = TRUE;
							}
							hHookGL = NULL;
						}
					}
				}
				else {
					if (bSubclassed) {
						if (hHookGL == NULL)	break;
						if (UnhookWindowsHookEx(hHookGL) != FALSE) {
							if (FreeLibrary(hGLInstance)) {
								bSubclassed = FALSE;
							}
						}
					}
				}
			}
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


/* = EOF = */
