// FlushMouseCursor.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/03/17  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "FlushMouseCursor.h"

//
// Local Data
//
static HINSTANCE    hDLLInstance = NULL;

//
// DllMain()
//
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			hDLLInstance = hModule;
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
// hGetDllInstance()
//
HINSTANCE hGetDllInstance()
{
	return hDLLInstance;
}
