//
// KeyboardHookDll.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/02/12  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "FlushMouseDll.h"
#include "..\MiscLIB\CSharedMemory.h"

//
// Define
//
#define KEYBOARDHOOKLLMEM _T("KeyboardHookLLSharedMem")
typedef struct tagKEYBOARDLL_SHAREDMEM {
	HWND	hWnd;
	HHOOK	hHook;
	BOOL	bEnableEPHelper;
} KEYBOARDLL_SHAREDMEM, * PKEYBOARDLL_SHAREDMEM, * LPKEYBOARDLL_SHAREDMEM;

//
// Global Prototype Define
//
DLLEXPORT BOOL __stdcall bKeyboardHookLLSet(HWND);
DLLEXPORT BOOL __stdcall bKeyboardHookLLUnset(void);
DLLEXPORT BOOL __stdcall bSetEnableEPHelperLL64(BOOL bEPHelper);;


/* = EOF = */