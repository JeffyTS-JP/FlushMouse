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
#include "..\MiscLIB\CThread.h"

//
// Define
//
#define MAX_LOADSTRING 100
#define	MODECHAR		3

// for IME
#define IMECLOSE				0x0000
#define IMEOPEN					0x0001
#define IMC_GETCONVERSIONMODE   0x0001
#define IMC_SETCONVERSIONMODE   0x0002
#define IMC_GETSENTENCEMODE		0x0003
#define IMC_SETSENTENCEMODE		0x0004
#define IMC_GETOPENSTATUS		0x0005
#define IMC_SETOPENSTATUS		0x0006

// IME mode
#define IMEOFF			(IME_CMODE_ALPHANUMERIC)														// 0x00 : A 英数(直接入力)
#define HANEISU_IMEON	(IME_CMODE_ROMAN | IME_CMODE_ALPHANUMERIC)										// 0x10 :_A 半角英数 0001 0000
#define HANKANA_IMEON	(IME_CMODE_ROMAN | IME_CMODE_KATAKANA | IME_CMODE_NATIVE)						// 0x13 :_ｶ 半角カナ 0001 0011
#define ZENEISU_IMEON	(IME_CMODE_ROMAN | IME_CMODE_FULLSHAPE)											// 0x18 :Ａ 全角英数 0001 1000
#define ZENHIRA_IMEON	(IME_CMODE_ROMAN | IME_CMODE_FULLSHAPE | IME_CMODE_NATIVE)						// 0x19 :あ 全角ひら 0001 1001（漢字変換モード)
#define ZENKANA_IMEON	(IME_CMODE_ROMAN | IME_CMODE_FULLSHAPE | IME_CMODE_KATAKANA | IME_CMODE_NATIVE)	// 0x1B :カ 全角カナ 0001 1011

//
// Struct Define
//
typedef struct tagMOUSECURSOR {
	DWORD		id;								// OCR_xxx
	UINT		uResourceID;					// Resource ID
	BOOL		bReadReg;						// TRUE = Read from Registry
	TCHAR		szRegValue[MAX_LOADSTRING];		// Registory Value
	TCHAR		szFile[_MAX_PATH];				// Cursor File Name
} MOUSECURSOR, * PMOUSECURSOR, * LPMOUSECURSOR;

typedef struct tagFLUSHMOUSECURSOR {
	UINT		dwIMEMode;						// IME Mode ID
	TCHAR		szMode[MODECHAR];				// Mode Display Char
	MOUSECURSOR	stArrow;						// Arrow MOUSECURSOR
	MOUSECURSOR	stHand;							// Hand MOUSECURSOR
	MOUSECURSOR	stIbeam;						// Ibeam MOUSECURSOR
} FLUSHMOUSECURSOR, * PFLUSHMOUSECURSOR, * LPFLUSHMOUSECURSOR;

typedef struct tagIMECursorData
{
	// Use in Curosr
	HWND		hWndObserved;
	UINT		dwIMEState;
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
// Class CCursor
// 
class CCursor
{
public:
	CCursor();
	~CCursor();

public:
	BOOL			bInitialize(HWND hWnd);
	BOOL			bIsIMEOpen(HWND hWndObserved);
	VOID			vIMEOpenCloseForced(HWND hWndObserved, DWORD dwIMEOpenClose);
	VOID			vIMEConvertModeChangeForced(HWND hWndObserved, DWORD dwConvertMode);
	BOOL			bStartIMECursorChangeThread(HWND hWndObserved);
	BOOL			bStartDrawIMEModeThread(HWND hWndObserved);
	BOOL			bStartDrawIMEModeThreadWait(HWND hWndObserved);
	BOOL			bStartDrawIMEModeThreadWaitDblClk(HWND hWndObserved);

private:
	static BOOL CALLBACK bEnumChildProcIMEOpenClose(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK bEnumChildProcIMECnvertMode(HWND hWnd, LPARAM lParam);

	LPTSTR			lpszGetCursorDataName();
	HMODULE			hCursorDllLoad();
	BOOL			bCursorDllUnload();
	BOOL			bSystemCursorLoad();

	BOOL			bRegisterIMECursorChangeThread(HWND hWndObserved);
	BOOL			bIsIMECursorChanged(LPIMECURSORDATA lpstCursorData);
	DWORD			dwIMECursorMode(HWND hWndObserved, BOOL bForceHiragana);
	static BOOL WINAPI	bIMECursorChangeRoutine(LPVOID lpvParam);

	BOOL			bRegisterDrawIMEModeThread(HWND hWndObserved);
	BOOL			bDrawIMEModeOnDisplay(LPIMECURSORDATA lpstCursorData);
	BOOL			bCalcDispModeRect(int iModeSizeX, int iModeSizeY, LPRECT lpRect);
	BOOL			bCalcDispModeCaretRect(int iModeSizeX, int iModeSizeY, LPRECT lpRect, LPPOINT lpPt);
	BOOL			bDrawIMEModeOnDisplaySub(LPIMECURSORDATA lpstCursorData);
	static BOOL		bIconDrawEnumProc(HDC hDC, LPCRECT lprcClip, MONITORENUMPROC lpfnEnum, LPARAM dwData);
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

	CThread			*IMECursorChangeThread;
	CCursorWindow	*CursorWindow;
	CThread			*DrawIMEModeThread;
	CCursorWindow	*CaretWindow;
	CThread			*DrawIMEModeCaretThread;

};
