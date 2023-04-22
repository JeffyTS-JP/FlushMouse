// MouseHookDll.h
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
#include "FlushMouseDll32.h"
#include "..\MiscLIB\CSharedMemory.h"

//
// Message Handler Define
//
// 
// void Cls_OnLButtonDownEx(HWND hWnd, int x, int y, HWND hForeground)
#define HANDLE_WM_LBUTTONDOWNEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)
// void Cls_OnRButtonDownEx(HWND hWnd, int x, int y, HWND hForeground)
#define HANDLE_WM_RBUTTONDOWNEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)
//* void Cls_OnLButtonUpEx(HWND hWnd, int x, int y, HWND hForeground)
#define HANDLE_WM_LBUTTONUPEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)
//* void Cls_OnRButtonUpEx(HWND hWnd, int x, int y, HWND hForeground)
#define HANDLE_WM_RBUTTONUPEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)
// void Cls_OnLButtonDblClkEx(HWND hWnd, int x, int y, HWND hForeground)
#define HANDLE_WM_LBUTTONDBLCLKEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)
// void Cls_OnRButtonDblClkEx(HWND hWnd, int x, int y, HWND hForeground)
#define HANDLE_WM_RBUTTONDBLCLKEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)

//
// Define
//
#define WM_LBUTTONDOWNEX	(WM_USER + WM_LBUTTONDOWN)
#define WM_RBUTTONDOWNEX	(WM_USER + WM_RBUTTONDOWN)
#define WM_LBUTTONUPEX      (WM_USER + WM_LBUTTONUP)
#define WM_RBUTTONUPEX      (WM_USER + WM_RBUTTONUP)
#define WM_LBUTTONDBLCLKEX  (WM_USER + WM_LBUTTONDBLCLK)
#define WM_RBUTTONDBLCLKEX  (WM_USER + WM_RBUTTONDBLCLK)

#define MOUSEHOOKMEM32 _T("MouseHookSharedMem32")
typedef struct tagMOUSE_SHAREDMEM {     // 共有メモリ用構造体
	HWND    hWnd;                       // 親ウィンドウハンドル
	HHOOK	hHook;                      // Mouseフックハンドル
	HWND    hForeground;
} MOUSE_SHAREDMEM32, * PMOUSE_SHAREDMEM32, * LPMOUSE_SHAREDMEM32;

//
// Global Prototype Define
//
DLLEXPORT BOOL __stdcall bMouseHookSet32(HWND hWnd);
DLLEXPORT BOOL __stdcall bMouseHookUnset32(void);

/* EOF */