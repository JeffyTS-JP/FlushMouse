// KeyboardHookDll32.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/04/04  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "..\FlushMouse\CommonDef.h"
#include "KeyboardHookDll32.h"

//
// Define
//

//
// Local Prototype Define
//
static LRESULT CALLBACK lpKeyboardHookLLProc(int nCode, WPARAM wParam, LPARAM lParam);
static BOOL	bKeyboardHookLLProcSub();
static BOOL	bKBisNotJP();
static BOOL	bKBisEP();

//
// Local Data
//
static BOOL		bOnlyCtrlLL = FALSE;
static DWORD	dwPreviousVKLL = 0;
static HWND		hWndKBParentLL = NULL;
static LPKEYBOARDLL_SHAREDMEM32  lpDatKeyboardLL = NULL;
static BOOL		bEnableEPHelperLL = FALSE;
static BOOL		bStartConversioningLL = FALSE;

//
// bKeyboardHookLLSet32()
//
DLLEXPORT BOOL  __stdcall bKeyboardHookLLSet32(HWND hWnd)
{
	HHOOK   hHook = NULL;
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL,						// 種類はキーボードフック
							(HOOKPROC)lpKeyboardHookLLProc,			// キーボードプロシージャをセット
							hGetInstance(), 0);						// このDLLのインスタンスハンドルを指定
	if (hHook) {
		KEYBOARDLL_SHAREDMEM32   datKeyboardLL{};
		if (bSharedMemoryCreate(hWnd, KEYBOARDHOOKLLMEM32, sizeof(KEYBOARDLL_SHAREDMEM32))) {
			datKeyboardLL.hWnd = hWnd;    datKeyboardLL.hHook = hHook;	hWndKBParentLL = hWnd;
			datKeyboardLL.bEnableEPHelper = bEnableEPHelperLL;
			if (bSharedMemoryWrite(KEYBOARDHOOKLLMEM32, (LPBYTE)&datKeyboardLL, sizeof(KEYBOARDLL_SHAREDMEM32))) {
				return TRUE;
			}
			bSharedMemoryDelete(KEYBOARDHOOKLLMEM32);
		}
		UnhookWindowsHookEx(hHook);
	}
	return FALSE;
}

//
// bKeyboardHookLLUnset32()
//
DLLEXPORT BOOL __stdcall bKeyboardHookLLUnset32()
{
	KEYBOARDLL_SHAREDMEM32   datKeyboardLL{};
	if (bSharedMemoryRead(KEYBOARDHOOKLLMEM32, (LPBYTE)&datKeyboardLL, sizeof(KEYBOARDLL_SHAREDMEM32))) {
		if (datKeyboardLL.hHook) {
			if (UnhookWindowsHookEx(datKeyboardLL.hHook)) {
				bSharedMemoryDelete(KEYBOARDHOOKLLMEM32);
				return TRUE;
			}
		}
	}
	bSharedMemoryDelete(KEYBOARDHOOKLLMEM32);
	return FALSE;
}

//
// bSetEnableEPHelperLL32()
//
DLLEXPORT BOOL __stdcall bSetEnableEPHelperLL32(BOOL bEPHelper)
{
	KEYBOARDLL_SHAREDMEM32   datKeyboardLL{};
	if (bSharedMemoryRead(KEYBOARDHOOKLLMEM32, (LPBYTE)&datKeyboardLL, sizeof(KEYBOARDLL_SHAREDMEM32))) {
		datKeyboardLL.bEnableEPHelper = bEPHelper;
		if (bSharedMemoryWrite(KEYBOARDHOOKLLMEM32, (LPBYTE)&datKeyboardLL, sizeof(KEYBOARDLL_SHAREDMEM32))) {
			return TRUE;
		}
		bSharedMemoryDelete(KEYBOARDHOOKLLMEM32);
	}
	return FALSE;
}

//
// lpKeyboardHookLLProc()
//
static LRESULT CALLBACK lpKeyboardHookLLProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) {														// nCoeが0より小さい場合は関係ないので次に渡す
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	if (nCode == HC_ACTION) {
		LPKBDLLHOOKSTRUCT	lpstKBH = (LPKBDLLHOOKSTRUCT)lParam;
		if ((lpstKBH->flags & LLKHF_UP)) {									// Key up
			dwPreviousVKLL = 0;												// Reset vkCode
			switch (lpstKBH->vkCode) {
			case VK_CONTROL:		// Ctrl   (0x11)
			case VK_LCONTROL:		// Ctrl L (0xa2)
			case VK_RCONTROL:		// Ctrl R (0xa3)
				if (bOnlyCtrlLL) {
					if (bKeyboardHookLLProcSub()) {
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_ONLY_CTRLUP, (0x80000000 | (0xff000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24))));
					}
				}
				bOnlyCtrlLL = FALSE;
				break;
			case VK_KANJI:			// Alt + 漢字 (0x19)
			case VK_OEM_3:			// JP(IME/ENG) [@] / US(ENG) IME ON (0xc0) = ['] ALT + 半角/全角 or 漢字
			case VK_OEM_8:			// JP(IME/ENG) [`] / UK(ENG) IME ON (0xdf) = ['] ALT + 半角/全角 or 漢字
			case VK_MENU:			// ALT (0x12)
			case VK_LMENU:			// ALT L (0xa4)
			case VK_RMENU:			// ALT R (0xa5)
			case VK_SHIFT:			// Shift (0x10)
			case VK_LSHIFT:			// Shift L (0xa0)
			case VK_RSHIFT:			// Shift R (0xa1)
			case VK_LWIN:			// Left  Win (0x5b)
			case VK_RWIN:			// Right Win (0x5c)
				//case VK_PAUSE:		// Pause (0x13)
				//case VK_CAPITAL:		// Caps Lock (0x14) CapsLockされてしまうので処理しない 
			case VK_KANA:			// かな (0x15)
			case VK_IME_ON:			// IME ON (0x16)
				//case VK_JUNJA:		// (0x17)
				//case VK_FINAL:		// (0x18)
			case VK_IME_OFF:		// IME OFF (0x1a)
				//case VK_ESCAPE:		// Esc (0x1b)
			case VK_CONVERT:		// 変換 (0x1c)
			case VK_NONCONVERT:		// 無変換 (0x1d)
				//case VK_ACCEPT:		// (0x1e)
				//case VK_MODECHANGE:	// (0x1f)
				//case VK_APPS:			// (0x5d)
			case VK_F6:				// (0x75)
			case VK_F7:				// (0x76)
			case VK_F8:				// (0x77)
			case VK_F9:				// (0x78)``
			case VK_F10:			// (0x79)
				//case VK_NUMLOCK:		// Num Lock (0x90)
			case VK_OEM_ATTN:		// OEM 英数/CapsLock (0xf0)
			case VK_OEM_FINISH:		// OEM カタカナ (0xf1)
			case VK_OEM_COPY:		// OEM ひらがな (0xf2)
				//case VK_OEM_AUTO:		// OEM 半角/全角 (0xf3 = VK_OEM_IME_OFF)
			case VK_OEM_IME_OFF:	// OEM IME OFF (0xf3)
			case VK_OEM_IME_ON:		// OEM IME ON  (0xf4)
			case VK_OEM_BACKTAB:	// OEM Alt+カタカナ/ひらがな (0xf5)
			case VK_OEM_PA1:		// US(ENG) Non Convert (0xeb)
				bOnlyCtrlLL = FALSE;
				if (bKeyboardHookLLProcSub()) {
					if (!bEnableEPHelperLL)	break;
					PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, (WM_USER + lpstKBH->vkCode), ((0x80000000 | 0xff000000 & (static_cast<LPARAM>(lpstKBH->flags)) << 24)));
				}
				break;
			case 0xff:				// US(ENG) Convert (0xff)
				if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
					if (bKeyboardHookLLProcSub()) {
						if (!bEnableEPHelperLL)	break;
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_OEM_FINISH, ((0x80000000 | 0xff000000 & (static_cast<LPARAM>(lpstKBH->flags)) << 24)));
					}
				}
				else if (lpstKBH->scanCode == 0x0070) {
					if (bKeyboardHookLLProcSub()) {
						if (!bEnableEPHelperLL)	break;
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_OEM_COPY, ((0x80000000 | 0xff000000 & (static_cast<LPARAM>(lpstKBH->flags)) << 24)));
					}
				}
				else if (lpstKBH->scanCode == 0x0079) {
					if (bKeyboardHookLLProcSub()) {
						if (!bEnableEPHelperLL)	break;
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_CONVERT, ((0x80000000 | 0xff000000 & (static_cast<LPARAM>(lpstKBH->flags)) << 24)));
					}
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
				bStartConversioningLL = FALSE;
				break;
			case VK_MENU:			// ALT    (0x12)
			case VK_LMENU:			// ALT L  (0xa4)
			case VK_RMENU:			// ALT R  (0xa5)
				bOnlyCtrlLL = FALSE;
				bStartConversioningLL = FALSE;
				break;
			case VK_CAPITAL:		// 英数/CapsLock (0x14)
				bOnlyCtrlLL = FALSE;
				bStartConversioningLL = FALSE;
				if (bKBisNotJP() || bKBisEP()) {
					dwPreviousVKLL = VK_OEM_ATTN;
					if (bKeyboardHookLLProcSub()) {
						if (!bEnableEPHelperLL)	break;
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_OEM_ATTN, (0x7f000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24)));
					}
					return 1;
				}
				break;
			case VK_KANJI:			// JP(IME/ENG) Alt + 漢字	(0x19)
				bOnlyCtrlLL = FALSE;
				bStartConversioningLL = FALSE;
				dwPreviousVKLL = VK_KANJI;
				if (bKeyboardHookLLProcSub()) {
					if (!bEnableEPHelperLL)	break;
					PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, KEY_KANJI, (0x7f000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24)));
				}
				break;
			case VK_OEM_3:			// JP(IME/ENG) [@] / US(ENG) IME ON (0xc0) = ['] ALT + 半角/全角 or 漢字
			case VK_OEM_8:			// JP(IME/ENG) [`] / UK(ENG) IME ON (0xdf) = ['] ALT + 半角/全角 or 漢字
				bOnlyCtrlLL = FALSE;
				bStartConversioningLL = FALSE;
				if (bKBisNotJP() || bKBisEP()) {
					if (bKeyboardHookLLProcSub()) {
						if (!bEnableEPHelperLL)	break;
						PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, (WM_USER + lpstKBH->vkCode), (0x7f000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24)));
					}
					return 1;
				}
				break;
			case VK_OEM_ATTN:		// OEM 英数/CapsLock (0xf0)
			case VK_OEM_FINISH:		// OEM カタカナ (0xf1)
			case VK_OEM_COPY:		// OEM ひらがな (0xf2)
				bOnlyCtrlLL = FALSE;
				bStartConversioningLL = FALSE;
				if (bKeyboardHookLLProcSub()) {
					if (!bEnableEPHelperLL)	break;
					PostMessage(hWndKBParentLL, WM_SYSKEYDOWNUPEX, (WM_USER + lpstKBH->vkCode), (0x7f000000 & (static_cast<LPARAM>(lpstKBH->flags) << 24)));
				}
				break;
			case VK_RETURN:			// RETURN (0x0d)
			case VK_ESCAPE:			// ESC (0x1b)
			case VK_ACCEPT:			// IME 使用可能 (0x1e)
			case VK_MODECHANGE:		// IME モード変更要求 (0x1f)
			case VK_INSERT:			// INSERT (0x2d)
			case VK_F1:				// (0x70)
			case VK_F3:				// (0x72)
			case VK_PROCESSKEY:		// IME PROCESS キー (0xe5)
				bOnlyCtrlLL = FALSE;
				bStartConversioningLL = FALSE;
				if (bKeyboardHookLLProcSub()) {
					PostMessage(hWndKBParentLL, WM_CHECKIMESTARTCONVEX, (WPARAM)bStartConversioningLL, (LPARAM)(DWORD)(WM_USER + lpstKBH->vkCode));
				}
				break;
			default:
				bOnlyCtrlLL = FALSE;
				if (bStartConversioningLL == FALSE) {
					if (((0x30 <= lpstKBH->vkCode) && (lpstKBH->vkCode <= 0x39))	// 「0」～「9」
						|| ((0x41 <= lpstKBH->vkCode) && (lpstKBH->vkCode <= 0x5A))	// 「A」～「Z」
						|| ((0x60 <= lpstKBH->vkCode) && (lpstKBH->vkCode <= 0x6f))	//  Ten Key
						|| ((0xba <= lpstKBH->vkCode) && (lpstKBH->vkCode <= 0xc0))	// 「: *」「; +」「, <」「- =」「. >」「/ ?」「@ `」
						|| ((0xdb <= lpstKBH->vkCode) && (lpstKBH->vkCode <= 0xdf))	// 「[ {」「\ |」「- =」「^ ~」
						|| ((lpstKBH->vkCode == 0xe2))) {								// 「\ _」
						bStartConversioningLL = TRUE;
						if (bKeyboardHookLLProcSub()) {
							PostMessage(hWndKBParentLL, WM_CHECKIMESTARTCONVEX, (WPARAM)bStartConversioningLL, (WM_USER + lpstKBH->vkCode));
						}
					}
					else bStartConversioningLL = FALSE;
				}
				break;
			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//
// bKBisNotJP()
//
static BOOL		bKBisNotJP()
{
#define	KB_MASK		0xffff0000
#define	KB_JP		0x04110000

	DWORD	dProcessID = 0;
	DWORD	dThreadID = GetWindowThreadProcessId(GetForegroundWindow(), &dProcessID);
	HKL	hkl = GetKeyboardLayout(dThreadID);
	if (((UINT64)hkl & KB_MASK) != KB_JP) {
		return TRUE;
	}
	return FALSE;
}

//
// bKBisEP()
//
static BOOL		bKBisEP()
{
	if (GetKeyboardType(1) == 0) {
		return TRUE;
	}
	return FALSE;
}

//
// bKeyboardHookLLProcSub
//
static BOOL	bKeyboardHookLLProcSub()
{
	if ((hWndKBParentLL == NULL) || (lpDatKeyboardLL == NULL)) {
		if (lpDatKeyboardLL == NULL) {
			if ((lpDatKeyboardLL = (LPKEYBOARDLL_SHAREDMEM32)lpvSharedMemoryOpen(KEYBOARDHOOKLLMEM32, sizeof(KEYBOARDLL_SHAREDMEM32))) == NULL) {
				return	FALSE;
			}
		}
		if (lpDatKeyboardLL->hWnd == NULL) {
			return	FALSE;
		}
		hWndKBParentLL = lpDatKeyboardLL->hWnd;
		bEnableEPHelperLL = lpDatKeyboardLL->bEnableEPHelper;
	}
	return TRUE;
}

/* = EOF = */