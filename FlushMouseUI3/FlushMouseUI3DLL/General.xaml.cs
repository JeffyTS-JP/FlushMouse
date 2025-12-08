//
// General.Xaml.cs for FlushMouseUI3DLL
//
//      Copyright (C) 1993- JeffyTS. All rights reserved.
//      Licensed under the GPL-2.0 License.
//
// No.    Date          Name            Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000    2024/07/15  JeffyTS     New edit.
//

using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

using System;
using System.Diagnostics;

using static FlushMouseUI3DLL.Settings;

namespace FlushMouseUI3DLL {
	public sealed partial class Settings
	{
		public static bool bDisplayFocusWindowIME { get; set; }
		public static bool bDisplayIMEModeOnCursor { get; set; }
		public static bool bDisplayIMEModeIMEOFF { get; set; }
		public static bool bOffChangedFocus { get; set; }
		public static bool bForceHiragana { get; set; }
		public static bool bDoModeDispByIMEKeyDown { get; set; }
		public static bool bDoModeDispByMouseBttnUp { get; set; }
		public static bool bDoModeDispByCtrlUp { get; set; }
		public static bool bDrawNearCaret { get; set; }
		public static bool bSupportVirtualDesktop { get; set; }
		public static bool bIMEModeForced { get; set; }
		public static bool bEnableEPHelper { get; set; }

		public static Int32 iCursorSize { get; set; }
		public static Int32 dwDisplayModeTime { get; set; }
		public static Int32 dwAdditionalWaitTime { get; set; }
		public static Int32 dwWaitWaveTime { get; set; }
		public static Int32 dwDisplayIMEModeMethod { get; set; }
		internal const Int32 DisplayIMEModeMethod_RESOURCE = 0;
		internal const Int32 DisplayIMEModeMethod_ByWindow = 1;
		internal const Int32 DisplayIMEModeMethod_RES_AND_Window = 2;
	}

	public sealed partial class General
	{
		private static bool m_Sentinel {  get; set; }
	}

	public sealed partial class General : Page {
		public General()
		{
			m_Sentinel = false;
			InitializeComponent();
		}

		private void EnableDisableItems(object sender, RoutedEventArgs e) {
			if (e == null) { }
			if (sender != null) {
				if (!bDisplayIMEModeOnCursor) {
					Combo1?.IsEnabled = false;	Combo2?.IsEnabled = false;
					sl1?.IsEnabled = false;
				}
				else {
					Combo1?.IsEnabled = true;	Combo2?.IsEnabled = true;
					if (dwDisplayIMEModeMethod == DisplayIMEModeMethod_ByWindow) {
						sl1?.IsEnabled = false;
					}
					else {
						sl1?.IsEnabled = true;
					}
				}
				if (!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp) {
					ts9?.IsEnabled = false;
					sl2?.IsEnabled = false;	sl3?.IsEnabled = false;
				}
				else {
					ts9?.IsEnabled = true;
					sl2?.IsEnabled = true;	sl3?.IsEnabled = true;
				}
				if (!bDoModeDispByCtrlUp) {
					sl4?.IsEnabled = false;
				}
				else {
					sl4?.IsEnabled = true;
				}
				if (ts1.IsOn)   text1.Text = "オン";        else text1.Text = "オフ";
				if (ts3.IsOn)   text3.Text = "オン";        else text3.Text = "オフ";
				if (ts4.IsOn)   text4.Text = "オン";        else text4.Text = "オフ";
				if (ts5.IsOn)   text5.Text = "オン";        else text5.Text = "オフ";
				if (ts6.IsOn)   text6.Text = "オン";        else text6.Text = "オフ";
				if (ts7.IsOn)   text7.Text = "オン";        else text7.Text = "オフ";
				if (ts8.IsOn)   text8.Text = "オン";        else text8.Text = "オフ";
				if (ts9.IsOn)   text9.Text = "オン";        else text9.Text = "オフ";
				if (ts10.IsOn)  text10.Text = "オン";   else text10.Text = "オフ";
				if (ts11.IsOn)  text11.Text = "オン";   else text11.Text = "オフ";
				if (ts12.IsOn)  text12.Text = "オン";   else text12.Text = "オフ";
			}
		}

		private void General_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }

			SetComboBox();
			SetSlider();
			SetToggleSwitch();
			m_Sentinel = true;
			EnableDisableItems(sender, e);
		}

		private void SetComboBox()
		{
			if (Combo1 == null) Combo1 = new ComboBox();
			if (Combo2 == null) Combo2 = new ComboBox();
			if (Combo1 != null) {
				if (bDisplayFocusWindowIME) Combo1.SelectedItem = Item1_1;
				else Combo1.SelectedItem = Item1_2;
			}
			if (Combo2 != null) {
				if (dwDisplayIMEModeMethod == DisplayIMEModeMethod_RESOURCE) Combo2.SelectedItem = Item2_1;
				else if (dwDisplayIMEModeMethod == DisplayIMEModeMethod_ByWindow) Combo2.SelectedItem = Item2_2;
				else if (dwDisplayIMEModeMethod == DisplayIMEModeMethod_RES_AND_Window) Combo2.SelectedItem = Item2_3;
			}
		}

		private void Combo_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
		}

		private void Combo_SelectionChanged(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			ComboBox combo = sender as ComboBox;
			ComboBoxItem selectedItem = (ComboBoxItem)combo.SelectedItem;
			if (selectedItem != null) {
				if ((selectedItem.Name == "Item1_1") || (selectedItem.Name == "Item1_2")) {
					if (selectedItem.Name == "Item1_1") bDisplayFocusWindowIME = true;
					else if (selectedItem.Name == "Item1_2") bDisplayFocusWindowIME = false;
					EnableDisableItems(sender, e);
					UpdateProfile(SETTINGSEX_SETTINGS_GENERAL_SETREGISTRY);
					return;
				}

				if (selectedItem.Name == "Item2_1") dwDisplayIMEModeMethod = DisplayIMEModeMethod_RESOURCE;
				else if (selectedItem.Name == "Item2_2") dwDisplayIMEModeMethod = DisplayIMEModeMethod_ByWindow;
				else if (selectedItem.Name == "Item2_3") dwDisplayIMEModeMethod = DisplayIMEModeMethod_RES_AND_Window;
				EnableDisableItems(sender, e);
				UpdateProfile(SETTINGSEX_SETTINGS_GENERAL_SETREGISTRY);
				UpdateProfile(SETTINGSEX_RELOAD_MOUSECURSOR);
			}
		}

		private void SetToggleSwitch()
		{
			if (ts1 == null)    ts1 = new ToggleSwitch();
			if (ts3 == null)    ts3 = new ToggleSwitch();
			if (ts4 == null)    ts4 = new ToggleSwitch();
			if (ts5 == null)    ts5 = new ToggleSwitch();
			if (ts6 == null)    ts6 = new ToggleSwitch();
			if (ts7 == null)    ts7 = new ToggleSwitch();
			if (ts8 == null)    ts8 = new ToggleSwitch();
			if (ts9 == null)    ts9 = new ToggleSwitch();
			if (ts10 == null)   ts10 = new ToggleSwitch();
			if (ts11 == null)   ts11 = new ToggleSwitch();
			if (ts12 == null)   ts12 = new ToggleSwitch();
			ts1.IsOn = bDisplayIMEModeOnCursor;
			ts3.IsOn = bDisplayIMEModeIMEOFF;
			ts4.IsOn = bOffChangedFocus;
			ts5.IsOn = bForceHiragana;
			ts6.IsOn = bDoModeDispByIMEKeyDown;
			ts7.IsOn = bDoModeDispByMouseBttnUp;
			ts8.IsOn = bDoModeDispByCtrlUp;
			ts9.IsOn = bDrawNearCaret;
			ts10.IsOn = bSupportVirtualDesktop;
			ts11.IsOn = bIMEModeForced;
			ts12.IsOn = bEnableEPHelper;
		}

		private void HandleToggleSwitch(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			ToggleSwitch ts = sender as ToggleSwitch;
			if (ts != null) {
				if (ts.Name == "ts1") {
					bDisplayIMEModeOnCursor = !bDisplayIMEModeOnCursor;
					EnableDisableItems(sender, e);
					UpdateProfile(SETTINGSEX_RELOAD_MOUSECURSOR);
					return;
				}
				else if (ts.Name == "ts3")
				{
					bDisplayIMEModeIMEOFF = !bDisplayIMEModeIMEOFF;
					EnableDisableItems(sender, e);
					UpdateProfile(SETTINGSEX_RELOAD_MOUSECURSOR);
					return;
				}
				else if (ts.Name == "ts4") bOffChangedFocus = !bOffChangedFocus;
				else if (ts.Name == "ts5") bForceHiragana = !bForceHiragana;
				else if (ts.Name == "ts6") bDoModeDispByIMEKeyDown = !bDoModeDispByIMEKeyDown;
				else if (ts.Name == "ts7") bDoModeDispByMouseBttnUp = !bDoModeDispByMouseBttnUp;
				else if (ts.Name == "ts8") bDoModeDispByCtrlUp = !bDoModeDispByCtrlUp;
				else if (ts.Name == "ts9") bDrawNearCaret = !bDrawNearCaret;
				else if (ts.Name == "ts10") bSupportVirtualDesktop = !bSupportVirtualDesktop;
				else if (ts.Name == "ts11") bIMEModeForced = !bIMEModeForced;
				else if (ts.Name == "ts12") bEnableEPHelper = !bEnableEPHelper;
				EnableDisableItems(sender, e);
				UpdateProfile(SETTINGSEX_SETTINGS_GENERAL_SETREGISTRY);
			}
		}

		private void SetSlider()
		{
			if (sl1 == null)    sl1 = new Slider();
			if (sl2 == null)    sl2 = new Slider();
			if (sl3 == null)    sl3 = new Slider();
			if (sl4 == null)    sl4 = new Slider();
			sl1.Value = iCursorSize;
			sl2.Value = dwDisplayModeTime / 10;
			sl3.Value = dwAdditionalWaitTime / 10;
			sl4.Value = dwWaitWaveTime / 10;
		}

		private void Slider_ValueChanged(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			Slider sl = sender as Slider;
			if (sl != null) {
				if (sl.Name == "sl1")       iCursorSize = (Int32)sl.Value;
				else if (sl.Name == "sl2")  dwDisplayModeTime = (Int32)(sl.Value * 10);
				else if (sl.Name == "sl3")  dwAdditionalWaitTime = (Int32)(sl.Value * 10);
				else if (sl.Name == "sl4")  dwWaitWaveTime = (Int32)(sl.Value * 10);
				EnableDisableItems(sender, e);
				UpdateProfile(SETTINGSEX_SETTINGS_GENERAL_SETREGISTRY);
			}
		}
	}
}


/* = EOF = */
