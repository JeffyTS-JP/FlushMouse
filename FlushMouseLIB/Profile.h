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
	int			iModeSize = 24;
	DWORD		dwInThreadSleepTime = 0;
	DWORD		dwWaitWaveTime = 400;
	DWORD		dwAdditionalWaitTime = 300;
	DWORD		dwDisplayModeTime = 400;
	BOOL		bDisplayIMEModeOnCursor = FALSE;
	BOOL		bDisplayIMEModeByWindow = FALSE;
	BOOL		bForceHiragana = FALSE;
	BOOL		bDenyChangedByApp = FALSE;
	BOOL		bUseBigArrow = FALSE;
	COLORREF	dwNearDrawMouseColor = aRGB(48, 255, 0, 0);;
	COLORREF	dwNearDrawCaretColor= aRGB(48, 0, 0, 255);
	COLORREF	dwNearMouseColor = aRGB(48, 0, 255, 0);

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

	DWORD		dwSynTPHelper1 = 0;
	TCHAR		szSynTPSendIPAddr1[MAX_IPV4_ADDRESS]{};
	TCHAR		szSynTPSendHostname1[MAX_FQDN]{};
	
	DWORD		dwSynTPPortNo1 = 50008;

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

public:
	LPAPPREGDATA	lpstAppRegData;
};

/* = EOF = */