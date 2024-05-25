//
// SynTP_Receiver.cpp
//		Copyright (C) 2024 JeffyTS
//
//  SynTP_Receiver.exe
//      
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/03/26  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "SynTP_Receiver.h"
#include "Resource.h"

#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#define _CRTDBG_MAP_ALLOC
#define new DEBUG_CLIENTBLOCK
#endif

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

//
// Define
//
#define	MAX_LOADSTRING				100
#define	WINDOWCLASS					_T("SynTP_Receiver-{B7BA568A-A893-4AF7-9064-C615BDB75C88}")

//
// Struct Define
//

//
// Global Data
//


//
// Local Data
//
static TCHAR			szTitle[] = _T("SynTP_Receiver");
static CSynTP_Receiver	*SynTP_Receiver = NULL;
static int				iPort = 0;

//
// Global Prototype Define
//
int APIENTRY	wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

//
// Local Prototype Define
//
static int		iCheckCmdLine(LPCTSTR lpCmdLine);
static LPCTSTR*	lpCheckCmdLine(LPCTSTR lpCmdLine, int* argc);
static void		vMessageBox(HINSTANCE hInstance, HWND hWnd, UINT uID, UINT uType);

//
//  関数: wWinMain()
//
int APIENTRY	wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(nCmdShow);

#ifdef _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_ASSERTE(_CrtCheckMemory());
#endif

#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL)

	int	iRet = 0;
	if ((iRet = iCheckCmdLine(lpCmdLine)) != 1) {
		vMessageBox(hInstance, NULL, IDS_BADARGS, MessageBoxTYPE);
		return (iRet);
	}
	
	if (iPort == 0) {
		vMessageBox(hInstance, NULL, IDS_BADARGS, MessageBoxTYPE);
		return (-1);
	}

	HWND	hWnd = NULL;
	if ((hWnd = FindWindow(WINDOWCLASS, NULL)) != NULL) {
		SetFocus(GetLastActivePopup(hWnd));
		PostMessage(hWnd, WM_DESTROY, NULL, NULL);
		for (int i = 5; i > 0; i--) {
			Sleep(500);
			if ((hWnd = FindWindow(WINDOWCLASS, NULL)) != NULL) {
				SetFocus(GetLastActivePopup(hWnd));
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);
				if (i == 1) {
					if (hInstance) {
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL)
						vMessageBox(hInstance, NULL, IDS_ALREADYRUN, MessageBoxTYPE);
					}
					return (-1);
				}
			}
		}
	}

	if (SynTP_Receiver == NULL)	SynTP_Receiver = new CSynTP_Receiver;
	if (SynTP_Receiver) {
		if (!SynTP_Receiver->bRegister(hInstance, WINDOWCLASS)) {
			delete SynTP_Receiver;
			SynTP_Receiver = NULL;
			vMessageBox(hInstance, NULL, IDS_CANTRECEIVER, MessageBoxTYPE);
			return (-1);
		}
	}

	MSG		msg{};
	BOOL	bRet = FALSE;
	while (TRUE) {
		try {
			bRet = GetMessage(&msg, NULL, 0, 0);
		}
		catch (...) {
		}
		if ((bRet == 0) || (bRet == (-1))) {
			break;
		}
		else {
			HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SYNTP_RECEIVER));
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	if (SynTP_Receiver)	delete SynTP_Receiver;

	return 0;
}

//
// iCheckCmdLine()
//	Return	0, -1 = Exit Code
//			2     = Continue App
//
static int			iCheckCmdLine(LPCTSTR lpCmdLine)
{
	int			argc = 0;
	LPCTSTR*	lpRet = lpCheckCmdLine(lpCmdLine, &argc);
	if ((lpRet == NULL) && (argc == 0))	return (-1);	// No command line args
	for (int i = 0; i < argc; i++) {
		if (lpRet == NULL)	break;
		if ((*lpRet[i] != L'\0') && (CompareStringOrdinal(lpRet[i], -1, L"/Port", -1, TRUE) == CSTR_EQUAL)) {
			if ((i < argc) && (*lpRet[i + 1] != L'\0')) {
				if ((iPort = _wtoi(lpRet[i + 1])) == 0) {
					return (-1);
				}
			}
		}
	}
	return (1);
}

//
// lpCmdLineCheck()
//
static LPCTSTR* lpCheckCmdLine(LPCTSTR lpCmdLine, int* argc)
{
	if ((lpCmdLine == NULL) || (argc == NULL))	return NULL;

	if (lpCmdLine[0] == L'\0') {
		*argc = 0;
		return NULL;
	}
	LPCTSTR*	lpRet = (LPCTSTR *)CommandLineToArgvW((LPCWSTR)lpCmdLine, argc);
	if (lpRet != NULL)	return lpRet;
	return NULL;
}

//
// class CSynTP_Receiver : private CWindow, private CSynTP
//
CSynTP_Receiver::CSynTP_Receiver()
	:	SynTP()
{
	
}

CSynTP_Receiver::~CSynTP_Receiver()
{
}

//
// bRegister()
//
BOOL		CSynTP_Receiver::bRegister(HINSTANCE hInstance, LPCTSTR szWindowClassName)
{
	if (hInstance == NULL)	return FALSE;

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
// WndProc()
//
LRESULT CALLBACK CSynTP_Receiver::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
			HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
			HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		default:
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// WM_CREATE
// Cls_OnCreate()
//
BOOL		CSynTP_Receiver::Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lpCreateStruct);

#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL)

	if (SynTP == NULL) {
		SynTP = new CSynTP;
		if (SynTP) {
			if (SynTP->bStartReceiver(hWnd, iPort)) {
				return TRUE;
			}
			delete SynTP;
			SynTP = NULL;
			vMessageBox((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL, IDS_CANTRECEIVER, MessageBoxTYPE);
		}
	}
	return FALSE;
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
void		CSynTP_Receiver::Cls_OnDestroy(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);

	if (SynTP) {
		SynTP->vStopReceiver();
		delete SynTP;
		SynTP = NULL;
	}
	PostQuitMessage(0);
}

// 
// vMessageBox()
//
static void		vMessageBox(HINSTANCE hInstance, HWND hWnd, UINT uID, UINT uType)
{
	TCHAR	lpText[MAX_LOADSTRING];
	try {
		throw LoadString(hInstance, uID, lpText, MAX_LOADSTRING);
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



/* = EOF = */