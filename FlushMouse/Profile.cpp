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
// calss CProfile
//
CProfile::CProfile()
{
	// 初期値の設定
	// for Cursor
	stAppRegData.iCursorSize = 32;					// 表示するマウスカーソルのサイズ
	stAppRegData.iModeSize = 48;					// 表示するIMEモードのサイズ
	stAppRegData.dwInThreadSleepTime = 0;			// Thread内の待ち時間(IMECursorChangeThreadのみ)
	stAppRegData.dwWaitWaveTime = 400;				// IME mode displayのwaveの待ち時間
	stAppRegData.dwDisplayModeTime = 380;			// IME mode displayの表示時間
	stAppRegData.bForceHiragana = FALSE;			// 「全角ひらがな」へ強制的に変更する
	stAppRegData.dwNearDrawMouseColor = aRGB(15, 254, 192, 0);	// マウスカーソルへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
	stAppRegData.dwNearDrawCaretColor = aRGB(15, 254, 192, 0);	// キャレットへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0) / RGB(96, 124, 255)
	// @@@未作成
	stAppRegData.bDenyChangedByApp = FALSE;			// アプリケーションで変更されるのを防ぐか <--今のところ作っていない(難しいので後回し)
	stAppRegData.bUseBigArrow = FALSE;				// 大きいサイズのアイコンを使うか？ <--今のところ作っていない

	// for FlushMouse & Cursor
	stAppRegData.bDisplayIMEModeOnCursor = TRUE;	// マウスカーソルへのIMEモード表示
	stAppRegData.bDisplayFocusWindowIME = FALSE;	// フォーカスウィンドウのIMEモードを表示する(TRUE)/マウスカーソル下のウィンドウのIMEモードを表示する(FALSE)

	// for FlushMouse
	stAppRegData.bDoModeDispByIMEKeyDown = FALSE;	// IMEのモード変更のキーが押されたときにIMEモードを表示する
	stAppRegData.bDoModeDispByMouseBttnUp = FALSE;	// Mouse L/Rボタンが離されたときにIMEモードを表示する
	stAppRegData.bDoModeDispByCtrlUp = FALSE;		// Ctrlが離されたときにIMEモードを表示する

	// for FlushMouse & FlushMouseSub
	stAppRegData.bDrawNearCaret = FALSE;			// Caretの横にIMEモードを表示
	stAppRegData.bEnableEPHelper = FALSE;			// for Explorer Patcher Simple Window Switcher Helper

	// for FlushMouseSub
	stAppRegData.bOffChangedFocus = FALSE;			// アプリケーションが切り替わったときIMEをOFFにする(bDisplayFocusWindowIMEとは排他的動作になる)
}

CProfile::~CProfile()
{
	// NOP
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
	// DWORD registry for Cursor
	if (CReg->bGetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CusrorSize"), (LPDWORD)&stAppRegData.iCursorSize, 32)) {					// 表示するマウスカーソルのサイズ
		if (CReg->bGetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("ModeSize"), (LPDWORD)&stAppRegData.iModeSize, 48)) {						// 表示するIMEモードのサイズ
			if (CReg->bGetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("InThreadSleepTime"), (LPDWORD)&stAppRegData.dwInThreadSleepTime, 0)) {	// Thread内の待ち時間(IMECursorChangeThreadのみ)
				if (CReg->bGetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("WaitWaveTime"), (LPDWORD)&stAppRegData.dwWaitWaveTime, 400)) {			// IME mode displayのwaveの待ち時間
					if (CReg->bGetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayModeTime"), (LPDWORD)&stAppRegData.dwDisplayModeTime, 380)) {		// IME mode displayの表示時間
						if (CReg->bGetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseColor"), (LPDWORD)&stAppRegData.dwNearDrawMouseColor, aRGB(15, 254, 192, 0))) {	// マウスカーソルへのIMEモード表示色
							if (CReg->bGetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretColor"), (LPDWORD)&stAppRegData.dwNearDrawCaretColor, aRGB(15, 254, 192, 0))) {	// キャレットへのIMEモード表示色
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
		if (CReg->bGetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeOnCursor"), (LPBOOL)&stAppRegData.bDisplayIMEModeOnCursor, TRUE)) {	// マウスカーソルへのIMEモード表示ON/OFF
			if (CReg->bGetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("ForceHiragana"), (LPBOOL) & (stAppRegData.bForceHiragana), FALSE)) {					//「全角ひらがな」へ強制的に変更する
				if (CReg->bGetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DenyChangedByApp"), (LPBOOL) & (stAppRegData.bDenyChangedByApp), FALSE)) {			// アプリケーションで変更されるのを防ぐか <--今のところ作っていない(難しいので後回し)
					if (CReg->bGetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("UseBigArrow"), (LPBOOL) & (stAppRegData.bUseBigArrow), FALSE)) {						// 大きいサイズのアイコンを使うか？ <--今のところ作っていない
						bRet = TRUE;
					}
				}
			}
		}
	}
	// BOOL registry for FlushMouse & Cursor
	if (bRet) {
		bRet = FALSE;
		if (CReg->bGetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayFocusWindowIME"), (LPBOOL) & (stAppRegData.bDisplayFocusWindowIME), FALSE)) {	// フォーカスウィンドウのIMEモードを表示する(TRUE)/マウスカーソル下のウィンドウのIMEモードを表示する(FALSE)
			bRet = TRUE;
		}
	}
	// BOOL registry for FlushMouse
	if (bRet) {
		bRet = FALSE;
		if (CReg->bGetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByMouseBttnUp"), (LPBOOL) & (stAppRegData.bDoModeDispByMouseBttnUp), FALSE)) {	// Mouse L/Rボタンが離されたときにIMEモードを表示する
			if (CReg->bGetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByIMEKeyDown"), (LPBOOL) & (stAppRegData.bDoModeDispByIMEKeyDown), FALSE)) {	// IMEのモード変更のキーが押されたときにIMEモードを表示する
				if (CReg->bGetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByCtrlUp"), (LPBOOL) & (stAppRegData.bDoModeDispByCtrlUp), FALSE)) {		// Ctrlが離されたときにIMEモードを表示する
					bRet = TRUE;
				}
			}
		}
	}
	// BOOL registry for FlushMouse & FlushMouseSub
	if (bRet) {
		bRet = FALSE;
		if (CReg->bGetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DrawNearCaret"), (LPBOOL) & (stAppRegData.bDrawNearCaret), FALSE)) {			// Ctrlが離されたときにIMEモードを表示する
			if (CReg->bGetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("EnableEPHelper"), (LPBOOL) & (stAppRegData.bEnableEPHelper), FALSE)) {		// Ctrlが離されたときにIMEモードを表示する
				bRet = TRUE;
			}
		}
	}
	// BOOL registry for FlushMouseSub
	if (bRet) {
		bRet = FALSE;
		if (CReg->bGetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("OffChangedFocus"), (LPBOOL)&(stAppRegData.bOffChangedFocus), FALSE)) {	// アプリケーションが切り替わったときIMEをOFFにする
			bRet = TRUE;
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
	if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CusrorSize"), stAppRegData.iCursorSize)) {
		if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("ModeSize"), stAppRegData.iModeSize)) {
			if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("InThreadSleepTime"), stAppRegData.dwInThreadSleepTime)) {
				if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("WaitWaveTime"), stAppRegData.dwWaitWaveTime)) {
					if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayModeTime"), stAppRegData.dwDisplayModeTime)) {
						if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseColor"), stAppRegData.dwNearDrawMouseColor)) {
							if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretColor"), stAppRegData.dwNearDrawCaretColor)) {
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
		if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeOnCursor"), stAppRegData.bDisplayIMEModeOnCursor)) {
			if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("ForceHiragana"), stAppRegData.bForceHiragana)) {
				if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DenyChangedByApp"), stAppRegData.bDenyChangedByApp)) {
					if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("UseBigArrow"), stAppRegData.bUseBigArrow)) {
						bRet = TRUE;
					}
				}
			}
		}
	}
	// BOOL registry for FlushMouse & FlushMouseSub
	if (bRet) {
		bRet = FALSE;
		if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayFocusWindowIME"), stAppRegData.bDisplayFocusWindowIME)) {
			bRet = TRUE;
		}
	}
	// BOOL registry in Use FlushMouse
	if (bRet) {
		bRet = FALSE;
		if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByIMEKeyDown"), stAppRegData.bDoModeDispByIMEKeyDown)) {
			if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByCtrlUp"), stAppRegData.bDoModeDispByCtrlUp)) {
				if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByMouseBttnUp"), stAppRegData.bDoModeDispByMouseBttnUp)) {
					bRet = TRUE;
				}
			}
		}
	}
	// BOOL registry in Use FlushMouse & FlushMouseSub
	if (bRet) {
		bRet = FALSE;
		if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DrawNearCaret"), stAppRegData.bDrawNearCaret)) {
			if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("EnableEPHelper"), stAppRegData.bEnableEPHelper)) {
				bRet = TRUE;
			}
		}
	}
	// BOOL registry in Use FlushMouseSub
	if (bRet) {
		bRet = FALSE;
		if (CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("OffChangedFocus"), stAppRegData.bOffChangedFocus)) {
			bRet = TRUE;
		}
	}
	delete	CReg;
	return bRet;
#undef	PROFILE_HKEY
#undef	PROFILE_SUBKEY
}

/* EOF */