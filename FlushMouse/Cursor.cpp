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
#include "..\FlushMouseCursor\Resource.h"
#include "..\MiscLIB\CRegistry.h"

//
// Library
//
#pragma comment(lib, "Imm32.lib")

//
// Define
//
#ifndef OCR_NORMAL
#define OCR_NORMAL  32512		// IDC_ARROW	 (WinUser.h)
#endif // !OCR_NORMAL
#ifndef OCR_IBEAM
#define OCR_IBEAM   32513		// IDC_IBEAM	 (WinUser.h)
#endif // !OCR_IBEAM
#ifndef OCR_HAND
#define OCR_HAND    32649		// IDC_HAND (WinUser.h)
#endif // !OCR_HAND
#ifndef OCR_HELP
#define	OCR_HELP	32651		// IDC_HELP (WinUser.h)
#endif // !OCR_HELP
#ifndef OCR_PIN
#define OCR_PIN		32671		// IDC_PIN (WinUser.h)
#endif // !OCR_PIN
#ifndef OCR_PERSON
#define	OCR_PERSON	32672		// IDC_PERSON (WinUser.h)	
#endif // !OCR_PERSON

//
// Struct Define
//

//
// Class Define

//
// Local Data
//

#define FLUSHMOUSECURSORDIR	_T("\\JeffyTS\\FlushMouse\\")	// Cursor data dir
#define FLUSHMOUSECURSORDAT	_T("FlushMouseCursor.dat")		// Cursor data Name

// IME Cursor Change Thread
#define IMECURSORCHANGETHREAD	1							// IME Cursor Change THread ID
static DWORD			dwIMECursorChangeThreadID = IMECURSORCHANGETHREAD;
static TCHAR			szIMECursorChangeThreadName[] = _T("IMECursorChangeThread");

// Draw IME Mode near by Mouse cursor Thread
#define	DRAWIMEMODETHREAD		2							// Draw IME Mode near by Mouse cursor Thead ID
static DWORD			dwDrawIMEModeThreadID = DRAWIMEMODETHREAD;
static TCHAR			szDrawIMEModeThreadName[] = _T("DrawIMEModeThread");

// Draw IME Mode near by Caret cursor Thread
#define	DRAWIMEMODECARETTHREAD		3						// Draw IME Mode near by Caret cursor Thead ID
static DWORD			dwDrawIMEModeCaretThreadID = DRAWIMEMODECARETTHREAD;
static TCHAR			szDrawIMEModeCaretThreadName[] = _T("DrawIMEModeCaretThread");

static MOUSECURSOR	stAllMouseCursor[] = {
		{ OCR_APPSTARTING, 0, TRUE,  _T("AppStarting"), _T("") },
		{ OCR_NORMAL,	   0, FALSE, _T("Arrow"),		_T("") },
		{ OCR_CROSS,	   0, TRUE,  _T("Crosshair"),	_T("") },
		{ OCR_HAND,		   0, FALSE, _T("Hand"),	    _T("") },
		{ OCR_HELP,		   0, TRUE,  _T("Help"),	    _T("") },
		{ OCR_IBEAM,	   0, FALSE, _T("IBeam"),		_T("") },
		{ OCR_NO,		   0, TRUE,  _T("No"),			_T("") },
	 // { OCR_NWPEN,	   0, TRUE,  _T("NWPen"),		_T("") },
	 // { OCR_PEN,		   0, TRUE,  _T("Pen"),		    _T("") },
		{ OCR_PERSON,	   0, TRUE,  _T("Person"),		_T("") },
		{ OCR_PIN,		   0, TRUE,  _T("Pin"),		    _T("") },
		{ OCR_SIZEALL,	   0, TRUE,  _T("SizeAll"),	    _T("") },
		{ OCR_SIZENESW,	   0, TRUE,  _T("SizeNESW"),    _T("") },
		{ OCR_SIZENS,	   0, TRUE,  _T("SizeNS"),		_T("") },
		{ OCR_SIZENWSE,	   0, TRUE,  _T("SizeNWSE"),    _T("") },
		{ OCR_SIZEWE,	   0, TRUE,  _T("SizeWE"),		_T("") },
		{ OCR_UP,		   0, TRUE,  _T("UpArrow"),	    _T("") },
		{ OCR_WAIT,		   0, TRUE,  _T("Wait"),	    _T("") },
		{ (DWORD)(-1), 	   0, FALSE, _T(""),		    _T("") }		// Terminater
};

static FLUSHMOUSECURSOR	stFlushMouseCursor[] = {
		{ IMEOFF,			_T("A"),	{OCR_NORMAL, IDC_IMEOFF_ARROW,		  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeOffArrow.cur")},
										{OCR_HAND,   IDC_IMEOFF_HAND,		  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeOffHand.cur")},
										{OCR_IBEAM,  IDC_IMEOFF_IBEAM,		  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeOffIbeam.cur")}},
		{ ZENHIRA_IMEON,	_T("あ"),	{OCR_NORMAL, IDC_ZENHIRA_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenHiraOnArrow.cur")},
										{OCR_HAND,   IDC_ZENHIRA_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenHiraOnHand.cur")},
										{OCR_IBEAM,  IDC_ZENHIRA_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenHiraOnIbeam.cur")}},
		{ HANEISU_IMEON,	_T("_A"),	{OCR_NORMAL, IDC_HANEISU_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanEisuOnArrow.cur")},
										{OCR_HAND,   IDC_HANEISU_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanEisuOnHand.cur")},
										{OCR_IBEAM,  IDC_HANEISU_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanEisuOnIbeam.cur")}},
		{ HANKANA_IMEON,	_T("_ｱ"),	{OCR_NORMAL, IDC_HANKANA_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanKanaOnArrow.cur")},
										{OCR_HAND,   IDC_HANKANA_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanKanaOnHand.cur")},
										{OCR_IBEAM,  IDC_HANKANA_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanKanaOnIbeam.cur")}},
		{ ZENEISU_IMEON,	_T("Ａ"),	{OCR_NORMAL, IDC_ZENEISU_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenEisuOnArrow.cur")},
										{OCR_HAND,   IDC_ZENEISU_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenEisuOnHand.cur")},
										{OCR_IBEAM,  IDC_ZENEISU_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenEisuOnIbeam.cur")}},
		{ ZENKANA_IMEON,	_T("ア"),	{OCR_NORMAL, IDC_ZENKANA_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenKanaOnArrow.cur")},
										{OCR_HAND,   IDC_ZENKANA_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenKanaOnHand.cur")},
										{OCR_IBEAM,  IDC_ZENKANA_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenKanaOnIbeam.cur")}},
		{ (UINT)(-1),		_T(""),		{0,			 0,						  FALSE, _T(""), _T("")},		// Terminater
										{0,			 0,						  FALSE, _T(""), _T("")},		// Terminater
										{0,			 0,						  FALSE, _T(""), _T("")} }		// Terminater
};

//
// class CCursor
//
CCursor::CCursor()
{
	this->hMainWnd = NULL;

	// DLL data の初期化
	hModDll = NULL;
	iCursorDataLoadCount = 0;

	stIMECursorData.hWndObserved = NULL;
	stIMECursorData.dwIMEState = (UINT)(-1);			// IMEの状態(初期処理でチェックするように-1を入れる)
	stIMECursorData.bDrawIMEModeWait = FALSE;
	stIMECursorData.bDrawNearCaret = FALSE;
	stIMECursorData.lpszLoadDatName = NULL;
	stIMECursorData.lpstFlushMouseCursor = (LPFLUSHMOUSECURSOR)&stFlushMouseCursor;

	IMECursorChangeThread = NULL;
	CursorWindow = NULL;
	DrawIMEModeThread = NULL;
	CaretWindow = NULL;
	DrawIMEModeCaretThread = NULL;
}

CCursor::~CCursor()
{
	if (DrawIMEModeCaretThread != NULL) {
		delete	DrawIMEModeCaretThread;				// Draw IME Mode near by Caret cursor Threadの後始末
	}
	if (CaretWindow != NULL) {
		delete	CaretWindow;						// Draw Window near by Caret cursorの削除
	}
	if (DrawIMEModeThread != NULL) {
		delete	DrawIMEModeThread;					// Draw IME Mode near by Mouse cursor Threadの後始末
	}
	if (CursorWindow != NULL) {
		delete	CursorWindow;						// Draw Window near by Mouse cursorの削除
	}
	if (IMECursorChangeThread != NULL) {
		delete	IMECursorChangeThread;				// IME Cursor Change Threadの後始末
	}
	if (stIMECursorData.lpszLoadDatName != NULL)	delete []	stIMECursorData.lpszLoadDatName;
}

//
// bInitialize()
//
BOOL			CCursor::bInitialize(HWND hWnd)
{
	this->hMainWnd = hWnd;

	// from Regitry
	stIMECursorData.iCursorSize = Profile->stAppRegData.iCursorSize;
	stIMECursorData.iModeSize = Profile->stAppRegData.iModeSize;
	stIMECursorData.dwInThreadSleepTime = Profile->stAppRegData.dwInThreadSleepTime;
	stIMECursorData.dwWaitWaveTime = Profile->stAppRegData.dwWaitWaveTime;
	stIMECursorData.dwDisplayModeTime = Profile->stAppRegData.dwDisplayModeTime;
	stIMECursorData.bDisplayFocusWindowIME = Profile->stAppRegData.bDisplayFocusWindowIME;
	stIMECursorData.bDisplayIMEModeOnCursor = Profile->stAppRegData.bDisplayIMEModeOnCursor;
	stIMECursorData.bForceHiragana = Profile->stAppRegData.bForceHiragana;
	stIMECursorData.bDrawNearCaret = Profile->stAppRegData.bDrawNearCaret;
	stIMECursorData.dwNearDrawMouseColor = Profile->stAppRegData.dwNearDrawMouseColor;
	stIMECursorData.dwNearDrawCaretColor = Profile->stAppRegData.dwNearDrawCaretColor;

	stIMECursorData.bDenyChangedByApp = Profile->stAppRegData.bDenyChangedByApp;
	stIMECursorData.bUseBigArrow = Profile->stAppRegData.bUseBigArrow;

	if ((stIMECursorData.lpszLoadDatName = lpszGetCursorDataName()) == NULL)		return FALSE;
	if (hCursorDllLoad() == NULL)	return FALSE;
	if (!bSystemCursorLoad())	return FALSE;

	IMECursorChangeThread = new CThread;
	if (!bChangeFlushMouseCursor(IMEOFF, &stIMECursorData)) return FALSE;
	if (!bRegisterIMECursorChangeThread(hWnd)) return FALSE;
	
#define	WINDOWCLASS		_T("CursorWindow")
	CursorWindow = new CCursorWindow;
	if (!CursorWindow->bRegister((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), WINDOWCLASS, stIMECursorData.dwNearDrawMouseColor))		return FALSE;
	CursorWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
	DrawIMEModeThread = new CThread;
#undef WINDOWCLASS
#define	WINDOWCLASS		_T("CaretWindow")
	CaretWindow = new CCursorWindow;
	if (!CaretWindow->bRegister((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), WINDOWCLASS, stIMECursorData.dwNearDrawCaretColor))	return FALSE;
	CaretWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
	DrawIMEModeCaretThread = new CThread;
#undef WINDOWCLASS

	if (!bRegisterDrawIMEModeThread(hWnd)) return FALSE;

	return TRUE;
}

//
// lpszGetCursorDataName()
//
LPTSTR		CCursor::lpszGetCursorDataName()
{
	LPTSTR	lpszCursorDataName = NULL;
	LPTSTR	lpszBuffer = new TCHAR[_MAX_PATH];
	ZeroMemory(lpszBuffer, _MAX_PATH);
	DWORD	dwSize = 0;
	GetCurrentDirectory(_MAX_PATH, lpszBuffer);
	_tcsncat_s(lpszBuffer, _MAX_PATH, _T("\\"), sizeof(_T("\\")));
	_tcsncat_s(lpszBuffer, _MAX_PATH, FLUSHMOUSECURSORDAT, sizeof(FLUSHMOUSECURSORDAT));
	dwSize = ExpandEnvironmentStrings(lpszBuffer, NULL, 0);
	lpszCursorDataName = new TCHAR[dwSize];
	ZeroMemory(lpszCursorDataName, dwSize);
	dwSize = ExpandEnvironmentStrings(lpszBuffer, lpszCursorDataName, dwSize);
	if (GetFileAttributes(lpszCursorDataName) == INVALID_FILE_ATTRIBUTES) {					// Try Current Dir
		delete []	lpszCursorDataName;
		ZeroMemory(lpszBuffer, _MAX_PATH);
		_tcsncat_s(lpszBuffer, _MAX_PATH, _T("%APPDATA%"), sizeof(_T("%APPDATA%")));
		_tcsncat_s(lpszBuffer, _MAX_PATH, FLUSHMOUSECURSORDIR, sizeof(FLUSHMOUSECURSORDIR));
		_tcsncat_s(lpszBuffer, _MAX_PATH, FLUSHMOUSECURSORDAT, sizeof(FLUSHMOUSECURSORDAT));
		dwSize = ExpandEnvironmentStrings(lpszBuffer, NULL, 0);
		lpszCursorDataName = new TCHAR[dwSize];
		ZeroMemory(lpszCursorDataName, dwSize);
		dwSize = ExpandEnvironmentStrings(lpszBuffer, lpszCursorDataName, dwSize);
		if (GetFileAttributes(lpszCursorDataName) == INVALID_FILE_ATTRIBUTES) {				// Try %APPDATA% Dir
			delete []	lpszCursorDataName;
			ZeroMemory(lpszBuffer, _MAX_PATH);
			_tcsncat_s(lpszBuffer, _MAX_PATH, _T("%ALLUSERSPROFILE%"), sizeof(_T("%ALLUSERSPROFILE%")));
			_tcsncat_s(lpszBuffer, _MAX_PATH, FLUSHMOUSECURSORDIR, sizeof(FLUSHMOUSECURSORDIR));
			_tcsncat_s(lpszBuffer, _MAX_PATH, FLUSHMOUSECURSORDAT, sizeof(FLUSHMOUSECURSORDAT));
			dwSize = ExpandEnvironmentStrings(lpszBuffer, NULL, 0);
			lpszCursorDataName = new TCHAR[dwSize];
			ZeroMemory(lpszCursorDataName, dwSize);
			dwSize = ExpandEnvironmentStrings(lpszBuffer, lpszCursorDataName, dwSize);
			if (GetFileAttributes(lpszCursorDataName) == INVALID_FILE_ATTRIBUTES) {			// Try %ALLUSERSPROFILE% Dir
				delete []	lpszBuffer;
				delete []	lpszCursorDataName;
				return NULL;
			}
		}
	}
	delete []	lpszBuffer;
	return lpszCursorDataName;
}

//
// bIsIMEOpen()
//
BOOL			CCursor::bIsIMEOpen(HWND hWndObserved)
{
	HWND    hIMWnd = NULL;
	if ((hIMWnd = ImmGetDefaultIMEWnd(hWndObserved)) != NULL) {
		if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETOPENSTATUS, (LPARAM)NULL) != 0) {
			return TRUE;			// IME Open
		}
	}
	return FALSE;					// IME Close
}

//
// vIMEOpenCloseForced()
//
VOID		CCursor::vIMEOpenCloseForced(HWND hWndObserved, DWORD dwIMEOpenClose)
{
	if (hWndObserved == NULL)	return;
	LPARAM	lParam = (LPARAM)dwIMEOpenClose;
	EnumChildWindows(hWndObserved, &bEnumChildProcIMEOpenClose, lParam);
	return;
}

//
// vIMEConvertModeChangeForced()
//
VOID		CCursor::vIMEConvertModeChangeForced(HWND hWndObserved, DWORD dwConvertMode)
{
	if (hWndObserved == NULL)	return;
	LPARAM	lParam = (LPARAM)dwConvertMode;
	EnumChildWindows(hWndObserved, &bEnumChildProcIMECnvertMode, lParam);
	return;
}

//
// bEnumChildProcIMEOpenClose()
//
BOOL CALLBACK CCursor::bEnumChildProcIMEOpenClose(HWND hWndObserved, LPARAM lParam)
{
	if (hWndObserved == NULL)	return FALSE;
	HWND    hIMWnd = NULL;
	if ((hIMWnd = ImmGetDefaultIMEWnd(hWndObserved)) != NULL) {
		if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETOPENSTATUS, lParam) == 0) {	// lParam = FALSE to IME CLOSE / TRUE to IME OPEN
		}
	}
	return TRUE;
}

//
// bEnumChildProcIMECnvertMode()
//
BOOL CALLBACK CCursor::bEnumChildProcIMECnvertMode(HWND hWndObserved, LPARAM lParam)
{
	if (hWndObserved == NULL)	return FALSE;
	HWND    hIMWnd = NULL;
	if ((hIMWnd = ImmGetDefaultIMEWnd(hWndObserved)) != NULL) {
		if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETOPENSTATUS, (LPARAM)TRUE) == 0) {	// lParam = FALSE to IME CLOSE / TRUE to IME OPEN
			if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETCONVERSIONMODE, (LPARAM)lParam) == 0) {
			}
		}
	}
	return TRUE;
}

//
// bStartIMECursorChangeThread()
//
BOOL		CCursor::bStartIMECursorChangeThread(HWND hWndObserved)
{
	stIMECursorData.hWndObserved = hWndObserved;
	if (!IMECursorChangeThread->bStart()) {
		if (IMECursorChangeThread != NULL) {
			delete	IMECursorChangeThread;
		}
		IMECursorChangeThread = new CThread;
		if (!bRegisterIMECursorChangeThread(this->hMainWnd)) return FALSE;
	}
	return TRUE;
}

//
// bStartDrawIMEModeThread()
//
BOOL		CCursor::bStartDrawIMEModeThread(HWND hWndObserved)
{
	stIMECursorData.hWndObserved = hWndObserved;
	stIMECursorData.bDrawIMEModeWait = FALSE;
	stIMECursorData.dwWaitWaveTime = Profile->stAppRegData.dwWaitWaveTime;
	if (!DrawIMEModeThread->bStart()) {
		if (DrawIMEModeThread != NULL) {
			delete	DrawIMEModeThread;
		}
		DrawIMEModeThread = new CThread;
		if (!bRegisterDrawIMEModeThread(this->hMainWnd)) return FALSE;
	}
	return TRUE;
}

//
// bStartDrawIMEModeThreadWait()
//
BOOL		CCursor::bStartDrawIMEModeThreadWait(HWND hWndObserved)
{
	stIMECursorData.hWndObserved = hWndObserved;
	stIMECursorData.bDrawIMEModeWait = TRUE;
	stIMECursorData.dwWaitWaveTime = Profile->stAppRegData.dwWaitWaveTime;
	if (!DrawIMEModeThread->bStart()) {
		if (DrawIMEModeThread != NULL) {
			delete	DrawIMEModeThread;
		}
		DrawIMEModeThread = new CThread;
		if (!bRegisterDrawIMEModeThread(this->hMainWnd)) return FALSE;
	}
	return TRUE;
}

//
// bStartDrawIMEModeThreadWaitDblClk()
//
BOOL		CCursor::bStartDrawIMEModeThreadWaitDblClk(HWND hWndObserved)
{
	stIMECursorData.hWndObserved = hWndObserved;
	stIMECursorData.bDrawIMEModeWait = TRUE;
	stIMECursorData.dwWaitWaveTime = GetDoubleClickTime();
	if (!DrawIMEModeThread->bStart()) {
		if (DrawIMEModeThread != NULL) {
			delete	DrawIMEModeThread;
		}
		DrawIMEModeThread = new CThread;
		if (!bRegisterDrawIMEModeThread(this->hMainWnd)) return FALSE;
	}
	return TRUE;
}

//
// hCursorDllLoad()
// 
HMODULE		CCursor::hCursorDllLoad()
{
	if ((hModDll == NULL) && (iCursorDataLoadCount == 0)) {
		if ((hModDll = LoadLibraryEx(stIMECursorData.lpszLoadDatName, NULL, (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE))) == NULL) {
			return NULL;
		}
	}
	++iCursorDataLoadCount;
	return hModDll;
}

//
// bCursorDllUnload()
//
BOOL		CCursor::bCursorDllUnload()
{
	if ((hModDll != NULL) && (iCursorDataLoadCount == 1)) {
		if (!FreeLibrary(hModDll)) {
			return FALSE;
		}
	}
	--iCursorDataLoadCount;
	return TRUE;
}

//
// bSystemCursorLoad()
//
BOOL		CCursor::bSystemCursorLoad()
{
	// Set Flush Mouse Cursor to System
	if (!bChangeFlushMouseCursor(IMEOFF, &stIMECursorData))		return FALSE;
	
	// Set Other Mouse Cursor to System
	int	i = 0;
	while (stAllMouseCursor[i].id != (DWORD)(-1)) {
		if (stAllMouseCursor[i].bReadReg) {
			// Get Other System Cursor Filename from Registry
			if (!bGetMouseRegValue(stAllMouseCursor[i].szRegValue, stAllMouseCursor[i].szFile))		return FALSE;
		}
		i++;
	}
	return TRUE;
}

//
// bRegisterIMECursorChangeThread()
//
BOOL		CCursor::bRegisterIMECursorChangeThread(HWND hWndObserved)
{
	stIMECursorData.hWndObserved = hWndObserved;
	if (!IMECursorChangeThread->bRegister(szIMECursorChangeThreadName, dwIMECursorChangeThreadID,
							(LPTHREAD_START_ROUTINE)&bIMECursorChangeRoutine, &stIMECursorData, stIMECursorData.dwInThreadSleepTime)) {
			return	FALSE;
	}
	return	TRUE;
}

//
// bIMECursorChangeRoutine()
//
BOOL		CCursor::bIMECursorChangeRoutine(LPVOID lpvParam)
{
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lpvParam;
	CCursor* This = reinterpret_cast<CCursor*>(lpvParam);
	BOOL		bRet = TRUE;
	if (lpstCursorData->bDisplayIMEModeOnCursor) {
		This->bIsIMECursorChanged(lpstCursorData);
		if (!This->bChangeFlushMouseCursor(lpstCursorData->dwIMEState, lpstCursorData)) {
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
// bRegisterDrawIMEModeThread()
//
BOOL		CCursor::bRegisterDrawIMEModeThread(HWND hWndObserved)
{
	// Register thread
	stIMECursorData.hWndObserved = hWndObserved;
	if (!DrawIMEModeThread->bRegister(szDrawIMEModeThreadName, dwDrawIMEModeThreadID,
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
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lpvParam;
	CCursor* This = reinterpret_cast<CCursor*>(lpvParam);
	This->bIsIMECursorChanged(lpstCursorData);
	if (!This->bDrawIMEModeOnDisplay(lpstCursorData)) {
		return FALSE;
	}
	return	TRUE;
}

//
// bIMECursorChange()
//
BOOL		CCursor::bIsIMECursorChanged(LPIMECURSORDATA lpstCursorData)
{
	UINT		dwIMEState = (UINT)(-1);

	dwIMEState = dwIMECursorMode(lpstCursorData->hWndObserved, lpstCursorData->bForceHiragana);
	if (lpstCursorData->dwIMEState == dwIMEState) return FALSE;
	lpstCursorData->dwIMEState = dwIMEState;
	return TRUE;
}

//
// dwIMECursorMode()
//
DWORD		CCursor::dwIMECursorMode(HWND hWndObserved, BOOL bForceHiragana)
{
	if (hWndObserved != NULL) {
		HWND    hIMWnd = NULL;
		DWORD   dwConvertMode = 0;
		if ((hIMWnd = ImmGetDefaultIMEWnd(hWndObserved)) != NULL) {
			if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETOPENSTATUS, NULL) != 0) {
				if ((dwConvertMode = (DWORD)SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETCONVERSIONMODE, NULL)) != 0) {
					switch (dwConvertMode) {
					case ZENHIRA_IMEON:
					case (ZENHIRA_IMEON ^ IME_CMODE_ROMAN):
						dwConvertMode = ZENHIRA_IMEON;
						break;
					case HANEISU_IMEON:
					case (HANEISU_IMEON ^ IME_CMODE_ROMAN):
						dwConvertMode = HANEISU_IMEON;
						break;
					case HANKANA_IMEON:
					case (HANKANA_IMEON ^ IME_CMODE_ROMAN):
						dwConvertMode = HANKANA_IMEON;
						break;
					case ZENEISU_IMEON:
					case (ZENEISU_IMEON ^ IME_CMODE_ROMAN):
						dwConvertMode = ZENEISU_IMEON;
						break;
					case ZENKANA_IMEON:
					case (ZENKANA_IMEON ^ IME_CMODE_ROMAN):
						dwConvertMode = ZENKANA_IMEON;
						break;
					default:
						dwConvertMode = IMEOFF;
					}
					if ((bForceHiragana != FALSE) && (dwConvertMode != ZENHIRA_IMEON)) {
						vIMEConvertModeChangeForced(hWndObserved, ZENHIRA_IMEON);
						return ZENHIRA_IMEON;
					}
					return dwConvertMode;
				}
			}
		}
	}
	return IMEOFF;
}

//
// bDrawIMEModeOnDisplay()
//
BOOL		CCursor::bDrawIMEModeOnDisplay(LPIMECURSORDATA lpstCursorData)
{
	BOOL		bRet = FALSE;
	RECT		rc{};
	if (lpstCursorData->bDrawIMEModeWait) {
		if (lpstCursorData->dwWaitWaveTime != 0)	Sleep(lpstCursorData->dwWaitWaveTime);
	}
	if (bCalcDispModeRect(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rc)) {
		if (EnumDisplayMonitors(NULL, &rc, (MONITORENUMPROC)&bIconDrawEnumProc, (LPARAM)lpstCursorData)) {
			bRet = TRUE;
		}
		else {
			_Post_equals_last_error_ DWORD	err = GetLastError();
			if ((err == ERROR_INVALID_PARAMETER) || (err == ERROR_SUCCESS)) {
				bRet = TRUE;
			}
		}
	}
	return bRet;
}

//
// bIconDrawEnumProc()
//
BOOL		CCursor::bIconDrawEnumProc(HDC hDC, LPCRECT lprcClip, MONITORENUMPROC lpfnEnum, LPARAM dwData)
{
	UNREFERENCED_PARAMETER(hDC);
	UNREFERENCED_PARAMETER(lprcClip);
	UNREFERENCED_PARAMETER(lpfnEnum);
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)dwData;
	CCursor* This = reinterpret_cast<CCursor*>(dwData);
	BOOL		bRet = FALSE;
	bRet = This->bDrawIMEModeOnDisplaySub(lpstCursorData);
	return bRet;
}

//
// bCalcDispModeRect()
//
BOOL		CCursor::bCalcDispModeRect(int iModeSizeX, int iModeSizeY, LPRECT lpRect)
{
	POINT	pt{};
	RECT	rect{};
	if (GetCursorPos(&pt)) {
		HMONITOR hMonitor = NULL;
		if ((hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST)) != NULL) {
			UINT		dpiX = 0, dpiY = 0;
			if ((GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY)) == S_OK) {
				int iIconSizeX = 0, iIconSizeY = 0;
				iIconSizeX = iModeSizeX * (dpiX + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI + 1;
				iIconSizeY = iModeSizeY * (dpiX + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI + 1;
				// Draw near by Mouse cursor Left
				lpRect->left = pt.x - ((iIconSizeX * 3 + 1) / 2);	lpRect->right = lpRect->left + iIconSizeX;
				lpRect->top = pt.y - ((iIconSizeY + 1) / 2);		lpRect->bottom = lpRect->top + iIconSizeY;
				return TRUE;
			}
		}
	}
	return FALSE;
}

//
// bCalcDispModeCaretRect()
//
BOOL		CCursor::bCalcDispModeCaretRect(int iModeSizeX, int iModeSizeY, LPRECT lpRect, LPPOINT lpPt)
{
	POINT	pt{};
	RECT	rcFore{}, rcTop{};
	HWND	hForeWnd = NULL;
	DWORD	dwPID = 0, dwForeThreadID = 0;
	if ((hForeWnd = GetForegroundWindow()) != NULL) {
		if (GetWindowRect(hForeWnd, &rcFore)) {
			if ((dwForeThreadID = GetWindowThreadProcessId(hForeWnd, &dwPID)) != 0) {
				GUITHREADINFO	GuiThredInfo{};	GuiThredInfo.cbSize = sizeof(GUITHREADINFO);
				if (GetGUIThreadInfo(dwForeThreadID, &GuiThredInfo)) {
					if (GuiThredInfo.hwndCaret != NULL) {
#define DESCENT		2
						pt.x = GuiThredInfo.rcCaret.left;	pt.y = GuiThredInfo.rcCaret.top;
#undef DESCEN
						if (ClientToScreen(GuiThredInfo.hwndCaret, &pt)) {
							lpPt->x = pt.x;	lpPt->y = pt.y;
							pt.x = GuiThredInfo.rcCaret.right;	pt.y = GuiThredInfo.rcCaret.bottom;
							if (ClientToScreen(GuiThredInfo.hwndCaret, &pt)) {
								pt.x = lpPt->x;	pt.y = lpPt->y + (pt.y - lpPt->y) / 2;

								if (GetWindowRect(GuiThredInfo.hwndCaret, &rcTop)) {
									lpPt->x = pt.x;	lpPt->y = pt.y;
#define	MARGIN		2
									if (((pt.x == 0) && (pt.y == 0)) || (pt.x <= rcTop.left + MARGIN) || (pt.y <= rcTop.top + MARGIN) || (pt.x <= rcFore.left) || (pt.y <= rcFore.top)) {
										lpRect->left = 0;		lpRect->right = 0;
										lpRect->top = 0;		lpRect->bottom = 0;
										return TRUE;
									}
									else {
										HMONITOR hMonitor = NULL;
										if ((hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST)) != NULL) {
											UINT	dpiX = 0, dpiY = 0;
											if ((GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY)) == S_OK) {
												HDC	hDC = NULL;
												if ((hDC = GetDC(GuiThredInfo.hwndCaret)) != NULL) {
													int iIconSizeX = 0, iIconSizeY = 0;
													iIconSizeX = iModeSizeX * (dpiX + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI + 1;
													iIconSizeY = iModeSizeY * (dpiY + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI + 1;
#define DESCENT		2
													// Draw near by Caret cursor Left
													lpRect->left = pt.x - ((iIconSizeX * 5) / 4);
													lpRect->right = lpRect->left + iIconSizeX;
													lpRect->top = pt.y - (DESCENT * (dpiY + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI) - (iIconSizeY / 2);
													lpRect->bottom = lpRect->top + iIconSizeY;
													if (ReleaseDC(GuiThredInfo.hwndCaret, hDC)) {
														return TRUE;
													}
												}
											}
										}
									}
								}
							}
						}
					}
					else {
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

//
// bDrawIMEModeOnDisplaySub()
//
BOOL		CCursor::bDrawIMEModeOnDisplaySub(LPIMECURSORDATA lpstCursorData)
{
#define	COUNT	20
	Sleep(50);
	BOOL	bRet = FALSE;
	BOOL	bFoundCaret = FALSE;
	RECT	rcCursor{}, rcCaret{};
	int		iCursorSizeX = 0, iCursorSizeY = 0, iCaretSizeX = 0, iCaretSizeY = 0;
	DWORD	dwIMEState = dwIMECursorMode(lpstCursorData->hWndObserved, lpstCursorData->bForceHiragana);
	if (dwIMEState != IMEOFF) {
		int		i = 0;
		while (dwIMEState != lpstCursorData->lpstFlushMouseCursor[i].dwIMEMode) {
			++i;
		}
		CursorWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[i].szMode);
	}
	if (lpstCursorData->bDrawNearCaret != FALSE) {
		POINT	pt{};
		if (GetCursorPos(&pt)) {
			HWND	hWnd = WindowFromPoint(pt);
			if ((!lpstCursorData->bDisplayFocusWindowIME) || ((hWnd != NULL) && (hWnd == GetForegroundWindow()))) {
				if (bCalcDispModeCaretRect(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcCaret, &pt)) {
					hWnd = WindowFromPoint(pt);
					UINT	uIMEStateCaret = dwIMECursorMode(hWnd, lpstCursorData->bForceHiragana);
					int		i = 0;
					while (uIMEStateCaret != lpstCursorData->lpstFlushMouseCursor[i].dwIMEMode) {
						++i;
					}
					if ((rcCaret.left != 0) || (rcCaret.top != 0) || (rcCaret.right != 0) || (rcCaret.bottom != 0)) {		// Determin Caret position
						CaretWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[i].szMode);
						iCaretSizeX = rcCaret.right - rcCaret.left, iCaretSizeY = rcCaret.bottom - rcCaret.top;
						if ((uIMEStateCaret == IMEOFF) || (uIMEStateCaret == HANEISU_IMEON) || (uIMEStateCaret == HANKANA_IMEON)) {
							// Draw near by Caret cursor Left
							iCaretSizeX = (iCaretSizeX * 2 + 2) / 3;	rcCaret.left = rcCaret.left + iCaretSizeX / 4;	rcCaret.right = rcCaret.left + iCaretSizeX;
						}
						bFoundCaret = TRUE;
					}
					else {
						bFoundCaret = FALSE;
					}
				}
			}
		}
	}
	//
	int	iCount = 0;
	if (lpstCursorData->dwDisplayModeTime != 0)	iCount = (lpstCursorData->dwDisplayModeTime + (COUNT - 1)) / COUNT;
	else iCount = COUNT;															// default count
	//
	if (bFoundCaret != FALSE) {
		if (!CaretWindow->bSetWindowPos(HWND_TOPMOST, rcCaret.left, rcCaret.top, iCaretSizeX, iCaretSizeY, (SWP_SHOWWINDOW | SWP_NOACTIVATE))) {
			bRet = FALSE;
		}
	}
	//
	for (; iCount >= 0; iCount--) {
		if (bFoundCaret == FALSE) {
			if (bCalcDispModeRect(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcCursor)) {
				iCursorSizeX = rcCursor.right - rcCursor.left; iCursorSizeY = rcCursor.bottom - rcCursor.top;
				if ((dwIMEState == IMEOFF) || (dwIMEState == HANEISU_IMEON) || (dwIMEState == HANKANA_IMEON)) {
					iCursorSizeX = (iCursorSizeX * 2 + 2) / 3; rcCursor.left = rcCursor.right - iCursorSizeX;
				}
				if (dwIMEState != IMEOFF) {
					if (!CursorWindow->bSetWindowPos(HWND_TOPMOST, rcCursor.left, rcCursor.top, iCursorSizeX, iCursorSizeY, (SWP_SHOWWINDOW | SWP_NOACTIVATE))) {
						bRet = FALSE;
						break;
					}
				}
			}
			else {
				bRet = FALSE;
				break;
			}
		}
		Sleep(COUNT);
		bRet = TRUE;
	}
	if (!CursorWindow->bSetWindowPos(HWND_BOTTOM, rcCursor.left, rcCursor.top, iCursorSizeX, iCursorSizeY, (SWP_HIDEWINDOW | SWP_NOACTIVATE))) {
		bRet = FALSE;
	}
	CursorWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
	if (bFoundCaret) {
		if (!CaretWindow->bSetWindowPos(HWND_BOTTOM, rcCaret.left, rcCaret.top, iCaretSizeX, iCaretSizeY, (SWP_HIDEWINDOW | SWP_NOACTIVATE))) {
			bRet = FALSE;
		}
		CaretWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
	}
	return bRet;
}

//
// bGetMouseRegValue()
//
BOOL		CCursor::bGetMouseRegValue(LPCTSTR szValue, LPTSTR szFile)
{
// \HKEY_CURRENT_USER\Control Panel\Cursors
#define MOUSE_HKEY		HKEY_CURRENT_USER
#define MOUSE_SUBKEY	_T("Control Panel\\Cursors")
	BOOL		bRet = FALSE;
	CRegistry* CReg = new CRegistry;
	if (CReg->bGetRegValueString(MOUSE_HKEY, MOUSE_SUBKEY, szValue, szFile, _MAX_PATH)) {
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
	int		i = 0;
	while (uCurID != lpstCursorData->lpstFlushMouseCursor[i].dwIMEMode) {
		++i;
	}
	if (bSetSystemCursor((LPMOUSECURSOR)&(lpstCursorData->lpstFlushMouseCursor[i]).stArrow, lpstCursorData->iCursorSize, lpstCursorData->iCursorSize)) {
		if (bSetSystemCursor((LPMOUSECURSOR) & (lpstCursorData->lpstFlushMouseCursor[i]).stHand, lpstCursorData->iCursorSize, lpstCursorData->iCursorSize)) {
			if (bSetSystemCursor((LPMOUSECURSOR) & (lpstCursorData->lpstFlushMouseCursor[i]).stIbeam, lpstCursorData->iCursorSize, lpstCursorData->iCursorSize)) {
				return TRUE;				// Success
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
	HCURSOR		hCur;
	UINT			fuLoad = 0;
	if (lpstMC->bReadReg) {										// Load Cursor from registry
		if (lpstMC->szFile[0] == '\0')		return TRUE;
		fuLoad = (LR_VGACOLOR | LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		if ((hCur = (HCURSOR)LoadImage(NULL, lpstMC->szFile, IMAGE_CURSOR, iCursorSizeX, iCursorSizeY, fuLoad)) == NULL) {
			return FALSE;
		}
	} else {
		if (lpstMC->uResourceID == 0) {							// Load Cursor from file
			if (lpstMC->szFile[0] == '\0')		return FALSE;
			DWORD	dwSize = 0;
			dwSize = ExpandEnvironmentStrings(lpstMC->szFile, NULL, 0);
			LPTSTR	lpszBuffer = new TCHAR[dwSize];
			ZeroMemory(lpszBuffer, dwSize);
			dwSize = ExpandEnvironmentStrings(lpstMC->szFile, lpszBuffer, dwSize);
			fuLoad = (LR_VGACOLOR | LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			if ((hCur = (HCURSOR)LoadImage(NULL, lpszBuffer, IMAGE_CURSOR, iCursorSizeX, iCursorSizeY, fuLoad)) == NULL) {
				delete	[]	lpszBuffer;
				return FALSE;
			}
			delete[]	lpszBuffer;
		} else {												// Load Cursor from DLL
			// Load Cursoor data DLL
			HMODULE	hMod = NULL;
			if ((hMod = hCursorDllLoad()) == NULL)	return FALSE;
			fuLoad = (LR_VGACOLOR | LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
			if ((hCur = (HCURSOR)LoadImage((HINSTANCE)hMod, MAKEINTRESOURCE(lpstMC->uResourceID),
															IMAGE_CURSOR, iCursorSizeX, iCursorSizeY, fuLoad)) == NULL) {
				// Unload Cursoor data DLL
				if (!bCursorDllUnload()) return FALSE;
				return FALSE;
			}
			// Unload Cursoor data DLL
			if (!bCursorDllUnload()) return FALSE;
		}
	}
	if (!SetSystemCursor(hCur, lpstMC->id))		return FALSE;
	DestroyCursor(hCur);
	return TRUE;
}

//
//class CCursorWindow
//
CCursorWindow::CCursorWindow()
{
	lpszWindowClass = NULL;
	hCursorWindow = NULL;
	lpszMode = NULL;
	dwTextColor = aRGB(240, 254, 192, 0);
	dwBackColor = aRGB(240, 254, 192, 0);
}

CCursorWindow::~CCursorWindow()
{
	if (hCursorWindow != NULL)	 {
		DestroyWindow(hCursorWindow);
		UnregisterClass(lpszWindowClass, (HINSTANCE)GetWindowLongPtr(hCursorWindow, GWLP_HINSTANCE));
	}
	if (lpszWindowClass != NULL)		delete []	lpszWindowClass;
}

//
// bRegister()
//
BOOL		CCursorWindow::bRegister(HINSTANCE hInstance, LPCTSTR szWindowClassName, COLORREF dwRGB)
{
	if (hInstance == NULL)	return FALSE;
	lpszWindowClass = new TCHAR[MAX_LOADSTRING];
	ZeroMemory(lpszWindowClass, MAX_LOADSTRING);
	_tcsncpy_s(lpszWindowClass, MAX_LOADSTRING, szWindowClassName, _TRUNCATE);

	dwTextColor = dwRGB;
	if (dwRGB & 0x00000100)	dwBackColor = dwRGB & 0xfffffeff;
	else dwBackColor = dwRGB | 0x00000100;

	if (!MyRegisterClass(hInstance)) {
		return FALSE;
	}
	if ((hCursorWindow = InitInstance(hInstance, SW_HIDE)) == NULL) {
		return FALSE;
	}
	return TRUE;
}

//
// bSetWindowPos()
// 
BOOL		CCursorWindow::bSetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
{
	if (!SetWindowPos(hCursorWindow, hWndInsertAfter, X, Y, cx, cy, uFlags)) {
		return FALSE;
	}
	return TRUE;
}

//
// bSetModeString()
// 
VOID		CCursorWindow::vSetModeString(LPTSTR lpszIMEMode)
{
	lpszMode = lpszIMEMode;
}

//
// CursorWindowRegisterClass()
//
ATOM CCursorWindow::MyRegisterClass(HINSTANCE hInstance)
{
#define CLASSSTYLE (CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE)
	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CLASSSTYLE;						// クラススタイル
	wcex.lpfnWndProc = WndProc;						// Window Proc.
	wcex.cbClsExtra = 0;							// クラスの補足データなし
	wcex.cbWndExtra = 0;							// ウィンドウの補足データなし
	wcex.hInstance = hInstance;						// クラスのウィンドウハンドル
	wcex.hIcon = NULL;								// アイコンハンドル
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);		// マウスカーソルハンドル
	wcex.hbrBackground = NULL;						// ウィンドウ背景色
	wcex.lpszMenuName = NULL;						// デフォルトメニュー名
	wcex.lpszClassName = lpszWindowClass;			// このウインドウクラスにつける名前
	wcex.hIconSm = NULL;							// 16×16の小さいサイズのアイコン

	ATOM		atom = 0;
	if ((atom = RegisterClassEx(&wcex)) == 0) {
		_Post_equals_last_error_ DWORD err = GetLastError();
		if (err == 1410)		return 1;			// non error
	}
	return atom;
}

//
// hCreateCursorWindow()
//
HWND CCursorWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
#define		EX_WINDOWSTYLE	(WS_EX_LAYERED | WS_EX_NOACTIVATE)
#define		WINDOWSTYLE		(WS_POPUP)

	HWND		hWnd = NULL;						// ウィンドウのハンドル
	hWnd = CreateWindowEx(
					EX_WINDOWSTYLE,					// 拡張ウィンドウスタイル
					lpszWindowClass,				// RegisterClass()呼び出しを参照
					NULL,							// Title barのテキスト
					WINDOWSTYLE,					// Window style
					0, 0,							// 水平・垂直位置
					0, 0,							// 幅・高さ
					NULL,							// 親オーバーラップウィンドウ
					NULL,							// ウィンドウクラスのメニューを使う
					hInstance,						// 所有インスタンス
					this);							// 自分のポインタを渡す
	if (!hWnd) {
		return NULL;								// ウィンドウを作成できなかったときはNULLを返す
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return hWnd;
}

//
// WndProc()
//
LRESULT CALLBACK CCursorWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CCursorWindow* This = reinterpret_cast<CCursorWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (This == NULL) {
		if (message == WM_CREATE) {
			LPCREATESTRUCT lpCreate = reinterpret_cast<LPCREATESTRUCT>(lParam);
			This = reinterpret_cast<CCursorWindow*>(lpCreate->lpCreateParams);
			if (This != NULL) {
				SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)This);
				return This->_WndProc(hWnd, message, wParam, lParam);
			}
		}
	}
	else {
		return This->_WndProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// CursorWindowProc()
//
LRESULT CALLBACK CCursorWindow::_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
BOOL		CCursorWindow::Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lpCreateStruct);
	return SetLayeredWindowAttributes(hWnd, dwBackColor, (dwBackColor >> 24) & 0x000000ff, LWA_COLORKEY);
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
void		CCursorWindow::Cls_OnDestroy(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);
	//PostQuitMessage(0);
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
		RECT		rc{};
		if (GetClientRect(hWnd, &rc)) {
			HBRUSH	hBrush = NULL;
			if ((hBrush = CreateSolidBrush(dwBackColor & 0x00ffffff)) != NULL) {								// 背景色を設定
				if (SelectObject(hDC, hBrush) != HGDI_ERROR) {
					if (FillRect(hDC, &rc, hBrush) != 0) {
						if (SetBkMode(hDC, TRANSPARENT) != 0) {
							COLORREF	dwColorPrev = 0;
							if ((dwColorPrev = SetTextColor(hDC, dwTextColor & 0x00ffffff)) != CLR_INVALID) {	// 文字色を設定
								int	iBkModePrev = 0;
								if ((iBkModePrev = SetBkMode(hDC, TRANSPARENT)) != 0) {							// 背景モードを設定
									if (lpszMode != NULL) {
										HFONT	hFont = NULL;
										DWORD	dwUnderline = 0;
										if (GetKeyState(VK_CAPITAL) & 0x0001) dwUnderline = TRUE;	else dwUnderline = FALSE;
#define	FONT	_T("Yu Gothic UI")
										if ((hFont = CreateFont((rc.bottom - rc.top), (rc.right - rc.left) / 2, 0, 0, FW_BOLD, FALSE, dwUnderline, FALSE,
											SHIFTJIS_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
											PROOF_QUALITY, FIXED_PITCH, FONT)) != NULL) {
											HFONT	hFontPrev = NULL;
											if ((hFontPrev = (HFONT)SelectObject(hDC, hFont)) != NULL) {
												if (DrawTextEx(hDC, lpszMode, -1, &rc, DT_RIGHT | DT_SINGLELINE | DT_VCENTER, NULL) == 0) {
												}
												SelectObject(hDC, hFontPrev);
											}
											DeleteObject(hFont);
										}
										SetBkMode(hDC, iBkModePrev);
									}
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
	EndPaint(hWnd, &ps);
	return;
}

