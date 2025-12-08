//
// Ime.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/03  JeffyTS  	New edit.
//

// Include
//
#pragma once
#include "pch.h"
#include "Ime.h"
#include "CommonDef.h"
#include "..\MiscLIB\CRegistry.h"

//
// Library
//

//
// Define
//
#define SENDMESSAGETIMEOUT	100

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
}

CIME::~CIME()
{
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
VOID		CIME::vIMEConvertModeChangeForced(HWND hWnd, DWORD dwConvertMode)
{
	if (hWnd == NULL)	return;
	LPARAM	lParam = (LPARAM)dwConvertMode;
	EnumChildWindows(hWnd, &bEnumChildProcIMEConvertMode, lParam);
	return;
}

//
// bEnumChildProcIMEConvertMode()
//
BOOL CALLBACK CIME::bEnumChildProcIMEConvertMode(HWND hWnd, LPARAM lParam)
{
	if (hWnd == NULL) return FALSE;
	HWND hIMWnd = ImmGetDefaultIMEWnd(hWnd);
	if (hIMWnd == NULL) return TRUE;
	DWORD_PTR dwRes = 0;
	LRESULT lResult = SendMessageTimeout(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETOPENSTATUS, (LPARAM)TRUE,
											SMTO_ABORTIFHUNG, SENDMESSAGETIMEOUT, &dwRes);
	if ((lResult == 0) || (dwRes == 0)) {
		(void)SendMessageTimeout(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETCONVERSIONMODE, lParam,
											SMTO_ABORTIFHUNG, SENDMESSAGETIMEOUT, &dwRes);
	}
	return TRUE;
}

//
// bEnumChildProcIMEOpenClose()
//
BOOL CALLBACK CIME::bEnumChildProcIMEOpenClose(HWND hWnd, LPARAM lParam)
{
	if (hWnd == NULL)	return FALSE;
	HWND    hIMWnd = NULL;
	if ((hIMWnd = ImmGetDefaultIMEWnd(hWnd)) != NULL) {
		DWORD_PTR dwRes = 0;
		(void)SendMessageTimeout(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETOPENSTATUS, lParam,
											SMTO_ABORTIFHUNG, SENDMESSAGETIMEOUT, &dwRes);
	}
	return TRUE;
}

//
// dwIMEMode()
//
DWORD		CIME::dwIMEMode(HWND hWnd, BOOL bForceHiragana)
{
	if (hWnd == NULL) return IMEOFF;
	HWND hIMWnd = ImmGetDefaultIMEWnd(hWnd);
	if (hIMWnd == NULL) return IMEOFF;
	DWORD_PTR dwRes = 0;
	LRESULT lResult = SendMessageTimeout(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETOPENSTATUS, NULL,
											SMTO_ABORTIFHUNG, SENDMESSAGETIMEOUT, &dwRes);
	if ((lResult == 0) || (dwRes == 0)) return IMEOFF;
	lResult = SendMessageTimeout(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETCONVERSIONMODE, NULL,
									SMTO_ABORTIFHUNG, SENDMESSAGETIMEOUT, &dwRes);
	if ((lResult == 0) || (dwRes == 0)) return IMEOFF;
	DWORD dwConvertMode = (DWORD)dwRes;
	DWORD dwResult = IMEOFF;
	const DWORD dwBaseMode = dwConvertMode & ~IME_CMODE_ROMAN;
	switch (dwBaseMode) {
		case IME_CMODE_NATIVE:
			dwResult = IMEOFF;
			break;
		case (ZENHIRA_IMEON & ~IME_CMODE_ROMAN):
			dwResult = ZENHIRA_IMEON;
			break;
		case (HANEISU_IMEON & ~IME_CMODE_ROMAN):
			dwResult = HANEISU_IMEON;
			break;
		case (HANKANA_IMEON & ~IME_CMODE_ROMAN):
			dwResult = HANKANA_IMEON;
			break;
		case (ZENEISU_IMEON & ~IME_CMODE_ROMAN):
			dwResult = ZENEISU_IMEON;
			break;
		case (ZENKANA_IMEON & ~IME_CMODE_ROMAN):
			dwResult = ZENKANA_IMEON;
			break;
		default:
			dwResult = IMEOFF;
	}
	if (bForceHiragana && (dwResult != ZENHIRA_IMEON)) {
		vIMEConvertModeChangeForced(hWnd, ZENHIRA_IMEON);
		dwResult = ZENHIRA_IMEON;
	}
	HKL hkl = hklGetInputLocale(hWnd);
	if (hkl != (HKL)0 && hkl != JP_IME) {
		dwResult = IMEOFF;
	}
	return dwResult;
}

//
// vActivateIME()
//
VOID		CIME::vActivateIME(HWND hWndObserved)
{
	if (!hWndObserved) return;
	EnumChildWindows(hWndObserved, &bEnumChildProcActivateIME, NULL);
}

//
// bEnumChildProcActivateIME()
//
BOOL CALLBACK CIME::bEnumChildProcActivateIME(HWND hWnd, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	HWND hIMWnd = ImmGetDefaultIMEWnd(hWnd);
	if (hIMWnd == NULL) return TRUE;
	DWORD_PTR dwRes = 0;
	(void)SendMessageTimeout(hIMWnd, WM_ACTIVATE, (WPARAM)WA_ACTIVE, (LPARAM)NULL,
								SMTO_ABORTIFHUNG, SENDMESSAGETIMEOUT, &dwRes);
	TCHAR szBuffer[_MAX_PATH];
	if (GetClassName(hIMWnd, szBuffer, _MAX_PATH) == 0) {
		return TRUE;
	}
	HWND hIMEWnd = FindWindow(szBuffer, NULL);
	if (hIMEWnd != NULL) {
		(void)SendMessageTimeout(hIMEWnd, WM_ACTIVATE, (WPARAM)WA_ACTIVE, (LPARAM)NULL,
									SMTO_ABORTIFHUNG, SENDMESSAGETIMEOUT, &dwRes);
	}
	return TRUE;
}

//
// dGetInputLocale()
//
DWORD		CIME::dwGetInputLocale()
{
#define SUBKEY	_T("SOFTWARE\\Microsoft\\Input\\Locales")
#define VALUE	_T("InputLocale")
	DWORD	dwInputLocale = 0;
	CRegistry	*CReg = new CRegistry;
	if (CReg) {
		(void)CReg->bReadSystemRegValueDWORD(HKEY_LOCAL_MACHINE, SUBKEY, VALUE, &dwInputLocale);
		delete	CReg;
	}
	return (dwInputLocale & 0x0000ffff);
#undef SUBKEY
#undef VALUE
}

//
// hklGetInputLocale()
//
HKL		CIME::hklGetInputLocale(HWND hWndObserved)
{
	if (!hWndObserved) return (HKL)0;
	
	DWORD dwProcessID = 0;
	DWORD dwThreadID = GetWindowThreadProcessId(hWndObserved, &dwProcessID);
	if (dwThreadID == 0) return (HKL)0;
	HKL hkl = GetKeyboardLayout(dwThreadID);
	if (hkl == NULL) return (HKL)0;
	int iKeyboardType = GetKeyboardType(1);
	if ((iKeyboardType != 2) || (hkl != JP_IME)) {
		hkl = US_ENG;
	}
	return hkl;
}


/* = EOF = */
