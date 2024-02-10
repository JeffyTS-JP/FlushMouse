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
		Application::Start([](auto&&) {windowApp = make<FlusMouseUI3>(); });

		try {
			CoUninitialize();
		}
		catch (...) {
		}

	}

	return 0;
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

	PostQuitMessage(0);
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