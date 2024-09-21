//
// Settings.xaml.cs
//      Copyright (C) 2023 JeffyTS
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
using System.Runtime.InteropServices;
using System.Threading.Tasks;

using Windows.Graphics;

namespace FlushMouseUI3DLL
{
    public struct RectDouble
    {
        public double X;
        public double Y;
        public double Width;
        public double Height;
    }
    
    public struct SizeDouble
    {
        public double Width;
        public double Height; 
    }

    public partial class Settings
    {
        [LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.I8)]
        internal static partial Int64 SendMessageW(Int64 hWnd, UInt32 uMsg, Int64 wParam, Int64 lParam);
        internal const UInt32 WM_DESTROY = (0x0002);

        [LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = false)]
        [return: MarshalAs(UnmanagedType.I8)]
        internal static partial Int64 FindWindowW(String lpClassName, String lpWindowName);

        [LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static partial bool EnableMenuItem(Int64 hMenu, UInt64 uIDEnableItem, UInt64 uEnable);
        internal const UInt32 SC_CLOSE = 0x0000F060;
        internal const UInt32 SC_MINIMIZE = 0x0000F020;
        internal const UInt32 SC_MAXIMIZE = 0x0000F030;
        internal const UInt32 SC_RESTORE = 0x0000F120;

        internal const UInt32 MF_DISABLED = 0x00000002;

        [LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.I8)]
        internal static partial Int64 GetSystemMenu(Int64 hWnd, [MarshalAs(UnmanagedType.Bool)] bool bRevert);

        [LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.U4)]
        internal static partial UInt32 GetDpiForWindow(Int64 hWnd);
        internal const UInt32 USER_DEFAULT_SCREEN_DPI = 96;

        [LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.I8)]
        internal static partial Int64 MonitorFromWindow(Int64 hWnd, UInt32 dwFlags);
        internal const UInt32 MONITOR_DEFAULTTONULL    = 0x00000000;
        internal const UInt32 MONITOR_DEFAULTTOPRIMARY = 0x00000001;
        internal const UInt32 MONITOR_DEFAULTTONEAREST = 0x00000002;

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto, Pack = 4)]
        internal class MONITORINFOEXW {
            public int          cbSize;
            public RectInt32    rcMonitor;
            public RectInt32    rcWork;
            public uint         dwFlags;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public char[] szDevice = new char[32];
        };
#pragma warning disable SYSLIB1054
        [DllImport("User32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern bool GetMonitorInfoW(Int64 hMonitor, [In, Out] MONITORINFOEXW lpmi);
#pragma warning restore SYSLIB1054

        [LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.I8)]
        internal static partial Int64 MessageBoxW(Int64 hWnd, string MessageBox, string lpCaption, UInt32 uType);
        public const UInt32 MB_OK = 0x00000000;
        public const UInt32 MB_ICONINFORMATION = 0x00000040;
        public const UInt32 MB_TOPMOST = 0x00040000;

        internal const String CLASS_FLUSHMOUSE = "FlushMouse-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}";
        internal const String CLASS_FLUSHMOUSESETTINGS = "FlushMouseSettings-{E598B54C-A36A-4CDF-BC77-7082CEEDAA46}";
        
        public const UInt32 WM_SETTINGSEX = (0x0400 + 0xfe);
        public const Int64 SETTINGSEX_OK = 0;
        public const Int64 SETTINGSEX_CANCEL = 1;
        public const Int64 SETTINGSEX_APPLY = 2;
        public const Int64 SETTINGSEX_RELOAD_REGISTRY = 3;
        public const Int64 SETTINGSEX_RELOAD_MOUSE = 4;
        public const Int64 SETTINGSEX_SETTINGS_CHANGE_PANE = 10;
        public const Int64 SETTINGSEX_SETTINGS_STARTED = 11;
        public const Int64 SETTINGSEX_SETTINGS_SETREGISTRY = 12;
        public const Int64 SETTINGSEX_SYNTP_START = 20;
        public const Int64 SETTINGSEX_SYNTP_IS_STARTED = 21;
        public const Int64 SETTINGSEX_SYNTP_STOP = 22;
        public const Int64 SETTINGSEX_SYNTP_SETREGISRY = 23;

        public static Settings g_Settings { get; set; }
        public Int64 g_hSettingsWnd { get; set; }
        
        public static Int64 g_hMainWnd { get; set; }
        public static UInt32 g_uMsg { get; set; }
        private static RectDouble g_ContentFrameRectDouble;
        public static RectDouble G_ContentFrameRectDouble { get => g_ContentFrameRectDouble; set => g_ContentFrameRectDouble = value; }

        private static AppWindow m_AppWindow { get; set; }
        private static RectDouble m_WindowRectDouble;

        internal const double Window_Width = 1000;
        internal const double Window_Height = 920;
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
        
        public static bool bDisplayFocusWindowIME { get; set; }
        public static bool bDisplayIMEModeOnCursor { get; set; }
        public static bool bDisplayIMEModeByWindow { get; set; }
        public static bool bDisplayIMEModeIMEOFF { get; set; }
        public static bool bOffChangedFocus { get; set; }
        public static bool bForceHiragana { get; set; }
        public static bool bDoModeDispByIMEKeyDown { get; set; }
        public static bool bDoModeDispByMouseBttnUp { get; set; }
        public static bool bDoModeDispByCtrlUp { get; set; }
        public static bool bDrawNearCaret { get; set; }
        public static bool bIMEModeForced { get; set; }
        public static bool bEnableEPHelper { get; set; }

        public static Int32 iCursorSize { get; set; }
        public static Int32 iIMEModeDistance { get; set; }
        public static Int32 iModeSize { get; set; }
        public static Int32 dwDisplayModeTime { get; set; }
        public static Int32 dwAdditionalWaitTime { get; set; }
        public static Int32 dwWaitWaveTime { get; set; }
        public static Int32 dwNearDrawMouseIMEOFFColor { get; set; }
        public static Int32 dwNearDrawMouseHANEISU_IMEONColor { get; set; }
        public static Int32 dwNearDrawMouseHANKANA_IMEONColor { get; set; }
        public static Int32 dwNearDrawMouseZENEISU_IMEONColor { get; set; }
        public static Int32 dwNearDrawMouseZENHIRA_IMEONColor { get; set; }
        public static Int32 dwNearDrawMouseZENKANA_IMEONColor { get; set; }
        public static Int32 dwNearDrawCaretIMEOFFColor { get; set; }
        public static Int32 dwNearDrawCaretHANEISU_IMEONColor { get; set; }
        public static Int32 dwNearDrawCaretHANKANA_IMEONColor { get; set; }
        public static Int32 dwNearDrawCaretZENEISU_IMEONColor { get; set; }
        public static Int32 dwNearDrawCaretZENHIRA_IMEONColor { get; set; }
        public static Int32 dwNearDrawCaretZENKANA_IMEONColor { get; set; }
        public static Int32 dwNearDrawMouseByWndIMEOFFColor { get; set; }
        public static Int32 dwNearDrawMouseByWndHANEISU_IMEONColor { get; set; }
        public static Int32 dwNearDrawMouseByWndHANKANA_IMEONColor { get; set; }
        public static Int32 dwNearDrawMouseByWndZENEISU_IMEONColor { get; set; }
        public static Int32 dwNearDrawMouseByWndZENHIRA_IMEONColor { get; set; }
        public static Int32 dwNearDrawMouseByWndZENKANA_IMEONColor { get; set; }

        public static Int32 dwSynTPHelper1 { get; set; }
        public static Int32 dwSynTPPadX { get; set; }
        public static Int32 dwSynTPPadY { get; set; }
        public static Int32 dwSynTPEdgeX { get; set; }
        public static Int32 dwSynTPEdgeY { get; set; }
        public static String szSynTPSendIPAddr1_1 { get; set; }
        public static String szSynTPSendIPAddr1_2 { get; set; }
        public static String szSynTPSendIPAddr1_3 { get; set; }
        public static String szSynTPSendIPAddr1_4 { get; set; }
        public static String szSynTPSendHostname1 { get; set; }
        public static Int32 dwSynTPPortNo1 { get; set; }
        public static bool bSynTPStarted1 { get; set; }
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
                var op = OverlappedPresenter.Create();
                op.IsMaximizable = false;
                op.IsMinimizable = false;
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

        private void navigationView_Loading(FrameworkElement sender, object args)
        {
            if (args == null) { }
            if (sender != null) {
                NavigationView NaviView = sender as NavigationView;
                
                NaviView.IsPaneOpen = bIsPaneOpen;
                
                SizeDouble sizeDouble;
                sizeDouble.Width = (int)Window_OpenPaneLength;
                sizeDouble.Height = (int)Window_CompactPaneLength;
                CalcWindowSizeByDPI(g_hSettingsWnd, ref sizeDouble);        
                NaviView.OpenPaneLength = sizeDouble.Width;
                NaviView.CompactPaneLength = sizeDouble.Height;
                if ((dwSettingsX == 0) && (dwSettingsY == 0) && (dwSettingsWidth == 0) && (dwSettingsHeight == 0))
                {
                    if (bIsPaneOpen) {
                        m_WindowRectDouble.Width = Window_Width + sizeDouble.Height;
                    }
                    else
                    {
                        m_WindowRectDouble.Width = Window_Width;
                    }
                    m_WindowRectDouble.Height = Window_Height;
                    CalcWindowCentralizeByDesktop(g_hSettingsWnd, ref m_WindowRectDouble);
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
                m_AppWindow.MoveAndResize(rect);
                if (CalcWindowAdjustByMonitor(g_hSettingsWnd, ref m_WindowRectDouble) == true) {
                    rect.X = (int)m_WindowRectDouble.X;
                    rect.Y = (int)m_WindowRectDouble.Y;
                    rect.Width = (int)m_WindowRectDouble.Width;
                    rect.Height = (int)m_WindowRectDouble.Height;
                    m_AppWindow.MoveAndResize(rect);
                }
            }
        }

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
            if (args != null)
            {
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

        private void navigationView_PaneOpening(NavigationView sender, object args)
        {
            if (args == null) { }
            if (sender != null) {
                SizeDouble sizeDouble;
                sizeDouble.Width = Window_OpenPaneLength;
                sizeDouble.Height = Window_CompactPaneLength;
                CalcWindowSizeByDPI(g_hSettingsWnd, ref sizeDouble);
                sender.OpenPaneLength = sizeDouble.Width;
                sender.CompactPaneLength = sizeDouble.Height;
            }
        }

        private void navigationView_PaneOpened(NavigationView sender, object args)
        {
            if (args == null) { }
            if (sender != null) {
                SetWindowSize();
                bIsPaneOpen = true;
                UpdateProfile(SETTINGSEX_SETTINGS_SETREGISTRY);
            }
        }

        private void navigationView_PaneClosing(NavigationView sender, object args)
        {
            if (args == null) { }
            if (sender != null) {
                SizeDouble sizeDouble;
                sizeDouble.Width = Window_OpenPaneLength;
                sizeDouble.Height = Window_CompactPaneLength;
                CalcWindowSizeByDPI(g_hSettingsWnd, ref sizeDouble);
                sender.OpenPaneLength = sizeDouble.Width;
            }
        }

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
            if (FlushMouseUI3DLL.Settings.g_Settings != null) {
                if (navigationView != null) {
                    if ((settingsMain.ActualWidth != 0) && (settingsMain.ActualHeight != 0)) {
                        navigationView.Width = settingsMain.ActualWidth;
                        navigationView.Height = settingsMain.ActualHeight;
                    }
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
                    UpdateProfile(SETTINGSEX_SETTINGS_SETREGISTRY);
                }
                else if (_args.DidPositionChange) {
                    m_WindowRectDouble.X = sender.Position.X;
                    m_WindowRectDouble.Y = sender.Position.Y;

                    dwSettingsX = (Int32)m_WindowRectDouble.X;
                    dwSettingsY = (Int32)m_WindowRectDouble.Y;
                    dwSettingsWidth = (Int32)m_WindowRectDouble.Width;
                    dwSettingsHeight = (Int32)m_WindowRectDouble.Height;
                    UpdateProfile(SETTINGSEX_SETTINGS_SETREGISTRY);
                }
                else if (_args.DidPresenterChange) {
                    SetWindowSize();
                }
            }
        }

        private void Window_Closed(object sender, WindowEventArgs args)
        {
            if (sender == null) { }
            if (args == null) { }
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
            	if (iRet == 0) return iRet;
            }
            Int64 _hWnd = FindWindowW(CLASS_FLUSHMOUSE, null);
            if ((_hWnd != 0) && (_hWnd != g_hMainWnd)) {
                SendMessageW(_hWnd, g_uMsg, SETTINGSEX_RELOAD_REGISTRY, 0);
                iRet = SendMessageW(_hWnd, g_uMsg, iSettingsEX, 0);
            }
            return iRet;
        }

        unsafe private static void CalcWindowCentralizeByDesktop(Int64 hWnd, ref RectDouble rectWindowDouble)
        {
            Int64   hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
            if (hMonitor != (Int64)0) {
                MONITORINFOEXW lpmi = new() { cbSize = (int)Marshal.SizeOf(typeof(MONITORINFOEXW)) };
                if (GetMonitorInfoW(hMonitor, lpmi)) {
                    SizeDouble sizeDouble;
                    sizeDouble.Width = rectWindowDouble.Width;
                    sizeDouble.Height = rectWindowDouble.Height;
                    CalcWindowSizeByDPI(hWnd, ref sizeDouble);
                    if (sizeDouble.Width >= (double)lpmi.rcWork.Width) {
                        rectWindowDouble.Width = lpmi.rcWork.Width;
                    }
                    else {
                        rectWindowDouble.Width = sizeDouble.Width;
                    }
                    if (sizeDouble.Height >= (double)lpmi.rcWork.Height) {
                        rectWindowDouble.Height = lpmi.rcWork.Height;
                    }
                    else {
                        rectWindowDouble.Height = sizeDouble.Height;
                    }
                    rectWindowDouble.X = (int)((double)lpmi.rcWork.X + ((double)lpmi.rcWork.Width - (double)lpmi.rcWork.X - rectWindowDouble.Width) / 2.0);
                    rectWindowDouble.Y = (int)((double)lpmi.rcWork.Y + ((double)lpmi.rcWork.Height - (double)lpmi.rcWork.Y - rectWindowDouble.Height) / 2.0);
                }
            }
        }

        unsafe private static bool CalcWindowAdjustByMonitor(Int64 hWnd, ref RectDouble rectWindowDouble)
        {
            Int64   hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
            if (hMonitor != (Int64)0) {
                MONITORINFOEXW lpmi = new() { cbSize = (int)Marshal.SizeOf(typeof(MONITORINFOEXW)) };
                if (GetMonitorInfoW(hMonitor, lpmi)) {
                    if ((rectWindowDouble.X < lpmi.rcWork.X) 
                            || (lpmi.rcWork.Width > (lpmi.rcWork.Width + rectWindowDouble.Width)) 
                            || (rectWindowDouble.Y < lpmi.rcWork.Y)
                            || (lpmi.rcWork.Height < (rectWindowDouble.Y + rectWindowDouble.Height))) {
                        rectWindowDouble.X = (int)((double)lpmi.rcWork.X + ((double)lpmi.rcWork.Width - (double)lpmi.rcWork.X - rectWindowDouble.Width) / 2.0);
                        rectWindowDouble.Y = (int)((double)lpmi.rcWork.Y + ((double)lpmi.rcWork.Height - (double)lpmi.rcWork.Y - rectWindowDouble.Height) / 2.0);
                        return true;
                    }
                }
            }
            return false;
        }
        
        unsafe private static void CalcWindowSizeByDPI(Int64 hWnd, ref SizeDouble sizeDouble)
        {
            UInt32 dpi = GetDpiForWindow(hWnd);
            if (dpi != 0) {
                sizeDouble.Width = ((double)sizeDouble.Width * (double)dpi / (double)USER_DEFAULT_SCREEN_DPI);
                sizeDouble.Height = ((double)sizeDouble.Height * (double)dpi / (double)USER_DEFAULT_SCREEN_DPI);
            }
        }

        public static long MessageBox(Int64 hWnd, string MessageBox, string lpCaption, UInt32 uType)
        {
            try {
                Task<long> ret = Task<long>.Run(() => MessageBoxW(hWnd, MessageBox, lpCaption, uType));
                return ret.Result;
            }
            catch (Exception) {
            }
            return 0;
        }
    }
}


/* = EOF = */