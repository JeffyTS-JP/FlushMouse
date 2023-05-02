// EventHookDll.cpp
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
#include "EventHookDll.h"
#include "..\FlushMouse\CommonDef.h"
#include "..\..\MiscLIB\CDebug\CDebug.h"

//
// Local Data
//
// Event Handller
static CEventHookDll	*EventHook = NULL;
static BOOL				bIMEInConversion = FALSE;

//
// bEventSet()
//
DLLEXPORT BOOL  __stdcall 	bEventSet()
{
	// Set Event Handller
	if (EventHook == NULL) {
		EventHook = new CEventHookDll;
		if (!EventHook->bEventSet()) {
			return FALSE;
		}
	}
	return TRUE;
}

//
// bEventUnset()
//
DLLEXPORT VOID  __stdcall 	vEventUnset()
{
	// Unset Event Handller
	if (EventHook != NULL) {
		delete EventHook;
		EventHook = NULL;
	}
}

DLLEXPORT BOOL	__stdcall	bGetIMEInConversion()
{
	return bIMEInConversion;
}

//
// class CEventHookDll
//
// https://docs.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-setwineventhook
// https://docs.microsoft.com/en-us/windows/win32/winauto/event-constants
//
CEventHookDll::CEventHookDll()
{
	//@@@ for WinUI3 hResult = CoInitialize(NULL);
	hEventHook = NULL;
	hEventHookIME = NULL;
}

CEventHookDll::~CEventHookDll()
{
	if (bEventUnset()) {
		hEventHook = NULL;
		hEventHookIME = NULL;
	}
}

//
// bEventSet()
//
BOOL		CEventHookDll::bEventSet()
{
	//#define	EVENT_FLAGS		(WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS | WINEVENT_SKIPOWNTHREAD)
//#define	EVENT_FLAGS		(WINEVENT_OUTOFCONTEXT)
#define	EVENT_FLAGS		(WINEVENT_INCONTEXT)
	hEventHook = SetWinEventHook(
							EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,	// Range of events
							hGetInstance(),										// Handle to DLL.
							&vHandleEvent,										// The callback.
							0, 0,												// Process and thread IDs of interest (0 = all)
							EVENT_FLAGS);										// Flags.
	if (hEventHook == NULL)	return FALSE;

	hEventHookIME = SetWinEventHook(
							EVENT_OBJECT_TEXTSELECTIONCHANGED, EVENT_OBJECT_TEXTEDIT_CONVERSIONTARGETCHANGED,	// Range of events
							hGetInstance(),										// Handle to DLL.
							&vHandleEventIME,									// The callback.
							0, 0,												// Process and thread IDs of interest (0 = all)
							EVENT_FLAGS);										// Flags.
	if (hEventHookIME == NULL)	return FALSE;

	return TRUE;
}

//
// bEventUnset()
//
BOOL		CEventHookDll::bEventUnset()
{
	if (hEventHookIME) {
		if (UnhookWinEvent(hEventHookIME)) {
			hEventHookIME = NULL;
		}
	}
	if (hEventHook) {
		if (UnhookWinEvent(hEventHook)) {
			hEventHook = NULL;
			//dwIMEEvent = 0;
		}
	}
	if ((hEventHookIME == NULL) && (hEventHook == NULL))	return TRUE;
	return FALSE;
}

//
// vHandleEvent()
//
void CALLBACK CEventHookDll::vHandleEvent(HWINEVENTHOOK hook, DWORD dwEvent, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	UNREFERENCED_PARAMETER(hook);
	UNREFERENCED_PARAMETER(idObject);
	UNREFERENCED_PARAMETER(idChild);
	UNREFERENCED_PARAMETER(dwEventThread);
	UNREFERENCED_PARAMETER(dwmsEventTime);

#ifdef _DEBUG
	if ((dwEvent == EVENT_SYSTEM_FOREGROUND) || (dwEvent == EVENT_OBJECT_FOCUS)) {
		//DBvPrintf(_T("vHandleEvent1 : event = 0x%08x hook = 0x%08x  idChild = 0x%08x\n"), dwEvent, hook, idChild);
		//DBvPrintf(_T("vHandleEvent2 : hWnd = 0x%08x GetForegroundWindow = 0x%08x idObject = 0x%08x\n"), hWnd, GetForegroundWindow(), idObject);
		//DBvPrintf(_T("vHandleEvent3 : dwEventThread = 0x%08x dwmsEventTime = 0x%08x\n"), dwEventThread, dwmsEventTime);
	}
#endif // _DEBUG

	if (dwEvent == EVENT_SYSTEM_FOREGROUND) {
		HWND	hFindWnd = FindWindow(_T("FLUSHMOUSE"), NULL);
#ifdef _DEBUG
		DBvPrintf(_T("vHandleEvent : event = 0x%08x hFindWnd = 0x%08x hWnd = 0x%08x GetForegroundWindow = 0x%08x\n"), dwEvent, hFindWnd, hWnd, GetForegroundWindow());
#endif // _DEBUG
		if (hFindWnd != NULL) {
			PostMessage(hFindWnd, WM_EVENT_SYSTEM_FOREGROUND, (WPARAM)dwEvent, (LPARAM)hWnd);
		}
	}
}

//
// vHandleEventIME()
//
void CALLBACK CEventHookDll::vHandleEventIME(HWINEVENTHOOK hook, DWORD dwEvent, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	UNREFERENCED_PARAMETER(hook);
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(idObject);
	UNREFERENCED_PARAMETER(idChild);
	UNREFERENCED_PARAMETER(dwEventThread);
	UNREFERENCED_PARAMETER(dwmsEventTime);

#ifdef _DEBUG
	//DBvPrintf(_T("vHandleEventIME1 : event = 0x%08x hook = 0x%08x  idChild = 0x%08x\n"), dwEvent, hook, idChild);
	//DBvPrintf(_T("vHandleEventIME2 : hWnd = 0x%08x GetForegroundWindow = 0x%08x idObject = 0x%08x\n"), hWnd, GetForegroundWindow(), idObject);
	//DBvPrintf(_T("vHandleEventIME3 : dwEventThread = 0x%08x dwmsEventTime = 0x%08x\n"), dwEventThread, dwmsEventTime);
#endif // _DEBUG

	switch (dwEvent) {
		case EVENT_OBJECT_CLOAKED:
		case EVENT_OBJECT_IME_HIDE:
			bIMEInConversion = FALSE;
			break;
		case EVENT_OBJECT_UNCLOAKED:
		case EVENT_OBJECT_IME_SHOW:
		/*
		{
			HWND	hFindWnd = FindWindow(_T("FLUSHMOUSE"), NULL);
			if (hFindWnd != NULL) {
				PostMessage(hFindWnd, WM_EVENT_OBJECT_IME_INCONVERSION, (WPARAM)dwEvent, (LPARAM)0);
			}
		}
		*/
			bIMEInConversion = TRUE;
			break;
		case EVENT_OBJECT_IME_CHANGE:
#ifdef _DEBUG
			DBvPrintf(_T("EVENT_OBJECT_IME_CHANGE\n"));
#endif // _DEBUG
			break;
		case EVENT_OBJECT_TEXTEDIT_CONVERSIONTARGETCHANGED:
#ifdef _DEBUG
			DBvPrintf(_T("EVENT_OBJECT_TEXTEDIT_CONVERSIONTARGETCHANGED\n"));
#endif // _DEBUG
			break;
		case EVENT_OBJECT_TEXTSELECTIONCHANGED:
		case EVENT_OBJECT_LIVEREGIONCHANGED:
			break;
		default:
	#ifdef _DEBUG
			DBvPrintf(_T("Unkown event = 0x%08x\n"), dwEvent);
	#endif // _DEBUG
			return;
	}
}


/* = EOF = */