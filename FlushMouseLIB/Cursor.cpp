//
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
	DrawIMEModeMouseThread = NULL;

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
	vUnRegisterDrawIMEModeMouseThread();
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
		if (!CursorWindow->bRegister((HINSTANCE)GetWindowLongPtr(hMainWnd, GWLP_HINSTANCE), WINDOWCLASS, stIMECursorData.dwNearDrawMouseColor))		return FALSE;
		CursorWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
		CursorWindow->vSetTextColor(stIMECursorData.dwNearDrawMouseColor);
	}
	else {
		CursorWindow->vSetTextColor(stIMECursorData.dwNearDrawMouseColor);
	}
#undef WINDOWCLASS
#define	WINDOWCLASS		_T("FlushMouseCaretWindow-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}")
	if (CaretWindow == NULL) {
		CaretWindow = new CCursorWindow;
		if (CaretWindow == NULL)	return FALSE;
		if (!CaretWindow->bRegister((HINSTANCE)GetWindowLongPtr(hMainWnd, GWLP_HINSTANCE), WINDOWCLASS, stIMECursorData.dwNearDrawCaretColor))	return FALSE;
		CaretWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
		CaretWindow->vSetTextColor(stIMECursorData.dwNearDrawCaretColor);
	}
	else {
		CaretWindow->vSetTextColor(stIMECursorData.dwNearDrawCaretColor);
	}
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
	if (stIMECursorData.bDisplayIMEModeByWindow) {
		vUnRegisterIMECursorChangeThread();
		if (!bRegisterDrawIMEModeMouseThread(hMainWnd))	return FALSE;
		SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);			// System Cursorに戻す
	}
	else {
		vStopDrawIMEModeMouseThread();
		vUnRegisterDrawIMEModeMouseThread();
		if (!bRegisterIMECursorChangeThread(hMainWnd)) return FALSE;
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
		stIMECursorData.iModeSize = Profile->lpstAppRegData->iModeSize;
		stIMECursorData.dwInThreadSleepTime = Profile->lpstAppRegData->dwInThreadSleepTime;
		stIMECursorData.dwDisplayModeTime = Profile->lpstAppRegData->dwDisplayModeTime;
		stIMECursorData.bDisplayFocusWindowIME = Profile->lpstAppRegData->bDisplayFocusWindowIME;
		stIMECursorData.bDisplayIMEModeOnCursor = Profile->lpstAppRegData->bDisplayIMEModeOnCursor;
		stIMECursorData.bDisplayIMEModeByWindow = Profile->lpstAppRegData->bDisplayIMEModeByWindow;
		stIMECursorData.bForceHiragana = Profile->lpstAppRegData->bForceHiragana;
		stIMECursorData.bDrawNearCaret = Profile->lpstAppRegData->bDrawNearCaret;
		stIMECursorData.dwNearDrawMouseColor = (~Profile->lpstAppRegData->dwNearDrawMouseColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseColor & 0x00ffffff);
		stIMECursorData.dwNearDrawCaretColor = (~Profile->lpstAppRegData->dwNearDrawCaretColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretColor & 0x00ffffff);
		stIMECursorData.dwNearMouseColor = (~Profile->lpstAppRegData->dwNearMouseColor & 0xff000000) | (Profile->lpstAppRegData->dwNearMouseColor & 0x00ffffff);

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
	if (&stIMECursorData == NULL)	return FALSE;
	vSetParamFromRegistry();
	stIMECursorData.hWndObserved = hWndObserved;
	if (Profile->lpstAppRegData->bDisplayIMEModeByWindow) {
		if (!bStartDrawIMEModeMouseThread(hWndObserved))	return FALSE;
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
	stIMECursorData.dwWaitTime = GetDoubleClickTime() +  Profile->lpstAppRegData->dwAdditionalWaitTime;
	return bStartDrawIMEModeThreadSub(hWndObserved);
}

//
// bStartDrawIMEModeThreadSub()
//
BOOL		CCursor::bStartDrawIMEModeThreadSub(HWND hWndObserved)
{
	if ((Cursor == NULL) || (DrawIMEModeThread == NULL))		return FALSE;
	stIMECursorData.hWndObserved = hWndObserved;
	stIMECursorData.hWndCaret = hGetCaretPosByAccessibleObjectFromWindow(hWndObserved, &stIMECursorData.rcCaret, FALSE);
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
	return TRUE;
}

//
// bSystemCursorLoad()
//
BOOL		CCursor::bSystemCursorLoad()
{
	if (&stIMECursorData == NULL)	return FALSE;
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
	if (&stIMECursorData == NULL)	return FALSE;
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
// bRegisterDrawIMEModeMouseThread()
//
BOOL		CCursor::bRegisterDrawIMEModeMouseThread(HWND hWnd)
{
#define	WINDOWCLASS		_T("FlushMouseMouseWindow-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}")
	if (MouseWindow == NULL) {
		MouseWindow = new CCursorWindow;
		if (MouseWindow == NULL)	return FALSE;
		if (!MouseWindow->bRegister((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), WINDOWCLASS, stIMECursorData.dwNearMouseColor))	return FALSE;
		MouseWindow->vSetTextColor(stIMECursorData.dwNearMouseColor);
		MouseWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
	}
	else {
		MouseWindow->vSetTextColor(stIMECursorData.dwNearMouseColor);
	}
#undef WINDOWCLASS
	if (DrawIMEModeMouseThread == NULL) {
		DrawIMEModeMouseThread = new CThread;
		if (DrawIMEModeMouseThread == NULL)	return FALSE;
		stIMECursorData.hWndObserved = hWnd;
		if (!DrawIMEModeMouseThread->bRegister(DRAWIMEMODEMOUSETHREADNAME, DRAWIMEMODEMOUSETHREADID,
			(LPTHREAD_START_ROUTINE)&bIMEModeMouseThreadRoutine, &stIMECursorData, stIMECursorData.dwInThreadSleepTime)) {
			return	FALSE;
		}
	}
	if (!bRegisterIMECursorChangeThread(hMainWnd)) return FALSE;
	return	TRUE;
}

//
// vUnRegisterDrawIMEModeMouseThread()
//
VOID		CCursor::vUnRegisterDrawIMEModeMouseThread()
{
	vStopDrawIMEModeMouseThread();
	if (DrawIMEModeMouseThread != NULL) {
		delete	DrawIMEModeMouseThread;
		DrawIMEModeMouseThread = NULL;
	}
	if (MouseWindow != NULL) {
		MouseWindow->bShowWindow(SW_HIDE);
		delete	MouseWindow;
		MouseWindow = NULL;
	}
}

//
// bStartDrawIMEModeMouseThread()
//
BOOL		CCursor::bStartDrawIMEModeMouseThread(HWND hWndObserved)
{
	if ((Cursor == NULL) || (DrawIMEModeMouseThread == NULL))	return FALSE;
	if (&stIMECursorData == NULL)	return FALSE;
	if (stIMECursorData.bIMECursorChangeThreadSentinel != FALSE)	return TRUE;
	stIMECursorData.hWndObserved = hWndObserved;
	if (!DrawIMEModeMouseThread->bStart()) {
		if (DrawIMEModeMouseThread != NULL) {
			delete	DrawIMEModeMouseThread;
			DrawIMEModeMouseThread = NULL;
		}
		DrawIMEModeMouseThread = new CThread;
		if (!bRegisterDrawIMEModeMouseThread(this->hMainWnd)) return FALSE;
	}
	return TRUE;
}

//
// bStopDrawIMEModeMouseThread()
//
VOID		CCursor::vStopDrawIMEModeMouseThread()
{
	if (MouseWindow != NULL)		MouseWindow->bShowWindow(SW_HIDE);
	if (&stIMECursorData != NULL)	stIMECursorData.bIMECursorChangeThreadSentinel = FALSE;
}

//
// bIMEModeMouseThreadRoutine()
//
BOOL		CCursor::bIMEModeMouseThreadRoutine(LPVOID lpvParam)
{
	if (lpvParam == NULL)	return FALSE;
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lpvParam;
	CCursor* This = reinterpret_cast<CCursor*>(lpvParam);

	POINT	pt{};
	RECT	rcMouse{};
	int		iMouse = 0;
	int		iMouseSizeX = 0, iMouseSizeY = 0;
	DWORD	dwIMEModeMouse = IMEOFF;
	BOOL	bCapsLock = FALSE, _bCapsLock = FALSE;	

	lpstCursorData->bIMECursorChangeThreadSentinel = TRUE;
	do {
		if (lpstCursorData->bDisplayIMEModeOnCursor && lpstCursorData->bDisplayIMEModeByWindow) {
			dwIMEModeMouse = Cime->dwIMEMode(lpstCursorData->hWndObserved, lpstCursorData->bForceHiragana);
			if (!This->bIsIMECursorChanged(lpstCursorData)) {							// IMEモードの変更あるなしにかかわらず処理する
				iMouse = This->iGetCurosrID(dwIMEModeMouse, lpstCursorData);
				if (iMouse == 0) dwIMEModeMouse = IMEOFF;
				This->MouseWindow->vSetModeString(lpstCursorData->lpstFlushMouseCursor[iMouse].szMode);
				if (!GetCursorPos(&pt))	return FALSE;
				rcMouse.left = pt.x;	rcMouse.top = pt.y;
				rcMouse.right = pt.x + lpstCursorData->iModeSize;
				rcMouse.bottom = pt.y + lpstCursorData->iModeSize;
				if (!This->bAdjustModeSizeByMonitorDPI(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcMouse))	return FALSE;
				iMouseSizeX = (rcMouse.right - rcMouse.left); iMouseSizeY = (rcMouse.bottom - rcMouse.top);
				if ((dwIMEModeMouse == IMEOFF) || (dwIMEModeMouse == HANEISU_IMEON) || (dwIMEModeMouse == HANKANA_IMEON)) {
					iMouseSizeX = (iMouseSizeX * 2 + 2) / 3;	rcMouse.left = rcMouse.left + iMouseSizeX / 4;	rcMouse.right = rcMouse.left + iMouseSizeX;
				}
			}
#define MERGIN_X	(-42)
#define MERGIN_Y	(-28)
			pt.x = pt.x + lpstCursorData->iModeSize + lpstCursorData->iCursorSize + MERGIN_X;
			pt.y = pt.y + lpstCursorData->iModeSize + lpstCursorData->iCursorSize + MERGIN_Y;
#undef MERGIN_X
#undef MERGIN_Y
			if (GetKeyState(VK_CAPITAL) & 0x0001) _bCapsLock = TRUE;	else _bCapsLock = FALSE;
			if (bCapsLock != _bCapsLock) {
				if (!This->MouseWindow->bInvalidateRect(NULL, FALSE))	return FALSE;
				if (!This->MouseWindow->bUpdateWindow())	return FALSE;
				bCapsLock = _bCapsLock;
			}
			if (!This->MouseWindow->bSetWindowPos(HWND_TOPMOST, pt.x, pt.y, iMouseSizeX, iMouseSizeY, (SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS)))	return FALSE;
			Sleep(1);
		}
	} while(lpstCursorData->bIMECursorChangeThreadSentinel);
	if (!This->MouseWindow->bSetWindowPos(HWND_BOTTOM, rcMouse.left, rcMouse.top, iMouseSizeX, iMouseSizeX, (SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS)))	return FALSE;
	return TRUE;
}


//
// bRegisterDrawIMEModeThread()
//
BOOL		CCursor::bRegisterDrawIMEModeThread(HWND hWndObserved)
{
	if (&stIMECursorData == NULL)	return FALSE;
	// Register thread
	stIMECursorData.hWndObserved = hWndObserved;
	if (!DrawIMEModeThread->bRegister(DRAWIMEMODETHREADNAME, DRAWIMEMODETHREADID,
							(LPTHREAD_START_ROUTINE)&_bDrawIMEModeRoutine, &stIMECursorData, 0)) {
		return	FALSE;
	}
	return	TRUE;
}

//
// _bDrawIMEModeRoutine()
//
BOOL WINAPI	CCursor::_bDrawIMEModeRoutine(LPVOID lpvParam)
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
	if (bCalcDisplayModeRect(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rc)) {
		if (EnumDisplayMonitors(NULL, &rc, (MONITORENUMPROC)&_bIconDrawEnumProc, (LPARAM)lpstCursorData) != 0) {
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
BOOL		CCursor::_bIconDrawEnumProc(HMONITOR hMonitor, HDC hDC, LPCRECT lprcClip, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hMonitor);
	UNREFERENCED_PARAMETER(hDC);
	UNREFERENCED_PARAMETER(lprcClip);
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lParam;
	CCursor* This = reinterpret_cast<CCursor*>(lParam);
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
HWND		CCursor::hGetCaretPosByAccessibleObjectFromWindow(HWND hForeWnd, LPRECT lpRect, BOOL bAttachThreadInput)
{
	HWND	hWnd = NULL;
	POINT	pt{};
	DWORD	dwPID = 0, dwForeThreadID = 0;
	if (hForeWnd != NULL) {
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
										lpRect->left = pt.x;	lpRect->top = pt.y;	lpRect->right = pt.x + sz.cx;	lpRect->bottom = pt.y + sz.cy;
										if ((pt.x == 0) && (pt.y == 0) && (sz.cx == 0) && (sz.cy == 0)) {
											lpRect->left = 0;	lpRect->right = 0;	lpRect->top = 0;	lpRect->bottom = 0;
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
							lpRect->left = pt.x;	lpRect->top = pt.y;
							pt.x = lpGuiThreadInfo->rcCaret.right;	pt.y = lpGuiThreadInfo->rcCaret.bottom;
							if (ClientToScreen(lpGuiThreadInfo->hwndCaret, &pt)) {
								lpRect->right = pt.x;	lpRect->bottom = pt.y;
								RECT	rcTop{};
								if (GetWindowRect(lpGuiThreadInfo->hwndCaret, &rcTop)) {
#define	MARGIN		2
									if (((pt.x == 0) && (pt.y == 0)) || (pt.x <= rcTop.left + MARGIN) || (pt.y <= rcTop.top + MARGIN)) {
#undef MARGIN
										lpRect->left = 0;	lpRect->right = 0;	lpRect->top = 0;	lpRect->bottom = 0;
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
	if (lpstCursorData->bDrawIMEModeWait) {
		if (lpstCursorData->dwWaitTime != 0)	Sleep(lpstCursorData->dwWaitTime);
	}
	if (lpstCursorData->bDrawNearCaret != FALSE) {
		if (lpstCursorData->hWndCaret != NULL) {
			rcCaret.left = lpstCursorData->rcCaret.left;	rcCaret.top = lpstCursorData->rcCaret.top;
			rcCaret.right = lpstCursorData->rcCaret.right;	rcCaret.bottom = lpstCursorData->rcCaret.bottom;
			hCaretWnd = lpstCursorData->hWndCaret;
			bFoundCaret = TRUE;
		}
		if (bFoundCaret && (hCaretWnd != NULL)) {
			if (bAdjustModeSizeByMonitorDPI(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcCaret) != FALSE) {
				if ((rcCaret.left == 0) && (rcCaret.top == 0) && (rcCaret.right == 0) && (rcCaret.bottom == 0)) {
					bFoundCaret = FALSE;
				}
				else {
					dwIMEModeCaret = Cime->dwIMEMode(hCaretWnd, lpstCursorData->bForceHiragana);
					int	iCaret = iGetCurosrID(dwIMEModeCaret, lpstCursorData);
					if (iCaret == 0) dwIMEModeCaret = IMEOFF;
					CaretWindow->vSetModeString(lpstCursorData->lpstFlushMouseCursor[iCaret].szMode);

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
	dwIMEModeCursor = Cime->dwIMEMode(lpstCursorData->hWndObserved, lpstCursorData->bForceHiragana);
	if (bFoundCaret == FALSE) {
		int	iCursor = iGetCurosrID(dwIMEModeCursor, lpstCursorData);
		if (iCursor == 0) dwIMEModeCursor = IMEOFF;
		CursorWindow->vSetModeString(lpstCursorData->lpstFlushMouseCursor[iCursor].szMode);
	}

#define	COUNT	10
	BOOL	bRet = FALSE;
	int		iCount = 0;
	if (lpstCursorData->dwDisplayModeTime != 0)	iCount = (lpstCursorData->dwDisplayModeTime + (COUNT - 1)) / COUNT;
	else iCount = COUNT;
	for (; iCount >= 0; iCount--) {
		if (bFoundCaret == FALSE) {
			if (bCalcDisplayModeRect(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcCursor)) {
				iCursorSizeX = rcCursor.right - rcCursor.left; iCursorSizeY = rcCursor.bottom - rcCursor.top;
				if ((dwIMEModeCursor == IMEOFF) || (dwIMEModeCursor == HANEISU_IMEON) || (dwIMEModeCursor == HANKANA_IMEON)) {
					iCursorSizeX = (iCursorSizeX * 2 + 2) / 3; rcCursor.left = rcCursor.right - iCursorSizeX;
				}
				if (dwIMEModeCursor != IMEOFF) {
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
		}
		Sleep(COUNT);
		bRet = TRUE;
	}
	try {
		bRet = CursorWindow->bSetWindowPos(HWND_BOTTOM, rcCursor.left, rcCursor.top, iCursorSizeX, iCursorSizeY, (SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS));
	}
	catch (...) {
	}
	CursorWindow->vSetModeString(lpstCursorData->lpstFlushMouseCursor[0].szMode);
	if (bFoundCaret) {
		try {
			bRet = CaretWindow->bSetWindowPos(HWND_BOTTOM, rcCaret.left, rcCaret.top, iCaretSizeX, iCaretSizeY, (SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS));
		}
		catch (...) {
		}
		CaretWindow->vSetModeString(lpstCursorData->lpstFlushMouseCursor[0].szMode);
	}
	return bRet;
}

//
// iGetCurosrID()
//
int			CCursor::iGetCurosrID(DWORD dwIMEMode, LPIMECURSORDATA lpstCursorData)
{
	int		i = 0;
	while (dwIMEMode != lpstCursorData->lpstFlushMouseCursor[i].dwIMEMode) {
		++i;
		if (lpstCursorData->lpstFlushMouseCursor[i].dwIMEMode == (DWORD)(-1)) {
			i = 0;
			break;
		}
	}
	return i;
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
		if (bSetSystemCursor((LPMOUSECURSOR) & (lpstCursorData->lpstFlushMouseCursor[i]).stHand, lpstCursorData->iCursorSize, lpstCursorData->iCursorSize)) {
			if (bSetSystemCursor((LPMOUSECURSOR) & (lpstCursorData->lpstFlushMouseCursor[i]).stIBeam, lpstCursorData->iCursorSize, lpstCursorData->iCursorSize)) {
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
					PostMessage(hMainWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
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
BOOL		CCursorWindow::bRegister(HINSTANCE hInstance, LPCTSTR szWindowClassName, COLORREF dwRGB)
{
	if (hInstance == NULL)	return FALSE;

	vSetTextColor(dwRGB);

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
// vSetTextColor()
//
VOID		CCursorWindow::vSetTextColor(COLORREF dwRGB)
{
	dwTextColor = dwRGB;
	if (dwRGB & 0x00000100)	dwBackColor = dwRGB & 0xfffffeff;
	else dwBackColor = dwRGB | 0x00000100;
}

//
// vSetModeString()
// 
VOID		CCursorWindow::vSetModeString(LPTSTR _lpszIMEMode)
{
	lpszIMEMode = _lpszIMEMode;
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
BOOL		CCursorWindow::Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
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
	PostQuitMessage(0);
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