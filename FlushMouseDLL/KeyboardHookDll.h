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

//
// Global Prototype Define
//
DLLEXPORT BOOL __stdcall bKeyboardHookLLSet(HWND hWnd);
DLLEXPORT VOID __stdcall vKeyboardHookLLUnset();
DLLEXPORT BOOL __stdcall bSetEnableEPHelperLL64(BOOL bEPHelper);
DLLEXPORT BOOL __stdcall bSetEnableIMEModeForcedLL64(BOOL bIMEModeForced);


/* = EOF = */