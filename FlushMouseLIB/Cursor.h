//
// Cursor.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/03/12  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "CommonDef.h"
#include "Ime.h"
#include "..\FlushMouseCursor\Resource.h"
#include "..\MiscLIB\CThread.h"
#include "..\MiscLIB\CWindow.h"

//
// Define
//
#define	MAX_IMEMODECHAR		3

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
	DWORD		id;	
	UINT		uResourceID;
	BOOL		bReadReg;
	TCHAR		szRegValue[MAX_LOADSTRING];
	TCHAR		szFile[_MAX_PATH];
} MOUSECURSOR, * PMOUSECURSOR, * LPMOUSECURSOR;

typedef struct tagFLUSHMOUSECURSOR {
	DWORD		dwIMEMode;
	TCHAR		szMode[MAX_IMEMODECHAR];	
	COLORREF	dwColor;
	TCHAR		szFont[LF_FACESIZE];
	MOUSECURSOR	stArrow;	
	MOUSECURSOR	stHand;
	MOUSECURSOR	stIBeam;	
} FLUSHMOUSECURSOR, * PFLUSHMOUSECURSOR, * LPFLUSHMOUSECURSOR;

typedef struct tagIMECursorData
{
	HWND		hWndObserved;
	DWORD		dwIMEModeCursor;
	DWORD		dwWaitTime;
	BOOL		bDrawIMEModeWait;
	LPCTSTR		lpszLoadDatName;
	LPFLUSHMOUSECURSOR	lpstNearDrawMouseCursor;
	LPFLUSHMOUSECURSOR	lpstNearDrawCaretCursor;
	LPFLUSHMOUSECURSOR	lpstNearDrawMouseByWndCursor;

	int			iCursorSize;
	int			iIMEModeDistance;
	int			iModeMouseSize;
	int			iModeCaretSize;
	int			iModeByWndSize;
	DWORD		dwInThreadSleepTime;
	DWORD		dwDisplayModeTime;
	BOOL		bDisplayIMEModeOnCursor;
	BOOL		bDisplayIMEModeByWindow;
	BOOL		bDisplayIMEModeIMEOFF;
	BOOL		bForceHiragana;
	BOOL		bDrawNearCaret;

	BOOL		bDenyChangedByApp;
	BOOL		bUseBigArrow;
	BOOL		bDisplayFocusWindowIME;

	BOOL		bIMEModeByWindowThreadSentinel;
	BOOL		bIMECursorChangeThreadSentinel;

	HWND		hWndCaret;
	RECT		rcCaret;
} IMECURSORDATA, * PIMECURSORDATA, * LPIMECURSORDATA;

//
// Class CCursorWindow
//
class CCursorWindow : public CWindow
{
public:
	CCursorWindow();
	~CCursorWindow();

public:
	BOOL		bRegister(HINSTANCE hInstance, LPCTSTR szWindowClass);
	VOID		vSetModeStringColorFont(LPCTSTR _lpszIMEMode, COLORREF dwRGB, LPCTSTR _lpszFontFace);

private:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	BOOL		Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) const;
	void		Cls_OnDestroy(HWND hWnd);
	void		Cls_OnPaint(HWND hWnd);

public:

private:
	LPTSTR		lpszIMEMode;
	COLORREF	dwTextColor;
	COLORREF	dwBackColor;
	LPTSTR		lpszFontFace;
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
	BOOL		bInitialize(HWND hWnd);
	BOOL		bReloadCursor();
	VOID		vSetParamFromRegistry();
	BOOL		bStartIMECursorChangeThread(HWND hWndObserved);
	BOOL		bStartDrawIMEModeThread(HWND hWndObserved);
	BOOL		bStartDrawIMEModeThreadWait(HWND hWndObserved);
	BOOL		bStartDrawIMEModeThreadWaitWave(HWND hWndObserved);
	BOOL		bStartDrawIMEModeThreadWaitEventForeGround(HWND hWndObserved);
	BOOL		bStartDrawIMEModeThreadWaitDblClk(HWND hWndObserved);

	BOOL		bShowHideCursor(HWND hWndObserved, BOOL bShow);
	BOOL		bStartDrawIMEModeMouseByWndThread();
	VOID		vStopDrawIMEModeMouseByWndThread();

private:
	BOOL		bStartDrawIMEModeThreadSub(HWND hWndObserved);

	LPTSTR		lpszGetCursorDataName();
	HMODULE		hCursorDllLoad();
	BOOL		bCursorDllUnload();
	BOOL		bSystemCursorLoad();

	BOOL		bRegisterIMECursorChangeThread(HWND hWnd);
	VOID		vUnRegisterIMECursorChangeThread();
	static BOOL WINAPI		bIMECursorChangeRoutine(LPVOID lpvParam);

	BOOL		bRegisterDrawIMEModeMouseByWndThread(HWND hWnd);
	VOID		vUnRegisterDrawIMEModeMouseByWndThread();
	static BOOL WINAPI	bIMEModeMouseByWndThreadRoutine(LPVOID lpvParam);

	BOOL		bRegisterDrawIMEModeThread(HWND hWndObserved);
	BOOL		bIsIMECursorChanged(LPIMECURSORDATA lpstCursorData);
	BOOL		bDrawIMEModeOnDisplay(LPIMECURSORDATA lpstCursorData);
	BOOL		bCalcDisplayModeRect(int iModeSizeX, int iModeSizeY, LPRECT lpRect);
	HWND		hGetCaretPosByAccessibleObjectFromWindow(HWND hForeWnd, LPIMECURSORDATA lpstCursorData, BOOL bAttachThreadInput);
	BOOL		bAdjustModeSizeByMonitorDPI(int iModeSizeX, int iModeSizeY, LPRECT lprcCaret);
	BOOL		bDrawIMEModeOnDisplaySub(LPIMECURSORDATA lpstCursorData);
	static BOOL	CALLBACK	bIconDrawEnumProc(HMONITOR hMonitor, HDC hDC, LPCRECT lprcClip, LPARAM lParam);
	static BOOL WINAPI		bDrawIMEModeRoutine(LPVOID lpvParam);
	int			iGetCursorID(DWORD dwIMEMode, LPFLUSHMOUSECURSOR lpstCursorData);

	BOOL		bGetMouseRegValue(LPCTSTR szValue, LPTSTR szFile);
	BOOL		bChangeFlushMouseCursor(UINT uCurID, LPIMECURSORDATA lpstCursorData);
	BOOL		bSetSystemCursor(LPMOUSECURSOR lpstMC, int iCursorSizeX, int iCursorSizeY);

public:

private:
	IMECURSORDATA	stIMECursorData;

	HWND			hMainWnd;
	HMODULE			hCursorData;
	int				iCursorDataLoadCount;

	CThread			*IMECursorChangeThread;
	CThread			*DrawIMEModeThread;
	CThread			*DrawIMEModeCaretThread;
	CThread			*DrawIMEModeMouseByWndThread;

	CCursorWindow	*CursorWindow;
	CCursorWindow	*CaretWindow;
	CCursorWindow	*MouseWindow;
};


/* = EOF = */