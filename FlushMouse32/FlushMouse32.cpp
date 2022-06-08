// FlushMouse32.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/04/06  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "FlushMouse32.h"
#include "Resource.h"
#include "..\FlushMouseDLL32\FlushMouseDll32.h"
#include "..\FlushMouseDLL32\MouseHookDll32.h"
#include "..\FlushMouseDLL32\KeyboardHookDll32.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif // _DEBUG

//
// Define
//
#define MAX_LOADSTRING 100

//
// Struct Define
//

//
// Global Data
//

//
// Local Data
//
static TCHAR		szTitle[MAX_LOADSTRING]{};			// タイトル バーのテキスト
static TCHAR		szWindowClass[MAX_LOADSTRING]{};	// メイン ウィンドウ クラス名
static HINSTANCE	hInst = NULL;						// 現在のインターフェイス
static HWND			hParentWnd = NULL;					// 親のWindow Handle

//
// Global Prototype Define
//

//
// Local Prototype Define
//
static ATOM             MyRegisterClass(HINSTANCE);
static BOOL             InitInstance(HINSTANCE, int);
static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// Handler
static BOOL				Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
static void				Cls_OnDestroy(HWND hWnd);
#define WM_SYSINUPUTLANGCHANGEEX	(WM_USER + WM_INPUTLANGCHANGE)
#define	HANDLE_WM_SYSINUPUTLANGCHANGEEX(hWnd, wParam, lParam, fn) ((fn)((hWnd), (BOOL)(wParam)), 0L)
static void				Cls_OnSysInputLangChangepEx(HWND hWnd, BOOL bEnableEPHelper);

// Sub
static LONG WINAPI		lExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
static void				vMessageBox(HWND hWnd, UINT uID, UINT uType);

//
// wWinMain()
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)					// MessageBox style
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif	
	SetUnhandledExceptionFilter(&lExceptionFilter);				// 例外ハンドラの登録

	if (*lpCmdLine == _T('\0')) {
		hParentWnd = NULL;
		vMessageBox(NULL, IDS_NOTFORKBY64, MessageBoxTYPE);		// 不正起動のためメッセージを表示
		return (-1);
	}
	else {
		unsigned long long ll = 0;
		if ((ll = _tstoll(lpCmdLine)) == 0) return (-1);
		hParentWnd = (HWND)ll;
		if (hParentWnd != FindWindow(_T("FlushMouse"), NULL)) {	// 渡された値をチェック
			vMessageBox(NULL, IDS_NOTFORKBY64, MessageBoxTYPE);	// 不正起動のためメッセージを表示
			return (-1);
		}
	}

	if (LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) return (-1);
	if (LoadString(hInstance, IDC_FLUSHMOUSE32, szWindowClass, MAX_LOADSTRING) == 0) return (-1);

	if (!hPrevInstance) {						                // ほかのインスタンスが実行中か?
		if (!MyRegisterClass(hInstance)) {		                // 共通の初期化処理
			return (-1);										// 初期化に失敗した場合はエラー終了
		}
	}

	HWND	    hWnd = NULL;
	if ((hWnd = FindWindow(szWindowClass, NULL)) != NULL) {
		SetFocus(GetLastActivePopup(hWnd));
		PostMessage(hWnd, WM_DESTROY, NULL, NULL);
		for (int i = 3; i > 0; i--) {
			Sleep(500);
			if (FindWindow(szWindowClass, NULL) != NULL) {
				if (i == 1) {
					vMessageBox(NULL, IDS_ALREADYRUN, MessageBoxTYPE);
					return (-1);
				}
			}
			else break;
		}
	}

	if (!InitInstance(hInstance, nCmdShow)) {                   // Window作成
		return (-1);
	}
	
	MSG		msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLUSHMOUSE32));
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

//
// MyRegisterClass()
//
static ATOM MyRegisterClass(HINSTANCE hInstance)
{
#define CLASSSTYLE CS_HREDRAW | CS_VREDRAW // クラススタイル

	WNDCLASSEX wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CLASSSTYLE;                                                // クラススタイル
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;                                                    // クラスの補足データなし
	wcex.cbWndExtra = 0;                                                    // ウィンドウの補足データなし
	wcex.hInstance = hInstance;                                             // クラスのウィンドウハンドル
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FLUSHMOUSE32));    // アイコンハンドル
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);								// マウスカーソルハンドル
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);               // ウィンドウ背景色
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_FLUSHMOUSE32);                  // デフォルトメニュー名
	wcex.lpszClassName = szWindowClass;                                     // このウインドウクラスにつける名前
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));    // 16×16の小さいサイズのアイコン

	return RegisterClassEx(&wcex);
}

//
// InitInstance(HINSTANCE, int)
//
static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	UNREFERENCED_PARAMETER(nCmdShow);
#define		WINDOWSTYLE		WS_OVERLAPPEDWINDOW  | WS_HSCROLL | WS_VSCROLL // | WS_VISIBLE // WS_MINIMIZE | WS_SYSMENU
	hInst = hInstance;							// グローバル変数にインスタンス ハンドルを格納する
	HWND	hWnd = NULL;						// メインウィンドウのハンドル
	hWnd = CreateWindowEx(
							WS_DISABLED,		// Disabled Window
							szWindowClass,      // RegisterClass()呼び出しを参照
							szTitle,            // Title barのテキスト
							WINDOWSTYLE,        // Window style
							0, 0,               // 水平・垂直位置
							0, 0,               // 幅・高さ
							NULL,				// 親オーバーラップウィンドウ
							NULL,				// ウィンドウクラスのメニューを使う
							hInstance,			// 所有インスタンス
							NULL);				// ポインタは不要
	if (!hWnd) {
		return FALSE;							// ウィンドウを作成できなかったときはFALSEを返す
	}
	return TRUE;
}

//
// WndProc(HWND, UINT, WPARAM, LPARAM)
//
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_SYSINUPUTLANGCHANGEEX, Cls_OnSysInputLangChangepEx);
	default:
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// WM_CREATE
// Cls_OnCreate()
//
static BOOL Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	UNREFERENCED_PARAMETER(lpCreateStruct);
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)						// MessageBox style
	// Register Hook
	if (!bMouseHookSet32(hParentWnd)) {	
		vMessageBox(hWnd, IDS_NOTREGISTEHOOK, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}
	if (!bKeyboardHookLLSet32(hParentWnd)) {
		vMessageBox(hWnd, IDS_NOTREGISTEHOOK, MessageBoxTYPE);
		PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
		return FALSE;
	}
	return TRUE;
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
static void Cls_OnDestroy(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);
	EXCEPTION_POINTERS ExceptionInfo{};
	lExceptionFilter(&ExceptionInfo);								// Exceptionを呼んで終了処理させる

	PostQuitMessage(0);
}

//
// Cls_OnSysInputLangChangepEx()
//
static void		Cls_OnSysInputLangChangepEx(HWND hWnd, BOOL bEPHelper)
{
	UNREFERENCED_PARAMETER(hWnd);
	if (!bSetEnableEPHelperLL(bEPHelper)) {
		DestroyWindow(hWnd);
	}
}

//
// lExceptionFilter()
//
static LONG WINAPI lExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	UNREFERENCED_PARAMETER(pExceptionInfo);
	bKeyboardHookLLUnset32();										// Keyboard Hook LL の解除
	bMouseHookUnset32();											// Mouse Hook の解除
	return EXCEPTION_CONTINUE_SEARCH;
}

//
// vMessageBox()
//
static void vMessageBox(HWND hWnd, UINT uID, UINT uType)
{
	TCHAR       lpText[MAX_LOADSTRING];
	if (LoadString(hInst, uID, lpText, MAX_LOADSTRING) != 0) {
		MessageBox(hWnd, lpText, szTitle, uType);
	}
}

