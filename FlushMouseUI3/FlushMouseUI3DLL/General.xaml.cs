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
					if (!bDisplayIMEModeByWindow) {
						if (grid4_1 != null)    grid4_1.Visibility = Visibility.Visible;
						if (sl1_1 != null)      sl1_1.IsEnabled = true;
					}
					else {
						if (grid4_1 != null)    grid4_1.Visibility = Visibility.Collapsed;
					}
				}
				else {
					if (Combo1 != null) Combo1.IsEnabled = true;
					if (!bDisplayIMEModeByWindow) {
						if (grid4_1 != null)    grid4_1.Visibility = Visibility.Visible;
						if (sl1_1 != null)      sl1_1.IsEnabled = true;
					}
					else {
						if (grid4_1 != null)    grid4_1.Visibility = Visibility.Collapsed;
					}
				}
				if (!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp) {
					if (ts8 != null)        ts8.IsEnabled = false;
					if (sl2 != null)        sl2.IsEnabled = false;
					if (sl3 != null)        sl3.IsEnabled = false;
					if (sl4 != null)        sl4.IsEnabled = false;
					if (Button4 != null)    Button4.IsEnabled = false;
					if (Button5 != null)    Button5.IsEnabled = false;
				}
				else {
					if (ts8 != null)	ts8.IsEnabled = true;
					if (sl2 != null)	sl2.IsEnabled = true;
					if (sl3 != null)	sl3.IsEnabled = true;
					if (sl4 != null)	sl4.IsEnabled = true;
					if (Button4 != null)	Button4.IsEnabled = true;
					if (Button5 != null)	Button5.IsEnabled = true;
				}
				if (!bDoModeDispByCtrlUp) {
					if (sl5 != null)	sl5.IsEnabled = false;
				}
				else {
					if (sl5 != null)	sl5.IsEnabled = true;
				}
			}
		}
		
		private void General_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
			
			SetGrid();
			SetComboBox();
			SetSlider();
			SetColorButton();
			SetToggleSwitch();
			m_Sentinel = true;
			EnableDisableItems(sender, e);
		}
		
		private void SetGrid()
		{
			if (grid4_1 == null)    grid4_1 = new Grid();
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
			if (ts3 == null)	ts3 = new ToggleSwitch();
			if (ts4 == null)	ts4 = new ToggleSwitch();
			if (ts5 == null)	ts5 = new ToggleSwitch();
			if (ts6 == null)	ts6 = new ToggleSwitch();
			if (ts7 == null)	ts7 = new ToggleSwitch();
			if (ts8 == null)	ts8 = new ToggleSwitch();
			if (ts9 == null)	ts9 = new ToggleSwitch();
			if (ts10 == null)	ts10 = new ToggleSwitch();
			ts1.IsOn = bDisplayIMEModeOnCursor;
			ts3.IsOn = bOffChangedFocus;
			ts4.IsOn = bForceHiragana;
			ts5.IsOn = bDoModeDispByIMEKeyDown;
			ts6.IsOn = bDoModeDispByMouseBttnUp;
			ts7.IsOn = bDoModeDispByCtrlUp;
			ts8.IsOn = bDrawNearCaret;
			ts9.IsOn = bIMEModeForced;
			ts10.IsOn = bEnableEPHelper;
		}

		private void HandleToggleSwitch(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			ToggleSwitch ts = sender as ToggleSwitch;
			if (ts != null) {
				if (ts.Name == "ts1") {
					bDisplayIMEModeOnCursor = !bDisplayIMEModeOnCursor;
					EnableDisableItems(sender, e);
					UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					return;
				}
				else if (ts.Name == "ts3") bOffChangedFocus = !bOffChangedFocus;
				else if (ts.Name == "ts4") bForceHiragana = !bForceHiragana;
				else if (ts.Name == "ts5") bDoModeDispByIMEKeyDown = !bDoModeDispByIMEKeyDown;
				else if (ts.Name == "ts6") bDoModeDispByMouseBttnUp = !bDoModeDispByMouseBttnUp;
				else if (ts.Name == "ts7") bDoModeDispByCtrlUp = !bDoModeDispByCtrlUp;
				else if (ts.Name == "ts8") bDrawNearCaret = !bDrawNearCaret;
				else if (ts.Name == "ts9") bIMEModeForced = !bIMEModeForced;
				else if (ts.Name == "ts10") bEnableEPHelper = !bEnableEPHelper;
				EnableDisableItems(sender, e);
				UpdateProfile(SETTINGSEX_APPLY);
			}
		}

		private void SetSlider()
		{
			if (sl1_1 == null)	sl1_1 = new Slider();
			if (sl2 == null)	sl2 = new Slider();
			if (sl3 == null)	sl3 = new Slider();
			if (sl4 == null)	sl4 = new Slider();
			if (sl5 == null)	sl5 = new Slider();
			sl1_1.Value = iCursorSize;
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
				else if (sl.Name == "sl2")      iModeSize = (Int32)sl.Value;
				else if (sl.Name == "sl3")      dwDisplayModeTime = (Int32)(sl.Value * 10);
				else if (sl.Name == "sl4")      dwAdditionalWaitTime = (Int32)(sl.Value * 10);
				else if (sl.Name == "sl5")      dwWaitWaveTime = (Int32)(sl.Value * 10);
				EnableDisableItems(sender, e);
				UpdateProfile(SETTINGSEX_APPLY);
			}
		}

		private void SetColorButton()
		{
			if (Button4 == null)    Button4 = new Button();
			if (Button5 == null)    Button5 = new Button();
			Color clr = Color.FromArgb(dwNearDrawMouseColor);
			Button4.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			clr = Color.FromArgb(dwNearDrawCaretColor);
			Button5.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
		}

		unsafe private void ColorButton_Click(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			if (e == null) { }
			Button btn = sender as Button;
			if (btn != null) {
				if (btn.Name == "Button4") {
					Color clr = Color.FromArgb(dwNearDrawMouseColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker1.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker1.PreviousColor = ColorPicker1.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button5") {
					Color clr = Color.FromArgb(dwNearDrawCaretColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker2.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker2.PreviousColor = ColorPicker2.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
			}
		}

		private void Button_Click(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			if (sender == null) { }
			if (e == null) { }
			Button btn = sender as Button;
			if (btn != null) {
				if (btn.Name == "CP_Button1") {
					Windows.UI.Color wclr = ColorPicker1.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseColor;
					dwNearDrawMouseColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout1.Hide();
					if (i != dwNearDrawMouseColor) {
						Color   clr = Color.FromArgb(dwNearDrawMouseColor);
						Button4.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button2") {
					Flyout1.Hide();
				}
				else if (btn.Name == "CP_Button3") {
					Windows.UI.Color wclr = ColorPicker2.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawCaretColor;
					dwNearDrawCaretColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout2.Hide();
					if (i != dwNearDrawCaretColor) {
						Color   clr = Color.FromArgb(dwNearDrawCaretColor);
						Button5.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button4") {
					Flyout2.Hide();
				}
			}
		}
	}
}
