// FlushMouseDll.cpp : DLL アプリケーションのエントリ ポイントを定義します。
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/02/12  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "FlushMouseDll.h"

#pragma comment(linker, "/SECTION:FLUSHMOUSEDLL_SEG,RWS")
#pragma data_seg("FLUSHMOUSEDLL_SEG")
//
// Global Data
//
UINT				WM_HOOKEX = 0;

//
// Local Data
//
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
			if (WM_HOOKEX == 0)	WM_HOOKEX = RegisterWindowMessage(_T("FlushMouseDLL"));
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