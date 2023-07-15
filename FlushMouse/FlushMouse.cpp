//
// FlushMouse.cpp
//		for Windows10/11 Win32 API msi Packaage App.
//		Copyright (C) 1993 JeffyTS
//
//  FlushMouse.exe
//      	マウスカーソルにIMEの状態を表示する
//      
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		1993/11/17  JeffyTS  	New edit.
// #0001		2022/02/04  JeffyTS		Visual Studio 2022 (C++)へ移植
//

//
// Include
//
#pragma once
#include "pch.h"

#include "FlushMouse.h"
#include "..\version.h"
#include "FlushMouseLIB.h"
#include "Resource.h"
#include "TaskTray.h"
#include "..\FlushMouseDLL\EventlogDll.h"

#if defined _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#include <crtdbg.h>
#define new DEBUG_CLIENTBLOCK
#endif

//
// Define
//

//
// Struct Define
//

//
// Global Data
//
// Hook
TCHAR		szFlushMouseDLL[] = FLUSHMOUSE_DLL;
TCHAR		szFlushMouse32[] = FLUSHMOUSE32_EXE;

//
// Local Data
//

//
// Global Prototype Define
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

//
// Local Prototype Define
//
static BOOL		bSetHeapInformation();
static INT_PTR CALLBACK	AboutDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

//
//  関数: wWinMain()
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

#if defined _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	HANDLE	hHandle = GetCurrentProcess();
	if (!SetPriorityClass(hHandle, NORMAL_PRIORITY_CLASS)) {
		return (-1);
	}

	if (!bSetHeapInformation())	return (-1);

	if (*lpCmdLine == _T('\0')) {
		// NOP
	}
	else {
		int	iRet = 0;
		if ((iRet = CompareStringOrdinal(lpCmdLine, -1, L"/quit", -1, TRUE)) != 0) {
			if (iRet == CSTR_EQUAL) {
				bReportEvent(MSG_DONE_FLUSHMOUSE, Installer_CATEGORY);
				HWND	hWnd = NULL;
				if ((hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL)) != NULL) {
					SetFocus(GetLastActivePopup(hWnd));
					PostMessage(hWnd, WM_DESTROY, NULL, NULL);
					for (int i = 3; i > 0; i--) {
						Sleep(500);
						if ((hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL)) != NULL) {
							SetFocus(GetLastActivePopup(hWnd));
							PostMessage(hWnd, WM_DESTROY, NULL, NULL);
							if (i == 1) {
								return (-1);
							}
						}
						else return 0;
					}
				}
			}
		}
		if (hHandle != NULL)	CloseHandle(hHandle);
		return 0;
	}

	// Load Resource
	Resource = new CResource(FLUSHMOUSE_EXE);
	if (Resource->hLoad() == NULL) {
		return (-1);
	}
	if (!bWinMain(hInstance, hPrevInstance))		return (-1);

	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0)) {
		HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLUSHMOUSE));
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	if (Resource)	delete	Resource;
	return (int)msg.wParam;
}

//
// bSetHeapInformation()
//
BOOL	bSetHeapInformation()
{
	HANDLE	hHeap = NULL;
	if ((hHeap = GetProcessHeap()) != NULL) {
		ULONG	HeapInformation = 2;
		if (!HeapSetInformation(hHeap, HeapCompatibilityInformation, &HeapInformation, sizeof(ULONG))) {
			_Post_equals_last_error_ DWORD err = GetLastError();
			if (err != ERROR_INVALID_PARAMETER) {
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}

// 
// vMessageBox()
//
void vMessageBox(HWND hWnd, UINT uID, UINT uType)
{
	TCHAR       lpText[MAX_LOADSTRING];
	try {
		throw LoadString(Resource->hLoad(), uID, lpText, MAX_LOADSTRING);
	}
	catch (int i) {
		if (i != 0) {
			try {
				throw MessageBox(hWnd, lpText, szTitle, uType);
			}
			catch (int) {
				return;
			}
			catch (...) {
				return;					// error
			}
		}
	}
	catch (...) {
		return;							// error
	}
}

//
// vAboutDialog()
//
VOID		vAboutDialog(HWND hWnd)
{
	if (hAboutDlg != NULL) {
		SetForegroundWindow(hAboutDlg);
		return;
	}
	DialogBox(Resource->hLoad(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlg);
}

//
// AboutDlg()
//
static INT_PTR CALLBACK AboutDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
		case WM_INITDIALOG:
			hAboutDlg = hDlg;
			{
				RECT		rc{};
				int		cx = 0, cy = 0;
				cx = GetSystemMetrics(SM_CXSCREEN);
				cy = GetSystemMetrics(SM_CYSCREEN);
				GetWindowRect(hDlg, (LPRECT)&rc);
				rc.right = rc.right - rc.left + 1;
				rc.bottom = rc.bottom - rc.top + 1;
				rc.left = (cx - rc.right) / 2;
				rc.top = (cy - rc.bottom) / 2;
				MoveWindow(hDlg, rc.left, rc.top, rc.right, rc.bottom, TRUE);
				HICON	hIcon = NULL;
				hIcon = (HICON)LoadImage(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 16, 16, 0);
				SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
				LPTSTR	lpszVersion = new TCHAR[MAX_LOADSTRING];
				ZeroMemory(lpszVersion, (sizeof(TCHAR) * MAX_LOADSTRING));
				if (lpszVersion) {
					ZeroMemory(lpszVersion, (sizeof(TCHAR) * MAX_LOADSTRING));
					_sntprintf_s(lpszVersion, MAX_LOADSTRING, _TRUNCATE, _T("%d.%d.%d.%d"), MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION, REVISON_VERSION);
					SetDlgItemText(hDlg, IDC_VERSION, lpszVersion);
					delete[]	lpszVersion;
				}
				else return (INT_PTR)FALSE;
			}
			return (INT_PTR)TRUE;
		case WM_CTLCOLORDLG:
			return (INT_PTR)GetStockObject(WHITE_BRUSH);
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT:
			SetBkMode(((HDC)wParam), TRANSPARENT);
			SetTextColor(((HDC)wParam), RGB(0, 0, 0));
			return (INT_PTR)GetStockObject(WHITE_BRUSH);
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				hAboutDlg = NULL;
				return (INT_PTR)TRUE;
			}
	}
	return (INT_PTR)FALSE;
}

/* = EOF = */