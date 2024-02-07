// About.Xaml.cpp for FlushMouseUI3
//		Copyright (C) 2022 JeffyTS
//
//	  
// No.	  Date			Name			Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/07/16  JeffyTS  	New edit.
//
#include "pch.h"
#include "WinRT.h"

#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime
#include "About.xaml.h"
#if __has_include("About.g.cpp")
#include "About.g.cpp"
#endif
#pragma pop_macro("GetCurrentTime")

#include "FlushMouseUI3.xaml.h"
#include "..\..\FlushMouseLIB\FlushMouseLIB.h"
#include "..\..\FlushMouseLIB\Resource.h"
#include "..\..\version.h"

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

Window	windowAbout{ nullptr };
HWND	hAboutWnd{ nullptr };

namespace winrt::FlushMouseUI3::implementation
{
	Window	windowAbout{ nullptr };
	HWND	hAboutWnd{ nullptr };

	void	AboutExec()
	{
		if (windowAbout == nullptr) {
			windowAbout = make<About>();
			windowAbout.Activate();
		}
		else {
			SetForegroundWindow(hAboutWnd);
		}
	}

	void	AboutClose()
	{
		if (windowAbout != nullptr) {
			windowAbout.Close();
		}
	}

	About::About()
	{
		if (windowAbout == nullptr) {
			InitializeComponent();
			
			auto windowNative{ this->m_inner.as<::IWindowNative>() };
#pragma warning(push)
#pragma warning(disable : 28112)
			InterlockedIncrement64((volatile LONG64*)&s_module);
			windowNative->get_WindowHandle(&hAboutWnd);
			InterlockedDecrement64((volatile LONG64*)&s_module);
#pragma warning(pop)
			Microsoft::UI::WindowId windowId = Microsoft::UI::GetWindowIdFromWindow(hAboutWnd);
			Microsoft::UI::Windowing::AppWindow appWindow = Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);
			if (appWindow) {
				// You now have an AppWindow object, and you can call its methods to manipulate the window.
				// As an example, let's change the title text of the window.
				appWindow.Title(L"FlushMouseUI3 バージョン情報");
				// Move and resize
				Windows::Graphics::RectInt32	Rect32{};
				Rect32.X = 0;	Rect32.Y = 0;
				Rect32.Width = 400;	Rect32.Height = 300;
				CalcWindowCentralizeRect(hAboutWnd, &Rect32);
				appWindow.MoveAndResize(Rect32);

			}
			
			// Set Activated event handler
			m_ActivatedRevoker = this->Activated(winrt::auto_revoke, [&](auto&&, auto&& args) {
				Microsoft::UI::Xaml::WindowActivatedEventArgs _args = (Microsoft::UI::Xaml::WindowActivatedEventArgs)args;
				if (_args.WindowActivationState() == Microsoft::UI::Xaml::WindowActivationState::Deactivated) {
				}
				else {
				}
				});
			// Set Closed event handler
			m_ClosedRevoker = this->Closed(winrt::auto_revoke, [&](auto&&, auto&&) {
				if (windowAbout != nullptr) {
					this->ExtendsContentIntoTitleBar(false);
					windowAbout = nullptr;
				}
				});
			// Set Size Changed event handler
			m_SizeChangedRevoker = this->SizeChanged(winrt::auto_revoke, [&](auto&&, auto&&) {
				});
			// Set Visibility Changed event handler
			m_VisibilityChangedRevoker = this->VisibilityChanged(winrt::auto_revoke, [&](auto&&, auto&&) {
				});
			
			HMENU hMenu = GetSystemMenu(hAboutWnd, false);
			if (hMenu != nullptr) {
				RemoveMenu(hMenu, SC_RESTORE, MF_BYCOMMAND | MF_DISABLED);
				RemoveMenu(hMenu, SC_SIZE, MF_BYCOMMAND | MF_DISABLED);
				RemoveMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND | MF_DISABLED);
				RemoveMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND | MF_DISABLED);
			}
			if (SetWindowLong(hAboutWnd, GWL_STYLE, (GetWindowLongPtr(hAboutWnd, GWL_STYLE) & (~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX) /*(~WS_SYSMENU)*/))) == 0) {
#if _DEBUG
#pragma warning(push)
#pragma warning(disable : 4189)
				{_Post_equals_last_error_ DWORD err = GetLastError(); }
#pragma warning(pop)
#endif // _DEBUG
			}
			if (Resource != nullptr) {
				HICON hIcon = LoadIcon(Resource->hLoad(), MAKEINTRESOURCE(IDI_FLUSHMOUSE));
				SendMessage(hAboutWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			}
		}
	}

	About::~About()
	{
		//
	}

	void About::Button_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{
		UNREFERENCED_PARAMETER(sender);
		UNREFERENCED_PARAMETER(e);

		AboutClose();
	}
	
	void About::About_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
	{
		UNREFERENCED_PARAMETER(sender);
		UNREFERENCED_PARAMETER(e);
	}

	void About::Version_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
	{
		UNREFERENCED_PARAMETER(sender);
		UNREFERENCED_PARAMETER(e);

		LPTSTR	lpszVersion = new TCHAR[MAX_LOADSTRING];
		if (lpszVersion) {
			ZeroMemory(lpszVersion, (sizeof(TCHAR) * MAX_LOADSTRING));
			_sntprintf_s(lpszVersion, MAX_LOADSTRING, _TRUNCATE, _T("Version : %d.%d.%d.%d"), MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION, REVISION_VERSION);
			Version().Text(lpszVersion);
			delete[]	lpszVersion;
		}
	}
}


/* = EOF = */

