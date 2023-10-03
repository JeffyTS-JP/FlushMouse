// MouseHookDll.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/02/12  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "MouseHookDll.h"
#include "..\FlushMouseLIB\CommonDef.h"

//
// Define
//

//
// Local Prototype Define
//
static LRESULT  CALLBACK    lpMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);

//
// Local Data
//
#pragma comment( linker, "/SECTION:FLUSHMOUSEDLL_SEG,RWS" )
#pragma data_seg("FLUSHMOUSEDLL_SEG")
static HWND		hWndMSParent = NULL;
static LPMOUSE_SHAREDMEM  lpDatMouse = NULL;
static CSharedMemory* CSharedMem = NULL;
#pragma data_seg()

//
// bMouseHookSet()
//
DLLEXPORT BOOL  __stdcall bMouseHookSet(HWND hWnd)
{
	hWndMSParent = hWnd;
	if ((CSharedMem = new CSharedMemory(MOUSEHOOKMEM, sizeof(MOUSE_SHAREDMEM))) != NULL) {
		if ((lpDatMouse = (LPMOUSE_SHAREDMEM)CSharedMem->lpvSharedMemoryRead()) != NULL) {
			lpDatMouse->hWnd = hWnd;
			if (CSharedMem->bSharedMemoryWrite(lpDatMouse)) {
				HHOOK	hHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)lpMouseHookProc, hGetInstance(), 0);
				if (hHook) {
					lpDatMouse->hHook = hHook;
					if (CSharedMem->bSharedMemoryWrite(lpDatMouse)) {
						return TRUE;
					}
					UnhookWindowsHookEx(hHook);
				}
			}
		}
		delete CSharedMem;
		CSharedMem = NULL;
		hWndMSParent = NULL;
		lpDatMouse = NULL;
	}
	return FALSE;
}

//
// bMouseHookUnset()
//
DLLEXPORT BOOL __stdcall bMouseHookUnset()
{
	BOOL		bRet = FALSE;
	if (CSharedMem != NULL) {
		if ((lpDatMouse = (LPMOUSE_SHAREDMEM)CSharedMem->lpvSharedMemoryRead()) != NULL) {
			if (lpDatMouse->hHook) {
				if (UnhookWindowsHookEx(lpDatMouse->hHook)) {
					bRet = TRUE;
				}
			}
		}
		delete CSharedMem;
		CSharedMem = NULL;
		hWndMSParent = NULL;
		lpDatMouse = NULL;
	}
	return bRet;
}

//
// lpMouseHookProc()
//
template<typename T1, typename T2>
constexpr DWORD WORD2DWORD(T1 h, T2  l) { return (DWORD)(((((DWORD)(l)) << 16) & 0xffff0000) | ((DWORD)(h) & 0xffff)); }

static LRESULT CALLBACK lpMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	if (nCode == HC_ACTION) {
		switch (wParam) {
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_NCRBUTTONUP:
		{
			MOUSEHOOKSTRUCT* mh = (MOUSEHOOKSTRUCT*)lParam;
			DWORD pt = WORD2DWORD(mh->pt.x, mh->pt.y);
			PostMessage(hWndMSParent, (UINT)(WM_USER + wParam), (WPARAM)mh->hwnd, (LPARAM)pt);
		}
			break;
		default:
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


/* = EOF = */