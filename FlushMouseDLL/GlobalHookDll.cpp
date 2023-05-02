// GlobalHookDll.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/03/04  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "GlobalHookDll.h"
#include "..\FlushMouse\CommonDef.h"

//
// Define
//

//
// Local Prototype Define
//
static LRESULT CALLBACK lpGlobalHookProc(int, WPARAM, LPARAM);
static BOOL	bGlobalHookProcSub()
;

//
// Local Data
//
static HWND		hWndGLParent = NULL;
static LPGLOBAL_SHAREDMEM  lpDatGlobal = NULL;

//
// bGlobalHookSet()
//
DLLEXPORT BOOL  __stdcall bGlobalHookSet(HWND hWnd)
{
	HHOOK   hHook = NULL;
	hHook = SetWindowsHookEx(WH_CALLWNDPROCRET,
							(HOOKPROC)lpGlobalHookProc,
							hGetInstance(), 0);
	if (hHook) {
		GLOBAL_SHAREDMEM   datGlobal{};
		if (bSharedMemoryCreate(hWnd, GLOBALHOOKMEM, sizeof(GLOBAL_SHAREDMEM))) {
			datGlobal.hWnd = hWnd;    datGlobal.hHook = hHook;
			if (bSharedMemoryWrite(GLOBALHOOKMEM, (LPBYTE)&datGlobal, sizeof(GLOBAL_SHAREDMEM))) {
				return TRUE;
			}
			bSharedMemoryDelete(GLOBALHOOKMEM);
		}
		UnhookWindowsHookEx(hHook);
	}
	return FALSE;
}

//
// bGlobalHookUnset()
//
DLLEXPORT BOOL __stdcall bGlobalHookUnset()
{
	GLOBAL_SHAREDMEM   datGlobal{};
	if (bSharedMemoryRead(GLOBALHOOKMEM, (LPBYTE)&datGlobal, sizeof(GLOBAL_SHAREDMEM))) {
		if (datGlobal.hHook) {
			if (UnhookWindowsHookEx(datGlobal.hHook)) {
				bSharedMemoryDelete(GLOBALHOOKMEM);
				return TRUE;
			}
		}
	}
	bSharedMemoryDelete(GLOBALHOOKMEM);
	return FALSE;
}

//
//フックの処理
//
static LRESULT CALLBACK lpGlobalHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	LPCWPSTRUCT	lpCW = (LPCWPSTRUCT)lParam;
	if (nCode == HC_ACTION) {
		switch (lpCW->message) {
		case WM_IME_STARTCOMPOSITION:		// 0x010D
			if (bGlobalHookProcSub()) {
				PostMessage(hWndGLParent, WM_CHECKIMESTARTCONVEX, (WPARAM)TRUE, (LPARAM)0);
			}
			break;
		case WM_IME_ENDCOMPOSITION:			// 0x010E
			if (bGlobalHookProcSub()) {
				PostMessage(hWndGLParent, WM_CHECKIMESTARTCONVEX, (WPARAM)FALSE, (LPARAM)0);
			}
			break;
		default:
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//
// bGlobalHookProcSub
//
static BOOL	bGlobalHookProcSub()
{
	if ((hWndGLParent == NULL) || (lpDatGlobal == NULL)) {
		if (lpDatGlobal == NULL) {
			if ((lpDatGlobal = (LPGLOBAL_SHAREDMEM)lpvSharedMemoryOpen(GLOBALHOOKMEM, sizeof(GLOBAL_SHAREDMEM))) == NULL) {
				return	FALSE;
			}
		}
		if (lpDatGlobal->hWnd == NULL) {
			return	FALSE;
		}
		hWndGLParent = lpDatGlobal->hWnd;
	}
	return TRUE;
}

/* = EOF = */