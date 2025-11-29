//
// MouseHookDll.h
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
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

//
// Define
//
#define MOUSEHOOKMEM32 _T("MouseHookSharedMem32")

//
// Global Prototype Define
//
DLLEXPORT BOOL __stdcall bMouseHookSet32(HWND hWnd);
DLLEXPORT BOOL __stdcall bMouseHookUnset32(void);

/* = EOF = */
