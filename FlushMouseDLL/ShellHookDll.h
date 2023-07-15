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
#include "..\MiscLIB\CSharedMemory.h"

//
// Define
// 
#define SHELLHOOKMEM _T("ShellHookSharedMem")
typedef struct tagSHELL_SHAREDMEM {
	HINSTANCE	hInstance;
	HWND		hWnd;
	HHOOK		hHook;
} SHELL_SHAREDMEM, * PSHELL_SHAREDMEM, * LPSHELL_SHAREDMEM;

//
// Global Prototype Define
//
DLLEXPORT BOOL __stdcall bShellHookSet(HWND);
DLLEXPORT BOOL __stdcall bShellHookUnset(void);


/* = EOF = */