//
// GlobalHookDll.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/04  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "FlushMouseDll.h"

//
// Message Handler Define
// 

//
// Define
//

//
// Global Prototype Define
//
DLLEXPORT BOOL __stdcall bGlobalHookSet(HWND);
DLLEXPORT BOOL __stdcall bGlobalHookUnset(void);

/* = EOF = */
