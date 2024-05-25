//
// ShellHookDll.h
//		Copyright (C) 2022 JeffyTS
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