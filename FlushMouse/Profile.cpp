// Profile.cpp
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
#include "pch.h"
#include "Profile.h"
#include "..\MiscLIB\CRegistry.h"

// Global Data
APPREGDATA	stAppRegData{};							// Registry Data

//
// Local Prototype Define
//

//
// Class CProfile
//
CProfile::CProfile()
{
	lpstAppRegData = new APPREGDATA[sizeof(APPREGDATA)];
	ZeroMemory(lpstAppRegData, sizeof(APPREGDATA));

	// 初期値の設定
	// for Cursor
	lpstAppRegData->iCursorSize = 32;					// 表示するマウスカーソルのサイズ
	lpstAppRegData->iModeSize = 48;						// 表示するIMEモードのサイズ
	lpstAppRegData->dwInThreadSleepTime = 0;			// Thread内の待ち時間(IMECursorChangeThreadのみ)
	lpstAppRegData->dwWaitWaveTime = 400;				// IME mode displayの waveの待ち時間
	lpstAppRegData->dwDisplayModeTime = 380;			// IME mode displayの表示時間
	lpstAppRegData->bForceHiragana = FALSE;				// 「全角ひらがな」へ強制的に変更する
	lpstAppRegData->dwNearDrawMouseColor = aRGB(15, 254, 192, 0);	// マウスカーソルへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
	lpstAppRegData->dwNearDrawCaretColor = aRGB(15, 254, 192, 0);	// キャレットへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0) / RGB(96, 124, 255)

	lpstAppRegData->bDenyChangedByApp = FALSE;
	lpstAppRegData->bUseBigArrow = FALSE;

	// for FlushMouse & Cursor
	lpstAppRegData->bDisplayIMEModeOnCursor = TRUE;		// マウスカーソルへのIMEモード表示
	lpstAppRegData->bDisplayFocusWindowIME = FALSE;		// フォーカスウィンドウのIMEモードを表示する(TRUE)/マウスカーソル下のウィンドウのIMEモードを表示する(FALSE)

	// for FlushMouse
	lpstAppRegData->bDoModeDispByIMEKeyDown = FALSE;	// IMEのモード変更のキーが押されたときにIMEモードを表示する
	lpstAppRegData->bDoModeDispByMouseBttnUp = FALSE;	// Mouse L/Rボタンが離されたときにIMEモードを表示する
	lpstAppRegData->bDoModeDispByCtrlUp = FALSE;		// Ctrlが離されたときにIMEモードを表示する
	lpstAppRegData->bIMEModeForced = FALSE;				// Change IME Mode Forced

	// for FlushMouse & FlushMouseSub
	lpstAppRegData->bOffChangedFocus = FALSE;			// アプリケーションが切り替わったときIMEをOFFにする(bDisplayFocusWindowIMEとは排他的動作になる)
	lpstAppRegData->bDrawNearCaret = FALSE;				// Caretの横にIMEモードを表示
	lpstAppRegData->bMoveIMEToolbar = FALSE;			// New IMEのToolbarを移動する 
	lpstAppRegData->bEnableEPHelper = FALSE;			// for Explorer Patcher Simple Window Switcher Helper
}

CProfile::~CProfile()
{
	if (lpstAppRegData != NULL) {
		delete[]	lpstAppRegData;
		lpstAppRegData = NULL;
	}
}

//
//　bGetProfileData()
//
BOOL		CProfile::bGetProfileData()
{
// \HKEY_CURRENT_USER\SOFTWARE\JeffyTS\FlushMouse
#define PROFILE_HKEY		HKEY_CURRENT_USER
#define PROFILE_SUBKEY		_T("SOFTWARE\\JeffyTS\\FlushMouse")
	BOOL		bRet = FALSE;
	CRegistry* CReg = new CRegistry;
	
	DWORD	dw = 0;
	if (CReg->bReadRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CusrorSize"), (LPDWORD)&dw)) {
		if (dw != 0) {
			if (CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CursorSize"), (LPDWORD)&lpstAppRegData->iCursorSize, dw)) {
				if (CReg->bDeleteRegValue(PROFILE_HKEY, PROFILE_SUBKEY, _T("CusrorSize"))) {
					// success
				}
			}
		}
	}


	// DWORD registry for Cursor
	if (CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CursorSize"), (LPDWORD)&lpstAppRegData->iCursorSize, 32)) {					// 表示するマウスカーソルのサイズ
		if (CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("ModeSize"), (LPDWORD)&lpstAppRegData->iModeSize, 48)) {						// 表示するIMEモードのサイズ
			if (CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("InThreadSleepTime"), (LPDWORD)&lpstAppRegData->dwInThreadSleepTime, 0)) {	// Thread内の待ち時間(IMECursorChangeThreadのみ)
				if (CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("WaitWaveTime"), (LPDWORD)&lpstAppRegData->dwWaitWaveTime, 400)) {			// IME mode displayのwaveの待ち時間
					if (CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayModeTime"), (LPDWORD)&lpstAppRegData->dwDisplayModeTime, 380)) {		// IME mode displayの表示時間
						if (CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseColor, aRGB(15, 254, 192, 0))) {	// マウスカーソルへのIMEモード表示色
							if (CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretColor, aRGB(15, 254, 192, 0))) {	// キャレットへのIMEモード表示色
								bRet = TRUE;
							}
						}
					}
				}
			}
		}
	}
	// BOOL registry for Cursor
	if (bRet) {
		bRet = FALSE;
		if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeOnCursor"), (LPBOOL)&lpstAppRegData->bDisplayIMEModeOnCursor, TRUE)) {	// マウスカーソルへのIMEモード表示ON/OFF
			if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("ForceHiragana"), (LPBOOL) & (lpstAppRegData->bForceHiragana), FALSE)) {					//「全角ひらがな」へ強制的に変更する
				if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DenyChangedByApp"), (LPBOOL) & (lpstAppRegData->bDenyChangedByApp), FALSE)) {			// アプリケーションで変更されるのを防ぐか <--今のところ作っていない(難しいので後回し)
					if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("UseBigArrow"), (LPBOOL) & (lpstAppRegData->bUseBigArrow), FALSE)) {						// 大きいサイズのアイコンを使うか？ <--今のところ作っていない
						bRet = TRUE;
					}
				}
			}
		}
	}
	// BOOL registry for FlushMouse & Cursor
	if (bRet) {
		bRet = FALSE;
		if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayFocusWindowIME"), (LPBOOL) & (lpstAppRegData->bDisplayFocusWindowIME), FALSE)) {		// フォーカスウィンドウのIMEモードを表示する(TRUE)/マウスカーソル下のウィンドウのIMEモードを表示する(FALSE)
			bRet = TRUE;
		}
	}
	// BOOL registry for FlushMouse
	if (bRet) {
		bRet = FALSE;
		if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByMouseBttnUp"), (LPBOOL) & (lpstAppRegData->bDoModeDispByMouseBttnUp), FALSE)) {	// Mouse L/Rボタンが離されたときにIMEモードを表示する
			if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByIMEKeyDown"), (LPBOOL) & (lpstAppRegData->bDoModeDispByIMEKeyDown), FALSE)) {	// IMEのモード変更のキーが押されたときにIMEモードを表示する
				if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByCtrlUp"), (LPBOOL) & (lpstAppRegData->bDoModeDispByCtrlUp), FALSE)) {		// Ctrlが離されたときにIMEモードを表示する
					if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("IMEModeForced"), (LPBOOL) & (lpstAppRegData->bIMEModeForced), FALSE)) {			// Change IME Mode Forced
						if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("MoveIMEToolbar"), (LPBOOL) & (lpstAppRegData->bMoveIMEToolbar), FALSE)) {			// Change IME Mode Forced
							bRet = TRUE;
						}
					}
				}
			}
		}
	}
	// BOOL registry for FlushMouse & FlushMouseSub
	if (bRet) {
		bRet = FALSE;
		if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("OffChangedFocus"), (LPBOOL) & (lpstAppRegData->bOffChangedFocus), FALSE)) {						// アプリケーションが切り替わったときIMEをOFFにする
			if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DrawNearCaret"), (LPBOOL) & (lpstAppRegData->bDrawNearCaret), FALSE)) {						// Ctrlが離されたときにIMEモードを表示する
				if (CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("EnableEPHelper"), (LPBOOL) & (lpstAppRegData->bEnableEPHelper), FALSE)) {					// Ctrlが離されたときにIMEモードを表示する
					bRet = TRUE;
				}
			}
		}
	}
	delete	CReg;
	return bRet;
#undef	PROFILE_HKEY
#undef	PROFILE_SUBKEY
}

//
// bSetProfileData()
//
BOOL		CProfile::bSetProfileData()
{
// \HKEY_CURRENT_USER\SOFTWARE\JeffyTS\FlushMouse
#define PROFILE_HKEY		HKEY_CURRENT_USER
#define PROFILE_SUBKEY		_T("SOFTWARE\\JeffyTS\\FlushMouse")
	BOOL		bRet = FALSE;
	CRegistry* CReg = new CRegistry;
	// DWORD registry in Use Cursor
	if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CursorSize"), lpstAppRegData->iCursorSize)) {
		if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("ModeSize"), lpstAppRegData->iModeSize)) {
			if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("InThreadSleepTime"), lpstAppRegData->dwInThreadSleepTime)) {
				if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("WaitWaveTime"), lpstAppRegData->dwWaitWaveTime)) {
					if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayModeTime"), lpstAppRegData->dwDisplayModeTime)) {
						if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseColor"), lpstAppRegData->dwNearDrawMouseColor)) {
							if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretColor"), lpstAppRegData->dwNearDrawCaretColor)) {
								bRet = TRUE;
							}
						}
					}
				}
			}
		}
	}
	// BOOL registry in Use Cursor
	if (bRet) {
		bRet = FALSE;
		if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeOnCursor"), lpstAppRegData->bDisplayIMEModeOnCursor)) {
			if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("ForceHiragana"), lpstAppRegData->bForceHiragana)) {
				if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DenyChangedByApp"), lpstAppRegData->bDenyChangedByApp)) {
					if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("UseBigArrow"), lpstAppRegData->bUseBigArrow)) {
						bRet = TRUE;
					}
				}
			}
		}
	}
	// BOOL registry for FlushMouse & FlushMouseSub
	if (bRet) {
		bRet = FALSE;
		if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayFocusWindowIME"), lpstAppRegData->bDisplayFocusWindowIME)) {
			bRet = TRUE;
		}
	}
	// BOOL registry in Use FlushMouse
	if (bRet) {
		bRet = FALSE;
		if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByIMEKeyDown"), lpstAppRegData->bDoModeDispByIMEKeyDown)) {
			if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByCtrlUp"), lpstAppRegData->bDoModeDispByCtrlUp)) {
				if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByMouseBttnUp"), lpstAppRegData->bDoModeDispByMouseBttnUp)) {
					if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("IMEModeForced"), lpstAppRegData->bIMEModeForced)) {
						if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("MoveIMEToolbar"), lpstAppRegData->bMoveIMEToolbar)) {
							bRet = TRUE;
						}
					}
				}
			}
		}
	}
	// BOOL registry in Use FlushMouse & FlushMouseSub
	if (bRet) {
		bRet = FALSE;
		if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("OffChangedFocus"), lpstAppRegData->bOffChangedFocus)) {
			if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DrawNearCaret"), lpstAppRegData->bDrawNearCaret)) {
				if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("EnableEPHelper"), lpstAppRegData->bEnableEPHelper)) {
					bRet = TRUE;
				}
			}
		}
	}
	delete	CReg;
	return bRet;
}

/* = EOF = */