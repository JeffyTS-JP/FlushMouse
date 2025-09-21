//
// CommonDef.h
//		Copyright (C) 1993 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2023/04/27  JeffyTS		New edit
//

//
// Include
//
#pragma once
#include "pch.h"

//
// Define
//
#define MAX_LOADSTRING 100

#define FLUSHMOUSE					L"FlushMouse"
#define FLUSHMOUSE32				L"FlushMouse32"
#define FLUSHMOUSESETTINGS			L"FlushMouseSettings"

// Files
#define	FULL_FLUSHMOUSE_EXE			L"%ProgramFiles%\\JeffyTS\\FlushMouse\\FlushMouse.exe"
#define	FULL_FLUSHMOUSE32_EXE		L"%ProgramFiles%\\JeffyTS\\FlushMouse\\FlushMouse32.exe"
#define FULL_FLUSHMOUSESETTINGS_EXE	L"%ProgramFiles%\\JeffyTS\\FlushMouse\\FlushMouseSettings\\FlushMouseSettings.exe"
#define RELATIVE_FLUSHMOUSESETTINGS_EXE	L"FlushMouseSettings\\FlushMouseSettings.exe"
#define FULL_FLUSHMOUSE_DLL			L"%ProgramFiles%\\JeffyTS\\FlushMouse\\FlushMouseDLL.dll"
#define	FULL_FLUSHMOUSE32_DLL		L"%ProgramFiles%\\JeffyTS\\FlushMouse\\FlushMouseDLL32.dll"
#define FULL_FLUSHMOUSEUI3_DLL		L"%ProgramFiles%\\JeffyTS\\FlushMouse\\FlushMouseUI3DLL.dll"
#define RELATIVE_FLUSHMOUSEUI3DLL_DLL	L"FlushMouseSettings\\FlushMouseUI3DLL.dll"
#define	FLUSHMOUSE_EXE				L"FlushMouse.exe"
#define	FLUSHMOUSE32_EXE			L"FlushMouse32.exe"
#define	FLUSHMOUSEUI3_EXE			L"FlushMouseUI3.exe"
#define	FLUSHMOUSESETTINGS_EXE		L"FlushMouseSettings.exe"
#define FLUSHMOUSE_DLL				L"FlushMouseDLL.dll"
#define FLUSHMOUSE32_DLL			L"FlushMouseDLL32.dll"
#define FLUSHMOUSEUI3_DLL			L"FlushMouseUI3DLL.dll"
#define FLUSHMOUSECURSOR_DAT		L"FlushMouseCursor.dat"

// Window Class
#define	CLASS_FLUSHMOUSE			L"FlushMouse-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}"
#define CLASS_FLUSHMOUSE32			L"FlushMouse32-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}"
#define	CLASS_FLUSHMOUSESETTINGS	L"FlushMouseSettings-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}"

// for IME
#define IMC_GETCONVERSIONMODE   0x0001
#define IMC_SETCONVERSIONMODE   0x0002
#define IMC_GETSENTENCEMODE		0x0003
#define IMC_SETSENTENCEMODE		0x0004
#define IMC_GETOPENSTATUS		0x0005
#define IMC_SETOPENSTATUS		0x0006

// for KB
#define	KB_MASK		0xffff0000
#define	KB_JP		0x04110000
#define	KB_US		0x04090000
#define	LANG_MASK	0x0000ffff
#define LANG_IME	0x00000411
#define	LANG_ENG	0x00000409
#define JP_IME		(HKL)(KB_JP | LANG_IME)
#define JP_ENG		(HKL)(KB_JP | LANG_ENG)
#define US_IME		(HKL)(KB_US | LANG_IME)
#define US_ENG		(HKL)(KB_US | LANG_ENG)

// for Shell_NotifyIcon
#define	NOTIFYICONDATA_ID				0x684B9604

typedef struct tagPOINTD
{
	double	x;
	double	y;
} POINTD, * PPOINTD, * LPPOINTD;

typedef struct tagRECTF {
	float    left;
	float    top;
	float    right;
	float    bottom;
} RECTF, * PRECTF, * LPRECTF;

typedef struct tagRECTD {
	double    left;
	double    top;
	double    right;
	double    bottom;
} RECTD, * PRECTD, * LPRECTD;

typedef struct tagSIZEF
{
	float  cx;
	float  cy;
} SIZEF, * PSIZEF, * LPSIZEF;

typedef struct tagSIZED
{
	double	cx;
	double	cy;
} SIZED, * PSIZED, * LPSIZED;

//
// Message Handler Define
// 

// void Cls_OnTaskTrayEx(HWND hWnd, UINT id, UINT uMsg);
#define WM_TASKTRAYEX						(WM_USER + 0x0001)
#define HANDLE_WM_TASKTRAYEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), (UINT)(wParam), (UINT)(lParam)), 0L)

// void Cls_OnEventForegroundEx(HWND hWnd, DWORD dwEvent, HWND hForeWnd);
#define WM_EVENT_SYSTEM_FOREGROUNDEX		(WM_USER + 0x0002)
#define HANDLE_WM_EVENT_SYSTEM_FOREGROUNDEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), (DWORD)(wParam), (HWND)(lParam)), 0L)

// void	Cls_OnCheckIMEStartConvertingEx(HWND hWnd, BOOL bStartConverting, DWORD vkCode);
#define WM_CHECKIMESTARTCONVEX				(WM_USER + 0x0004)
#define	HANDLE_WM_CHECKIMESTARTCONVEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), (BOOL)(wParam), (DWORD)(lParam)), 0L)

// void Cls_OnLButtonDownEx(HWND hWnd, int x, int y, HWND hForeground)
#define WM_LBUTTONDOWNEX					(WM_USER + WM_LBUTTONDOWN)
#define HANDLE_WM_LBUTTONDOWNEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)

// void Cls_OnRButtonDownEx(HWND hWnd, int x, int y, HWND hForeground)
#define WM_RBUTTONDOWNEX					(WM_USER + WM_RBUTTONDOWN)
#define HANDLE_WM_RBUTTONDOWNEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)

// void Cls_OnLButtonUpEx(HWND hWnd, int x, int y, HWND hForeground)
#define WM_LBUTTONUPEX						(WM_USER + WM_LBUTTONUP)
#define HANDLE_WM_LBUTTONUPEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)

// void Cls_OnRButtonUpEx(HWND hWnd, int x, int y, HWND hForeground)
#define WM_RBUTTONUPEX						(WM_USER + WM_RBUTTONUP)
#define HANDLE_WM_RBUTTONUPEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)

// void Cls_OnLButtonDblClkEx(HWND hWnd, int x, int y, HWND hForeground)
#define WM_LBUTTONDBLCLKEX					(WM_USER + WM_LBUTTONDBLCLK)
#define HANDLE_WM_LBUTTONDBLCLKEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)

// void Cls_OnRButtonDblClkEx(HWND hWnd, int x, int y, HWND hForeground)
#define WM_RBUTTONDBLCLKEX					(WM_USER + WM_RBUTTONDBLCLK)
#define HANDLE_WM_RBUTTONDBLCLKEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)(wParam)), 0L)

// void Cls_OnSysKeyDownUpEx(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
#define WM_SYSKEYDOWNUPEX					(WM_USER + WM_SYSKEYDOWN)
#define HANDLE_WM_SYSKEYDOWNUPEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), (UINT)(wParam), (!(BOOL)(lParam & 0x80000000)), (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L)

// void Cls_OnCreateEx(HWND hWnd, WPARAM wParam, LPCREATESTRUCT lParam)
#define WM_CREATEEX							(WM_USER + WM_CREATE)
#define HANDLE_WM_CREATEEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), (WPARAM)(wParam), (LPCREATESTRUCT )(lParam)), 0L)

// void Cls_OnInputLangChangeEx(HWND hWnd, UINT CodePage, HKL hkl);
#define WM_INPUTLANGCHANGEEX				(WM_USER + WM_INPUTLANGCHANGE)
#define HANDLE_WM_INPUTLANGCHANGEEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), (UINT)(wParam), (HKL)(lParam)), 0L)

//void Cls_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
#define	WM_MOUSEWHEELEX						(WM_USER + WM_MOUSEWHEEL)
#define HANDLE_WM_MOUSEWHEELEX(hwnd, wParam, lParam, fn) ((fn)((hwnd), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam)), 0L)

//void Cls_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
#define	WM_MOUSEHWHEELEX					(WM_USER + WM_MOUSEHWHEEL)
#define HANDLE_WM_MOUSEHWHEELEX(hwnd, wParam, lParam, fn) ((fn)((hwnd), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam)), 0L)

// BOOL Cls_OnSettings(HWND hWnd, int iCode, LPARAM lParam);
#define WM_SETTINGSEX						(WM_USER + 0xfe)
#define HANDLE_WM_SETTINGSEX(hWnd, wParam, lParam, fn) ((LRESULT)(BOOL)(fn)((hWnd), (int)(wParam), (int)(lParam)))
#define SETTINGSEX_OK								0
#define SETTINGSEX_CANCEL							1
#define SETTINGSEX_APPLY							2
#define SETTINGSEX_RELOAD_REGISTRY					3
#define SETTINGSEX_RELOAD_MOUSECURSOR				4
#define SETTINGSEX_SETTINGS_CHANGE_PANE				10
#define SETTINGSEX_SETTINGS_STARTED					11
#define SETTINGSEX_SETTINGS_SETREGISTRY				12
#define SETTINGSEX_SETTINGS_GENERAL_SETREGISTRY		13
#define SETTINGSEX_SETTINGS_IMEMODE_SETREGISTRY		14
#define SETTINGSEX_SETTINGS_CLOSE					15 
#define SETTINGSEX_SYNTP_START						20
#define SETTINGSEX_SYNTP_IS_STARTED					21
#define SETTINGSEX_SYNTP_STOP						22
#define SETTINGSEX_SYNTP_SETREGISRY					23

#define SETTINGSEX_SELECTEDPANE_GENERAL				1
#define SETTINGSEX_SELECTEDPANE_IMEMODE				2
#define SETTINGSEX_SELECTEDPANE_SYNTPHELPER			3
#define SETTINGSEX_SELECTEDPANE_ABOUT				4

#define KEY_TAB				(WM_USER + VK_TAB)						// Tab    (0x09)
#define KEY_RETURN			(WM_USER + VK_RETURN)					// Enter  (0x0d)
#define KEY_ESCAPE			(WM_USER + VK_ESCAPE)					// Esc    (0x1b)
#define KEY_ACCEPT			(WM_USER + VK_ACCEPT)					// IME 使用可能 (0x1e)
#define KEY_MODECHANGE		(WM_USER + VK_MODECHANGE)				// IME モード変更要求 (0x1f)
#define KEY_CTRL			(WM_USER + VK_CONTROL)					// Ctrl   (0x11)
#define KEY_LCTRL			(WM_USER + VK_LCONTROL)					// Ctrl L (0xa2)
#define KEY_RCTRL			(WM_USER + VK_RCONTROL)					// Ctrl R (0xa3)
#define	KEY_ALT				(WM_USER + VK_MENU)						// ALT    (0x12)
#define	KEY_LALT			(WM_USER + VK_LMENU)					// ALT L  (0xa4)
#define	KEY_RALT			(WM_USER + VK_RMENU)					// ALT R  (0xa5)
#define KEY_SHIFT			(WM_USER + VK_SHIFT)					// Shift  (0x10)
#define KEY_LSHIFT			(WM_USER + VK_LSHIFT)					// Shift L(0xa0)
#define KEY_RSHIFT			(WM_USER + VK_RSHIFT)					// Shift R(0xa1)
#define KEY_LWIN			(WM_USER + VK_LWIN)						// L Win  (0x5b)
#define KEY_RWIN			(WM_USER + VK_RWIN)						// R Win  (0x5c)
#define KEY_CAPITAL			(WM_USER + VK_CAPITAL)					// Caps Lock (0x14)
#define KEY_KANA			(WM_USER + VK_KANA)						// カナ   (0x15)
#define KEY_IME_ON			(WM_USER + VK_IME_ON)					// IME ON (0x16)
#define KEY_KANJI			(WM_USER + VK_KANJI)					// 漢字   (0x19)
#define KEY_IME_OFF			(WM_USER + VK_IME_OFF)					// IME OFF(0x1a)
#define KEY_CONVERT			(WM_USER + VK_CONVERT)					// IME convert    (0x1c)
#define KEY_NONCONVERT		(WM_USER + VK_NONCONVERT)				// IME nonconvert (0x1d)
#define KEY_F5				(WM_USER + VK_F5)						// F5  (0x74) 
#define KEY_F6				(WM_USER + VK_F6)						// F6  (0x75) 
#define KEY_F7				(WM_USER + VK_F7)						// F7  (0x76)
#define KEY_F8				(WM_USER + VK_F8)						// F8  (0x77)
#define KEY_F9				(WM_USER + VK_F9)						// F9  (0x78)
#define KEY_F10				(WM_USER + VK_F10)						// F10 (0x79)
#define KEY_OEM_3			(WM_USER + VK_OEM_3)					// OEM US(ENG) IME ON (0xc0) [`] for US
#define KEY_OEM_8			(WM_USER + VK_OEM_8)					// OEM UK(ENG) IME ON (0xc0) [`] for UK
#define KEY_OEM_ATTN		(WM_USER + VK_OEM_ATTN)					// 英数/CapsLock (0xf0)
#define KEY_OEM_FINISH		(WM_USER + VK_OEM_FINISH)				// OEM カタカナ (0xf1)
#define KEY_OEM_COPY		(WM_USER + VK_OEM_COPY)					// OEM ひらがな (0xf2)
#define VK_OEM_IME_OFF		0xf3									// Unassigned VK_CODE
#define KEY_OEM_IME_OFF		(WM_USER + VK_OEM_IME_OFF)				// OEM IME OFF (0xf3) But It's IME ON when key up
#define VK_OEM_IME_ON		0xf4									// Unassigned VK_CODE
#define KEY_OEM_IME_ON		(WM_USER + VK_OEM_IME_ON)				// OEM IME ON  (0xf4) But It's IME OFF when key up
#define KEY_OEM_BACKTAB		(WM_USER + VK_OEM_BACKTAB)				// OEM Alt+カタカナ/ひらがな (0xf5)
#define VK_ONLY_CTRLUP		0xe8									// Unassigned VK_CODE
#define KEY_ONLY_CTRLUP		(WM_USER + VK_ONLY_CTRLUP)				// Only Ctrl key up
#define KEY_OEM_PA1			(WM_USER + VK_OEM_PA1)					// US(ENG) KB 無変換	 (0xeb)
#define VK_FF				0xff									// Unassigned VK_CODE
#define KEY_FF				(WM_USER + VK_FF)						// US(ENG) KB 変換/ひら/カタ	(0xff)

/* = EOF = */