//
// FlushMouse.cpp
//		for Windows10/11 Win32 API msi Package App.
//		Copyright (C) 1993 JeffyTS
//
//  FlushMouse.exe
//      	マウスカーソルにIMEの状態を表示する
//      
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	1993/11/17  JeffyTS  	New edit.
// #0001	2022/02/04  JeffyTS		Visual Studio 2022 (C++)へ移植
//

//
// Include
//
#pragma once
#include "pch.h"
#include "..\FlushMouseLIB\FlushMouseLIB.h"
#include "..\FlushMouseLIB\CommonDef.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#define _CRTDBG_MAP_ALLOC
#define new DEBUG_CLIENTBLOCK
#endif

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

//
// Define
//

//
// Struct Define
//

//
// Global Data
//

//
// Local Data
//

//
// Global Prototype Define
//
int APIENTRY	wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd);

//
// Local Prototype Define
//

//
//  関数: wWinMain()
//
int APIENTRY	wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
#if defined _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_ASSERTE(_CrtCheckMemory());
#endif
	
	SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);

	int	iRet = 0;
	if ((iRet = iCheckCmdLine(lpCmdLine)) != 1) {
		return (iRet);
	}
	
	if (!bWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd)) {
		return (-1);
	}

	HWND	_hWnd = FindWindow(CLASS_FLUSHMOUSESETTINGS, NULL);
	if (_hWnd != NULL) {
		SendMessage(_hWnd, WM_DESTROY, 0, 0);
	}

#if defined _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); 
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}

//
// vSettingDialogApply()
//
VOID		vSettingDialogApply()
{
}

//
// vSettingDialogClose()
//
VOID		vSettingDialogClose()
{
}


/* = EOF = */
