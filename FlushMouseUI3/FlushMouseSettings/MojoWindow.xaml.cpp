//
// MojoWindow.Xaml.cpp for FlushMouseSettings
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
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

#include "FlushMouseSettings.xaml.h"
#include "..\..\FlushMouseLIB\FlushMouseLIB.h"
#include "..\..\FlushMouseLIB\Profile.h"
#include "..\..\FlushMouseLIB\CommonDef.h"

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
static LONG_PTR	m_SettingsWndProc = 0;

//
// Local Prototype Define
//
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static void			Cls_OnNCLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT codeHitTest);
static BOOL			SetWindowProcSafe(HWND hwnd, WNDPROC newProc, LONG_PTR *pPrevProc);

//
// FlushMouseSettings
//
namespace winrt::FlushMouseSettings::implementation
{
	void MojoWindowExec()
	{
		if (wMojoWnd == nullptr) {
			try {
				wMojoWnd = make<MojoWindow>();
			}
			catch (winrt::hresult_error const & e) {
				UNREFERENCED_PARAMETER(e);
				return;
			}
			catch (std::exception const & e) {
				UNREFERENCED_PARAMETER(e);
				return;
			}
			catch (...) {
				return;
			}
			if (wMojoWnd != nullptr) {
				auto windowNative{ wMojoWnd.try_as<::IWindowNative>() };
				if (!windowNative) {
					return;
				}
#pragma warning(push)
#pragma warning(disable : 28112)
				InterlockedIncrement64(reinterpret_cast<LONG64*>(&s_module));
				HRESULT hr = S_OK;
				try {
					windowNative->get_WindowHandle(&hMojoWnd);
				}
				catch (winrt::hresult_error const & e) {
					hr = static_cast<HRESULT>(e.code());
				}
				InterlockedDecrement64(reinterpret_cast<LONG64*>(&s_module));
#pragma warning(pop)
				if (hMojoWnd == NULL) {
					return;
				}
				Microsoft::UI::WindowId windowId = Microsoft::UI::GetWindowIdFromWindow(hMojoWnd);
				Microsoft::UI::Windowing::AppWindow appWindow = Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);
				if (appWindow) {
					Windows::Graphics::RectInt32 Rect32{};
					Rect32.X = 0;	Rect32.Y = 0;
					Rect32.Width = 0;	Rect32.Height = 0;
					CalcWindowCentralizeRect(hMojoWnd, &Rect32);
					try {
						appWindow.MoveAndResize(Rect32);
					}
					catch (winrt::hresult_error const & e) {
						UNREFERENCED_PARAMETER(e);
					}
					catch (...) {
					}
				}
			}
		}
	}

	void MojoWindowClose()
	{
		SettingsClose();
		wMojoWnd = nullptr;
		hMojoWnd = nullptr;
	}

	MojoWindow::MojoWindow()
	{
		InitializeComponent();

		auto windowNative{ this->try_as<::IWindowNative>() };
		if (windowNative) {
			try {
				windowNative->get_WindowHandle(&hMojoWnd);
			}
			catch (winrt::hresult_error const & e) {
				UNREFERENCED_PARAMETER(e);
			}
			catch (...) {
			}
		}
		else {
		}
	}

	MojoWindow::~MojoWindow()
	{
	}
}

//
// SettingsExec()
//
void SettingsExec(HWND hWnd, UINT32 uMsg, INT32 iSelectedPane)
{
	if (Profile == nullptr) return;

	if (m_Settings == nullptr) {
		try {
			m_Settings = FlushMouseUI3DLL::Settings(iSelectedPane);
		}
		catch (winrt::hresult_error const & e) {
			UNREFERENCED_PARAMETER(e);
			return;
		}
		catch (std::exception const & e) {
			UNREFERENCED_PARAMETER(e);
			return;
		}
		catch (...) {
			return;
		}

		if (!m_Settings) {
			return;
		}

		try {
			const LPAPPREGDATA lpAppRegData = Profile->lpstAppRegData;

			m_Settings.g_Settings(m_Settings);
			m_Settings.g_hMainWnd((INT64)hWnd);
			m_Settings.g_uMsg((UINT32)uMsg);

			m_Settings.bIsPaneOpen(lpAppRegData->bIsPaneOpen);
			m_Settings.dwSettingsX(lpAppRegData->dwSettingsX);
			m_Settings.dwSettingsY(lpAppRegData->dwSettingsY);
			m_Settings.dwSettingsWidth(lpAppRegData->dwSettingsWidth);
			m_Settings.dwSettingsHeight(lpAppRegData->dwSettingsHeight);

			m_Settings.bDisplayFocusWindowIME(lpAppRegData->bDisplayFocusWindowIME);
			m_Settings.dwDisplayIMEModeMethod(lpAppRegData->dwDisplayIMEModeMethod);

			m_Settings.bDisplayIMEModeOnCursor(lpAppRegData->bDisplayIMEModeOnCursor);
			m_Settings.bDisplayIMEModeIMEOFF(lpAppRegData->bDisplayIMEModeIMEOFF);
			m_Settings.bOffChangedFocus(lpAppRegData->bOffChangedFocus);
			m_Settings.bForceHiragana(lpAppRegData->bForceHiragana);
			m_Settings.bDoModeDispByIMEKeyDown(lpAppRegData->bDoModeDispByIMEKeyDown);
			m_Settings.bDoModeDispByMouseBttnUp(lpAppRegData->bDoModeDispByMouseBttnUp);
			m_Settings.bDoModeDispByCtrlUp(lpAppRegData->bDoModeDispByCtrlUp);
			m_Settings.bDrawNearCaret(lpAppRegData->bDrawNearCaret);
			m_Settings.bSupportVirtualDesktop(lpAppRegData->bSupportVirtualDesktop);
			m_Settings.bIMEModeForced(lpAppRegData->bIMEModeForced);
			m_Settings.bEnableEPHelper(lpAppRegData->bEnableEPHelper);

			m_Settings.iCursorSize(lpAppRegData->iCursorSize);
			m_Settings.iIMEModeDistance(lpAppRegData->iIMEModeDistance);
			m_Settings.iModeMouseSize(lpAppRegData->iModeMouseSize);
			m_Settings.iModeCaretSize(lpAppRegData->iModeCaretSize);
			m_Settings.iModeByWndSize(lpAppRegData->iModeByWndSize);
			m_Settings.iModeMouseDistanceX(lpAppRegData->iModeMouseDistanceX);
			m_Settings.iModeCaretDistanceX(lpAppRegData->iModeCaretDistanceX);
			m_Settings.dwDisplayModeTime(lpAppRegData->dwDisplayModeTime);
			m_Settings.dwAdditionalWaitTime(lpAppRegData->dwAdditionalWaitTime);
			m_Settings.dwWaitWaveTime(lpAppRegData->dwWaitWaveTime);

			m_Settings.szNearDrawMouseIMEOFFChar(lpAppRegData->szNearDrawMouseIMEOFFChar);
			m_Settings.szNearDrawMouseHANEISU_IMEONChar(lpAppRegData->szNearDrawMouseHANEISU_IMEONChar);
			m_Settings.szNearDrawMouseHANKANA_IMEONChar(lpAppRegData->szNearDrawMouseHANKANA_IMEONChar);
			m_Settings.szNearDrawMouseZENEISU_IMEONChar(lpAppRegData->szNearDrawMouseZENEISU_IMEONChar);
			m_Settings.szNearDrawMouseZENHIRA_IMEONChar(lpAppRegData->szNearDrawMouseZENHIRA_IMEONChar);
			m_Settings.szNearDrawMouseZENKANA_IMEONChar(lpAppRegData->szNearDrawMouseZENKANA_IMEONChar);
			m_Settings.szNearDrawCaretIMEOFFChar(lpAppRegData->szNearDrawCaretIMEOFFChar);
			m_Settings.szNearDrawCaretHANEISU_IMEONChar(lpAppRegData->szNearDrawCaretHANEISU_IMEONChar);
			m_Settings.szNearDrawCaretHANKANA_IMEONChar(lpAppRegData->szNearDrawCaretHANKANA_IMEONChar);
			m_Settings.szNearDrawCaretZENEISU_IMEONChar(lpAppRegData->szNearDrawCaretZENEISU_IMEONChar);
			m_Settings.szNearDrawCaretZENHIRA_IMEONChar(lpAppRegData->szNearDrawCaretZENHIRA_IMEONChar);
			m_Settings.szNearDrawCaretZENKANA_IMEONChar(lpAppRegData->szNearDrawCaretZENKANA_IMEONChar);
			m_Settings.szNearDrawMouseByWndIMEOFFChar(lpAppRegData->szNearDrawMouseByWndIMEOFFChar);
			m_Settings.szNearDrawMouseByWndHANEISU_IMEONChar(lpAppRegData->szNearDrawMouseByWndHANEISU_IMEONChar);
			m_Settings.szNearDrawMouseByWndHANKANA_IMEONChar(lpAppRegData->szNearDrawMouseByWndHANKANA_IMEONChar);
			m_Settings.szNearDrawMouseByWndZENEISU_IMEONChar(lpAppRegData->szNearDrawMouseByWndZENEISU_IMEONChar);
			m_Settings.szNearDrawMouseByWndZENHIRA_IMEONChar(lpAppRegData->szNearDrawMouseByWndZENHIRA_IMEONChar);
			m_Settings.szNearDrawMouseByWndZENKANA_IMEONChar(lpAppRegData->szNearDrawMouseByWndZENKANA_IMEONChar);

			m_Settings.dwNearDrawMouseIMEOFFColor(lpAppRegData->dwNearDrawMouseIMEOFFColor);
			m_Settings.dwNearDrawMouseHANEISU_IMEONColor(lpAppRegData->dwNearDrawMouseHANEISU_IMEONColor);
			m_Settings.dwNearDrawMouseHANKANA_IMEONColor(lpAppRegData->dwNearDrawMouseHANKANA_IMEONColor);
			m_Settings.dwNearDrawMouseZENEISU_IMEONColor(lpAppRegData->dwNearDrawMouseZENEISU_IMEONColor);
			m_Settings.dwNearDrawMouseZENHIRA_IMEONColor(lpAppRegData->dwNearDrawMouseZENHIRA_IMEONColor);
			m_Settings.dwNearDrawMouseZENKANA_IMEONColor(lpAppRegData->dwNearDrawMouseZENKANA_IMEONColor);
			m_Settings.dwNearDrawCaretIMEOFFColor(lpAppRegData->dwNearDrawCaretIMEOFFColor);
			m_Settings.dwNearDrawCaretHANEISU_IMEONColor(lpAppRegData->dwNearDrawCaretHANEISU_IMEONColor);
			m_Settings.dwNearDrawCaretHANKANA_IMEONColor(lpAppRegData->dwNearDrawCaretHANKANA_IMEONColor);
			m_Settings.dwNearDrawCaretZENEISU_IMEONColor(lpAppRegData->dwNearDrawCaretZENEISU_IMEONColor);
			m_Settings.dwNearDrawCaretZENHIRA_IMEONColor(lpAppRegData->dwNearDrawCaretZENHIRA_IMEONColor);
			m_Settings.dwNearDrawCaretZENKANA_IMEONColor(lpAppRegData->dwNearDrawCaretZENKANA_IMEONColor);
			m_Settings.dwNearDrawMouseByWndIMEOFFColor(lpAppRegData->dwNearDrawMouseByWndIMEOFFColor);
			m_Settings.dwNearDrawMouseByWndHANEISU_IMEONColor(lpAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor);
			m_Settings.dwNearDrawMouseByWndHANKANA_IMEONColor(lpAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor);
			m_Settings.dwNearDrawMouseByWndZENEISU_IMEONColor(lpAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor);
			m_Settings.dwNearDrawMouseByWndZENHIRA_IMEONColor(lpAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor);
			m_Settings.dwNearDrawMouseByWndZENKANA_IMEONColor(lpAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor);

			m_Settings.szNearDrawMouseFont(lpAppRegData->szNearDrawMouseFont);
			m_Settings.szNearDrawCaretFont(lpAppRegData->szNearDrawCaretFont);
			m_Settings.szNearDrawMouseByWndFont(lpAppRegData->szNearDrawMouseByWndFont);

			m_Settings.dwSynTPHelper1(lpAppRegData->dwSynTPHelper1);
			m_Settings.dwSynTPPadX(lpAppRegData->dwSynTPPadX);
			m_Settings.dwSynTPPadY(lpAppRegData->dwSynTPPadY);
			m_Settings.dwSynTPEdgeX(lpAppRegData->dwSynTPEdgeX);
			m_Settings.dwSynTPEdgeY(lpAppRegData->dwSynTPEdgeY);
			TCHAR addr1[4]{};
			TCHAR addr2[4]{};
			TCHAR addr3[4]{};
			TCHAR addr4[4]{};
			if (!bGetString2IPv4Addr(lpAppRegData->szSynTPSendIPAddr1, addr1, addr2, addr3, addr4)) {
			}
			m_Settings.szSynTPSendIPAddr1_1(addr1);
			m_Settings.szSynTPSendIPAddr1_2(addr2);
			m_Settings.szSynTPSendIPAddr1_3(addr3);
			m_Settings.szSynTPSendIPAddr1_4(addr4);
			m_Settings.szSynTPSendHostname1(lpAppRegData->szSynTPSendHostname1);
			m_Settings.dwSynTPPortNo1(lpAppRegData->dwSynTPPortNo1);
			_tcsncpy_s(lpAppRegData->szSynTPSendHostname1, MAX_FQDN, (LPTSTR)(m_Settings.szSynTPSendHostname1().c_str()), _TRUNCATE);
			m_Settings.bSynTPStarted1(lpAppRegData->bSynTPStarted1);
		}
		catch (winrt::hresult_error const & e) {
			UNREFERENCED_PARAMETER(e);
			return;
		}
		catch (...) {
			return;
		}

		HWND _hSettingsWnd = (HWND)m_Settings.g_hSettingsWnd();
		if (_hSettingsWnd) {
			if (!SetWindowProcSafe(_hSettingsWnd, WndProc, &m_SettingsWndProc)) {
			}
			else {
				HWND _hWnd = FindWindow(CLASS_FLUSHMOUSE, NULL);
				if (_hWnd != NULL) {
					SendMessage(_hWnd, WM_SETTINGSEX, SETTINGSEX_SETTINGS_STARTED, 0);
				}
			}
		}
	}
	else {
		try {
			m_Settings.ChangePane(iSelectedPane);
			if ((HWND)m_Settings.g_hSettingsWnd() != NULL) {
				if (SynTP) m_Settings.bSynTPStarted1(true);
				else m_Settings.bSynTPStarted1(false);
				SetForegroundWindow((HWND)m_Settings.g_hSettingsWnd());
			}
		}
		catch (winrt::hresult_error const & e) {
			UNREFERENCED_PARAMETER(e);
		}
		catch (...) {
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
	if (m_SettingsWndProc) {
		return CallWindowProc(reinterpret_cast<WNDPROC>(m_SettingsWndProc), hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// WM_NCLBUTTONDBLCLK
// Cls_OnNCLButtonDown()
//
void Cls_OnNCLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT codeHitTest)
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
void SettingsApply()
{
	if (Profile == nullptr) return;
	const LPAPPREGDATA lpAppRegData = Profile->lpstAppRegData;
	if (lpAppRegData == nullptr) return;

	if (m_Settings != nullptr) {
		lpAppRegData->bIsPaneOpen = m_Settings.bIsPaneOpen();
		lpAppRegData->dwSettingsX = m_Settings.dwSettingsX();
		lpAppRegData->dwSettingsY = m_Settings.dwSettingsY();
		lpAppRegData->dwSettingsWidth = m_Settings.dwSettingsWidth();
		lpAppRegData->dwSettingsHeight = m_Settings.dwSettingsHeight();

		lpAppRegData->bDisplayFocusWindowIME = m_Settings.bDisplayFocusWindowIME();
		lpAppRegData->dwDisplayIMEModeMethod = m_Settings.dwDisplayIMEModeMethod();

		lpAppRegData->bDisplayIMEModeOnCursor = m_Settings.bDisplayIMEModeOnCursor();
		lpAppRegData->bDisplayIMEModeIMEOFF = m_Settings.bDisplayIMEModeIMEOFF();
		lpAppRegData->bOffChangedFocus = m_Settings.bOffChangedFocus();
		lpAppRegData->bForceHiragana = m_Settings.bForceHiragana();
		lpAppRegData->bDoModeDispByIMEKeyDown = m_Settings.bDoModeDispByIMEKeyDown();
		lpAppRegData->bDoModeDispByMouseBttnUp = m_Settings.bDoModeDispByMouseBttnUp();
		lpAppRegData->bDoModeDispByCtrlUp = m_Settings.bDoModeDispByCtrlUp();
		lpAppRegData->bDrawNearCaret = m_Settings.bDrawNearCaret();
		lpAppRegData->bSupportVirtualDesktop = m_Settings.bSupportVirtualDesktop();
		lpAppRegData->bIMEModeForced = m_Settings.bIMEModeForced();
		lpAppRegData->bEnableEPHelper = m_Settings.bEnableEPHelper();

		lpAppRegData->iCursorSize = m_Settings.iCursorSize();
		lpAppRegData->iIMEModeDistance = m_Settings.iIMEModeDistance();
		lpAppRegData->iModeMouseSize = m_Settings.iModeMouseSize();
		lpAppRegData->iModeCaretSize = m_Settings.iModeCaretSize();
		lpAppRegData->iModeByWndSize = m_Settings.iModeByWndSize();
		lpAppRegData->iModeMouseDistanceX = m_Settings.iModeMouseDistanceX();
		lpAppRegData->iModeCaretDistanceX = m_Settings.iModeCaretDistanceX();
		lpAppRegData->dwDisplayModeTime = m_Settings.dwDisplayModeTime();
		lpAppRegData->dwAdditionalWaitTime = m_Settings.dwAdditionalWaitTime();
		lpAppRegData->dwWaitWaveTime = m_Settings.dwWaitWaveTime();

		_tcsncpy_s(lpAppRegData->szNearDrawMouseIMEOFFChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseIMEOFFChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseHANEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseHANEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseHANKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseHANKANA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseZENEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseZENEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseZENHIRA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseZENHIRA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseZENKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseZENKANA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawCaretIMEOFFChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretIMEOFFChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawCaretHANEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretHANEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawCaretHANKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretHANKANA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawCaretZENEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretZENEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawCaretZENHIRA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretZENHIRA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawCaretZENKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawCaretZENKANA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseByWndIMEOFFChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndIMEOFFChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseByWndHANEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndHANEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseByWndHANKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndHANKANA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseByWndZENEISU_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndZENEISU_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseByWndZENHIRA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndZENHIRA_IMEONChar().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseByWndZENKANA_IMEONChar, MAX_IMEMODECHAR, (LPCTSTR)(m_Settings.szNearDrawMouseByWndZENKANA_IMEONChar().c_str()), _TRUNCATE);

		lpAppRegData->dwNearDrawMouseIMEOFFColor = m_Settings.dwNearDrawMouseIMEOFFColor();
		lpAppRegData->dwNearDrawMouseHANEISU_IMEONColor = m_Settings.dwNearDrawMouseHANEISU_IMEONColor();
		lpAppRegData->dwNearDrawMouseHANKANA_IMEONColor = m_Settings.dwNearDrawMouseHANKANA_IMEONColor();
		lpAppRegData->dwNearDrawMouseZENEISU_IMEONColor = m_Settings.dwNearDrawMouseZENEISU_IMEONColor();
		lpAppRegData->dwNearDrawMouseZENHIRA_IMEONColor = m_Settings.dwNearDrawMouseZENHIRA_IMEONColor();
		lpAppRegData->dwNearDrawMouseZENKANA_IMEONColor = m_Settings.dwNearDrawMouseZENKANA_IMEONColor();
		lpAppRegData->dwNearDrawCaretIMEOFFColor = m_Settings.dwNearDrawCaretIMEOFFColor();
		lpAppRegData->dwNearDrawCaretHANEISU_IMEONColor = m_Settings.dwNearDrawCaretHANEISU_IMEONColor();
		lpAppRegData->dwNearDrawCaretHANKANA_IMEONColor = m_Settings.dwNearDrawCaretHANKANA_IMEONColor();
		lpAppRegData->dwNearDrawCaretZENEISU_IMEONColor = m_Settings.dwNearDrawCaretZENEISU_IMEONColor();
		lpAppRegData->dwNearDrawCaretZENHIRA_IMEONColor = m_Settings.dwNearDrawCaretZENHIRA_IMEONColor();
		lpAppRegData->dwNearDrawCaretZENKANA_IMEONColor = m_Settings.dwNearDrawCaretZENKANA_IMEONColor();
		lpAppRegData->dwNearDrawMouseByWndIMEOFFColor = m_Settings.dwNearDrawMouseByWndIMEOFFColor();
		lpAppRegData->dwNearDrawMouseByWndHANEISU_IMEONColor = m_Settings.dwNearDrawMouseByWndHANEISU_IMEONColor();
		lpAppRegData->dwNearDrawMouseByWndHANKANA_IMEONColor = m_Settings.dwNearDrawMouseByWndHANKANA_IMEONColor();
		lpAppRegData->dwNearDrawMouseByWndZENEISU_IMEONColor = m_Settings.dwNearDrawMouseByWndZENEISU_IMEONColor();
		lpAppRegData->dwNearDrawMouseByWndZENHIRA_IMEONColor = m_Settings.dwNearDrawMouseByWndZENHIRA_IMEONColor();
		lpAppRegData->dwNearDrawMouseByWndZENKANA_IMEONColor = m_Settings.dwNearDrawMouseByWndZENKANA_IMEONColor();

		_tcsncpy_s(lpAppRegData->szNearDrawMouseFont, LF_FACESIZE, (LPCTSTR)(m_Settings.szNearDrawMouseFont().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawCaretFont, LF_FACESIZE, (LPCTSTR)(m_Settings.szNearDrawCaretFont().c_str()), _TRUNCATE);
		_tcsncpy_s(lpAppRegData->szNearDrawMouseByWndFont, LF_FACESIZE, (LPCTSTR)(m_Settings.szNearDrawMouseByWndFont().c_str()), _TRUNCATE);

		lpAppRegData->dwSynTPHelper1 = m_Settings.dwSynTPHelper1();
		lpAppRegData->dwSynTPPadX = m_Settings.dwSynTPPadX();
		lpAppRegData->dwSynTPPadY = m_Settings.dwSynTPPadY();
		lpAppRegData->dwSynTPEdgeX = m_Settings.dwSynTPEdgeX();
		lpAppRegData->dwSynTPEdgeY = m_Settings.dwSynTPEdgeY();
		ZeroMemory(lpAppRegData->szSynTPSendIPAddr1, (MAX_IPV4_ADDRESS * sizeof(TCHAR)));
		if (_sntprintf_s(lpAppRegData->szSynTPSendIPAddr1, MAX_IPV4_ADDRESS, _TRUNCATE, L"%s.%s.%s.%s",
				(LPCTSTR)(m_Settings.szSynTPSendIPAddr1_1().c_str()), (LPCTSTR)(m_Settings.szSynTPSendIPAddr1_2().c_str()), (LPCTSTR)(m_Settings.szSynTPSendIPAddr1_3().c_str()), (LPCTSTR)(m_Settings.szSynTPSendIPAddr1_4().c_str())) <= 0) {
		}
		ZeroMemory(lpAppRegData->szSynTPSendHostname1, (MAX_FQDN * sizeof(TCHAR)));
		if (_tcsncpy_s(lpAppRegData->szSynTPSendHostname1, MAX_FQDN, (LPCTSTR)(m_Settings.szSynTPSendHostname1().c_str()), _TRUNCATE) != 0) {
		}
		lpAppRegData->dwSynTPPortNo1 = m_Settings.dwSynTPPortNo1();
		lpAppRegData->bSynTPStarted1 = m_Settings.bSynTPStarted1();
	}
	Profile->bSetProfileData();
}

//
// SettingsClose()
//
void SettingsClose()
{
	if (m_Settings != nullptr) {
		HWND hSettingsWnd = (HWND)m_Settings.g_hSettingsWnd();
		if (hSettingsWnd != nullptr && m_SettingsWndProc != 0) {
			SetWindowLongPtr(hSettingsWnd, GWLP_WNDPROC, m_SettingsWndProc);
			m_SettingsWndProc = 0;
		}
		try {
			m_Settings.Close();
		}
		catch (...) {
		}
		m_Settings = nullptr;
	}
}

//
// SetWindowProcSafe()
//
static BOOL SetWindowProcSafe(HWND hwnd, WNDPROC newProc, LONG_PTR *pPrevProc)
{
	if (hwnd == NULL || newProc == NULL || pPrevProc == NULL) return FALSE;
	SetLastError(0);
	LONG_PTR prev = SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(newProc));
	DWORD err = GetLastError();
	if (prev == 0 && err != 0) {
		return FALSE;
	}
	*pPrevProc = prev;
	return TRUE;
}


/* = EOF = */
