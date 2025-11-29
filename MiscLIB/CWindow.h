//
// CWindow.h
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

//
// Class CWindow
//
class CWindow
{
	public:
		CWindow();
		virtual ~CWindow();

	public:
		BOOL		bRegisterClass(_In_ LPWNDCLASS lpWndClass);
		BOOL		bRegisterClassEx(_In_ LPWNDCLASSEX lpWndClassEx);
		BOOL		bCreateWindow(_In_opt_ LPCTSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y,
			_In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu);
		BOOL		bCreateWindowEx(_In_ DWORD dwExStyle, _In_opt_ LPCTSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y,
			_In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu);
		virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HINSTANCE	hGetInstance() const {return _hInstance; }
		HWND		hGetHWND() const { return _hWnd; }
		BOOL		bUpdateWindow() const;
		BOOL		bShowWindow(_In_ int nCmdShow) const;
		BOOL		bSetWindowText(_In_opt_ LPCTSTR szTitle) const;
		BOOL		bSetWindowPos(_In_opt_ HWND hWndInsertAfter, _In_ int X, _In_ int Y, _In_ int cx, _In_ int cy,_In_  UINT uFlags) const;
		BOOL		bInvalidateRect(_In_opt_ LPRECT lpRect, _In_ BOOL bErase) const;
		BOOL		bSetWndThreadPriority(int nPriority) const;

	private:
		void		vSetWindowLongPtr(HWND hWnd);
		static LRESULT CALLBACK		_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HINSTANCE		_hInstance;
		HWND			_hWnd;
		LPWNDCLASS		_lpWndClass;
		LPWNDCLASSEX	_lpWndClassEx;
};

/* = EOF = */
