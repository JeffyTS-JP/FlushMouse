//
// Cursor.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/03  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "Profile.h"
#include "Cursor.h"
#include "FlushMouseLIB.h"
#include "Resource.h"
#include "CommonDef.h"
#include "Eventlog.h"
#include "CursorSub.h"
#include "..\MiscLIB\CThread.h"
#include "..\MiscLIB\CRegistry.h"

//
// Library
//

//
// Define
//
constexpr DWORD IMECURSORCHANGETHREADID = 1;
#define IMECURSORCHANGETHREADNAME	_T("IMECursorChangeThread")

constexpr DWORD DRAWIMEMODETHREADID = 2;
#define	DRAWIMEMODETHREADNAME		_T("DrawIMEModeThread")

constexpr DWORD DRAWIMEMODECARETTHREADID = 3;
#define	DRAWIMEMODECARETTHREADNAME	_T("DrawIMEModeCaretThread")

constexpr DWORD DRAWIMEMODEMOUSETHREADID = 4;
#define	DRAWIMEMODEMOUSETHREADNAME	_T("DrawIMEModeMouseThread")

//
// Struct Define
//

//
// Class Define

//
// Local Data
//
static const FLUSHMOUSECURSOR	stFlushMouseCursor[sizeof(FLUSHMOUSECURSOR)] {
	{ IMEOFF,		 _T("A"), aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), FALSE,
		{OCR_NORMAL, IDC_IMEOFF_ARROW},
		{OCR_HAND,   IDC_IMEOFF_HAND},
		{OCR_IBEAM,  IDC_IMEOFF_IBEAM}},
	{ ZENHIRA_IMEON, _T("あ"), aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), FALSE,
		{OCR_NORMAL, IDC_ZENHIRA_IMEON_ARROW},
		{OCR_HAND,   IDC_ZENHIRA_IMEON_HAND},
		{OCR_IBEAM,  IDC_ZENHIRA_IMEON_IBEAM}},
	{ HANEISU_IMEON, _T("_A"), aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), FALSE,
		{OCR_NORMAL, IDC_HANEISU_IMEON_ARROW},
		{OCR_HAND,   IDC_HANEISU_IMEON_HAND},
		{OCR_IBEAM,  IDC_HANEISU_IMEON_IBEAM}},
	{ HANKANA_IMEON, _T("_ｱ"),  aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), FALSE,
		{OCR_NORMAL, IDC_HANKANA_IMEON_ARROW},
		{OCR_HAND,   IDC_HANKANA_IMEON_HAND},
		{OCR_IBEAM,  IDC_HANKANA_IMEON_IBEAM}},
	{ ZENEISU_IMEON, _T("Ａ"), aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), FALSE,
		{OCR_NORMAL, IDC_ZENEISU_IMEON_ARROW},
		{OCR_HAND,   IDC_ZENEISU_IMEON_HAND},
		{OCR_IBEAM,  IDC_ZENEISU_IMEON_IBEAM}},
	{ ZENKANA_IMEON, _T("ア"), aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), FALSE,
		{OCR_NORMAL, IDC_ZENKANA_IMEON_ARROW},
		{OCR_HAND,   IDC_ZENKANA_IMEON_HAND},
		{OCR_IBEAM,  IDC_ZENKANA_IMEON_IBEAM}},
	{ IMEHIDE,		 _T(""), aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), FALSE,
		{OCR_NORMAL, IDC_HIDE_ARROW},
		{OCR_HAND,   IDC_HIDE_HAND},
		{OCR_IBEAM,  IDC_HIDE_IBEAM}},
	{ IMEOFF,		 _T("A"), aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), TRUE,
		{OCR_NORMAL, IDC_IMEOFF_ARROW_CAPSON},
		{OCR_HAND,   IDC_IMEOFF_HAND_CAPSON},
		{OCR_IBEAM,  IDC_IMEOFF_IBEAM_CAPSON}},
	{ ZENHIRA_IMEON, _T("あ"), aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), TRUE,
		{OCR_NORMAL, IDC_ZENHIRA_IMEON_ARROW_CAPSON},
		{OCR_HAND,   IDC_ZENHIRA_IMEON_HAND_CAPSON},
		{OCR_IBEAM,  IDC_ZENHIRA_IMEON_IBEAM_CAPSON}},
	{ HANEISU_IMEON, _T("_A"), aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), TRUE,
		{OCR_NORMAL, IDC_HANEISU_IMEON_ARROW_CAPSON},
		{OCR_HAND,   IDC_HANEISU_IMEON_HAND_CAPSON},
		{OCR_IBEAM,  IDC_HANEISU_IMEON_IBEAM_CAPSON}},
	{ HANKANA_IMEON, _T("_ｱ"),  aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), TRUE,
		{OCR_NORMAL, IDC_HANKANA_IMEON_ARROW_CAPSON},
		{OCR_HAND,   IDC_HANKANA_IMEON_HAND_CAPSON},
		{OCR_IBEAM,  IDC_HANKANA_IMEON_IBEAM_CAPSON}},
	{ ZENEISU_IMEON, _T("Ａ"), aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), TRUE,
		{OCR_NORMAL, IDC_ZENEISU_IMEON_ARROW_CAPSON},
		{OCR_HAND,   IDC_ZENEISU_IMEON_HAND_CAPSON},
		{OCR_IBEAM,  IDC_ZENEISU_IMEON_IBEAM_CAPSON}},
	{ ZENKANA_IMEON, _T("ア"), aRGB(48, 255, 0, 0), _T("Yu Gothic UI"), TRUE,
		{OCR_NORMAL, IDC_ZENKANA_IMEON_ARROW_CAPSON},
		{OCR_HAND,   IDC_ZENKANA_IMEON_HAND_CAPSON},
		{OCR_IBEAM,  IDC_ZENKANA_IMEON_IBEAM_CAPSON}},
	{ (DWORD)(-1),	 _T(""), aRGB(0, 0, 0, 0), L"",	FALSE,
		{0,			 0},
		{0,			 0},
		{0,			 0}}
};

//
// Local Prototype Define
//

//
// class CCursor
//
CCursor::CCursor()
	: stIMECursorData(), m_hMainWnd(NULL), hCursorData(NULL), iCursorDataLoadCount(0),
	IMECursorChangeThread(NULL), DrawIMEModeThread(NULL), DrawIMEModeCaretThread(NULL), DrawIMEModeMouseByWndThread(NULL),
	uuMouseWindowTick(GetTickCount64()), CursorSub(NULL)
{
	ZeroMemory(&stIMECursorData, sizeof(stIMECursorData));
	stIMECursorData.dwIMEModeCursor = (DWORD)(-1);
	stIMECursorData.dwIMEModeMouseWindow = IMEHIDE;
	
	stIMECursorData.lpstNearDrawMouseCursor = static_cast<LPFLUSHMOUSECURSOR>(new FLUSHMOUSECURSOR[sizeof(stFlushMouseCursor)]);
	if (stIMECursorData.lpstNearDrawMouseCursor) {
		ZeroMemory(stIMECursorData.lpstNearDrawMouseCursor, sizeof(stFlushMouseCursor));
		memcpy_s(stIMECursorData.lpstNearDrawMouseCursor, sizeof(stFlushMouseCursor), stFlushMouseCursor, sizeof(stFlushMouseCursor));
	}
	stIMECursorData.lpstNearDrawCaretCursor = static_cast<LPFLUSHMOUSECURSOR>(new FLUSHMOUSECURSOR[sizeof(stFlushMouseCursor)]); 
	if (stIMECursorData.lpstNearDrawCaretCursor) {
		ZeroMemory(stIMECursorData.lpstNearDrawCaretCursor, sizeof(stFlushMouseCursor));
		memcpy_s(stIMECursorData.lpstNearDrawCaretCursor, sizeof(stFlushMouseCursor), stFlushMouseCursor, sizeof(stFlushMouseCursor));
	}
	stIMECursorData.lpstNearDrawMouseByWndCursor = static_cast<LPFLUSHMOUSECURSOR>(new FLUSHMOUSECURSOR[sizeof(stFlushMouseCursor)]);
	if (stIMECursorData.lpstNearDrawMouseByWndCursor) {
		ZeroMemory(stIMECursorData.lpstNearDrawMouseByWndCursor, sizeof(stFlushMouseCursor));
		memcpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor, sizeof(stFlushMouseCursor), stFlushMouseCursor, sizeof(stFlushMouseCursor));
	}

	stIMECursorData.hCursorArrow = LoadCursor(NULL, IDC_ARROW);
	stIMECursorData.hCursorIBeam = LoadCursor(NULL, IDC_IBEAM);
	stIMECursorData.hCursorWait = LoadCursor(NULL, IDC_WAIT);
	stIMECursorData.hCursorHand = LoadCursor(NULL, IDC_HAND);
	stIMECursorData.hCursorAppStarting = LoadCursor(NULL, IDC_APPSTARTING);
	stIMECursorData.hCursorSizeNWSE = LoadCursor(NULL, IDC_SIZENWSE);
	stIMECursorData.hCursorSizeNESW = LoadCursor(NULL, IDC_SIZENESW);
	stIMECursorData.hCursorSizeWE = LoadCursor(NULL, IDC_SIZEWE);
	stIMECursorData.hCursorSizeNS = LoadCursor(NULL, IDC_SIZENS);
	stIMECursorData.hCursorSizeAll = LoadCursor(NULL, IDC_SIZEALL);
	stIMECursorData.hCursorHand = LoadCursor(NULL, IDC_HAND);
	stIMECursorData.hCursorAppStarting = LoadCursor(NULL, IDC_APPSTARTING);

	if (GetKeyState(VK_CAPITAL) & 0x0001) stIMECursorData.bCapsLock = TRUE;	else stIMECursorData.bCapsLock = FALSE;
}

CCursor::~CCursor()
{
	if (DrawIMEModeThread != NULL) {
		DrawIMEModeThread->bSetSentinel(FALSE);
		delete	DrawIMEModeThread;
		DrawIMEModeThread = NULL;
	}
	if (DrawIMEModeCaretThread != NULL) {
		DrawIMEModeCaretThread->bSetSentinel(FALSE);
		delete	DrawIMEModeCaretThread;
		DrawIMEModeCaretThread = NULL;
	}
	if (stIMECursorData.CursorWindow != NULL) {
		delete	stIMECursorData.CursorWindow;
		stIMECursorData.CursorWindow = NULL;
	}
	if (stIMECursorData.CaretWindow != NULL) {
		delete	stIMECursorData.CaretWindow;
		stIMECursorData.CaretWindow = NULL;
	}
	if (stIMECursorData.MouseWindow != NULL) {
		delete	stIMECursorData.MouseWindow;
		stIMECursorData.MouseWindow = NULL;
	}
	vUnRegisterDrawIMEModeMouseByWndThread();
	vUnRegisterIMECursorChangeThread();
	if (stIMECursorData.lpstNearDrawMouseCursor != NULL) {
		delete []	stIMECursorData.lpstNearDrawMouseCursor;
		stIMECursorData.lpstNearDrawMouseCursor = NULL;
	}
	if (stIMECursorData.lpstNearDrawCaretCursor != NULL) {
		delete []	stIMECursorData.lpstNearDrawCaretCursor;
		stIMECursorData.lpstNearDrawCaretCursor = NULL;
	}
	if (stIMECursorData.lpstNearDrawMouseByWndCursor != NULL) {
		delete []	stIMECursorData.lpstNearDrawMouseByWndCursor;
		stIMECursorData.lpstNearDrawMouseByWndCursor = NULL;
	}
	if (CursorSub)	delete CursorSub;
	CursorSub = NULL;
}

//
// bInitialize()
//
BOOL			CCursor::bInitialize(HWND hWnd)
{
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_TOPMOST)
	if (!hWnd)	return FALSE;
	m_hMainWnd = hWnd;
	CursorSub = new CCursorSub();
	if (!CursorSub)	return FALSE;
	if (!CursorSub->bInitialize(hWnd, FLUSHMOUSECURSOR_DAT))	return FALSE;
	if (!bReloadCursor()) {
		vMessageBox(hWnd, IDS_CANTLOADCURSOR, MessageBoxTYPE, __func__, __LINE__);
		return FALSE;
	}
	return TRUE;
}

//
// bReloadCursor()
//
BOOL			CCursor::bReloadCursor()
{
	BOOL		bRet = FALSE;
	vUnRegisterDrawIMEModeMouseByWndThread();

	if (stIMECursorData.dwDisplayIMEModeMethod == DisplayIMEModeMethod_RESOURCE) {
		SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
	}

	if (!CursorSub->bUnLoadCursorData())	goto Cleanup;

	vSetParamFromRegistry();

	if (!CursorSub->bMakeAllCursor(stIMECursorData.lpstNearDrawMouseByWndCursor))	goto Cleanup;
	
	if (!CursorSub->hLoadCursorData())	goto Cleanup;
	
	if (!stIMECursorData.CursorWindow) {
		stIMECursorData.CursorWindow = new CCursorWindow;
		if (stIMECursorData.CursorWindow == NULL)	goto Cleanup;
		if (!stIMECursorData.CursorWindow->bRegister((HINSTANCE)GetWindowLongPtr(m_hMainWnd, GWLP_HINSTANCE), CLASS_CURSORWINDOW))	goto Cleanup;
	}
	stIMECursorData.CursorWindow->vSetModeStringColorFont(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_IMEHIDE].szMode, stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_IMEHIDE].dwColor, stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_IMEHIDE].szFont);
	
	if (!stIMECursorData.CaretWindow) {
		stIMECursorData.CaretWindow = new CCursorWindow;
		if (stIMECursorData.CaretWindow == NULL)	goto Cleanup;
		if (!stIMECursorData.CaretWindow->bRegister((HINSTANCE)GetWindowLongPtr(m_hMainWnd, GWLP_HINSTANCE), CLASS_CARETWINDOW))	goto Cleanup;
	}
	stIMECursorData.CaretWindow->vSetModeStringColorFont(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_IMEHIDE].szMode, stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_IMEHIDE].dwColor, stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_IMEHIDE].szFont);
	
	if (stIMECursorData.MouseWindow) {
		delete	stIMECursorData.MouseWindow;
		stIMECursorData.MouseWindow = NULL;
	}
	if (!stIMECursorData.MouseWindow) {
		stIMECursorData.MouseWindow = new CCursorWindow;
		if (stIMECursorData.MouseWindow == NULL)	goto Cleanup;
		if (!stIMECursorData.MouseWindow->bRegister((HINSTANCE)GetWindowLongPtr(m_hMainWnd, GWLP_HINSTANCE), CLASS_MOUSEWINDOW))	goto Cleanup;
	}
	stIMECursorData.MouseWindow->vSetModeStringColorFont(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_IMEHIDE].szMode, stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_IMEHIDE].dwColor, stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_IMEHIDE].szFont);
	
	if (!DrawIMEModeCaretThread) {
		DrawIMEModeCaretThread = new CThread;
		if (DrawIMEModeCaretThread == NULL)	goto Cleanup;
	}

	if (!DrawIMEModeThread) {
		DrawIMEModeThread = new CThread;
		if (DrawIMEModeThread != NULL) {
			if (!bRegisterDrawIMEModeThread(m_hMainWnd))	goto Cleanup;
		}
	}

	if (stIMECursorData.dwDisplayIMEModeMethod != DisplayIMEModeMethod_RESOURCE) {
		vUnRegisterIMECursorChangeThread();
		if (!bRegisterDrawIMEModeMouseByWndThread(m_hMainWnd))	goto Cleanup;
		stIMECursorData.uuMouseWindowDiffTick = stIMECursorData.uuMouseWindowDiffTickDrawByWindow;
		if (!bStartIMECursorChangeThread(GetForegroundWindow()))	goto Cleanup;
	}
	else {
		vUnRegisterDrawIMEModeMouseByWndThread();
		if (!bRegisterIMECursorChangeThread(m_hMainWnd)) goto Cleanup;
		stIMECursorData.uuMouseWindowDiffTick = stIMECursorData.uuMouseWindowDiffTickDrawByResource;
		if (!bStartIMECursorChangeThread(GetForegroundWindow()))	goto Cleanup;
	}

	bRet = TRUE;

Cleanup:
	if (!bRet) {
		SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
	}
	return bRet;
}

//
// vSetParamFromRegistry()
//
VOID			CCursor::vSetParamFromRegistry()
{
	if (Profile && Profile->lpstAppRegData) {
		stIMECursorData.iCursorSize = Profile->lpstAppRegData->iCursorSize;
		stIMECursorData.iIMEModeDistance = Profile->lpstAppRegData->iIMEModeDistance;
		stIMECursorData.iModeMouseSize = Profile->lpstAppRegData->iModeMouseSize;
		stIMECursorData.iModeCaretSize = Profile->lpstAppRegData->iModeCaretSize;
		stIMECursorData.iModeByWndSize = Profile->lpstAppRegData->iModeByWndSize;
		stIMECursorData.iModeMouseDistanceX = Profile->lpstAppRegData->iModeMouseDistanceX;
		stIMECursorData.iModeCaretDistanceX = Profile->lpstAppRegData->iModeCaretDistanceX;
		stIMECursorData.dwInThreadSleepTime = Profile->lpstAppRegData->dwInThreadSleepTime;
		stIMECursorData.dwDisplayModeTime = Profile->lpstAppRegData->dwDisplayModeTime;
		stIMECursorData.bDisplayFocusWindowIME = Profile->lpstAppRegData->bDisplayFocusWindowIME;
		stIMECursorData.bDisplayIMEModeOnCursor = Profile->lpstAppRegData->bDisplayIMEModeOnCursor;
		stIMECursorData.dwDisplayIMEModeMethod = Profile->lpstAppRegData->dwDisplayIMEModeMethod;
		stIMECursorData.bDisplayIMEModeIMEOFF = Profile->lpstAppRegData->bDisplayIMEModeIMEOFF;
		stIMECursorData.bForceHiragana = Profile->lpstAppRegData->bForceHiragana;
		stIMECursorData.bDrawNearCaret = Profile->lpstAppRegData->bDrawNearCaret;
		stIMECursorData.bSupportVirtualDesktop = Profile->lpstAppRegData->bSupportVirtualDesktop;
		stIMECursorData.uuMouseWindowDiffTickDrawByResource = Profile->lpstAppRegData->uuMouseWindowDiffTickDrawByResource;
		stIMECursorData.uuMouseWindowDiffTickDrawByWindow = Profile->lpstAppRegData->uuMouseWindowDiffTickDrawByWindow;
		stIMECursorData.uuMouseWindowDiffTickNotDrawing = Profile->lpstAppRegData->uuMouseWindowDiffTickNotDrawing;
		stIMECursorData.uuMouseWindowDiffTickInVDT = Profile->lpstAppRegData->uuMouseWindowDiffTickInVDT;

		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_IMEOFF].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseIMEOFFColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseIMEOFFColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_HANEISU_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_HANKANA_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENEISU_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENHIRA_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENKANA_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_IMEOFF_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseIMEOFFColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseIMEOFFColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_HANEISU_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_HANKANA_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENEISU_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENHIRA_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENKANA_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_IMEOFF_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseIMEOFFColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseIMEOFFColor & 0x00ffffff);

		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_IMEOFF].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretIMEOFFColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretIMEOFFColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_HANEISU_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_HANKANA_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENEISU_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENHIRA_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENKANA_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_IMEOFF_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretIMEOFFColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretIMEOFFColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_HANEISU_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_HANKANA_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENEISU_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENHIRA_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENKANA_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor & 0x00ffffff);

		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_IMEOFF].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_HANEISU_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_HANKANA_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENEISU_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENHIRA_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENKANA_IMEON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_IMEOFF_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_HANEISU_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_HANKANA_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENEISU_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENHIRA_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor & 0x00ffffff);
		stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENKANA_IMEON_CAPSON].dwColor = (~Profile->lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor & 0xff000000) | (Profile->lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor & 0x00ffffff);

		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_IMEOFF].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseIMEOFFChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENHIRA_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseZENHIRA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_HANEISU_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseHANEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_HANKANA_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseHANKANA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENEISU_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseZENEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENKANA_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseZENKANA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_IMEOFF_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseIMEOFFChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENHIRA_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseZENHIRA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_HANEISU_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseHANEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_HANKANA_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseHANKANA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENEISU_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseZENEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_ZENKANA_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseZENKANA_IMEONChar, _TRUNCATE);

		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_IMEOFF].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretIMEOFFChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENHIRA_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretZENHIRA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_HANEISU_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretHANEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_HANKANA_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretHANKANA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENEISU_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretZENEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENKANA_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretZENKANA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_IMEOFF_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretIMEOFFChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENHIRA_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretZENHIRA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_HANEISU_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretHANEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_HANKANA_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretHANKANA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENEISU_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretZENEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_ZENKANA_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawCaretZENKANA_IMEONChar, _TRUNCATE);

		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_IMEOFF].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndIMEOFFChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENHIRA_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndZENHIRA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_HANEISU_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndHANEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_HANKANA_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndHANKANA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENEISU_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndZENEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENKANA_IMEON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndZENKANA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_IMEOFF_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndIMEOFFChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENHIRA_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndZENHIRA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_HANEISU_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndHANEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_HANKANA_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndHANKANA_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENEISU_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndZENEISU_IMEONChar, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_ZENKANA_IMEON_CAPSON].szMode, MAX_IMEMODECHAR, Profile->lpstAppRegData->szNearDrawMouseByWndZENKANA_IMEONChar, _TRUNCATE);

		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_IMEOFF].szFont, LF_FACESIZE, Profile->lpstAppRegData->szNearDrawMouseFont, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_IMEOFF].szFont, LF_FACESIZE, Profile->lpstAppRegData->szNearDrawCaretFont, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_IMEOFF].szFont, LF_FACESIZE, Profile->lpstAppRegData->szNearDrawMouseByWndFont, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseCursor[IMEMODE_IMEOFF_CAPSON].szFont, LF_FACESIZE, Profile->lpstAppRegData->szNearDrawMouseFont, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawCaretCursor[IMEMODE_IMEOFF_CAPSON].szFont, LF_FACESIZE, Profile->lpstAppRegData->szNearDrawCaretFont, _TRUNCATE);
		_tcsncpy_s(stIMECursorData.lpstNearDrawMouseByWndCursor[IMEMODE_IMEOFF_CAPSON].szFont, LF_FACESIZE, Profile->lpstAppRegData->szNearDrawMouseByWndFont, _TRUNCATE);

		stIMECursorData.bDenyChangedByApp = Profile->lpstAppRegData->bDenyChangedByApp;
		stIMECursorData.bUseBigArrow = Profile->lpstAppRegData->bUseBigArrow;
	}
}	

//
// bStartIMECursorChangeThread()
//
BOOL		CCursor::bStartIMECursorChangeThread(HWND hWndObserved)
{
	stIMECursorData.dwWaitTimeMouseWindow = 0;
	stIMECursorData.bIMEModeByWindowThreadSentinel = FALSE;
	return bStartIMECursorChangeThreadSub(hWndObserved);
}

//
// bStartIMECursorChangeThreadWait()
//
BOOL		CCursor::bStartIMECursorChangeThreadWait(HWND hWndObserved, DWORD dwWaitTime)
{
	stIMECursorData.dwWaitTimeMouseWindow = dwWaitTime;
	stIMECursorData.bIMEModeByWindowThreadSentinel = FALSE;
	return bStartIMECursorChangeThreadSub(hWndObserved);
}

//
// bStartIMECursorChangeThreadTimer()
//
BOOL		CCursor::bStartIMECursorChangeThreadTimer(HWND hWndObserved)
{
	if (stIMECursorData.bIMEModeByWindowThreadSentinel)	return TRUE;
	stIMECursorData.dwWaitTimeMouseWindow = 0;
	return bStartIMECursorChangeThreadSub(hWndObserved);
}

//
// bStartIMECursorChangeThreadRawInput()
//
BOOL		CCursor::bStartIMECursorChangeThreadRawInput(HWND hWndObserved)
{
	ULONGLONG	_uuMouseWindowTick = GetTickCount64();
	if ((_uuMouseWindowTick - uuMouseWindowTick) < stIMECursorData.uuMouseWindowDiffTick) {
		return TRUE;
	}
	uuMouseWindowTick = _uuMouseWindowTick;
	stIMECursorData.dwWaitTimeMouseWindow = 0;
	stIMECursorData.bIMEModeByWindowThreadSentinel = FALSE;
	return bStartIMECursorChangeThreadSub(hWndObserved);
}

//
// bStartIMECursorChangeThreadSub()
//
BOOL		CCursor::bStartIMECursorChangeThreadSub(HWND hWndObserved)
{
	vSetParamFromRegistry();
	stIMECursorData.hWndObserved = hWndObserved;
	if (Profile->lpstAppRegData->bDisplayIMEModeOnCursor && (Profile->lpstAppRegData->dwDisplayIMEModeMethod != DisplayIMEModeMethod_RESOURCE)) {
		if (!stIMECursorData.MouseWindow) {
			if (!bReloadCursor())	return FALSE;
		}
		if (DrawIMEModeMouseByWndThread == NULL) {
			if (!bRegisterDrawIMEModeMouseByWndThread(m_hMainWnd))	return FALSE;
		}
		if (stIMECursorData.MouseWindow && (stIMECursorData.MouseWindow->hGetHWND() == hWndObserved))	return TRUE;
		if (Profile->lpstAppRegData->dwDisplayIMEModeMethod == DisplayIMEModeMethod_ByWindow) {
			stIMECursorData.uuMouseWindowDiffTick = stIMECursorData.uuMouseWindowDiffTickDrawByWindow;
		}
		else if (Profile->lpstAppRegData->dwDisplayIMEModeMethod == DisplayIMEModeMethod_RES_AND_Window) {
			stIMECursorData.uuMouseWindowDiffTick = stIMECursorData.uuMouseWindowDiffTickDrawByResource;
		}
		if (!DrawIMEModeMouseByWndThread->bStart())	return FALSE;
		return TRUE;
	}
	else {
		if (IMECursorChangeThread == NULL) {
			if (!bRegisterIMECursorChangeThread(m_hMainWnd))	return FALSE;
		}
		if (stIMECursorData.bIMEModeByWindowThreadSentinel)	return TRUE;
		if (!IMECursorChangeThread->bStart())	return FALSE;
	}
	return TRUE;
}

//
// bStartDrawIMEModeThread()
//
BOOL		CCursor::bStartDrawIMEModeThread(HWND hWndObserved)
{
	stIMECursorData.bDrawIMEModeWait = FALSE;
	stIMECursorData.dwDrawIMEModeWaitTime = 0;
	return bStartDrawIMEModeThreadSub(hWndObserved);
}

//
// bStartDrawIMEModeThreadWait()
//
BOOL		CCursor::bStartDrawIMEModeThreadWait(HWND hWndObserved, DWORD dwWaitTime)
{
	stIMECursorData.bDrawIMEModeWait = TRUE;
	stIMECursorData.dwDrawIMEModeWaitTime = dwWaitTime;
	return bStartDrawIMEModeThreadSub(hWndObserved);
}

//
// bStartDrawIMEModeThreadSub()
//
BOOL		CCursor::bStartDrawIMEModeThreadSub(HWND hWndObserved)
{
	if (!stIMECursorData.CursorWindow || !stIMECursorData.CaretWindow)	return FALSE;
	if (stIMECursorData.CursorWindow->hGetHWND() == hWndObserved || stIMECursorData.CaretWindow->hGetHWND() == hWndObserved)	return TRUE;
	vSetParamFromRegistry();
	if (stIMECursorData.bSupportVirtualDesktop) {
		TCHAR	szBuffer[_MAX_PATH]{};
		if (GetClassName(hWndObserved, szBuffer, _MAX_PATH) != 0 && szBuffer[0] != _T('\0')) {
			static const LPCTSTR szTargetClasses[] = { L"VMUIFrame", L"VMPlayerFrame", L"TscShellContainerClass" };
			if (std::any_of(std::begin(szTargetClasses), std::end(szTargetClasses), [&](const LPCTSTR szClassName) {
				return _tcscmp(szBuffer, szClassName) == 0;})) {
				stIMECursorData.bDisplayIMEModeOnCursor = FALSE;
				stIMECursorData.bDisplayIMEModeIMEOFF = FALSE;
				stIMECursorData.bDrawNearCaret = FALSE;
				stIMECursorData.bIMEModeByWindowThreadSentinel = FALSE;
				SleepEx(50, FALSE);
				if (stIMECursorData.MouseWindow)	stIMECursorData.MouseWindow->bSetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DEFERERASE | SWP_HIDEWINDOW | SWP_NOACTIVATE);
				if (stIMECursorData.CursorWindow)	stIMECursorData.CursorWindow->bSetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DEFERERASE | SWP_HIDEWINDOW | SWP_NOACTIVATE);
				if (stIMECursorData.CaretWindow)	stIMECursorData.CaretWindow->bSetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DEFERERASE | SWP_HIDEWINDOW | SWP_NOACTIVATE);
				bChangeFlushMouseCursor(IMEHIDE, &stIMECursorData, FALSE);
				return TRUE;
			}
		}
	}
	stIMECursorData.hWndObserved = hWndObserved;
	if (!DrawIMEModeThread) {
		DrawIMEModeThread = new CThread;
		if (DrawIMEModeThread != NULL) {
			if (!bRegisterDrawIMEModeThread(m_hMainWnd))	return FALSE;
		}
	}
	if (stIMECursorData.bDrawNearCaret)	stIMECursorData.hWndCaret = hGetCaretPosByAccessibleObjectFromWindow(GetForegroundWindow(), &stIMECursorData.rcCaret, FALSE);
	else stIMECursorData.hWndCaret = NULL;
	if (!DrawIMEModeThread->bStart()) {
		delete	DrawIMEModeThread;
		DrawIMEModeThread = NULL;
		DrawIMEModeThread = new CThread;
		if (!DrawIMEModeThread)	return FALSE;
		if (!bRegisterDrawIMEModeThread(m_hMainWnd)) return FALSE;
		if (!DrawIMEModeThread->bStart())	return FALSE;
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
		if (!IMECursorChangeThread)	return FALSE;
		if (!bChangeFlushMouseCursor(IMEHIDE, &stIMECursorData, FALSE)) return FALSE;
		stIMECursorData.hWndObserved = hWnd;
		if (!IMECursorChangeThread->bRegister(IMECURSORCHANGETHREADNAME, IMECURSORCHANGETHREADID,
										(LPTHREAD_START_ROUTINE)&bIMECursorChangeRoutine, &stIMECursorData, stIMECursorData.dwInThreadSleepTime)) {
			return	FALSE;
		}
		if (!bChangeFlushMouseCursor(IMEHIDE, &stIMECursorData, FALSE)) return FALSE;
	}
	return	TRUE;
}

//
// vUnRegisterIMECursorChangeThread()
//
VOID		CCursor::vUnRegisterIMECursorChangeThread()
{
	if (IMECursorChangeThread != NULL) {
		IMECursorChangeThread->bSetSentinel(FALSE);
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
	LPIMECURSORDATA	lpstCursorData = reinterpret_cast<LPIMECURSORDATA>(lpvParam);
	CCursor	*This = reinterpret_cast<CCursor*>(lpvParam);

	BOOL	bRet = TRUE;
	if (lpstCursorData->bDisplayIMEModeOnCursor && lpstCursorData->dwDisplayIMEModeMethod == DisplayIMEModeMethod_RESOURCE) {
		This->bIsIMECursorChanged(lpstCursorData);
		if ((lpstCursorData->dwIMEModeCursor == IMEOFF) && !lpstCursorData->bDisplayIMEModeIMEOFF) lpstCursorData->dwIMEModeCursor = IMEHIDE;
		BOOL	bCapsLock = (GetKeyState(VK_CAPITAL) & 0x0001) ? TRUE : FALSE;
		if (!This->bChangeFlushMouseCursor(lpstCursorData->dwIMEModeCursor, lpstCursorData, bCapsLock)) {
			bRet = FALSE;
		}
	}
	else {
		if (!This->bChangeFlushMouseCursor(IMEHIDE, lpstCursorData, FALSE)) {
			bRet = FALSE;
		}
	}
	SleepEx(0, TRUE);
	return bRet;
}

//
// bRegisterDrawIMEModeMouseByWndThread()
//
BOOL		CCursor::bRegisterDrawIMEModeMouseByWndThread(HWND hWnd)
{
	if (!stIMECursorData.MouseWindow || !Profile)	return FALSE;
	stIMECursorData.bIMEModeByWindowThreadSentinel = FALSE;
	SleepEx(50, FALSE);
	vSetParamFromRegistry();
	if (DrawIMEModeMouseByWndThread == NULL) {
		DrawIMEModeMouseByWndThread = new CThread;
		if (!DrawIMEModeMouseByWndThread)	return FALSE;
		stIMECursorData.hWndObserved = hWnd;
		if (!DrawIMEModeMouseByWndThread->bRegister(DRAWIMEMODEMOUSETHREADNAME, DRAWIMEMODEMOUSETHREADID,
			(LPTHREAD_START_ROUTINE)&bIMEModeMouseByWndThreadRoutine, &stIMECursorData, stIMECursorData.dwInThreadSleepTime)) {
			return	FALSE;
		}
	}
	else return TRUE;
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
	if (!stIMECursorData.MouseWindow->bSetWndThreadPriority(THREAD_PRIORITY_TIME_CRITICAL)) {
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
	return	TRUE;
}

//
// vUnRegisterDrawIMEModeMouseByWndThread()
//
VOID		CCursor::vUnRegisterDrawIMEModeMouseByWndThread()
{
	vStopIMECursorChangeThread();
	if (stIMECursorData.MouseWindow != NULL) {
		delete	stIMECursorData.MouseWindow;
		stIMECursorData.MouseWindow = NULL;
	}
	if (DrawIMEModeMouseByWndThread != NULL) {
		DrawIMEModeMouseByWndThread->bSetSentinel(FALSE);
		delete	DrawIMEModeMouseByWndThread;
		DrawIMEModeMouseByWndThread = NULL;
	}
}

//
// vStopIMECursorChangeThread()
//
VOID		CCursor::vStopIMECursorChangeThread()
{
	if (DrawIMEModeMouseByWndThread)	DrawIMEModeMouseByWndThread->bSetSentinel(FALSE);
	if (stIMECursorData.MouseWindow)	stIMECursorData.MouseWindow->bShowWindow(SW_HIDE);
}

//
// bIMEModeMouseByWndThreadRoutine()
//
BOOL CCursor::bIMEModeMouseByWndThreadRoutine(LPVOID lpvParam)
{
	if (!lpvParam)	return FALSE;
	LPIMECURSORDATA	lpstCursorData = reinterpret_cast<LPIMECURSORDATA>(lpvParam);
	CCursor*	This = reinterpret_cast<CCursor*>(lpvParam);

	lpstCursorData->bIMEModeByWindowThreadSentinel = TRUE;

	BOOL		bRet = TRUE;
	CURSORINFO	CursorInfo{ CursorInfo.cbSize = sizeof(CURSORINFO) };

	if (lpstCursorData->dwWaitTimeMouseWindow) {
		SleepEx(lpstCursorData->dwWaitTimeMouseWindow, TRUE);
		if (!lpstCursorData->bIMEModeByWindowThreadSentinel) return TRUE;
	}
	do {
		if (!lpstCursorData->bDisplayIMEModeOnCursor) break;
		if (!lpstCursorData->MouseWindow)  { bRet = FALSE;	break; }
		CursorInfo.cbSize = sizeof(CURSORINFO);
		if (!GetCursorInfo(&CursorInfo)) { bRet = FALSE; break; }
		if (lpstCursorData->bSupportVirtualDesktop) {
			static const LPCTSTR szTargetClasses[] = { L"VMUIFrame", L"VMPlayerFrame", L"TscShellContainerClass" };
			constexpr int iClassCount = sizeof(szTargetClasses) / sizeof(szTargetClasses[0]);
			HWND hTargetWnd[iClassCount]{};
			for (int i = 0; i < iClassCount; ++i) {
				hTargetWnd[i] = FindWindowEx(NULL, NULL, szTargetClasses[i], NULL);
			}
			std::function<BOOL()> hideCursorAndReturn = [&]() {
				if (lpstCursorData->MouseWindow)	if (!lpstCursorData->MouseWindow->bSetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DEFERERASE | SWP_HIDEWINDOW | SWP_NOACTIVATE))	return FALSE;
				if (lpstCursorData->CursorWindow)	if (!lpstCursorData->CursorWindow->bSetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DEFERERASE | SWP_HIDEWINDOW | SWP_NOACTIVATE))	return FALSE;
				if (lpstCursorData->CaretWindow)	if (!lpstCursorData->CaretWindow->bSetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DEFERERASE | SWP_HIDEWINDOW | SWP_NOACTIVATE))	return FALSE;
				if (!This->bChangeFlushMouseCursor(IMEHIDE, lpstCursorData, FALSE)) return FALSE;
				lpstCursorData->uuMouseWindowDiffTick = lpstCursorData->uuMouseWindowDiffTickInVDT;
				lpstCursorData->bIMEModeByWindowThreadSentinel = FALSE;
				SleepEx(0, TRUE);
				return TRUE;
				};
			HWND	hWndAtPoint = WindowFromPoint(CursorInfo.ptScreenPos);
			HWND	hWndForeground = GetForegroundWindow();
			for (int i = 0; i < iClassCount; ++i) {
				if (hWndAtPoint == hTargetWnd[i] || hWndForeground == hTargetWnd[i]) {
					return hideCursorAndReturn();
				}
			}
			TCHAR	szBuffer[_MAX_PATH]{};
			if (GetClassName(lpstCursorData->hWndObserved, szBuffer, _MAX_PATH) != 0 && szBuffer[0] != _T('\0')) {
				for (int i = 0; i < iClassCount; ++i) {
					if (_tcscmp(szBuffer, szTargetClasses[i]) == 0) {
						return hideCursorAndReturn();
					}
				}
			}
		}
		const HCURSOR	hCur = CursorInfo.hCursor;
		if (CursorInfo.flags != CURSOR_SHOWING
			|| hCur == lpstCursorData->hCursorWait || hCur == lpstCursorData->hCursorAppStarting || hCur == lpstCursorData->hCursorSizeNWSE
			|| hCur == lpstCursorData->hCursorSizeNESW || hCur == lpstCursorData->hCursorSizeWE || hCur == lpstCursorData->hCursorSizeNS || hCur == lpstCursorData->hCursorSizeAll) {
			if (!lpstCursorData->MouseWindow)  { bRet = FALSE;	break; }
			if (!lpstCursorData->MouseWindow->bSetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DEFERERASE | SWP_HIDEWINDOW | SWP_NOACTIVATE)) { bRet = FALSE; break; }
			lpstCursorData->uuMouseWindowDiffTick = lpstCursorData->uuMouseWindowDiffTickNotDrawing;
			break;
		}
		DWORD	dwIMEMode = Cime->dwIMEMode(lpstCursorData->hWndObserved, lpstCursorData->bForceHiragana);
		if ((dwIMEMode == IMEOFF) && !lpstCursorData->bDisplayIMEModeIMEOFF) dwIMEMode = IMEHIDE;
		BOOL	bCapsLock = (GetKeyState(VK_CAPITAL) & 0x0001) ? TRUE : FALSE;
		if ((hCur == lpstCursorData->hCursorArrow || hCur == lpstCursorData->hCursorIBeam || hCur == lpstCursorData->hCursorHand)) {
			if (lpstCursorData->dwDisplayIMEModeMethod == DisplayIMEModeMethod_ByWindow) {
				if(!This->bChangeFlushMouseCursor(IMEHIDE, lpstCursorData, FALSE)) { bRet = FALSE; break; }
				lpstCursorData->uuMouseWindowDiffTick = lpstCursorData->uuMouseWindowDiffTickDrawByWindow;
			}
			else if (lpstCursorData->dwDisplayIMEModeMethod == DisplayIMEModeMethod_RES_AND_Window) {
				if (!lpstCursorData->MouseWindow)  { bRet = FALSE;	break; }
				lpstCursorData->MouseWindow->bSetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DEFERERASE | SWP_HIDEWINDOW | SWP_NOACTIVATE);
				if (!This->bChangeFlushMouseCursor(dwIMEMode, lpstCursorData, bCapsLock)) { bRet = FALSE; break; }
				lpstCursorData->uuMouseWindowDiffTick = lpstCursorData->uuMouseWindowDiffTickDrawByResource;
				break;
			}
		}
		int		iMouse = iGetCursorID(dwIMEMode, lpstCursorData->lpstNearDrawMouseByWndCursor, bCapsLock);
		if (!lpstCursorData->MouseWindow)  { bRet = FALSE;	break; }
		lpstCursorData->MouseWindow->vSetModeStringColorFont(lpstCursorData->lpstNearDrawMouseByWndCursor[iMouse].szMode, lpstCursorData->lpstNearDrawMouseByWndCursor[iMouse].dwColor, lpstCursorData->lpstNearDrawMouseByWndCursor[iMouse].szFont);
		RECT	rcMouse{
			CursorInfo.ptScreenPos.x + lpstCursorData->iModeByWndSize + lpstCursorData->iIMEModeDistance,
			CursorInfo.ptScreenPos.y + lpstCursorData->iModeByWndSize + lpstCursorData->iIMEModeDistance,
			CursorInfo.ptScreenPos.x + lpstCursorData->iModeByWndSize * 2 + lpstCursorData->iIMEModeDistance,
			CursorInfo.ptScreenPos.y + lpstCursorData->iModeByWndSize * 2 + lpstCursorData->iIMEModeDistance
		};
		rcMouse.right = rcMouse.left + lpstCursorData->iModeByWndSize;
		rcMouse.bottom = rcMouse.top + lpstCursorData->iModeByWndSize;
		if (!This->bAdjustModeSizeByMonitorDPIAsync()) break;
		if (!This->bAdjustModeSizeByMonitorDPI(lpstCursorData->iModeByWndSize, lpstCursorData->iModeByWndSize, &rcMouse)) { bRet = FALSE; break; }
		int		iMouseSizeX = rcMouse.right - rcMouse.left;
		int		iMouseSizeY = rcMouse.bottom - rcMouse.top;
		rcMouse.left += iMouseSizeX / 2;	rcMouse.right += iMouseSizeX;
		rcMouse.top -= iMouseSizeY / 4;		rcMouse.bottom += iMouseSizeY;
		vAdjustFontXRightPosition(dwIMEMode, lpstCursorData->lpstNearDrawMouseByWndCursor[iMouse].szMode, &iMouseSizeX, &rcMouse);
		if (lpstCursorData->dwIMEModeMouseWindow != dwIMEMode || lpstCursorData->bCapsLock != bCapsLock) {
			if (!lpstCursorData->MouseWindow)  { bRet = FALSE;	break; }
			if (!lpstCursorData->MouseWindow->bSetWindowPos(HWND_NOTOPMOST, 0, 0, iMouseSizeX, iMouseSizeY, SWP_DEFERERASE | SWP_HIDEWINDOW | SWP_NOACTIVATE)) { bRet = FALSE; break; }
			SleepEx(50, TRUE);
		}
		SleepEx(0, TRUE);
		if (!lpstCursorData->MouseWindow)  { bRet = FALSE;	break; }
		if (!lpstCursorData->MouseWindow->bSetWindowPos(HWND_TOPMOST, rcMouse.left, rcMouse.top, iMouseSizeX, iMouseSizeY, SWP_SHOWWINDOW | SWP_NOACTIVATE)) { bRet = FALSE; break; }
		lpstCursorData->bCapsLock = bCapsLock;
		lpstCursorData->dwIMEModeMouseWindow = dwIMEMode;
		break;
	} while (TRUE);

	SleepEx(0, TRUE);
	lpstCursorData->bIMEModeByWindowThreadSentinel = FALSE;
	if (!bRet) This->bReloadCursor();
	return bRet;
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
	LPIMECURSORDATA	lpstCursorData = reinterpret_cast<LPIMECURSORDATA>(lpvParam);
	CCursor	*This = reinterpret_cast<CCursor*>(lpvParam);

	CURSORINFO	CursorInfo{ CursorInfo.cbSize = sizeof(CURSORINFO) };
	if (!GetCursorInfo(&CursorInfo))	return FALSE;
	if (((CursorInfo.hCursor == lpstCursorData->hCursorWait) || (CursorInfo.hCursor == lpstCursorData->hCursorAppStarting)
		|| (CursorInfo.hCursor == lpstCursorData->hCursorSizeNWSE) || (CursorInfo.hCursor == lpstCursorData->hCursorSizeNESW)
		|| (CursorInfo.hCursor == lpstCursorData->hCursorSizeWE) || (CursorInfo.hCursor == lpstCursorData->hCursorSizeNS)
		|| (CursorInfo.hCursor == lpstCursorData->hCursorSizeAll)))	return TRUE;

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

	DWORD	dwIMEMode = Cime->dwIMEMode(lpstCursorData->hWndObserved, lpstCursorData->bForceHiragana);
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
	int	iModeSizeX = lpstCursorData->iModeMouseSize;
	int	iModeSizeY = iModeSizeX;
	if (bCalcDisplayModeRect(&iModeSizeX, &iModeSizeY, &rc)) {
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

	LPIMECURSORDATA	lpstCursorData = reinterpret_cast<LPIMECURSORDATA>(lParam);
	CCursor* This = reinterpret_cast<CCursor*>(lParam);

	return This->bDrawIMEModeOnDisplaySub(lpstCursorData, &lpstCursorData->rcCaret);
}

//
// bCalcDisplayModeRect()
//
BOOL		CCursor::bCalcDisplayModeRect(LPINT iModeSizeX, LPINT iModeSizeY, LPRECT lpRect)
{
	POINT	pt{};
	if (GetCursorPos(&pt)) {
		HMONITOR hMonitor = NULL;
		if ((hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST)) != NULL) {
			UINT	dpiX = 0, dpiY = 0;
			if ((GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY)) == S_OK) {
				*iModeSizeX = *iModeSizeX * (dpiX + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI + 1;
				*iModeSizeY = *iModeSizeY * (dpiY + USER_DEFAULT_SCREEN_DPI - 1) / USER_DEFAULT_SCREEN_DPI + 1;
				lpRect->left = pt.x;	lpRect->right = lpRect->left + *iModeSizeX;
				lpRect->top = pt.y;	lpRect->bottom = lpRect->top + *iModeSizeY;
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
HWND		CCursor::hGetCaretPosByAccessibleObjectFromWindow(HWND hForeWnd, LPRECT lprcCaret, BOOL bAttachThreadInput)
{
	HWND	hWnd = NULL;
	POINT	pt{};
	DWORD	dwPID = 0, dwForeThreadID = 0;

	if ((hForeWnd != NULL) && (lprcCaret != NULL)) {
		if ((dwForeThreadID = GetWindowThreadProcessId(hForeWnd, &dwPID)) != 0) {
			LPGUITHREADINFO	lpGuiThreadInfo = new GUITHREADINFO[sizeof(GUITHREADINFO)];
			if (lpGuiThreadInfo != NULL) {
				ZeroMemory(lpGuiThreadInfo, sizeof(GUITHREADINFO));
				lpGuiThreadInfo->cbSize = sizeof(GUITHREADINFO);
				MSG	msg{};
				if (PeekMessage(&msg, NULL, WM_NCLBUTTONDBLCLK, WM_LBUTTONDBLCLK, (PM_NOREMOVE | PM_QS_INPUT))) {
					if ((msg.message == WM_NCLBUTTONDBLCLK) || (msg.message == WM_LBUTTONDBLCLK)) {
						SleepEx(GetDoubleClickTime(), TRUE);
					}
				}
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
							IAccessible* _IAccessible = NULL;
							HRESULT	hResult = NULL;
							if (!bAccessibleObjectFromWindowAsync(lpGuiThreadInfo->hwndFocus, (DWORD)OBJID_CARET, IID_IAccessible, reinterpret_cast<LPVOID*>(&_IAccessible))) {
								if (lpGuiThreadInfo)	delete[]	lpGuiThreadInfo;
								return NULL;
							}
							hResult = AccessibleObjectFromWindow(lpGuiThreadInfo->hwndFocus, (DWORD)OBJID_CARET, IID_IAccessible, reinterpret_cast<LPVOID*>(&_IAccessible));
							if (hResult == S_OK) {
								if (_IAccessible != NULL) {
									LPVARIANT	lpVariant = new VARIANT[sizeof(VARIANT)];
									if (lpVariant != NULL) {
										ZeroMemory(lpVariant, sizeof(VARIANT));
										SIZE	sz{};
										lpVariant->vt = VT_I4;	lpVariant->lVal = CHILDID_SELF;
										if ((hResult = _IAccessible->accLocation(&pt.x, &pt.y, &sz.cx, &sz.cy, *lpVariant)) == S_OK) {
											_IAccessible->Release();
											if ((pt.x == 0) && (pt.y == 0) && (sz.cx == 0) && (sz.cy == 0)) {
												lprcCaret->left = 0;	lprcCaret->right = 0;
												lprcCaret->top = 0;		lprcCaret->bottom = 0;
												hWnd = NULL;
											}
											else {
												hWnd = lpGuiThreadInfo->hwndFocus;
												lprcCaret->left = pt.x;				lprcCaret->top = pt.y;
												lprcCaret->right = pt.x + sz.cx;	lprcCaret->bottom = pt.y + sz.cy;
											}
										}
										if (lpVariant)	delete[]	lpVariant;
									}
								}
							}
						}
					}
					else {
						pt.x = lpGuiThreadInfo->rcCaret.left;	pt.y = lpGuiThreadInfo->rcCaret.top;
						if (ClientToScreen(lpGuiThreadInfo->hwndCaret, &pt)) {
							lprcCaret->left = pt.x;	lprcCaret->top = pt.y;
							pt.x = lpGuiThreadInfo->rcCaret.right;	pt.y = lpGuiThreadInfo->rcCaret.bottom;
							if (ClientToScreen(lpGuiThreadInfo->hwndCaret, &pt)) {
								lprcCaret->right = pt.x;	lprcCaret->bottom = pt.y;
								RECT	rcTop{};
								if (GetWindowRect(lpGuiThreadInfo->hwndCaret, &rcTop)) {
#define	MARGIN_X		2
#define	MARGIN_Y		2
									if (((pt.x == 0) && (pt.y == 0)) || (pt.x <= rcTop.left + MARGIN_X) || (pt.y <= rcTop.top + MARGIN_Y)) {
#undef MARGIN_X
#undef MARGIN_y
										lprcCaret->left = 0;	lprcCaret->right = 0;	
										lprcCaret->top = 0;		lprcCaret->bottom = 0;
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
				if (lpGuiThreadInfo)	delete[]	lpGuiThreadInfo;
			}
		}
	}
	return hWnd;
}

//
// bAccessibleObjectFromWindowAsync()
//
BOOL		CCursor::bAccessibleObjectFromWindowAsync(HWND hWnd, DWORD dwId, REFIID riId, void **ppvObject)
{
	UNREFERENCED_PARAMETER(ppvObject);

#define	TIMEOUT	10
	if (!hWnd)	return FALSE;

	std::future<BOOL> _future;
	std::promise<BOOL> _promise;
	_future = _promise.get_future();
	try {
		std::thread([&](std::promise<BOOL> _promise) {
			(void)_promise;
			IAccessible* _IAccessible = NULL;
			AccessibleObjectFromWindow(hWnd, dwId, riId, reinterpret_cast<LPVOID*>(&_IAccessible));
			}, std::move(_promise)).detach();

		if(std::future_status::ready == _future.wait_until(std::chrono::system_clock::now() + std::chrono::milliseconds(TIMEOUT))) {
			return TRUE;
		}
	}
	catch (...) {
	}
	return FALSE;
#undef TIMEOUT
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
// bAdjustModeSizeByMonitorDPIAsync()
//
BOOL		CCursor::bAdjustModeSizeByMonitorDPIAsync()
{
#define	TIMEOUT	10
	std::future<BOOL> _future;
	std::promise<BOOL> _promise;
	_future = _promise.get_future();
	try {
		std::thread([&](std::promise<BOOL> _promise) {
			(void)_promise;
			RECT	rc{};
			bAdjustModeSizeByMonitorDPI(32, 32, &rc);
		}, std::move(_promise)).detach();

		if(std::future_status::ready == _future.wait_until(std::chrono::system_clock::now() + std::chrono::milliseconds(TIMEOUT))) {
			return TRUE;
		}
	}
	catch (...) {
	}
	return FALSE;
#undef TIMEOUT

}

//
// bDrawIMEModeOnDisplaySub()
//
BOOL		CCursor::bDrawIMEModeOnDisplaySub(LPIMECURSORDATA lpstCursorData, LPRECT lprcCaret)
{
	if (lpstCursorData == NULL)	return FALSE;
	if (!lpstCursorData->CursorWindow || !lpstCursorData->CaretWindow)	return FALSE;

	BOOL	bFoundCaret = FALSE;
	RECT	rcCursor{};
	int		iCursor = 0;
	DWORD	_dwIMEModeCursor = IMEHIDE, _dwIMEModeCaret = IMEHIDE;
	HWND	hCaretWnd = NULL;
	int		iCursorSizeX = 0, iCursorSizeY = 0;
	int		iCaretSizeX = 0, iCaretSizeY = 0;
	BOOL	_bCapsLock = (GetKeyState(VK_CAPITAL) & 0x0001) ? TRUE : FALSE;

	lpstCursorData->bIMEDrawIMEModeThreadSentinel = TRUE;

	if (lpstCursorData->bDrawIMEModeWait) {
#define	COUNT	10
		int	iCount = (lpstCursorData->dwDrawIMEModeWaitTime + (COUNT + 1)) / COUNT;
		for (; iCount >= 0; iCount--) {
			if (!lpstCursorData->bIMEDrawIMEModeThreadSentinel) {
				return TRUE;
			}
			SleepEx(COUNT, TRUE);
		}
	}
#undef COUNT
	if (lpstCursorData->bDrawNearCaret != FALSE) {
		if (lpstCursorData->hWndCaret != NULL) {
			hCaretWnd = lpstCursorData->hWndCaret;
			bFoundCaret = TRUE;
		}
		if (bFoundCaret && (hCaretWnd != NULL)) {
			if (bAdjustModeSizeByMonitorDPI(lpstCursorData->iModeCaretSize, lpstCursorData->iModeCaretSize, lprcCaret) != FALSE) {
				if ((lprcCaret->left == 0) && (lprcCaret->top == 0) && (lprcCaret->right == 0) && (lprcCaret->bottom == 0)) {
					bFoundCaret = FALSE;
				}
				else {
					iCaretSizeX = lprcCaret->right - lprcCaret->left, iCaretSizeY = lprcCaret->bottom - lprcCaret->top;
#define MERGIN_X	8
#define MERGIN_Y	-3
					lprcCaret->left = lprcCaret->left + lpstCursorData->iModeCaretDistanceX + MERGIN_X;
					lprcCaret->right = lprcCaret->left + iCaretSizeX;
					lprcCaret->top = lprcCaret->top + ((lprcCaret->bottom - lprcCaret->top) / 2) - ((iCaretSizeY) / 2) + MERGIN_Y;
					lprcCaret->bottom = lprcCaret->bottom + iCaretSizeY;
#undef MERGIN_X
#undef MERGIN_Y
					_dwIMEModeCaret = Cime->dwIMEMode(hCaretWnd, lpstCursorData->bForceHiragana);
					int	iCaret = iGetCursorID(_dwIMEModeCaret, lpstCursorData->lpstNearDrawCaretCursor, _bCapsLock);
					if (iCaret == 0) _dwIMEModeCaret = IMEOFF;
					vAdjustFontXLeftPosition(_dwIMEModeCaret, lpstCursorData->lpstNearDrawCaretCursor[iCaret].szMode, &iCaretSizeX, lprcCaret);
					lpstCursorData->CaretWindow->vSetModeStringColorFont(lpstCursorData->lpstNearDrawCaretCursor[iCaret].szMode, lpstCursorData->lpstNearDrawCaretCursor[iCaret].dwColor, lpstCursorData->lpstNearDrawCaretCursor[IMEMODE_IMEOFF].szFont);
					SleepEx(0, TRUE);
					if (lpstCursorData->CaretWindow->bSetWindowPos(HWND_TOPMOST, lprcCaret->left, lprcCaret->top, iCaretSizeX, iCaretSizeY, (SWP_SHOWWINDOW | SWP_NOACTIVATE))) {
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
		_dwIMEModeCursor = Cime->dwIMEMode(lpstCursorData->hWndObserved, lpstCursorData->bForceHiragana);
	}
	else {
		_dwIMEModeCursor = Cime->dwIMEMode(GetForegroundWindow(), lpstCursorData->bForceHiragana);
	}
	if (bFoundCaret == FALSE) {
		iCursor = iGetCursorID(_dwIMEModeCursor, lpstCursorData->lpstNearDrawMouseCursor, _bCapsLock);
		lpstCursorData->CursorWindow->vSetModeStringColorFont(lpstCursorData->lpstNearDrawMouseCursor[iCursor].szMode, lpstCursorData->lpstNearDrawMouseCursor[iCursor].dwColor, lpstCursorData->lpstNearDrawMouseCursor[IMEMODE_IMEOFF].szFont);
	}
#define	COUNT	10
	BOOL	bRet = FALSE;
	int		iCount = 0;
	if (lpstCursorData->dwDisplayModeTime != 0)	iCount = (lpstCursorData->dwDisplayModeTime + (COUNT + 1)) / COUNT;
	else iCount = COUNT;
	for (; iCount >= 0; iCount--) {
		bRet = FALSE;
		if (bFoundCaret == FALSE) {
			iCursorSizeX = lpstCursorData->iModeMouseSize, iCursorSizeY = lpstCursorData->iModeMouseSize;
			if (bCalcDisplayModeRect(&iCursorSizeX, &iCursorSizeY, &rcCursor)) {
#define MERGIN_X	2
#define MERGIN_Y	0
				rcCursor.right = rcCursor.right - iCursorSizeX + lpstCursorData->iModeMouseDistanceX - MERGIN_X;
				rcCursor.left = rcCursor.left - iCursorSizeX + lpstCursorData->iModeMouseDistanceX - MERGIN_X;
				rcCursor.bottom = rcCursor.bottom - (iCursorSizeY / 2) + MERGIN_Y;
				rcCursor.top = rcCursor.top - (iCursorSizeY / 2) + MERGIN_Y;
#undef MERGIN_X
#undef MERGIN_Y
				vAdjustFontXLeftPosition(_dwIMEModeCursor, lpstCursorData->lpstNearDrawCaretCursor[iCursor].szMode, &iCursorSizeX, &rcCursor);
				if (_dwIMEModeCursor != IMEHIDE) {
					SleepEx(0, TRUE);
					if (!lpstCursorData->CursorWindow->bSetWindowPos(HWND_TOPMOST, rcCursor.left, rcCursor.top, iCursorSizeX, iCursorSizeY, (SWP_SHOWWINDOW | SWP_NOACTIVATE))) {
						bRet = FALSE;
						break;
					}
				}
				bRet = TRUE;
			}
			else {
				bRet = FALSE;
				break;
			}
		}
		SleepEx(COUNT, TRUE);
		if (!lpstCursorData->bIMEDrawIMEModeThreadSentinel) {
			bRet = TRUE;
			break;
		}
	}
#undef COUNT
	lpstCursorData->CursorWindow->vSetModeStringColorFont(lpstCursorData->lpstNearDrawMouseCursor[IMEMODE_IMEHIDE].szMode, lpstCursorData->lpstNearDrawMouseCursor[IMEMODE_IMEHIDE].dwColor, lpstCursorData->lpstNearDrawMouseCursor[IMEMODE_IMEHIDE].szFont);
	SleepEx(0, TRUE);
	if (!lpstCursorData->CursorWindow->bSetWindowPos(HWND_NOTOPMOST, rcCursor.left, rcCursor.top, iCursorSizeX, iCursorSizeY, (SWP_DEFERERASE | SWP_HIDEWINDOW | SWP_NOACTIVATE))) {
	}
	if (bFoundCaret) {
		lpstCursorData->CaretWindow->vSetModeStringColorFont(lpstCursorData->lpstNearDrawCaretCursor[IMEMODE_IMEHIDE].szMode, lpstCursorData->lpstNearDrawCaretCursor[IMEMODE_IMEHIDE].dwColor, lpstCursorData->lpstNearDrawCaretCursor[IMEMODE_IMEHIDE].szFont);
		SleepEx(0, TRUE);
		if (!lpstCursorData->CaretWindow->bSetWindowPos(HWND_NOTOPMOST, lpstCursorData->rcCaret.left, lpstCursorData->rcCaret.top, iCaretSizeX, iCaretSizeY, (SWP_DEFERERASE | SWP_HIDEWINDOW | SWP_NOACTIVATE))) {
		}	}
	return bRet;
}

//
// iGetCursorID()
//
int			CCursor::iGetCursorID(DWORD dwIMEMode, const LPFLUSHMOUSECURSOR lpstCursorData, BOOL bUnderLine)
{
	if (lpstCursorData == NULL)	return FALSE;
	int		i = bUnderLine ? IMEMODE_IMEOFF_CAPSON : IMEMODE_IMEOFF;
	while (dwIMEMode != lpstCursorData[i].dwIMEMode) {
		i++;
		if (lpstCursorData[i].dwIMEMode == (DWORD)(-1)) {
			return 0;
		}
	}
	return i;
}

//
// bChangeFlushMouseCursor()
//
BOOL		CCursor::bChangeFlushMouseCursor(UINT uCurID, LPIMECURSORDATA lpstCursorData, BOOL bUnderLine)
{
	if (lpstCursorData == NULL)	return FALSE;
	
	int		i = bUnderLine ? IMEMODE_IMEOFF_CAPSON : IMEMODE_IMEOFF;
	while (uCurID != lpstCursorData->lpstNearDrawMouseCursor[i].dwIMEMode) {
		++i;
	}
	if (bSetSystemCursor(reinterpret_cast<LPMOUSECURSOR>(&(lpstCursorData->lpstNearDrawMouseCursor[i]).stArrow), lpstCursorData->iCursorSize, lpstCursorData->iCursorSize)) {
		if (bSetSystemCursor(reinterpret_cast<LPMOUSECURSOR>(&(lpstCursorData->lpstNearDrawMouseCursor[i]).stHand), lpstCursorData->iCursorSize, lpstCursorData->iCursorSize)) {
			if (bSetSystemCursor(reinterpret_cast<LPMOUSECURSOR>(&(lpstCursorData->lpstNearDrawMouseCursor[i]).stIBeam), lpstCursorData->iCursorSize, lpstCursorData->iCursorSize)) {
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
	
	HCURSOR	hCur = NULL;
	static const UINT	fuLoad = (LR_VGACOLOR | LR_DEFAULTSIZE | LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	HMODULE	hMod = NULL;
	if (!CursorSub)	return FALSE;
	if (!(hMod = CursorSub->hLoadCursorData()))	return FALSE;
	if ((hCur = (HCURSOR)LoadImage((HINSTANCE)hMod, MAKEINTRESOURCE(lpstMC->iResourceID),
		IMAGE_CURSOR, iCursorSizeX, iCursorSizeY, fuLoad)) == NULL) {
		_Post_equals_last_error_ DWORD err = GetLastError();
		if (err == ERROR_MOD_NOT_FOUND) {
			if (CursorSub)	CursorSub->bUnLoadCursorData();
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_TOPMOST)
			vMessageBox(m_hMainWnd, IDS_CANTCHANGECURSOR, MessageBoxTYPE, __func__, __LINE__);
			PostMessage(m_hMainWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}
	else {
		CursorSub->bUnLoadCursorData();
		if(!SetSystemCursor(hCur, lpstMC->id))	return FALSE;
	}
	return TRUE;
}

//
// bIsDisplayWindow()
//
BOOL		CCursor::bIsDisplayWindow(HWND hWnd)
{
	if (stIMECursorData.CaretWindow && hWnd == stIMECursorData.CaretWindow->hGetHWND()) return TRUE;
	if (stIMECursorData.MouseWindow && hWnd == stIMECursorData.MouseWindow->hGetHWND()) return TRUE;
	if (stIMECursorData.CursorWindow && hWnd == stIMECursorData.CursorWindow->hGetHWND()) return TRUE;
	return FALSE;
}


/* = EOF = */