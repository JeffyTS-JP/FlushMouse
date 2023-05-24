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
// Define
//
#define MOUSEHOOKMEM32 _T("MouseHookSharedMem32")
typedef struct tagMOUSE_SHAREDMEM32 {
	HWND    hWnd;
	HHOOK	hHook;
} MOUSE_SHAREDMEM32, * PMOUSE_SHAREDMEM32, * LPMOUSE_SHAREDMEM32;

//
// Global Prototype Define
//
DLLEXPORT BOOL __stdcall bMouseHookSet32(HWND hWnd);
DLLEXPORT BOOL __stdcall bMouseHookUnset32(void);

/* = EOF = */
