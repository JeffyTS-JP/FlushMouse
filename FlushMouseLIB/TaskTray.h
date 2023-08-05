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
extern HWND		hAboutDlg;
extern HWND		hSettingDlg;
extern BOOL		bIMEInConverting;

//
// Global Prototype Define
//
extern BOOL 	bCreateTaskTrayWindow(HWND hWnd, HICON hIcon, LPCTSTR lpszTitile);
extern BOOL		bReCreateTaskTrayWindow(HWND hWnd, UINT message);
extern BOOL		bDestroyTaskTrayWindow(HWND hWnd);
extern BOOL		bGetTaskTrayWindowRect(HWND hWnd, LPRECT lpRect);
extern void		Cls_OnTaskTrayEx(HWND hWnd, UINT id, UINT uMsg);
extern void		vMessageBox(HWND hWnd, UINT uID, UINT uType);
extern VOID		vAboutDialog(HWND hWnd);
extern VOID		vSettingDialog(HWND hWnd);
extern VOID		vGetSetProfileData();


/* = EOF = */