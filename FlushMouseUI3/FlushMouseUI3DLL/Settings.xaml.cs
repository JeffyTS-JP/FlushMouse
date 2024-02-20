//
// Settings.cs
//      Copyright (C) 2023 JeffyTS
//
// No.      Date            Name            Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000    2023/12/10  JeffyTS     New edit.
//

using WinRT;

using Microsoft.UI;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls.Primitives;
using Microsoft.UI.Xaml.Media;

using System;
using System.Collections.ObjectModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Threading;
using System.Reflection;
using System.Runtime.ConstrainedExecution;
using System.Runtime.InteropServices;
using System.Runtime.Intrinsics.Arm;
using System.Xml.Linq;

using Windows.Foundation;
using Windows.Graphics;
using Windows.Graphics.Display;
using Windows.UI.Popups;
using Windows.System;

using ABI.Windows.Foundation;

namespace FlushMouseUI3DLL
{
	public sealed partial class Settings
	{
		[LibraryImport("User32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.SysInt)]
		internal static partial IntPtr SendMessageW(Int64 hWnd, int Msg, Int64 wParam, Int64 lParam);
		[LibraryImport("User32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		internal static partial bool EnableMenuItem(IntPtr hMenu, uint uIDEnableItem, uint uEnable);
		internal const UInt32 SC_CLOSE = 0x0000F060;
		internal const UInt32 MF_DISABLED = 0x00000002;
		[LibraryImport("User32.dll", SetLastError = true)]
        internal static partial IntPtr GetSystemMenu(IntPtr hWnd, [MarshalAs(UnmanagedType.Bool)] bool bRevert);
		[LibraryImport("User32.dll", SetLastError = true)]
        internal static partial UInt32 GetDpiForWindow(Int64 hWnd);
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
		public bool bDisplayIMEModeByWindow { get; set; }
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
		public Int32 dwNearMouseColor { get; set; }
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

				Window_Width = 700; Window_Height = 780;
				wSettings.SetIcon("FlushMouse.ico");
			}
			else {
				wSettings.Show();
			}
		}

		private void EnableDisableItems(object sender, RoutedEventArgs e) {
			if (e == null) { }
			if (sender != null) {
				if (bDisplayIMEModeOnCursor) {
					if (Combo1 == null)	Combo1 = new ComboBox();
					if (Combo1 != null)	Combo1.IsEnabled = true;
					if (bDisplayIMEModeByWindow) {
						if (grid4_1 != null)	grid4_1.Visibility = Visibility.Collapsed;
					}
					else {
						if (grid4_1 != null)	grid4_1.Visibility = Visibility.Visible;
					}
				}
				else {
					if (Combo1 != null)	Combo1.IsEnabled = false;
					if (grid4_1 == null)	grid4_1 = new Grid();
					if (grid4_1 != null)	grid4_1.Visibility = Visibility.Visible;
				}
				if (bDoModeDispByIMEKeyDown || bDoModeDispByMouseBttnUp) {
					if (sl2 == null)	sl2 = new Slider();
					else sl2.IsEnabled = true;
					if (sl3 == null)	sl3 = new Slider();
					else sl3.IsEnabled = true;
					if (sl4 == null)	sl4 = new Slider();
					else sl4.IsEnabled = true;
				}
				else {
					if (sl2 != null)	sl2.IsEnabled = false;
					if (sl3 != null)	sl3.IsEnabled = false;
					if (sl4 != null)	sl4.IsEnabled = false;
				}
				if (bDoModeDispByCtrlUp) {
					if (sl5 == null)	sl5 = new Slider();
					else sl5.IsEnabled = true;
				}
				else {
					if (sl5 != null)	sl5.IsEnabled = false;
				}
			}
		}
		
		unsafe private void Settings_Loading(FrameworkElement sender, object args)
		{
			if (sender == null) { }
			if (args == null) { }
			if (wSettings != null) { }
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
					EnableDisableItems(sender, e);
				}
				wSettings.Show(true);
			}
		}

		private void    SetComboBox()
		{
			ComboBox combo = (ComboBox)Combo1;
			if (combo != null) {
				if (bDisplayFocusWindowIME) combo.SelectedItem = Item1;
				else combo.SelectedItem = Item2;
			}
		}

		private void Combo1_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
		}

		private void Combo1_SelectionChanged(object sender, RoutedEventArgs e)
		{
			ComboBox combo = sender as ComboBox;
			ComboBoxItem selectedItem = (ComboBoxItem)combo.SelectedItem;
			if (selectedItem != null) {
				if (selectedItem.Name == "Item1") bDisplayFocusWindowIME = true;
				else if (selectedItem.Name == "Item2") bDisplayFocusWindowIME = false;
				EnableDisableItems(sender, e);
				SendMessageW(hMainWnd, msg, 2, 0);
			}
		}

		private void SetToggleSwitch()
		{
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
			ToggleSwitch ts = sender as ToggleSwitch;
			if (ts != null) {
				if (ts.Name == "ts1") {
					bDisplayIMEModeOnCursor = ts1.IsOn;
					EnableDisableItems(sender, e);
					SendMessageW(hMainWnd, msg, 3, 0);
					return;
				}
				else if (ts.Name == "ts3") bOffChangedFocus = ts3.IsOn;
				else if (ts.Name == "ts4") bForceHiragana = ts4.IsOn;
				else if (ts.Name == "ts5") bDoModeDispByIMEKeyDown = ts5.IsOn;
				else if (ts.Name == "ts6") bDoModeDispByMouseBttnUp = ts6.IsOn;
				else if (ts.Name == "ts7") bDoModeDispByCtrlUp = ts7.IsOn;
				else if (ts.Name == "ts8") bDrawNearCaret = ts8.IsOn;
				else if (ts.Name == "ts9") bIMEModeForced = ts9.IsOn;
				else if (ts.Name == "ts10") bEnableEPHelper = ts10.IsOn;
				EnableDisableItems(sender, e);
				SendMessageW(hMainWnd, msg, 2, 0);
			}
		}

		private void SetSlider()
		{
			sl1_1.Value = iCursorSize;
			sl2.Value = iModeSize;
			sl3.Value = dwDisplayModeTime / 10;
			sl4.Value = dwAdditionalWaitTime / 10;
			sl5.Value = dwWaitWaveTime / 10;
		}

		private void Slider_ValueChanged(object sender, RoutedEventArgs e)
		{
			Slider sl = sender as Slider;
			if (sl != null) {
				if (sl.Name == "sl1_1")			iCursorSize = (Int32)sl.Value;
				else if (sl.Name == "sl2")		iModeSize = (Int32)sl.Value;
				else if (sl.Name == "sl3")		dwDisplayModeTime = (Int32)(sl.Value * 10);
				else if (sl.Name == "sl4")		dwAdditionalWaitTime = (Int32)(sl.Value * 10);
				else if (sl.Name == "sl5")		dwWaitWaveTime = (Int32)(sl.Value * 10);
				EnableDisableItems(sender, e);
				SendMessageW(hMainWnd, msg, 2, 0);
			}
		}

		private void SetColorButton()
		{
			Color clr = Color.FromArgb(dwNearDrawMouseColor);
			Button4.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			clr = Color.FromArgb(dwNearDrawCaretColor);
			Button5.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
		}

		unsafe private void ColorButton_Click(object sender, RoutedEventArgs e)
		{
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

		// Button
		private void Button_Click(object sender, RoutedEventArgs e)
		{
			Button btn = sender as Button;
			if (btn != null) {
				if (btn.Name == "Button1") {
					SendMessageW(hMainWnd, msg, 0, 0);
				}
				else if (btn.Name == "CP_Button1") {
					Windows.UI.Color wclr = ColorPicker1.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseColor;
					dwNearDrawMouseColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout1.Hide();
					if (i != dwNearDrawMouseColor) {
						Color	clr = Color.FromArgb(dwNearDrawMouseColor);
						Button4.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						SendMessageW(hMainWnd, msg, 3, 0);
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
						Color	clr = Color.FromArgb(dwNearDrawCaretColor);
						Button5.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						SendMessageW(hMainWnd, msg, 3, 0);
					}
				}
				else if (btn.Name == "CP_Button4") {
					Flyout2.Hide();
				}
			}
		}

		private void Window_Closed(object sender, WindowEventArgs args)
		{
			if (sender == null) { }
			if (args == null) { }
			SendMessageW(hMainWnd, msg, 2, 0);
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