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
#include "FlushMouseDll.h"
#include "..\MiscLIB\CSharedMemory.h"

//
// Define
//
#define MOUSEHOOKMEM _T("MouseHookSharedMem")
typedef struct tagMOUSE_SHAREDMEM {
	HWND    hWnd;
	HHOOK	hHook;
} MOUSE_SHAREDMEM, * PMOUSE_SHAREDMEM, * LPMOUSE_SHAREDMEM;

//
// Global Prototype Define
//
DLLEXPORT BOOL __stdcall bMouseHookSet(HWND hWnd);
DLLEXPORT BOOL __stdcall bMouseHookUnset(void);

/* = EOF = */
