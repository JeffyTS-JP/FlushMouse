//
// FlushMouseDll.h
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
#include "..\MISCLIB\DLLDefs.h"

//
// Global Data
//
extern UINT			WM_HOOKEX;

//
// Global Prototype Define
//
BOOL APIENTRY	DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);
HINSTANCE		hGetInstance();

/* = EOF = */