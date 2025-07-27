//
// Cursor.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/12  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "CommonDef.h"
#include "Ime.h"
#include "CursorSub.h"
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

#define DisplayIMEModeMethod_RESOURCE		0
#define DisplayIMEModeMethod_ByWindow		1
#define DisplayIMEModeMethod_RES_AND_Window	2

#define	CLASS_CURSORWINDOW		_T("FlushMouseCursorWindow-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}")
#define	CLASS_CARETWINDOW		_T("FlushMouseCaretWindow-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}")
#define	CLASS_MOUSEWINDOW		_T("FlushMouseMouseWindow-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}")

//
// Struct Define
//
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
	int			iModeMouseDistanceX;
	int			iModeCaretDistanceX;
	DWORD		dwInThreadSleepTime;
	DWORD		dwDisplayModeTime;
	BOOL		bDisplayIMEModeOnCursor;
	DWORD		dwDisplayIMEModeMethod;
	BOOL		bDisplayIMEModeIMEOFF;
	BOOL		bForceHiragana;
	BOOL		bDrawNearCaret;

	BOOL		bDenyChangedByApp;
	BOOL		bUseBigArrow;
	BOOL		bDisplayFocusWindowIME;
} IMECURSORDATA, * PIMECURSORDATA, * LPIMECURSORDATA;

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
		BOOL		bStartIMECursorChangeThreadWait(HWND hWndObserved);
		BOOL		bStartIMECursorChangeThreadByWnd(HWND hWndObserved);
		BOOL		bStartDrawIMEModeThread(HWND hWndObserved);
		BOOL		bStartDrawIMEModeThreadWaitWave(HWND hWndObserved);
		BOOL		bStartDrawIMEModeThreadWaitEventForeGround(HWND hWndObserved);
		BOOL		bStartDrawIMEModeThreadWaitDblClk(HWND hWndObserved);

		BOOL		bStartDrawIMEModeMouseByWndThread();
		VOID		vStopDrawIMEModeMouseByWndThread();

	private:
		BOOL		bStartIMECursorChangeThreadSub(HWND hWndObserved);
		BOOL		bStartDrawIMEModeThreadSub(HWND hWndObserved);

		BOOL		bRegisterIMECursorChangeThread(HWND hWnd);
		VOID		vUnRegisterIMECursorChangeThread();
		static BOOL WINAPI		bIMECursorChangeRoutine(LPVOID lpvParam);

		BOOL		bRegisterDrawIMEModeMouseByWndThread(HWND hWnd);
		VOID		vUnRegisterDrawIMEModeMouseByWndThread();
		static BOOL WINAPI	bIMEModeMouseByWndThreadRoutine(LPVOID lpvParam);

		BOOL		bRegisterDrawIMEModeThread(HWND hWndObserved);
		BOOL		bIsIMECursorChanged(LPIMECURSORDATA lpstCursorData);
		BOOL		bDrawIMEModeOnDisplay(LPIMECURSORDATA lpstCursorData);
		BOOL		bCalcDisplayModeRect(LPINT iModeSizeX, LPINT iModeSizeY, LPRECT lpRect);
		HWND		hGetCaretPosByAccessibleObjectFromWindow(HWND hForeWnd, LPRECT lprcCaret, BOOL bAttachThreadInput);
		BOOL		bAccessibleObjectFromWindowAsync(HWND hWnd, DWORD dwId, REFIID riId, void **ppvObject);
		BOOL		bAdjustModeSizeByMonitorDPI(int iModeSizeX, int iModeSizeY, LPRECT lpRect);
		BOOL		bAdjustModeSizeByMonitorDPIAsync();
		BOOL		bDrawIMEModeOnDisplaySub(LPIMECURSORDATA lpstCursorData, LPRECT lprcCaret);
		static int	iGetCursorID(DWORD dwIMEMode, LPFLUSHMOUSECURSOR lpstCursorData);
		static BOOL	CALLBACK	bIconDrawEnumProc(HMONITOR hMonitor, HDC hDC, LPCRECT lprcClip, LPARAM lParam);
		static BOOL WINAPI		bDrawIMEModeRoutine(LPVOID lpvParam);

		BOOL		bChangeFlushMouseCursor(UINT uCurID, LPIMECURSORDATA lpstCursorData);
		BOOL		bSetSystemCursor(LPMOUSECURSOR lpstMC, int iCursorSizeX, int iCursorSizeY);

	public:

	private:
		IMECURSORDATA	stIMECursorData;

		HWND			_hMainWnd;
		HMODULE			hCursorData;
		int				iCursorDataLoadCount;

		CThread			*IMECursorChangeThread;
		CThread			*DrawIMEModeThread;
		CThread			*DrawIMEModeCaretThread;
		CThread			*DrawIMEModeMouseByWndThread;

		CCursorWindow	*CursorWindow;
		CCursorWindow	*CaretWindow;
		CCursorWindow	*MouseWindow;
		DWORD			dwIMEModeMouseWindow;
		DWORD			dwWaitTimeMouseWindow;
		ULONGLONG		uuMouseWindowTick;
		CCursorSub		*CursorSub;
	
		HCURSOR			hCursorArrow;
		HCURSOR			hCursorIBeam;
		HCURSOR			hCursorHand;
		HCURSOR			hCursorWait;
		HCURSOR			hCursorSizeNWSE;
		HCURSOR			hCursorSizeNESW;
		HCURSOR			hCursorSizeWE;
		HCURSOR			hCursorSizeNS;
		HCURSOR			hCursorSizeAll;
		HCURSOR			hCursorAppStarting;

		HWND			hWndCaret;
		RECT			rcCaret;
		BOOL			bCapsLock;

		BOOL			bIMEModeByWindowThreadSentinel;
		BOOL			bIMECursorChangeThreadSentinel;
};



/* = EOF = */