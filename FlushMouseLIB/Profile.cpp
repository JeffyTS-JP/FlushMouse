//
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
	if (lpstAppRegData != NULL) {
		ZeroMemory(lpstAppRegData, sizeof(APPREGDATA));

		// 初期値の設定
		// for Cursor
		lpstAppRegData->iCursorSize = 32;							// 表示するマウスカーソルのサイズ
		lpstAppRegData->iModeSize = 24;								// 表示するIMEモードのサイズ
		lpstAppRegData->dwInThreadSleepTime = 0;					// Thread内の待ち時間(IMECursorChangeThreadのみ)
		lpstAppRegData->dwWaitWaveTime = 400;						// IME mode displayの waveの待ち時間
		lpstAppRegData->dwAdditionalWaitTime = 300;					// IME mode displayの 追加待ち時間
		lpstAppRegData->dwDisplayModeTime = 400;					// IME mode displayの表示時間
		lpstAppRegData->bForceHiragana = FALSE;						// 「全角ひらがな」へ強制的に変更する
		lpstAppRegData->dwNearDrawMouseColor = aRGB(48, 255, 0, 0);	// マウスカーソルへのIMEモード表示色
		lpstAppRegData->dwNearDrawCaretColor = aRGB(48, 0, 0, 255);	// キャレットへのIMEモード表示色
		lpstAppRegData->dwNearMouseColor = aRGB(48, 0, 255, 0);		// マウスカーソルへのIMEモード表示色

		lpstAppRegData->bDenyChangedByApp = FALSE;					// アプリケーションで変更されるのを防ぐか
		lpstAppRegData->bUseBigArrow = FALSE;						// 大きいサイズのアイコンを使うか

		// for FlushMouse & Cursor
		lpstAppRegData->bDisplayIMEModeOnCursor = TRUE;				// マウスカーソルへのIMEモード表示
		lpstAppRegData->bDisplayIMEModeByWindow = FALSE;			// マウスカーソルへのIMEモード表示
		lpstAppRegData->bDisplayFocusWindowIME = FALSE;				// フォーカスウィンドウのIMEモードを表示する(TRUE)/マウスカーソル下のウィンドウのIMEモードを表示する(FALSE)

		// for FlushMouse
		lpstAppRegData->bDoModeDispByIMEKeyDown = TRUE;				// IMEのモード変更のキーが押されたときにIMEモードを表示する
		lpstAppRegData->bDoModeDispByMouseBttnUp = TRUE;			// Mouse L/Rボタンが離されたときにIMEモードを表示する
		lpstAppRegData->bDoModeDispByCtrlUp = TRUE;					// Ctrlが離されたときにIMEモードを表示する
		lpstAppRegData->bIMEModeForced = FALSE;						// Change IME Mode Forced
		lpstAppRegData->bEnableEPHelper = FALSE;					// for Explorer Patcher Simple Window Switcher Helper

		// for FlushMouse & FlushMouseSub
		lpstAppRegData->bOffChangedFocus = FALSE;					// アプリケーションが切り替わったときIMEをOFFにする(bDisplayFocusWindowIMEとは排他的動作になる)
		lpstAppRegData->bDrawNearCaret = FALSE;						// Caretの横にIMEモードを表示
		lpstAppRegData->bMoveIMEToolbar = FALSE;					// New IMEのToolbarを移動する 
		
		// for SynTP Helper
		lpstAppRegData->bSynTPStarted1 = FALSE;						// SynTP_Helperが起動状態か
		lpstAppRegData->dwSynTPPadX = 528;							// TouchPad X
		lpstAppRegData->dwSynTPPadY = 528;							// TouchPad X
		lpstAppRegData->dwSynTPEdgeX = 88;							// Edge X
		lpstAppRegData->dwSynTPEdgeY = 88;							// Edge Y
		lpstAppRegData->dwSynTPHelper1 = SYNTPH_DISABLE;			// SynTP Helper #1 (0 = disable  0x01 = sender (IPv4)           / 0x02 = sender (always start IPV4)           / 0x03 = receiver (IPv4) / 0x04 =receiver (always start IPv4)
																	//                               0x11 = sender (Hosstname IPV4) / 0x12 = sender (always start Hosstname IPV4)
																	//                               0x31 = sender (Hosstname IPV6) / 0x32 = sender (always start Hosstname IPV6) / 0x33 = receiver (IPv6) / 0x34 =receiver (always start IPv6)
		_tcsncpy_s(lpstAppRegData->szSynTPSendIPAddr1, MAX_IPV4_ADDRESS, L"", _TRUNCATE);	// SynTP Helper Send IP Addr 1
		_tcsncpy_s(lpstAppRegData->szSynTPSendIPAddr1, MAX_IPV4_ADDRESS, L"10.0.100.128", _TRUNCATE);	// SynTP Helper Send IP Addr 1
		lpstAppRegData->dwSynTPPortNo1 = 50008;						// SynTP Helper Port Number 1

		// for Settings
		lpstAppRegData->dwSettingsX = 0;							// Settings Position X
		lpstAppRegData->dwSettingsY = 0;							// Settings Position Y
		lpstAppRegData->dwSettingsWidth = 0;						// Settings Position Width
		lpstAppRegData->dwSettingsHeight = 0;						// Settings Position Height
		lpstAppRegData->bIsPaneOpen = TRUE;							// Is Pane Open

	}
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
BOOL		CProfile::bGetProfileData() const
{
// \HKEY_CURRENT_USER\SOFTWARE\JeffyTS\FlushMouse
#define PROFILE_HKEY		HKEY_CURRENT_USER
#define PROFILE_SUBKEY		_T("SOFTWARE\\JeffyTS\\FlushMouse")

	if (lpstAppRegData == NULL) return FALSE;

	BOOL		bRet = FALSE;
	CRegistry* CReg = new CRegistry;
	if (CReg == NULL)	return FALSE;

	// for Bug
	DWORD	dw = 0;
	if (CReg->bReadRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CusrorSize"), (LPDWORD)&dw)) {
		if (dw != 0) {
			if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CursorSize"), (LPDWORD)&lpstAppRegData->iCursorSize, dw))	goto Cleanup;
			if (!CReg->bDeleteRegValue(PROFILE_HKEY, PROFILE_SUBKEY, _T("CusrorSize")))	goto Cleanup;
		}
	}

	// DWORD registry for Cursor
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CursorSize"), (LPDWORD)&lpstAppRegData->iCursorSize, lpstAppRegData->iCursorSize))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("ModeSize"), (LPDWORD)&lpstAppRegData->iModeSize, lpstAppRegData->iModeSize))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("InThreadSleepTime"), (LPDWORD)&lpstAppRegData->dwInThreadSleepTime, lpstAppRegData->dwInThreadSleepTime))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("WaitWaveTime"), (LPDWORD)&lpstAppRegData->dwWaitWaveTime, lpstAppRegData->dwWaitWaveTime))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("AdditionalWaitTime"), (LPDWORD)&lpstAppRegData->dwAdditionalWaitTime, lpstAppRegData->dwAdditionalWaitTime))	goto Cleanup;
	//if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayWaitTime"), (LPDWORD)&lpstAppRegData->dwDisplayWaitTime, lpstAppRegData->dwDisplayWaitTime))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayModeTime"), (LPDWORD)&lpstAppRegData->dwDisplayModeTime, lpstAppRegData->dwDisplayModeTime))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseColor, lpstAppRegData->dwNearDrawMouseColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretColor, lpstAppRegData->dwNearDrawCaretColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearMouseColor"), (LPDWORD)&lpstAppRegData->dwNearMouseColor, lpstAppRegData->dwNearMouseColor))	goto Cleanup;

	// BOOL registry for Cursor
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeOnCursor"), (LPBOOL)&lpstAppRegData->bDisplayIMEModeOnCursor, lpstAppRegData->bDisplayIMEModeOnCursor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeByWindow"), (LPBOOL)&lpstAppRegData->bDisplayIMEModeByWindow, lpstAppRegData->bDisplayIMEModeByWindow))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("ForceHiragana"), (LPBOOL)&(lpstAppRegData->bForceHiragana), lpstAppRegData->bForceHiragana))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DenyChangedByApp"), (LPBOOL)&(lpstAppRegData->bDenyChangedByApp), lpstAppRegData->bDenyChangedByApp))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("UseBigArrow"), (LPBOOL)&(lpstAppRegData->bUseBigArrow), lpstAppRegData->bUseBigArrow))	goto Cleanup;

	// BOOL registry for FlushMouse & Cursor
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayFocusWindowIME"), (LPBOOL)&(lpstAppRegData->bDisplayFocusWindowIME), lpstAppRegData->bDisplayFocusWindowIME))	goto Cleanup;

	// BOOL registry for FlushMouse
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByMouseBttnUp"), (LPBOOL)&(lpstAppRegData->bDoModeDispByMouseBttnUp), lpstAppRegData->bDoModeDispByMouseBttnUp))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByIMEKeyDown"), (LPBOOL)&(lpstAppRegData->bDoModeDispByIMEKeyDown), lpstAppRegData->bDoModeDispByIMEKeyDown))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByCtrlUp"), (LPBOOL)&(lpstAppRegData->bDoModeDispByCtrlUp), lpstAppRegData->bDoModeDispByCtrlUp))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("IMEModeForced"), (LPBOOL)&(lpstAppRegData->bIMEModeForced), lpstAppRegData->bIMEModeForced))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("MoveIMEToolbar"), (LPBOOL)&(lpstAppRegData->bMoveIMEToolbar), lpstAppRegData->bMoveIMEToolbar))	goto Cleanup;

	// BOOL registry for FlushMouse & FlushMouseSub
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("OffChangedFocus"), (LPBOOL)&(lpstAppRegData->bOffChangedFocus), lpstAppRegData->bOffChangedFocus))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DrawNearCaret"), (LPBOOL)&(lpstAppRegData->bDrawNearCaret), lpstAppRegData->bDrawNearCaret))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("EnableEPHelper"), (LPBOOL)&(lpstAppRegData->bEnableEPHelper), lpstAppRegData->bEnableEPHelper))	goto Cleanup;

	// Registry for SynTP
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPStarted1"), (LPBOOL)&(lpstAppRegData->bSynTPStarted1), lpstAppRegData->bSynTPStarted1))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPPadX"), (LPDWORD)&(lpstAppRegData->dwSynTPPadX), lpstAppRegData->dwSynTPPadX))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPPadY"), (LPDWORD)&(lpstAppRegData->dwSynTPPadY), lpstAppRegData->dwSynTPPadY))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPEdgeX"), (LPDWORD)&(lpstAppRegData->dwSynTPEdgeX), lpstAppRegData->dwSynTPEdgeX))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPEdgeY"), (LPDWORD)&(lpstAppRegData->dwSynTPEdgeY), lpstAppRegData->dwSynTPEdgeY))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPPadX"), (LPDWORD)&(lpstAppRegData->dwSynTPPadX), lpstAppRegData->dwSynTPPadX))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPHelper1"), (LPDWORD)&(lpstAppRegData->dwSynTPHelper1), lpstAppRegData->dwSynTPHelper1))	goto Cleanup;
	if (!CReg->bGetSetRegValueString(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPSendIPAddr1"), lpstAppRegData->szSynTPSendIPAddr1, sizeof(lpstAppRegData->szSynTPSendIPAddr1)))	goto Cleanup;
	if (!CReg->bGetSetRegValueString(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPSendHostname1"), lpstAppRegData->szSynTPSendHostname1, sizeof(lpstAppRegData->szSynTPSendHostname1)))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPPortNo1"), (LPDWORD) & lpstAppRegData->dwSynTPPortNo1, lpstAppRegData->dwSynTPPortNo1))	goto Cleanup;

	// Registry in Use Settings
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsX"), (LPDWORD)&(lpstAppRegData->dwSettingsX), lpstAppRegData->dwSettingsX))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsY"), (LPDWORD)&(lpstAppRegData->dwSettingsY), lpstAppRegData->dwSettingsY))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsWidth"), (LPDWORD)&(lpstAppRegData->dwSettingsWidth), lpstAppRegData->dwSettingsWidth))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsHeight"), (LPDWORD)&(lpstAppRegData->dwSettingsHeight), lpstAppRegData->dwSettingsHeight))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsIsPaneOpen"), (LPBOOL)&(lpstAppRegData->bIsPaneOpen), lpstAppRegData->bIsPaneOpen))	goto Cleanup;

	bRet = TRUE;

Cleanup:
	if (CReg)	delete	CReg;
	return bRet;
#undef	PROFILE_HKEY
#undef	PROFILE_SUBKEY
}

//
// bSetProfileData()
//
BOOL		CProfile::bSetProfileData() const
{
// \HKEY_CURRENT_USER\SOFTWARE\JeffyTS\FlushMouse
#define PROFILE_HKEY		HKEY_CURRENT_USER
#define PROFILE_SUBKEY		_T("SOFTWARE\\JeffyTS\\FlushMouse")

	if (lpstAppRegData == NULL) return FALSE;

	BOOL		bRet = FALSE;
	CRegistry* CReg = new CRegistry;
	if (CReg == NULL)	return FALSE;

	// DWORD registry in Use Cursor
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CursorSize"), lpstAppRegData->iCursorSize))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("ModeSize"), lpstAppRegData->iModeSize))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("InThreadSleepTime"), lpstAppRegData->dwInThreadSleepTime))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("WaitWaveTime"), lpstAppRegData->dwWaitWaveTime))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("AdditionalWaitTime"), lpstAppRegData->dwAdditionalWaitTime))	goto Cleanup;
	//if (CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayWaitTime"), lpstAppRegData->dwDisplayWaitTime))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayModeTime"), lpstAppRegData->dwDisplayModeTime))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseColor"), lpstAppRegData->dwNearDrawMouseColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretColor"), lpstAppRegData->dwNearDrawCaretColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearMouseColor"), lpstAppRegData->dwNearMouseColor))	goto Cleanup;

	// BOOL registry in Use Cursor
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeOnCursor"), lpstAppRegData->bDisplayIMEModeOnCursor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeByWindow"), lpstAppRegData->bDisplayIMEModeByWindow))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("ForceHiragana"), lpstAppRegData->bForceHiragana))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DenyChangedByApp"), lpstAppRegData->bDenyChangedByApp))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("UseBigArrow"), lpstAppRegData->bUseBigArrow))	goto Cleanup;

	// BOOL registry for FlushMouse & FlushMouseSub
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayFocusWindowIME"), lpstAppRegData->bDisplayFocusWindowIME))	goto Cleanup;

	// BOOL registry in Use FlushMouse
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByIMEKeyDown"), lpstAppRegData->bDoModeDispByIMEKeyDown))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByCtrlUp"), lpstAppRegData->bDoModeDispByCtrlUp))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByMouseBttnUp"), lpstAppRegData->bDoModeDispByMouseBttnUp))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("IMEModeForced"), lpstAppRegData->bIMEModeForced))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("MoveIMEToolbar"), lpstAppRegData->bMoveIMEToolbar))	goto Cleanup;

	// BOOL registry in Use FlushMouse & FlushMouseSub
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("OffChangedFocus"), lpstAppRegData->bOffChangedFocus))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DrawNearCaret"), lpstAppRegData->bDrawNearCaret))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("EnableEPHelper"), lpstAppRegData->bEnableEPHelper))	goto Cleanup;

	// Registry in Use SynTP
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPStarted1"), lpstAppRegData->bSynTPStarted1))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPPadX"), lpstAppRegData->dwSynTPPadX))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPPadY"), lpstAppRegData->dwSynTPPadY))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPEdgeX"), lpstAppRegData->dwSynTPEdgeX))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPEdgeY"), lpstAppRegData->dwSynTPEdgeY))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPHelper1"), lpstAppRegData->dwSynTPHelper1))	goto Cleanup;
	if (!CReg->bSetRegValueString(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPSendIPAddr1"), lpstAppRegData->szSynTPSendIPAddr1, sizeof(lpstAppRegData->szSynTPSendIPAddr1)))	goto Cleanup;
	if (!CReg->bSetRegValueString(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPSendHostname1"), lpstAppRegData->szSynTPSendHostname1, sizeof(lpstAppRegData->szSynTPSendHostname1)))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPPortNo1"), lpstAppRegData->dwSynTPPortNo1))	goto Cleanup;

	// Registry in Use Settings
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsX"), lpstAppRegData->dwSettingsX))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsY"), lpstAppRegData->dwSettingsY))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsWidth"), lpstAppRegData->dwSettingsWidth))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsHeight"), lpstAppRegData->dwSettingsHeight))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsIsPaneOpen"), lpstAppRegData->bIsPaneOpen))	goto Cleanup;


	bRet = TRUE;

Cleanup:
	if (CReg)	delete	CReg;	
	return bRet;
#undef	PROFILE_HKEY
#undef	PROFILE_SUBKEY
}


/* = EOF = */