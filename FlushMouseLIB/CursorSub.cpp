//
// CursorSub.cpp
//		Copyright (C) 2024 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/10/14  JeffyTS  	New edit.
//

// Include
//
#pragma once
#include "pch.h"
#include "CursorSub.h"
#include "Profile.h"
#include "..\FlushMouseCursor\Resource.h"

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
// Local Prototype Define
//
static BOOL		TextDraw(HDC hDC, RECT rcSize, LPTSTR lpszIMEMode, COLORREF dwRGB, LPCTSTR lpszFontFace, int cWeight, DWORD dwUnderline);

//
// class CCursorSub
//
CCursorSub::CCursorSub()
{
	lpszCursorDataFullPath = NULL;
	lpszCursorDataTempFullPath = NULL;
	hCursorData = NULL;
	iCursorDataLoadCount = 0;
}

CCursorSub::~CCursorSub()
{
	if (hCursorData)	bUnLoadCursorData();
	if (lpszCursorDataFullPath)	delete []	lpszCursorDataFullPath;
	if (lpszCursorDataTempFullPath) {
		if (!DeleteFile(lpszCursorDataTempFullPath)) {
		}
		delete []	lpszCursorDataTempFullPath;
	}
}

//
//	bInitialize()
//
BOOL		CCursorSub::bInitialize(LPCTSTR lpszCursorDataFileName)
{
	BOOL	bRet = FALSE;
	
	if (!bGetCursorDataFullPath(lpszCursorDataFileName))	goto Cleanup;

	if (!bGetCursorDataTempFullPath(lpszCursorDataFileName))	goto Cleanup;
	if (!DeleteFile(lpszCursorDataTempFullPath)) {
	}
	if (!bCopyFile(lpszCursorDataTempFullPath, lpszCursorDataFullPath))	goto Cleanup;

	if (!hLoadCursorData())	goto Cleanup;

	bRet = TRUE;

Cleanup:
	return bRet;
}

//
// bLoadCursorData()
//
HMODULE		CCursorSub::hLoadCursorData()
{
	if ((hCursorData == NULL) && (iCursorDataLoadCount <= 0)) {
		if ((hCursorData = LoadLibraryEx(lpszCursorDataTempFullPath, NULL, (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE))) == NULL) {
			return NULL;
		}
	}
	++iCursorDataLoadCount;
	return hCursorData;
}

//
// bUnLoadCursorData()
//
BOOL		CCursorSub::bUnLoadCursorData()
{
	if ((hCursorData != NULL) && (iCursorDataLoadCount == 1)) {
		if (!FreeLibrary(hCursorData)) {
			hCursorData = NULL;
			return FALSE;
		}
	}
	--iCursorDataLoadCount;
	if (iCursorDataLoadCount == 0)	hCursorData = NULL;
	return TRUE;
}

//
// bGetCursorDataFullPath()
//
BOOL		CCursorSub::bGetCursorDataFullPath(LPCTSTR lpszCursorDataFileName)
{
#define FLUSHMOUSECURSORDIR			L"\\JeffyTS\\FlushMouse\\"	// Cursor data dir
	
	LPTSTR	lpszBuffer = new TCHAR[_MAX_PATH];
	if (lpszBuffer) {
		ZeroMemory(lpszBuffer, _MAX_PATH);
		DWORD	dwSize = 0;
		GetCurrentDirectory(_MAX_PATH, lpszBuffer);
		_tcsncat_s(lpszBuffer, _MAX_PATH, L"\\", _TRUNCATE);
		_tcsncat_s(lpszBuffer, _MAX_PATH, lpszCursorDataFileName, _TRUNCATE);
		dwSize = ExpandEnvironmentStrings(lpszBuffer, NULL, 0);
		lpszCursorDataFullPath = new TCHAR[dwSize];
		if (lpszCursorDataFullPath) {
			ZeroMemory(lpszCursorDataFullPath, dwSize);
			dwSize = ExpandEnvironmentStrings(lpszBuffer, lpszCursorDataFullPath, dwSize);
			if (GetFileAttributes(lpszCursorDataFullPath) == INVALID_FILE_ATTRIBUTES) {
				delete[]	lpszCursorDataFullPath;
				lpszCursorDataFullPath = NULL;
				ZeroMemory(lpszBuffer, _MAX_PATH);
				_tcsncat_s(lpszBuffer, _MAX_PATH, L"%ALLUSERSPROFILE%", _TRUNCATE);
				_tcsncat_s(lpszBuffer, _MAX_PATH, FLUSHMOUSECURSORDIR, _TRUNCATE);
				_tcsncat_s(lpszBuffer, _MAX_PATH, lpszCursorDataFileName, wcsnlen_s(lpszCursorDataFileName, _TRUNCATE));
				dwSize = ExpandEnvironmentStrings(lpszBuffer, NULL, 0);
				lpszCursorDataFullPath = new TCHAR[dwSize];
				if (lpszCursorDataFullPath) {
					ZeroMemory(lpszCursorDataFullPath, dwSize);
					dwSize = ExpandEnvironmentStrings(lpszBuffer, lpszCursorDataFullPath, dwSize);
					if (GetFileAttributes(lpszCursorDataFullPath) == INVALID_FILE_ATTRIBUTES) {
						delete[]	lpszCursorDataFullPath;
						lpszCursorDataFullPath = NULL;
					}
				}
			}
		}
		delete[]	lpszBuffer;
	}
	if (lpszCursorDataFullPath)	return TRUE;
	return FALSE;
}

//
// bGetCursorDataTempFullPath()
//
BOOL		CCursorSub::bGetCursorDataTempFullPath(LPCTSTR lpszCursorDataFileName)
{
	LPTSTR	lpszBuffer = new TCHAR[_MAX_PATH + 1];
	size_t	size = 0;
	if (lpszBuffer) {
		ZeroMemory(lpszBuffer, _MAX_PATH + 1);
		GetTempPath(_MAX_PATH, lpszBuffer);
		_tcsncat_s(lpszBuffer, _MAX_PATH + 1, lpszCursorDataFileName, _TRUNCATE);
		size = wcsnlen_s(lpszBuffer, _MAX_PATH + 1);
		lpszCursorDataTempFullPath = new TCHAR[size + 1];
		if (lpszCursorDataTempFullPath) {
			ZeroMemory(lpszCursorDataTempFullPath, size + 1);
			_tcsncpy_s(lpszCursorDataTempFullPath, size + 1, lpszBuffer, _TRUNCATE);
		}
		delete[]	lpszBuffer;
	}
	if (lpszCursorDataTempFullPath)	return TRUE;
	return FALSE;
}

//
// bCopyFile()
//
BOOL		CCursorSub::bCopyFile(LPCTSTR lpszDstPath, LPCTSTR lpszSrcPath)
{
	HANDLE	hSrcFile = NULL;
	LARGE_INTEGER	SrcSize{};
	HANDLE	hDstFile = NULL;
	DWORD	dwFileSize = 0;
	LPBYTE	lpBuffer = NULL;
	DWORD	dwNumberOfBytesRead = 0;
	BOOL	bRet = FALSE;

	if ((hSrcFile = CreateFile(lpszSrcPath, (GENERIC_READ), FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)	goto Cleanup;
	if (!GetFileSizeEx(hSrcFile, &SrcSize))	goto Cleanup;

	if ((hDstFile = CreateFile(lpszDstPath, (GENERIC_READ | GENERIC_WRITE), FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL)) == INVALID_HANDLE_VALUE)	goto Cleanup;
	if ((dwFileSize = GetFileSize(hSrcFile, NULL)) == INVALID_FILE_SIZE)	goto Cleanup;

#define	BUFFERSIZE	(1024 * 1024)
	lpBuffer = new BYTE[BUFFERSIZE];
	if (!lpBuffer)	goto Cleanup;
	for (DWORD i = 0; i < dwFileSize; i++) {
		if (!ReadFile(hSrcFile, lpBuffer, BUFFERSIZE, &dwNumberOfBytesRead, NULL))	goto Cleanup;
		if (!WriteFile(hDstFile, lpBuffer, dwNumberOfBytesRead, NULL, NULL))		goto Cleanup;
		if (dwNumberOfBytesRead < BUFFERSIZE)	break;
	}

	bRet = TRUE;

Cleanup:
	if (lpBuffer)	delete [] 	lpBuffer;
	lpBuffer = NULL;
	if (hDstFile)	CloseHandle(hSrcFile);
	hSrcFile = NULL;
	if (hDstFile)	CloseHandle(hDstFile);
	hDstFile = NULL;
	return bRet;
}

//
// bMakeAllCursor()
//
BOOL		CCursorSub::bMakeAllCursor(LPFLUSHMOUSECURSOR lpstIMECursorData)
{
	HMODULE		hSrcMod;
	HANDLE		hDstRes;
	BOOL	bRet = FALSE;

	if ((hSrcMod = LoadLibrary(lpszCursorDataFullPath)) == NULL)	goto Cleanup;
	if ((hDstRes = BeginUpdateResource(lpszCursorDataTempFullPath, FALSE)) == NULL)	goto Cleanup;

	if (!_bMakeOneUnitCursor(hSrcMod, hDstRes, lpstIMECursorData, IMEMODE_IMEOFF))	goto Cleanup;
	if (!_bMakeOneUnitCursor(hSrcMod, hDstRes, lpstIMECursorData, IMEMODE_ZENHIRA_IMEON))	goto Cleanup;
	if (!_bMakeOneUnitCursor(hSrcMod, hDstRes, lpstIMECursorData, IMEMODE_HANEISU_IMEON))	goto Cleanup;
	if (!_bMakeOneUnitCursor(hSrcMod, hDstRes, lpstIMECursorData, IMEMODE_HANKANA_IMEON))	goto Cleanup;
	if (!_bMakeOneUnitCursor(hSrcMod, hDstRes, lpstIMECursorData, IMEMODE_ZENEISU_IMEON))	goto Cleanup;
	if (!_bMakeOneUnitCursor(hSrcMod, hDstRes, lpstIMECursorData, IMEMODE_ZENKANA_IMEON))	goto Cleanup;
	if (!_bMakeOneUnitCursor(hSrcMod, hDstRes, lpstIMECursorData, IMEMODE_IMEHIDE))	goto Cleanup;

	if (hDstRes)	if (!EndUpdateResource(hDstRes, FALSE))	goto Cleanup;

	bRet = TRUE;

Cleanup:

	if (hSrcMod)	FreeLibrary(hSrcMod);
	hSrcMod = NULL;

	return bRet;
}

//
// bMakeOneUnitCursor();
//
BOOL		CCursorSub::bMakeOneUnitCursor(LPFLUSHMOUSECURSOR lpstIMECursorData, int iIMEMode)
{
	HMODULE		hSrcMod;
	HANDLE		hDstRes;
	BOOL	bRet = FALSE;

	if ((hSrcMod = LoadLibrary(lpszCursorDataFullPath)) == NULL)	goto Cleanup;
	if ((hDstRes = BeginUpdateResource(lpszCursorDataTempFullPath, FALSE)) == NULL)	goto Cleanup;
	
	if (!_bMakeOneUnitCursor(hSrcMod, hDstRes, lpstIMECursorData, iIMEMode))	goto Cleanup;

	if (hDstRes)	if (!EndUpdateResource(hDstRes, FALSE))	goto Cleanup;
	
	bRet = TRUE;

Cleanup:
	if (hSrcMod)	FreeLibrary(hSrcMod);
	hSrcMod = NULL;
	
	return bRet;

}

//
// _bMakeOneUnitCursor();
//
BOOL		CCursorSub::_bMakeOneUnitCursor(HMODULE hSrcMod, HANDLE hDstRes, LPFLUSHMOUSECURSOR lpstIMECursorData, int iIMEMode)
{
	BOOL	bRet = FALSE;
	
	if (!bMakeCursor(hSrcMod, hDstRes, IDC_HIDE_ARROW,	lpstIMECursorData[iIMEMode].stArrow.iResourceID,
														lpstIMECursorData[iIMEMode].dwIMEMode,
														lpstIMECursorData[iIMEMode].szMode,
														lpstIMECursorData[iIMEMode].dwColor,
														lpstIMECursorData[iIMEMode].szFont))	goto Cleanup;
	if (!bMakeCursor(hSrcMod, hDstRes, IDC_HIDE_IBEAM,	lpstIMECursorData[iIMEMode].stIBeam.iResourceID,
														lpstIMECursorData[iIMEMode].dwIMEMode,
														lpstIMECursorData[iIMEMode].szMode,
														lpstIMECursorData[iIMEMode].dwColor,
														lpstIMECursorData[iIMEMode].szFont))	goto Cleanup;
	if (!bMakeCursor(hSrcMod, hDstRes, IDC_HIDE_HAND,	lpstIMECursorData[iIMEMode].stHand.iResourceID,
														lpstIMECursorData[iIMEMode].dwIMEMode,
														lpstIMECursorData[iIMEMode].szMode,
														lpstIMECursorData[iIMEMode].dwColor,
														lpstIMECursorData[iIMEMode].szFont))	goto Cleanup;

	bRet = TRUE;
	
Cleanup:
	return bRet;

}

//
// bMakeCursor()
//
BOOL		CCursorSub::bMakeCursor(HMODULE hSrcMod, HANDLE hDstRes, int iSrcResID, int iDstResID, DWORD dwIMEMode, LPTSTR szIMEMode, COLORREF dwRGB, LPCTSTR szFontFace)
{

	LPRTGROUPCURSORHEAD	lpRTGrpCursorHead = NULL;
	LPVOID	lpGrpResLock = NULL;
	DWORD	dwGrpResSize = 0;
	LPRTCURSORHEAD	lpMakeCursorData = NULL;

	BOOL	bRet = FALSE;

	if (!(lpRTGrpCursorHead = (LPRTGROUPCURSORHEAD)_FindResource(hSrcMod, iSrcResID, RT_GROUP_CURSOR, NULL)))	goto Cleanup;

	if (!(lpGrpResLock =_FindResource(hSrcMod, iDstResID, RT_GROUP_CURSOR, &dwGrpResSize)))	goto Cleanup;
	if (dwGrpResSize == 0)		goto Cleanup;

	if (lpRTGrpCursorHead->NewHeader.ResType != 2 || lpRTGrpCursorHead->NewHeader.ResCount == 0)	goto Cleanup;

	for (int i = 0; i < lpRTGrpCursorHead->NewHeader.ResCount; i++) {
		LPICONDIRENTRY	lpIconDirEntry = NULL;
		LPRTCURSORHEAD	lpRTCursorHead = NULL;
		DWORD	dwResSize = 0;
		int		cx = 0, cy = 0;
		int		iSrcID = 0;
		int		iDstID = 0;

		lpIconDirEntry = (LPICONDIRENTRY)(LPBYTE)((LPBYTE)lpRTGrpCursorHead + sizeof(RTGROUPCURSORHEAD) + (sizeof(ICONDIRENTRY) * i));
		cx = lpIconDirEntry->Width;	cy = lpIconDirEntry->Height / 2;
		if ((iSrcID = LookupIconIdFromDirectoryEx((LPBYTE)lpRTGrpCursorHead, FALSE, cx, cy, (LR_DEFAULTCOLOR))) == 0)	goto Cleanup;

		if (!(lpRTCursorHead = (LPRTCURSORHEAD)_FindResource(hSrcMod, iSrcID, RT_CURSOR, &dwResSize)))	goto Cleanup;
		if (dwResSize == 0)		goto Cleanup;

		if (!(lpMakeCursorData = new RTCURSORHEAD[dwResSize]))	goto Cleanup;
		ZeroMemory(lpMakeCursorData, dwResSize);

		if (!bMakeCursorSub(lpRTCursorHead, lpMakeCursorData, dwResSize, cx, cy, dwIMEMode, szIMEMode, dwRGB, szFontFace))	goto Cleanup;

		if ((iDstID = LookupIconIdFromDirectoryEx((PBYTE)lpGrpResLock, FALSE, cx, cy, (LR_DEFAULTCOLOR))) == 0)	goto Cleanup;
		if (!UpdateResource(hDstRes, RT_CURSOR, MAKEINTRESOURCE(iDstID), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), NULL, 0))	goto Cleanup;
		if (!UpdateResource(hDstRes, RT_CURSOR, MAKEINTRESOURCE(iDstID), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), lpMakeCursorData, dwResSize))	goto Cleanup;
		if (lpMakeCursorData)	delete []	lpMakeCursorData;
		lpMakeCursorData = NULL;
	}

	if (!UpdateResource(hDstRes, RT_GROUP_CURSOR, MAKEINTRESOURCE(iDstResID), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), lpGrpResLock, dwGrpResSize))	goto Cleanup;


	bRet = TRUE;

Cleanup:
	
	if (lpMakeCursorData)	delete []	lpMakeCursorData;
	lpMakeCursorData = NULL;
	
	return bRet;
}

//
// bMakeCursorSub()
//
BOOL		CCursorSub::bMakeCursorSub(LPRTCURSORHEAD	lpRTCursorHead, LPRTCURSORHEAD lpMakeCursorData, DWORD dwResSize, int cx, int cy, DWORD dwIMEMode, LPTSTR lpszIMEMode, COLORREF dwRGB, LPCTSTR lpszFontFace)
{
	HDC		hDC = NULL, hCursorMemDC = NULL, hTextMemDC = NULL;
	HBITMAP	hCursorBitmap = NULL, hTextBitmap = NULL;
	HGDIOBJ	hCursorGDIObj = NULL, hTextGDIObj = NULL; 
	LPVOID	lpCursorBits = NULL, lpTextBits = NULL;
	RTCURSORHEAD	RTCursorHead{};
	RECT	rc{};
	int		iXSize = 0;
	size_t	CursorBytes = 0;
	BOOL	bRet = FALSE;

	if ((hDC = GetDC(NULL)) == NULL)	goto Cleanup;
	if ((hCursorMemDC = CreateCompatibleDC(hDC)) == NULL)	goto Cleanup;
	if ((hTextMemDC = CreateCompatibleDC(hDC)) == NULL)	goto Cleanup;
	if (hDC)	DeleteDC(hDC);
	hDC = NULL;
	
	memcpy_s(&RTCursorHead, sizeof(RTCURSORHEAD), lpRTCursorHead, sizeof(RTCURSORHEAD));
	RTCursorHead.BitmapInfoHeader.biHeight = -(RTCursorHead.BitmapInfoHeader.biHeight);
	if ((hCursorBitmap = CreateDIBSection(hCursorMemDC, (LPBITMAPINFO)&(RTCursorHead.BitmapInfoHeader), DIB_RGB_COLORS, &lpCursorBits, NULL, 0)) == NULL)	goto Cleanup;
	hCursorGDIObj = SelectObject(hCursorMemDC, hCursorBitmap);
	if ((hCursorGDIObj == NULL) || (hCursorGDIObj == HGDI_ERROR))	goto Cleanup;
	CursorBytes = static_cast<unsigned long long>(cx * cy * (lpRTCursorHead->BitmapInfoHeader.biBitCount / 8));
	memcpy_s((LPBYTE)lpCursorBits, CursorBytes, (LPBYTE)&(lpRTCursorHead->lpData), CursorBytes);

	RTCursorHead.BitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	RTCursorHead.BitmapInfoHeader.biWidth = cx;
	RTCursorHead.BitmapInfoHeader.biHeight = cy;
	RTCursorHead.BitmapInfoHeader.biPlanes = 1;
	RTCursorHead.BitmapInfoHeader.biBitCount = 32;
	RTCursorHead.BitmapInfoHeader.biCompression = BI_RGB;
	RTCursorHead.BitmapInfoHeader.biSizeImage = 0;
	RTCursorHead.BitmapInfoHeader.biXPelsPerMeter = 0;
	RTCursorHead.BitmapInfoHeader.biYPelsPerMeter = 0;
	RTCursorHead.BitmapInfoHeader.biClrUsed = 0;
	RTCursorHead.BitmapInfoHeader.biClrImportant = 0;
	if ((hTextBitmap = CreateDIBSection(hTextMemDC, (LPBITMAPINFO)&(RTCursorHead.BitmapInfoHeader), DIB_RGB_COLORS, &lpTextBits, NULL, 0)) == NULL)	goto Cleanup;
	hTextGDIObj = SelectObject(hTextMemDC, hTextBitmap);
	if ((hTextGDIObj == NULL) || (hTextGDIObj == HGDI_ERROR))	goto Cleanup;

	rc.left = cx / 3 + 2;	rc.right = rc.left + cx * 2 / 3;	rc.top = cy / 3;	rc.bottom = rc.top + cy * 2 / 3 + 2;
	iXSize = rc.right - rc.left;
	vAdjustFontXPosition(dwIMEMode, lpszIMEMode, &iXSize, &rc);
	if (!TextDraw(hTextMemDC, rc, lpszIMEMode, dwRGB, lpszFontFace, FW_NORMAL, FALSE))	goto Cleanup;

	ReverseDataTopDown((LPDWORD)lpTextBits, cx, cy);
	MakeAlphaBlend((LPDWORD)lpTextBits, cx, cy, dwRGB);

	if (!BitBlt(hCursorMemDC, 0, 0, cx, cy, hTextMemDC, 0, 0, SRCINVERT))	goto Cleanup;

	memcpy_s(lpMakeCursorData, dwResSize, lpRTCursorHead, sizeof(RTCURSORHEAD));
	memcpy_s(&(lpMakeCursorData->lpData), CursorBytes, lpCursorBits, CursorBytes);

	bRet = TRUE;

Cleanup:
	if (hCursorGDIObj)	DeleteObject(hCursorGDIObj);
	if (hCursorBitmap)	DeleteObject(hCursorBitmap);
	if (hCursorMemDC)	DeleteDC(hCursorMemDC);
	if (hTextGDIObj)	DeleteObject(hTextGDIObj);
	if (hTextBitmap)	DeleteObject(hTextBitmap);
	if (hTextMemDC)		DeleteDC(hTextMemDC);
	if (hDC)			DeleteDC(hDC);
	return bRet;
}

//
// _FindResource()
//
LPVOID		CCursorSub::_FindResource(HMODULE hModule, int iResID, LPCTSTR ResType, LPDWORD lpdwResSize)
{
	if (!hModule)	return NULL;
	
	HRSRC	hRes = NULL;
	HGLOBAL hResLoad = NULL;
	LPVOID	lpVOID = NULL;

	if ((hRes = FindResource(hModule, MAKEINTRESOURCE(iResID), ResType)) == NULL)	goto Cleanup;
	if ((hResLoad = LoadResource(hModule, hRes)) == NULL)	goto Cleanup;
	if (lpdwResSize)	if ((*lpdwResSize = SizeofResource(hModule, hRes)) == 0)	goto Cleanup;
	if ((lpVOID = LockResource(hResLoad)) == NULL)	goto Cleanup;

Cleanup:
	return lpVOID;
}

//
// ReverseDataTopDown()
//
void		CCursorSub::ReverseDataTopDown(LPDWORD lpData, int cx, int cy)
{
	DWORD	data = 0;
	for (int y1 = 0, y2 = cy - 1; y1 < cy / 2; y1++, y2--) {
		for (int x = 0; x < cx; x++) {
			data = lpData[(y2 * cx + x)];
			lpData[(y2 * cx + x)] = lpData[(y1 * cx + x)];
			lpData[(y1 * cx + x)] = data;
		}
	}
}

//
// MakeAlphaBlend()
//
void		CCursorSub::MakeAlphaBlend(LPDWORD lpData, int cx, int cy, COLORREF aRGB)
{
	for (int y = 0; y < cy; y++) {
		for (int x = 0; x < cx; x++) {
			if (*lpData)	*lpData = ((aRGB & 0xff000000) | ((aRGB << 16) & 0x00ff0000)) | ((aRGB) & 0x0000ff00) | ((aRGB >> 16) & 0x000000ff);
			lpData++;
		}
	}
}

//
//class CCursorWindow : public CWindow
//
CCursorWindow::CCursorWindow()
{
	lpszIMEMode = NULL;
	dwTextColor = aRGB(15, 254, 192, 0);
	dwBackColor = aRGB(15, 254, 192, 0);
	lpszFontFace = NULL;
}

CCursorWindow::~CCursorWindow()
{
	lpszIMEMode = NULL;
}

//
// bRegister()
//
BOOL		CCursorWindow::bRegister(HINSTANCE hInstance, LPCTSTR szWindowClassName)
{
	if (hInstance == NULL)	return FALSE;

	//vSetTextColor(dwRGB);

#define CLASSSTYLE (CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE)
	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CLASSSTYLE;
	wcex.lpfnWndProc = NULL;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClassName;
	wcex.hIconSm = NULL;

	if (!bRegisterClassEx(&wcex))	return FALSE;

#define		EX_WINDOWSTYLE	(WS_EX_LAYERED | WS_EX_NOACTIVATE)
#define		WINDOWSTYLE		(WS_POPUP)
	if (!bCreateWindowEx(
		EX_WINDOWSTYLE,
		NULL,
		WINDOWSTYLE,
		0, 0,
		0, 0,
		NULL,
		NULL)
		)	return FALSE;


	if (hGetHWND() == NULL)	return FALSE;
	if (!bShowWindow(SW_HIDE))	return FALSE;
	if (!bUpdateWindow())	return FALSE;
	return TRUE;
}

//
// vSetModeStringAndColor()
// 
VOID		CCursorWindow::vSetModeStringColorFont(LPCTSTR _lpszIMEMode, COLORREF dwRGB, LPCTSTR _lpszFontFace)
{
	lpszIMEMode = (LPTSTR)_lpszIMEMode;
	dwTextColor = dwRGB;
	if (dwRGB & 0x00000100)	dwBackColor = dwRGB & 0xfffffeff;
	else dwBackColor = dwRGB | 0x00000100;
	if (!_lpszFontFace)	lpszFontFace = (LPTSTR)L"Yu Gothic UI";
	else lpszFontFace = (LPTSTR)_lpszFontFace;
}

//
// WndProc()
//
LRESULT CALLBACK CCursorWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_PAINT, Cls_OnPaint);
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// WM_CREATE
// Cls_OnCreate()
//
BOOL		CCursorWindow::Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct) const
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lpCreateStruct);
	return SetLayeredWindowAttributes(hWnd, dwBackColor, (dwBackColor >> 24) & 0x000000ff, LWA_COLORKEY | LWA_ALPHA);
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
void		CCursorWindow::Cls_OnDestroy(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);
}

//
// WM_ERASEBKGND
// Cls_OnEraseBkgnd()
//
BOOL		CCursorWindow::Cls_OnEraseBkgnd(HWND hWnd, HDC hDC)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(hDC);

	return TRUE;
}

//
// WM_PAINT
// Cls_OnPaint()
//
void		CCursorWindow::Cls_OnPaint(HWND hWnd) const
{
	RECT	rcSize{};
	PAINTSTRUCT	ps{};
	HDC hDC = BeginPaint(hWnd, &ps);
	if (hDC != NULL) {
		if (SetLayeredWindowAttributes(hWnd, dwBackColor, (dwBackColor >> 24) & 0x000000ff, LWA_COLORKEY | LWA_ALPHA)) {
			if (GetClientRect(hWnd, &rcSize)) {
				HBRUSH	hBrush = NULL;
				if ((hBrush = CreateSolidBrush(dwBackColor & 0x00ffffff)) != NULL) {								// 背景色を設定
					if (SelectObject(hDC, hBrush) != HGDI_ERROR) {
						if (FillRect(hDC, &rcSize, hBrush) != 0) {
							DWORD	dwUnderline = 0;
							if (GetKeyState(VK_CAPITAL) & 0x0001) dwUnderline = TRUE;	else dwUnderline = FALSE;
							TextDraw(hDC, rcSize, lpszIMEMode, dwTextColor, lpszFontFace, FW_BOLD, dwUnderline);
						}
					}
					DeleteObject(hBrush);
				}
			}
		}
	}
	EndPaint(hWnd, &ps);
	return;
}

//
// TextDraw()
//
BOOL		TextDraw(HDC hDC, RECT rcSize, LPTSTR szIMEMode, COLORREF dwRGB, LPCTSTR szFontFace, int cWeight, DWORD dwUnderline)
{
	BOOL bRet = FALSE;
	if (hDC != NULL) {
		if (SetBkMode(hDC, TRANSPARENT) != 0) {
			COLORREF	dwColorPrev = 0;
			if ((dwColorPrev = SetTextColor(hDC, dwRGB & 0x80ffffff)) != CLR_INVALID) {
				int	iBkModePrev = 0;
				if ((iBkModePrev = SetBkMode(hDC, TRANSPARENT)) != 0) {
					HFONT	hFont = NULL;
					if ((hFont = CreateFont((rcSize.bottom - rcSize.top), (rcSize.right - rcSize.left) / 2, 0, 0, cWeight, FALSE, dwUnderline, FALSE,
						SHIFTJIS_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, (FIXED_PITCH | FF_DONTCARE), szFontFace)) != NULL) {
						HFONT	hFontPrev = NULL;
						if ((hFontPrev = (HFONT)SelectObject(hDC, hFont)) != NULL) {
							if (szIMEMode != NULL) {
								SIZE	sz{};
								int		len = (int)wcsnlen_s(szIMEMode, sizeof(szIMEMode));
								if (GetTextExtentExPoint(hDC, szIMEMode, len, 0, NULL, NULL, &sz)) {
									rcSize.left = rcSize.right - sz.cx;
									if (DrawTextEx(hDC, szIMEMode, -1, &rcSize, DT_RIGHT | DT_SINGLELINE | DT_VCENTER, NULL) != 0) {
										bRet = TRUE;
									}
								}
							}
						}
						DeleteObject(hFont);
					}
					SetBkMode(hDC, iBkModePrev);
				}
				SetTextColor(hDC, dwColorPrev);
			}
		}
	}
	return bRet;
}

//
// vAdjustFontXPosition()
//
VOID		vAdjustFontXPosition(DWORD dwIMEMode,LPCTSTR szMode, LPINT lpiXSize, LPRECT lprc)
{
	int		len = (int)wcsnlen_s(szMode, sizeof(szMode));
	if ((len < 2) && ((dwIMEMode == IMEOFF) || (dwIMEMode == HANEISU_IMEON) || (dwIMEMode == HANKANA_IMEON))) {
		*lpiXSize = (*lpiXSize * 2) / 3;	lprc->left = lprc->left + (*lpiXSize / 3);	lprc->right = lprc->left + *lpiXSize;
	}
}


/* = EOF = */