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

	hModDll = NULL;
	iCursorDataLoadCount = 0;

	stIMECursorData.hWndObserved = NULL;
	stIMECursorData.dwIMEModeCursor = (DWORD)(-1);
	stIMECursorData.bDrawIMEModeWait = FALSE;
	stIMECursorData.bDrawNearCaret = FALSE;
	stIMECursorData.lpszLoadDatName = NULL;
	stIMECursorData.lpstFlushMouseCursor = (LPFLUSHMOUSECURSOR)&stFlushMouseCursor;

	stIMECursorData.hWndCaret = NULL;

	IMECursorChangeThread = NULL;
	CursorWindow = NULL;
	DrawIMEModeThread = NULL;
	CaretWindow = NULL;
	DrawIMEModeCaretThread = NULL;
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
	if (IMECursorChangeThread != NULL) {
		delete	IMECursorChangeThread;
		IMECursorChangeThread = NULL;
	}
	if (stIMECursorData.lpszLoadDatName != NULL) {
		delete []	stIMECursorData.lpszLoadDatName;
		stIMECursorData.lpszLoadDatName = NULL;
	}
	if (hModDll != NULL) {
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
	if (hModDll == NULL) {
		if (hCursorDllLoad() == NULL)	return FALSE;
		if (!bSystemCursorLoad())	return FALSE;
	}

#define	WINDOWCLASS		_T("FlushMouseCursorWindow-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}")
	if (CursorWindow == NULL) {
		CursorWindow = new CCursorWindow;
		if (!CursorWindow->bRegister((HINSTANCE)GetWindowLongPtr(hMainWnd, GWLP_HINSTANCE), WINDOWCLASS, stIMECursorData.dwNearDrawMouseColor))		return FALSE;
		CursorWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
		DrawIMEModeThread = new CThread;
	}
	else {
		CursorWindow->vSetTextColor(stIMECursorData.dwNearDrawMouseColor);
	}
#undef WINDOWCLASS
#define	WINDOWCLASS		_T("FlushMouseCaretWindow-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}")
	if (CaretWindow == NULL) {
		CaretWindow = new CCursorWindow;
		if (!CaretWindow->bRegister((HINSTANCE)GetWindowLongPtr(hMainWnd, GWLP_HINSTANCE), WINDOWCLASS, stIMECursorData.dwNearDrawCaretColor))	return FALSE;
		CaretWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
		DrawIMEModeCaretThread = new CThread;
	}
	else {
		CaretWindow->vSetTextColor(stIMECursorData.dwNearDrawCaretColor);
	}
#undef WINDOWCLASS

	if (IMECursorChangeThread == NULL) {
		IMECursorChangeThread = new CThread;
		if (!bChangeFlushMouseCursor(IMEOFF, &stIMECursorData)) return FALSE;
		if (!bRegisterIMECursorChangeThread(hMainWnd)) return FALSE;
		if (!bStartIMECursorChangeThread(hMainWnd))	return FALSE;
	}
	if (DrawIMEModeThread == NULL) {
		if (!bRegisterDrawIMEModeThread(hMainWnd))	return FALSE;
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
		stIMECursorData.bForceHiragana = Profile->lpstAppRegData->bForceHiragana;
		stIMECursorData.bDrawNearCaret = Profile->lpstAppRegData->bDrawNearCaret;
		stIMECursorData.dwNearDrawMouseColor = (~Profile->lpstAppRegData->dwNearDrawMouseColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseColor & 0x00ffffff);
		stIMECursorData.dwNearDrawCaretColor = (~Profile->lpstAppRegData->dwNearDrawCaretColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretColor & 0x00ffffff);

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
	if ((Cursor == NULL) || (IMECursorChangeThread == NULL))	return FALSE;
	vSetParamFromRegistry();
	stIMECursorData.hWndObserved = hWndObserved;
	if (!IMECursorChangeThread->bStart()) {
		if (IMECursorChangeThread != NULL) {
			delete	IMECursorChangeThread;
			IMECursorChangeThread = NULL;
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
	vSetParamFromRegistry();
	stIMECursorData.bDrawIMEModeWait = FALSE;
	stIMECursorData.dwWaitTime = 0;
	return _bStartDrawIMEModeThread(hWndObserved);
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
	return _bStartDrawIMEModeThread(hWndObserved);
}

//
// bStartDrawIMEModeThreadWaitWave()
//
BOOL		CCursor::bStartDrawIMEModeThreadWaitWave(HWND hWndObserved)
{
	vSetParamFromRegistry();
	stIMECursorData.bDrawIMEModeWait = TRUE;
	if (Profile == NULL)	return FALSE;
	stIMECursorData.dwWaitTime = Profile->lpstAppRegData->dwWaitWaveTime;
	return _bStartDrawIMEModeThread(hWndObserved);
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
	return _bStartDrawIMEModeThread(hWndObserved);
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
	return _bStartDrawIMEModeThread(hWndObserved);
}

//
// _bStartDrawIMEModeThread()
//
BOOL		CCursor::_bStartDrawIMEModeThread(HWND hWndObserved)
{
	if ((Cursor == NULL) || (DrawIMEModeThread == NULL))		return FALSE;
	stIMECursorData.hWndObserved = hWndObserved;
	stIMECursorData.hWndCaret = _hGetCaretPosByAccessibleObjectFromWindow(hWndObserved, &stIMECursorData.rcCaret, FALSE);
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
BOOL		CCursor::bRegisterIMECursorChangeThread(HWND hWndObserved)
{
	if (&stIMECursorData == NULL)	return FALSE;
	stIMECursorData.hWndObserved = hWndObserved;
	if (!IMECursorChangeThread->bRegister(IMECURSORCHANGETHREADNAME, IMECURSORCHANGETHREADID,
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
	if (lpvParam == NULL)	return FALSE;
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lpvParam;
	CCursor	*This = reinterpret_cast<CCursor*>(lpvParam);
	BOOL	bRet = TRUE;
	if (lpstCursorData->bDisplayIMEModeOnCursor) {
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
	if (_bCalcDispModeRect(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rc)) {
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
// _bCalcDispModeRect()
//
BOOL		CCursor::_bCalcDispModeRect(int iModeSizeX, int iModeSizeY, LPRECT lpRect)
{
	POINT	pt{};
	if (GetCursorPos(&pt)) {
		HMONITOR hMonitor = NULL;
		if ((hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST)) != NULL) {
			UINT		dpiX = 0, dpiY = 0;
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
// _hGetCaretPosByAccessibleObjectFromWindow()
// 
HWND		CCursor::_hGetCaretPosByAccessibleObjectFromWindow(HWND hForeWnd, LPRECT lpRect, BOOL bAttachThreadInput)
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
// _bAdjustCaretByMonitorDPI()
//
BOOL		CCursor::_bAdjustCaretByMonitorDPI(int iModeSizeX, int iModeSizeY, LPRECT lpRect)
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
				lpRect->top = (lpRect->bottom - ((lpRect->bottom - lpRect->top) / 2 ) - (iIconSizeY / 2));// + (DESCENT * (dpiY + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI);
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
			if (_bAdjustCaretByMonitorDPI(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcCaret) != FALSE) {
				if ((rcCaret.left == 0) && (rcCaret.top == 0) && (rcCaret.right == 0) && (rcCaret.bottom == 0)) {
					bFoundCaret = FALSE;
				}
				else {
					dwIMEModeCaret = Cime->dwIMEMode(hCaretWnd, lpstCursorData->bForceHiragana);
					int		iCaret = 0;
					while (dwIMEModeCaret != lpstCursorData->lpstFlushMouseCursor[iCaret].dwIMEMode) {
						++iCaret;
						if (lpstCursorData->lpstFlushMouseCursor[iCaret].dwIMEMode == (DWORD)(-1)) {
							iCaret = 0;	dwIMEModeCaret = IMEOFF;
							break;
						}
					}
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
		int		iCursor = 0;
		while (dwIMEModeCursor != lpstCursorData->lpstFlushMouseCursor[iCursor].dwIMEMode) {
			++iCursor;
			if (lpstCursorData->lpstFlushMouseCursor[iCursor].dwIMEMode == (DWORD)(-1)) {
				iCursor = 0;	dwIMEModeCursor = IMEOFF;
				break;
			}
		}
		CursorWindow->vSetModeString(lpstCursorData->lpstFlushMouseCursor[iCursor].szMode);
	}

#define	COUNT	10
	BOOL	bRet = FALSE;
	int		iCount = 0;
	if (lpstCursorData->dwDisplayModeTime != 0)	iCount = (lpstCursorData->dwDisplayModeTime + (COUNT - 1)) / COUNT;
	else iCount = COUNT;
	for (; iCount >= 0; iCount--) {
		if (bFoundCaret == FALSE) {
			if (_bCalcDispModeRect(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcCursor)) {
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
		if (lpstMC->szFile[0] == '\0')		return TRUE;	
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
//class CCursorWindow
//
CCursorWindow::CCursorWindow()
{
	lpszWindowClass = NULL;
	hCursorWindow = NULL;
	lpszMode = NULL;
	dwTextColor = aRGB(15, 254, 192, 0);
	dwBackColor = aRGB(15, 254, 192, 0);
}

CCursorWindow::~CCursorWindow()
{
	if (hCursorWindow != NULL)	 {
		PostMessage(hCursorWindow, WM_DESTROY, (WPARAM)0, (LPARAM)0);
		hCursorWindow = NULL;
	}
	if (lpszWindowClass != NULL) {
		delete []	lpszWindowClass;
		lpszWindowClass = NULL;
	}
}

//
// bRegister()
//
BOOL		CCursorWindow::bRegister(HINSTANCE hInstance, LPCTSTR szWindowClassName, COLORREF dwRGB)
{
	if (hInstance == NULL)	return FALSE;
	lpszWindowClass = new TCHAR[MAX_LOADSTRING];
	if (lpszWindowClass) {
		ZeroMemory(lpszWindowClass, MAX_LOADSTRING);
		_tcsncpy_s(lpszWindowClass, MAX_LOADSTRING, szWindowClassName, _TRUNCATE);

		vSetTextColor(dwRGB);

		if (!MyRegisterClass(hInstance)) {
			return FALSE;
		}
		if ((hCursorWindow = InitInstance(hInstance, SW_HIDE)) == NULL) {
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
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
ATOM CCursorWindow::MyRegisterClass(HINSTANCE hInstance) const
{
#define CLASSSTYLE (CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE)
	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CLASSSTYLE;	
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;	
	wcex.cbWndExtra = 0;	
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = lpszWindowClass;
	wcex.hIconSm = NULL;	

	ATOM		atom = 0;
	if ((atom = RegisterClassEx(&wcex)) == 0) {
		_Post_equals_last_error_ DWORD err = GetLastError();
		if (err == 1410)		return 1;
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

	HWND		hWnd = NULL;	
	hWnd = CreateWindowEx(
					EX_WINDOWSTYLE,
					lpszWindowClass,	
					NULL,
					WINDOWSTYLE,	
					0, 0,
					0, 0,
					NULL,
					NULL,
					hInstance,
					this);
	if (!hWnd) {
		return NULL;	
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
	return SetLayeredWindowAttributes(hWnd, dwBackColor, (dwBackColor >> 24) & 0x000000ff, LWA_COLORKEY | LWA_ALPHA);
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
		if (SetLayeredWindowAttributes(hCursorWindow, dwBackColor, (dwBackColor >> 24) & 0x000000ff, LWA_COLORKEY | LWA_ALPHA)) {
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
											SHIFTJIS_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
											PROOF_QUALITY, FIXED_PITCH, FONT)) != NULL) {
											HFONT	hFontPrev = NULL;
											if ((hFontPrev = (HFONT)SelectObject(hDC, hFont)) != NULL) {
												if (lpszMode != NULL) {
													if (DrawTextEx(hDC, lpszMode, -1, &rc, DT_RIGHT | DT_SINGLELINE | DT_VCENTER, NULL) == 0) {
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