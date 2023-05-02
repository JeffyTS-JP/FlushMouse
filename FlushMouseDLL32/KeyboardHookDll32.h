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
// Define
//
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
DLLEXPORT BOOL __stdcall bSetEnableEPHelperLL32(BOOL bEPHelper);;


/* = EOF = */