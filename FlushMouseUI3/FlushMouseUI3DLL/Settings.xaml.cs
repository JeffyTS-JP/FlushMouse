//
// Settings.cs
//		Copyright (C) 2023 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2023/12/10  JeffyTS  	New edit.
//

using WinRT;
using Microsoft.UI;
using Microsoft.UI.Xaml;
using Microsoft.UI.Windowing;

using System;
using Windows.Graphics;
using Windows.Foundation;
using System.Threading;
using Windows.Graphics.Display;
using System.Runtime.InteropServices;
using ABI.Windows.Foundation;
using Microsoft.UI.Xaml.Controls;
using System.IO;
using System.Reflection;
using Microsoft.UI.Xaml.Media;
using System.Collections.ObjectModel;
using System.Linq;
using Microsoft.UI.Xaml.Controls.Primitives;
using Windows.UI.Popups;
using System.Drawing;
using System.Runtime.Intrinsics.Arm;
using System.Xml.Linq;

namespace FlushMouseUI3DLL
{
	public sealed partial class Settings
	{
		[DllImport("user32.dll")]
		extern static int SendMessage(Int64 hWnd, int Msg, Int64 wParam, Int64 lParam);
		[DllImport("user32.dll")]
		static extern bool EnableMenuItem(IntPtr hMenu, uint uIDEnableItem, uint uEnable);
		internal const UInt32 SC_CLOSE = 0x0000F060;
		internal const UInt32 MF_DISABLED = 0x00000002;
		[DllImport("user32.dll")]
		static extern IntPtr GetSystemMenu(IntPtr hWnd, bool bRevert);
		[DllImport("user32.dll")]
		static extern UInt32 GetDpiForWindow(Int64 hWnd);
		internal const UInt32 USER_DEFAULT_SCREEN_DPI = 96;
		
		public Settings settings { get; set; }
		public Int64 hSettingsWnd { get; set; }
		public AppWindow wSettings { get; private set; }

		public Int32 Monitor_DPI { get; set; }
		public Int32 Monitor_Left { get; set; }
		public Int32 Monitor_Top { get; set; }
		public Int32 Monitor_Right { get; set; }
		public Int32 Monitor_Bottom { get; set; }
		
		public Int32 Window_Top { get; set; }
		public Int32 Window_Left { get; set; }
		public Int32 Window_Width { get; set; }
		public Int32 Window_Height { get; set; }

		public Int64 hMainWnd { get; set; }
		public int msg { get; set; }

		public bool bDisplayFocusWindowIME { get; set; }
		public bool bDisplayIMEModeOnCursor { get; set; }
		public bool bOffChangedFocus { get; set; }
		public bool bForceHiragana { get; set; }
		public bool bDoModeDispByIMEKeyDown { get; set; }
		public bool bDoModeDispByMouseBttnUp { get; set; }
		public bool bDoModeDispByCtrlUp { get; set; }
		public bool bDrawNearCaret { get; set; }
		public bool bIMEModeForced { get; set; }
		public bool bEnableEPHelper { get; set; }
		public Int32 iCursorSize { get; set; }
		public Int32 iModeSize { get; set; }
		public Int32 dwDisplayModeTime { get; set; }
		public Int32 dwAdditionalWaitTime { get; set; }
		public Int32 dwWaitWaveTime { get; set; }
		public Int32 dwNearDrawMouseColor { get; set; }
		public Int32 dwNearDrawCaretColor { get; set; }
	
	}

	public sealed partial class Settings : Window
	{
		public Settings()
		{
			InitializeComponent();
			if (wSettings == null) {
				hSettingsWnd = (Int64)WinRT.Interop.WindowNative.GetWindowHandle(this);
				WindowId windowId = Win32Interop.GetWindowIdFromWindow((nint)hSettingsWnd);
				wSettings = AppWindow.GetFromWindowId(windowId);
				var op = OverlappedPresenter.Create();
				op.IsMaximizable = false;
				op.IsMinimizable = false;
				op.IsResizable = false;
				op.IsAlwaysOnTop = false;
				wSettings.SetPresenter(op);

				IntPtr hMenu = GetSystemMenu((nint)hSettingsWnd, false);
				if (hMenu != IntPtr.Zero) {
				}

				Window_Width = 700; Window_Height = 780;
				wSettings.SetIcon("FlushMouse.ico");
			}
			else {
				wSettings.Show();
			}
		}

		unsafe private void Settings_Loading(FrameworkElement sender, object args)
		{
			if (wSettings != null) {
			}
		}

		private void Settings_Loaded(object sender, RoutedEventArgs e)
		{
			if (wSettings != null) {
				Grid grid = sender as Grid;
				if (grid != null) {
					SetComboBox();
					SetToggleSwitch();
					SetSlider();
					SetColorButton();
					EnabledDisabled_Button3(false);
				}
				wSettings.Show(true);
			}
		}

		// ComboBox (IMEÉÇÅ[ÉhÇÃéÊìæï˚ñ@)
		private void	SetComboBox()
		{
			ComboBox combo = (ComboBox)Combo1;
			if (combo != null) {
				if (bDisplayFocusWindowIME) combo.SelectedItem = Item1;
				else combo.SelectedItem = Item2;
			}
		}

		private void Combo1_Loaded(object sender, RoutedEventArgs e)
		{
		}

		private void Combo1_SelectionChanged(object sender, RoutedEventArgs e)
		{
			ComboBox combo = sender as ComboBox;
			ComboBoxItem selectedItem = (ComboBoxItem)combo.SelectedItem;
			if (selectedItem != null) {
				if (selectedItem.Name == "Item1") bDisplayFocusWindowIME = true;
				else if (selectedItem.Name == "Item2") bDisplayFocusWindowIME = false;
				SendMessage(hMainWnd, msg, 3, 0);
				EnabledDisabled_Button3(true);
			}
		}

		// ToggleSwitch
		private void SetToggleSwitch()
		{
			if (bDisplayIMEModeOnCursor != true) ts1.IsOn = false; else ts1.IsOn = true;
			if (bOffChangedFocus != true) ts2.IsOn = false; else ts2.IsOn = true;
			if (bForceHiragana != true) ts3.IsOn = false; else ts3.IsOn = true;
			if (bDoModeDispByIMEKeyDown != true) ts4.IsOn = false; else ts4.IsOn = true;
			if (bDoModeDispByMouseBttnUp != true) ts5.IsOn = false; else ts5.IsOn = true;
			if (bDoModeDispByCtrlUp != true) ts6.IsOn = false; else ts6.IsOn = true;
			if (bDrawNearCaret != true) ts7.IsOn = false; else ts7.IsOn = true;
			if (bIMEModeForced != true) ts8.IsOn = false; else ts8.IsOn = true;
			if (bEnableEPHelper != true) ts9.IsOn = false; else ts9.IsOn = true;
		}

		private void HandleToggleSwitch(object sender, RoutedEventArgs e)
		{
			ToggleSwitch ts = sender as ToggleSwitch;
			if (ts != null) {
				if (ts.Name == "ts1") bDisplayIMEModeOnCursor = ts1.IsOn;
				else if (ts.Name == "ts2") bOffChangedFocus = ts2.IsOn;
				else if (ts.Name == "ts3") bForceHiragana = ts3.IsOn;
				else if (ts.Name == "ts4") bDoModeDispByIMEKeyDown = ts4.IsOn;
				else if (ts.Name == "ts5") bDoModeDispByMouseBttnUp = ts5.IsOn;
				else if (ts.Name == "ts6") bDoModeDispByCtrlUp = ts6.IsOn;
				else if (ts.Name == "ts7") bDrawNearCaret = ts7.IsOn;
				else if (ts.Name == "ts8") bIMEModeForced = ts8.IsOn;
				else if (ts.Name == "ts9") bEnableEPHelper = ts9.IsOn;
				SendMessage(hMainWnd, msg, 3, 0);
				EnabledDisabled_Button3(true);
			}
		}

		// Slider
		private void SetSlider()
		{
			sl1.Value = iCursorSize;
			sl2.Value = iModeSize;
			sl3.Value = dwDisplayModeTime / 10;
			sl4.Value = dwAdditionalWaitTime / 10;
			sl5.Value = dwWaitWaveTime / 10;
		}

		private void Slider_ValueChanged(object sender, RoutedEventArgs e)
		{
			Slider sl = sender as Slider;
			if (sl != null) {
				if (sl.Name == "sl1") {
					iCursorSize = (Int32)sl.Value;
					SendMessage(hMainWnd, msg, 2, 0);
					EnabledDisabled_Button3(true);
					return;
				}
				else if (sl.Name == "sl2") iModeSize = (Int32)sl.Value;
				else if (sl.Name == "sl3") dwDisplayModeTime = (Int32)(sl.Value * 10);
				else if (sl.Name == "sl4") dwAdditionalWaitTime = (Int32)(sl.Value * 10);
				else if (sl.Name == "sl5") dwWaitWaveTime = (Int32)(sl.Value * 10);
				SendMessage(hMainWnd, msg, 3, 0);
				EnabledDisabled_Button3(true);
			}
		}

		// Color Button
		private void SetColorButton()
		{
			Color clr = Color.FromArgb(dwNearDrawMouseColor);
			NearMouse.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			clr = Color.FromArgb(dwNearDrawCaretColor);
			NearCaret.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
		}

		unsafe private void ColorButton_Click(object sender, RoutedEventArgs e)
		{
			Button btn = sender as Button;
			if (btn != null) {
				// Open Color Picker
				if (btn.Name == "NearMouse") {
					Color clr = Color.FromArgb(dwNearDrawMouseColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					myColorPicker1.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					myColorPicker1.PreviousColor = myColorPicker1.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "NearCaret") {
					Color clr = Color.FromArgb(dwNearDrawCaretColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					myColorPicker2.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					myColorPicker2.PreviousColor = myColorPicker2.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
			}
		}

		// Button
		private void Button_Click(object sender, RoutedEventArgs e)
		{
			Button btn = sender as Button;
			if (btn != null) {
				if (btn.Name == "Button1") {
					// OK
					SendMessage(hMainWnd, msg, 0, 0);
				}
				else if (btn.Name == "CP_Button1") {
					// OK
					Windows.UI.Color clr = myColorPicker1.Color;
					byte A = (byte)(255 - clr.A);
					byte R = clr.B;
					byte G = clr.G;
					byte B = clr.R;
					Int32 i = dwNearDrawMouseColor;
					dwNearDrawMouseColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					// Close the Flyout.
					if (i != dwNearDrawMouseColor) {
						SetColorButton();
						EnabledDisabled_Button3(true);
						SendMessage(hMainWnd, msg, 2, 0);
}
					flyout1.Hide();
				}
				else if (btn.Name == "CP_Button2") {
					// Cancel
					// Close the Flyout.
					flyout1.Hide();
				}
				else if (btn.Name == "CP_Button3") {
					// OK
					Windows.UI.Color clr = myColorPicker2.Color;
					byte A = (byte)(255 - clr.A);
					byte R = clr.B;
					byte G = clr.G;
					byte B = clr.R;
					Int32 i = dwNearDrawCaretColor;
					dwNearDrawCaretColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					// Close the Flyout.
					if (i != dwNearDrawCaretColor) {
						SetColorButton();
						EnabledDisabled_Button3(true);
						SendMessage(hMainWnd, msg, 2, 0);
					}
					flyout2.Hide();
				}
				else if (btn.Name == "CP_Button4") {
					// Cancel
					// Close the Flyout.
					flyout2.Hide();
				}
}
		}

		private void EnabledDisabled_Button3(bool b) {
		}
		
		private void Window_Closed(object sender, WindowEventArgs args)
		{
			SendMessage((Int64)hMainWnd, msg, 1, 0);
			if (wSettings != null) {
				wSettings = null;
				hSettingsWnd = 0;
			}
		}

		unsafe private void CalcWindowCentralize(int dpi, RectInt32 rectOutside, RectInt32* rectWindow)
		{
			rectWindow->Width = (int)((float)rectWindow->Width * (float)dpi / (float)USER_DEFAULT_SCREEN_DPI);
			rectWindow->Height = (int)((float)rectWindow->Height * (float)dpi / (float)USER_DEFAULT_SCREEN_DPI);
			rectWindow->X = rectOutside.X * 2 + (rectOutside.Width - rectOutside.X - rectWindow->Width) / 2;
			rectWindow->Y = rectOutside.Y * 2 + (rectOutside.Height - rectOutside.Y - rectWindow->Height) / 2;
		}

	}
}

/* = EOF = */