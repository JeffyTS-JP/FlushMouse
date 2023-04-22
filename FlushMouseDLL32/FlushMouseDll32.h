// FlushMouseDll32.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/04/04  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "..\MISCLIB\DLLDefs.h"

//
// Global Prototype Define
//
BOOL APIENTRY	DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);
HINSTANCE		hGetInstance();

/* EOF */