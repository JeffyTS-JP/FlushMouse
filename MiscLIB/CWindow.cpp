//
// CWindow.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/02/13  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "CWindow.h"

//
// Define
//
#define MAX_LOADSTRING 100

//
//class CWindow
//
CWindow::CWindow()
	: _hInstance(NULL), _hWnd(NULL), _lpWndClass(NULL), _lpWndClassEx(NULL)
{
}

CWindow::~CWindow()
{
	if (_lpWndClassEx)	delete [] _lpWndClassEx;
	_lpWndClassEx = NULL;
	if (_lpWndClass)	delete [] _lpWndClass;
	_lpWndClass = NULL;

	_hInstance = NULL;
	_hWnd = NULL;
}

//
// bRegisterClass()
//
BOOL	CWindow::bRegisterClass(_In_ LPWNDCLASS lpWndClass)
{
	errno_t err = 0;

	if (lpWndClass == NULL)	return FALSE;
	if (lpWndClass->hInstance == NULL)	return FALSE;
	_hInstance = lpWndClass->hInstance;

	_lpWndClass = new WNDCLASS[sizeof(WNDCLASS)];
	if (_lpWndClass == NULL)	goto error_cleanup;
	ZeroMemory(_lpWndClass, sizeof(WNDCLASS));
	if ((err = memcpy_s(_lpWndClass, sizeof(WNDCLASS), lpWndClass, sizeof(WNDCLASS))) != 0)	goto error_cleanup;
	_lpWndClass->lpfnWndProc = (WNDPROC)_WndProc;

	if (RegisterClass(_lpWndClass) == 0) {
		err = GetLastError();
		if (err == ERROR_CLASS_ALREADY_EXISTS)		return TRUE;
		else goto error_cleanup;
	}
	return TRUE;

error_cleanup:
	if (_lpWndClass)	delete [] _lpWndClass;
	return FALSE;
}

//
// bRegisterClassEx()
//
BOOL	CWindow::bRegisterClassEx(_In_ LPWNDCLASSEX lpWndClassEx)
{
	errno_t err = 0;

	if (lpWndClassEx == NULL)	return FALSE;
	if (lpWndClassEx->hInstance == NULL)	return FALSE;
	_hInstance = lpWndClassEx->hInstance;

	_lpWndClassEx = new WNDCLASSEX[sizeof(WNDCLASSEX)];
	if (_lpWndClassEx == NULL)	goto error_cleanup;
	ZeroMemory(_lpWndClassEx, sizeof(WNDCLASSEX));
	if ((err = memcpy_s(_lpWndClassEx, sizeof(WNDCLASSEX), lpWndClassEx, sizeof(WNDCLASSEX))) != 0)	goto error_cleanup;
	_lpWndClassEx->lpfnWndProc = (WNDPROC)_WndProc;

	if (RegisterClassEx(_lpWndClassEx) == 0) {
		err = GetLastError();
		if (err == ERROR_CLASS_ALREADY_EXISTS)		return TRUE;
		else goto error_cleanup;
	}
	return TRUE;

error_cleanup:
	if (_lpWndClassEx)	delete [] _lpWndClassEx;
	return FALSE;
}

//
// bCreateWindow()
//
BOOL	CWindow::bCreateWindow(_In_opt_ LPCTSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y,
	_In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu)
{
	if (_lpWndClass == NULL)	return FALSE;
	if (_lpWndClass->lpszClassName == NULL)	return FALSE;

	if ((_hWnd = CreateWindow(_lpWndClass->lpszClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, _hInstance, this)) != NULL) {
		return TRUE;
	}
	return FALSE;
}

//
// bCreateWindowEx()
//
BOOL	CWindow::bCreateWindowEx(_In_ DWORD dwExStyle, _In_opt_ LPCTSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y,
	_In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu)
{
	if (_lpWndClassEx == NULL)	return FALSE;
	if (_lpWndClassEx->lpszClassName == NULL)	return FALSE;

	if ((_hWnd = CreateWindowEx(dwExStyle, _lpWndClassEx->lpszClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, _hInstance, this)) != NULL) {
		return TRUE;
	}
	return FALSE;
}

//
// _WndProc()
//
LRESULT CALLBACK CWindow::_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CWindow* This = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (This == NULL) {
		if (message == WM_CREATE) {
			LPCREATESTRUCT lpCreate = reinterpret_cast<LPCREATESTRUCT>(lParam);
			This = reinterpret_cast<CWindow*>(lpCreate->lpCreateParams);
			if (This != NULL) {
				This->vSetWindowLongPtr(hWnd);
			}
		}
	}
	if (This != NULL) {
		if (This->_hWnd != hWnd)	return DefWindowProc(hWnd, message, wParam, lParam);
		LRESULT	l = 0;
		try {
			l = This->WndProc(hWnd, message, wParam, lParam);
		}
		catch (...) {

		}
		return l;
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

//
// SetWindowLongPtr()
//
void		CWindow::vSetWindowLongPtr(HWND hWnd)
{
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	_hWnd = hWnd;
}

//
// WndProc()
//
LRESULT CWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const CWindow* This = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (This != NULL) {
		switch (message) {
			case WM_CREATE:
				break;
			case WM_DESTROY:
				break;
			default:
				break;
		}
	}
	else {
		if (message == WM_CREATE) {
			This = reinterpret_cast<CWindow*>((reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams));
		}
		if (This != NULL) {
			vSetWindowLongPtr(hWnd);
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// bUpdateWindow()
//
BOOL		CWindow::bUpdateWindow() const
{
	if (_hWnd == NULL)	return FALSE;

	if (UpdateWindow(_hWnd))	return TRUE;
	return FALSE;
}

//
// bShowWindow()
//
BOOL		CWindow::bShowWindow(_In_ int nCmdShow) const
{
	if (_hWnd == NULL)	return FALSE;

	ShowWindow(_hWnd, nCmdShow);
	return TRUE;
}

//
// bSetWindowText()
//
BOOL		CWindow::bSetWindowText(_In_opt_ LPCTSTR szTitle) const
{
	if (_hWnd == NULL)		return FALSE;
	if (szTitle == NULL)	return FALSE;
	if (SetWindowText(_hWnd, szTitle)) return TRUE;
	return FALSE;
}

//
// bSetWindowPos()
//
BOOL		CWindow::bSetWindowPos(_In_opt_ HWND hWndInsertAfter, _In_ int X, _In_ int Y, _In_ int cx, _In_ int cy,_In_  UINT uFlags) const
{
	if (_hWnd == NULL)		return FALSE;
	if (SetWindowPos(_hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags))	return TRUE;
	return FALSE;
}
//
// bInvalidateRect()
//
BOOL		CWindow::bInvalidateRect(_In_opt_ LPRECT lpRect, _In_ BOOL bErase) const
{
	if (_hWnd == NULL)		return FALSE;
	if (InvalidateRect(_hWnd, lpRect, bErase))	return TRUE;
	return FALSE;
}

//
// bSetWndThreadPriority()
//
BOOL		CWindow::bSetWndThreadPriority(int nPriority) const
{
	if (_hWnd == NULL)		return FALSE;
	if (SetThreadPriority(GetCurrentThread(), nPriority) != 0)	return TRUE;
	return FALSE;
}

/* = EOF = */
