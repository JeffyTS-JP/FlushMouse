//
// Profile.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/03/23  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "CommonDef.h"
#include "Cursor.h"
#include "SynTP.h"
#include "TCPIP.h"

//
// Define
// 
#define	aRGB(a, R, G, B)	((COLORREF)(((BYTE)(R) | ((WORD)((BYTE)(G)) << 8)) | (((DWORD)(BYTE)(B)) << 16)) | (((DWORD)(BYTE)(a)) << 24))

//
// Struct Define
//
typedef struct tagAppRegData
{
	// for Cursor
	int			iCursorSize = 32;
	int			iIMEModeDistance = 32;
	int			iModeMouseSize = 24;
	int			iModeCaretSize = 24;
	int			iModeByWndSize = 24;
	int			iModeMouseDistanceX = -24;
	int			iModeCaretDistanceX = -12;
	DWORD		dwInThreadSleepTime = 0;
	DWORD		dwWaitWaveTime = 400;
	DWORD		dwAdditionalWaitTime = 300;
	DWORD		dwDisplayModeTime = 400;
	BOOL		bDisplayIMEModeOnCursor = FALSE;
	DWORD		dwDisplayIMEModeMethod = 0;
	BOOL		bDisplayIMEModeIMEOFF = FALSE;
	BOOL		bForceHiragana = FALSE;
	BOOL		bDenyChangedByApp = FALSE;
	BOOL		bUseBigArrow = FALSE;

	TCHAR		szNearDrawMouseIMEOFFChar[MAX_IMEMODECHAR] = L"A";
	TCHAR		szNearDrawMouseHANEISU_IMEONChar[MAX_IMEMODECHAR] = L"_A";
	TCHAR		szNearDrawMouseHANKANA_IMEONChar[MAX_IMEMODECHAR] = L"_ｱ";
	TCHAR		szNearDrawMouseZENEISU_IMEONChar[MAX_IMEMODECHAR] = L"Ａ";
	TCHAR		szNearDrawMouseZENHIRA_IMEONChar[MAX_IMEMODECHAR] = L"あ";
	TCHAR		szNearDrawMouseZENKANA_IMEONChar[MAX_IMEMODECHAR] = L"ア";
	TCHAR		szNearDrawCaretIMEOFFChar[MAX_IMEMODECHAR] = L"A";
	TCHAR		szNearDrawCaretHANEISU_IMEONChar[MAX_IMEMODECHAR] = L"_A";
	TCHAR		szNearDrawCaretHANKANA_IMEONChar[MAX_IMEMODECHAR] = L"_ｱ";
	TCHAR		szNearDrawCaretZENEISU_IMEONChar[MAX_IMEMODECHAR] = L"Ａ";
	TCHAR		szNearDrawCaretZENHIRA_IMEONChar[MAX_IMEMODECHAR] = L"あ";
	TCHAR		szNearDrawCaretZENKANA_IMEONChar[MAX_IMEMODECHAR] = L"ア";
	TCHAR		szNearDrawMouseByWndIMEOFFChar[MAX_IMEMODECHAR] = L"A";
	TCHAR		szNearDrawMouseByWndHANEISU_IMEONChar[MAX_IMEMODECHAR] = L"_A";
	TCHAR		szNearDrawMouseByWndHANKANA_IMEONChar[MAX_IMEMODECHAR] = L"_ｱ";
	TCHAR		szNearDrawMouseByWndZENEISU_IMEONChar[MAX_IMEMODECHAR] = L"Ａ";
	TCHAR		szNearDrawMouseByWndZENHIRA_IMEONChar[MAX_IMEMODECHAR] = L"あ";
	TCHAR		szNearDrawMouseByWndZENKANA_IMEONChar[MAX_IMEMODECHAR] = L"ア";
	COLORREF	dwNearDrawMouseIMEOFFColor = aRGB(48, 255, 0, 0);
	COLORREF	dwNearDrawMouseHANEISU_IMEONColor = aRGB(48, 255, 0, 0);
	COLORREF	dwNearDrawMouseHANKANA_IMEONColor = aRGB(48, 255, 0, 0);
	COLORREF	dwNearDrawMouseZENEISU_IMEONColor = aRGB(48, 255, 0, 0);
	COLORREF	dwNearDrawMouseZENHIRA_IMEONColor = aRGB(48, 255, 0, 0);
	COLORREF	dwNearDrawMouseZENKANA_IMEONColor = aRGB(48, 255, 0, 0);
	COLORREF	dwNearDrawCaretIMEOFFColor = aRGB(48, 0, 0, 255);
	COLORREF	dwNearDrawCaretHANEISU_IMEONColor = aRGB(48, 0, 0, 255);
	COLORREF	dwNearDrawCaretHANKANA_IMEONColor = aRGB(48, 0, 0, 255);
	COLORREF	dwNearDrawCaretZENEISU_IMEONColor = aRGB(48, 0, 0, 255);
	COLORREF	dwNearDrawCaretZENHIRA_IMEONColor = aRGB(48, 0, 0, 255);
	COLORREF	dwNearDrawCaretZENKANA_IMEONColor = aRGB(48, 0, 0, 255);
	COLORREF	dwNearDrawMouseByWndIMEOFFColor = aRGB(48, 255, 0, 0);
	COLORREF	dwNearDrawMouseByWndHANEISU_IMEONColor = aRGB(48, 255, 0, 0);
	COLORREF	dwNearDrawMouseByWndHANKANA_IMEONColor = aRGB(48, 255, 0, 0);
	COLORREF	dwNearDrawMouseByWndZENEISU_IMEONColor = aRGB(48, 255, 0, 0);
	COLORREF	dwNearDrawMouseByWndZENHIRA_IMEONColor = aRGB(48, 255, 0, 0);
	COLORREF	dwNearDrawMouseByWndZENKANA_IMEONColor = aRGB(48, 255, 0, 0);
	TCHAR		szNearDrawMouseFont[LF_FACESIZE] = L"Yu Gothic UI";
	TCHAR		szNearDrawCaretFont[LF_FACESIZE] = L"Yu Gothic UI";
	TCHAR		szNearDrawMouseByWndFont[LF_FACESIZE] = L"Yu Gothic UI";

	// for FlushMouse & Cursor
	BOOL		bDisplayFocusWindowIME = FALSE;

	// for FlushMouse
	BOOL		bDoModeDispByIMEKeyDown = TRUE;
	BOOL		bDoModeDispByMouseBttnUp = TRUE;
	BOOL		bDoModeDispByCtrlUp = TRUE;
	BOOL		bIMEModeForced = FALSE;
	BOOL		bMoveIMEToolbar = FALSE;

	// for FlushMouse & FlushMouseSub
	BOOL		bOffChangedFocus = FALSE;
	BOOL		bDrawNearCaret = FALSE;
	BOOL		bEnableEPHelper = FALSE;

	// for SynTP Helper
	BOOL		bSynTPStarted1 = FALSE;
	DWORD		dwSynTPPadX = 528;
	DWORD		dwSynTPPadY = 528;
	DWORD		dwSynTPEdgeX = 88;
	DWORD		dwSynTPEdgeY = 88;
	DWORD		dwSynTPHelper1 = SYNTPH_DISABLE;
	TCHAR		szSynTPSendIPAddr1[MAX_IPV4_ADDRESS]{};
	TCHAR		szSynTPSendHostname1[MAX_FQDN]{};
	DWORD		dwSynTPPortNo1 = 50008;
	DWORD		dwSynTPTimeOut = 100;

	// for Settings
	BOOL		bIsPaneOpen = TRUE;
	DWORD		dwSettingsX = 0;
	DWORD		dwSettingsY = 0;
	DWORD		dwSettingsWidth = 0;
	DWORD		dwSettingsHeight = 0;

} APPREGDATA, * PAPPREGDATA, * LPAPPREGDATA;

//
// Global Data
//
//extern APPREGDATA	stAppRegData;

//
// Class Define
//
//
// Class CProfile
//
class CProfile
{
	public:
					CProfile();
					~CProfile();
		BOOL		bGetProfileData() const;
		BOOL		bSetProfileData() const;

		BOOL		bFixChangedProfileData() const;
		BOOL		bGetProfileData4Mouse() const;
		BOOL		bSetProfileData4Mouse() const;
		BOOL		bGetProfileData4IMEMode() const;
		BOOL		bSetProfileData4IMEMode() const;
		BOOL		bGetProfileData4SynTPHelper() const;
		BOOL		bSetProfileData4SynTPHelper() const;
		BOOL		bGetProfileData4Settings() const;
		BOOL		bSetProfileData4Settings() const;

	public:
		LPAPPREGDATA	lpstAppRegData;
};

/* = EOF = */