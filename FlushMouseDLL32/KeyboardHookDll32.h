// KeyboardHookDll32.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/04/04  JeffyTS  	New edit.
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
// void Cls_OnSysKeyDownEx(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
#define HANDLE_WM_SYSKEYDOWNUPEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), (UINT)(wParam), (!(BOOL)(lParam & 0x80000000)), (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L)

//
// Define
//
#define VK_OEM_IME_OFF	0xf3
#define VK_OEM_IME_ON	0xf4
#define VK_ONLY_CTRLUP  0xe8
#define WM_SYSKEYDOWNUPEX		(WM_USER + WM_SYSKEYDOWN)
#define WM_SYSINUPUTLANGCHANGE	(WM_USER + WM_INUPUTLANGCHANGE)

#define KEYBOARDHOOKLLMEM32 _T("KeyboardHookLLSharedMem32")
typedef struct tagKEYBOARDLL_SHAREDMEM32 {
    HWND    hWnd; 
    HHOOK   hHook;
    BOOL	bEnableEPHelper;
} KEYBOARDLL_SHAREDMEM32, * PKEYBOARDLL_SHAREDMEM32, * LPKEYBOARDLL_SHAREDMEM32;

//
// Global Prototype Define
//
DLLEXPORT BOOL __stdcall bKeyboardHookLLSet32(HWND);
DLLEXPORT BOOL __stdcall bKeyboardHookLLUnset32(void);
DLLEXPORT BOOL __stdcall bSetEnableEPHelperLL(BOOL bEPHelper);;


