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

//
// Define
//
// \HKEY_CURRENT_USER\SOFTWARE\JeffyTS\FlushMouse
#define PROFILE_HKEY		HKEY_CURRENT_USER
#define PROFILE_SUBKEY		_T("SOFTWARE\\JeffyTS\\FlushMouse")

//
// Global Data
//

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

		lpstAppRegData->iCursorSize = 32;												// 表示するマウスカーソルのサイズ
		lpstAppRegData->iModeSize = 24;													// 表示するIMEモードのサイズ
		lpstAppRegData->iIMEModeDistance = 32;											// 表示する IME Mode の距離
		lpstAppRegData->dwInThreadSleepTime = 0;										// Thread内の待ち時間(IMECursorChangeThreadのみ)
		lpstAppRegData->dwWaitWaveTime = 400;											// IME mode displayの waveの待ち時間
		lpstAppRegData->dwAdditionalWaitTime = 300;										// IME mode displayの 追加待ち時間
		lpstAppRegData->dwDisplayModeTime = 400;										// IME mode displayの表示時間
		lpstAppRegData->bForceHiragana = FALSE;											// 「全角ひらがな」へ強制的に変更する
		lpstAppRegData->dwNearDrawMouseIMEOFFColor = aRGB(48, 255, 0, 0);				// IMEOFF マウスカーソルへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
		lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor = aRGB(48, 255, 0, 0);		// 半英数　マウスカーソルへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
		lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor = aRGB(48, 255, 0, 0);		// 半カナ　マウスカーソルへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
		lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor = aRGB(48, 255, 0, 0);		// 全英数　マウスカーソルへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
		lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor = aRGB(48, 255, 0, 0);		// 全ひら　マウスカーソルへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
		lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor = aRGB(48, 255, 0, 0);		// 全カナ　マウスカーソルへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
		lpstAppRegData->dwNearDrawCaretIMEOFFColor = aRGB(48, 0, 0, 255);				// IMEOFF キャレットへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0) / RGB(96, 124, 255)
		lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor = aRGB(48, 0, 0, 255);		// 半英数　キャレットへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0) / RGB(96, 124, 255)
		lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor = aRGB(48, 0, 0, 255);		// 半カナ　キャレットへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0) / RGB(96, 124, 255)
		lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor = aRGB(48, 0, 0, 255);		// 全英数　キャレットへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0) / RGB(96, 124, 255)
		lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor = aRGB(48, 0, 0, 255);		// 全ひら　キャレットへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0) / RGB(96, 124, 255)
		lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor = aRGB(48, 0, 0, 255);		// 全カナ　キャレットへのIMEモード表示色 RGB(255, 192, 0) + α (0xf0) / RGB(96, 124, 255)
		lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor = aRGB(48, 255, 0, 0);			// IMEOFF マウスカーソル右下へのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
		lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor = aRGB(48, 255, 0, 0);	// 半英数　マウスカーソル右下へのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
		lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor = aRGB(48, 255, 0, 0);	// 半カナ　マウスカーソル右下へのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
		lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor = aRGB(48, 255, 0, 0);	// 全英数　マウスカーソル右下へのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
		lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor = aRGB(48, 255, 0, 0);	// 全ひら　マウスカーソル右下へのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)
		lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor = aRGB(48, 255, 0, 0);	// 全カナ　マウスカーソル右下へのIMEモード表示色 RGB(255, 192, 0) + α (0xf0)

		lpstAppRegData->bDisplayIMEModeOnCursor = TRUE;				// マウスカーソルへのIMEモード表示
		lpstAppRegData->bDisplayIMEModeByWindow = FALSE;			// マウスカーソルへのIMEモード表示 (Window版)
		lpstAppRegData->bDisplayIMEModeIMEOFF = FALSE;				// IME OFFの時のIMEモード表示
		lpstAppRegData->bDisplayFocusWindowIME = FALSE;				// フォーカスウィンドウのIMEモードを表示する(TRUE)/マウスカーソル下のウィンドウのIMEモードを表示する(FALSE)

		lpstAppRegData->bDoModeDispByIMEKeyDown = TRUE;				// IMEのモード変更のキーが押されたときにIMEモードを表示する
		lpstAppRegData->bDoModeDispByMouseBttnUp = TRUE;			// Mouse L/Rボタンが離されたときにIMEモードを表示する
		lpstAppRegData->bDoModeDispByCtrlUp = TRUE;					// Ctrlが離されたときにIMEモードを表示する
		lpstAppRegData->bIMEModeForced = FALSE;						// Change IME Mode Forced
		lpstAppRegData->bEnableEPHelper = FALSE;					// for Explorer Patcher Simple Window Switcher Helper

		lpstAppRegData->bOffChangedFocus = FALSE;					// アプリケーションが切り替わったときIMEをOFFにする(bDisplayFocusWindowIMEとは排他的動作になる)
		lpstAppRegData->bDrawNearCaret = FALSE;						// Caretの横にIMEモードを表示
		lpstAppRegData->bMoveIMEToolbar = FALSE;					// New IMEのToolbarを移動する 

		lpstAppRegData->bSynTPStarted1 = FALSE;						// SynTP_Helperが起動状態か
		lpstAppRegData->dwSynTPPadX = 528;							// TouchPad X
		lpstAppRegData->dwSynTPPadY = 528;							// TouchPad X
		lpstAppRegData->dwSynTPEdgeX = 88;							// Edge X
		lpstAppRegData->dwSynTPEdgeY = 88;							// Edge Y
		lpstAppRegData->dwSynTPHelper1 = SYNTPH_DISABLE;			// SynTP Helper #1 (0 = disable  0x01 = sender (IPv4)           / 0x02 = sender (always start IPV4)           / 0x03 = receiver (IPv4) / 0x04 =receiver (always start IPv4)
																	//                               0x11 = sender (Hosstname IPV4) / 0x12 = sender (always start Hosstname IPV4)
																	//                               0x31 = sender (Hosstname IPV6) / 0x32 = sender (always start Hosstname IPV6) / 0x33 = receiver (IPv6) / 0x34 =receiver (always start IPv6)
		_tcsncpy_s(lpstAppRegData->szSynTPSendIPAddr1, MAX_IPV4_ADDRESS, L"10.0.100.128", _TRUNCATE);	// SynTP Helper Send IP Addr 1
		_tcsncpy_s(lpstAppRegData->szSynTPSendHostname1, MAX_FQDN, L"", _TRUNCATE);			// SynTP Helper Hostname 1
		lpstAppRegData->dwSynTPPortNo1 = 50008;						// SynTP Helper Port Number 1

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
	if (lpstAppRegData == NULL) return FALSE;

	BOOL		bRet = FALSE;

	if (!bGetProfileData4FlushMouse())	goto Cleanup;
	if (!bGetProfileData4SynTPHelper())	goto Cleanup;
	if (!bGetProfileData4Settings())	goto Cleanup;

	bRet = TRUE;

Cleanup:
	return bRet;
}

//
// bSetProfileData()
//
BOOL		CProfile::bSetProfileData() const
{
	if (lpstAppRegData == NULL) return FALSE;

	BOOL		bRet = FALSE;

	if (!bSetProfileData4FlushMouse())	goto Cleanup;
	if (!bSetProfileData4SynTPHelper())	goto Cleanup;
	if (!bSetProfileData4Settings())	goto Cleanup;

	bRet = TRUE;

Cleanup:
	return bRet;
}

//
// bFixChangedProfileData()
//
BOOL		CProfile::bFixChangedProfileData() const
{
	if (lpstAppRegData == NULL) return FALSE;

	BOOL		bRet = FALSE;
	CRegistry	*CReg = new CRegistry;
	if (CReg == NULL)	return FALSE;

	// for Bug
	DWORD	dw = 0;
	if (CReg->bReadRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CusrorSize"), (LPDWORD)&dw)) {
		if (dw != 0) {
			if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CursorSize"), (LPDWORD)&lpstAppRegData->iCursorSize, dw))	goto Cleanup;
			if (!CReg->bDeleteRegValue(PROFILE_HKEY, PROFILE_SUBKEY, _T("CusrorSize")))	goto Cleanup;
		}
	}

	// for Ver.3.3
	if (CReg->bReadRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseZENHIRA_IMEONColor"), (LPDWORD)&dw)) {
		if (dw == 0) {
			if (CReg->bReadRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseColor"), (LPDWORD)&dw)) {
				if (dw != 0) {
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseIMEOFFColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseIMEOFFColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseHANEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseHANKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseZENEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseZENHIRA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseZENKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor, dw))	goto Cleanup;
				}
			}
			if (CReg->bReadRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretColor"), (LPDWORD)&dw)) {
				if (dw != 0) {

					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretIMEOFFColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretIMEOFFColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretHANEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretHANKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretZENEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretZENHIRA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretZENKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor, dw))	goto Cleanup;
				}
			}
			if (CReg->bReadRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearMouseColor"), (LPDWORD)&dw)) {
				if (dw != 0) {
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndIMEOFFColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndHANEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndHANKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndZENEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndZENHIRA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor, dw))	goto Cleanup;
					if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndZENKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor, dw))	goto Cleanup;
				}
			}
		}
	}
	bRet = TRUE;
Cleanup:
	if (CReg)	delete	CReg;
	return bRet;

}

//
// bGetProfileData4FlushMouse()
//
BOOL		CProfile::bGetProfileData4FlushMouse() const
{
	if (lpstAppRegData == NULL) return FALSE;

	BOOL		bRet = FALSE;
	CRegistry	*CReg = new CRegistry;
	if (CReg == NULL)	return FALSE;

	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CursorSize"), (LPDWORD)&lpstAppRegData->iCursorSize, lpstAppRegData->iCursorSize))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("IMEModeDistance"), (LPDWORD)&lpstAppRegData->iIMEModeDistance, lpstAppRegData->iIMEModeDistance))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("ModeSize"), (LPDWORD)&lpstAppRegData->iModeSize, lpstAppRegData->iModeSize))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("InThreadSleepTime"), (LPDWORD)&lpstAppRegData->dwInThreadSleepTime, lpstAppRegData->dwInThreadSleepTime))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("WaitWaveTime"), (LPDWORD)&lpstAppRegData->dwWaitWaveTime, lpstAppRegData->dwWaitWaveTime))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("AdditionalWaitTime"), (LPDWORD)&lpstAppRegData->dwAdditionalWaitTime, lpstAppRegData->dwAdditionalWaitTime))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayModeTime"), (LPDWORD)&lpstAppRegData->dwDisplayModeTime, lpstAppRegData->dwDisplayModeTime))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseIMEOFFColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseIMEOFFColor, lpstAppRegData->dwNearDrawMouseIMEOFFColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseHANEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor, lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseHANKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor, lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseZENEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor, lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseZENHIRA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor, lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseZENKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor, lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretIMEOFFColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretIMEOFFColor, lpstAppRegData->dwNearDrawCaretIMEOFFColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretHANEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor, lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretHANKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor, lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretZENEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor, lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretZENHIRA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor, lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretZENKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor, lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndIMEOFFColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor, lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndHANEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor, lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndHANKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor, lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndZENEISU_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor, lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndZENHIRA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor, lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndZENKANA_IMEONColor"), (LPDWORD)&lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor, lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor))	goto Cleanup;

	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeOnCursor"), (LPBOOL)&lpstAppRegData->bDisplayIMEModeOnCursor, lpstAppRegData->bDisplayIMEModeOnCursor))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeByWindow"), (LPBOOL)&lpstAppRegData->bDisplayIMEModeByWindow, lpstAppRegData->bDisplayIMEModeByWindow))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeIMEOFF"), (LPBOOL)&lpstAppRegData->bDisplayIMEModeIMEOFF, lpstAppRegData->bDisplayIMEModeIMEOFF))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("ForceHiragana"), (LPBOOL)&(lpstAppRegData->bForceHiragana), lpstAppRegData->bForceHiragana))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DenyChangedByApp"), (LPBOOL)&(lpstAppRegData->bDenyChangedByApp), lpstAppRegData->bDenyChangedByApp))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("UseBigArrow"), (LPBOOL)&(lpstAppRegData->bUseBigArrow), lpstAppRegData->bUseBigArrow))	goto Cleanup;

	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayFocusWindowIME"), (LPBOOL)&(lpstAppRegData->bDisplayFocusWindowIME), lpstAppRegData->bDisplayFocusWindowIME))	goto Cleanup;

	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByMouseBttnUp"), (LPBOOL)&(lpstAppRegData->bDoModeDispByMouseBttnUp), lpstAppRegData->bDoModeDispByMouseBttnUp))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByIMEKeyDown"), (LPBOOL)&(lpstAppRegData->bDoModeDispByIMEKeyDown), lpstAppRegData->bDoModeDispByIMEKeyDown))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByCtrlUp"), (LPBOOL)&(lpstAppRegData->bDoModeDispByCtrlUp), lpstAppRegData->bDoModeDispByCtrlUp))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("IMEModeForced"), (LPBOOL)&(lpstAppRegData->bIMEModeForced), lpstAppRegData->bIMEModeForced))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("MoveIMEToolbar"), (LPBOOL)&(lpstAppRegData->bMoveIMEToolbar), lpstAppRegData->bMoveIMEToolbar))	goto Cleanup;

	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("OffChangedFocus"), (LPBOOL)&(lpstAppRegData->bOffChangedFocus), lpstAppRegData->bOffChangedFocus))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DrawNearCaret"), (LPBOOL)&(lpstAppRegData->bDrawNearCaret), lpstAppRegData->bDrawNearCaret))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("EnableEPHelper"), (LPBOOL)&(lpstAppRegData->bEnableEPHelper), lpstAppRegData->bEnableEPHelper))	goto Cleanup;

	bRet = TRUE;

Cleanup:
	if (CReg)	delete	CReg;
	return bRet;
}

//
// bSetProfileData4FlushMouse()
//
BOOL		CProfile::bSetProfileData4FlushMouse() const
{
	if (lpstAppRegData == NULL) return FALSE;

	BOOL		bRet = FALSE;
	CRegistry	*CReg = new CRegistry;
	if (CReg == NULL)	return FALSE;

	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("CursorSize"), lpstAppRegData->iCursorSize))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("IMEModeDistance"), lpstAppRegData->iIMEModeDistance))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("ModeSize"), lpstAppRegData->iModeSize))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("InThreadSleepTime"), lpstAppRegData->dwInThreadSleepTime))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("WaitWaveTime"), lpstAppRegData->dwWaitWaveTime))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("AdditionalWaitTime"), lpstAppRegData->dwAdditionalWaitTime))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayModeTime"), lpstAppRegData->dwDisplayModeTime))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseIMEOFFColor"), lpstAppRegData->dwNearDrawMouseIMEOFFColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseHANEISU_IMEONColor"), lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseHANKANA_IMEONColor"), lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseZENEISU_IMEONColor"), lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseZENHIRA_IMEONColor"), lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseZENKANA_IMEONColor"), lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretIMEOFFColor"), lpstAppRegData->dwNearDrawCaretIMEOFFColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretHANEISU_IMEONColor"), lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretHANKANA_IMEONColor"), lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretZENEISU_IMEONColor"), lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretZENHIRA_IMEONColor"), lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawCaretZENKANA_IMEONColor"), lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndIMEOFFColor"), lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndHANEISU_IMEONColor"), lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndHANKANA_IMEONColor"), lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndZENEISU_IMEONColor"), lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndZENHIRA_IMEONColor"), lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("NearDrawMouseByWndZENKANA_IMEONColor"), lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor))	goto Cleanup;

	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeOnCursor"), lpstAppRegData->bDisplayIMEModeOnCursor))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeByWindow"), lpstAppRegData->bDisplayIMEModeByWindow))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayIMEModeIMEOFF"), lpstAppRegData->bDisplayIMEModeIMEOFF))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("ForceHiragana"), lpstAppRegData->bForceHiragana))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DenyChangedByApp"), lpstAppRegData->bDenyChangedByApp))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("UseBigArrow"), lpstAppRegData->bUseBigArrow))	goto Cleanup;

	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DisplayFocusWindowIME"), lpstAppRegData->bDisplayFocusWindowIME))	goto Cleanup;

	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByIMEKeyDown"), lpstAppRegData->bDoModeDispByIMEKeyDown))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByCtrlUp"), lpstAppRegData->bDoModeDispByCtrlUp))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DoModeDispByMouseBttnUp"), lpstAppRegData->bDoModeDispByMouseBttnUp))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("IMEModeForced"), lpstAppRegData->bIMEModeForced))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("MoveIMEToolbar"), lpstAppRegData->bMoveIMEToolbar))	goto Cleanup;

	// BOOL registry in Use FlushMouse & FlushMouseSub
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("OffChangedFocus"), lpstAppRegData->bOffChangedFocus))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("DrawNearCaret"), lpstAppRegData->bDrawNearCaret))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("EnableEPHelper"), lpstAppRegData->bEnableEPHelper))	goto Cleanup;

	bRet = TRUE;

Cleanup:
	if (CReg)	delete	CReg;
	return bRet;
}

//
// bGetProfileData4SynTPHelper()
//
BOOL		CProfile::bGetProfileData4SynTPHelper() const
{
	if (lpstAppRegData == NULL) return FALSE;

	BOOL		bRet = FALSE;
	CRegistry	*CReg = new CRegistry;
	if (CReg == NULL)	return FALSE;

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

	bRet = TRUE;

Cleanup:
	if (CReg)	delete	CReg;
	return bRet;
}

//
// bSetProfileData4SynTPHelper()
//
BOOL		CProfile::bSetProfileData4SynTPHelper() const
{
	if (lpstAppRegData == NULL) return FALSE;

	BOOL		bRet = FALSE;
	CRegistry	*CReg = new CRegistry;
	if (CReg == NULL)	return FALSE;

	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPStarted1"), lpstAppRegData->bSynTPStarted1))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPPadX"), lpstAppRegData->dwSynTPPadX))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPPadY"), lpstAppRegData->dwSynTPPadY))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPEdgeX"), lpstAppRegData->dwSynTPEdgeX))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPEdgeY"), lpstAppRegData->dwSynTPEdgeY))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPHelper1"), lpstAppRegData->dwSynTPHelper1))	goto Cleanup;
	if (!CReg->bSetRegValueString(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPSendIPAddr1"), lpstAppRegData->szSynTPSendIPAddr1, sizeof(lpstAppRegData->szSynTPSendIPAddr1)))	goto Cleanup;
	if (!CReg->bSetRegValueString(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPSendHostname1"), lpstAppRegData->szSynTPSendHostname1, sizeof(lpstAppRegData->szSynTPSendHostname1)))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SynTPPortNo1"), lpstAppRegData->dwSynTPPortNo1))	goto Cleanup;

	bRet = TRUE;

Cleanup:
	if (CReg)	delete	CReg;
	return bRet;
}

//
// bGetProfileData4Settings()
//
BOOL		CProfile::bGetProfileData4Settings() const
{
	if (lpstAppRegData == NULL) return FALSE;

	BOOL		bRet = FALSE;
	CRegistry	*CReg = new CRegistry;
	if (CReg == NULL)	return FALSE;

	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsX"), (LPDWORD)&(lpstAppRegData->dwSettingsX), lpstAppRegData->dwSettingsX))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsY"), (LPDWORD)&(lpstAppRegData->dwSettingsY), lpstAppRegData->dwSettingsY))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsWidth"), (LPDWORD)&(lpstAppRegData->dwSettingsWidth), lpstAppRegData->dwSettingsWidth))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsHeight"), (LPDWORD)&(lpstAppRegData->dwSettingsHeight), lpstAppRegData->dwSettingsHeight))	goto Cleanup;
	if (!CReg->bGetSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsIsPaneOpen"), (LPBOOL)&(lpstAppRegData->bIsPaneOpen), lpstAppRegData->bIsPaneOpen))	goto Cleanup;

	bRet = TRUE;

Cleanup:
	if (CReg)	delete	CReg;
	return bRet;
}

//
// bSetProfileData4Settings()
//
BOOL		CProfile::bSetProfileData4Settings() const
{
	if (lpstAppRegData == NULL) return FALSE;

	BOOL		bRet = FALSE;
	CRegistry	*CReg = new CRegistry;
	if (CReg == NULL)	return FALSE;

	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsX"), lpstAppRegData->dwSettingsX))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsY"), lpstAppRegData->dwSettingsY))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsWidth"), lpstAppRegData->dwSettingsWidth))	goto Cleanup;
	if (!CReg->bSetRegValueDWORD(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsHeight"), lpstAppRegData->dwSettingsHeight))	goto Cleanup;
	if (!CReg->bSetRegValueDWORDasBOOL(PROFILE_HKEY, PROFILE_SUBKEY, _T("SettingsIsPaneOpen"), lpstAppRegData->bIsPaneOpen))	goto Cleanup;

	bRet = TRUE;

Cleanup:
	if (CReg)	delete	CReg;
	return bRet;
}


/* = EOF = */