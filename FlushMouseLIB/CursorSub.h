//
// CursorSub.h
//		Copyright (C) 2024 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/10/14  JeffyTS  	New edit.

//
// Include
//
#pragma once
#include "CommonDef.h"
#include "..\MiscLIB\CWindow.h"

//
// Define
//
#define	MAX_IMEMODECHAR		3

#define	IMEMODE_IMEOFF			0
#define	IMEMODE_ZENHIRA_IMEON	1
#define	IMEMODE_HANEISU_IMEON	2
#define	IMEMODE_HANKANA_IMEON	3
#define	IMEMODE_ZENEISU_IMEON	4
#define	IMEMODE_ZENKANA_IMEON	5
#define	IMEMODE_IMEHIDE			6

//
// Struct Define
//
typedef struct tagMOUSECURSOR {
	DWORD		id;
	int			iResourceID;
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

#pragma pack(push,1)
typedef struct {
	WORD Reserved;
	WORD ResType;
	WORD ResCount;
} NEWHEADER, *LPNEWHEADER;

typedef struct {
	WORD xHotSpot;
	WORD yHotSpot;
} LOCALHEADER, *LPLOCALHEADER;


typedef struct {
	WORD	Width;
	WORD	Height;
	WORD	xHotSpot;
	WORD	yHotSpot;
	DWORD	icoDIBSize;
	WORD	icoDIBOffset;
} ICONDIRENTRY, *LPICONDIRENTRY;

typedef struct {
	NEWHEADER	NewHeader;
} RTGROUPCURSORHEAD, *LPRTGROUPCURSORHEAD;

typedef struct {
	LOCALHEADER			LocalHeader;
	BITMAPINFOHEADER	BitmapInfoHeader;
	LPVOID				lpData;
} RTCURSORHEAD, *LPRTCURSORHEAD;
#pragma pack(pop)

//
// Global Prototype Define
//
extern VOID		vAdjustFontXPosition(DWORD dwIMEMode,LPCTSTR szMode, LPINT lpiXSize, LPRECT lprc);

//
// Class CCursorSub
// 
class CCursorSub
{
public:
	CCursorSub();
	~CCursorSub();

public:
	BOOL		bInitialize(LPCTSTR lpszCursorDataFileName);
	HMODULE		hLoadCursorData();
	BOOL		bUnLoadCursorData();

	BOOL		bMakeAllCursor(LPFLUSHMOUSECURSOR lpstIMECursorData);
	BOOL		bMakeOneUnitCursor(LPFLUSHMOUSECURSOR lpstIMECursorData, int iIMEMode);

private:
	BOOL		bGetCursorDataFullPath(LPCTSTR lpszCursorDataFileName);
	BOOL		bGetCursorDataTempFullPath(LPCTSTR lpszCursorDataFileName);
	BOOL		bCopyFile(LPCTSTR lpszDstPath,  LPCTSTR lpszSrcPath);

	BOOL		_bMakeOneUnitCursor(HMODULE hSrcMod, HANDLE hDstRes, LPFLUSHMOUSECURSOR lpstIMECursorData, int iIMEMode);
	BOOL		bMakeCursor(HMODULE hSrcMod, HANDLE hDstRes, int iSrcResID, int iDstResID, DWORD dwIMEMode, LPTSTR szIMEMode, COLORREF dwRGB, LPCTSTR szFontFace);
	BOOL		bMakeCursorSub(LPRTCURSORHEAD	lpRTCursorHead, LPRTCURSORHEAD lpMakeCursorData, DWORD dwResSize, int cx, int cy, DWORD dwIMEMode, LPTSTR lpszIMEMode, COLORREF dwRGB, LPCTSTR lpszFontFace);
	LPVOID		_FindResource(HMODULE hModule, int iResID, LPCTSTR ResType, LPDWORD lpdwResSize);
	void		ReverseDataTopDown(LPDWORD lpData, int cx, int cy);
	void		MakeAlphaBlend(LPDWORD lpData, int cx, int cy, COLORREF aRGB);

public:
	
private:
	LPTSTR		lpszCursorDataFullPath;	
	LPTSTR		lpszCursorDataTempFullPath;
	HMODULE		hCursorData;
	int			iCursorDataLoadCount;
};

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
	void		Cls_OnPaint(HWND hWnd) const;

public:

private:
	LPTSTR		lpszIMEMode;
	COLORREF	dwTextColor;
	COLORREF	dwBackColor;
	LPTSTR		lpszFontFace;
};



/* = EOF = */