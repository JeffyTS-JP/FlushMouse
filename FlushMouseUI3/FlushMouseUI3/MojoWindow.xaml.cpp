// MojoWindow.Xaml.cpp for FlushMouseUI3
//		Copyright (C) 2022 JeffyTS
//
//	  
// No.	  Date			Name			Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/07/16  JeffyTS  	New edit.
//

#include "pch.h"
#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime
#include "MojoWindow.xaml.h"
#if __has_include("MojoWindow.g.cpp")
#include "MojoWindow.g.cpp"
#endif
#pragma pop_macro("GetCurrentTime")

#include "FlushMouseUI3.xaml.h"
#include "About.xaml.h"
#include "..\..\FlushMouseLIB\FlushMouseLIB.h"

static Window	wMojoWnd{ nullptr };
static HWND		hMojoWnd{ nullptr };

static FlushMouseUI3DLL::Settings	settings{ nullptr };

namespace winrt::FlushMouseUI3::implementation
{
	void		MojoWindowExec()
	{
		if (wMojoWnd == nullptr) {
			wMojoWnd = make<MojoWindow>();
			if (wMojoWnd != nullptr) {
				auto windowNative{ wMojoWnd.try_as<::IWindowNative>() };
				winrt::check_bool(windowNative);
#pragma warning(push)
#pragma warning(disable : 28112)
				InterlockedIncrement64((volatile LONG64*)&s_module);
				windowNative->get_WindowHandle(&hMojoWnd);
				InterlockedDecrement64((volatile LONG64*)&s_module);
#pragma warning(pop)
				Microsoft::UI::WindowId windowId = Microsoft::UI::GetWindowIdFromWindow(hMojoWnd);
				Microsoft::UI::Windowing::AppWindow appWindow = Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);
				if (appWindow) {
					// Move and resize
					Windows::Graphics::RectInt32	Rect32{};
					Rect32.X = 0;	Rect32.Y = 0;
					Rect32.Width = 0;	Rect32.Height = 0;
					CalcWindowCentralizeRect(hMojoWnd, &Rect32);
					appWindow.MoveAndResize(Rect32);
				}
			}
		}
		else {
		}
	}

	void		MojoWindowClose()
	{
		SettingsClose();
		AboutClose();
		if (wMojoWnd != nullptr) {
			// Do NOT Close
			// wMojoWnd.Close();
			wMojoWnd = nullptr;
		}
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

void		SettingsExec(HWND hWnd, int msg)
{
	if (settings == nullptr) {
		settings = FlushMouseUI3DLL::Settings();
		if (settings != nullptr) {
			settings.settings(settings);
			settings.hMainWnd((int64_t)hWnd);
			settings.msg((int)msg);

			HWND	_hWnd = (HWND)settings.hSettingsWnd();
			if (_hWnd != NULL) {
				Windows::Graphics::RectInt32	Rect32{};
				Rect32.X = 0;	Rect32.Y = 0;
				Rect32.Width = settings.Window_Width();
				Rect32.Height = settings.Window_Height();
				CalcWindowCentralizeRect(_hWnd, &Rect32);
				SetWindowPos(_hWnd, HWND_TOP, Rect32.X, Rect32.Y, Rect32.Width, Rect32.Height, SW_SHOWNOACTIVATE);
				settings.Window_Left(Rect32.X);		settings.Window_Top(Rect32.Y);
				settings.Window_Width(Rect32.Width);	settings.Window_Height(Rect32.Height);
				
				RECT	rc{};
				UINT	dpi=0;
				GetMonitorDPIandRect(_hWnd, &dpi, &rc);
				settings.Monitor_DPI(dpi);
				settings.Monitor_Left(rc.left);
				settings.Monitor_Top(rc.top);
				settings.Monitor_Right(rc.right);
				settings.Monitor_Bottom(rc.bottom);

				settings.Activate();
			}

			if (Profile != nullptr) {
				// ComboBox
				settings.bDisplayFocusWindowIME(Profile->lpstAppRegData->bDisplayFocusWindowIME);

				// Check Box
				settings.bDisplayIMEModeOnCursor(Profile->lpstAppRegData->bDisplayIMEModeOnCursor);
				settings.bOffChangedFocus(Profile->lpstAppRegData->bOffChangedFocus);
				settings.bForceHiragana(Profile->lpstAppRegData->bForceHiragana);
				settings.bDoModeDispByIMEKeyDown(Profile->lpstAppRegData->bDoModeDispByIMEKeyDown);
				settings.bDoModeDispByMouseBttnUp(Profile->lpstAppRegData->bDoModeDispByMouseBttnUp);
				settings.bDoModeDispByCtrlUp(Profile->lpstAppRegData->bDoModeDispByCtrlUp);
				settings.bDrawNearCaret(Profile->lpstAppRegData->bDrawNearCaret);
				settings.bIMEModeForced(Profile->lpstAppRegData->bIMEModeForced);
				settings.bEnableEPHelper(Profile->lpstAppRegData->bEnableEPHelper);

				// Slider
				settings.iCursorSize(Profile->lpstAppRegData->iCursorSize);
				settings.iModeSize(Profile->lpstAppRegData->iModeSize);
				settings.dwDisplayModeTime(Profile->lpstAppRegData->dwDisplayModeTime);
				settings.dwAdditionalWaitTime(Profile->lpstAppRegData->dwAdditionalWaitTime);
				settings.dwWaitWaveTime(Profile->lpstAppRegData->dwWaitWaveTime);

				// Color
				settings.dwNearDrawMouseColor(Profile->lpstAppRegData->dwNearDrawMouseColor);
				settings.dwNearDrawCaretColor(Profile->lpstAppRegData->dwNearDrawCaretColor);
			}
		}
	}
	else {
		if ((HWND)settings.hSettingsWnd() != NULL) {
			SetForegroundWindow((HWND)settings.hSettingsWnd());
		}
	}
}

void		SettingsApply()
{
	if ((settings != nullptr) && (Profile != nullptr)) {
		// ComboBox
		Profile->lpstAppRegData->bDisplayFocusWindowIME = settings.bDisplayFocusWindowIME();

		// Check Box
		Profile->lpstAppRegData->bDisplayIMEModeOnCursor = settings.bDisplayIMEModeOnCursor();
		Profile->lpstAppRegData->bOffChangedFocus = settings.bOffChangedFocus();
		Profile->lpstAppRegData->bForceHiragana = settings.bForceHiragana();
		Profile->lpstAppRegData->bDoModeDispByIMEKeyDown = settings.bDoModeDispByIMEKeyDown();
		Profile->lpstAppRegData->bDoModeDispByMouseBttnUp = settings.bDoModeDispByMouseBttnUp();
		Profile->lpstAppRegData->bDoModeDispByCtrlUp = settings.bDoModeDispByCtrlUp();
		Profile->lpstAppRegData->bDrawNearCaret = settings.bDrawNearCaret();
		Profile->lpstAppRegData->bIMEModeForced = settings.bIMEModeForced();
		Profile->lpstAppRegData->bEnableEPHelper = settings.bEnableEPHelper();

		// Slider
		Profile->lpstAppRegData->iCursorSize = settings.iCursorSize();
		Profile->lpstAppRegData->iModeSize = settings.iModeSize();
		Profile->lpstAppRegData->dwDisplayModeTime = settings.dwDisplayModeTime();
		Profile->lpstAppRegData->dwAdditionalWaitTime = settings.dwAdditionalWaitTime();
		Profile->lpstAppRegData->dwWaitWaveTime = settings.dwWaitWaveTime();
	
		// Color
		Profile->lpstAppRegData->dwNearDrawMouseColor = settings.dwNearDrawMouseColor();
		Profile->lpstAppRegData->dwNearDrawCaretColor = settings.dwNearDrawCaretColor();
	}
}

void		SettingsClose()
{
	if (settings != nullptr) {
		if ((HWND)settings.hSettingsWnd() != nullptr) {
			settings.hSettingsWnd(NULL);
		}
		settings.Close();
		settings = nullptr;
	}
}

/* = EOF = */