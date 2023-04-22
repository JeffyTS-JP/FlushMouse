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
#include "MouseHookDll32.h"

//
// Define
//

//
// Local Prototype Define
//
static LRESULT  CALLBACK    lpMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
static BOOL	bMouseHookProcSub();

//
// Local Data
//
static HWND		hWndMSParent = NULL;
static LPMOUSE_SHAREDMEM32  lpDatMouse = NULL;

//
// bMouseHookSet()
//
DLLEXPORT BOOL  __stdcall bMouseHookSet32(HWND hWnd)
{
	HHOOK   hHook = NULL;
	hHook = SetWindowsHookEx(WH_MOUSE,							// 種類はマウスフック
							(HOOKPROC)lpMouseHookProc,			// マウスプロシージャをセット
							hGetInstance(), 0);					// このDLLのインスタンスハンドルを指定
	if (hHook) {
		MOUSE_SHAREDMEM32   datMouse{};
		if (bSharedMemoryCreate(hWnd, MOUSEHOOKMEM32, sizeof(MOUSE_SHAREDMEM32))) {
			datMouse.hWnd = hWnd;    datMouse.hHook = hHook;		hWndMSParent = hWnd;
			if (bSharedMemoryWrite(MOUSEHOOKMEM32, (LPBYTE)&datMouse, sizeof(MOUSE_SHAREDMEM32))) {
				return TRUE;
			}
			bSharedMemoryDelete(MOUSEHOOKMEM32);
		}
		UnhookWindowsHookEx(hHook);
	}
	return FALSE;
}

//
// bMouseHookUnset()
//
DLLEXPORT BOOL __stdcall bMouseHookUnset32()
{
	MOUSE_SHAREDMEM32   datMouse{};
	if (bSharedMemoryRead(MOUSEHOOKMEM32, (LPBYTE)&datMouse, sizeof(MOUSE_SHAREDMEM32))) {
		if (datMouse.hHook) {
			if (UnhookWindowsHookEx(datMouse.hHook)) {
			// 共有メモリを削除
				bSharedMemoryDelete(MOUSEHOOKMEM32);
				return TRUE;
			}
		}
	}
	bSharedMemoryDelete(MOUSEHOOKMEM32);
	return FALSE;
}

//
// lpMouseHookProc()
//
template<typename T1, typename T2>
constexpr DWORD WORD2DWORD(T1 h, T2  l) { return (DWORD)(((((DWORD)(l)) << 16) & 0xffff0000) | ((DWORD)(h) & 0xffff)); }

static LRESULT CALLBACK lpMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {                            // nCodeが0より小さい場合は関係ないので次に渡す
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	if (nCode == HC_ACTION) {
		if (lParam) {
			switch (wParam) {
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
				case WM_NCLBUTTONDOWN:
				case WM_NCRBUTTONDOWN:
				case WM_NCLBUTTONUP:
				case WM_NCRBUTTONUP:
					if (bMouseHookProcSub()) {
						UINT  uCode = (UINT)(WM_USER + wParam);
						MOUSEHOOKSTRUCT* mh = (MOUSEHOOKSTRUCT*)lParam;
						DWORD pt = WORD2DWORD(mh->pt.x, mh->pt.y);
						PostMessage(hWndMSParent, uCode, (WPARAM)mh->hwnd, (LPARAM)pt);
					}
					break;
				default:
					break;
			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//
// bMouseHookProcSub
//
static BOOL	bMouseHookProcSub()
{
	if ((hWndMSParent == NULL) || (lpDatMouse == NULL)) {
		if (lpDatMouse == NULL) {
			if ((lpDatMouse = (LPMOUSE_SHAREDMEM32)lpvSharedMemoryOpen(MOUSEHOOKMEM32, sizeof(MOUSE_SHAREDMEM32))) == NULL) {
				return	FALSE;
			}
		}
		if (lpDatMouse->hWnd == NULL) {
			return	FALSE;
		}
		hWndMSParent = lpDatMouse->hWnd;
	}
	return TRUE;
}

/* EOF */