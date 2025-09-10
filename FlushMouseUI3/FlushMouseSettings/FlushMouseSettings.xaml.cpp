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
static HINSTANCE	m_hInstance{ nullptr };
static int			iSelectedPane = SETTINGSEX_SELECTEDPANE_GENERAL;

//
// Global Prototype Define
//

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
static BOOL		Cls_OnSettingsEx(HWND hWnd, int iCode, int iSubCode);

// 
// vMessageBox()
//
void vMessageBox(HWND hWnd, UINT uID, UINT uType, LPCSTR lpFunc, DWORD dwLine)
{
	TCHAR	_lpFunc[MAX_LOADSTRING]{};
	TCHAR	lpText[(MAX_LOADSTRING * 2)]{};

	LoadString(Resource->hLoad(), uID, lpText, MAX_LOADSTRING);
	if (lpFunc && (dwLine != 0)) {
		MultiByteToWideChar (CP_ACP, 0, lpFunc, -1, _lpFunc, MAX_LOADSTRING);
		_sntprintf_s(lpText, (MAX_LOADSTRING * 2), _TRUNCATE, L"%s\n\n (%s : %d : 0x%08X)", lpText, _lpFunc, dwLine, GetLastError());
	}
	MessageBox(hWnd, lpText, szTitle, uType);
	return;
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
	if (Profile != NULL) {

	}
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
		MessageBox(NULL, L"EXCEPTION", L"FlushMouseSettings", (MB_ICONSTOP | MB_OK));
		});
#endif // _DEBUG

#if defined _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_ASSERTE(_CrtCheckMemory());
#endif
	
	LPCTSTR	_lpCmdLine = GetCommandLine();
	int		iNumArgs = 0;
	LPTSTR	*_lpArgv = CommandLineToArgvW(_lpCmdLine, &iNumArgs);
	if (iNumArgs != 0) {
		if (_lpArgv[1]  != NULL) {
			if (_tcscmp(_lpArgv[1], L"1") == 0) {
				iSelectedPane = SETTINGSEX_SELECTEDPANE_GENERAL;
			}
			else if (_tcscmp(_lpArgv[1], L"2") == 0) {
				iSelectedPane = SETTINGSEX_SELECTEDPANE_IMEMODE;
			}
			else if (_tcscmp(_lpArgv[1], L"3") == 0) {
				iSelectedPane = SETTINGSEX_SELECTEDPANE_SYNTPHELPER;
			}
			else if (_tcscmp(_lpArgv[1], L"4") == 0) {
				iSelectedPane = SETTINGSEX_SELECTEDPANE_ABOUT;
			}
		}
		LocalFree(_lpArgv);
	}

	if ((m_hInstance = GetModuleHandle(FLUSHMOUSESETTINGS_EXE)) == NULL) {
		PostQuitMessage(0);
		return;
	}

	hMicrosoft_ui_xaml_dll = LoadLibrary(L"Microsoft.ui.xaml.dll");
	hFlushMouseUI3DLL = LoadLibrary(FLUSHMOUSEUI3_DLL);
	
	MojoWindowExec();
	
	if (!bSettingsWinMain((HINSTANCE)m_hInstance, NULL, NULL, SW_HIDE)) {
	}

	MojoWindowClose();

	if (hFlushMouseUI3DLL)	FreeLibrary(hFlushMouseUI3DLL);
	if (hMicrosoft_ui_xaml_dll)	FreeLibrary(hMicrosoft_ui_xaml_dll);

#if defined _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); 
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
	
	PostQuitMessage(0);
	return;
}

//
// OnSuspending()
//
void App::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] Windows::ApplicationModel::SuspendingEventArgs const& e)
{
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
			return FALSE;
		}
		CloseHandle(hHandle);
	}

	if (!bSetHeapInformation())	return FALSE;
	
	Resource = new CResource(FLUSHMOUSESETTINGS_EXE);
	if (Resource && Resource->hLoad() == NULL) {
		delete	Resource;
		return FALSE;
	}

	if (Resource && LoadString(Resource->hLoad(), IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) {
		delete	Resource;
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
			if (err != ERROR_INVALID_PARAMETER) {
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
		if (!Profile->bFixChangedProfileData()) {
			vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE, __func__, __LINE__);
			PostMessage(hWnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
			return FALSE;
		}
		if (!Profile->bGetProfileData()) {
			vMessageBox(hWnd, IDS_CANTLOADREG, MessageBoxTYPE, __func__, __LINE__);
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
static BOOL		Cls_OnSettingsEx(HWND hWnd, int iCode, int iSubCode)
{

	if (!hWnd || !Profile)	return FALSE;

	switch (iCode) {
		case SETTINGSEX_OK:	
			vSettingDialogApply();
			if (!Profile->bSetProfileData())	return FALSE;
			vSettingDialogClose();
			return TRUE;
		case SETTINGSEX_CANCEL:
			vSettingDialogClose();
			return TRUE;
		case SETTINGSEX_APPLY:
			vSettingDialogApply();
			if (!Profile->bSetProfileData())	return FALSE;
			return TRUE;
		case SETTINGSEX_RELOAD_REGISTRY:
			return TRUE;
		case SETTINGSEX_RELOAD_MOUSECURSOR:
			vSettingDialogApply();
			if (!Profile->bSetProfileData4Mouse())	return FALSE;
			if (!Profile->bSetProfileData4IMEMode())	return FALSE;
			return TRUE;
		case SETTINGSEX_SETTINGS_CHANGE_PANE:
			vSettingDialogApply();
			iSelectedPane = iSubCode;
			if (!Profile->bSetProfileData4Settings())	return FALSE;
			switch (iSubCode) {
				case SETTINGSEX_SELECTEDPANE_GENERAL:
					vSettingDialog(hWnd);
					break;
				case SETTINGSEX_SELECTEDPANE_IMEMODE:
					vIMEModeDialog(hWnd);
					break;
				case SETTINGSEX_SELECTEDPANE_SYNTPHELPER:
					vSynTPHelperDialog(hWnd);
					break;
				case SETTINGSEX_SELECTEDPANE_ABOUT:
					vAboutDialog(hWnd);
					break;
			}
			return TRUE;
		case SETTINGSEX_SETTINGS_STARTED:
			return TRUE;
		case SETTINGSEX_SETTINGS_SETREGISTRY:
			vSettingDialogApply();
			if (!Profile->bSetProfileData4Settings())	return FALSE;
			return TRUE;
		case SETTINGSEX_SETTINGS_GENERAL_SETREGISTRY:
			vSettingDialogApply();
			if (!Profile->bSetProfileData4Mouse())	return FALSE;
			return TRUE;
		case SETTINGSEX_SETTINGS_IMEMODE_SETREGISTRY:
			vSettingDialogApply();
			if (!Profile->bSetProfileData4IMEMode())	return FALSE;
			return TRUE;
		case SETTINGSEX_SETTINGS_CLOSE:
			return TRUE;
		case SETTINGSEX_SYNTP_START:
			vSettingDialogApply();
			return TRUE;
		case SETTINGSEX_SYNTP_STOP:
		case SETTINGSEX_SYNTP_IS_STARTED:
			return TRUE;
		case SETTINGSEX_SYNTP_SETREGISRY:
			vSettingDialogApply();
			if (!Profile->bSetProfileData4SynTPHelper())	return FALSE;
			return TRUE;
	}
	return TRUE;
}



/* = EOF = */