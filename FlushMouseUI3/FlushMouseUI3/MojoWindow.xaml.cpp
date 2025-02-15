// MojoWindow.Xaml.cpp for FlushMouseUI3
//		Copyright (C) 2022 JeffyTS
//
//	  
// No.	  Date			Name			Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/07/16  JeffyTS  	New edit.
//

//
// Include
//
#include "pch.h"
#include "WinRT.h"

#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime
#include "MojoWindow.xaml.h"
#if __has_include("MojoWindow.g.cpp")
#include "MojoWindow.g.cpp"
#endif
#pragma pop_macro("GetCurrentTime")

#include "FlushMouseUI3.xaml.h"
#include "..\..\FlushMouseLIB\Resource.h"
#include "..\..\FlushMouseLIB\FlushMouseLIB.h"

//
// Using
//
using namespace winrt;

//
// Local Data
//
static Window	wMojoWnd{ nullptr };
static HWND		hMojoWnd{ nullptr };
static FlushMouseUI3DLL::Settings	m_Settings{ nullptr };
static LONG_PTR	m_SettingsWndProc = NULL;

//
// Local Prototype Define
//
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static void			Cls_OnNCLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT codeHitTest);

//
// FlushMouseUI3
//
namespace winrt::FlushMouseUI3::implementation
{
	void		MojoWindowExec()
	{
		if (wMojoWnd == nullptr) {
			try {
				wMojoWnd = make<MojoWindow>();
			}
			catch (const std::exception&) {
			}
			catch (...) {
			}
			if (wMojoWnd != nullptr) {
				auto windowNative{ wMojoWnd.try_as<::IWindowNative>() };
				winrt::check_bool(windowNative);
#pragma warning(push)
#pragma warning(disable : 28112)
				InterlockedIncrement64(reinterpret_cast<LONG64*>(&s_module));
				windowNative->get_WindowHandle(&hMojoWnd);
				InterlockedDecrement64(reinterpret_cast<LONG64*>(&s_module));
#pragma warning(pop)
				Microsoft::UI::WindowId windowId = Microsoft::UI::GetWindowIdFromWindow(hMojoWnd);
				Microsoft::UI::Windowing::AppWindow appWindow = Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);
				if (appWindow) {
					Windows::Graphics::RectInt32	Rect32{};
					Rect32.X = 0;	Rect32.Y = 0;
					Rect32.Width = 0;	Rect32.Height = 0;
					CalcWindowCentralizeRect(hMojoWnd, &Rect32);
					appWindow.MoveAndResize(Rect32);
				}
			}
		}
	}

	void		MojoWindowClose()
	{
		SettingsClose();
		wMojoWnd = nullptr;
	}

	MojoWindow::MojoWindow()
	{
		InitializeComponent();

		auto windowNative{ this->try_as<::IWindowNative>() };
		winrt::check_bool(windowNative);
		windowNative->get_WindowHandle(&hMojoWnd);
	}

	MojoWindow::~MojoWindow()
	{
	}

}

//
// SettingsExec()
//
void		SettingsExec(HWND hWnd, UINT32 uMsg, INT32 iSelectedPane)
{
	if (Profile == nullptr)	return;
	if (m_Settings == nullptr) {
		if (Cursor != NULL) {
			Cursor->vStopDrawIMEModeMouseByWndThread();
		}
		try {
			m_Settings = FlushMouseUI3DLL::Settings(iSelectedPane);
		}
		catch (...) {
		}
		if (m_Settings != nullptr) {
			m_Settings.g_Settings(m_Settings);
			m_Settings.g_hMainWnd((INT64)hWnd);
			m_Settings.g_uMsg((UINT32)uMsg);

			m_Settings.bIsPaneOpen(Profile->lpstAppRegData->bIsPaneOpen);
			m_Settings.dwSettingsX(Profile->lpstAppRegData->dwSettingsX);
			m_Settings.dwSettingsY(Profile->lpstAppRegData->dwSettingsY);
			m_Settings.dwSettingsWidth(Profile->lpstAppRegData->dwSettingsWidth);
			m_Settings.dwSettingsHeight(Profile->lpstAppRegData->dwSettingsHeight);
			
			m_Settings.bDisplayFocusWindowIME(Profile->lpstAppRegData->bDisplayFocusWindowIME);
			m_Settings.dwDisplayIMEModeMethod(Profile->lpstAppRegData->dwDisplayIMEModeMethod);

			m_Settings.bDisplayIMEModeOnCursor(Profile->lpstAppRegData->bDisplayIMEModeOnCursor);
			m_Settings.bDisplayIMEModeIMEOFF(Profile->lpstAppRegData->bDisplayIMEModeIMEOFF);
			m_Settings.bOffChangedFocus(Profile->lpstAppRegData->bOffChangedFocus);
			m_Settings.bForceHiragana(Profile->lpstAppRegData->bForceHiragana);
			m_Settings.bDoModeDispByIMEKeyDown(Profile->lpstAppRegData->bDoModeDispByIMEKeyDown);
			m_Settings.bDoModeDispByMouseBttnUp(Profile->lpstAppRegData->bDoModeDispByMouseBttnUp);
			m_Settings.bDoModeDispByCtrlUp(Profile->lpstAppRegData->bDoModeDispByCtrlUp);
			m_Settings.bDrawNearCaret(Profile->lpstAppRegData->bDrawNearCaret);
			m_Settings.bIMEModeForced(Profile->lpstAppRegData->bIMEModeForced);
			m_Settings.bEnableEPHelper(Profile->lpstAppRegData->bEnableEPHelper);
			
			m_Settings.iCursorSize(Profile->lpstAppRegData->iCursorSize);
			m_Settings.iIMEModeDistance(Profile->lpstAppRegData->iIMEModeDistance);
			m_Settings.iModeMouseSize(Profile->lpstAppRegData->iModeMouseSize);
			m_Settings.iModeCaretSize(Profile->lpstAppRegData->iModeCaretSize);
			m_Settings.iModeByWndSize(Profile->lpstAppRegData->iModeByWndSize);
			m_Settings.iModeMouseDistanceX(Profile->lpstAppRegData->iModeMouseDistanceX);
			m_Settings.iModeCaretDistanceX(Profile->lpstAppRegData->iModeCaretDistanceX);
			m_Settings.dwDisplayModeTime(Profile->lpstAppRegData->dwDisplayModeTime);
			m_Settings.dwAdditionalWaitTime(Profile->lpstAppRegData->dwAdditionalWaitTime);
			m_Settings.dwWaitWaveTime(Profile->lpstAppRegData->dwWaitWaveTime);
			
			m_Settings.szNearDrawMouseIMEOFFChar(Profile->lpstAppRegData->szNearDrawMouseIMEOFFChar);
			m_Settings.szNearDrawMouseHANEISU_IMEONChar(Profile->lpstAppRegData->szNearDrawMouseHANEISU_IMEONChar);
			m_Settings.szNearDrawMouseHANKANA_IMEONChar(Profile->lpstAppRegData->szNearDrawMouseHANKANA_IMEONChar);
			m_Settings.szNearDrawMouseZENEISU_IMEONChar(Profile->lpstAppRegData->szNearDrawMouseZENEISU_IMEONChar);
			m_Settings.szNearDrawMouseZENHIRA_IMEONChar(Profile->lpstAppRegData->szNearDrawMouseZENHIRA_IMEONChar);
			m_Settings.szNearDrawMouseZENKANA_IMEONChar(Profile->lpstAppRegData->szNearDrawMouseZENKANA_IMEONChar);
			m_Settings.szNearDrawCaretIMEOFFChar(Profile->lpstAppRegData->szNearDrawCaretIMEOFFChar);
			m_Settings.szNearDrawCaretHANEISU_IMEONChar(Profile->lpstAppRegData->szNearDrawCaretHANEISU_IMEONChar);
			m_Settings.szNearDrawCaretHANKANA_IMEONChar(Profile->lpstAppRegData->szNearDrawCaretHANKANA_IMEONChar);
			m_Settings.szNearDrawCaretZENEISU_IMEONChar(Profile->lpstAppRegData->szNearDrawCaretZENEISU_IMEONChar);
			m_Settings.szNearDrawCaretZENHIRA_IMEONChar(Profile->lpstAppRegData->szNearDrawCaretZENHIRA_IMEONChar);
			m_Settings.szNearDrawCaretZENKANA_IMEONChar(Profile->lpstAppRegData->szNearDrawCaretZENKANA_IMEONChar);
			m_Settings.szNearDrawMouseByWndIMEOFFChar(Profile->lpstAppRegData->szNearDrawMouseByWndIMEOFFChar);
			m_Settings.szNearDrawMouseByWndHANEISU_IMEONChar(Profile->lpstAppRegData->szNearDrawMouseByWndHANEISU_IMEONChar);
			m_Settings.szNearDrawMouseByWndHANKANA_IMEONChar(Profile->lpstAppRegData->szNearDrawMouseByWndHANKANA_IMEONChar);
			m_Settings.szNearDrawMouseByWndZENEISU_IMEONChar(Profile->lpstAppRegData->szNearDrawMouseByWndZENEISU_IMEONChar);
			m_Settings.szNearDrawMouseByWndZENHIRA_IMEONChar(Profile->lpstAppRegData->szNearDrawMouseByWndZENHIRA_IMEONChar);
			m_Settings.szNearDrawMouseByWndZENKANA_IMEONChar(Profile->lpstAppRegData->szNearDrawMouseByWndZENKANA_IMEONChar);
			
			m_Settings.dwNearDrawMouseIMEOFFColor(Profile->lpstAppRegData->dwNearDrawMouseIMEOFFColor);
			m_Settings.dwNearDrawMouseHANEISU_IMEONColor(Profile->lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor);
			m_Settings.dwNearDrawMouseHANKANA_IMEONColor(Profile->lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor);
			m_Settings.dwNearDrawMouseZENEISU_IMEONColor(Profile->lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor);
			m_Settings.dwNearDrawMouseZENHIRA_IMEONColor(Profile->lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor);
			m_Settings.dwNearDrawMouseZENKANA_IMEONColor(Profile->lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor);
			m_Settings.dwNearDrawCaretIMEOFFColor(Profile->lpstAppRegData->dwNearDrawCaretIMEOFFColor);
			m_Settings.dwNearDrawCaretHANEISU_IMEONColor(Profile->lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor);
			m_Settings.dwNearDrawCaretHANKANA_IMEONColor(Profile->lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor);
			m_Settings.dwNearDrawCaretZENEISU_IMEONColor(Profile->lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor);
			m_Settings.dwNearDrawCaretZENHIRA_IMEONColor(Profile->lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor);
			m_Settings.dwNearDrawCaretZENKANA_IMEONColor(Profile->lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor);
			m_Settings.dwNearDrawMouseByWndIMEOFFColor(Profile->lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor);
			m_Settings.dwNearDrawMouseByWndHANEISU_IMEONColor(Profile->lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor);
			m_Settings.dwNearDrawMouseByWndHANKANA_IMEONColor(Profile->lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor);
			m_Settings.dwNearDrawMouseByWndZENEISU_IMEONColor(Profile->lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor);
			m_Settings.dwNearDrawMouseByWndZENHIRA_IMEONColor(Profile->lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor);
			m_Settings.dwNearDrawMouseByWndZENKANA_IMEONColor(Profile->lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor);
			
			m_Settings.szNearDrawMouseFont(Profile->lpstAppRegData->szNearDrawMouseFont);
			m_Settings.szNearDrawCaretFont(Profile->lpstAppRegData->szNearDrawCaretFont);
			m_Settings.szNearDrawMouseByWndFont(Profile->lpstAppRegData->szNearDrawMouseByWndFont);

			m_Settings.dwSynTPHelper1(Profile->lpstAppRegData->dwSynTPHelper1);
			m_Settings.dwSynTPPadX(Profile->lpstAppRegData->dwSynTPPadX);
			m_Settings.dwSynTPPadY(Profile->lpstAppRegData->dwSynTPPadY);
			m_Settings.dwSynTPEdgeX(Profile->lpstAppRegData->dwSynTPEdgeX);
			m_Settings.dwSynTPEdgeY(Profile->lpstAppRegData->dwSynTPEdgeY);
			TCHAR	addr1[4]{};
			TCHAR	addr2[4]{};
			TCHAR	addr3[4]{};
			TCHAR	addr4[4]{};
			if (!bGetString2IPv4Addr(Profile->lpstAppRegData->szSynTPSendIPAddr1, addr1, addr2, addr3, addr4)) {
			}
			m_Settings.szSynTPSendIPAddr1_1(addr1);
			m_Settings.szSynTPSendIPAddr1_2(addr2);
			m_Settings.szSynTPSendIPAddr1_3(addr3);
			m_Settings.szSynTPSendIPAddr1_4(addr4);
			m_Settings.szSynTPSendHostname1(Profile->lpstAppRegData->szSynTPSendHostname1);
			m_Settings.dwSynTPPortNo1(Profile->lpstAppRegData->dwSynTPPortNo1);
			if (_tcsncpy_s(Profile->lpstAppRegData->szSynTPSendHostname1, MAX_FQDN, (LPTSTR)(m_Settings.szSynTPSendHostname1().c_str()), _TRUNCATE) != 0) {
			}
			m_Settings.bSynTPStarted1(Profile->lpstAppRegData->bSynTPStarted1);

			HWND	_hSettingsWnd = (HWND)m_Settings.g_hSettingsWnd();
			if (_hSettingsWnd) {
				m_SettingsWndProc = GetWindowLongPtr(_hSettingsWnd, GWLP_WNDPROC);
				if (m_SettingsWndProc) {
					if (SetWindowLongPtr(_hSettingsWnd, GWLP_WNDPROC, (LONG_PTR)WndProc)) {
						return;
					}
				}
			}
			HWND	_hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL);
			if (_hWnd != NULL) {
				SendMessage(_hWnd, WM_SETTINGSEX, SETTINGSEX_SETTINGS_STARTED, 0);
			}
		}
		else {
#define MessageBoxTYPE (MB_ICONSTOP | MB_OK | MB_TOPMOST)
			vMessageBox(hWnd, IDS_SETTINGDLG_FAIL, MessageBoxTYPE, __func__, __LINE__);
			SettingsClose();
		}
	}
	else {
		m_Settings.ChangePane(iSelectedPane);
		if ((HWND)m_Settings.g_hSettingsWnd() != NULL) {
			if (SynTP)	m_Settings.bSynTPStarted1(true);
			else m_Settings.bSynTPStarted1(false);
			SetForegroundWindow((HWND)m_Settings.g_hSettingsWnd());
		}
	}
}

//
// WndProc()
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		HANDLE_MSG(hWnd, WM_NCLBUTTONDBLCLK, Cls_OnNCLButtonDown);
		break;

		default:
			break;
	}
	return CallWindowProc((WNDPROC)m_SettingsWndProc, hWnd, message, wParam, lParam);
}

//
// WM_NCLBUTTONDBLCLK
// Cls_OnNCLButtonDown()
//
void			Cls_OnNCLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT codeHitTest)
{
	UNREFERENCED_PARAMETER(hwnd);
	UNREFERENCED_PARAMETER(fDoubleClick);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(codeHitTest);
	return;
}

//
// SettingsApply()
//
void		SettingsApply()
{
	if (Profile == nullptr)	return;
	if (m_Settings != nullptr) {
		Profile->lpstAppRegData->bIsPaneOpen = m_Settings.bIsPaneOpen();
		Profile->lpstAppRegData->dwSettingsX = m_Settings.dwSettingsX();
		Profile->lpstAppRegData->dwSettingsY = m_Settings.dwSettingsY();
		Profile->lpstAppRegData->dwSettingsWidth = m_Settings.dwSettingsWidth();
		Profile->lpstAppRegData->dwSettingsHeight = m_Settings.dwSettingsHeight();
		
		Profile->lpstAppRegData->bDisplayFocusWindowIME = m_Settings.bDisplayFocusWindowIME();
		Profile->lpstAppRegData->dwDisplayIMEModeMethod = m_Settings.dwDisplayIMEModeMethod();

		Profile->lpstAppRegData->bDisplayIMEModeOnCursor = m_Settings.bDisplayIMEModeOnCursor();
		Profile->lpstAppRegData->bDisplayIMEModeIMEOFF = m_Settings.bDisplayIMEModeIMEOFF();
		Profile->lpstAppRegData->bOffChangedFocus = m_Settings.bOffChangedFocus();
		Profile->lpstAppRegData->bForceHiragana = m_Settings.bForceHiragana();
		Profile->lpstAppRegData->bDoModeDispByIMEKeyDown = m_Settings.bDoModeDispByIMEKeyDown();
		Profile->lpstAppRegData->bDoModeDispByMouseBttnUp = m_Settings.bDoModeDispByMouseBttnUp();
		Profile->lpstAppRegData->bDoModeDispByCtrlUp = m_Settings.bDoModeDispByCtrlUp();
		Profile->lpstAppRegData->bDrawNearCaret = m_Settings.bDrawNearCaret();
		Profile->lpstAppRegData->bIMEModeForced = m_Settings.bIMEModeForced();
		Profile->lpstAppRegData->bEnableEPHelper = m_Settings.bEnableEPHelper();
		
		Profile->lpstAppRegData->iCursorSize = m_Settings.iCursorSize();
		Profile->lpstAppRegData->iIMEModeDistance = m_Settings.iIMEModeDistance();
		Profile->lpstAppRegData->iModeMouseSize = m_Settings.iModeMouseSize();
		Profile->lpstAppRegData->iModeCaretSize = m_Settings.iModeCaretSize();
		Profile->lpstAppRegData->iModeByWndSize = m_Settings.iModeByWndSize();
		Profile->lpstAppRegData->iModeMouseDistanceX = m_Settings.iModeMouseDistanceX();
		Profile->lpstAppRegData->iModeCaretDistanceX = m_Settings.iModeCaretDistanceX();
		Profile->lpstAppRegData->dwDisplayModeTime = m_Settings.dwDisplayModeTime();
		Profile->lpstAppRegData->dwAdditionalWaitTime = m_Settings.dwAdditionalWaitTime();
		Profile->lpstAppRegData->dwWaitWaveTime = m_Settings.dwWaitWaveTime();
				
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseIMEOFFChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseIMEOFFChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseHANEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseHANEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseHANKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseHANKANA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseZENEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseZENEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseZENHIRA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseZENHIRA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseZENKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseZENKANA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawCaretIMEOFFChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretIMEOFFChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawCaretHANEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretHANEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawCaretHANKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretHANKANA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawCaretZENEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretZENEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawCaretZENHIRA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretZENHIRA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawCaretZENKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretZENKANA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseByWndIMEOFFChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndIMEOFFChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseByWndHANEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndHANEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseByWndHANKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndHANKANA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseByWndZENEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndZENEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseByWndZENHIRA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndZENHIRA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseByWndZENKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndZENKANA_IMEONChar().c_str()), _TRUNCATE);
		
		Profile->lpstAppRegData->dwNearDrawMouseIMEOFFColor = m_Settings.dwNearDrawMouseIMEOFFColor();
		Profile->lpstAppRegData->dwNearDrawMouseHANEISU_IMEONColor = m_Settings.dwNearDrawMouseHANEISU_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawMouseHANKANA_IMEONColor = m_Settings.dwNearDrawMouseHANKANA_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawMouseZENEISU_IMEONColor = m_Settings.dwNearDrawMouseZENEISU_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawMouseZENHIRA_IMEONColor = m_Settings.dwNearDrawMouseZENHIRA_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawMouseZENKANA_IMEONColor = m_Settings.dwNearDrawMouseZENKANA_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawCaretIMEOFFColor = m_Settings.dwNearDrawCaretIMEOFFColor();
		Profile->lpstAppRegData->dwNearDrawCaretHANEISU_IMEONColor = m_Settings.dwNearDrawCaretHANEISU_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawCaretHANKANA_IMEONColor = m_Settings.dwNearDrawCaretHANKANA_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawCaretZENEISU_IMEONColor = m_Settings.dwNearDrawCaretZENEISU_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawCaretZENHIRA_IMEONColor = m_Settings.dwNearDrawCaretZENHIRA_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawCaretZENKANA_IMEONColor = m_Settings.dwNearDrawCaretZENKANA_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawMouseByWndIMEOFFColor = m_Settings.dwNearDrawMouseByWndIMEOFFColor();
		Profile->lpstAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor = m_Settings.dwNearDrawMouseByWndHANEISU_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor = m_Settings.dwNearDrawMouseByWndHANKANA_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor = m_Settings.dwNearDrawMouseByWndZENEISU_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor = m_Settings.dwNearDrawMouseByWndZENHIRA_IMEONColor();
		Profile->lpstAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor = m_Settings.dwNearDrawMouseByWndZENKANA_IMEONColor();
		
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseFont, LF_FACESIZE, (LPCTSTR)(m_Settings.szNearDrawMouseFont().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawCaretFont, LF_FACESIZE, (LPCTSTR)(m_Settings.szNearDrawCaretFont().c_str()), _TRUNCATE);
		_tcsncpy_s(Profile->lpstAppRegData->szNearDrawMouseByWndFont, LF_FACESIZE, (LPCTSTR)(m_Settings.szNearDrawMouseByWndFont().c_str()), _TRUNCATE);

		Profile->lpstAppRegData->dwSynTPHelper1 = m_Settings.dwSynTPHelper1();
		Profile->lpstAppRegData->dwSynTPPadX = m_Settings.dwSynTPPadX();
		Profile->lpstAppRegData->dwSynTPPadY = m_Settings.dwSynTPPadY();
		Profile->lpstAppRegData->dwSynTPEdgeX = m_Settings.dwSynTPEdgeX();
		Profile->lpstAppRegData->dwSynTPEdgeY = m_Settings.dwSynTPEdgeY();
		ZeroMemory(Profile->lpstAppRegData->szSynTPSendIPAddr1, (MAX_IPV4_ADDRESS * sizeof(TCHAR)));
		if (_sntprintf_s(Profile->lpstAppRegData->szSynTPSendIPAddr1, MAX_IPV4_ADDRESS, _TRUNCATE, L"%s.%s.%s.%s", 
				(LPCTSTR)(m_Settings.szSynTPSendIPAddr1_1().c_str()), (LPCTSTR)(m_Settings.szSynTPSendIPAddr1_2().c_str()), (LPCTSTR)(m_Settings.szSynTPSendIPAddr1_3().c_str()), (LPCTSTR)(m_Settings.szSynTPSendIPAddr1_4().c_str())) <= 0) {
		}
		ZeroMemory(Profile->lpstAppRegData->szSynTPSendHostname1, (MAX_FQDN * sizeof(TCHAR)));
		if (_tcsncpy_s(Profile->lpstAppRegData->szSynTPSendHostname1, MAX_FQDN, (LPCTSTR)(m_Settings.szSynTPSendHostname1().c_str()), _TRUNCATE) != 0) {
		}
		Profile->lpstAppRegData->dwSynTPPortNo1 = m_Settings.dwSynTPPortNo1();
		Profile->lpstAppRegData->bSynTPStarted1 = m_Settings.bSynTPStarted1();
	}
	Profile->bSetProfileData();
}

//
// SettingsClose()
//
void		SettingsClose()
{
	if (m_Settings != nullptr) {
		if ((HWND)m_Settings.g_hSettingsWnd() != nullptr) {
			m_Settings.g_hSettingsWnd(NULL);
		}
		m_Settings.Close();
		m_Settings = nullptr;
	}
}


/* = EOF = */