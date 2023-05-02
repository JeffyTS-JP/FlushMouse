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
#include "CommonDef.h"
#include "FlushMouseLIB.h"
#include "..\FlushMouseCursor\Resource.h"
#include "..\MiscLIB\CRegistry.h"

//
// Define
//
#define FLUSHMOUSECURSORDIR	_T("\\JeffyTS\\FlushMouse\\")
#define FLUSHMOUSECURSORDAT	_T("FlushMouseCursor.dat")

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
// Class CIME
//
CIME::CIME()
{
	lpstVertialDesktop = new VERTIALDESKTOP[sizeof(VERTIALDESKTOP)];
	ZeroMemory(lpstVertialDesktop, sizeof(VERTIALDESKTOP));
	bGetVertialDesktopSize();
}

CIME::~CIME()
{
	if (lpstVertialDesktop != NULL)	delete[]	lpstVertialDesktop;
	lpstVertialDesktop = NULL;
}

//
// bIsIMEOpen()
//
BOOL		CIME::bIsIMEOpen(HWND hWndObserved)
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
VOID		CIME::vIMEOpenCloseForced(HWND hWndObserved, DWORD dwIMEOpenClose)
{
	if (hWndObserved == NULL)	return;
	LPARAM	lParam = (LPARAM)dwIMEOpenClose;
	EnumChildWindows(hWndObserved, &bEnumChildProcIMEOpenClose, lParam);
	return;
}

//
// vIMEConvertModeChangeForced()
//
VOID		CIME::vIMEConvertModeChangeForced(HWND hWndObserved, DWORD dwConvertMode)
{
	if (hWndObserved == NULL)	return;
	LPARAM	lParam = (LPARAM)dwConvertMode;
	EnumChildWindows(hWndObserved, &bEnumChildProcIMECnvertMode, lParam);
	return;
}

//
// bEnumChildProcIMECnvertMode()
//
BOOL CALLBACK CIME::bEnumChildProcIMECnvertMode(HWND hWndObserved, LPARAM lParam)
{
	if (hWndObserved == NULL)	return FALSE;
	HWND    hIMWnd = NULL;
	if ((hIMWnd = ImmGetDefaultIMEWnd(hWndObserved)) != NULL) {
		if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETOPENSTATUS, (LPARAM)TRUE) == 0) {	// lParam = FALSE to IME CLOSE / TRUE to IME OPEN
			if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETCONVERSIONMODE, (LPARAM)lParam) == 0) {
#ifdef _DEBUG
				//DBvPrintf(_T("==hWndObserved = 0x%08x lParam = 0x%08x\n"), hWndObserved, lParam);
#endif // _DEBUG
			}
		}
	}
	return TRUE;			// If error, but Continue all child Window
}

//
// bEnumChildProcIMEOpenClose()
//
BOOL CALLBACK CIME::bEnumChildProcIMEOpenClose(HWND hWndObserved, LPARAM lParam)
{
	if (hWndObserved == NULL)	return FALSE;
	HWND    hIMWnd = NULL;
	if ((hIMWnd = ImmGetDefaultIMEWnd(hWndObserved)) != NULL) {
		if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETOPENSTATUS, lParam) == 0) {	// lParam = FALSE to IME CLOSE / TRUE to IME OPEN
#ifdef _DEBUG
			//DBvPrintf(_T("==hWndObserved = 0x%08x lParam = 0x%08x\n"), hWndObserved, lParam);
#endif // _DEBUG
		}
	}
	return TRUE;			// If error, but Continue all child Window
}

//
// dwIMEMode()
//
DWORD		CIME::dwIMEMode(HWND hWndObserved, BOOL bForceHiragana)
{
	if (hWndObserved != NULL) {
		HWND    hIMWnd = NULL;
		DWORD   dwConvertMode = 0;
		if ((hIMWnd = ImmGetDefaultIMEWnd(hWndObserved)) != NULL) {
			if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETOPENSTATUS, NULL) != 0) {
				if ((dwConvertMode = (DWORD)SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETCONVERSIONMODE, NULL)) != 0) {
					switch (dwConvertMode) {
					case IME_CMODE_NATIVE:
						dwConvertMode = IMEOFF;
						break;
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
// vActivateIME()
//
VOID		CIME::vActivateIME(HWND hWndObserved)
{
	EnumChildWindows(hWndObserved, &bEnumChildProcActivateIME, NULL);
}

// 
// bEnumChildProcActivateIME()
// 
BOOL CALLBACK CIME::bEnumChildProcActivateIME(HWND hWnd, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	BOOL	bRet = FALSE;
	HWND	hIMWnd = ImmGetDefaultIMEWnd(hWnd);
	if (hIMWnd != NULL) {
		SendMessage(hIMWnd, WM_ACTIVATE, (WPARAM)WA_ACTIVE, (LPARAM)NULL);
		LPTSTR	lpszBuffer = new TCHAR[_MAX_PATH];
		ZeroMemory(lpszBuffer, _MAX_PATH);
		if (GetClassName(hIMWnd, lpszBuffer, _MAX_PATH) != 0) {
			HWND	hIMEWnd = FindWindow(lpszBuffer, NULL);
			if (hIMEWnd != NULL) {
				SendMessage(hIMEWnd, WM_ACTIVATE, (WPARAM)WA_ACTIVE, (LPARAM)NULL);

				bRet = TRUE;
			}
		}
		if (lpszBuffer != NULL)	delete[] lpszBuffer;
	}
	//return bRet;
	return TRUE;
}

//
// bMoveIMEToolbar()
//
BOOL		CIME::bMoveIMEToolbar()
{
	if (bExsistIMEToolbar()) {
		HWND	hIMEWnd = FindWindow(_T("ApplicationFrameWindow"), NULL);
		if (hIMEWnd != NULL) {
			POINT	pt{};
			if (GetCursorPos(&pt)) {
				RECT	rc{};
				if (GetWindowRect(hIMEWnd, &rc)) {
					VERTIALDESKTOP	stVertianDesktop{};
					if (EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)&bGetVertialDesktopSizeEnumProc, (LPARAM)&stVertianDesktop)) {
						// Success
					}
					else {
						_Post_equals_last_error_ DWORD	err = GetLastError();
						if ((err == ERROR_INVALID_PARAMETER) || (err == ERROR_SUCCESS)) {
						}
						else if (err == ERROR_ACCESS_DENIED) {
						}
						else {
							return FALSE;
						}
					}
					LPINPUT lpInputs = NULL;
					if ((lpInputs = new INPUT[sizeof(INPUT) * 4]) != NULL) {
						ZeroMemory(lpInputs, sizeof(INPUT) * 4);
						LONG	xCursor = 0, yCursor = 0, xToolbar = 0, yToolbar = 0;
						if ((rc.right - rc.left) > (rc.bottom - rc.top)) {
							rc.top = rc.top + 16;	rc.left = rc.left + 10;
						}
						else {
							rc.top = rc.top + 10;	rc.left = rc.left + 16;
						}
						LONG	px0 = stVertianDesktop.rcMonitorSize.left;
						LONG	py0 = stVertianDesktop.rcMonitorSize.top;
						ULONG	cx = stVertianDesktop.rcMonitorSize.right - stVertianDesktop.rcMonitorSize.left;
						ULONG	cy = stVertianDesktop.rcMonitorSize.bottom - stVertianDesktop.rcMonitorSize.top;
						xCursor = ((((LONG)pt.x - px0) * 65536L) + cx - 1L) / cx;
						yCursor = ((((LONG)pt.y - py0) * 65536L) + cy - 1L) / cy;
						xToolbar = ((((LONG)rc.left - px0) * 65536L) + cx - 1L) / cx;
						yToolbar = ((((LONG)rc.top - py0) * 65536L) + cy - 1L) / cy;
						// #0
						int	i = 0;
						lpInputs[i].type = INPUT_MOUSE; lpInputs[i].mi.dx = (DWORD)xToolbar; lpInputs[i].mi.dy = (DWORD)yToolbar;
						lpInputs[i].mi.mouseData = 0; lpInputs[i].mi.time = 0; lpInputs[i].mi.dwExtraInfo = 0;
						lpInputs[i].mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK;
						// #1
						i = 1;
						lpInputs[i].type = INPUT_MOUSE; lpInputs[i].mi.dx = (DWORD)xCursor; lpInputs[i].mi.dy = (DWORD)yCursor;
						lpInputs[i].mi.mouseData = 0; lpInputs[i].mi.time = 0; lpInputs[i].mi.dwExtraInfo = 0;
						lpInputs[i].mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK;
						// #2
						i = 2;
						lpInputs[i].type = INPUT_MOUSE; lpInputs[i].mi.dx = (DWORD)xCursor; lpInputs[i].mi.dy = (DWORD)yCursor;
						lpInputs[i].mi.mouseData = 0; lpInputs[i].mi.time = 0; lpInputs[i].mi.dwExtraInfo = 0;
						lpInputs[i].mi.dwFlags = MOUSEEVENTF_LEFTUP;
						UINT	cInputs = (UINT)(3);
						if (SendInput(cInputs, &lpInputs[0], sizeof(INPUT)) == cInputs) {
							Sleep(10);
						}
						delete[]	lpInputs;
					}

				}
				else {
					return FALSE;
				}
			}
			else {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}
	}
	return TRUE;
}

//
// bGetVertialDesktopSize()
//
BOOL			CIME::bGetVertialDesktopSize()
{
	BOOL	bRet = FALSE;
	if (lpstVertialDesktop != NULL) {
		ZeroMemory(lpstVertialDesktop, sizeof(VERTIALDESKTOP));
		if (EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)&bGetVertialDesktopSizeEnumProc, (LPARAM)lpstVertialDesktop) != 0) {
			bRet = TRUE;
		}
		else {
			_Post_equals_last_error_ DWORD	err = GetLastError();
			if ((err == ERROR_INVALID_PARAMETER) || (err == ERROR_SUCCESS)) {
				bRet = TRUE;
			}
			else if (err == ERROR_ACCESS_DENIED) {
				// error but be able to On Sleep
				bRet = TRUE;
			}
			else {
			}
		}
	}
	return bRet;
}

//
// bGetVertialDesktopSizeEnumProc()
//
BOOL		CIME::bGetVertialDesktopSizeEnumProc(HMONITOR hMonitor, HDC hDC, LPCRECT lprcClip, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hDC);
	UNREFERENCED_PARAMETER(lprcClip);
	LPVERTIALDESKTOP	lpstVertialDesktop = (LPVERTIALDESKTOP)lParam;
	//CIME* This = reinterpret_cast<CIME*>(lParam);
	MONITORINFO	stMonInfo{};	stMonInfo.cbSize = sizeof(MONITORINFO);
	if (GetMonitorInfo(hMonitor, &stMonInfo) != 0) {
		if (stMonInfo.rcMonitor.left <= lpstVertialDesktop->rcMonitorSize.left)		lpstVertialDesktop->rcMonitorSize.left = stMonInfo.rcMonitor.left;
		if (stMonInfo.rcMonitor.right >= lpstVertialDesktop->rcMonitorSize.right)	lpstVertialDesktop->rcMonitorSize.right = stMonInfo.rcMonitor.right;
		if (stMonInfo.rcMonitor.top <= lpstVertialDesktop->rcMonitorSize.top)		lpstVertialDesktop->rcMonitorSize.top = stMonInfo.rcMonitor.top;
		if (stMonInfo.rcMonitor.bottom >= lpstVertialDesktop->rcMonitorSize.bottom)	lpstVertialDesktop->rcMonitorSize.bottom = stMonInfo.rcMonitor.bottom;
		++lpstVertialDesktop->iNumOfMonitors;
		return TRUE;
	}
	return FALSE;
}

//
// bExsistIMEToolbar
//
BOOL		CIME::bExsistIMEToolbar()
{
#define SUBKEY	_T("Software\\appdatalow\\software\\microsoft\\ime\\15.0\\IMEJP\\MSIME")
#define VALUE	_T("ToolBarEnabled")
	CRegistry* CReg = new CRegistry;
	DWORD	dwToolBar = 0;
	if (!CReg->bReadRegValueDWORD(HKEY_CURRENT_USER, SUBKEY, VALUE, &dwToolBar)) {
	}
	delete	CReg;
	if ((bIsNewIME() == TRUE) && (dwToolBar != 0))	return TRUE;
	else return FALSE;
#undef SUBKEY
#undef VALUE
}

//
// bIsNewIME()
//
BOOL		CIME::bIsNewIME()
{
#define SUBKEY	_T("Software\\Microsoft\\CTF\\TIP\\{03B5835F-F03C-411B-9CE2-AA23E1171E36}")
#define VALUE	_T("DummyValue")
	CRegistry* CReg = new CRegistry;
	DWORD	dwCTF = 0;
	if (!CReg->bReadRegValueDWORD(HKEY_CURRENT_USER, SUBKEY, VALUE, &dwCTF)) {
	}
	delete	CReg;
	if (dwCTF != 0)	return TRUE;
	else			return FALSE;
#undef SUBKEY
#undef VALUE
}

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
	stIMECursorData.dwIMEModeCursor = (DWORD)(-1);
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
		DrawIMEModeCaretThread = NULL;
	}
	if (CaretWindow != NULL) {
		delete	CaretWindow;						// Draw Window near by Caret cursorの削除
		CaretWindow = NULL;
	}
	if (DrawIMEModeThread != NULL) {
		delete	DrawIMEModeThread;					// Draw IME Mode near by Mouse cursor Threadの後始末
		DrawIMEModeThread = NULL;
	}
	if (CursorWindow != NULL) {
		delete	CursorWindow;						// Draw Window near by Mouse cursorの削除
		CursorWindow = NULL;
	}
	if (IMECursorChangeThread != NULL) {
		delete	IMECursorChangeThread;				// IME Cursor Change Threadの後始末
		IMECursorChangeThread = NULL;
	}
	if (stIMECursorData.lpszLoadDatName != NULL) {
		delete[]	stIMECursorData.lpszLoadDatName;
		stIMECursorData.lpszLoadDatName = NULL;
	}
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
	stIMECursorData.dwNearDrawMouseColor = (~Profile->stAppRegData.dwNearDrawMouseColor & 0xff000000) | (Profile->stAppRegData.dwNearDrawMouseColor & 0x00ffffff);
	stIMECursorData.dwNearDrawCaretColor = (~Profile->stAppRegData.dwNearDrawCaretColor & 0xff000000) | (Profile->stAppRegData.dwNearDrawCaretColor & 0x00ffffff);

	stIMECursorData.bDenyChangedByApp = Profile->stAppRegData.bDenyChangedByApp;
	stIMECursorData.bUseBigArrow = Profile->stAppRegData.bUseBigArrow;

	if ((stIMECursorData.lpszLoadDatName = lpszGetCursorDataName()) == NULL)		return FALSE;
	if (hCursorDllLoad() == NULL)	return FALSE;
	if (!bSystemCursorLoad())	return FALSE;

	IMECursorChangeThread = new CThread;
	if (!bChangeFlushMouseCursor(IMEOFF, &stIMECursorData)) return FALSE;
	if (!bRegisterIMECursorChangeThread(hWnd)) return FALSE;
	
#define	WINDOWCLASS		_T("CursorWindow")
	if (CursorWindow == NULL) {
		CursorWindow = new CCursorWindow;
		if (!CursorWindow->bRegister((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), WINDOWCLASS, stIMECursorData.dwNearDrawMouseColor))		return FALSE;
		CursorWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
		DrawIMEModeThread = new CThread;
	}
#undef WINDOWCLASS
#define	WINDOWCLASS		_T("CaretWindow")
	if (CaretWindow == NULL) {
		CaretWindow = new CCursorWindow;
		if (!CaretWindow->bRegister((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), WINDOWCLASS, stIMECursorData.dwNearDrawCaretColor))	return FALSE;
		CaretWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
		DrawIMEModeCaretThread = new CThread;
	}
#undef WINDOWCLASS

	if (!bRegisterDrawIMEModeThread(hWnd))	return FALSE;
	if (!bStartIMECursorChangeThread(hWnd))	return FALSE;

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
// bStartIMECursorChangeThread()
//
BOOL		CCursor::bStartIMECursorChangeThread(HWND hWndObserved)
{
	if ((Cursor == NULL) || (IMECursorChangeThread == NULL))	return FALSE;
	stIMECursorData.hWndObserved = hWndObserved;
	stIMECursorData.bDisplayFocusWindowIME = bDisplayFocusWindowIME;
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
	if ((Cursor == NULL) || (DrawIMEModeThread == NULL))	return FALSE;
	stIMECursorData.hWndObserved = hWndObserved;
	stIMECursorData.bDrawIMEModeWait = FALSE;
	stIMECursorData.bDisplayFocusWindowIME = bDisplayFocusWindowIME;
	stIMECursorData.dwWaitWaveTime = Profile->stAppRegData.dwWaitWaveTime;
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
// bStartDrawIMEModeThreadWait()
//
BOOL		CCursor::bStartDrawIMEModeThreadWait(HWND hWndObserved)
{
	if ((Cursor == NULL) || (DrawIMEModeThread == NULL))	return FALSE;
	stIMECursorData.hWndObserved = hWndObserved;
	stIMECursorData.bDrawIMEModeWait = TRUE;
	stIMECursorData.bDisplayFocusWindowIME = bDisplayFocusWindowIME;
	stIMECursorData.dwWaitWaveTime = Profile->stAppRegData.dwWaitWaveTime;
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
// bStartDrawIMEModeThreadWaitDblClk()
//
BOOL		CCursor::bStartDrawIMEModeThreadWaitDblClk(HWND hWndObserved)
{
	if ((Cursor == NULL) || (DrawIMEModeThread == NULL))	return FALSE;
	stIMECursorData.hWndObserved = hWndObserved;
	stIMECursorData.bDrawIMEModeWait = TRUE;
	stIMECursorData.bDisplayFocusWindowIME = bDisplayFocusWindowIME;
	stIMECursorData.dwWaitWaveTime = GetDoubleClickTime();
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
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lpvParam;
	CCursor* This = reinterpret_cast<CCursor*>(lpvParam);
	BOOL		bRet = TRUE;
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
	// Register thread
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
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lpvParam;
	CCursor* This = reinterpret_cast<CCursor*>(lpvParam);
	This->bIsIMECursorChanged(lpstCursorData);
	if (!This->bDrawIMEModeOnDisplay(lpstCursorData)) {
		_Post_equals_last_error_ DWORD err = GetLastError();
		if (err == ERROR_ACCESS_DENIED) {
			return FALSE;
		}
		return FALSE;
	}
	return	TRUE;
}

//
// bIMECursorChange()
//
BOOL		CCursor::bIsIMECursorChanged(LPIMECURSORDATA lpstCursorData)
{
	DWORD		dwIMEMode = (DWORD)(-1);

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
	BOOL		bRet = FALSE;
	RECT		rc{};
	if (lpstCursorData->bDrawIMEModeWait) {
		if (lpstCursorData->dwWaitWaveTime != 0)	Sleep(lpstCursorData->dwWaitWaveTime);
	}
	if (bCalcDispModeRect(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rc)) {
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
// bIconDrawEnumProc()
//
BOOL		CCursor::bIconDrawEnumProc(HMONITOR hMonitor, HDC hDC, LPCRECT lprcClip, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hMonitor);
	UNREFERENCED_PARAMETER(hDC);
	UNREFERENCED_PARAMETER(lprcClip);
	LPIMECURSORDATA	lpstCursorData = (LPIMECURSORDATA)lParam;
	CCursor* This = reinterpret_cast<CCursor*>(lParam);
	return This->bDrawIMEModeOnDisplaySub(lpstCursorData);
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
				iIconSizeY = iModeSizeY * (dpiY + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI + 1;
				// Draw near by Mouse cursor Left
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
// bGetCaretPos()
//
BOOL		CCursor::bGetCaretPos(HWND hWnd, LPPOINT lpCaretPos)
{
	RECT	rc{};
	return bCalcDispModeCaretRect(hWnd, 0, 0, &rc, lpCaretPos);
}

//
// bCalcDispModeCaretRect()
//
BOOL		CCursor::bCalcDispModeCaretRect(HWND hForeWnd, int iModeSizeX, int iModeSizeY, LPRECT lpRect, LPPOINT lpPt)
{
	POINT	pt{};
	RECT	rcFore{}, rcTop{};
	DWORD	dwPID = 0, dwForeThreadID = 0;
	if (hForeWnd != NULL) {
		if (GetWindowRect(hForeWnd, &rcFore)) {
			if ((dwForeThreadID = GetWindowThreadProcessId(hForeWnd, &dwPID)) != 0) {
				GUITHREADINFO	GuiThredInfo{};	GuiThredInfo.cbSize = sizeof(GUITHREADINFO);
				if (GetGUIThreadInfo(dwForeThreadID, &GuiThredInfo)) {
					if (GuiThredInfo.hwndCaret != NULL) {
#define DESCENT		2
						pt.x = GuiThredInfo.rcCaret.left;	pt.y = GuiThredInfo.rcCaret.top;
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

BOOL		CCursor::bDrawIMEModeOnDisplaySub(LPIMECURSORDATA lpstCursorData)
{
#define	COUNT	20
	Sleep(50);
	BOOL	bRet = FALSE;
	BOOL	bFoundCaret = FALSE;
	RECT	rcCursor{}, rcCaret{};
	int		iCursorSizeX = 0, iCursorSizeY = 0, iCaretSizeX = 0, iCaretSizeY = 0;
	DWORD	dwIMEMode = Cime->dwIMEMode(lpstCursorData->hWndObserved, lpstCursorData->bForceHiragana);
	int		iCursor = 0;
	if (dwIMEMode != IMEOFF) {
		while (dwIMEMode != lpstCursorData->lpstFlushMouseCursor[iCursor].dwIMEMode) {
			++iCursor;
		}
		CursorWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[iCursor].szMode);
	}
	if (lpstCursorData->bDrawNearCaret != FALSE) {
		POINT	pt{};
		HWND	hWnd = GetForegroundWindow();
		if (bCalcDispModeCaretRect(hWnd, lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcCaret, &pt)) {
			hWnd = WindowFromPoint(pt);
			DWORD	dwIMEModeCaret = dwIMEMode;
			int		iCaret = iCursor;
			if (hWnd != lpstCursorData->hWndObserved) {
				dwIMEModeCaret = Cime->dwIMEMode(hWnd, lpstCursorData->bForceHiragana);
				while (dwIMEModeCaret != lpstCursorData->lpstFlushMouseCursor[iCaret].dwIMEMode) {
					++iCaret;
				}
			}
			if ((rcCaret.left != 0) || (rcCaret.top != 0) || (rcCaret.right != 0) || (rcCaret.bottom != 0)) {
				CaretWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[iCaret].szMode);
				iCaretSizeX = rcCaret.right - rcCaret.left, iCaretSizeY = rcCaret.bottom - rcCaret.top;
				if ((dwIMEModeCaret == IMEOFF) || (dwIMEModeCaret == HANEISU_IMEON) || (dwIMEModeCaret == HANKANA_IMEON)) {
					// Draw near by Caret cursor Top
					//iCaretSizeX = (iCaretSizeX * 2 + 2) / 3;	rcCaret.left = rcCaret.left + iCaretSizeX / 2;	rcCaret.right = rcCaret.left + iCaretSizeX;
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
	int	iCount = 0;
	if (lpstCursorData->dwDisplayModeTime != 0)	iCount = (lpstCursorData->dwDisplayModeTime + (COUNT - 1)) / COUNT;
	else iCount = COUNT;
	if (bFoundCaret != FALSE) {
		if (!CaretWindow->bSetWindowPos(HWND_TOPMOST, rcCaret.left, rcCaret.top, iCaretSizeX, iCaretSizeY, (SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS))) {
			bRet = FALSE;
		}
	}
	for (; iCount >= 0; iCount--) {
		if (bFoundCaret == FALSE) {	//@@@
			if (bCalcDispModeRect(lpstCursorData->iModeSize, lpstCursorData->iModeSize, &rcCursor)) {
				iCursorSizeX = rcCursor.right - rcCursor.left; iCursorSizeY = rcCursor.bottom - rcCursor.top;
				if ((dwIMEMode == IMEOFF) || (dwIMEMode == HANEISU_IMEON) || (dwIMEMode == HANKANA_IMEON)) {
					iCursorSizeX = (iCursorSizeX * 2 + 2) / 3; rcCursor.left = rcCursor.right - iCursorSizeX;
				}
				if (dwIMEMode != IMEOFF) {
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
	if (!CursorWindow->bSetWindowPos(HWND_BOTTOM, rcCursor.left, rcCursor.top, iCursorSizeX, iCursorSizeY, (SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS))) {
		bRet = FALSE;
	}
	CursorWindow->vSetModeString(stIMECursorData.lpstFlushMouseCursor[0].szMode);
	if (bFoundCaret) {
		if (!CaretWindow->bSetWindowPos(HWND_BOTTOM, rcCaret.left, rcCaret.top, iCaretSizeX, iCaretSizeY, (SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS))) {
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
	dwTextColor = aRGB(15, 254, 192, 0);
	dwBackColor = aRGB(15, 254, 192, 0);
}

CCursorWindow::~CCursorWindow()
{
	if (hCursorWindow != NULL)	 {
		DestroyWindow(hCursorWindow);
		UnregisterClass(lpszWindowClass, (HINSTANCE)GetWindowLongPtr(hCursorWindow, GWLP_HINSTANCE));
		hCursorWindow = NULL;
	}
	if (lpszWindowClass != NULL) {
		delete[]	lpszWindowClass;
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

/* = EOF = */