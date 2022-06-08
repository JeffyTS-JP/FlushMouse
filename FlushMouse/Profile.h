// Profile.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/23  JeffyTS  	New edit.
//

//
// Include
//
#pragma once

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
	int			iCursorSize;
	int			iModeSize;
	DWORD		dwInThreadSleepTime;
	DWORD		dwWaitWaveTime;
	DWORD		dwDisplayModeTime;
	BOOL		bDisplayIMEModeOnCursor;
	BOOL		bForceHiragana;
	BOOL		bDenyChangedByApp;
	BOOL		bUseBigArrow;
	COLORREF	dwNearDrawMouseColor;
	COLORREF	dwNearDrawCaretColor;

	// for FlushMouse & Cursor
	BOOL		bDisplayFocusWindowIME;

	// for FlushMouse
	BOOL		bDoModeDispByIMEKeyDown;
	BOOL		bDoModeDispByMouseBttnUp;
	BOOL		bDoModeDispByCtrlUp;

	// for FlushMouse & FlushMouseSub
	BOOL		bDrawNearCaret;
	BOOL		bEnableEPHelper;

	// for FlushMouseSub
	BOOL		bOffChangedFocus;
} APPREGDATA, *PAPPREGDATA, *LPAPPREGDATA;

//
// Global Data
//
//extern APPREGDATA	stAppRegData;

//
// Class Define
//
//
// calss CProfile
//
class CProfile
{
public:
				CProfile();
				~CProfile();
	BOOL		bGetProfileData();
	BOOL		bSetProfileData();

public:
	APPREGDATA	stAppRegData;
};



