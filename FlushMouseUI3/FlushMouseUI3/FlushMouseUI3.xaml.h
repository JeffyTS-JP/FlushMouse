// FlushMouseUI3.Xaml.h for FlushMouseUI3
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
#include "FlushMouseUI3.xaml.g.h"
#pragma pop_macro("GetCurrentTime")

using namespace winrt;

using namespace FlushMouseUI3;
using namespace FlushMouseUI3::implementation;

extern void CalcWindowCentralizeRect(HWND hWnd, Windows::Graphics::RectInt32* Rect32);
extern void GetMonitorDPIandRect(HWND hWnd, LPUINT lpUint,LPRECT lpRect);

namespace winrt::FlushMouseUI3::implementation
{

	struct FlusMouseUI3 : AppT<FlusMouseUI3>
	{
		FlusMouseUI3();
		~FlusMouseUI3();
		
		void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

	private:
		void OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] Windows::ApplicationModel::SuspendingEventArgs const& e);
		HINSTANCE	GetInstanceFromHWND(HWND hWnd);

	private:
		Microsoft::UI::Xaml::Window windowMain{ nullptr };
		HMODULE		hFlushMouseUI3DLL{ nullptr };
	};
}


/* = EOF = */