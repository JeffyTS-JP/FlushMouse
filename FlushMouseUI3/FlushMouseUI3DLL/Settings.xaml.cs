//
// Settings.xaml.cs
//
//      Copyright (C) 1993- JeffyTS. All rights reserved.
//      Licensed under the GPL-2.0 License.
//
// No.      Date            Name            Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000    2023/12/10  JeffyTS     New edit.
//

using Microsoft.UI;
using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

using System;
using System.Diagnostics;

using Windows.Graphics;

using static FlushMouseUI3DLL.Miscs;

#pragma warning disable IDE0079
namespace FlushMouseUI3DLL {
	public partial class Settings
	{
		internal const String CLASS_FLUSHMOUSE = "FlushMouse-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}";
		internal const String CLASS_FLUSHMOUSESETTINGS = "FlushMouseSettings-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}";

		public const UInt32 WM_SETTINGSEX = (0x0400 + 0xfe);
		public const Int64 SETTINGSEX_OK = 0;
		public const Int64 SETTINGSEX_CANCEL = 1;
		public const Int64 SETTINGSEX_APPLY = 2;
		public const Int64 SETTINGSEX_RELOAD_REGISTRY = 3;
		public const Int64 SETTINGSEX_RELOAD_MOUSECURSOR = 4;
		public const Int64 SETTINGSEX_SETTINGS_CHANGE_PANE = 10;
		public const Int64 SETTINGSEX_SETTINGS_STARTED = 11;
		public const Int64 SETTINGSEX_SETTINGS_SETREGISTRY = 12;
		public const Int64 SETTINGSEX_SETTINGS_GENERAL_SETREGISTRY = 13;
		public const Int64 SETTINGSEX_SETTINGS_IMEMODE_SETREGISTRY = 14;
		public const Int64 SETTINGSEX_SETTINGS_CLOSE = 15;
		public const Int64 SETTINGSEX_SYNTP_START = 20;
		public const Int64 SETTINGSEX_SYNTP_IS_STARTED = 21;
		public const Int64 SETTINGSEX_SYNTP_STOP = 22;
		public const Int64 SETTINGSEX_SYNTP_SETREGISRY = 23;

		public static Settings g_Settings { get; set; }
		public static Int64 g_hSettingsWnd { get; set; }

		public static Int64 g_hMainWnd { get; set; }
		public static UInt32 g_uMsg { get; set; }
		private static RectDouble g_ContentFrameRectDouble;
		public static RectDouble G_ContentFrameRectDouble { get => g_ContentFrameRectDouble; set => g_ContentFrameRectDouble = value; }

		private static AppWindow m_AppWindow { get; set; }
		private static RectDouble m_WindowRectDouble;

		internal const double Window_Width = 1050;
		internal const double Window_Height = 940;
		internal const double Window_Min_Width = 350;
		internal const double Window_Min_Height = 300;
		internal const double Window_CompactPaneLength = 48;
		internal const double Window_OpenPaneLength = 220;

		private static Int32 m_SelectedPane { get; set; }
		internal const Int32 SETTINGSEX_SELECTEDPANE_GENERAL = 1;
		internal const Int32 SETTINGSEX_SELECTEDPANE_IMEMODE = 2;
		internal const Int32 SETTINGSEX_SELECTEDPANE_SYNTPHELPER = 3;
		internal const Int32 SETTINGSEX_SELECTEDPANE_ABOUT = 4;
		private static General m_General { get; set; }
		private static IMEMode m_IMEMode { get; set; }
		private static SynTP_Helper m_SynTP_Helper { get; set; }
		private static About m_About { get; set; }

		public static bool bIsPaneOpen { get; set; }
		public static Int32 dwSettingsX { get; set; }
		public static Int32 dwSettingsY { get; set; }
		public static Int32 dwSettingsWidth { get; set; }
		public static Int32 dwSettingsHeight { get; set; }
	}

	public sealed partial class Settings : Window
	{
		public Settings(Int32 SelectedPane)
		{
			InitializeComponent();

			m_SelectedPane = SelectedPane;

			if (g_Settings == null) {
				ExtendsContentIntoTitleBar = true;
				SetTitleBar(TitleBar);
				g_hSettingsWnd = (Int64)WinRT.Interop.WindowNative.GetWindowHandle(this);
				WindowId windowId = Win32Interop.GetWindowIdFromWindow((nint)g_hSettingsWnd);
				m_AppWindow = AppWindow.GetFromWindowId(windowId);
				OverlappedPresenter op = OverlappedPresenter.Create();
				op.IsMaximizable = false;
				op.IsMinimizable = true;
				op.IsResizable = true;
				op.IsAlwaysOnTop = false;
				m_AppWindow.SetPresenter(op);

				m_AppWindow.SetIcon("FlushMouse.ico");
			}
			else {
				m_AppWindow.Show();
			}
		}

		public void ChangePane(Int32 SelectedPane)
		{
			m_SelectedPane = SelectedPane;
			switch (m_SelectedPane) {
				case SETTINGSEX_SELECTEDPANE_GENERAL:
					if (!Menu1.IsSelected) Menu1.IsSelected = true;
					break;
				case SETTINGSEX_SELECTEDPANE_IMEMODE:
					if (!Menu2.IsSelected) Menu2.IsSelected = true;
					break;
				case SETTINGSEX_SELECTEDPANE_SYNTPHELPER:
					if (!Menu3.IsSelected) Menu3.IsSelected = true;
					break;
				case SETTINGSEX_SELECTEDPANE_ABOUT:
					if (!Menu4.IsSelected) Menu4.IsSelected = true;
					break;
				default:
					if (!Menu1.IsSelected) Menu1.IsSelected = true;
					break;
			}
		}

#pragma warning disable CA1822
		private void navigationView_Loading(FrameworkElement sender, object args)
		{
			if (args == null) { }
			if (sender != null) {
				NavigationView NaviView = sender as NavigationView;

				NaviView.IsPaneOpen = bIsPaneOpen;

				SizeDouble sizeDouble;
				sizeDouble.Width = (int)Window_OpenPaneLength;
				sizeDouble.Height = (int)Window_CompactPaneLength;
				CalcWindowSizeByDPI(g_hSettingsWnd, sizeDouble, out sizeDouble);
				NaviView.OpenPaneLength = sizeDouble.Width;
				NaviView.CompactPaneLength = sizeDouble.Height;
				if ((dwSettingsX == 0) && (dwSettingsY == 0) && (dwSettingsWidth == 0) && (dwSettingsHeight == 0)) {
					if (bIsPaneOpen) {
						m_WindowRectDouble.Width = Window_Width + sizeDouble.Height;
					}
					else {
						m_WindowRectDouble.Width = Window_Width;
					}
					m_WindowRectDouble.Height = Window_Height;
					CalcWindowCentralizeByDesktop(g_hSettingsWnd, m_WindowRectDouble, out m_WindowRectDouble);
				}
				else if ((dwSettingsWidth < Window_Min_Width) && (dwSettingsHeight < Window_Min_Height)) {
					m_WindowRectDouble.X = dwSettingsX;
					m_WindowRectDouble.Y = dwSettingsY;
					if (bIsPaneOpen) {
						m_WindowRectDouble.Width = Window_Min_Width + sizeDouble.Height;
					}
					else {
						m_WindowRectDouble.Width = Window_Min_Width;
					}
					m_WindowRectDouble.Height = Window_Min_Height;
				}
				else {
					m_WindowRectDouble.X = dwSettingsX;
					m_WindowRectDouble.Y = dwSettingsY;
					m_WindowRectDouble.Width = dwSettingsWidth;
					m_WindowRectDouble.Height = dwSettingsHeight;
				}

				RectInt32 rect;
				rect.X = (int)m_WindowRectDouble.X;
				rect.Y = (int)m_WindowRectDouble.Y;
				rect.Width = (int)m_WindowRectDouble.Width;
				rect.Height = (int)m_WindowRectDouble.Height;
				m_AppWindow.MoveAndResize(rect);
				if (CalcWindowAdjustByMonitor(g_hSettingsWnd, m_WindowRectDouble, out m_WindowRectDouble) == true) {
					rect.X = (int)m_WindowRectDouble.X;
					rect.Y = (int)m_WindowRectDouble.Y;
					rect.Width = (int)m_WindowRectDouble.Width;
					rect.Height = (int)m_WindowRectDouble.Height;
					m_AppWindow.MoveAndResize(rect);
				}
			}
		}
#pragma warning restore CA1822

		private void contentFrame_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
			if (m_General == null) m_General = new General();
			if (m_IMEMode == null) m_IMEMode = new IMEMode();
			if (m_SynTP_Helper == null) m_SynTP_Helper = new SynTP_Helper();
			if (m_About == null) m_About = new About();

			ChangePane(m_SelectedPane);

			m_AppWindow.Changed += Window_SizeChanged;
			m_AppWindow.Show();
		}

		private void navigationView_SelectionChanged(NavigationView sender, object args)
		{
			if (sender == null) { }
			if (args != null) {
				NavigationViewSelectionChangedEventArgs _args = args as NavigationViewSelectionChangedEventArgs;
				NavigationViewItem selectedItem = _args.SelectedItem as NavigationViewItem;
				if (selectedItem != null) {
					System.Type pageType = null;
					if ((string)selectedItem.Tag == "Menu1") {
						sender.Header = "基本動作設定";
						if (m_General == null) { m_General = new General(); }
						pageType = m_General.GetType();
					}
					else if ((string)selectedItem.Tag == "Menu2") {
						sender.Header = "IME モード表示設定";
						if (m_IMEMode == null) { m_IMEMode = new IMEMode(); }
						pageType = m_IMEMode.GetType();
					}
					else if ((string)selectedItem.Tag == "Menu3") {
						sender.Header = "SynTP Helper";
						if (m_SynTP_Helper == null) { m_SynTP_Helper = new SynTP_Helper(); }
						pageType = m_SynTP_Helper.GetType();
					}
					else if ((string)selectedItem.Tag == "Menu4") {
						sender.Header = "バージョン";
						if (m_About == null) { m_About = new About(); }
						pageType = m_About.GetType();
					}
					contentFrame.Navigate(pageType);
					navigationView.SelectedItem = contentFrame.Navigate(pageType);
				}
				else {
					SetWindowSize();
				}
			}
		}

		private void navigationView_BackRequested(NavigationView sender, object args)
		{
			if (sender == null) { }
			if (args == null) { }
			if (contentFrame.CanGoBack) {
				contentFrame.GoBack();
			}
		}

#pragma warning disable CA1822
		private void navigationView_PaneOpening(NavigationView sender, object args)
		{
			if (args == null) { }
			if (sender != null) {
				SizeDouble sizeDouble;
				sizeDouble.Width = Window_OpenPaneLength;
				sizeDouble.Height = Window_CompactPaneLength;
				CalcWindowSizeByDPI(g_hSettingsWnd, sizeDouble, out sizeDouble);
				sender.OpenPaneLength = sizeDouble.Width;
				sender.CompactPaneLength = sizeDouble.Height;
			}
		}
#pragma warning restore CA1822

		private void navigationView_PaneOpened(NavigationView sender, object args)
		{
			if (args == null) { }
			if (sender != null) {
				SetWindowSize();
				bIsPaneOpen = true;
				UpdateProfile(SETTINGSEX_SETTINGS_SETREGISTRY);
			}
		}

#pragma warning disable CA1822
		private void navigationView_PaneClosing(NavigationView sender, object args)
		{
			if (args == null) { }
			if (sender != null) {
				SizeDouble sizeDouble;
				sizeDouble.Width = Window_OpenPaneLength;
				sizeDouble.Height = Window_CompactPaneLength;
				CalcWindowSizeByDPI(g_hSettingsWnd, sizeDouble, out sizeDouble);
				sender.OpenPaneLength = sizeDouble.Width;
			}
		}
#pragma warning restore CA1822

		private void navigationView_PaneClosed(NavigationView sender, object args)
		{
			if (args == null) { }
			if (sender != null) {
				SetWindowSize();
				bIsPaneOpen = false;
				UpdateProfile(SETTINGSEX_SETTINGS_SETREGISTRY);
			}
		}

		private void SetWindowSize()
		{
			if (navigationView != null) {
				if ((settingsMain.ActualWidth != 0) && (settingsMain.ActualHeight != 0)) {
					navigationView.Width = settingsMain.ActualWidth;
					navigationView.Height = settingsMain.ActualHeight - TitleBar.ActualHeight;
				}
				bIsPaneOpen = navigationView.IsPaneOpen;
				m_WindowRectDouble.X = AppWindow.Position.X; m_WindowRectDouble.Y = AppWindow.Position.Y;
				m_WindowRectDouble.Width = AppWindow.Size.Width; m_WindowRectDouble.Height = AppWindow.Size.Height;
			}
			if (contentFrame != null) {
				g_ContentFrameRectDouble.Width = contentFrame.ActualWidth;
				g_ContentFrameRectDouble.Height = contentFrame.ActualHeight;
			}
			if (navigationView != null) {
				bIsPaneOpen = navigationView.IsPaneOpen;
				m_WindowRectDouble.X = AppWindow.Position.X; m_WindowRectDouble.Y = AppWindow.Position.Y;
				m_WindowRectDouble.Width = AppWindow.Size.Width; m_WindowRectDouble.Height = AppWindow.Size.Height;
			}
		}

		private void Window_SizeChanged(AppWindow sender, object args)
		{
			if (sender == null) { }
			if (args != null) {
				AppWindowChangedEventArgs _args = args as AppWindowChangedEventArgs;
				if (_args.DidSizeChange) {
					SetWindowSize();
					m_WindowRectDouble.X = (Double)sender.Position.X;
					m_WindowRectDouble.Y = (Double)sender.Position.Y;

					dwSettingsX = (Int32)m_WindowRectDouble.X;
					dwSettingsY = (Int32)m_WindowRectDouble.Y;
					dwSettingsWidth = (Int32)m_WindowRectDouble.Width;
					dwSettingsHeight = (Int32)m_WindowRectDouble.Height;
					if (IsIconic(g_hSettingsWnd) == false) {
						UpdateProfile(SETTINGSEX_SETTINGS_SETREGISTRY);
					}
				}
				else if (_args.DidPositionChange) {
					m_WindowRectDouble.X = sender.Position.X;
					m_WindowRectDouble.Y = sender.Position.Y;

					dwSettingsX = (Int32)m_WindowRectDouble.X;
					dwSettingsY = (Int32)m_WindowRectDouble.Y;
					dwSettingsWidth = (Int32)m_WindowRectDouble.Width;
					dwSettingsHeight = (Int32)m_WindowRectDouble.Height;
					if (IsIconic(g_hSettingsWnd) == false) {
						UpdateProfile(SETTINGSEX_SETTINGS_SETREGISTRY);
					}
				}
				else if (_args.DidPresenterChange) {
					if (IsIconic(g_hSettingsWnd) == false) {
						Debug.WriteLine($"NOT Iconic3：({sender.Position.X}, {sender.Position.Y})");
						UpdateProfile(SETTINGSEX_SETTINGS_SETREGISTRY);
					}
					SetWindowSize();
				}
			}
		}

		private void Window_Closed(object sender, WindowEventArgs args)
		{
			if (sender == null) { }
			if (args == null) { }
			UpdateProfile(SETTINGSEX_SETTINGS_CLOSE);
			if (g_Settings != null) {
				g_Settings = null;
				g_hSettingsWnd = 0;
			}
			Int64 _hWnd = FindWindowW(CLASS_FLUSHMOUSESETTINGS, null);
			if (_hWnd != 0) {
				SendMessageW(_hWnd, WM_DESTROY, 0, 0);
			}
		}

		public static Int64 UpdateProfile(Int64 iSettingsEX)
		{
			Int64 iRet = 0;
			if (g_hMainWnd != 0) {
				iRet = SendMessageW(g_hMainWnd, g_uMsg, iSettingsEX, 0);
				if (iRet == 0) return 0;
			}
			Int64 _hWnd = FindWindowW(CLASS_FLUSHMOUSE, null);
			if ((_hWnd != 0) && (_hWnd != g_hMainWnd)) {
				SendMessageW(_hWnd, g_uMsg, SETTINGSEX_RELOAD_REGISTRY, 0);
				iRet = SendMessageW(_hWnd, g_uMsg, iSettingsEX, 0);
			}
			return iRet;
		}
	}
}
#pragma warning restore IDE0079


/* = EOF = */
