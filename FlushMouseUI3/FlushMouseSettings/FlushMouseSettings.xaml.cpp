// FlushMouseSettings.Xaml.cpp for FlushMouseSettings
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

#include "FlushMouseSettings.xaml.h"
#include "MojoWindow.xaml.h"

#include "..\..\FlushMouseLIB\Profile.h"
#include "..\..\FlushMouseLIB\CResource.h"
#include "..\..\FlushMouseLIB\Resource.h"
#include "..\..\FlushMouseLIB\TaskTray.h"

#pragma pop_macro("GetCurrentTime")

#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")

//
// Using
//
using namespace winrt;
using namespace winrt::Microsoft::Windows::AppLifecycle;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::ApplicationModel::Activation;

//
// Global Data
//
TCHAR		szTitle[MAX_LOADSTRING] = FLUSHMOUSE;
HWND		hMainWnd = NULL;

CProfile	*Profile = NULL;
CResource	*Resource = NULL;
CSynTP		*SynTP = NULL;

//
// Local Data
//
static Application	windowApp{ nullptr };
static HINSTANCE	_hInstance{ nullptr };

static int			activationCount = 1;
static event_token	activationToken;
static Collections::IVector<IInspectable> messages;
static int			iSelectedPane = SETTINGSEX_SELECTEDPANE_GENERAL;

//
// Global Prototype Define
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

//
// Local Prototype Define
//
static BOOL		bSettingsWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_opt_ LPWSTR lpCmdLine, _In_ int nCmdShow);
static BOOL		bSetHeapInformation();
static ATOM		MyRegisterClass(HINSTANCE hInstance);
static HWND		InitInstance(HINSTANCE hInstance, int nCmdShow);
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Handler
static BOOL		Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
static void		Cls_OnDestroy(HWND hWnd);
static bool		Cls_OnSettingsEx(HWND hWnd, int iCode, LPARAM lParam);

//
// wWinMain()
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	_hInstance = hInstance;

	LPCTSTR	_lpCmdLine = GetCommandLine();
	int		iNumArgs = 0;
	LPTSTR	*_lpArgv = CommandLineToArgvW(_lpCmdLine, &iNumArgs);
	if (iNumArgs != 0) {
		if (_lpArgv[1]  != _T('\0')) {
			if (CompareStringOrdinal(_lpArgv[1], -1, L"1", -1, TRUE) == CSTR_EQUAL) {
				iSelectedPane = SETTINGSEX_SELECTEDPANE_GENERAL;
			}
			else if (CompareStringOrdinal(_lpArgv[1], -1, L"3", -1, TRUE) == CSTR_EQUAL) {
				iSelectedPane = SETTINGSEX_SELECTEDPANE_SYNTPHELPER;
			}
			else if (CompareStringOrdinal(_lpArgv[1], -1, L"4", -1, TRUE) == CSTR_EQUAL) {
				iSelectedPane = SETTINGSEX_SELECTEDPANE_ABOUT;
			}
		}
	}
	LocalFree(_lpArgv);

	init_apartment(apartment_type::multi_threaded);
	messages = multi_threaded_observable_vector<IInspectable>();
	
	HRESULT hre = CoInitializeEx(NULL, (COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE));
	if (FAILED(hre)) {
		return (-1);
	}
	else {
		try {
			Application::Start([](auto&&) {
				windowApp = make<SettingsMain>(); });
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
	SettingsExec(hWnd, WM_SETTINGSEX, SETTINGSEX_SELECTEDPANE_ABOUT);
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
	SettingsExec(hWnd, WM_SETTINGSEX, iSelectedPane);
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
// bSettingSynTPStart()
//
BOOL		bSettingSynTPStart()
{
	return SettingsSynTPStart();
}

//
// bSettingSynTPStop()
//
BOOL		bSettingSynTPStop()
{
	return SettingsSynTPStop();
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
// SettingsMain()
//
SettingsMain::SettingsMain()
{
	InitializeComponent();
}

//
// ~FlushMouseSettings()
//
SettingsMain::~SettingsMain()
{
	return;
}

//
// OnLaunched()
//
void SettingsMain::OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&)
{
	hMicrosoft_ui_xaml_dll = ::LoadLibrary(L"Microsoft.ui.xaml.dll");
	hFlushMouseUI3DLL = ::LoadLibrary(FLUSHMOUSEUI3_DLL);
	
	MojoWindowExec();
	
	if (!bSettingsWinMain((HINSTANCE)_hInstance, NULL, NULL, SW_HIDE)) {
	}

	MojoWindowClose();

	if (hFlushMouseUI3DLL)	::FreeLibrary(hFlushMouseUI3DLL);
	if (hMicrosoft_ui_xaml_dll)	::FreeLibrary(hMicrosoft_ui_xaml_dll);

	PostQuitMessage(0);
	return;
}

//
// OnSuspending()
//
void SettingsMain::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] Windows::ApplicationModel::SuspendingEventArgs const& e)
{
}

//
// GetInstanceFromHWND()
//
HINSTANCE SettingsMain::GetInstanceFromHWND(HWND hWnd)
{
	return (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
}

//
//  bSettingsWinMain()
//
static BOOL		bSettingsWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_opt_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(lpCmdLine);

	HANDLE	hHandle = GetCurrentProcess();
	if (hHandle != NULL) {
		if (!SetPriorityClass(hHandle, NORMAL_PRIORITY_CLASS)) {
			CloseHandle(hHandle);
			//bReportEvent(MSG_STOPPED_FLUSHMOUSE, APPLICATION_CATEGORY);		// Eventlog
			return FALSE;
		}
		CloseHandle(hHandle);
	}

	if (!bSetHeapInformation())	return FALSE;
	
	Resource = new CResource(FLUSHMOUSESETTINGS_EXE);
	if (Resource->hLoad() == NULL) {
		if (Resource)	delete	Resource;
		return FALSE;
	}

	if (LoadString(Resource->hLoad(), IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) {
		if (Resource)	delete	Resource;
		return FALSE;
	}

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	if (!hPrevInstance) {
		if (!MyRegisterClass(hInstance)) {
			if (Resource)	delete	Resource;
			return FALSE;
		}
	}

	HWND	hWnd = NULL;
	if ((hWnd = InitInstance(hInstance, nCmdShow)) == NULL) {
		if (Resource)	delete	Resource;
		return FALSE;
	}
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
	hMainWnd = hWnd;

	MSG		msg{};
	BOOL	bRet = FALSE;
	while (TRUE) {
		try {
			bRet = GetMessage(&msg, NULL, 0, 0);
		}
		catch (...) {
			throw;
		}
		if ((bRet == 0) || (bRet == (-1))) {
			break;
		}
		else {
			HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLUSHMOUSESETTINGS));
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	if (Resource)	delete	Resource;
	return TRUE;
}

//
// bSetHeapInformation()
//
static BOOL		bSetHeapInformation()
{
	HANDLE	hHeap = NULL;
	if ((hHeap = GetProcessHeap()) != NULL) {
		ULONG	HeapInformation = 2;
		if (!HeapSetInformation(hHeap, HeapCompatibilityInformation, &HeapInformation, sizeof(ULONG))) {
			_Post_equals_last_error_ DWORD err = GetLastError();
			if (err != ERROR_INVALID_PARAMETER) {	// 87 (0x57)
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}

//
//  MyRegisterClass()
//
static ATOM MyRegisterClass(HINSTANCE hInstance)
{
#define CLASSSTYLE CS_HREDRAW | CS_VREDRAW

	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CLASSSTYLE;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_FLUSHMOUSE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_FLUSHMOUSE);
	wcex.lpszClassName = CLASS_FLUSHMOUSESETTINGS;
	wcex.hIconSm = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
// InitInstance(HINSTANCE, int)
//
static HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
#define		WINDOWSTYLE		WS_DISABLED
	HWND	hWnd = NULL;
	hWnd = CreateWindowEx(
		0,
		CLASS_FLUSHMOUSESETTINGS,
		szTitle,
		WINDOWSTYLE,
		0, 0,
		0, 0,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!hWnd) {
		return NULL;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return hWnd;
}

//
// WndProc()
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_SETTINGSEX, Cls_OnSettingsEx);
		break;

		default:
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// WM_CREATE
// Cls_OnCreate()
//
static BOOL Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	UNREFERENCED_PARAMETER(lpCreateStruct);
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_TOPMOST)

	hMainWnd = hWnd;

	CHANGEFILTERSTRUCT	cf{};
	cf.cbSize = sizeof(CHANGEFILTERSTRUCT);
	if (!ChangeWindowMessageFilterEx(hWnd, WM_SETTINGSEX, MSGFLT_ALLOW, &cf)) {
	}

	Profile = new CProfile;
	if (Profile != NULL) {
		if (!Profile->bGetProfileData()) {
			vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
	}
	vSettingDialog(hWnd);
	return TRUE;
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
static void Cls_OnDestroy(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);
	
	//vDestroyWindow(hWnd);

	if (Profile != NULL) {
		delete	Profile;
		Profile = NULL;
	}

	PostQuitMessage(0);
}

//
// WM_SETTINGSEX (WM_USER + 0xfe)
// Cls_OnSettingsEx()
//
static bool		Cls_OnSettingsEx(HWND hWnd, int iCode, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lParam);
	if (iCode == SETTINGSEX_CANCEL) {
		vSettingDialogClose();
		return true;
	}

	vSettingDialogApply();

	if (iCode == SETTINGSEX_CHANGE_PANE) {
		if (hWnd) {
			if (lParam == SETTINGSEX_SELECTEDPANE_GENERAL)	{
				iSelectedPane = SETTINGSEX_SELECTEDPANE_GENERAL;
				vSettingDialog(hWnd);
			}
			else if (lParam == SETTINGSEX_SELECTEDPANE_SYNTPHELPER)	{
				iSelectedPane = SETTINGSEX_SELECTEDPANE_SYNTPHELPER;
				vSynTPHelperDialog(hWnd);
			}
			else if (lParam == SETTINGSEX_SELECTEDPANE_ABOUT) {
				iSelectedPane = SETTINGSEX_SELECTEDPANE_ABOUT;
				vAboutDialog(hWnd);
			}
			return true;
		}
		return false;
	}

	if (iCode == SETTINGSEX_OK) {
		vSettingDialogClose();
		return true;
	}
	if (iCode == SETTINGSEX_SYNTP_START) {
		if (Profile) {
			switch (Profile->lpstAppRegData->dwSynTPHelper1) {
				case SYNTPH_DISABLE:
					return false;
				case SYNTPH_SENDERIPV4:
				case SYNTPH_SENDERIPV4_START:
					if (!bIsPrivateAddress(Profile->lpstAppRegData->szSynTPSendIPAddr1)) {
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_TOPMOST)
						vMessageBox(hWnd, IDS_NOTPRIVATEADDR, MessageBoxTYPE);
						return false;
					}
					break;
				case SYNTPH_SENDERHOSNAMEIPV4:
				case SYNTPH_SENDERHOSNAMEIPV4_START:
					if (!bIsPrivateAddress(Profile->lpstAppRegData->szSynTPSendHostname1)) {
						vMessageBox(hWnd, IDS_CANTSYTPHELPER, MessageBoxTYPE);
						return false;
					}
					break;
				case SYNTPH_RECEIVERIPV4:
				case SYNTPH_RECEIVERIPV4_START:
					break;
			}
		}
		return bSettingSynTPStart();
	}
	if (iCode == SETTINGSEX_SYNTP_STOP) {
		return bSettingSynTPStop();
	}
	return true;
}


/* = EOF = */