﻿// FlushMouseDll32.cpp
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
#include "pch.h"
#include "FlushMouseDll32.h"

//
// Local Data
//
#pragma comment( linker, "/SECTION:FLUSHMOUSEDLL_SEG32,RWS" )
#pragma data_seg("FLUSHMOUSEDLL_SEG32")
static HINSTANCE    hDLLInstance = NULL;
#pragma data_seg()

//
// DllMain()
//
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		hDLLInstance = hModule;
		DisableThreadLibraryCalls(hDLLInstance);

		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

//
// hGetInstance()
//
HINSTANCE hGetInstance()
{
	return hDLLInstance;
}

/* = EOF = */