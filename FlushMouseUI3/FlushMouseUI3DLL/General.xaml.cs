// General.Xaml.cs for FlushMouseUI3DLL
//      Copyright (C) 2024 JeffyTS
//
//    
// No.    Date          Name            Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000    2024/07/15  JeffyTS     New edit.
//

using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls.Primitives;
using Microsoft.UI.Xaml.Media;

using System;
using System.Drawing;
using System.Diagnostics;

using static FlushMouseUI3DLL.Settings;

namespace FlushMouseUI3DLL {
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
					if (Combo1 != null) Combo1.IsEnabled = false;
					if (ts2 != null)    ts2.IsEnabled = false;
					if (!bDisplayIMEModeByWindow) {
						if (grid4_1 != null)    grid4_1.Visibility = Visibility.Visible;
						if (grid4_2 != null)    grid4_2.Visibility = Visibility.Collapsed;
						if (sl1_1 != null)      sl1_1.IsEnabled = true;
					}
					else {
						if (grid4_1 != null)    grid4_1.Visibility = Visibility.Collapsed;
						if (grid4_2 != null)    grid4_2.Visibility = Visibility.Visible;
						if (sl1_2 != null)      sl1_2.IsEnabled = false;
					}
				}
				else {
					if (Combo1 != null) Combo1.IsEnabled = true;
					if (ts2 != null)    ts2.IsEnabled = true;
					if (!bDisplayIMEModeByWindow) {
						if (grid4_1 != null)    grid4_1.Visibility = Visibility.Visible;
						if (grid4_2 != null)    grid4_2.Visibility = Visibility.Collapsed;
						if (sl1_1 != null)      sl1_1.IsEnabled = true;
					}
					else {
						if (grid4_1 != null)    grid4_1.Visibility = Visibility.Collapsed;
						if (grid4_2 != null)    grid4_2.Visibility = Visibility.Visible;
						if (sl1_2 != null)      sl1_2.IsEnabled = true;
					}
				}
				if (!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp) {
					if (ts9 != null)        ts9.IsEnabled = false;
					if (sl2 != null)        sl2.IsEnabled = false;
					if (sl3 != null)        sl3.IsEnabled = false;
					if (sl4 != null)        sl4.IsEnabled = false;
				}
				else {
					if (ts9 != null)	ts9.IsEnabled = true;
					if (sl2 != null)	sl2.IsEnabled = true;
					if (sl3 != null)	sl3.IsEnabled = true;
					if (sl4 != null)	sl4.IsEnabled = true;
				}
				if (!bDoModeDispByCtrlUp) {
					if (sl5 != null)	sl5.IsEnabled = false;
				}
				else {
					if (sl5 != null)	sl5.IsEnabled = true;
				}
				if (ts1.IsOn)	text1.Text = "オン";		else text1.Text = "オフ";
				if (ts2.IsOn)	text2.Text = "オン";		else text2.Text = "オフ";
				if (ts3.IsOn)	text3.Text = "オン";		else text3.Text = "オフ";
				if (ts4.IsOn)	text4.Text = "オン";		else text4.Text = "オフ";
				if (ts5.IsOn)	text5.Text = "オン";		else text5.Text = "オフ";
				if (ts6.IsOn)	text6.Text = "オン";		else text6.Text = "オフ";
				if (ts7.IsOn)	text7.Text = "オン";		else text7.Text = "オフ";
				if (ts8.IsOn)	text8.Text = "オン";		else text8.Text = "オフ";
				if (ts9.IsOn)	text9.Text = "オン";		else text9.Text = "オフ";
				if (ts10.IsOn)	text10.Text = "オン";	else text10.Text = "オフ";
				if (ts11.IsOn)	text11.Text = "オン";	else text11.Text = "オフ";
			}
		}
		
		private void General_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
			
			SetGrid();
			SetComboBox();
			SetSlider();
			SetToggleSwitch();
			m_Sentinel = true;
			EnableDisableItems(sender, e);
		}

		private void SetGrid()
		{
			if (grid4_1 == null)    grid4_1 = new Grid();
			if (grid4_2 == null)    grid4_2 = new Grid();
		}

		private void SetComboBox()
		{
			if (Combo1 == null) Combo1 = new ComboBox();
			if (Combo1 != null) {
				if (bDisplayFocusWindowIME) Combo1.SelectedItem = Item1;
				else Combo1.SelectedItem = Item2;
			}
		}

		private void Combo1_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
		}

		private void Combo1_SelectionChanged(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			ComboBox combo = sender as ComboBox;
			ComboBoxItem selectedItem = (ComboBoxItem)combo.SelectedItem;
			if (selectedItem != null) {
				if (selectedItem.Name == "Item1") bDisplayFocusWindowIME = true;
				else if (selectedItem.Name == "Item2") bDisplayFocusWindowIME = false;
				EnableDisableItems(sender, e);
				UpdateProfile(SETTINGSEX_APPLY);
			}
		}

		private void SetToggleSwitch()
		{
			if (ts1 == null)	ts1 = new ToggleSwitch();
			if (ts2 == null)	ts2 = new ToggleSwitch();
			if (ts3 == null)	ts3 = new ToggleSwitch();
			if (ts4 == null)	ts4 = new ToggleSwitch();
			if (ts5 == null)	ts5 = new ToggleSwitch();
			if (ts6 == null)	ts6 = new ToggleSwitch();
			if (ts7 == null)	ts7 = new ToggleSwitch();
			if (ts8 == null)	ts8 = new ToggleSwitch();
			if (ts9 == null)	ts9 = new ToggleSwitch();
			if (ts10 == null)	ts10 = new ToggleSwitch();
			if (ts11 == null)	ts11 = new ToggleSwitch();
			ts1.IsOn = bDisplayIMEModeOnCursor;
			ts2.IsOn = bDisplayIMEModeByWindow;
			ts3.IsOn = bDisplayIMEModeIMEOFF;
			ts4.IsOn = bOffChangedFocus;
			ts5.IsOn = bForceHiragana;
			ts6.IsOn = bDoModeDispByIMEKeyDown;
			ts7.IsOn = bDoModeDispByMouseBttnUp;
			ts8.IsOn = bDoModeDispByCtrlUp;
			ts9.IsOn = bDrawNearCaret;
			ts10.IsOn = bIMEModeForced;
			ts11.IsOn = bEnableEPHelper;
		}

		private void HandleToggleSwitch(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			ToggleSwitch ts = sender as ToggleSwitch;
			if (ts != null) {
				if (ts.Name == "ts1") {
					bDisplayIMEModeOnCursor = !bDisplayIMEModeOnCursor;
					EnableDisableItems(sender, e);
					UpdateProfile(SETTINGSEX_RELOAD_MOUSE);
					return;
				}
				else if (ts.Name == "ts2") {
					bDisplayIMEModeByWindow = !bDisplayIMEModeByWindow;
					EnableDisableItems(sender, e);
					UpdateProfile(SETTINGSEX_RELOAD_MOUSE);
					return;
				}
				else if (ts.Name == "ts3") bDisplayIMEModeIMEOFF = !bDisplayIMEModeIMEOFF;
				else if (ts.Name == "ts4") bOffChangedFocus = !bOffChangedFocus;
				else if (ts.Name == "ts5") bForceHiragana = !bForceHiragana;
				else if (ts.Name == "ts6") bDoModeDispByIMEKeyDown = !bDoModeDispByIMEKeyDown;
				else if (ts.Name == "ts7") bDoModeDispByMouseBttnUp = !bDoModeDispByMouseBttnUp;
				else if (ts.Name == "ts8") bDoModeDispByCtrlUp = !bDoModeDispByCtrlUp;
				else if (ts.Name == "ts9") bDrawNearCaret = !bDrawNearCaret;
				else if (ts.Name == "ts10") bIMEModeForced = !bIMEModeForced;
				else if (ts.Name == "ts11") bEnableEPHelper = !bEnableEPHelper;
				EnableDisableItems(sender, e);
				UpdateProfile(SETTINGSEX_APPLY);
			}
		}

		private void SetSlider()
		{
			if (sl1_1 == null)	sl1_1 = new Slider();
			if (sl1_2 == null)	sl1_2 = new Slider();
			if (sl2 == null)	sl2 = new Slider();
			if (sl3 == null)	sl3 = new Slider();
			if (sl4 == null)	sl4 = new Slider();
			if (sl5 == null)	sl5 = new Slider();
			sl1_1.Value = iCursorSize;
			sl1_2.Value = iIMEModeDistance;
			sl2.Value = iModeSize;
			sl3.Value = dwDisplayModeTime / 10;
			sl4.Value = dwAdditionalWaitTime / 10;
			sl5.Value = dwWaitWaveTime / 10;
		}

		private void Slider_ValueChanged(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			Slider sl = sender as Slider;
			if (sl != null) {
				if (sl.Name == "sl1_1")         iCursorSize = (Int32)sl.Value;
				else if (sl.Name == "sl1_2")    iIMEModeDistance = (Int32)sl.Value;
				else if (sl.Name == "sl2")      iModeSize = (Int32)sl.Value;
				else if (sl.Name == "sl3")      dwDisplayModeTime = (Int32)(sl.Value * 10);
				else if (sl.Name == "sl4")      dwAdditionalWaitTime = (Int32)(sl.Value * 10);
				else if (sl.Name == "sl5")      dwWaitWaveTime = (Int32)(sl.Value * 10);
				EnableDisableItems(sender, e);
				UpdateProfile(SETTINGSEX_APPLY);
			}
		}
	}
}


/* = EOF = */