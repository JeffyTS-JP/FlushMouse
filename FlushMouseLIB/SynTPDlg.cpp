//
// SynTPDlg.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/03/21  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "Resource.h"
#include "FlushMouseLIB.h"
#include "TaskTray.h"
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#include "..\..\MiscLIB\CRegistry\CRegistry.h"

//
// Define
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
static INT_PTR CALLBACK SynTPHelperDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

static BOOL		bInitSynTPHelperDlg(HWND hDlg, DWORD dwSynTPHelper);
static BOOL		bSetSynTPHelperProfile(HWND hDlg);
static BOOL		bInitDlgCenter(HWND hDlg, HICON hIcon, DWORD dwICC);
static BOOL		bSetCheckDlgButton(HWND hDlg, int iIDButton, BOOL bCheck);
static BOOL		bGetDlgButtonChecked(HWND hDlg, int iIDButton);

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
			{	// Initialize
				HICON	hIcon = NULL;
				hIcon = (HICON)LoadImage(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 16, 16, 0);
				SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
				bInitDlgCenter(hDlg, hIcon, (ICC_BAR_CLASSES | ICC_HOTKEY_CLASS | ICC_INTERNET_CLASSES));
				bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);
			}
			return (INT_PTR)TRUE;
		case WM_CTLCOLORDLG:
			return (INT_PTR)GetStockObject(WHITE_BRUSH);
		case WM_CTLCOLOREDIT:
			SetBkMode(((HDC)wParam), TRANSPARENT);
			SetTextColor(((HDC)wParam), RGB(0, 0, 0));
			return	(INT_PTR)GetStockObject(WHITE_BRUSH);
		case WM_CTLCOLORSTATIC:
			{
			SetBkMode(((HDC)wParam), TRANSPARENT);
			SetTextColor(((HDC)wParam), RGB(0, 0, 0));
			HWND	hWnd = NULL;
			if ((hWnd = GetDlgItem(hDlg, IDC_EDIT1)) == (HWND)lParam) {
				if (!IsWindowEnabled(hWnd)) {
					return (INT_PTR)FALSE;
				}
			}
			if ((hWnd = GetDlgItem(hDlg, IDC_EDIT2)) == (HWND)lParam) {
				if (!IsWindowEnabled(hWnd)) {
					return (INT_PTR)FALSE;
				}
			}
			return	(INT_PTR)GetStockObject(WHITE_BRUSH);
		}
		case WM_COMMAND:
			switch LOWORD((wParam)) {
			case IDC_RADIO1:
				Profile->lpstAppRegData->dwSynTPHelper1 = 0x00;
				bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				return (INT_PTR)FALSE;
			case IDC_RADIO2:
				if (bGetDlgButtonChecked(hDlg, IDC_CHECK1)) {
					if (bGetDlgButtonChecked(hDlg, IDC_RADIO4))	Profile->lpstAppRegData->dwSynTPHelper1 = 2;
					else Profile->lpstAppRegData->dwSynTPHelper1 = 0x12;
				}
				else {
					if (bGetDlgButtonChecked(hDlg, IDC_RADIO4))	Profile->lpstAppRegData->dwSynTPHelper1 = 1;
					else Profile->lpstAppRegData->dwSynTPHelper1 = 0x11;
				}
				bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				return (INT_PTR)FALSE;
			case IDC_RADIO3:
				if (bGetDlgButtonChecked(hDlg, IDC_CHECK1)) {
					Profile->lpstAppRegData->dwSynTPHelper1 = 0x04;
				}
				else {
					Profile->lpstAppRegData->dwSynTPHelper1 = 0x03;
				}
				bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				return (INT_PTR)FALSE;
			case IDC_RADIO4:
				if (bGetDlgButtonChecked(hDlg, IDC_CHECK1)) {
					Profile->lpstAppRegData->dwSynTPHelper1 = 0x02;
				}
				else {
					Profile->lpstAppRegData->dwSynTPHelper1 = 0x01;
				}
				bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				return (INT_PTR)FALSE;
			case IDC_RADIO5:
				if (bGetDlgButtonChecked(hDlg, IDC_CHECK1)) {
					Profile->lpstAppRegData->dwSynTPHelper1 = 0x12;
				}
				else {
					Profile->lpstAppRegData->dwSynTPHelper1 = 0x11;
				}
				bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				return (INT_PTR)FALSE;
			case IDC_IPADDRESS1:
				if (HIWORD(wParam) == EN_KILLFOCUS) {
					LPTSTR	lpszIPAddr =  new TCHAR[MAX_IPV4_ADDRESS];
					if (lpszIPAddr) {
						ZeroMemory(lpszIPAddr, (MAX_IPV4_ADDRESS * sizeof(TCHAR)));
						if (GetDlgItemText(hDlg, IDC_IPADDRESS1, lpszIPAddr, MAX_IPV4_ADDRESS) == 0) {
							if (lpszIPAddr)	delete [] lpszIPAddr;
							return (INT_PTR)FALSE;
						}
						else {
							if (_wcsnicmp(Profile->lpstAppRegData->szSynTPSendIPAddr1, lpszIPAddr, MAX_IPV4_ADDRESS) == 0) break;
						}
						if (lpszIPAddr)	delete [] lpszIPAddr;
					}
					else return (INT_PTR)FALSE;
					EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				}
				return (INT_PTR)TRUE;
			case IDC_EDIT1:
				if (HIWORD(wParam) == EN_KILLFOCUS) {
					LPTSTR	lpszHostname =  new TCHAR[MAX_FQDN];
					if (lpszHostname) {
						ZeroMemory(lpszHostname, (MAX_FQDN * sizeof(TCHAR)));
						if (GetDlgItemText(hDlg, IDC_EDIT1, lpszHostname, MAX_FQDN) == 0) {
							if (lpszHostname)	delete [] lpszHostname;
							return (INT_PTR)FALSE;
						}
						else {
							if (_wcsnicmp(Profile->lpstAppRegData->szSynTPSendHostname1, lpszHostname, MAX_IPV4_ADDRESS) == 0) break;
						}
						if (lpszHostname)	delete [] lpszHostname;
					}
					else return (INT_PTR)FALSE;
					EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				}
				return (INT_PTR)TRUE;
			case IDC_EDIT2:
				if (HIWORD(wParam) == EN_KILLFOCUS) {
					TCHAR	szPort[6]{};
					if (GetDlgItemText(hDlg, IDC_EDIT2, szPort, (sizeof(szPort) / sizeof(TCHAR))) == 0) {
						_snwprintf_s(szPort, sizeof(szPort), L"%05d", (DWORD)(50008));
					}
					int	 i = _wtoi(szPort);
					if ((50000 <= i) && (i < 60000)) {
						if (Profile->lpstAppRegData->dwSynTPPortNo1 != (DWORD)i) {
							EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
						}
					}
					else return (INT_PTR)FALSE;
				}
				return (INT_PTR)TRUE;
			case IDC_CHECK1:
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
				return (INT_PTR)FALSE;
			case IDC_START:
				if (!bSetSynTPHelperProfile(hDlg))	return (INT_PTR)FALSE;
				if (SynTP == NULL)	SynTP = new CSynTP;
				if (SynTP) {
					switch (Profile->lpstAppRegData->dwSynTPHelper1) {
						case 0x01:
						case 0x02:
							SynTP->vStoptSender();
							if (!SynTP->bStartSender(hMainWnd, Profile->lpstAppRegData->szSynTPSendIPAddr1, Profile->lpstAppRegData->dwSynTPPortNo1)) {
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK)
								vMessageBox(hMainWnd, IDS_CANTSYTPHELPER, MessageBoxTYPE);
								delete SynTP;
								SynTP = NULL;
							}
							break;
						case 0x11:
						case 0x12:
						case 0x31:
						case 0x32:
							SynTP->vStoptSender();
							if (bCheckExistHostnameIPv4(Profile->lpstAppRegData->szSynTPSendHostname1)) {
								if (SynTP->bStartSender(hMainWnd, Profile->lpstAppRegData->szSynTPSendHostname1, Profile->lpstAppRegData->dwSynTPPortNo1)) {
									break;
								}
							}
							vMessageBox(hMainWnd, IDS_CANTSYTPHELPER, MessageBoxTYPE);
							delete SynTP;
							SynTP = NULL;
							break;
						case 0x03:
						case 0x04:
						case 0x33:
						case 0x34:
							SynTP->vStopReceiver();
							if (!SynTP->bStartReceiver(hMainWnd, Profile->lpstAppRegData->dwSynTPPortNo1)) {
								vMessageBox(hMainWnd, IDS_CANTSYTPHELPER, MessageBoxTYPE);
								delete SynTP;
								SynTP = NULL;
							}
							break;
					}
				}
				if (Profile)	bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);
				return (INT_PTR)FALSE;
			case IDC_STOP:
				if (SynTP) {
					SynTP->vStopReceiver();
					SynTP->vStoptSender();
					delete SynTP;
					SynTP = NULL;
				}
				if (Profile)	bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);
				return (INT_PTR)FALSE;
			case IDC_BUTTON1:
				if (!bSetSynTPHelperProfile(hDlg))	return (INT_PTR)FALSE;
				if (Profile)	bInitSynTPHelperDlg(hDlg, Profile->lpstAppRegData->dwSynTPHelper1);
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);
				break;
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				hSynTPHelperDlg = NULL;
				return (INT_PTR)FALSE;
			default:
				return (INT_PTR)FALSE;
			}
			break;
		default:
			return (INT_PTR)FALSE;
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
		if((dwAddr = dwGetString2IPv4Addr(Profile->lpstAppRegData->szSynTPSendIPAddr1)) == (-1)) dwAddr = 0;
		SendMessage(GetDlgItem(hDlg, IDC_IPADDRESS1), IPM_SETADDRESS, 0, (LPARAM)dwAddr);
	}
	if (Profile->lpstAppRegData->szSynTPSendHostname1 != NULL) {
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), Profile->lpstAppRegData->szSynTPSendHostname1);
	}
	if (_snwprintf_s(szPort, (sizeof(szPort) / sizeof(TCHAR)), _TRUNCATE, L"%05d", (WORD)Profile->lpstAppRegData->dwSynTPPortNo1) < 0) {
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT2), L"");
	}
	else SetWindowText(GetDlgItem(hDlg, IDC_EDIT2), szPort);

	EnableWindow(GetDlgItem(hDlg, IDC_RADIO1), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDC_RADIO2), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDC_RADIO3), TRUE);
	switch (dwSynTPHelper) {
		case 0x00:	// disable
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO4), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO5), FALSE);
			Edit_Enable(GetDlgItem(hDlg, IDC_IPADDRESS1), FALSE);
			Edit_Enable(GetDlgItem(hDlg, IDC_EDIT1), FALSE);
			Edit_Enable(GetDlgItem(hDlg, IDC_EDIT2), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_CHECK1), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_START), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_STOP), FALSE);
			break;
		case 0x01:	// sender (IPv4)
		case 0x02:	// sender (always start IPV4)
		case 0x11:	// sender (Hosstname IPV4)
		case 0x12:	// sender (always start Hosstname IPV4)
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO4), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO5), TRUE);
			if ((dwSynTPHelper == 0x01) || (dwSynTPHelper == 0x02)) {
				CheckRadioButton(hDlg, IDC_RADIO4, IDC_RADIO5, IDC_RADIO4);
				Edit_Enable(GetDlgItem(hDlg, IDC_IPADDRESS1), TRUE);
				Edit_Enable(GetDlgItem(hDlg, IDC_EDIT1), FALSE);
			}
			else if ((dwSynTPHelper == 0x11) || (dwSynTPHelper == 0x12)) {
				CheckRadioButton(hDlg, IDC_RADIO4, IDC_RADIO5, IDC_RADIO5);
				Edit_Enable(GetDlgItem(hDlg, IDC_IPADDRESS1), FALSE);
				Edit_Enable(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
				Edit_GetModify(GetDlgItem(hDlg, IDC_EDIT1));
			}
			Edit_Enable(GetDlgItem(hDlg, IDC_EDIT2), TRUE);
			Edit_GetModify(GetDlgItem(hDlg, IDC_EDIT2));
			EnableWindow(GetDlgItem(hDlg, IDC_CHECK1), TRUE);
			if ((dwSynTPHelper == 0x01) || (dwSynTPHelper == 0x11)) {
				bSetCheckDlgButton(hDlg, IDC_CHECK1, FALSE);
			}
			else if ((dwSynTPHelper == 0x02) || (dwSynTPHelper == 0x12)) {
				bSetCheckDlgButton(hDlg, IDC_CHECK1, TRUE);
			}
			EnableWindow(GetDlgItem(hDlg, IDC_START), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_STOP), FALSE);
			break;
		case 0x03:	// receiver (IPv4)
		case 0x04:	// receiver (always start IPv4)
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO4), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO5), FALSE);
			Edit_Enable(GetDlgItem(hDlg, IDC_IPADDRESS1), FALSE);
			Edit_Enable(GetDlgItem(hDlg, IDC_EDIT1), FALSE);
			Edit_Enable(GetDlgItem(hDlg, IDC_EDIT2), TRUE);
			Edit_GetModify(GetDlgItem(hDlg, IDC_EDIT2));
			EnableWindow(GetDlgItem(hDlg, IDC_CHECK1), TRUE);
			if (dwSynTPHelper == 3) {
				bSetCheckDlgButton(hDlg, IDC_CHECK1, FALSE);
			}
			else {
				bSetCheckDlgButton(hDlg, IDC_CHECK1, TRUE);
			}
			EnableWindow(GetDlgItem(hDlg, IDC_START), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_STOP), FALSE);
			break;
		case 0x31:	// sender(Hosstname IPV6)
		case 0x32:	// sender (always start Hosstname IPV6
		case 0x33:	// 33 = receiver (IPv6)
		case 0x34:	// 34 = receiver (always start IPv6)
			break;
	}
	if (SynTP) {
		EnableWindow(GetDlgItem(hDlg, IDC_RADIO1), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_RADIO2), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_RADIO3), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_RADIO4), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_RADIO5), FALSE);
		Edit_Enable(GetDlgItem(hDlg, IDC_IPADDRESS1), FALSE);
		Edit_Enable(GetDlgItem(hDlg, IDC_EDIT1), FALSE);
		Edit_Enable(GetDlgItem(hDlg, IDC_EDIT2), FALSE);
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
		Profile->lpstAppRegData->dwSynTPHelper1 = 0x00;
	}
	else if (bGetDlgButtonChecked(hDlg, IDC_RADIO3)) {
		if (bGetDlgButtonChecked(hDlg, IDC_CHECK1)) {
			Profile->lpstAppRegData->dwSynTPHelper1 = 0x04;
		}
		else {
			Profile->lpstAppRegData->dwSynTPHelper1 = 0x03;
		}
	}
	else if (bGetDlgButtonChecked(hDlg, IDC_RADIO2)) {
		if (bGetDlgButtonChecked(hDlg, IDC_CHECK1)) {
			if (bGetDlgButtonChecked(hDlg, IDC_RADIO4)) Profile->lpstAppRegData->dwSynTPHelper1 = 0x02;
			else  Profile->lpstAppRegData->dwSynTPHelper1 = 0x12;
		}
		else {
			if (bGetDlgButtonChecked(hDlg, IDC_RADIO4)) Profile->lpstAppRegData->dwSynTPHelper1 = 0x01;
			else  Profile->lpstAppRegData->dwSynTPHelper1 = 0x11;
		}
		//if (bGetDlgButtonChecked(hDlg, IDC_RADIO4)) {
		LPTSTR	lpszIPAddr =  new TCHAR[MAX_IPV4_ADDRESS];
		if (lpszIPAddr) {
			ZeroMemory(lpszIPAddr, (MAX_IPV4_ADDRESS * sizeof(TCHAR)));
			if (GetDlgItemText(hDlg, IDC_IPADDRESS1, lpszIPAddr, MAX_IPV4_ADDRESS) == 0) {
				if (lpszIPAddr)	delete [] lpszIPAddr;
				return FALSE;
			}
			else {
				ZeroMemory(Profile->lpstAppRegData->szSynTPSendIPAddr1, (MAX_IPV4_ADDRESS * sizeof(TCHAR)));
				if (wcsncpy_s(Profile->lpstAppRegData->szSynTPSendIPAddr1, MAX_IPV4_ADDRESS, lpszIPAddr, MAX_IPV4_ADDRESS) != 0) {
					if (lpszIPAddr)	delete [] lpszIPAddr;
					return FALSE;
				}
			}
			if (lpszIPAddr)	delete [] lpszIPAddr;
		}
		else return FALSE;
		//}

		//if (bGetDlgButtonChecked(hDlg, IDC_RADIO5)) {
		LPTSTR	lpszHostname =  new TCHAR[MAX_FQDN];
		if (lpszHostname) {
			ZeroMemory(lpszHostname, (MAX_FQDN * sizeof(TCHAR)));
			if (GetDlgItemText(hDlg, IDC_EDIT1, lpszHostname, MAX_FQDN) == 0) {
				if (lpszHostname)	delete [] lpszHostname;
				return FALSE;
			}
			else {
				ZeroMemory(Profile->lpstAppRegData->szSynTPSendHostname1, (MAX_FQDN * sizeof(TCHAR)));
				if (wcsncpy_s(Profile->lpstAppRegData->szSynTPSendHostname1, MAX_FQDN, lpszHostname, MAX_FQDN) != 0) {
					if (lpszHostname)	delete [] lpszHostname;
					return FALSE;
				}
			}
			if (lpszHostname)	delete [] lpszHostname;
		}
		else return FALSE;
		//}
	}

	TCHAR	szPort[6]{};
	if (GetDlgItemText(hDlg, IDC_EDIT2, szPort, (sizeof(szPort) / sizeof(TCHAR))) == 0) {
		_snwprintf_s(szPort, sizeof(szPort), L"%05d", (DWORD)(50008));
	}
	int	 i = _wtoi(szPort);
	if ((50000 <= i) && (i < 60000)) {
		Profile->lpstAppRegData->dwSynTPPortNo1 = i;
	}
	else return FALSE;
	if (Profile)	Profile->bSetProfileData();					// Set Registry data
	return TRUE;
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
// bGetDlgButtonChecked()
//
static BOOL		bGetDlgButtonChecked(HWND hDlg, int iIDButton)
{
	if (IsDlgButtonChecked(hDlg, iIDButton) == 0)	return FALSE;
	else return TRUE;
}


/* = EOF = */