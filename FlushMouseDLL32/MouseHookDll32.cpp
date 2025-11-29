//
// MouseHookDll.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
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
#include "MouseHookDll32.h"
#include "..\FlushMouseLIB\CommonDef.h"
#include "..\FlushMouseDLL\EventlogData.h"

//
// Define
//

//
// Local Prototype Define
//
static LRESULT  CALLBACK    lpMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
static BOOL	bReportEvent(DWORD dwEventID, WORD wCategory);

//
// Local Data
//
#pragma comment( linker, "/SECTION:FLUSHMOUSEDLL_SEG32,RWS" )
#pragma data_seg("FLUSHMOUSEDLL_SEG32")
static HWND		hWndMSParent = NULL;
static HHOOK	hHookMS = NULL;
#pragma data_seg()

//
// bMouseHookSet()
//
DLLEXPORT BOOL  __stdcall bMouseHookSet32(HWND hWnd)
{
	hWndMSParent = hWnd;
	hHookMS = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)lpMouseHookProc, hGetInstance(), 0);
	if (hHookMS) {
		return TRUE;
	}
	hWndMSParent = NULL;
	hHookMS = NULL;
	return FALSE;
}

//
// bMouseHookUnset()
//
DLLEXPORT BOOL __stdcall bMouseHookUnset32()
{
	BOOL		bRet = FALSE;
	if (hHookMS) {
		if (UnhookWindowsHookEx(hHookMS)) {
			bRet = TRUE;
		}
		hWndMSParent = NULL;
		hHookMS = NULL;
	}
	return bRet;
}

//
// lpMouseHookProc()
//
template<typename T1, typename T2>
constexpr DWORD WORD2DWORD(T1 h, T2  l) { return (DWORD)(((((DWORD)(l)) << 16) & 0xffff0000) | ((DWORD)(h) & 0xffff)); }

static LRESULT CALLBACK lpMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	if (nCode == HC_ACTION) {
		switch (wParam) {
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_NCRBUTTONUP:
		{
			MOUSEHOOKSTRUCT* mh = reinterpret_cast<MOUSEHOOKSTRUCT*>(lParam);
			DWORD pt = WORD2DWORD(mh->pt.x, mh->pt.y);
			try {
				PostMessage(hWndMSParent, (UINT)(WM_USER + wParam), (WPARAM)mh->hwnd, (LPARAM)pt);
			}
			catch (BOOL bRet) {
				if (bRet == FALSE) {
					if (GetLastError() == ERROR_ACCESS_DENIED) {
						bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, DLL32_CATEGORY);
					}
				}
			}
			catch (...) {
			}
		}
		break;
		default:
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//
// bReportEvent()
//
static BOOL		bReportEvent(DWORD dwEventID, WORD wCategory)
{
	BOOL	bRet = FALSE;
	HANDLE	hEvent = RegisterEventSource(NULL, FLUSHMOUSE);
	if (hEvent != NULL) {
		if (ReportEvent(hEvent, (0x0000000c & (dwEventID >> 28)), wCategory, dwEventID, NULL, 0, 0, NULL, NULL) != 0) {
			bRet = TRUE;
		}
		else {
		}
		if (DeregisterEventSource(hEvent) == 0) {
			bRet = FALSE;
		}
	}
	return bRet;
}

/* = EOF = */
