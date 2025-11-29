//
// ShellHookDll.h
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2023/07/14  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "FlushMouseDll.h"

//
// Define
//

//
// Global Prototype Define
//
DLLEXPORT BOOL __stdcall bShellHookSet(HWND);
DLLEXPORT BOOL __stdcall bShellHookUnset(void);


/* = EOF = */
