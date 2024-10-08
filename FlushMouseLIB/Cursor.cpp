﻿//
// Cursor.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/03  JeffyTS  	New edit.
//

// Include
//
#pragma once
#include "pch.h"
#include "Profile.h"
#include "Cursor.h"
#include "FlushMouseLIB.h"
#include "CommonDef.h"
#include "Eventlog.h"
#include "..\MiscLIB\CThread.h"
#include "..\MiscLIB\CRegistry.h"

//
// Library
//

//
// Define
//
#define FLUSHMOUSECURSORDIR			_T("\\JeffyTS\\FlushMouse\\")
#define FLUSHMOUSECURSORDAT			_T("FlushMouseCursor.dat")

// IME Cursor Change Thread
#define IMECURSORCHANGETHREADID		1
#define IMECURSORCHANGETHREADNAME	_T("IMECursorChangeThread")
// Draw IME Mode near by Mouse cursor Thread
#define	DRAWIMEMODETHREADID			2
#define	DRAWIMEMODETHREADNAME		_T("DrawIMEModeThread")
// Draw IME Mode near by Caret cursor Thread
#define	DRAWIMEMODECARETTHREADID	3
#define	DRAWIMEMODECARETTHREADNAME	_T("DrawIMEModeCaretThread")
// Draw IME Mode near by Mouse cursor right down Thread
#define	DRAWIMEMODEMOUSETHREADID	4
#define	DRAWIMEMODEMOUSETHREADNAME	_T("DrawIMEModeMouseThread")

//
// Struct Define
//

//
// Class Define

//
// Local Data
//
MOUSECURSOR	stAllMouseCursor[sizeof(MOUSECURSOR) * 20] = {
	{ OCR_APPSTARTING, 0, TRUE,  _T("AppStarting"), _T("") },
	{ OCR_NORMAL,	   0, FALSE, _T("Arrow"),		_T("") },
	{ OCR_CROSS,	   0, TRUE,  _T("Crosshair"),	_T("") },
	{ OCR_HAND,		   0, FALSE, _T("Hand"),		    _T("") },
	{ OCR_HELP,		   0, TRUE,  _T("Help"),		    _T("") },
	{ OCR_IBEAM,	   0, FALSE, _T("IBeam"),		_T("") },
	{ OCR_NO,		   0, TRUE,  _T("No"),			_T("") },
	// { OCR_NWPEN,	   0, TRUE,  _T("NWPen"),		_T("") },
	// { OCR_PEN,      0, TRUE,  _T("Pen"),		    _T("") },
	{ OCR_PERSON,	   0, TRUE,  _T("Person"),		_T("") },
	{ OCR_PIN,		   0, TRUE,  _T("Pin"),		    _T("") },
	{ OCR_SIZEALL,	   0, TRUE,  _T("SizeAll"),	    _T("") },
	{ OCR_SIZENESW,	   0, TRUE,  _T("SizeNESW"),    _T("") },
	{ OCR_SIZENS,	   0, TRUE,  _T("SizeNS"),		_T("") },
	{ OCR_SIZENWSE,	   0, TRUE,  _T("SizeNWSE"),    _T("") },
	{ OCR_SIZEWE,	   0, TRUE,  _T("SizeWE"),		_T("") },
	{ OCR_UP,		   0, TRUE,  _T("UpArrow"),	    _T("") },
	{ OCR_WAIT,		   0, TRUE,  _T("Wait"),		    _T("") },
	{ OCR_HIDE,		   0, TRUE,  _T("Hide"),		    _T("") },
	{ (DWORD)(-1), 	   0, FALSE, _T(""),			    _T("") }
};

FLUSHMOUSECURSOR	stFlushMouseCursor[sizeof(FLUSHMOUSECURSOR) * 8] = {
	{ IMEOFF,			_T("A"),	{OCR_NORMAL, IDC_IMEOFF_ARROW,		  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeOffArrow.cur")},
	{OCR_HAND,   IDC_IMEOFF_HAND,		  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeOffHand.cur")},
	{OCR_IBEAM,  IDC_IMEOFF_IBEAM,		  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeOffIBeam.cur")}},
	
	{ ZENHIRA_IMEON,	_T("あ"),	{OCR_NORMAL, IDC_ZENHIRA_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenHiraOnArrow.cur")},
	{OCR_HAND,   IDC_ZENHIRA_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenHiraOnHand.cur")},
	{OCR_IBEAM,  IDC_ZENHIRA_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenHiraOnIBeam.cur")}},
	
	{ HANEISU_IMEON,	_T("_A"),	{OCR_NORMAL, IDC_HANEISU_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanEisuOnArrow.cur")},
	{OCR_HAND,   IDC_HANEISU_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanEisuOnHand.cur")},
	{OCR_IBEAM,  IDC_HANEISU_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanEisuOnIBeam.cur")}},
	
	{ HANKANA_IMEON,	_T("_ｱ"),	{OCR_NORMAL, IDC_HANKANA_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanKanaOnArrow.cur")},
	{OCR_HAND,   IDC_HANKANA_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanKanaOnHand.cur")},
	{OCR_IBEAM,  IDC_HANKANA_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanKanaOnIBeam.cur")}},
	
	{ ZENEISU_IMEON,	_T("Ａ"),	{OCR_NORMAL, IDC_ZENEISU_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenEisuOnArrow.cur")},
	{OCR_HAND,   IDC_ZENEISU_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenEisuOnHand.cur")},
	{OCR_IBEAM,  IDC_ZENEISU_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenEisuOnIBeam.cur")}},
	
	{ ZENKANA_IMEON,	_T("ア"),	{OCR_NORMAL, IDC_ZENKANA_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenKanaOnArrow.cur")},
	{OCR_HAND,   IDC_ZENKANA_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenKanaOnHand.cur")},
	{OCR_IBEAM,  IDC_ZENKANA_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenKanaOnIBeam.cur")}},
	
	{ IMEHIDE,			_T(""),		{OCR_NORMAL, IDC_HIDE_ARROW,          FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHideOnArrow.cur")},
	{OCR_HAND,   IDC_HIDE_HAND,           FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHideOnHand.cur")},
	{OCR_IBEAM,  IDC_HIDE_IBEAM,          FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHideOnIBeam.cur")}},
	
	{ (DWORD)(-1),		_T(""),		{0,			 0,						  FALSE, _T(""), _T("")},
	{0,			 0,						  FALSE, _T(""), _T("")},
	{0,			 0,						  FALSE, _T(""), _T("")} }
};

//
// class CCursor
//
CCursor::CCursor()
{
	this->hMainWnd = NULL;

	hCursorData = NULL;
	iCursorDataLoadCount = 0;

	stIMECursorData.hWndObserved = NULL;
	stIMECursorData.dwIMEModeCursor = (DWORD)(-1);
	stIMECursorData.bDrawIMEModeWait = FALSE;
	stIMECursorData.bDrawNearCaret = FALSE;
	stIMECursorData.lpszLoadDatName = NULL;
	stIMECursorData.lpstFlushMouseCursor = (LPFLUSHMOUSECURSOR)&stFlushMouseCursor;

	stIMECursorData.hWndCaret = NULL;

	IMECursorChangeThread = NULL;
	DrawIMEModeThread = NULL;
	DrawIMEModeCaretThread = NULL;
	DrawIMEModeMouseByWndThread = NULL;

	CursorWindow = NULL;
	CaretWindow = NULL;
	MouseWindow = NULL;
}

CCursor::~CCursor()
{
	if (DrawIMEModeThread != NULL) {
		delete	DrawIMEModeThread;
		DrawIMEModeThread = NULL;
	}
	if (DrawIMEModeCaretThread != NULL) {
		delete	DrawIMEModeCaretThread;
		DrawIMEModeCaretThread = NULL;
	}
	if (CursorWindow != NULL) {
		delete	CursorWindow;
		CursorWindow = NULL;
	}
	if (CaretWindow != NULL) {
		delete	CaretWindow;
		CaretWindow = NULL;
	}
	vUnRegisterDrawIMEModeMouseByWndThread();
	vUnRegisterIMECursorChangeThread();
	if (stIMECursorData.lpszLoadDatName != NULL) {
		delete []	stIMECursorData.lpszLoadDatName;
		stIMECursorData.lpszLoadDatName = NULL;
	}
	if (hCursorData != NULL) {
		bCursorDllUnload();
	}
}

//
// bInitialize()
//
BOOL			CCursor::bInitialize(HWND hWnd)
{
	this->hMainWnd = hWnd;

	return bReloadCursor();
}

//
// bReloadCursor()
//
BOOL			CCursor::bReloadCursor()
{
	vSetParamFromRegistry();

	if (hCursorData != NULL) {
		bCursorDllUnload();
	}
	
	if (stIMECursorData.lpszLoadDatName == NULL) {
		if ((stIMECursorData.lpszLoadDatName = lpszGetCursorDataName()) == NULL)	return FALSE;
	}
	if (hCursorData == NULL) {
		if (hCursorDllLoad() == NULL)	return FALSE;
		if (!bSystemCursorLoad())	return FALSE;
	}
#define	WINDOWCLASS		_T("FlushMouseCursorWindow-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}")
	if (CursorWindow == NULL) {
		CursorWindow = new CCursorWindow;
		if (CursorWindow == NULL)	return FALSE;
		if (!CursorWindow->bRegister((HINSTANCE)GetWindowLongPtr(hMainWnd, GWLP_HINSTANCE), WINDOWCLASS))		return FALSE;
	}
	CursorWindow->vSetModeStringAndColor(stIMECursorData.lpstFlushMouseCursor[0].szMode, stIMECursorData.dwNearDrawMouseIMEOFFColor);
#undef WINDOWCLASS
#define	WINDOWCLASS		_T("FlushMouseCaretWindow-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}")
	if (CaretWindow == NULL) {
		CaretWindow = new CCursorWindow;
		if (CaretWindow == NULL)	return FALSE;
		if (!CaretWindow->bRegister((HINSTANCE)GetWindowLongPtr(hMainWnd, GWLP_HINSTANCE), WINDOWCLASS))	return FALSE;
	}
	CaretWindow->vSetModeStringAndColor(stIMECursorData.lpstFlushMouseCursor[0].szMode, stIMECursorData.dwNearDrawCaretIMEOFFColor);
#undef WINDOWCLASS
	if (DrawIMEModeCaretThread == NULL) {
		DrawIMEModeCaretThread = new CThread;
		if (DrawIMEModeCaretThread == NULL)	return FALSE;
	}
	if (DrawIMEModeThread == NULL) {
		DrawIMEModeThread = new CThread;
		if (DrawIMEModeThread != NULL) {
			if (!bRegisterDrawIMEModeThread(hMainWnd))	return FALSE;
		}
	}
	if (DrawIMEModeThread == NULL) {
		DrawIMEModeThread = new CThread;
		if (DrawIMEModeThread != NULL) {
			if (!bRegisterDrawIMEModeThread(hMainWnd))	return FALSE;
		}
	}
	if (!bRegisterDrawIMEModeMouseByWndThread(hMainWnd))	return FALSE;
	if (!bRegisterIMECursorChangeThread(hMainWnd)) return FALSE;
	vStopDrawIMEModeMouseByWndThread();
	if (stIMECursorData.bDisplayIMEModeByWindow) {
		SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
	}
	return TRUE;
}

//
// vSetParamFromRegistry()
//
VOID			CCursor::vSetParamFromRegistry()
{
	if (Profile != NULL) {
		stIMECursorData.iCursorSize = Profile->lpstAppRegData->iCursorSize;
		stIMECursorData.iIMEModeDistance = Profile->lpstAppRegData->iIMEModeDistance;
		stIMECursorData.iModeSize = Profile->lpstAppRegData->iModeSize;
		stIMECursorData.dwInThreadSleepTime = Profile->lpstAppRegData->dwInThreadSleepTime;
		stIMECursorData.dwDisplayModeTime = Profile->lpstAppRegData->dwDisplayModeTime;
		stIMECursorData.bDisplayFocusWindowIME = Profile->lpstAppRegData->bDisplayFocusWindowIME;
		stIMECursorData.bDisplayIMEModeOnCursor = Profile->lpstAppRegData->bDisplayIMEModeOnCursor;
		stIMECursorData.bDisplayIMEModeByWindow = Profile->lpstAppRegData->bDisplayIMEModeByWindow;
		stIMECursorData.bDisplayIMEModeIMEOFF = Profile->lpstAppRegData->bDisplayIMEModeIMEOFF;
		stIMECursorData.bForceHiragana = Profile->lpstAppRegData->bForceHiragana;
		stIMECursorData.bDrawNearCaret = Profile->lpstAppRegData->bDrawNearCaret;
		stIMECursorData.dwNearDrawMouseIMEOFFColor = (~Profile->lpstAppRegData->dwNearDrawMouseIMEOFFColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseIMEOFFColor & 0x00ffffff);
		stIMECursorData.dwNearDrawMouseHANEISU_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawMouseHANKANA_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawMouseZENEISU_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawMouseZENHIRA_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawMouseZENKANA_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawCaretIMEOFFColor = (~Profile->lpstAppRegData->dwNearDrawCaretIMEOFFColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretIMEOFFColor & 0x00ffffff);
		stIMECursorData.dwNearDrawCaretHANEISU_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawCaretHANKANA_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawCaretZENEISU_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawCaretZENHIRA_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawCaretZENKANA_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawMouseByWndIMEOFFColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor & 0x00ffffff);
		stIMECursorData.dwNearDrawMouseByWndHANEISU_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawMouseByWndHANKANA_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawMouseByWndZENEISU_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawMouseByWndZENHIRA_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor & 0x00ffffff);
		stIMECursorData.dwNearDrawMouseByWndZENKANA_IMEONColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor & 0x00ffffff);

		stIMECursorData.bDenyChangedByApp = Profile->lpstAppRegData->bDenyChangedByApp;
		stIMECursorData.bUseBigArrow = Profile->lpstAppRegData->bUseBigArrow;
	}
}	

//
// lpszGetCursorDataName()
//
LPTSTR		CCursor::lpszGetCursorDataName()
{
	LPTSTR	lpszCursorDataName = NULL;
	LPTSTR	lpszBuffer = new TCHAR[_MAX_PATH];
	if (lpszBuffer) {
		ZeroMemory(lpszBuffer, _MAX_PATH);
		DWORD	dwSize = 0;
		GetCurrentDirectory(_MAX_PATH, lpszBuffer);
		_tcsncat_s(lpszBuffer, _MAX_PATH, _T("\\"), sizeof(_T("\\")));
		_tcsncat_s(lpszBuffer, _MAX_PATH, FLUSHMOUSECURSORDAT, sizeof(FLUSHMOUSECURSORDAT));
		dwSize = ExpandEnvironmentStrings(lpszBuffer, NULL, 0);
		lpszCursorDataName = new TCHAR[dwSize];
		if (lpszCursorDataName) {
			ZeroMemory(lpszCursorDataName, dwSize);
			dwSize = ExpandEnvironmentStrings(lpszBuffer, lpszCursorDataName, dwSize);
			if (GetFileAttributes(lpszCursorDataName) == INVALID_FILE_ATTRIBUTES) {	
				delete[]	lpszCursorDataName;
				lpszCursorDataName = NULL;
				ZeroMemory(lpszBuffer, _MAX_PATH);
				_tcsncat_s(lpszBuffer, _MAX_PATH, _T("%APPDATA%"), sizeof(_T("%APPDATA%")));
				_tcsncat_s(lpszBuffer, _MAX_PATH, FLUSHMOUSECURSORDIR, sizeof(FLUSHMOUSECURSORDIR));
				_tcsncat_s(lpszBuffer, _MAX_PATH, FLUSHMOUSECURSORDAT, sizeof(FLUSHMOUSECURSORDAT));
				dwSize = ExpandEnvironmentStrings(lpszBuffer, NULL, 0);
				lpszCursorDataName = new TCHAR[dwSize];
				if (lpszCursorDataName) {
					ZeroMemory(lpszCursorDataName, dwSize);
					dwSize = ExpandEnvironmentStrings(lpszBuffer, lpszCursorDataName, dwSize);
					if (GetFileAttributes(lpszCursorDataName) == INVALID_FILE_ATTRIBUTES) {
						delete[]	lpszCursorDataName;
						lpszCursorDataName = NULL;
						ZeroMemory(lpszBuffer, _MAX_PATH);
						_tcsncat_s(lpszBuffer, _MAX_PATH, _T("%ALLUSERSPROFILE%"), sizeof(_T("%ALLUSERSPROFILE%")));
						_tcsncat_s(lpszBuffer, _MAX_PATH, FLUSHMOUSECURSORDIR, sizeof(FLUSHMOUSECURSORDIR));
						_tcsncat_s(lpszBuffer, _MAX_PATH, FLUSHMOUSECURSORDAT, sizeof(FLUSHMOUSECURSORDAT));
						dwSize = ExpandEnvironmentStrings(lpszBuffer, NULL, 0);
						lpszCursorDataName = new TCHAR[dwSize];
						if (lpszCursorDataName) {
							ZeroMemory(lpszCursorDataName, dwSize);
							dwSize = ExpandEnvironmentStrings(lpszBuffer, lpszCursorDataName, dwSize);
							if (GetFileAttributes(lpszCursorDataName) == INVALID_FILE_ATTRIBUTES) {
								delete[]	lpszCursorDataName;
								lpszCursorDataName = NULL;
							}
						}
					}
				}
			}
		}
		delete[]	lpszBuffer;
	}
	return lpszCursorDataName;
}

//
// bStartIMECursorChangeThread()
//
BOOL		CCursor::bStartIMECursorChangeThread(HWND hWndObserved)
{
	vSetParamFromRegistry();
	stIMECursorData.hWndObserved = hWndObserved;
	if ((Profile != NULL) && Profile->lpstAppRegData->bDisplayIMEModeByWindow) {
		if (!bStartDrawIMEModeMouseByWndThread())	return FALSE;
	}
	else {
		if ((Cursor == NULL) || (IMECursorChangeThread == NULL))	return FALSE;
		if (!IMECursorChangeThread->bStart()) {
			if (IMECursorChangeThread != NULL) {
				delete	IMECursorChangeThread;
				IMECursorChangeThread = NULL;
			}
			IMECursorChangeThread = new CThread;
			if (!bRegisterIMECursorChangeThread(this->hMainWnd)) return FALSE;
		}
	}
	return TRUE;
}

//
// bStartDrawIMEModeThread()
//
BOOL		CCursor::bStartDrawIMEModeThread(HWND hWndObserved)
{
	vSetParamFromRegistry();
	stIMECursorData.bDrawIMEModeWait = FALSE;
	stIMECursorData.dwWaitTime = 0;
	return bStartDrawIMEModeThreadSub(hWndObserved);
}

//
// bStartDrawIMEModeThreadWait()
//
BOOL		CCursor::bStartDrawIMEModeThreadWait(HWND hWndObserved)
{
	vSetParamFromRegistry();
	stIMECursorData.bDrawIMEModeWait = TRUE;
	if (Profile == NULL)	return FALSE;
	stIMECursorData.dwWaitTime = Profile->lpstAppRegData->dwAdditionalWaitTime;
	return bStartDrawIMEModeThreadSub(hWndObserved);
}

//
// bStartDrawIMEModeThreadWaitWave()
//
BOOL		CCursor::bStartDrawIMEModeThreadWaitWave(HWND hWndObserved)
{
	vSetParamFromRegistry();
	stIMECursorData.bDrawIMEModeWait = TRUE;
	stIMECursorData.bDrawNearCaret = FALSE;
	if (Profile == NULL)	return FALSE;
	stIMECursorData.dwWaitTime = Profile->lpstAppRegData->dwWaitWaveTime;
	stIMECursorData.bDrawNearCaret = Profile->lpstAppRegData->bDrawNearCaret;
	return bStartDrawIMEModeThreadSub(hWndObserved);
}

//
// bStartDrawIMEModeThreadWaitEventForeGround()
//
BOOL		CCursor::bStartDrawIMEModeThreadWaitEventForeGround(HWND hWndObserved)
{
	vSetParamFromRegistry();
	stIMECursorData.bDrawIMEModeWait = TRUE;
	if (Profile == NULL)	return FALSE;
	stIMECursorData.dwWaitTime = GetDoubleClickTime() +  Profile->lpstAppRegData->dwAdditionalWaitTime;
	return bStartDrawIMEModeThreadSub(hWndObserved);
}

//
// bStartDrawIMEModeThreadWaitDblClk()
//
BOOL		CCursor::bStartDrawIMEModeThreadWaitDblClk(HWND hWndObserved)
{
	vSetParamFromRegistry();
	stIMECursorData.bDrawIMEModeWait = TRUE;
	if (Profile == NULL)	return FALSE;
	if (GetDoubleClickTime() >  Profile->lpstAppRegData->dwAdditionalWaitTime)	stIMECursorData.dwWaitTime = GetDoubleClickTime();
	else stIMECursorData.dwWaitTime = Profile->lpstAppRegData->dwAdditionalWaitTime;
	return bStartDrawIMEModeThreadSub(hWndObserved);
}

//
// bStartDrawIMEModeThreadSub()
//
BOOL		CCursor::bStartDrawIMEModeThreadSub(HWND hWndObserved)
{
	if ((Cursor == NULL) || (DrawIMEModeThread == NULL))	return FALSE;
	stIMECursorData.hWndObserved = hWndObserved;
	stIMECursorData.bIMECursorChangeThreadSentinel = FALSE;
	DrawIMEModeThread->bSetSentinel(FALSE);
	stIMECursorData.hWndCaret = hGetCaretPosByAccessibleObjectFromWindow(GetForegroundWindow(), &stIMECursorData, FALSE);
	if (!DrawIMEModeThread->bStart()) {
		if (DrawIMEModeThread != NULL) {
			delete	DrawIMEModeThread;
			DrawIMEModeThread = NULL;
		}
		DrawIMEModeThread = new CThread;
		if (!bRegisterDrawIMEModeThread(this->hMainWnd)) return FALSE;
	}
	return TRUE;
}

//
// bShowHideCursor()
//
BOOL		CCursor::bShowHideCursor(HWND hWndObserved, BOOL bShow)
{
	BOOL	bRet = FALSE;
	if (bShow) {
		bRet = bStartDrawIMEModeThread(hWndObserved);
	}
	else {
		bRet = bChangeFlushMouseCursor(IMEHIDE, &stIMECursorData);
	}
	return bRet;
}

//
// hCursorDllLoad()
// 
HMODULE		CCursor::hCursorDllLoad()
{
	if ((hCursorData == NULL) && (iCursorDataLoadCount == 0)) {
		if ((hCursorData = LoadLibraryEx(stIMECursorData.lpszLoadDatName, NULL, (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE))) == NULL) {
			return NULL;
		}
	}
	++iCursorDataLoadCount;
	return hCursorData;
}

//
// bCursorDllUnload()
//
BOOL		CCursor::bCursorDllUnload()
{
	if ((hCursorData != NULL) && (iCursorDataLoadCount == 1)) {
		if (!FreeLibrary(hCursorData)) {
			return FALSE;
		}
	}
	--iCursorDataLoadCount;
	if (iCursorDataLoadCount == 0)	hCursorData = NULL;
	return TRUE;
}

//
// bSystemCursorLoad()
//
BOOL		CCursor::bSystemCursorLoad()
{
	if (!bChangeFlushMouseCursor(IMEOFF, &stIMECursorData))		return FALSE;
	
	int	i = 0;
	while (stAllMouseCursor[i].id != (DWORD)(-1)) {
		if (stAllMouseCursor[i].bReadReg) {
			if (!bGetMouseRegValue(stAllMouseCursor[i].szRegValue, stAllMouseCursor[i].szFile))		return FALSE;
		}
		i++;
	}
	return TRUE;
}

//
// bRegisterIMECursorChangeThread()
//
BOOL		CCursor::bRegisterIMECursorChangeThread(HWND hWnd)
{
	if (IMECursorChangeThread == NULL) {
		IMECursorChangeThread = new CThread;
		if (!bChangeFlushMouseCursor(IMEOFF, &stIMECursorData)) return FALSE;
		stIMECursorData.hWndObserved = hWnd;
		if (!IMECursorChangeThread->bRegister(IMECURSORCHANGETHREADNAME, IMECURSORCHANGETHREADID,
										(LPTHREAD_START_ROUTINE)&bIMECursorChangeRoutine, &stIMECursorData, stIMECursorData.dwInThreadSleepTime)) {
			return	FALSE;
		}
		if (!bChangeFlushMouseCursor(IMEOFF, &stIMECursorData)) return FALSE;
	}
	return	TRUE;
}

//
// vUnRegisterIMECursorChangeThread()
//
VOID		CCursor::vUnRegisterIMECursorChangeThread()
{
	if (IMECursorChangeThread != NULL) {
		delete	IMECursorChangeThread;
		IMECursorChangeThread = NULL;
	}
}

//
// bIMECursorChangeRoutine()
//
BOOL		CCursor::bIMECursorChangeRoutine(LPVOID lpvParam)
{
	if (lpvParam == NULL)	return FALSE;
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lpvParam;
	CCursor	*This = reinterpret_cast<CCursor*>(lpvParam);
	lpstCursorData->bIMECursorChangeThreadSentinel = TRUE;
	BOOL	bRet = TRUE;
	if (lpstCursorData->bDisplayIMEModeOnCursor && !lpstCursorData->bDisplayIMEModeByWindow) {
		This->bIsIMECursorChanged(lpstCursorData);
		if (!This->bChangeFlushMouseCursor(lpstCursorData->dwIMEModeCursor, lpstCursorData)) {
			bRet = FALSE;
		}
	}
	else {
		if (!This->bChangeFlushMouseCursor(IMEOFF, lpstCursorData)) {
			bRet = FALSE;
		}
	}
	return bRet;
}

//
// bRegisterDrawIMEModeMouseByWndThread()
//
BOOL		CCursor::bRegisterDrawIMEModeMouseByWndThread(HWND hWnd)
{
#define	WINDOWCLASS		_T("FlushMouseMouseWindow-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}")
	if (MouseWindow == NULL) {
		MouseWindow = new CCursorWindow;
		if (MouseWindow == NULL)	return FALSE;
		if (!MouseWindow->bRegister((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), WINDOWCLASS))	return FALSE;
	}
	MouseWindow->vSetModeStringAndColor(stIMECursorData.lpstFlushMouseCursor[IMEOFF].szMode, stIMECursorData.dwNearDrawMouseByWndIMEOFFColor);
#undef WINDOWCLASS
	if (DrawIMEModeMouseByWndThread == NULL) {
		DrawIMEModeMouseByWndThread = new CThread;
		if (DrawIMEModeMouseByWndThread == NULL)	return FALSE;
		stIMECursorData.hWndObserved = hWnd;
		if (!DrawIMEModeMouseByWndThread->bRegister(DRAWIMEMODEMOUSETHREADNAME, DRAWIMEMODEMOUSETHREADID,
			(LPTHREAD_START_ROUTINE)&bIMEModeMouseByWndThreadRoutine, &stIMECursorData, stIMECursorData.dwInThreadSleepTime)) {
			return	FALSE;
		}
	}
	HANDLE	hHandle = GetCurrentProcess();
	if (hHandle != NULL) {
		if (!SetPriorityClass(hHandle, REALTIME_PRIORITY_CLASS)) {
			CloseHandle(hHandle);
			return FALSE;
		}
	}
	if (!DrawIMEModeMouseByWndThread->bSetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL)) {
		if (hHandle != NULL)	CloseHandle(hHandle);
		return FALSE;
	}
	if (!MouseWindow->bSetWndThreadPriority(THREAD_PRIORITY_TIME_CRITICAL)) {
		if (hHandle != NULL)	CloseHandle(hHandle);
		return FALSE;
	}
	if (hHandle != NULL) {
		if (!SetPriorityClass(hHandle, NORMAL_PRIORITY_CLASS)) {
			CloseHandle(hHandle);
			return FALSE;
		}
		CloseHandle(hHandle);
	}
	if (Profile) {
		if (!stIMECursorData.bDisplayIMEModeByWindow)	vStopDrawIMEModeMouseByWndThread();
	}
	return	TRUE;
}

//
// vUnRegisterDrawIMEModeMouseByWndThread()
//
VOID		CCursor::vUnRegisterDrawIMEModeMouseByWndThread()
{
	vStopDrawIMEModeMouseByWndThread();
	if (DrawIMEModeMouseByWndThread != NULL) {
		delete	DrawIMEModeMouseByWndThread;
		DrawIMEModeMouseByWndThread = NULL;
	}
	if (MouseWindow != NULL) {
		MouseWindow->bShowWindow(SW_HIDE);
		delete	MouseWindow;
		MouseWindow = NULL;
	}
}

//
// bStartDrawIMEModeMouseByWndThread()
//
BOOL		CCursor::bStartDrawIMEModeMouseByWndThread()
{
	if ((Cursor == NULL) || (DrawIMEModeMouseByWndThread == NULL))	return FALSE;
	if (!DrawIMEModeMouseByWndThread->bStart()) {
		if (DrawIMEModeMouseByWndThread != NULL) {
			delete	DrawIMEModeMouseByWndThread;
			DrawIMEModeMouseByWndThread = NULL;
		}
		DrawIMEModeMouseByWndThread = new CThread;
		if (!bRegisterDrawIMEModeMouseByWndThread(this->hMainWnd)) return FALSE;
	}
	return TRUE;
}

//
// vStopDrawIMEModeMouseByWndThread()
//
VOID		CCursor::vStopDrawIMEModeMouseByWndThread()
{
	if (MouseWindow != NULL)		MouseWindow->bShowWindow(SW_HIDE);
	stIMECursorData.bIMEModeByWindowThreadSentinel = FALSE;
}

//
// bIMEModeMouseByWndThreadRoutine()
//
BOOL		CCursor::bIMEModeMouseByWndThreadRoutine(LPVOID lpvParam)
{
	if (lpvParam == NULL)	return FALSE;
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lpvParam;
	CCursor* This = reinterpret_cast<CCursor*>(lpvParam);

	POINT	pt{};
	RECT	rcMouse{};
	int		iMouse = 0;
	int		iMouseSizeX = 0, iMouseSizeY = 0;
	DWORD	dwIMEModeMouse = IMEOFF;
	DWORD	dwIMEModeMousePrev = IMEOFF;
	BOOL	bCapsLock = FALSE, _bCapsLock = FALSE;	

	lpstCursorData->bIMEModeByWindowThreadSentinel = TRUE;
	do {
		if (lpstCursorData->bDisplayIMEModeOnCursor && lpstCursorData->bDisplayIMEModeByWindow) {
			dwIMEModeMouse = Cime->dwIMEMode(lpstCursorData->hWndObserved, lpstCursorData->bForceHiragana);
			if ((dwIMEModeMouse == IMEHIDE) || (dwIMEModeMouse == IMEOFF)) {
				if (lpstCursorData->bDisplayIMEModeIMEOFF)  dwIMEModeMouse = IMEOFF;
				else dwIMEModeMouse = IMEHIDE;
			}
			iMouse = This->iGetCursorID(dwIMEModeMouse, lpstCursorData);
			This->MouseWindow->vSetModeStringAndColor(lpstCursorData->lpstFlushMouseCursor[iMouse].szMode, This->dwGetColorFromIMEMode(dwIMEModeMouse, 2));
			if (!GetCursorPos(&pt))	return FALSE;
			rcMouse.left = pt.x;	rcMouse.top = pt.y;
			rcMouse.right = pt.x + lpstCursorData->iModeSize;
			rcMouse.bottom = pt.y + lpstCursorData->iModeSize;
			if (!This->bAdjustModeSizeByMonitorDPI(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcMouse))	return FALSE;
			iMouseSizeX = (rcMouse.right - rcMouse.left); iMouseSizeY = (rcMouse.bottom - rcMouse.top);
			if ((dwIMEModeMouse == IMEOFF) || (dwIMEModeMouse == HANEISU_IMEON) || (dwIMEModeMouse == HANKANA_IMEON)) {
				iMouseSizeX = (iMouseSizeX * 2 + 2) / 3;	rcMouse.left = rcMouse.left + iMouseSizeX / 4;	rcMouse.right = rcMouse.left + iMouseSizeX;
			}
#define MERGIN_X	(-42)
#define MERGIN_Y	(-28)
			pt.x = pt.x + lpstCursorData->iModeSize + lpstCursorData->iIMEModeDistance + MERGIN_X;
			pt.y = pt.y + lpstCursorData->iModeSize + lpstCursorData->iIMEModeDistance + MERGIN_Y;
#undef MERGIN_X
#undef MERGIN_Y
			if (GetKeyState(VK_CAPITAL) & 0x0001) _bCapsLock = TRUE;	else _bCapsLock = FALSE;
			if (bCapsLock != _bCapsLock) {
				if (!This->MouseWindow->bInvalidateRect(NULL, FALSE))	return FALSE;
				if (!This->MouseWindow->bUpdateWindow())	return FALSE;
				bCapsLock = _bCapsLock;
			}
			if (dwIMEModeMouse != dwIMEModeMousePrev) {
				dwIMEModeMousePrev = dwIMEModeMouse;
				if (!This->MouseWindow->bSetWindowPos(HWND_BOTTOM, rcMouse.left, rcMouse.top, iMouseSizeX, iMouseSizeX, (SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS)))	return FALSE;
				Sleep(100);
			}
			if (!This->MouseWindow->bSetWindowPos(HWND_TOPMOST, pt.x, pt.y, iMouseSizeX, iMouseSizeY, (SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS)))	return FALSE;
			Sleep(0);
		}
		else Sleep(10);
	} while(lpstCursorData->bIMEModeByWindowThreadSentinel);
	if (!This->MouseWindow->bSetWindowPos(HWND_BOTTOM, rcMouse.left, rcMouse.top, iMouseSizeX, iMouseSizeX, (SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS)))	return FALSE;
	return TRUE;
}


//
// bRegisterDrawIMEModeThread()
//
BOOL		CCursor::bRegisterDrawIMEModeThread(HWND hWndObserved)
{
	stIMECursorData.hWndObserved = hWndObserved;
	if (!DrawIMEModeThread->bRegister(DRAWIMEMODETHREADNAME, DRAWIMEMODETHREADID,
							(LPTHREAD_START_ROUTINE)&bDrawIMEModeRoutine, &stIMECursorData, 0)) {
		return	FALSE;
	}
	return	TRUE;
}

//
// bDrawIMEModeRoutine()
//
BOOL WINAPI	CCursor::bDrawIMEModeRoutine(LPVOID lpvParam)
{
	if (lpvParam == NULL)	return FALSE;
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lpvParam;
	CCursor	*This = reinterpret_cast<CCursor*>(lpvParam);
	BOOL	bRet = TRUE;
	This->bIsIMECursorChanged(lpstCursorData);
	if (!This->bDrawIMEModeOnDisplay(lpstCursorData)) {
		_Post_equals_last_error_ DWORD err = GetLastError();
		if (err == ERROR_ACCESS_DENIED) {
			bRet = TRUE;
		}
		else {
			bRet = FALSE;
		}
	}
	return	bRet;
}

//
// bIMECursorChange()
//
BOOL		CCursor::bIsIMECursorChanged(LPIMECURSORDATA lpstCursorData)
{
	if (lpstCursorData == NULL)	return FALSE;
	
	DWORD	dwIMEMode = (DWORD)(-1);

	dwIMEMode = Cime->dwIMEMode(lpstCursorData->hWndObserved, lpstCursorData->bForceHiragana);
	if (lpstCursorData->dwIMEModeCursor == dwIMEMode) return FALSE;
	lpstCursorData->dwIMEModeCursor = dwIMEMode;
	return TRUE;
}

//
// bDrawIMEModeOnDisplay()
//
BOOL		CCursor::bDrawIMEModeOnDisplay(LPIMECURSORDATA lpstCursorData)
{
	if (lpstCursorData == NULL)	return FALSE;
	
	BOOL	bRet = FALSE;
	RECT	rc{};
	lpstCursorData->bIMECursorChangeThreadSentinel = TRUE;
	if (bCalcDisplayModeRect(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rc)) {
		if (EnumDisplayMonitors(NULL, &rc, (MONITORENUMPROC)&bIconDrawEnumProc, (LPARAM)lpstCursorData) != 0) {
			bRet = TRUE;
		}
		else {
			_Post_equals_last_error_ DWORD	err = GetLastError();
			if ((err == ERROR_INVALID_PARAMETER) || (err == ERROR_SUCCESS)) {
				bRet = TRUE;
			}
			else if (err == ERROR_ACCESS_DENIED) {
				bRet = FALSE;
			}
		}
	}
	else {
		_Post_equals_last_error_ DWORD err = GetLastError();
		if (err == ERROR_ACCESS_DENIED) {
			bRet = FALSE;
		}
	}
	return bRet;
}

//
// _bIconDrawEnumProc()
//
BOOL		CCursor::bIconDrawEnumProc(HMONITOR hMonitor, HDC hDC, LPCRECT lprcClip, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hMonitor);
	UNREFERENCED_PARAMETER(hDC);
	UNREFERENCED_PARAMETER(lprcClip);
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lParam;
	CCursor* This = reinterpret_cast<CCursor*>(lParam);
	lpstCursorData->bIMECursorChangeThreadSentinel = TRUE;
	return This->bDrawIMEModeOnDisplaySub(lpstCursorData);
}

//
// bCalcDisplayModeRect()
//
BOOL		CCursor::bCalcDisplayModeRect(int iModeSizeX, int iModeSizeY, LPRECT lpRect)
{
	POINT	pt{};
	if (GetCursorPos(&pt)) {
		HMONITOR hMonitor = NULL;
		if ((hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST)) != NULL) {
			UINT	dpiX = 0, dpiY = 0;
			if ((GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY)) == S_OK) {
				int iIconSizeX = 0, iIconSizeY = 0;
				iIconSizeX = iModeSizeX * (dpiX + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI + 1;
				iIconSizeY = iModeSizeY * (dpiY + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI + 1;
				lpRect->left = pt.x - ((iIconSizeX * 3 + 1) / 2);	lpRect->right = lpRect->left + iIconSizeX;
				lpRect->top = pt.y - ((iIconSizeY + 1) / 2);		lpRect->bottom = lpRect->top + iIconSizeY;
				return TRUE;
			}
		}
	}
	else {
		_Post_equals_last_error_ DWORD err = GetLastError();
		if (err == ERROR_ACCESS_DENIED) {
			SetLastError(err);
			return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}

//
// hGetCaretPosByAccessibleObjectFromWindow()
// 
HWND		CCursor::hGetCaretPosByAccessibleObjectFromWindow(HWND hForeWnd, LPIMECURSORDATA lpstCursorData, BOOL bAttachThreadInput)
{
	HWND	hWnd = NULL;
	POINT	pt{};
	DWORD	dwPID = 0, dwForeThreadID = 0;
	if ((hForeWnd != NULL) && (lpstCursorData != NULL)) {
		lpstCursorData->bIMECursorChangeThreadSentinel = TRUE;
		if ((dwForeThreadID = GetWindowThreadProcessId(hForeWnd, &dwPID)) != 0) {
			LPGUITHREADINFO	lpGuiThreadInfo = new GUITHREADINFO[sizeof(GUITHREADINFO)];
			if (lpGuiThreadInfo != NULL) {
				lpGuiThreadInfo->cbSize = sizeof(GUITHREADINFO);
				if (bAttachThreadInput) {
					if (!AttachThreadInput(dwForeThreadID, GetCurrentThreadId(), TRUE)) {
					}
				}
				if (GetGUIThreadInfo(dwForeThreadID, lpGuiThreadInfo)) {
					if (bAttachThreadInput) {
						if (!AttachThreadInput(dwForeThreadID, GetCurrentThreadId(), FALSE)) {
						}
					}
					if (lpstCursorData->bIMECursorChangeThreadSentinel) {
						if (lpGuiThreadInfo->hwndCaret == NULL) {
							if (lpGuiThreadInfo->hwndFocus != NULL) {
								IAccessible* IAccessible = NULL;
								HRESULT hResult = AccessibleObjectFromWindow(lpGuiThreadInfo->hwndFocus, (DWORD)OBJID_CARET, IID_IAccessible, (LPVOID*)&IAccessible);
								if (hResult == S_OK) {
									LPVARIANT	lpVariant = new VARIANT[sizeof(VARIANT)];
									if (lpVariant != NULL) {
										SIZE	sz{};
										lpVariant->vt = VT_I4;	lpVariant->lVal = CHILDID_SELF;
										if ((hResult = IAccessible->accLocation(&pt.x, &pt.y, &sz.cx, &sz.cy, *lpVariant)) == S_OK) {
											IAccessible->Release();
											lpstCursorData->rcCaret.left = pt.x;	lpstCursorData->rcCaret.top = pt.y;
											lpstCursorData->rcCaret.right = pt.x + sz.cx;	lpstCursorData->rcCaret.bottom = pt.y + sz.cy;
											if ((pt.x == 0) && (pt.y == 0) && (sz.cx == 0) && (sz.cy == 0)) {
												lpstCursorData->rcCaret.left = 0;	lpstCursorData->rcCaret.right = 0;
												lpstCursorData->rcCaret.top = 0;	lpstCursorData->rcCaret.bottom = 0;
												hWnd = NULL;
											}
											else {
												hWnd = lpGuiThreadInfo->hwndFocus;
											}
										}
										delete[]	lpVariant;
									}
								}
							}
						}
						else {
							pt.x = lpGuiThreadInfo->rcCaret.left;	pt.y = lpGuiThreadInfo->rcCaret.top;
							if (ClientToScreen(lpGuiThreadInfo->hwndCaret, &pt)) {
								lpstCursorData->rcCaret.left = pt.x;	lpstCursorData->rcCaret.top = pt.y;
								pt.x = lpGuiThreadInfo->rcCaret.right;	pt.y = lpGuiThreadInfo->rcCaret.bottom;
								if (ClientToScreen(lpGuiThreadInfo->hwndCaret, &pt)) {
									lpstCursorData->rcCaret.right = pt.x;	lpstCursorData->rcCaret.bottom = pt.y;
									RECT	rcTop{};
									if (GetWindowRect(lpGuiThreadInfo->hwndCaret, &rcTop)) {
#define	MARGIN		2
										if (((pt.x == 0) && (pt.y == 0)) || (pt.x <= rcTop.left + MARGIN) || (pt.y <= rcTop.top + MARGIN)) {
#undef MARGIN
											lpstCursorData->rcCaret.left = 0;	lpstCursorData->rcCaret.right = 0;	
											lpstCursorData->rcCaret.top = 0;	lpstCursorData->rcCaret.bottom = 0;
											hWnd = NULL;
										}
										else {
											hWnd = lpGuiThreadInfo->hwndCaret;
										}
									}
								}
							}
						}
					}
					else {
						lpstCursorData->rcCaret.left = 0;	lpstCursorData->rcCaret.right = 0;
						lpstCursorData->rcCaret.top = 0;	lpstCursorData->rcCaret.bottom = 0;
						hWnd = NULL;
					}
				}
				delete[]	lpGuiThreadInfo;
			}
		}
	}
	return hWnd;
}

//
// bAdjustModeSizeByMonitorDPI()
//
BOOL		CCursor::bAdjustModeSizeByMonitorDPI(int iModeSizeX, int iModeSizeY, LPRECT lpRect)
{
	POINT		pt{ lpRect->left, lpRect->bottom };
	HMONITOR	hMonitor = NULL;
	if ((hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST)) != NULL) {
		UINT	dpiX = 0, dpiY = 0;
		if ((GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY)) == S_OK) {
			int iIconSizeX = 0, iIconSizeY = 0;
			iIconSizeX = iModeSizeX * (dpiX + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI + 1;
			iIconSizeY = iModeSizeY * (dpiY + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI + 1;
#define DESCENT		4
			lpRect->left = pt.x - ((iIconSizeX * 5) / 4);
			lpRect->right = lpRect->left + iIconSizeX;
			if ((lpRect->bottom - lpRect->top) < iModeSizeY) {
				lpRect->top = (lpRect->bottom - iIconSizeY) + (DESCENT * (dpiY + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI);
				lpRect->bottom = lpRect->top + iIconSizeY;
			}
			else {
				lpRect->top = (lpRect->bottom - ((lpRect->bottom - lpRect->top) / 2 ) - (iIconSizeY / 2));
				lpRect->bottom = lpRect->top + iIconSizeY;
			}
#undef DESCENT
			return TRUE;
		}
	}
	return FALSE;
}

//
// bDrawIMEModeOnDisplaySub()
//
BOOL		CCursor::bDrawIMEModeOnDisplaySub(LPIMECURSORDATA lpstCursorData)
{
	if (lpstCursorData == NULL)	return FALSE;
	
	BOOL	bFoundCaret = FALSE;
	RECT	rcCursor{}, rcCaret{};
	int		iCursorSizeX = 0, iCursorSizeY = 0, iCaretSizeX = 0, iCaretSizeY = 0;
	DWORD	dwIMEModeCursor = IMEOFF, dwIMEModeCaret = IMEOFF;
	HWND	hCaretWnd = NULL;
	lpstCursorData->bIMECursorChangeThreadSentinel = TRUE;
	if (lpstCursorData->bDrawIMEModeWait) {
#define	COUNT	10
		int		iCount = 0;
		iCount = (lpstCursorData->dwWaitTime + (COUNT - 1)) / COUNT;
		for (; iCount >= 0; iCount--) {
			if (!lpstCursorData->bIMECursorChangeThreadSentinel)	return TRUE;
			Sleep(COUNT);
		}
	}
#undef COUNT
	if (lpstCursorData->bDrawNearCaret != FALSE) {
		if (lpstCursorData->hWndCaret != NULL) {
			rcCaret.left = lpstCursorData->rcCaret.left;	rcCaret.top = lpstCursorData->rcCaret.top;
			rcCaret.right = lpstCursorData->rcCaret.right;	rcCaret.bottom = lpstCursorData->rcCaret.bottom;
			hCaretWnd = lpstCursorData->hWndCaret;
			bFoundCaret = TRUE;
		}
		if (!lpstCursorData->bIMECursorChangeThreadSentinel)	return TRUE;
		if (bFoundCaret && (hCaretWnd != NULL)) {
			if (bAdjustModeSizeByMonitorDPI(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcCaret) != FALSE) {
				if (!lpstCursorData->bIMECursorChangeThreadSentinel)	return TRUE;
				if ((rcCaret.left == 0) && (rcCaret.top == 0) && (rcCaret.right == 0) && (rcCaret.bottom == 0)) {
					bFoundCaret = FALSE;
				}
				else {
					dwIMEModeCaret = Cime->dwIMEMode(hCaretWnd, lpstCursorData->bForceHiragana);
					int	iCaret = iGetCursorID(dwIMEModeCaret, lpstCursorData);
					if (iCaret == 0) dwIMEModeCaret = IMEOFF;
					CaretWindow->vSetModeStringAndColor(lpstCursorData->lpstFlushMouseCursor[iCaret].szMode, dwGetColorFromIMEMode(dwIMEModeCaret, 1));

					iCaretSizeX = rcCaret.right - rcCaret.left, iCaretSizeY = rcCaret.bottom - rcCaret.top;
#define MERGIN_X	1
#define MERGIN_Y	1
					rcCaret.left = rcCaret.left - MERGIN_X;
					rcCaret.top = rcCaret.top -MERGIN_Y;
#undef MERGIN_X
#undef MERGIN_Y
					if ((dwIMEModeCaret == IMEOFF) || (dwIMEModeCaret == HANEISU_IMEON) || (dwIMEModeCaret == HANKANA_IMEON)) {
						iCaretSizeX = (iCaretSizeX * 2 + 2) / 3;	rcCaret.left = rcCaret.left + iCaretSizeX / 4;	rcCaret.right = rcCaret.left + iCaretSizeX;
					}
					if (CaretWindow->bSetWindowPos(HWND_TOPMOST, rcCaret.left, rcCaret.top, iCaretSizeX, iCaretSizeY, (SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS))) {
						bFoundCaret = TRUE;
					}
				}
			}
			else {
				bFoundCaret = FALSE;
			}
		}
	}
	if (bFoundCaret == FALSE) {
		dwIMEModeCursor = Cime->dwIMEMode(lpstCursorData->hWndObserved, lpstCursorData->bForceHiragana);
	}
	else {
		dwIMEModeCursor = Cime->dwIMEMode(GetForegroundWindow(), lpstCursorData->bForceHiragana);
	}
	if (bFoundCaret == FALSE) {
		int	iCursor = iGetCursorID(dwIMEModeCursor, lpstCursorData);
		if (iCursor == 0) dwIMEModeCursor = IMEOFF;
		if ((iCursor == IMEHIDE) || (iCursor == IMEOFF)) {
			if (lpstCursorData->bDisplayIMEModeIMEOFF)  dwIMEModeCursor = IMEOFF;
			else dwIMEModeCursor = IMEHIDE;
		}
		CursorWindow->vSetModeStringAndColor(lpstCursorData->lpstFlushMouseCursor[iCursor].szMode, dwGetColorFromIMEMode(dwIMEModeCursor, 0));
	}

#define	COUNT	10
	BOOL	bRet = FALSE;
	int		iCount = 0;
	if (lpstCursorData->dwDisplayModeTime != 0)	iCount = (lpstCursorData->dwDisplayModeTime + (COUNT - 1)) / COUNT;
	else iCount = COUNT;
	for (; iCount >= 0; iCount--) {
		if (!lpstCursorData->bIMECursorChangeThreadSentinel) {
			bRet = TRUE;
			break;
		}
		if (bFoundCaret == FALSE) {
			if (bCalcDisplayModeRect(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcCursor)) {
				iCursorSizeX = rcCursor.right - rcCursor.left; iCursorSizeY = rcCursor.bottom - rcCursor.top;
				if ((dwIMEModeCursor == IMEOFF) || (dwIMEModeCursor == HANEISU_IMEON) || (dwIMEModeCursor == HANKANA_IMEON)) {
					iCursorSizeX = (iCursorSizeX * 2 + 2) / 3; rcCursor.left = rcCursor.right - iCursorSizeX;
				}
				if (!lpstCursorData->bIMECursorChangeThreadSentinel) {
					bRet = TRUE;
					break;
				}
				if (dwIMEModeCursor != IMEHIDE) {
					if (!CursorWindow->bSetWindowPos(HWND_TOPMOST, rcCursor.left, rcCursor.top, iCursorSizeX, iCursorSizeY, (SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS))) {
						bRet = FALSE;
						break;
					}
				}
			}
			else {
				bRet = FALSE;
				break;
			}
			if (!lpstCursorData->bIMECursorChangeThreadSentinel) {
				bRet = TRUE;
				break;
			}
		}
		Sleep(COUNT);
		bRet = TRUE;
	}
#undef COUNT
	try {
		bRet = CursorWindow->bSetWindowPos(HWND_BOTTOM, rcCursor.left, rcCursor.top, iCursorSizeX, iCursorSizeY, (SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS));
	}
	catch (...) {
	}
	CursorWindow->vSetModeStringAndColor(lpstCursorData->lpstFlushMouseCursor[IMEOFF].szMode, stIMECursorData.dwNearDrawMouseIMEOFFColor);
	if (bFoundCaret) {
		try {
			bRet = CaretWindow->bSetWindowPos(HWND_BOTTOM, rcCaret.left, rcCaret.top, iCaretSizeX, iCaretSizeY, (SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS));
		}
		catch (...) {
		}
		CaretWindow->vSetModeStringAndColor(lpstCursorData->lpstFlushMouseCursor[IMEOFF].szMode, stIMECursorData.dwNearDrawCaretIMEOFFColor);
	}
	return bRet;
}

//
// iGetCursorID()
//
int			CCursor::iGetCursorID(DWORD dwIMEMode, LPIMECURSORDATA lpstCursorData)
{
	int		i = 0;
	while (dwIMEMode != lpstCursorData->lpstFlushMouseCursor[i].dwIMEMode) {
		i++;
		if (lpstCursorData->lpstFlushMouseCursor[i].dwIMEMode == (DWORD)(-1)) {
			return 0;
		}
	}
	return i;
}

//
// dwGetColorFromIMEMode()
//
COLORREF	CCursor::dwGetColorFromIMEMode(DWORD dwIMEMode, int iWindow) const
{
	switch (dwIMEMode) {
	case IMEOFF:
		if (iWindow == 0)		return stIMECursorData.dwNearDrawMouseIMEOFFColor;
		else if (iWindow == 1)	return stIMECursorData.dwNearDrawCaretIMEOFFColor;
		else if (iWindow == 2)	return stIMECursorData.dwNearDrawMouseByWndIMEOFFColor;
		break;
	case HANEISU_IMEON:
		if (iWindow == 0)		return stIMECursorData.dwNearDrawMouseHANEISU_IMEONColor;
		else if (iWindow == 1)	return stIMECursorData.dwNearDrawCaretHANEISU_IMEONColor;
		else if (iWindow == 2)	return stIMECursorData.dwNearDrawMouseByWndHANEISU_IMEONColor;
		break;
	case HANKANA_IMEON:
		if (iWindow == 0)		return stIMECursorData.dwNearDrawMouseHANKANA_IMEONColor;
		else if (iWindow == 1)	return stIMECursorData.dwNearDrawCaretHANKANA_IMEONColor;
		else if (iWindow == 2)	return stIMECursorData.dwNearDrawMouseByWndHANKANA_IMEONColor;
		break;
	case ZENEISU_IMEON:
		if (iWindow == 0)		return stIMECursorData.dwNearDrawMouseZENEISU_IMEONColor;
		else if (iWindow == 1)	return stIMECursorData.dwNearDrawCaretZENEISU_IMEONColor;
		else if (iWindow == 2)	return stIMECursorData.dwNearDrawMouseByWndZENEISU_IMEONColor;
		break;
	case ZENHIRA_IMEON:
		if (iWindow == 0)		return stIMECursorData.dwNearDrawMouseZENHIRA_IMEONColor;
		else if (iWindow == 1)	return stIMECursorData.dwNearDrawCaretZENHIRA_IMEONColor;
		else if (iWindow == 2)	return stIMECursorData.dwNearDrawMouseByWndZENHIRA_IMEONColor;
		break;
	case ZENKANA_IMEON:
		if (iWindow == 0)		return stIMECursorData.dwNearDrawMouseZENKANA_IMEONColor;
		else if (iWindow == 1)	return stIMECursorData.dwNearDrawCaretZENKANA_IMEONColor;
		else if (iWindow == 2)	return stIMECursorData.dwNearDrawMouseByWndZENKANA_IMEONColor;
		break;
	}
	return (COLORREF)0;
}

//
// bGetMouseRegValue()
//
BOOL		CCursor::bGetMouseRegValue(LPCTSTR szValue, LPTSTR szFile)
{
#define MOUSE_HKEY		HKEY_CURRENT_USER
#define MOUSE_SUBKEY	_T("Control Panel\\Cursors")
	BOOL		bRet = FALSE;
	CRegistry	*CReg = new CRegistry;
	if (CReg->bReadRegValueString(MOUSE_HKEY, MOUSE_SUBKEY, szValue, szFile, _MAX_PATH)) {
		bRet = TRUE;
	}
	else {
	}
	delete	CReg;
	return bRet;
}

//
// bChangeFlushMouseCursor()
//
BOOL		CCursor::bChangeFlushMouseCursor(UINT uCurID, LPIMECURSORDATA lpstCursorData)
{
	if (lpstCursorData == NULL)	return FALSE;
	int	i = 0;
	while (uCurID != lpstCursorData->lpstFlushMouseCursor[i].dwIMEMode) {
		++i;
	}
	if (bSetSystemCursor((LPMOUSECURSOR)&(lpstCursorData->lpstFlushMouseCursor[i]).stArrow, lpstCursorData->iCursorSize, lpstCursorData->iCursorSize)) {
		if (bSetSystemCursor((LPMOUSECURSOR)&(lpstCursorData->lpstFlushMouseCursor[i]).stHand, lpstCursorData->iCursorSize, lpstCursorData->iCursorSize)) {
			if (bSetSystemCursor((LPMOUSECURSOR)&(lpstCursorData->lpstFlushMouseCursor[i]).stIBeam, lpstCursorData->iCursorSize, lpstCursorData->iCursorSize)) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

//
// bSetSystemCursor()
//
BOOL		CCursor::bSetSystemCursor(LPMOUSECURSOR lpstMC, int iCursorSizeX, int iCursorSizeY)
{
	if (lpstMC == NULL)	return FALSE;
	
	HCURSOR	hCur;
	UINT	fuLoad = 0;
	if (lpstMC->bReadReg) {
		if (lpstMC->szFile[0] == '\0')	return TRUE;
		fuLoad = (LR_VGACOLOR | LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		if ((hCur = (HCURSOR)LoadImage(NULL, lpstMC->szFile, IMAGE_CURSOR, iCursorSizeX, iCursorSizeY, fuLoad)) == NULL) {
			return FALSE;
		}
	} else {
		if (lpstMC->uResourceID == 0) {
			if (lpstMC->szFile[0] == '\0')		return FALSE;
			DWORD	dwSize = 0;
			dwSize = ExpandEnvironmentStrings(lpstMC->szFile, NULL, 0);
			LPTSTR	lpszBuffer = new TCHAR[dwSize];
			if (lpszBuffer) {
				ZeroMemory(lpszBuffer, dwSize);
				dwSize = ExpandEnvironmentStrings(lpstMC->szFile, lpszBuffer, dwSize);
				fuLoad = (LR_VGACOLOR | LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
				if ((hCur = (HCURSOR)LoadImage(NULL, lpszBuffer, IMAGE_CURSOR, iCursorSizeX, iCursorSizeY, fuLoad)) == NULL) {
					delete[]	lpszBuffer;
					return FALSE;
				}
				delete[]	lpszBuffer;
			}
			else return FALSE;
		} else {
			HMODULE	hMod = NULL;
			if ((hMod = hCursorDllLoad()) == NULL)	return FALSE;
			fuLoad = (LR_VGACOLOR | LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
			if ((hCur = (HCURSOR)LoadImage((HINSTANCE)hMod, MAKEINTRESOURCE(lpstMC->uResourceID),
															IMAGE_CURSOR, iCursorSizeX, iCursorSizeY, fuLoad)) == NULL) {
				_Post_equals_last_error_ DWORD err = GetLastError();
				if (err == ERROR_MOD_NOT_FOUND) {
					bReportEvent(MSG_RESTART_FLUSHMOUSE_EVENT, APPLICATION_CATEGORY);
				}
				if (!bCursorDllUnload()) return FALSE;
				return FALSE;
			}
			// Unload Cursor data DLL
			if (!bCursorDllUnload()) return FALSE;
		}
	}
	if (!SetSystemCursor(hCur, lpstMC->id))		return FALSE;
	DestroyCursor(hCur);
	return TRUE;
}

//
//class CCursorWindow : public CWindow
//
CCursorWindow::CCursorWindow()
{
	lpszIMEMode = NULL;
	dwTextColor = aRGB(15, 254, 192, 0);
	dwBackColor = aRGB(15, 254, 192, 0);
}

CCursorWindow::~CCursorWindow()
{
	lpszIMEMode = NULL;
}

//
// bRegister()
//
BOOL		CCursorWindow::bRegister(HINSTANCE hInstance, LPCTSTR szWindowClassName)
{
	if (hInstance == NULL)	return FALSE;

#define CLASSSTYLE (CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE)
	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CLASSSTYLE;	
	wcex.lpfnWndProc = NULL;
	wcex.cbClsExtra = 0;	
	wcex.cbWndExtra = 0;	
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClassName;
	wcex.hIconSm = NULL;	

	if (!bRegisterClassEx(&wcex))	return FALSE;

#define		EX_WINDOWSTYLE	(WS_EX_LAYERED | WS_EX_NOACTIVATE)
#define		WINDOWSTYLE		(WS_POPUP)
	if (!bCreateWindowEx(EX_WINDOWSTYLE, NULL, WINDOWSTYLE, 0, 0, 0, 0, NULL, NULL)) return FALSE;
	if (hGetHWND() == NULL)	return FALSE;
	if (!bShowWindow(SW_HIDE))	return FALSE;
	if (!bUpdateWindow())	return FALSE;
	return TRUE;
}

//
// vSetModeStringAndColor()
// 
VOID		CCursorWindow::vSetModeStringAndColor(LPCTSTR _lpszIMEMode, COLORREF dwRGB)
{
	lpszIMEMode = (LPTSTR)_lpszIMEMode;
	dwTextColor = dwRGB;
	if (dwRGB & 0x00000100)	dwBackColor = dwRGB & 0xfffffeff;
	else dwBackColor = dwRGB | 0x00000100;
}

//
// WndProc()
//
LRESULT CALLBACK CCursorWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_PAINT, Cls_OnPaint);
		default:
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// WM_CREATE
// Cls_OnCreate()
//
BOOL		CCursorWindow::Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) const
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lpCreateStruct);
	return SetLayeredWindowAttributes(hWnd, dwBackColor, (dwBackColor >> 24) & 0x000000ff, LWA_COLORKEY | LWA_ALPHA);
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
void		CCursorWindow::Cls_OnDestroy(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);
}

//
// WM_PAINT
// Cls_OnPaint()
//
void		CCursorWindow::Cls_OnPaint(HWND hWnd)
{
	PAINTSTRUCT	ps{};
	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC != NULL) {
		if (SetLayeredWindowAttributes(hGetHWND(), dwBackColor, (dwBackColor >> 24) & 0x000000ff, LWA_COLORKEY | LWA_ALPHA)) {
			RECT	rc{};
			if (GetClientRect(hWnd, &rc)) {
				HBRUSH	hBrush = NULL;
				if ((hBrush = CreateSolidBrush(dwBackColor & 0x00ffffff)) != NULL) {
					if (SelectObject(hDC, hBrush) != HGDI_ERROR) {
						if (FillRect(hDC, &rc, hBrush) != 0) {
							if (SetBkMode(hDC, TRANSPARENT) != 0) {
								COLORREF	dwColorPrev = 0;
								if ((dwColorPrev = SetTextColor(hDC, dwTextColor & 0x80ffffff)) != CLR_INVALID) {
									int	iBkModePrev = 0;
									if ((iBkModePrev = SetBkMode(hDC, TRANSPARENT)) != 0) {
										HFONT	hFont = NULL;
										DWORD	dwUnderline = 0;
										if (GetKeyState(VK_CAPITAL) & 0x0001) dwUnderline = TRUE;	else dwUnderline = FALSE;
#define	FONT	_T("Yu Gothic UI")
										if ((hFont = CreateFont((rc.bottom - rc.top), (rc.right - rc.left) / 2, 0, 0, FW_BOLD, FALSE, dwUnderline, FALSE,
															SHIFTJIS_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, FONT)) != NULL) {
											HFONT	hFontPrev = NULL;
											if ((hFontPrev = (HFONT)SelectObject(hDC, hFont)) != NULL) {
												if (lpszIMEMode != NULL) {
													if (DrawTextEx(hDC, lpszIMEMode, -1, &rc, DT_RIGHT | DT_SINGLELINE | DT_VCENTER, NULL) == 0) {
													}
												}
											}
											DeleteObject(hFont);
										}
										SetBkMode(hDC, iBkModePrev);
									}
									SetTextColor(hDC, dwColorPrev);
								}
							}
						}
					}
					DeleteObject(hBrush);
				}
			}
			DeleteObject(hDC);
		}
	}
	EndPaint(hWnd, &ps);
	return;
}

/* = EOF = */