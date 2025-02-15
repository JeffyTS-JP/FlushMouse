//
// TaskTray.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/03  JeffyTS  	New edit.
// #0001	2023/06/30	JeffyTS		Divided from FlushMouseSub.h

//
// Include
//
#pragma once
#include "pch.h"

//
// Global Data
//

//
// Global Prototype Define
//

//
// Class CTaskTray
// 
class CTaskTray
{
	public:
		explicit CTaskTray(HWND hWnd);
		~CTaskTray();

	public:
		BOOL		bCreateTaskTrayWindow(HWND hWnd, HICON hIcon, LPCTSTR lpszTitle) const;
		BOOL		bReCreateTaskTrayWindow(HWND hWnd) const;
		BOOL		bDestroyTaskTrayWindow(HWND hWnd) const;
		BOOL		bGetTaskTrayWindowRect(HWND hWnd, LPRECT lpRect) const;
		static int	iCheckTaskTrayMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL		bDisplayBalloon(HWND hWnd, DWORD dwInfoFlags, LPCTSTR szInfoTitle, LPCTSTR szInfo);
		BOOL		bModifyToolHints(HWND hWnd, LPCTSTR lpszToolHints) const;

	private:
		static void	Cls_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);
		static void	Cls_OnTaskTrayEx(HWND hWnd, UINT id, UINT uMsg);

	public:

	private:
		UINT		uTaskTrayID;
		UINT		uTaskbarCreatedMessage;
};


/* = EOF = */