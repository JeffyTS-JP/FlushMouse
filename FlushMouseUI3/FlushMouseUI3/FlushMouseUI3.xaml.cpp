// FlushMouseUI3.Xaml.cpp for FlushMouseUI3
//		Copyright (C) 2022 JeffyTS
//
//	  
// No.	  Date			Name			Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/07/23  JeffyTS  	New edit.
//

//
// Include
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

#include "..\..\FlushMouseLIB\FlushMouseLIB.h"
#include "..\..\FlushMouseLIB\Resource.h"
#include "..\..\FlushMouseLIB\Eventlog.h"

#pragma pop_macro("GetCurrentTime")

#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#include "..\..\FlushMouseLIB\TaskTray.h"

//
// Using
//
using namespace winrt;
using namespace winrt::Microsoft::Windows::AppLifecycle;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::ApplicationModel::Activation;

//
// Local Data
//
static HINSTANCE	m_hInstance{ nullptr };

//
// Global Prototype Define
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

//
// Local Prototype Define
//

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
	SettingsExec(hWnd, WM_SETTINGSEX, SETTINGSEX_SELECTEDPANE_ABOUT);
}

//
// vIMEModeDialog()
//
VOID		vIMEModeDialog(HWND hWnd)
{
	SettingsExec(hWnd, WM_SETTINGSEX, SETTINGSEX_SELECTEDPANE_IMEMODE);
}

//
// vSynTPHelperDialog()
//
VOID		vSynTPHelperDialog(HWND hWnd)
{
	SettingsExec(hWnd, WM_SETTINGSEX, SETTINGSEX_SELECTEDPANE_SYNTPHELPER);
}

//
// vSettingDialog()
//
VOID		vSettingDialog(HWND hWnd)
{
	SettingsExec(hWnd, WM_SETTINGSEX, SETTINGSEX_SELECTEDPANE_GENERAL);
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
// App()
//
App::App()
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	InitializeComponent();
}

//
// ~App()
//
App::~App()
{
	return;
}

//
// OnLaunched()
//
void App::OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&)
{
#ifdef _DEBUG
	UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const&) {
		MessageBox(NULL, L"EXCEPTION", L"FlushMouseUI3", (MB_ICONSTOP | MB_OK));
		});
#endif // _DEBUG

	int	iRet = 0;
	LPCTSTR	_lpCmdLine = GetCommandLine();
	int		iNumArgs = 0;
	LPTSTR	*_lpArgv = CommandLineToArgvW(_lpCmdLine, &iNumArgs);
	if (iNumArgs != 0) {
		if ((iRet = iCheckCmdLine((LPCTSTR)_lpArgv) != 1)) {
			LocalFree(_lpArgv);
			PostQuitMessage(0);
			return;
		}
	}
	LocalFree(_lpArgv);
	
	if ((m_hInstance = GetModuleHandle(NULL)) == NULL) {
		PostQuitMessage(0);
		return;
	}

	hMicrosoft_ui_xaml_dll = LoadLibrary(L"Microsoft.ui.xaml.dll");
	hFlushMouseUI3DLL = LoadLibrary(FLUSHMOUSEUI3_DLL);

	MojoWindowExec();

	if (!bWinMain((HINSTANCE)m_hInstance, NULL, NULL, SW_HIDE)) {
	}
	
	MojoWindowClose();

	if (hFlushMouseUI3DLL)	FreeLibrary(hFlushMouseUI3DLL);
	if (hMicrosoft_ui_xaml_dll)	FreeLibrary(hMicrosoft_ui_xaml_dll);

	PostQuitMessage(0);
	return;
}

//
// OnSuspending()
//
void App::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] Windows::ApplicationModel::SuspendingEventArgs const& e)
{
}



/* = EOF = */