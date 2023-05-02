// FlushMouse.cpp
//		for Windows10/11 Win32 API msi Packaage App.
//		Copyright (C) 1993 JeffyTS
//
//  FlushMouse.exe
//      	マウスカーソルにIMEの状態を表示する
//      
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	1993/11/17  JeffyTS  	New edit.
// #0001	2022/02/04  JeffyTS		Visual Studio 2022 (C++)へ移植
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
TCHAR		szFlushMouse64[] = _T("FlushMouseDLL.dll");
TCHAR		szFlushMouse32[] = _T("FlushMouse32.exe");

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
static INT_PTR CALLBACK AboutDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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

#define	RESOURCEFILE		_T("FlushMouse.exe")
	// Load Resource
	Resource = new CResource(RESOURCEFILE);
	if (Resource->hLoad() == NULL) {
		return (-1);
	}
	if (!bWinMain(hInstance, hPrevInstance))		return (-1);

	// メイン メッセージ ループ:
	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0)) {
		HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLUSHMOUSE));
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

// 
// vMessageBox()
//
void vMessageBox(HWND hWnd, UINT uID, UINT uType)
{
	TCHAR       lpText[MAX_LOADSTRING];
	if (LoadString(Resource->hLoad(), uID, lpText, MAX_LOADSTRING) != 0) {
		MessageBox(hWnd, lpText, szTitle, uType);
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
				cx = GetSystemMetrics(SM_CXSCREEN);								// スクリーンサイズの取得
				cy = GetSystemMetrics(SM_CYSCREEN);
				GetWindowRect(hDlg, (LPRECT)&rc);								// 子ウィンドウのスクリーン座標を取得
				rc.right = rc.right - rc.left + 1;								// 中央の計算
				rc.bottom = rc.bottom - rc.top + 1;
				rc.left = (cx - rc.right) / 2;
				rc.top = (cy - rc.bottom) / 2;
				MoveWindow(hDlg, rc.left, rc.top, rc.right, rc.bottom, TRUE);	// 移動
				HICON	hIcon = NULL;
				hIcon = (HICON)LoadImage(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 16, 16, 0);
				SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);		// Iconの設定
				LPTSTR	lpszVersion = new TCHAR[128];
				ZeroMemory(lpszVersion, (sizeof(TCHAR) * 128));
				_sntprintf_s(lpszVersion, 128, _TRUNCATE, _T("%d.%d.%d.%d"), MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION, REVISON_VERSION);
				SetDlgItemText(hDlg, IDC_VERSION, lpszVersion);					// Versionの設定
				delete[]	lpszVersion;
			}
			return (INT_PTR)TRUE;
		case WM_CTLCOLORDLG:
			return (INT_PTR)GetStockObject(WHITE_BRUSH);						// 背景の色
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT:
			SetBkMode(((HDC)wParam), TRANSPARENT);
			SetTextColor(((HDC)wParam), RGB(0, 0, 0));							// 文字の色
			return (INT_PTR)GetStockObject(WHITE_BRUSH);						// 背景の色
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