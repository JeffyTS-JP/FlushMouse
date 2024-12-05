//
// Ime.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/03/03  JeffyTS  	New edit.
// #0001		2023/06/30	JeffyTS		Divided from cursor.cpp
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
// bIsIMEOpen()
//
BOOL		CIME::bIsIMEOpen(HWND hWndObserved)
{
	HWND    hIMWnd = NULL;
	if ((hIMWnd = ImmGetDefaultIMEWnd(hWndObserved)) != NULL) {
		if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETOPENSTATUS, (LPARAM)NULL) != 0) {
			return TRUE;	
		}
	}
	return FALSE;
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
VOID		CIME::vIMEConvertModeChangeForced(HWND hWndObserved, DWORD dwConvertMode)
{
	if (hWndObserved == NULL)	return;
	LPARAM	lParam = (LPARAM)dwConvertMode;
	EnumChildWindows(hWndObserved, &bEnumChildProcIMEConvertMode, lParam);
	return;
}

//
// bEnumChildProcIMEConvertMode()
//
BOOL CALLBACK CIME::bEnumChildProcIMEConvertMode(HWND hWndObserved, LPARAM lParam)
{
	if (hWndObserved == NULL)	return FALSE;
	HWND    hIMWnd = NULL;
	if ((hIMWnd = ImmGetDefaultIMEWnd(hWndObserved)) != NULL) {
		if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETOPENSTATUS, (LPARAM)TRUE) == 0) {	
			if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETCONVERSIONMODE, (LPARAM)lParam) == 0) {
			}
		}
	}
	return TRUE;	
}

//
// bEnumChildProcIMEOpenClose()
//
BOOL CALLBACK CIME::bEnumChildProcIMEOpenClose(HWND hWndObserved, LPARAM lParam)
{
	if (hWndObserved == NULL)	return FALSE;
	HWND    hIMWnd = NULL;
	if ((hIMWnd = ImmGetDefaultIMEWnd(hWndObserved)) != NULL) {
		if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_SETOPENSTATUS, lParam) == 0) {
		}
	}
	return TRUE;	
}

//
// dwIMEMode()
//
DWORD		CIME::dwIMEMode(HWND hWndObserved, BOOL bForceHiragana)
{
	if (hWndObserved != NULL) {
		HWND    hIMWnd = NULL;
		HKL		hkl = NULL;
		DWORD   dwConvertMode = 0;
		if ((hIMWnd = ImmGetDefaultIMEWnd(hWndObserved)) != NULL) {
			if (SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETOPENSTATUS, NULL) != 0) {
				if ((dwConvertMode = (DWORD)SendMessage(hIMWnd, WM_IME_CONTROL, (WPARAM)IMC_GETCONVERSIONMODE, NULL)) != 0) {
					switch (dwConvertMode) {
					case IME_CMODE_NATIVE:
						dwConvertMode = IMEOFF;
						break;
					case ZENHIRA_IMEON:
					case (ZENHIRA_IMEON ^ IME_CMODE_ROMAN):
						dwConvertMode = ZENHIRA_IMEON;
						break;
					case HANEISU_IMEON:
					case (HANEISU_IMEON ^ IME_CMODE_ROMAN):
						dwConvertMode = HANEISU_IMEON;
						break;
					case HANKANA_IMEON:
					case (HANKANA_IMEON ^ IME_CMODE_ROMAN):
						dwConvertMode = HANKANA_IMEON;
						break;
					case ZENEISU_IMEON:
					case (ZENEISU_IMEON ^ IME_CMODE_ROMAN):
						dwConvertMode = ZENEISU_IMEON;
						break;
					case ZENKANA_IMEON:
					case (ZENKANA_IMEON ^ IME_CMODE_ROMAN):
						dwConvertMode = ZENKANA_IMEON;
						break;
					default:
						dwConvertMode = IMEOFF;
					}
					if ((bForceHiragana != FALSE) && (dwConvertMode != ZENHIRA_IMEON)) {
						vIMEConvertModeChangeForced(hWndObserved, ZENHIRA_IMEON);
						dwConvertMode = ZENHIRA_IMEON;
					}
					if ((hkl = hklGetInputLocale(hWndObserved)) != (HKL)0) {
						if (hkl != JP_IME) {
							dwConvertMode = IMEOFF;
						}
					}
					return dwConvertMode;
				}
			}
		}
	}
	return IMEOFF;
}

//
// vActivateIME()
//
VOID		CIME::vActivateIME(HWND hWndObserved)
{
	EnumChildWindows(hWndObserved, &bEnumChildProcActivateIME, NULL);
}

// 
// bEnumChildProcActivateIME()
// 
BOOL CALLBACK CIME::bEnumChildProcActivateIME(HWND hWnd, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	HWND	hIMWnd = ImmGetDefaultIMEWnd(hWnd);
	if (hIMWnd != NULL) {
		SendMessage(hIMWnd, WM_ACTIVATE, (WPARAM)WA_ACTIVE, (LPARAM)NULL);
		LPTSTR	lpszBuffer = new TCHAR[_MAX_PATH];
		if (lpszBuffer) {
			ZeroMemory(lpszBuffer, (sizeof(TCHAR) * _MAX_PATH));
			if (GetClassName(hIMWnd, lpszBuffer, _MAX_PATH) != 0) {
				HWND	hIMEWnd = FindWindow(lpszBuffer, NULL);
				if (hIMEWnd != NULL) {
					SendMessage(hIMEWnd, WM_ACTIVATE, (WPARAM)WA_ACTIVE, (LPARAM)NULL);
					delete[] lpszBuffer;
					return TRUE;
				}
			}
			delete[] lpszBuffer;
		}
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
	CRegistry* CReg = new CRegistry;
	if (CReg) {
		if (!CReg->bReadSystemRegValueDWORD(HKEY_LOCAL_MACHINE, SUBKEY, VALUE, &dwInputLocale)) {
		}
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

	DWORD	dwProcessID = 0;
	DWORD	dwThreadID = 0;
	HKL		hkl = NULL;
	if (!hWndObserved)	return (HKL)0;
	if ((dwThreadID = GetWindowThreadProcessId(hWndObserved, &dwProcessID)) != 0) {
		if ((hkl = GetKeyboardLayout(dwThreadID)) != NULL) {
			int	iKeyboardType = GetKeyboardType(1);
			if ((iKeyboardType != 2) || (hkl != JP_IME))	hkl = US_ENG;
			return hkl;
		}
	}
	return (HKL)0;
}


/* = EOF = */