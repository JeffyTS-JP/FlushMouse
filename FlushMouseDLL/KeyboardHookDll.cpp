//
// KeyboardHookDll.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/02/12  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "KeyboardHookDll.h"
#include "..\FlushMouseLIB\CommonDef.h"
#include "..\MiscLIB\CRegistry.h"

//
// Define
//


//
// Local Data
//
#pragma comment(linker, "/SECTION:FLUSHMOUSEDLL_SEG,RWS")
#pragma data_seg("FLUSHMOUSEDLL_SEG")
static HHOOK	hHookLL = NULL;
static BOOL		bOnlyCtrlLL = FALSE;
static DWORD	dwPreviousVKLL = 0;
static HWND		hWndKBParentLL = NULL;
static BOOL		bEnableEPHelperLL = FALSE;
static BOOL		bIMEModeForcedLL = FALSE;
static BOOL		bStartConvertingLL = FALSE;
#pragma data_seg()

//
// Local Prototype Define
//
static LRESULT CALLBACK lpKeyboardHookLLProc(int nCode, WPARAM wParam, LPARAM lParam);
static BOOL	bKBisEP();

//
// bKeyboardHookLLSet()
//
DLLEXPORT BOOL  __stdcall bKeyboardHookLLSet(HWND hWnd)
{
	if (!hWnd || hWndKBParentLL)	return FALSE;
	hWndKBParentLL = hWnd;
	if (!hHookLL)	hHookLL = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)lpKeyboardHookLLProc, hGetInstance(), 0);
	if (!hHookLL) {
		bOnlyCtrlLL = FALSE;
		dwPreviousVKLL = 0;
		hWndKBParentLL = NULL;
		bEnableEPHelperLL = FALSE;
		bIMEModeForcedLL = FALSE;
		bStartConvertingLL = FALSE;
		return FALSE;
	}
	return TRUE;
}

//
// vKeyboardHookLLUnset()
//
DLLEXPORT VOID __stdcall vKeyboardHookLLUnset()
{
	if (hHookLL)	UnhookWindowsHookEx(hHookLL);
	hHookLL = NULL;
	bOnlyCtrlLL = FALSE;
	dwPreviousVKLL = 0;
	hWndKBParentLL = NULL;
	bEnableEPHelperLL = FALSE;
	bIMEModeForcedLL = FALSE;
	bStartConvertingLL = FALSE;
}

//
// bSetEnableEPHelperLL64()
//
DLLEXPORT BOOL __stdcall bSetEnableEPHelperLL64(BOOL bEPHelper)
{
	bEnableEPHelperLL = bEPHelper;
	return TRUE;
}

//
// bSetEnableIMEModeForcedLL64()
//
DLLEXPORT BOOL __stdcall bSetEnableIMEModeForcedLL64(BOOL bIMEModeForced)
{
	bIMEModeForcedLL = bIMEModeForced;
	return TRUE;
}

//
// lpKeyboardHookLLProc()
//
static LRESULT CALLBACK lpKeyboardHookLLProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	if (nCode == HC_ACTION) {
		LPKBDLLHOOKSTRUCT	lpstKBH = (LPKBDLLHOOKSTRUCT)lParam;
		if ((lpstKBH->flags & LLKHF_UP)) {									// Key up
			dwPreviousVKLL = 0;
			switch (lpstKBH->vkCode) {
				case VK_CONTROL:		// Ctrl   (0x11)
				case VK_LCONTROL:		// Ctrl L (0xa2)
				case VK_RCONTROL:		// Ctrl R (0xa3)
					if (bOnlyCtrlLL) {
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_ONLY_CTRLUP, (0x80000000 | (0xff000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24))));
					}
					bOnlyCtrlLL = FALSE;
					break;
				case VK_RETURN:			// Enter (0x0d)
					bOnlyCtrlLL = FALSE;
					if (bStartConvertingLL) {
						bStartConvertingLL = FALSE;
						PostMessage(hWndKBParentLL, WM_CHECKIMESTARTCONVEX, (WPARAM)bStartConvertingLL, (LPARAM)(DWORD)(WM_USER + lpstKBH->vkCode));
					}
					else {
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_RETURN, (0x80000000 | (0xff000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24))));
					}
					break;
				case VK_TAB:			// Tab (0x09)
				case VK_KANJI:			// Alt + 漢字 (0x19)
				case VK_OEM_3:			// @@@ JP(IME/ENG) [@] / US(ENG) IME ON (0xc0) = ['] ALT + 半角/全角 or 漢字
				case VK_OEM_8:			// @@@ JP(IME/ENG) [`] / British(ENG) IME ON (0xdf) = ['] ALT + 半角/全角 or 漢字
				case VK_MENU:			// ALT (0x12)
				case VK_LMENU:			// ALT L (0xa4)
				case VK_RMENU:			// ALT R (0xa5)
				case VK_SHIFT:			// Shift (0x10)
				case VK_LSHIFT:			// Shift L (0xa0)
				case VK_RSHIFT:			// Shift R (0xa1)
				case VK_LWIN:			// Left  Win (0x5b)
				case VK_RWIN:			// Right Win (0x5c)
				//case VK_PAUSE:		// Pause (0x13)
				//case VK_CAPITAL:		// Caps Lock (0x14) 
				case VK_KANA:			// かな (0x15)
				case VK_IME_ON:			// IME ON (0x16)
				//case VK_JUNJA:		// (0x17)
				//case VK_FINAL:		// (0x18)
				case VK_IME_OFF:		// IME OFF (0x1a)
				//case VK_ESCAPE:		// Esc (0x1b)
				//case VK_CONVERT:		// 変換 (0x1c)
				//case VK_NONCONVERT:	// 無変換 (0x1d)
				//case VK_ACCEPT:		// (0x1e)
				//case VK_MODECHANGE:	// (0x1f)
				//case VK_APPS:			// (0x5d)
				case VK_F5:				// (0x74)
				case VK_F6:				// (0x75)
				case VK_F7:				// (0x76)
				case VK_F8:				// (0x77)
				case VK_F9:				// (0x78)
				case VK_F10:			// (0x79)
				//case VK_NUMLOCK:		// Num Lock (0x90)
				//case VK_OEM_AUTO:		// OEM 半角/全角 (0xf3 = VK_OEM_IME_OFF)
				case VK_OEM_IME_OFF:	// OEM IME OFF (0xf3)
				case VK_OEM_IME_ON:		// OEM IME ON  (0xf4)
				case VK_OEM_BACKTAB:	// OEM Alt+カタカナ/ひらがな (0xf5)
				case VK_OEM_PA1:		// US(ENG) Non Convert (0xeb)
					bOnlyCtrlLL = FALSE;
					PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, (WM_USER + lpstKBH->vkCode), ((0x80000000 | 0xff000000 & (static_cast<LPARAM>(lpstKBH->flags)) << 24)));
					break;
				case VK_OEM_ATTN:		// OEM 英数/CapsLock (0xf0)
				case VK_OEM_FINISH:		// OEM カタカナ (0xf1)
				case VK_OEM_COPY:		// OEM ひらがな (0xf2)
					bOnlyCtrlLL = FALSE;
					break;
				case VK_FF:				// US(ENG) Convert
					bOnlyCtrlLL = FALSE;
					if (!bEnableEPHelperLL)	break;
					if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, (KEY_OEM_FINISH), ((0x80000000 | 0xff000000 & (static_cast<LPARAM>(lpstKBH->flags)) << 24)));
					}
					else if (lpstKBH->scanCode == 0x70) {
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_OEM_COPY, ((0x80000000 | 0xff000000 & (static_cast<LPARAM>(lpstKBH->flags)) << 24)));
					}
					else if (lpstKBH->scanCode == 0x79) {
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_CONVERT, ((0x80000000 | 0xff000000 & (static_cast<LPARAM>(lpstKBH->flags)) << 24)));
					}
					break;
				default:
					bOnlyCtrlLL = FALSE;
					break;
			}
		}
		else {																// Key down
			if (dwPreviousVKLL == lpstKBH->vkCode) {
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			dwPreviousVKLL = lpstKBH->vkCode;
			switch (lpstKBH->vkCode) {
				case VK_CONTROL:		// Ctrl   (0x11)
				case VK_LCONTROL:		// Ctrl L (0xa2)
				case VK_RCONTROL:		// Ctrl R (0xa3)
					bOnlyCtrlLL = TRUE;
					bStartConvertingLL = FALSE;
					break;
				case VK_MENU:			// ALT    (0x12)
				case VK_LMENU:			// ALT L  (0xa4)
				case VK_RMENU:			// ALT R  (0xa5)
					bOnlyCtrlLL = FALSE;
					bStartConvertingLL = FALSE;
					break;
				case VK_CAPITAL:		// 英数/CapsLock (0x14)
					bOnlyCtrlLL = FALSE;
					bStartConvertingLL = FALSE;
					dwPreviousVKLL = 0;
					if (!bEnableEPHelperLL)	break;
					if (bKBisEP()) {
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_OEM_ATTN, (0x7f000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24)));
						return 1;
					}
					break;
				case VK_KANJI:			// JP(IME/ENG) Alt + 漢字	(0x19)
					bOnlyCtrlLL = FALSE;
					bStartConvertingLL = FALSE;
					dwPreviousVKLL = 0;
					if (GetAsyncKeyState(VK_MENU) & 0x8000) {
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_KANJI, (0x7f000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24)));
					}
					break;
				case VK_CONVERT:		// 変換 (0x1c)
				case VK_NONCONVERT:		// 無変換 (0x1d)
					bOnlyCtrlLL = FALSE;
					bStartConvertingLL = FALSE;
					dwPreviousVKLL = 0;
					PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, (WM_USER + lpstKBH->vkCode), (0x7f000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24)));
					if ((lpstKBH->vkCode == VK_NONCONVERT) && bIMEModeForcedLL)	return 1;
					break;
				case VK_OEM_3:			// JP(IME/ENG) [@] / US(ENG) IME ON (0xc0) = ['] ALT + 半角/全角 or 漢字
				case VK_OEM_8:			// JP(IME/ENG) [`] / British(ENG) IME ON (0xdf) = ['] ALT + 半角/全角 or 漢字
					bOnlyCtrlLL = FALSE;
					bStartConvertingLL = FALSE;
					if (!bEnableEPHelperLL)	break;
					if (bKBisEP()) {
						PostMessage(hWndKBParentLL, WM_CHECKIMESTARTCONVEX, (WPARAM)bStartConvertingLL, (WM_USER + lpstKBH->vkCode));
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, (WM_USER + lpstKBH->vkCode), (0x7f000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24)));
						return 1;
					}
					break;
				case VK_OEM_ATTN:		// OEM 英数/CapsLock (0xf0)
					bOnlyCtrlLL = FALSE;
					bStartConvertingLL = FALSE;
					dwPreviousVKLL = 0;
					if (!bStartConvertingLL) {
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, (WM_USER + lpstKBH->vkCode), (0x7f000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24)));
					}
					break;
				case VK_OEM_FINISH:		// OEM カタカナ (0xf1)
				case VK_OEM_COPY:		// OEM ひらがな (0xf2)
					bOnlyCtrlLL = FALSE;
					bStartConvertingLL = FALSE;
					dwPreviousVKLL = 0;
					if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, (KEY_OEM_FINISH), (0x7f000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24)));
					}
					else {
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, (KEY_OEM_COPY), (0x7f000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24)));
					}
					break;
				case VK_ESCAPE:			// ESC (0x1b)
				case VK_INSERT:			// INSERT (0x2d)
					bOnlyCtrlLL = FALSE;
					bStartConvertingLL = FALSE;
					PostMessage(hWndKBParentLL, WM_CHECKIMESTARTCONVEX, (WPARAM)bStartConvertingLL, (LPARAM)(DWORD)(WM_USER + lpstKBH->vkCode));
					break;
				default:
					bOnlyCtrlLL = FALSE;
					if (bStartConvertingLL == FALSE) {
						if (((0x30 <= lpstKBH->vkCode) && (lpstKBH->vkCode <= 0x39))	// 「0」～「9」
						 || ((0x41 <= lpstKBH->vkCode) && (lpstKBH->vkCode <= 0x5A))	// 「A」～「Z」
						 || ((0x60 <= lpstKBH->vkCode) && (lpstKBH->vkCode <= 0x6f))	//  Ten Key
						 || ((0xba <= lpstKBH->vkCode) && (lpstKBH->vkCode <= 0xc0))	// 「: *」「; +」「, <」「- =」「. >」「/ ?」「@ `」
						 || ((0xdb <= lpstKBH->vkCode) && (lpstKBH->vkCode <= 0xde))	// 「[ {」「\ |」「- =」「^ ~」
						 || ((lpstKBH->vkCode == 0xe2))) {								// 「\ _」
							bStartConvertingLL = TRUE;
							PostMessage(hWndKBParentLL, WM_CHECKIMESTARTCONVEX, (WPARAM)bStartConvertingLL, (WM_USER + lpstKBH->vkCode));
						}
						else bStartConvertingLL = FALSE;
					}
					break;
			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//
// bKBisEP()
//
static BOOL		bKBisEP()
{
	if (GetKeyboardType(1) == 0) {
		_Post_equals_last_error_ DWORD err = GetLastError();
		if (err != 0) {
		}
		else {
			return TRUE;
		}
	}

	DWORD	dProcessID = 0;
	DWORD	dThreadID = GetWindowThreadProcessId(GetForegroundWindow(), &dProcessID);
	if (dThreadID != 0) {
		HKL	hkl = GetKeyboardLayout(dThreadID);
		if (hkl == US_ENG) {
			return TRUE;
		}
	}

#define SUBKEY	_T("SOFTWARE\\Microsoft\\Input\\Locales")
#define VALUE	_T("InputLocale")
	DWORD	dwInputLocale = 0;
	CRegistry* CReg = new CRegistry;
	if (CReg) {
		if (!CReg->bReadSystemRegValueDWORD(HKEY_LOCAL_MACHINE, SUBKEY, VALUE, &dwInputLocale)) {
		}
		delete	CReg;
	}
	if ((dwInputLocale & 0x0000ffff) == LANG_IME)	return FALSE;
	return TRUE;
#undef SUBKEY
#undef VALUE
}


/* = EOF = */