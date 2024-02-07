//
// GlobalHookDll.cpp
//		Copyright (C) 2022 JeffyTS
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

//
// Local Prototype Define
//
static LRESULT CALLBACK lpGlobalHookProc(int, WPARAM, LPARAM);

//
// Local Data
//
#pragma comment(linker, "/SECTION:FLUSHMOUSEDLL_SEG,RWS")
#pragma data_seg("FLUSHMOUSEDLL_SEG")
static HINSTANCE	hGLInstance = NULL;
static HWND			hWndGLParent = NULL;
static HHOOK		hHookGL = NULL;
static LPGLOBAL_SHAREDMEM  lpDatGlobal = NULL;
static HWND			hPrevWnd = NULL;
static BOOL			bSubclassed = FALSE;
static CSharedMemory* CSharedMem = NULL;
#pragma data_seg()

//
// bGlobalHookSet()
//
DLLEXPORT BOOL  __stdcall bGlobalHookSet(HWND hWnd)
{
	hWndGLParent = hWnd;
	if ((CSharedMem = new CSharedMemory(GLOBALHOOKMEM, sizeof(GLOBAL_SHAREDMEM))) != NULL) {
		if ((lpDatGlobal = (LPGLOBAL_SHAREDMEM)CSharedMem->lpvSharedMemoryRead()) != NULL) {
			lpDatGlobal->hWnd = hWnd;	lpDatGlobal->hInstance = hGetInstance();
			if (CSharedMem->bSharedMemoryWrite(lpDatGlobal)) {
				HHOOK	hHook = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)lpGlobalHookProc, hGetInstance(), 0);
				if (hHook) {
					lpDatGlobal->hHook = hHook;	hHookGL = hHook;
					if (CSharedMem->bSharedMemoryWrite(lpDatGlobal)) {
						SendMessage(hWnd, WM_HOOKEX, 0, 1);
						return TRUE;
					}
					UnhookWindowsHookEx(hHook);
				}
			}
		}
		delete CSharedMem;
		CSharedMem = NULL;
		hGLInstance = NULL;
		hWndGLParent = NULL;
		hHookGL = NULL;
		lpDatGlobal = NULL;
		hPrevWnd = NULL;
		bSubclassed = FALSE;
	}
	return FALSE;
}

//
// bGlobalHookUnset()
//
DLLEXPORT BOOL __stdcall bGlobalHookUnset()
{
	BOOL		bRet = FALSE;
	if (CSharedMem != NULL) {
		if ((lpDatGlobal = (LPGLOBAL_SHAREDMEM)CSharedMem->lpvSharedMemoryRead()) != NULL) {
			if (lpDatGlobal->hWnd) {
				hHookGL = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)lpGlobalHookProc, hGLInstance, GetWindowThreadProcessId(lpDatGlobal->hWnd, NULL));
				if (hHookGL) {
					SendMessage(lpDatGlobal->hWnd, WM_HOOKEX, 0, 0);
					if (lpDatGlobal->hHook) {
						if ((bSubclassed == FALSE))	bRet = TRUE;
					}
				}
			}
		}
		delete CSharedMem;
		CSharedMem = NULL;
		hGLInstance = NULL;
		hWndGLParent = NULL;
		hHookGL = NULL;
		lpDatGlobal = NULL;
		hPrevWnd = NULL;
		bSubclassed = FALSE;
	}
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
		case WM_IME_STARTCOMPOSITION:		// 0x010D
			if (lpCW->lResult != 0) {
				SendMessage(hWndGLParent, WM_CHECKIMESTARTCONVEX, (WPARAM)TRUE, (LPARAM)0);
			}
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		case WM_IME_ENDCOMPOSITION:			// 0x010E
			if (lpCW->lResult != 0) {
				SendMessage(hWndGLParent, WM_CHECKIMESTARTCONVEX, (WPARAM)FALSE, (LPARAM)0);
			}
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		default:
			if (lpCW->message == WM_HOOKEX) {
				if (lpCW->lParam) {
					if (!bSubclassed) {
						if (hHookGL == NULL)	break;
						if (UnhookWindowsHookEx(hHookGL) != FALSE) {
							if (LoadLibraryEx(FLUSHMOUSE_DLL, NULL, 0)) {
								bSubclassed = TRUE;
							}
							hHookGL = NULL;
						}
					}
				}
				else {
					if (bSubclassed) {
						if (hHookGL == NULL)	break;
						bSubclassed = FALSE;
					}
				}
			}
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


/* = EOF = */