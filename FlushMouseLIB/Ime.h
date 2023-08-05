// Ime.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/12  JeffyTS  	New edit.
// #0001	2023/06/30	JeffyTS		Divided from cursor.h

//
// Include
//
#pragma once
#include "pch.h"

//
// Define
//
#define	IMEMODECHAR		3

// IME mode
#define IMECLOSE		0x0000
#define IMEOPEN			0x0001
#define IMEOFF			(IME_CMODE_ALPHANUMERIC)														// 0x00 : A 英数(直接入力)
#define HANEISU_IMEON	(IME_CMODE_ROMAN | IME_CMODE_ALPHANUMERIC)										// 0x10 :_A 半角英数 0001 0000
#define HANKANA_IMEON	(IME_CMODE_ROMAN | IME_CMODE_KATAKANA | IME_CMODE_NATIVE)						// 0x13 :_ｶ 半角カナ 0001 0011
#define ZENEISU_IMEON	(IME_CMODE_ROMAN | IME_CMODE_FULLSHAPE)											// 0x18 :Ａ 全角英数 0001 1000
#define ZENHIRA_IMEON	(IME_CMODE_ROMAN | IME_CMODE_FULLSHAPE | IME_CMODE_NATIVE)						// 0x19 :あ 全角ひら 0001 1001（漢字変換モード)
#define ZENKANA_IMEON	(IME_CMODE_ROMAN | IME_CMODE_FULLSHAPE | IME_CMODE_KATAKANA | IME_CMODE_NATIVE)	// 0x1B :カ 全角カナ 0001 1011
#define IMEHIDE			(0x88)																			// 0x88 :■  Hide    1000 1000

//
// Struct Define
//
typedef struct tagVirtualDesktop
{
	RECT	rcMonitorSize;
	int		iNumOfMonitors;
} VIRTUALDESKTOP, * PVIRTUALDESKTOP, * LPVIRTUALDESKTOP;

//
// Class CIME
//
class CIME
{
public:
	CIME();
	~CIME();

public:
	BOOL		bIsIMEOpen(HWND hWndObserved);
	VOID		vIMEOpenCloseForced(HWND hWndObserved, DWORD dwIMEOpenClose);
	VOID		vIMEConvertModeChangeForced(HWND hWndObserved, DWORD dwConvertMode);
	DWORD		dwIMEMode(HWND hWndObserved, BOOL bForceHiragana);
	VOID		vActivateIME(HWND hWndObserved);
	BOOL		bGetVirtualDesktopSize();
	BOOL		bIsNewIME();
	DWORD		dwGetInputLocale();
	HKL			hklGetInputLocale(HWND hWndObserved);

private:
	static BOOL bGetVirtualDesktopSizeEnumProc(HMONITOR hMonitor, HDC hDC, LPCRECT lprcClip, LPARAM lParam);

	static BOOL CALLBACK bEnumChildProcIMEOpenClose(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK bEnumChildProcIMEConvertMode(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK bEnumChildProcActivateIME(HWND hWnd, LPARAM lParam);

private:
	LPVIRTUALDESKTOP	lpstVirtualDesktop;
};


/* = EOF = */