// FlushMouseLIB.h
//		Copyright (C) 1993 - 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/07/09  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "Profile.h"
#include "Cursor.h"
#include "FlushMouseSub.h"

//
// Define
//

//
// Class CResource
//
class CResource
{
public:
	CResource(LPCTSTR lpszResFile);
	~CResource();

private:
	CResource();

public:
	HMODULE	hLoad();
	BOOL	bUnload();

private:
	LPTSTR	szResFile;
	HMODULE	hModRes;
	int		iResourceLoadCount;
};


//
// Global Prototype Define
//
extern BOOL			bWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance);
extern BOOL			bStartThredHookTimer(HWND hWnd);
extern VOID			vStartStopTimer(HWND hWnd, BOOL bStart);
extern VOID			vStopThredHookTimer(HWND hWnd);
extern BOOL			bForExplorerPatcherSWS(HWND hForeWnd, BOOL bChangeToIME, LPHKL lpNewHKL, LPHKL lpPreviousHKL);
extern BOOL			bCheckExistingJPIME();

//
// Global Data
//
extern TCHAR		szFlushMouse64[];
extern TCHAR		szFlushMouse32[];

extern TCHAR		szTitle[];
extern TCHAR		szWindowClass[];
extern HWND			hMainWnd;
//extern HINSTANCE		hMainInst;

extern BOOL			bDoModeDispByIMEKeyDown;
extern BOOL			bDoModeDispByCtrlUp;
extern BOOL			bDisplayFocusWindowIME;
extern BOOL			bDisplayIMEModeOnCursor;
extern BOOL			bDoModeDispByMouseBttnUp;
extern BOOL			bDrawNearCaret;
extern BOOL			bEnableEPHelper;

extern CProfile		*Profile;
extern CCursor		*Cursor;
extern CResource	*Resource;
extern CIME			*Cime;

/* EOF */