// About.Xaml.h for FlushMouseUI3
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
#include "About.g.h"
#pragma pop_macro("GetCurrentTime")

using namespace winrt;
using namespace winrt::Microsoft::UI;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::FlushMouseUI3::implementation
{
	void		AboutExec();
	void		AboutClose();

	struct About : AboutT<About>
	{

		About();
		~About();

		void Button_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void About_Loaded(IInspectable const& sender, RoutedEventArgs const& e);
		void Version_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

	private:
		Window::Activated_revoker			m_ActivatedRevoker;
		Window::Closed_revoker				m_ClosedRevoker;
		Window::SizeChanged_revoker			m_SizeChangedRevoker;
		Window::VisibilityChanged_revoker	m_VisibilityChangedRevoker;
	};
}

namespace winrt::FlushMouseUI3::factory_implementation
{
	struct About : AboutT<About, implementation::About>
	{
	};
}

/* = EOF = */