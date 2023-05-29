// Cursor.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/12  JeffyTS  	New edit.

//
// Include
//
#pragma once
#include "CommonDef.h"
#include "..\FlushMouseCursor\Resource.h"
#include "..\MiscLIB\CThread.h"

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
#define IMEHIDE			(0x88)																			// 0x88 :■  Hide    1001 1000

#ifndef OCR_NORMAL
#define OCR_NORMAL			32512		// IDC_ARROW	 (WinUser.h)
#endif // !OCR_NORMAL
#ifndef OCR_IBEAM
#define OCR_IBEAM			32513		// IDC_IBEAM	 (WinUser.h)
#endif // !OCR_IBEAM
#define OCR_WAIT            32514
#define OCR_CROSS           32515
#define OCR_UP              32516
#define OCR_SIZE            32640		/* OBSOLETE: use OCR_SIZEALL */
#define OCR_ICON            32641		/* OBSOLETE: use OCR_NORMAL */
#define OCR_SIZENWSE        32642
#define OCR_SIZENESW        32643
#define OCR_SIZEWE          32644
#define OCR_SIZENS          32645
#define OCR_SIZEALL         32646
#define OCR_ICOCUR          32647		/* OBSOLETE: use OIC_WINLOGO */
#define OCR_NO              32648
#if(WINVER >= 0x0500)
#ifndef OCR_HAND
#define OCR_HAND			32649		// IDC_HAND (WinUser.h)
#endif // !OCR_HAND
#endif /* WINVER >= 0x0500 */
#ifndef OCR_HELP
#if(WINVER >= 0x0400)
#define OCR_APPSTARTING     32650
#endif /* WINVER >= 0x0400 */
#define	OCR_HELP			32651		// IDC_HELP (WinUser.h)
#endif // !OCR_HELP
#ifndef OCR_PIN
#define OCR_PIN				32671		// IDC_PIN (WinUser.h)
#endif // !OCR_PIN
#ifndef OCR_PERSON
#define	OCR_PERSON			32672		// IDC_PERSON (WinUser.h)	
#endif // !OCR_PERSON
#define	OCR_HIDE			32896		// Hide Cursor

//
// Struct Define
//
typedef struct tagMOUSECURSOR {
	DWORD		id;								// OCR_xxx
	UINT		uResourceID;					// Resource ID
	BOOL		bReadReg;						// TRUE = Read from Registry
	TCHAR		szRegValue[MAX_LOADSTRING];		// Registry Value
	TCHAR		szFile[_MAX_PATH];				// Cursor File Name
} MOUSECURSOR, * PMOUSECURSOR, * LPMOUSECURSOR;

typedef struct tagFLUSHMOUSECURSOR {
	UINT		dwIMEMode;						// IME Mode ID
	TCHAR		szMode[IMEMODECHAR];			// Mode Display Char
	MOUSECURSOR	stArrow;						// Arrow MOUSECURSOR
	MOUSECURSOR	stHand;							// Hand MOUSECURSOR
	MOUSECURSOR	stIBeam;						// IBeam MOUSECURSOR
} FLUSHMOUSECURSOR, * PFLUSHMOUSECURSOR, * LPFLUSHMOUSECURSOR;

typedef struct tagIMECursorData
{
	// Use in Cursor
	HWND		hWndObserved;
	DWORD		dwIMEModeCursor;
	BOOL		bDrawIMEModeWait;
	LPCTSTR		lpszLoadDatName;
	LPFLUSHMOUSECURSOR	lpstFlushMouseCursor;

	// Use in Cursor from Registry
	int			iCursorSize;
	int			iModeSize;
	DWORD		dwInThreadSleepTime;
	DWORD		dwWaitWaveTime;
	DWORD		dwDisplayModeTime;
	BOOL		bDisplayIMEModeOnCursor;
	BOOL		bForceHiragana;
	BOOL		bDrawNearCaret;
	COLORREF	dwNearDrawCaretColor;
	COLORREF	dwNearDrawMouseColor;

	BOOL		bDenyChangedByApp;
	BOOL		bUseBigArrow;
	BOOL		bDisplayFocusWindowIME;
} IMECURSORDATA, * PIMECURSORDATA, * LPIMECURSORDATA;

typedef struct tagVirtualDesktop
{
	RECT	rcMonitorSize;
	int		iNumOfMonitors;
} VIRTUALDESKTOP, * PVIRTUALDESKTOP, * LPVIRTUALDESKTOP;

//
// Class CCursorWindow
//
class CCursorWindow
{
public:
	CCursorWindow();	
	~CCursorWindow();

public:
	BOOL			bRegister(HINSTANCE hInstance, LPCTSTR szWindowClass, COLORREF dwRGB);
	BOOL			bSetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
	VOID			vSetModeString(LPTSTR lpszIMEMode);

private:
	ATOM			MyRegisterClass(HINSTANCE hInstance);
	HWND			InitInstance(HINSTANCE hInstance, int nCmdShow);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL			Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
	void			Cls_OnDestroy(HWND hWnd);
	void			Cls_OnPaint(HWND hWnd);

public:

private:
	LPTSTR			lpszWindowClass;
	HWND			hCursorWindow;
	LPTSTR			lpszMode;
	COLORREF		dwTextColor;
	COLORREF		dwBackColor;

};

//
// Class CIME
//
class CIME
{
public:
	CIME();
	~CIME();

public:
	BOOL			bIsIMEOpen(HWND hWndObserved);
	VOID			vIMEOpenCloseForced(HWND hWndObserved, DWORD dwIMEOpenClose);
	VOID			vIMEConvertModeChangeForced(HWND hWndObserved, DWORD dwConvertMode);
	DWORD			dwIMEMode(HWND hWndObserved, BOOL bForceHiragana);
	VOID			vActivateIME(HWND hWndObserved);
	BOOL			bMoveIMEToolbar();
	BOOL			bGetVirtualDesktopSize();
	BOOL			bIsNewIME();
	DWORD			dwGetInputLocale();
	HKL				hklGetInputLocale(HWND hWndObserved);

private:
	BOOL			bExistIMEToolbar();
	static BOOL 	bGetVirtualDesktopSizeEnumProc(HMONITOR hMonitor, HDC hDC, LPCRECT lprcClip, LPARAM lParam);

	static BOOL CALLBACK bEnumChildProcIMEOpenClose(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK bEnumChildProcIMEConvertMode(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK bEnumChildProcActivateIME(HWND hWnd, LPARAM lParam);

private:
	LPVIRTUALDESKTOP	lpstVirtualDesktop;
};

//
// Class CCursor
// 
class CCursor
{
public:
	CCursor();
	~CCursor();

public:
	BOOL			bInitialize(HWND hWnd);
	BOOL			bStartIMECursorChangeThread(HWND hWndObserved);
	BOOL			bStartDrawIMEModeThread(HWND hWndObserved);
	BOOL			bStartDrawIMEModeThreadWait(HWND hWndObserved);
	BOOL			bStartDrawIMEModeThreadWaitDblClk(HWND hWndObserved);
	BOOL			bGetCaretPos(HWND hWnd, LPPOINT lpCaretPos);
	BOOL			bShowHideCursor(HWND hWndObserved, BOOL bShow);

private:
	LPTSTR			lpszGetCursorDataName();
	HMODULE			hCursorDllLoad();
	BOOL			bCursorDllUnload();
	BOOL			bSystemCursorLoad();

	BOOL			bRegisterIMECursorChangeThread(HWND hWndObserved);
	BOOL			bIsIMECursorChanged(LPIMECURSORDATA lpstCursorData);
	static BOOL WINAPI	bIMECursorChangeRoutine(LPVOID lpvParam);

	BOOL			bRegisterDrawIMEModeThread(HWND hWndObserved);
	BOOL			bDrawIMEModeOnDisplay(LPIMECURSORDATA lpstCursorData);
	BOOL			bCalcDispModeRect(int iModeSizeX, int iModeSizeY, LPRECT lpRect);
	BOOL			bCalcDispModeCaretRect(HWND hWnd, int iModeSizeX, int iModeSizeY, LPRECT lpRect, LPPOINT lpPt);
	BOOL			bDrawIMEModeOnDisplaySub(LPIMECURSORDATA lpstCursorData);
	static BOOL		bIconDrawEnumProc(HMONITOR hMonitor, HDC hDC, LPCRECT lprcClip, LPARAM lParam);
	static BOOL WINAPI	bDrawIMEModeRoutine(LPVOID lpvParam);

	BOOL			bGetMouseRegValue(LPCTSTR szValue, LPTSTR szFile);
	BOOL			bChangeFlushMouseCursor(UINT uCurID, LPIMECURSORDATA lpstCursorData);
	BOOL			bSetSystemCursor(LPMOUSECURSOR lpstMC, int iCursorSizeX, int iCursorSizeY);

public:

private:
	IMECURSORDATA	stIMECursorData;

	HWND			hMainWnd;
	HMODULE			hModDll;
	int				iCursorDataLoadCount;

	CThread* IMECursorChangeThread;
	CThread* DrawIMEModeThread;
	CThread* DrawIMEModeCaretThread;
	CCursorWindow* CursorWindow;
	CCursorWindow* CaretWindow;

	MOUSECURSOR	stAllMouseCursor[sizeof(MOUSECURSOR) * 19] = {
	{ OCR_APPSTARTING, 0, TRUE,  _T("AppStarting"), _T("") },
	{ OCR_NORMAL,	   0, FALSE, _T("Arrow"),		_T("") },
	{ OCR_CROSS,	   0, TRUE,  _T("Crosshair"),	_T("") },
	{ OCR_HAND,		   0, FALSE, _T("Hand"),	    _T("") },
	{ OCR_HELP,		   0, TRUE,  _T("Help"),	    _T("") },
	{ OCR_IBEAM,	   0, FALSE, _T("IBeam"),		_T("") },
	{ OCR_NO,		   0, TRUE,  _T("No"),			_T("") },
	// { OCR_NWPEN,	   0, TRUE,  _T("NWPen"),		_T("") },
	// { OCR_PEN,		   0, TRUE,  _T("Pen"),		    _T("") },
	{ OCR_PERSON,	   0, TRUE,  _T("Person"),		_T("") },
	{ OCR_PIN,		   0, TRUE,  _T("Pin"),		    _T("") },
	{ OCR_SIZEALL,	   0, TRUE,  _T("SizeAll"),	    _T("") },
	{ OCR_SIZENESW,	   0, TRUE,  _T("SizeNESW"),    _T("") },
	{ OCR_SIZENS,	   0, TRUE,  _T("SizeNS"),		_T("") },
	{ OCR_SIZENWSE,	   0, TRUE,  _T("SizeNWSE"),    _T("") },
	{ OCR_SIZEWE,	   0, TRUE,  _T("SizeWE"),		_T("") },
	{ OCR_UP,		   0, TRUE,  _T("UpArrow"),	    _T("") },
	{ OCR_WAIT,		   0, TRUE,  _T("Wait"),	    _T("") },
	{ OCR_HIDE,		   0, TRUE,  _T("Hide"),	    _T("") },
	{ (DWORD)(-1), 	   0, FALSE, _T(""),		    _T("") }		// Terminater
	};

	FLUSHMOUSECURSOR	stFlushMouseCursor[sizeof(FLUSHMOUSECURSOR) * 7] = {
		{ IMEOFF,			_T("A"),	{OCR_NORMAL, IDC_IMEOFF_ARROW,		  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeOffArrow.cur")},
										{OCR_HAND,   IDC_IMEOFF_HAND,		  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeOffHand.cur")},
										{OCR_IBEAM,  IDC_IMEOFF_IBEAM,		  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeOffIBeam.cur")}},
		{ ZENHIRA_IMEON,	_T("あ"),	{OCR_NORMAL, IDC_ZENHIRA_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenHiraOnArrow.cur")},
										{OCR_HAND,   IDC_ZENHIRA_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenHiraOnHand.cur")},
										{OCR_IBEAM,  IDC_ZENHIRA_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenHiraOnIBeam.cur")}},
		{ HANEISU_IMEON,	_T("_A"),	{OCR_NORMAL, IDC_HANEISU_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanEisuOnArrow.cur")},
										{OCR_HAND,   IDC_HANEISU_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanEisuOnHand.cur")},
										{OCR_IBEAM,  IDC_HANEISU_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanEisuOnIBeam.cur")}},
		{ HANKANA_IMEON,	_T("_ｱ"),	{OCR_NORMAL, IDC_HANKANA_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanKanaOnArrow.cur")},
										{OCR_HAND,   IDC_HANKANA_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanKanaOnHand.cur")},
										{OCR_IBEAM,  IDC_HANKANA_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHanKanaOnIBeam.cur")}},
		{ ZENEISU_IMEON,	_T("Ａ"),	{OCR_NORMAL, IDC_ZENEISU_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenEisuOnArrow.cur")},
										{OCR_HAND,   IDC_ZENEISU_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenEisuOnHand.cur")},
										{OCR_IBEAM,  IDC_ZENEISU_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenEisuOnIBeam.cur")}},
		{ ZENKANA_IMEON,	_T("ア"),	{OCR_NORMAL, IDC_ZENKANA_IMEON_ARROW, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenKanaOnArrow.cur")},
										{OCR_HAND,   IDC_ZENKANA_IMEON_HAND,  FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenKanaOnHand.cur")},
										{OCR_IBEAM,  IDC_ZENKANA_IMEON_IBEAM, FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeZenKanaOnIBeam.cur")}},
		{ IMEHIDE,			_T(""),		{OCR_NORMAL, IDC_HIDE_ARROW,          FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHideOnArrow.cur")},
										{OCR_HAND,   IDC_HIDE_HAND,           FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHideOnHand.cur")},
										{OCR_IBEAM,  IDC_HIDE_IBEAM,          FALSE, _T(""), _T("%APPDATA%\\JeffyTS\\FlushMouse\\ImeHideOnIBeam.cur")}},
		{ (DWORD)(-1),		_T(""),		{0,			 0,						  FALSE, _T(""), _T("")},		// Terminater
										{0,			 0,						  FALSE, _T(""), _T("")},		// Terminater
										{0,			 0,						  FALSE, _T(""), _T("")} }		// Terminater
	};

};

/* = EOF = */