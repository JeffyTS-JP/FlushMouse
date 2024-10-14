//
// Miscs.xaml.cs
//      Copyright (C) 2023 JeffyTS
//
// No.      Date            Name            Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000    2024/10/22  JeffyTS     New edit.
//

using System;
using System.Runtime.InteropServices;

using Windows.Graphics;

using static FlushMouseUI3DLL.Settings;

#pragma warning disable IDE0079
namespace FlushMouseUI3DLL {
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

	public struct POINT
	{
		public long x;
		public long y; 
	}

#pragma warning disable IDE0251
	public struct RECT
	{
		public int Left, Top, Right, Bottom;
		public int Width { get { return Right - Left; } }
		public int Height { get { return Bottom - Top; } }
	}
#pragma warning restore IDE0251
		
	public partial class Settings
	{
		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 SendMessageW(Int64 hWnd, UInt32 uMsg, Int64 wParam, Int64 lParam);
		internal const UInt32 WM_DESTROY = (0x0002);
		
		internal const UInt32 WM_COPYDATA = (0x004A);
		[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto, Pack = 4)]
		internal class COPYDATASTRUCT {
			public UInt64 dwData;
			public UInt32 cbData;
			public UInt64 lpData;
		};

#pragma warning disable SYSLIB1054
		[DllImport("User32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		private static extern Int64 SendMessageW(Int64 hWnd, UInt32 uMsg, Int64 wParam, [In, Out] COPYDATASTRUCT CopyData);
#pragma warning restore SYSLIB1054

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
			//public fixed char szDevice[32];
			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
			//public char[]     szDevice;
			public char[] szDevice = new char[32];
		};
#pragma warning disable SYSLIB1054 
		[DllImport("User32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		internal static extern bool GetMonitorInfoW(Int64 hMonitor, [In, Out] MONITORINFOEXW lpmi);
#pragma warning restore SYSLIB1054
		
		[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto, Pack = 4)]
		internal class WINDOWPLACEMENT {
			public uint         length;
			public uint         flags;
			public uint         showCmd;
			public PointInt32   ptMinPosition;
			public PointInt32   ptMaxPosition;
			public RectInt32    rcNormalPosition;
			public RectInt32    rcDevice;
		};
#pragma warning disable SYSLIB1054
		[DllImport("User32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		private static extern bool GetWindowPlacement(Int64 hMonitor, [In, Out] WINDOWPLACEMENT lpwndpl);
#pragma warning restore SYSLIB1054

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 MessageBoxW(Int64 hWnd, string MessageBox, string lpCaption, UInt32 uType);
		internal const UInt32 MB_OK = 0x00000000;
		internal const UInt32 MB_ICONINFORMATION = 0x00000040;
		internal const UInt32 MB_TOPMOST = 0x00040000;

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 GetWindowLongPtrW(Int64 hWnd,  Int32 nIndex);
		internal const Int32 GWL_WNDPROC = (-4);
		internal const Int32 GWLP_HINSTANCE = (-6);
		internal const Int32 GWL_STYLE = (-16);
		internal const int HCBT_ACTIVATE = 5;

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 DefWindowProcW(Int64 hWnd,  UInt32 nIndex, Int64 wParam, Int64 lParam);

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 CallNextHookEx(Int64 hHook,  Int32 nCode, Int64 wParam, Int64 lParam);

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		internal static partial bool IsIconic(Int64 hWnd);

		[LibraryImport("Kernel32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I4)]
		internal static partial Int32 GetCurrentThreadId();

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 SetWindowsHookExW(Int64 idHook,  HookProc lpFn, Int64 hMod, Int32 dwThreadId);
		internal delegate Int64 HookProc(Int32 nCode, Int64 wParam, Int64 lParam);
		internal const Int32 WH_CBT = (5);

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		internal static partial bool UnhookWindowsHookEx(Int64 hHook);
		
		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		internal static partial bool GetWindowRect(Int64 hWnd, ref RECT lpRect);
		
		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		internal static partial bool SetWindowPos(Int64 hWnd, Int64 hWndInsertAfter, long x, long y, long cx, long cy, UInt32 uFlags);
		internal const Int64 HWND_TOPMOST		= (-1);
		internal const UInt32 SWP_NOSIZE		= 0x0001;
		internal const UInt32 SWP_NOZORDER		= 0x0004;
		internal const UInt32 SWP_NOACTIVATE	= 0x0010;
	}
	
	public sealed partial class Miscs
	{
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

		public static void CalcWindowCentralizeByDesktop(Int64 hWnd, RectDouble rectWindowDouble)
		{
			Int64   hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
			if (hMonitor != (Int64)0) {
				MONITORINFOEXW lpmi = new() { cbSize = (int)Marshal.SizeOf(typeof(MONITORINFOEXW)) };
				if (GetMonitorInfoW(hMonitor, lpmi)) {
					SizeDouble sizeDouble;
					sizeDouble.Width = rectWindowDouble.Width;
					sizeDouble.Height = rectWindowDouble.Height;
					CalcWindowSizeByDPI(hWnd, sizeDouble);
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
					rectWindowDouble.X = (int)(lpmi.rcWork.X + ((double)lpmi.rcWork.Width - (double)lpmi.rcWork.X - rectWindowDouble.Width) / 2.0);
					rectWindowDouble.Y = (int)(lpmi.rcWork.Y + ((double)lpmi.rcWork.Height - (double)lpmi.rcWork.Y - rectWindowDouble.Height) / 2.0);
				}
			}
		}

		public static bool CalcWindowAdjustByMonitor(Int64 hWnd, RectDouble rectWindowDouble)
		{
			Int64   hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
			if (hMonitor != (Int64)0) {
				MONITORINFOEXW lpmi = new() { cbSize = (int)Marshal.SizeOf(typeof(MONITORINFOEXW)) };
				if (GetMonitorInfoW(hMonitor, lpmi)) {
					if ((rectWindowDouble.X < lpmi.rcWork.X) 
							|| (lpmi.rcWork.Width > (lpmi.rcWork.Width + rectWindowDouble.Width)) 
							|| (rectWindowDouble.Y < lpmi.rcWork.Y)
							|| (lpmi.rcWork.Height < (rectWindowDouble.Y + rectWindowDouble.Height))) {
						rectWindowDouble.X = (int)(lpmi.rcWork.X + ((double)lpmi.rcWork.Width - (double)lpmi.rcWork.X - rectWindowDouble.Width) / 2.0);
						rectWindowDouble.Y = (int)(lpmi.rcWork.Y + ((double)lpmi.rcWork.Height - (double)lpmi.rcWork.Y - rectWindowDouble.Height) / 2.0);
						return true;
					}
				}
			}
			return false;
		}
		
		public static void CalcWindowSizeByDPI(Int64 hWnd, SizeDouble sizeDouble)
		{
			UInt32 dpi = GetDpiForWindow(hWnd);
			if (dpi != 0) {
				sizeDouble.Width = ((double)sizeDouble.Width * (double)dpi / (double)USER_DEFAULT_SCREEN_DPI);
				sizeDouble.Height = ((double)sizeDouble.Height * (double)dpi / (double)USER_DEFAULT_SCREEN_DPI);
			}
		}


		private static Int64 hParentWnd = 0;
		private static Int64 hMessageBoxHook;
		public static long MessageBox(Int64 hWnd, string MessageBox, string lpCaption, UInt32 uType)
		{
			long result = 0;
			try {
				Int64 hInstance = GetWindowLongPtrW(hWnd, GWLP_HINSTANCE);
				Int32 dwThreadID = GetCurrentThreadId();
				hParentWnd = hWnd;
				hMessageBoxHook = SetWindowsHookExW(WH_CBT, new(_HookProc), hInstance, dwThreadID);
				result = MessageBoxW(hWnd, MessageBox, lpCaption, uType);
			}
			catch (Exception ) {
			}
			return result;
		}

		private static Int64 _HookProc(Int32 nCode, Int64 wParam, Int64 lParam)
		{
			if (nCode == HCBT_ACTIVATE) {
				RECT rcParent = new();
				RECT rcMsgBox = new();
				GetWindowRect(hParentWnd, ref rcParent);
				GetWindowRect(wParam, ref rcMsgBox);

				Int32 x = rcParent.Left + ((rcParent.Width - rcMsgBox.Width) / 2);
				Int32 y = rcParent.Top + ((rcParent.Height - rcMsgBox.Height) / 2);
				SetWindowPos(wParam, HWND_TOPMOST, x, y, rcMsgBox.Width, rcMsgBox.Height, (SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE));

				UnhookWindowsHookEx(hMessageBoxHook);
				hMessageBoxHook = (Int64)0;
		}
			return CallNextHookEx(hMessageBoxHook, nCode, wParam, lParam);
		}

	}
}
#pragma warning restore IDE0079


/* = EOF = */