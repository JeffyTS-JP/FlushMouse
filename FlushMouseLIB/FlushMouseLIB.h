// FlushMouseLIB.h
//		Copyright (C) 1993 - 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/07/09  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "Profile.h"
#include "Cursor.h"
#include "FlushMouseSub.h"
#include "CResource.h"
#include "TaskTray.h"
#include "SynTP.h"

//
// Define
//

//
// Global Prototype Define
//
extern BOOL			bWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_opt_ LPWSTR lpCmdLine, _In_ int nCmdShow);
extern int			iCheckCmdLine(LPCTSTR lpCmdLine);
extern VOID			vDestroyWindow(HWND hWnd);
extern BOOL			bStartThreadHookTimer(HWND hWnd);
extern VOID			vStopThreadHookTimer(HWND hWnd);

extern void			Cls_OnLButtonDownEx(HWND hWnd, int x, int y, HWND hForeground);
extern void			Cls_OnLButtonUpEx(HWND hWnd, int x, int y, HWND hForeground);

extern VOID			vMessageBox(HWND hWnd, UINT uID, UINT uType);
extern VOID			vAboutDialog(HWND hWnd);
extern VOID			vIMEModeDialog(HWND hWnd);
extern VOID			vSynTPHelperDialog(HWND hWnd);
extern VOID			vSettingDialog(HWND hWnd);
extern VOID			vSettingDialogApply();
extern VOID			vSettingDialogClose();

//
// Global Data
//
extern TCHAR		szFlushMouseDLL[];
extern TCHAR		szFlushMouse32[];

extern TCHAR		szTitle[];
extern HWND			hMainWnd;

extern CProfile		*Profile;
extern CCursor		*Cursor;
extern CResource	*Resource;
extern CIME			*Cime;
extern CTaskTray	*TaskTray;
extern CSynTP		*SynTP;

extern BOOL			bIMEInConverting;

/* = EOF = */