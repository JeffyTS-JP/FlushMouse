// GlobalHookDll.h
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
#include "FlushMouseDll.h"
#include "..\MiscLIB\CSharedMemory.h"

//
// Message Handler Define
// 

//
// Define
//
#define GLOBALHOOKMEM _T("GlobalHookSharedMem")
typedef struct tagGLOBAL_SHAREDMEM {
    HINSTANCE	hInstance;
    HWND	hWnd;
    HHOOK	hHook;
} GLOBAL_SHAREDMEM, * PGLOBAL_SHAREDMEM, * LPGLOBAL_SHAREDMEM;

//
// Global Prototype Define
//
DLLEXPORT BOOL __stdcall bGlobalHookSet(HWND);
DLLEXPORT BOOL __stdcall bGlobalHookUnset(void);

/* = EOF = */
