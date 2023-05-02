// EventtHookDll.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2023/04/14  JeffyTS  	New edit.
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
DLLEXPORT BOOL  __stdcall 	bEventSet();
DLLEXPORT VOID  __stdcall 	vEventUnset();
DLLEXPORT BOOL	__stdcall	bGetIMEInConversion();

//
// class CEventHookDll
//
class CEventHookDll
{
public:
	CEventHookDll();
	~CEventHookDll();
	BOOL		bEventSet();
	BOOL		bEventUnset();

private:
	static void CALLBACK vHandleEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
	static void CALLBACK vHandleEventIME(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

private:
	//@@@ for WinUI3 HRESULT			hResult;
	HWINEVENTHOOK	hEventHook;
	HWINEVENTHOOK	hEventHookIME;
};


/* = EOF = */