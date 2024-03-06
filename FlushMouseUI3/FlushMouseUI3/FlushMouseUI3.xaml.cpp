// FlushMouseUI3.Xaml.cpp for FlushMouseUI3
//		Copyright (C) 2022 JeffyTS
//
//	  
// No.	  Date			Name			Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/07/23  JeffyTS  	New edit.
//
#include "pch.h"
#include "WinRT.h"

#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime
#include <WindowsAppSDK-VersionInfo.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_CLIENTBLOCK
#define _CRTDBG_MAP_ALLOC
#define new DEBUG_CLIENTBLOCK
#endif

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

#include "FlushMouseUI3.xaml.h"
#include "MojoWindow.xaml.h"
#include "About.xaml.h"

#include "..\..\FlushMouseLIB\FlushMouseLIB.h"
#include "..\..\FlushMouseLIB\Resource.h"

#pragma pop_macro("GetCurrentTime")

#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#include "..\..\FlushMouseLIB\TaskTray.h"
static INT_PTR CALLBACK SynTPHelperDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static BOOL		bInitSynTPHelperDlg(HWND hDlg, DWORD dwSynTPHelper);
static BOOL		bSetSynTPHelperProfile(HWND hDlg);
static BOOL		bInitDlgCenter(HWND hDlg, HICON hIcon, DWORD dwICC);
static BOOL		bSetCheckDlgButton(HWND hDlg, int iIDButton, BOOL bCheck);
static BOOL		bGetDlgButtonChecked(HWND hDlg, int iIDButton);

using namespace winrt;
using namespace winrt::Microsoft::Windows::AppLifecycle;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::ApplicationModel::Activation;


static Application	windowApp{ nullptr };
static HINSTANCE	_hInstance{ nullptr };

static int			activationCount = 1;
static event_token	activationToken;
static Collections::IVector<IInspectable> messages;

//
// wWinMain()
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPTSTR lpCmdLine, _In_ int)
{
	UNREFERENCED_PARAMETER(hInstance);

	// TODO: ここにコードを挿入してください。
#ifdef _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_ASSERTE(_CrtCheckMemory());

#endif

	_hInstance = hInstance;

	int	iRet = 0;
	if ((iRet = iCheckCmdLine(lpCmdLine)) != 1) {
		return (iRet);
	}
	
	init_apartment(apartment_type::multi_threaded);
	messages = multi_threaded_observable_vector<IInspectable>();

	HRESULT hre = CoInitializeEx(NULL, (COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE));
	if (FAILED(hre)) {
		return (-1);
	}
	else {
		try {
			Application::Start([](auto&&) {windowApp = make<FlusMouseUI3>(); });
		}
		catch (const std::exception&) {
		}
		catch (...) {
		}

		try {
			CoUninitialize();
		}
		catch (...) {
		}
	}
	return iRet;
}

// 
// vMessageBox()
//
void vMessageBox(HWND hWnd, UINT uID, UINT uType)
{
	TCHAR	lpText[MAX_LOADSTRING];
	if (LoadString(Resource->hLoad(), uID, lpText, MAX_LOADSTRING) != 0) {
		MessageBox(hWnd, lpText, szTitle, uType);
	}
}

//
// vAboutDialog()
//
VOID		vAboutDialog(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);
	FlushMouseUI3::implementation::AboutExec();
}

//
// vSettingDialog()
//
VOID		vSettingDialog(HWND hWnd)
{
	SettingsExec(hWnd, WM_SETTINGSEX);
}

//
// vSettingDialogApply()
//
VOID		vSettingDialogApply()
{
	SettingsApply();
}

//
// vSettingDialogClose()
//
VOID		vSettingDialogClose()
{
	SettingsClose();
}

//
// vSynTPHelperDialog()
//
VOID		vSynTPHelperDialog(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);
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

//
// CalcWindowCentralizeRect()
//
void CalcWindowCentralizeRect(HWND hWnd, Windows::Graphics::RectInt32* Rect32)
{
	UINT	dpiX = 0, dpiY = 0;
	RECT	rc{};
	GetMonitorDPIandRect(hWnd, &dpiX, &rc);
	dpiY = dpiX;
	float dpiHX = (float)dpiX / (float)USER_DEFAULT_SCREEN_DPI;
	float dpiHY = (float)dpiY / (float)USER_DEFAULT_SCREEN_DPI;
	Rect32->Width = (LONG)round(Rect32->Width * dpiHX);
	Rect32->Height = (LONG)round(Rect32->Height * dpiHY);
	Rect32->X = rc.left + (rc.right - rc.left - Rect32->Width) / 2;
	Rect32->Y = rc.top + (rc.bottom - rc.top - Rect32->Height) / 2;
}

//
// GetMonitorDPIandRect()
//
void GetMonitorDPIandRect(HWND hWnd, LPUINT lpUint,LPRECT lpRect)
{
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi{};	mi.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hMonitor, &mi);
	lpRect->left = mi.rcWork.left;
	lpRect->top = mi.rcWork.top;
	lpRect->right = mi.rcWork.right;
	lpRect->bottom = mi.rcWork.bottom;
	*lpUint = GetDpiForWindow(hWnd);
}

//
// FlusMouseUI3()
//
FlusMouseUI3::FlusMouseUI3()
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	InitializeComponent();
}

//
// ~FlusMouseUI3()
//
FlusMouseUI3::~FlusMouseUI3()
{
	return;
}

//
// OnLaunched()
//
void FlusMouseUI3::OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const& /*args*/)
{
	hFlushMouseUI3DLL = ::LoadLibrary(FLUSHMOUSEUI3_DLL);
	
	MojoWindowExec();
	
	if (!bWinMain((HINSTANCE)_hInstance, NULL, NULL, SW_HIDE)) {
		MojoWindowClose();
		if (hFlushMouseUI3DLL)	::FreeLibrary(hFlushMouseUI3DLL);
		return;
	}

	MojoWindowClose();

	if (hFlushMouseUI3DLL)	::FreeLibrary(hFlushMouseUI3DLL);

	return;
}

//
// OnSuspending()
//
void FlusMouseUI3::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] Windows::ApplicationModel::SuspendingEventArgs const& e)
{
}

//
// GetInstanceFromHWND()
//
HINSTANCE FlusMouseUI3::GetInstanceFromHWND(HWND hWnd)
{
	return (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
}


/* = EOF = */