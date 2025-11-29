//
// MojoWindow.Xaml.h
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
//
// No.	  Date			Name			Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/07/16  JeffyTS  	New edit.
//
#pragma once
#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime
#include "MojoWindow.g.h"
#pragma pop_macro("GetCurrentTime")

using namespace winrt;
using namespace winrt::Microsoft::UI;
using namespace winrt::Microsoft::UI::Xaml;

void		SettingsExec(HWND hWnd, UINT32 uMsg, INT32 iSelectedPane);
void		SettingsApply();
void		SettingsClose();

namespace winrt::FlushMouseSettings::implementation
{
	void		MojoWindowExec();
	void		MojoWindowClose();

	struct MojoWindow : MojoWindowT<MojoWindow>
	{
		MojoWindow();
		~MojoWindow();

	private:

	};
}

namespace winrt::FlushMouseSettings::factory_implementation
{
	struct MojoWindow : MojoWindowT<MojoWindow, implementation::MojoWindow>
	{
	};
}
