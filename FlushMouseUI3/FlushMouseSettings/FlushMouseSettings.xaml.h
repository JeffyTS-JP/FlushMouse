// FlushMouseSettings.Xaml.h for FlushMouseSettings
//		Copyright (C) 1993 JeffyTS
//
//	  
// No.	  Date			Name			Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/07/16  JeffyTS  	New edit.
//

#pragma once
#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime
#include "FlushMouseSettings.xaml.g.h"
#pragma pop_macro("GetCurrentTime")

using namespace winrt;

using namespace FlushMouseSettings;
using namespace FlushMouseSettings::implementation;

extern void CalcWindowCentralizeRect(HWND hWnd, Windows::Graphics::RectInt32* Rect32);
extern void GetMonitorDPIandRect(HWND hWnd, LPUINT lpUint,LPRECT lpRect);

namespace winrt::FlushMouseSettings::implementation
{

	struct SettingsMain : AppT<SettingsMain>
	{
		SettingsMain();
		~SettingsMain();
		
		void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

	private:
		void OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] Windows::ApplicationModel::SuspendingEventArgs const& e);

		HINSTANCE	GetInstanceFromHWND(HWND hWnd);

	private:
		Microsoft::UI::Xaml::Window windowMain{ nullptr };
		HMODULE		hFlushMouseUI3DLL{ nullptr };
		HMODULE		hMicrosoft_ui_xaml_dll{ nullptr };

	};
}


/* = EOF = */