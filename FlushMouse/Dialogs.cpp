//
// Dialogs.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/03  JeffyTS  	New edit.
// #0001	2023/11/27	JeffyTS		Divided from TaskTray.cpp
//

// Include
//
#pragma once
#include "pch.h"
#include "Dialogs.h"
#include "..\version.h"
#include "..\FlushMouseLIB\Resource.h"
#include "..\FlushMouseLIB\CommonDef.h"
#include "..\FlushMouseLIB\FlushMouseLIB.h"
#include "..\FlushMouseLIB\TaskTray.h"

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
// Global Data
//

//
// Local Data
//

//
// Local Prototype Define
//
static INT_PTR CALLBACK	AboutDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK SettingDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK SynTPHelperDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

static BOOL		bInitSynTPHelperDlg(HWND hDlg, DWORD dwSynTPHelper);
static BOOL		bSetSynTPHelperProfile(HWND hDlg);

static BOOL		bInitDlgCenter(HWND hDlg, HICON hIcon, DWORD dwICC);
static BOOL		bSetCheckDlgButton(HWND hDlg, int iIDButton, BOOL bCheck);
static VOID		vSetTrackbar(HWND hDlg, int iIDTrackbar, int iRangeMin, int iRangeMax, int iTickFreq, int iPos, int iPageSize);
static BOOL		bGetDlgButtonChecked(HWND hDlg, int iIDButton);
static DWORD	dwGetTrackbarPos(HWND hDlg, int iIDTrackbar);

// 
// vMessageBox()
//
void vMessageBox(HWND hWnd, UINT uID, UINT uType)
{
	TCHAR	lpText[MAX_LOADSTRING];
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
				return;	
			}
		}
	}
	catch (...) {
		return;
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
				HICON	hIcon = NULL;
				hIcon = (HICON)LoadImage(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 16, 16, 0);
				SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
				bInitDlgCenter(hDlg, hIcon, (ICC_BAR_CLASSES | ICC_HOTKEY_CLASS));
				LPTSTR	lpszVersion = new TCHAR[MAX_LOADSTRING];
				if (lpszVersion) {
					ZeroMemory(lpszVersion, (sizeof(TCHAR) * MAX_LOADSTRING));
					_sntprintf_s(lpszVersion, MAX_LOADSTRING, _TRUNCATE, _T("%d.%d.%d.%d"), MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION, REVISION_VERSION);
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

//
// vSynTPHelperDialog()
//
VOID		vSynTPHelperDialog(HWND hWnd)
{
	if (hSynTPHelperDlg != NULL) {
		SetForegroundWindow(hSynTPHelperDlg);
		return;
	}
	DialogBox(Resource->hLoad(), MAKEINTRESOURCE(IDD_SYNTPHELPERBOX), hWnd, SynTPHelperDlg);
}

//
// SyTPHelperDlg()
//
static INT_PTR CALLBACK SynTPHelperDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	if (Profile == NULL)	return (INT_PTR)FALSE;

	switch (message) {
	case WM_INITDIALOG:
		hSynTPHelperDlg = hDlg;
		{
			HICON	hIcon = NULL;
			hIcon = (HICON)LoadImage(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 16, 16, 0);
			SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			bInitDlgCenter(hDlg, hIcon, (ICC_BAR_CLASSES | ICC_HOTKEY_CLASS));
			bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
		}
		return (INT_PTR)TRUE;
	case WM_CTLCOLORDLG:
		return (INT_PTR)GetStockObject(WHITE_BRUSH);
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLOREDIT:
	{
		SetBkMode(((HDC)wParam), TRANSPARENT);
		SetTextColor(((HDC)wParam), RGB(0, 0, 0));
		HWND	hWnd = NULL;
		if ((hWnd = GetDlgItem(hDlg, IDC_EDIT1)) == (HWND)lParam) {
			if (!IsWindowEnabled(hWnd)) {
				HDC	hDC = GetDC(hWnd);
				if (hDC) {
					HGDIOBJ hObj = SelectObject(hDC, CreateSolidBrush((COLORREF)0x00f0f0f0));
					DeleteObject(hObj);
					ReleaseDC(hWnd, hDC);
					return (INT_PTR)FALSE;
				}
			}
		}
		return (INT_PTR)GetStockObject(WHITE_BRUSH);
	}
	case WM_COMMAND:
		switch (wParam) {
		case IDC_RADIO1:
			bInitSynTPHelperDlg(hDlg, 0);
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
			break;
		case IDC_RADIO2:
			if (bGetDlgButtonChecked(hDlg, IDC_CHECK1)) {
				bInitSynTPHelperDlg(hDlg, 2);
			}
			else {
				bInitSynTPHelperDlg(hDlg, 1);
			}
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
			break;
		case IDC_RADIO3:
			if (bGetDlgButtonChecked(hDlg, IDC_CHECK1)) {
				bInitSynTPHelperDlg(hDlg, 4);
			}
			else {
				bInitSynTPHelperDlg(hDlg, 3);
			}
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
			break;
		case IDC_CHECK1:
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
			break;
		case IDC_START:
			if (!bSetSynTPHelperProfile(hDlg))	return (INT_PTR)FALSE;
			if (SynTP == NULL)	SynTP = new CSynTP;
			if (SynTP) {
				if ((Profile->lpstAppRegData->dwSynTPHelper1 == 1) || (Profile->lpstAppRegData->dwSynTPHelper1 == 2)) {
					SynTP->vStoptSender();
					SynTP->bStartSender(hMainWnd, Profile->lpstAppRegData->szSynTPSendIPAddr1, Profile->lpstAppRegData->dwSynTPPortNo1);

				}
				else if ((Profile->lpstAppRegData->dwSynTPHelper1 == 3) || (Profile->lpstAppRegData->dwSynTPHelper1 == 4)) {
					SynTP->vStopReceiver();
					SynTP->bStartReceiver(hMainWnd, Profile->lpstAppRegData->szSynTPSendIPAddr1, Profile->lpstAppRegData->dwSynTPPortNo1);
				}
			}
			bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);
			break;
		case IDC_BUTTON1:
			if (!bSetSynTPHelperProfile(hDlg))	return (INT_PTR)FALSE;
			bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);
			break;
		case IDC_STOP:
			if (SynTP) {
				SynTP->vStopReceiver();
				SynTP->vStoptSender();
				delete SynTP;
				SynTP = NULL;
			}
			bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			hSynTPHelperDlg = NULL;
			break;
		}
		return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

//
// bInitSynTPHelperDlg()
// 
static BOOL		bInitSynTPHelperDlg(HWND hDlg, DWORD dwSynTPHelper)
{
	DWORD	dwAddr = 0;
	TCHAR	szPort[6]{};
	if ((Profile->lpstAppRegData->szSynTPSendIPAddr1[0] != '\0')) {
		if((dwAddr = dwGetString2IPv4Addr(Profile->lpstAppRegData->szSynTPSendIPAddr1)) == (-1))	return FALSE;
	}
	if (_snwprintf_s(szPort, (sizeof(szPort) / sizeof(TCHAR)), _TRUNCATE, L"%05d", (WORD)Profile->lpstAppRegData->dwSynTPPortNo1) < 0)	return FALSE;

	EnableWindow(GetDlgItem(hDlg, IDC_RADIO1), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDC_RADIO2), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDC_RADIO3), TRUE);
	switch (dwSynTPHelper) {
	case 0:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
		SetFocus(GetDlgItem(hDlg, IDC_RADIO1));
		SendMessage(GetDlgItem(hDlg, IDC_IPADDRESS1), IPM_SETADDRESS, 0, (LPARAM)0);
		Edit_Enable(GetDlgItem(hDlg, IDC_IPADDRESS1), FALSE);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), 0);
		Edit_Enable(GetDlgItem(hDlg, IDC_EDIT1), FALSE);
		bSetCheckDlgButton(hDlg, IDC_CHECK1, FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK1), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_START), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_STOP), FALSE);
		break;
	case 1:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
		SetFocus(GetDlgItem(hDlg, IDC_RADIO2));
		SendMessage(GetDlgItem(hDlg, IDC_IPADDRESS1), IPM_SETADDRESS, 0, (LPARAM)dwAddr);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), szPort);
		Edit_Enable(GetDlgItem(hDlg, IDC_IPADDRESS1), TRUE);
		Edit_Enable(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
		bSetCheckDlgButton(hDlg, IDC_CHECK1, FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK1), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_START), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_STOP), FALSE);
		break;
	case 2:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
		SetFocus(GetDlgItem(hDlg, IDC_RADIO2));
		SendMessage(GetDlgItem(hDlg, IDC_IPADDRESS1), IPM_SETADDRESS, 0, (LPARAM)dwAddr);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), szPort);
		Edit_Enable(GetDlgItem(hDlg, IDC_IPADDRESS1), TRUE);
		Edit_Enable(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
		bSetCheckDlgButton(hDlg, IDC_CHECK1, TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK1), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_START), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_STOP), FALSE);
		break;
	case 3:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
		SetFocus(GetDlgItem(hDlg, IDC_RADIO3));
		SendMessage(GetDlgItem(hDlg, IDC_IPADDRESS1), IPM_SETADDRESS, 0, (LPARAM)0);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), szPort);
		Edit_Enable(GetDlgItem(hDlg, IDC_IPADDRESS1), FALSE);
		Edit_Enable(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
		bSetCheckDlgButton(hDlg, IDC_CHECK1, FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK1), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_START), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_STOP), FALSE);
		break;
	case 4:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
		SetFocus(GetDlgItem(hDlg, IDC_RADIO3));
		SendMessage(GetDlgItem(hDlg, IDC_IPADDRESS1), IPM_SETADDRESS, 0, (LPARAM)0);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), szPort);
		Edit_Enable(GetDlgItem(hDlg, IDC_IPADDRESS1), FALSE);
		Edit_Enable(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
		bSetCheckDlgButton(hDlg, IDC_CHECK1, TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK1), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_START), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_STOP), FALSE);
		break;
	}
	if (SynTP) {
		EnableWindow(GetDlgItem(hDlg, IDC_RADIO1), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_RADIO2), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_RADIO3), FALSE);
		Edit_Enable(GetDlgItem(hDlg, IDC_IPADDRESS1), FALSE);
		Edit_Enable(GetDlgItem(hDlg, IDC_EDIT1), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_CHECK1), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_START), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_STOP), TRUE);
	}
	return TRUE;
}

//
// bSetSynTPHelperProfile()
// 
static BOOL		bSetSynTPHelperProfile(HWND hDlg)
{
	if (Profile == NULL)	return FALSE;

	if (bGetDlgButtonChecked(hDlg, IDC_RADIO1)) {
		Profile->lpstAppRegData->dwSynTPHelper1 = 0;
	}
	else if (bGetDlgButtonChecked(hDlg, IDC_RADIO2)) {
		if (bGetDlgButtonChecked(hDlg, IDC_CHECK1)) {
			Profile->lpstAppRegData->dwSynTPHelper1 = 2;
		}
		else {
			Profile->lpstAppRegData->dwSynTPHelper1 = 1;
		}
		LPTSTR	lpszIPAddr =  new TCHAR[MAX_LOADSTRING];
		if (lpszIPAddr) {
			ZeroMemory(lpszIPAddr, (MAX_LOADSTRING * sizeof(TCHAR)));
			if (GetDlgItemText(hDlg, IDC_IPADDRESS1, lpszIPAddr, MAX_LOADSTRING) == 0) {
				delete [] lpszIPAddr;
				return FALSE;
			}
			else {
				ZeroMemory(Profile->lpstAppRegData->szSynTPSendIPAddr1, (MAX_LOADSTRING * sizeof(TCHAR)));
				if (wcsncpy_s(Profile->lpstAppRegData->szSynTPSendIPAddr1, MAX_LOADSTRING, lpszIPAddr, MAX_LOADSTRING) != 0) {
					delete [] lpszIPAddr;
					return FALSE;
				}
			}
			delete [] lpszIPAddr;
		}
		else return FALSE;
	}
	else if (bGetDlgButtonChecked(hDlg, IDC_RADIO3)) {
		if (bGetDlgButtonChecked(hDlg, IDC_CHECK1)) {
			Profile->lpstAppRegData->dwSynTPHelper1 = 4;
		}
		else {
			Profile->lpstAppRegData->dwSynTPHelper1 = 3;
		}
	}

	TCHAR	szPort[6]{};
	if (GetDlgItemText(hDlg, IDC_EDIT1, szPort, (sizeof(szPort) / sizeof(TCHAR))) == 0) {
		_snwprintf_s(szPort, sizeof(szPort), L"%05d", (DWORD)(50008));
	}
	int	 i = _wtoi(szPort);
	if ((50000 <= i) && (i < 60000)) {
		Profile->lpstAppRegData->dwSynTPPortNo1 = i;
	}
	else return FALSE;
	if (Profile)	Profile->bSetProfileData();
	return TRUE;
}

//
// vSettingDialog()
//
VOID		vSettingDialog(HWND hWnd)
{
	if (hSettingDlg != NULL) {
		SetForegroundWindow(hSettingDlg);
		return;
	}
	DialogBox(Resource->hLoad(), MAKEINTRESOURCE(IDD_SETTING), hWnd, SettingDlg);
}

//
// SettingDlg()
//
static INT_PTR CALLBACK SettingDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
		case WM_INITDIALOG:
			hSettingDlg = hDlg;
			{
				HICON	hIcon = (HICON)LoadImage(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 16, 16, 0);
				bInitDlgCenter(hDlg, hIcon, (ICC_BAR_CLASSES | ICC_HOTKEY_CLASS));
			}
			{
				TCHAR	lpText[MAX_LOADSTRING];
				if (LoadString(Resource->hLoad(), IDS_CMBX_LIST01, lpText, MAX_LOADSTRING) == 0)		return (INT_PTR)FALSE;
				SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)lpText);
				if (LoadString(Resource->hLoad(), IDS_CMBX_LIST02, lpText, MAX_LOADSTRING) == 0)		return (INT_PTR)FALSE;
				SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)lpText);
			}
			{
				if (Profile != NULL) {
					bSetCheckDlgButton(hDlg, IDC_CHECK1, Profile->lpstAppRegData->bDisplayIMEModeOnCursor);
					bSetCheckDlgButton(hDlg, IDC_CHECK2, Profile->lpstAppRegData->bOffChangedFocus);
					bSetCheckDlgButton(hDlg, IDC_CHECK3, Profile->lpstAppRegData->bForceHiragana);
					bSetCheckDlgButton(hDlg, IDC_CHECK4, Profile->lpstAppRegData->bDoModeDispByIMEKeyDown);
					bSetCheckDlgButton(hDlg, IDC_CHECK5, Profile->lpstAppRegData->bDoModeDispByMouseBttnUp);
					bSetCheckDlgButton(hDlg, IDC_CHECK6, Profile->lpstAppRegData->bDoModeDispByCtrlUp);
					bSetCheckDlgButton(hDlg, IDC_CHECK7, Profile->lpstAppRegData->bDrawNearCaret);
					bSetCheckDlgButton(hDlg, IDC_CHECK8, Profile->lpstAppRegData->bIMEModeForced);
					bSetCheckDlgButton(hDlg, IDC_CHECK9, Profile->lpstAppRegData->bEnableEPHelper);
				}
			}
			{
				if (Profile != NULL) {
					vSetTrackbar(hDlg, IDC_SLIDER1, 16, 64, 8, Profile->lpstAppRegData->iCursorSize, 8);
					vSetTrackbar(hDlg, IDC_SLIDER2, 8, 64, 8, Profile->lpstAppRegData->iModeSize, 8);
					vSetTrackbar(hDlg, IDC_SLIDER3, 100, 800, 70, Profile->lpstAppRegData->dwDisplayModeTime, 70);
					vSetTrackbar(hDlg, IDC_SLIDER4, 0, 1000, 100, Profile->lpstAppRegData->dwWaitWaveTime, 100);
				}
				if (IsDlgButtonChecked(hDlg, IDC_CHECK3) == 0 && IsDlgButtonChecked(hDlg, IDC_CHECK4) == 0) {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), FALSE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), FALSE);
				}
				else {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), TRUE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), TRUE);
				}
				if (IsDlgButtonChecked(hDlg, IDC_CHECK4) != 0)	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), TRUE);
				else EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), FALSE);
			}
			{
				if (Profile != NULL) {
					if (Profile->lpstAppRegData->bDisplayFocusWindowIME) {
						SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETCURSEL, 0, 0);
					}
					else {
						SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETCURSEL, 1, 0);
					}
				}
			}
			SetFocus(GetDlgItem(hDlg, IDC_CHECK1));
			return (INT_PTR)TRUE;
		case WM_HSCROLL:
			{
			if ((GetDlgItem(hDlg, IDC_SLIDER1) == (HWND)lParam)
						|| (GetDlgItem(hDlg, IDC_SLIDER2) == (HWND)lParam)
						|| (GetDlgItem(hDlg, IDC_SLIDER3) == (HWND)lParam)
						|| (GetDlgItem(hDlg, IDC_SLIDER4) == (HWND)lParam)) {
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
			}
		}
			break;
		case WM_CTLCOLORDLG:
			return (INT_PTR)GetStockObject(WHITE_BRUSH);
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT:
			SetBkMode(((HDC)wParam), TRANSPARENT);
			SetTextColor(((HDC)wParam), RGB(0, 0, 0));
			return (INT_PTR)GetStockObject(WHITE_BRUSH);
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDC_CHECK4:
			case IDC_CHECK5:
			case IDC_CHECK6:
				if (IsDlgButtonChecked(hDlg, IDC_CHECK4) == 0 && IsDlgButtonChecked(hDlg, IDC_CHECK5) == 0 && IsDlgButtonChecked(hDlg, IDC_CHECK6) == 0) {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), FALSE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), FALSE);
				}
				else {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), TRUE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), TRUE);
				}
				if (IsDlgButtonChecked(hDlg, IDC_CHECK6) != 0)	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), TRUE);
				else EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), FALSE);
				[[fallthrough]];
			case IDC_CHECK1:
			case IDC_CHECK2:
			case IDC_CHECK3:
			case IDC_CHECK7:
			case IDC_CHECK8:
			case IDC_CHECK9:
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				break;
			case IDC_COMBO1:
				if ((HIWORD(wParam) == CBN_SELCHANGE) && (Profile != NULL)) {
					switch (SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, 0, 0)) {
					case 0:
						break;
					case 1:
						if (IsDlgButtonChecked(hDlg, IDC_CHECK2) != 0) {
						}
						break;
					default:
						break;
					}
					EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				}
				break;
			case IDC_BUTTON1:
				SetFocus(GetDlgItem(hDlg, IDC_CHECK1));
				if (IsDlgButtonChecked(hDlg, IDC_CHECK4) == 0 && IsDlgButtonChecked(hDlg, IDC_CHECK5) == 0 && IsDlgButtonChecked(hDlg, IDC_CHECK6) == 0) {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), FALSE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), FALSE);
				}
				else {
					EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), TRUE);	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), TRUE);
				}
				if (IsDlgButtonChecked(hDlg, IDC_CHECK6) != 0)	EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), TRUE);
				else EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), FALSE);
				[[fallthrough]];						// fallthrough is explicit
			case IDOK:
				if (Profile != NULL) {
					Profile->lpstAppRegData->bDisplayIMEModeOnCursor = bGetDlgButtonChecked(hDlg, IDC_CHECK1);
					Profile->lpstAppRegData->bOffChangedFocus = bGetDlgButtonChecked(hDlg, IDC_CHECK2);
					Profile->lpstAppRegData->bForceHiragana = bGetDlgButtonChecked(hDlg, IDC_CHECK3);
					Profile->lpstAppRegData->bDoModeDispByIMEKeyDown = bGetDlgButtonChecked(hDlg, IDC_CHECK4);
					Profile->lpstAppRegData->bDoModeDispByMouseBttnUp = bGetDlgButtonChecked(hDlg, IDC_CHECK5);
					Profile->lpstAppRegData->bDoModeDispByCtrlUp = bGetDlgButtonChecked(hDlg, IDC_CHECK6);
					Profile->lpstAppRegData->bDrawNearCaret = bGetDlgButtonChecked(hDlg, IDC_CHECK7);
					Profile->lpstAppRegData->bIMEModeForced = bGetDlgButtonChecked(hDlg, IDC_CHECK8);
					Profile->lpstAppRegData->bEnableEPHelper = bGetDlgButtonChecked(hDlg, IDC_CHECK9);
					Profile->lpstAppRegData->iCursorSize = (int)dwGetTrackbarPos(hDlg, IDC_SLIDER1);
					Profile->lpstAppRegData->iModeSize = (int)dwGetTrackbarPos(hDlg, IDC_SLIDER2);
					Profile->lpstAppRegData->dwDisplayModeTime = dwGetTrackbarPos(hDlg, IDC_SLIDER3);
					Profile->lpstAppRegData->dwWaitWaveTime = dwGetTrackbarPos(hDlg, IDC_SLIDER4);
					switch (SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, 0, 0)) {
					case 0:
						Profile->lpstAppRegData->bDisplayFocusWindowIME = TRUE;
						break;
					case 1:
						Profile->lpstAppRegData->bDisplayFocusWindowIME = FALSE;
						break;
					default:
						break;
					}
					Profile->bSetProfileData();
				}
				Cursor->bReloadCursor();
				if (LOWORD(wParam) == IDC_BUTTON1) {
					EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);
					return (INT_PTR)FALSE;
				}
				[[fallthrough]];						// fallthrough is explicit
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))	hSettingDlg = NULL;
				return (INT_PTR)TRUE;
			}
}
	return (INT_PTR)FALSE;
}

//
// bInitDlgCenter()
//
static BOOL		bInitDlgCenter(HWND hDlg, HICON hIcon, DWORD dwICC)
{
	int		cx = GetSystemMetrics(SM_CXSCREEN);
	int		cy = GetSystemMetrics(SM_CYSCREEN);
	RECT	rc{};
	GetWindowRect(hDlg, (LPRECT)&rc);
	rc.right = rc.right - rc.left + 1;	rc.bottom = rc.bottom - rc.top + 1;
	rc.left = (cx - rc.right) / 2;		rc.top = (cy - rc.bottom) / 2;
	MoveWindow(hDlg, rc.left, rc.top, rc.right, rc.bottom, TRUE);
	SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	// Initialize
	INITCOMMONCONTROLSEX stICC{};
	stICC.dwSize = sizeof(INITCOMMONCONTROLSEX);
	stICC.dwICC = dwICC;
	return InitCommonControlsEx(&stICC);
}

//
// bSetCheckDlgButton()
//
static BOOL		bSetCheckDlgButton(HWND hDlg, int iIDButton, BOOL bCheck)
{
	UINT	uCheck = 0;
	if (bCheck) uCheck = BST_CHECKED;
	else		uCheck = BST_UNCHECKED;
	if (!CheckDlgButton(hDlg, iIDButton, uCheck)) {
		return FALSE;
	}
	return TRUE;
}

//
// vSetTrackbar()
//
static VOID		vSetTrackbar(HWND hDlg, int iIDTrackbar, int iRangeMin, int iRangeMax, int iTickFreq, int iPos, int iPageSize)
{
	SendDlgItemMessage(hDlg, iIDTrackbar, TBM_SETRANGE, TRUE, MAKELPARAM(iRangeMin, iRangeMax));
	SendDlgItemMessage(hDlg, iIDTrackbar, TBM_SETTICFREQ, iTickFreq, 0);
	SendDlgItemMessage(hDlg, iIDTrackbar, TBM_SETPOS, TRUE, iPos);
	SendDlgItemMessage(hDlg, iIDTrackbar, TBM_SETPAGESIZE, 0, iPageSize);
}

//
// bGetDlgButtonChecked()
//
static BOOL		bGetDlgButtonChecked(HWND hDlg, int iIDButton)
{
	if (IsDlgButtonChecked(hDlg, iIDButton) == 0)	return FALSE;
	else return TRUE;
}

//
// dwGetTrackbarPos()
//
static DWORD		dwGetTrackbarPos(HWND hDlg, int iIDTrackbar)
{
	return (DWORD)SendDlgItemMessage(hDlg, iIDTrackbar, TBM_GETPOS, NULL, NULL);
}


/* = EOF = */
