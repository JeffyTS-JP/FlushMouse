//
// Miscs.xaml.cs
//      Copyright (C) 2023 JeffyTS
//
// No.      Date            Name            Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000    2024/10/22  JeffyTS     New edit.
//

using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

using Windows.Graphics;

#pragma warning disable IDE0079
namespace FlushMouseUI3DLL {
	public struct RectDouble {
		public double X, Y, Width, Height;
	}

	public struct SizeDouble {
		public double Width, Height;
	}

	public struct POINT {
		public long x, y;
	}

#pragma warning disable IDE0251
	public struct RECT {
		public int Left, Top, Right, Bottom;
	}
#pragma warning restore IDE0251

	public partial class Miscs
	{
		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.U4)]
		internal static partial UInt32 SetProcessDPIAware();

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.U4)]
		internal static partial UInt32 SetProcessDpiAwarenessContext(Int32 value);

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.U4)]
		internal static partial UInt32 SetThreadDpiAwarenessContext(Int32 value);
		internal const Int32 DPI_AWARENESS_CONTEXT_UNAWARE = (-1);
		internal const Int32 DPI_AWARENESS_CONTEXT_SYSTEM_AWARE = (-2);
		internal const Int32 DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE = (-3);
		internal const Int32 DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 = (-4);
		internal const Int32 DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED = (-5);

		[LibraryImport("Kernel32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 LoadLibraryW(String lpLibFileName);

		[LibraryImport("Kernel32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		internal static partial bool FreeLibrary(Int64 hLibModule);

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
		internal const UInt32 MONITOR_DEFAULTTONULL = 0x00000000;
		internal const UInt32 MONITOR_DEFAULTTOPRIMARY = 0x00000001;
		internal const UInt32 MONITOR_DEFAULTTONEAREST = 0x00000002;

		[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto, Pack = 4)]
		internal class MONITORINFOEXW {
			public int cbSize = Marshal.SizeOf(typeof(MONITORINFOEXW));
			public RectInt32 rcMonitor = new();
			public RectInt32 rcWork = new();
			public int dwFlags = 0;
			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
			public char[] szDevice = new char[32];
		};
#pragma warning disable SYSLIB1054
		[DllImport("User32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		internal static extern bool GetMonitorInfoW(Int64 hMonitor, [In, Out] MONITORINFOEXW lpmi);
#pragma warning restore SYSLIB1054

		[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto, Pack = 4)]
		internal class WINDOWPLACEMENT {
			public uint length;
			public uint flags;
			public uint showCmd;
			public PointInt32 ptMinPosition;
			public PointInt32 ptMaxPosition;
			public RectInt32 rcNormalPosition;
			public RectInt32 rcDevice;
		};
#pragma warning disable SYSLIB1054
		[DllImport("User32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		private static extern bool GetWindowPlacement(Int64 hMonitor, [In, Out] WINDOWPLACEMENT lpwndpl);
#pragma warning restore SYSLIB1054

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 MessageBoxW(Int64 hWnd, string MessageBox, string lpCaption, UInt32 uType);
		internal const UInt32 MB_OK = 0x00000000;
		internal const UInt32 MB_ICONSTOP= 0x00000010;
		internal const UInt32 MB_ICONINFORMATION = 0x00000040;
		internal const UInt32 MB_TOPMOST = 0x00040000;

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 GetWindowLongPtrW(Int64 hWnd, Int32 nIndex);
		internal const Int32 GWL_WNDPROC = (-4);
		internal const Int32 GWLP_HINSTANCE = (-6);
		internal const Int32 GWL_STYLE = (-16);
		internal const int HCBT_ACTIVATE = 5;

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 DefWindowProcW(Int64 hWnd, UInt32 nIndex, Int64 wParam, Int64 lParam);

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 CallNextHookEx(Int64 hHook, Int32 nCode, Int64 wParam, Int64 lParam);

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		internal static partial bool IsIconic(Int64 hWnd);

		[LibraryImport("Kernel32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I4)]
		internal static partial Int32 GetCurrentThreadId();

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 SetWindowsHookExW(Int64 idHook, HookProc lpFn, Int64 hMod, Int32 dwThreadId);
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
		internal const Int64 HWND_TOPMOST = (-1);
		internal const UInt32 SWP_NOSIZE = 0x0001;
		internal const UInt32 SWP_NOZORDER = 0x0004;
		internal const UInt32 SWP_NOACTIVATE = 0x0010;

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		internal static partial bool MessageBeep(UInt32 uType);
		internal const UInt32 MB_ICONWARNING = 0x00000030;

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = false)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 SetCursor(Int64 hCursor);

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		internal static partial bool SetSystemCursor(Int64 hCursor, UInt32 id);
		internal const UInt32 OCR_NORMAL		=  32512;
		internal const UInt32 OCR_IBEAM			=  32513;
		internal const UInt32 OCR_WAIT			=  32514;
		internal const UInt32 OCR_HAND			=  32649;
		internal const UInt32 OCR_APPSTARTING	=  32650;
		
		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 LoadCursorW(Int64 hInstance, Int64 lpCursorName);
		internal const UInt32 IDC_ARROW			=  32512;
		internal const UInt32 IDC_IBEAM			=  32513;
		internal const UInt32 IDC_WAIT			=  32514;
		internal const UInt32 IDC_HAND			=  32649;
		internal const UInt32 IDC_APPSTARTING	=  32650;

		[LibraryImport("User32.dll", StringMarshalling = StringMarshalling.Utf16, SetLastError = true)]
		[return: MarshalAs(UnmanagedType.I8)]
		internal static partial Int64 CopyIcon(Int64 hIcon);
	}

	public sealed partial class Miscs {
		public static void CalcWindowCentralizeByDesktop(Int64 hWnd, RectDouble InRectWindowDouble, out RectDouble OutRectWindowDouble)
		{
			OutRectWindowDouble = InRectWindowDouble;
			Int64 hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
			if (hMonitor != (Int64)0)
			{
				MONITORINFOEXW lpmi = new();
				if (GetMonitorInfoW(hMonitor, lpmi))
				{
					SizeDouble sizeDouble;
					sizeDouble.Width = OutRectWindowDouble.Width;
					sizeDouble.Height = OutRectWindowDouble.Height;
					UInt32 dpi = GetDpiForWindow(hWnd);
					if (dpi != 0)
					{
						sizeDouble.Width = ((double)sizeDouble.Width * (double)dpi / (double)USER_DEFAULT_SCREEN_DPI);
						sizeDouble.Height = ((double)sizeDouble.Height * (double)dpi / (double)USER_DEFAULT_SCREEN_DPI);
					}
					if (sizeDouble.Width >= (double)lpmi.rcWork.Width)
					{
						OutRectWindowDouble.Width = lpmi.rcWork.Width;
					}
					else
					{
						OutRectWindowDouble.Width = sizeDouble.Width;
					}
					if (sizeDouble.Height >= (double)lpmi.rcWork.Height)
					{
						OutRectWindowDouble.Height = (double)lpmi.rcWork.Height;
					}
					else
					{
						OutRectWindowDouble.Height = sizeDouble.Height;
					}
					OutRectWindowDouble.X = (int)(lpmi.rcWork.X + ((double)lpmi.rcWork.Width - (double)lpmi.rcWork.X - OutRectWindowDouble.Width) / 2.0);
					OutRectWindowDouble.Y = (int)(lpmi.rcWork.Y + ((double)lpmi.rcWork.Height - (double)lpmi.rcWork.Y - OutRectWindowDouble.Height) / 2.0);
				}
			}
		}

		public static bool CalcWindowAdjustByMonitor(Int64 hWnd, RectDouble InRectWindowDouble, out RectDouble OutRectWindowDouble)
		{
			OutRectWindowDouble = InRectWindowDouble;
			Int64 hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
			if (hMonitor != (Int64)0)
			{
				MONITORINFOEXW lpmi = new();
				if (GetMonitorInfoW(hMonitor, lpmi))
				{
					if ((OutRectWindowDouble.X < lpmi.rcWork.X)
							|| (lpmi.rcWork.Width > (lpmi.rcWork.Width + OutRectWindowDouble.Width))
							|| (OutRectWindowDouble.Y < lpmi.rcWork.Y)
							|| (lpmi.rcWork.Height < (OutRectWindowDouble.Y + OutRectWindowDouble.Height)))
					{
						OutRectWindowDouble.X = (int)(lpmi.rcWork.X + ((double)lpmi.rcWork.Width - (double)lpmi.rcWork.X - OutRectWindowDouble.Width) / 2.0);
						OutRectWindowDouble.Y = (int)(lpmi.rcWork.Y + ((double)lpmi.rcWork.Height - (double)lpmi.rcWork.Y - OutRectWindowDouble.Height) / 2.0);
						return true;
					}
				}
			}
			return false;
		}

		public static void CalcWindowSizeByDPI(Int64 hWnd, SizeDouble InSizeDouble, out SizeDouble OutSizeDouble)
		{
			OutSizeDouble = InSizeDouble;
			UInt32 dpi = GetDpiForWindow(hWnd);
			if (dpi != 0)
			{
				OutSizeDouble.Width = ((double)OutSizeDouble.Width * (double)dpi / (double)USER_DEFAULT_SCREEN_DPI);
				OutSizeDouble.Height = ((double)OutSizeDouble.Height * (double)dpi / (double)USER_DEFAULT_SCREEN_DPI);
			}
		}

		public static async void MessageBox(Int64 hWnd, String lpText, String lpCaption, UInt32 uType)
		{
			try {
				Task<long> messageBox = Task<long>.Run(() => _MessageBox(hWnd, lpText, lpCaption, uType));
				long _ = await messageBox;
			}
			catch (Exception) {
				Debug.WriteLine("Message :");
			}
		}

		private static Int64 hParentWnd = 0;
		private static Int64 hMessageBoxHook;
		private static long _MessageBox(Int64 hWnd, string lpText, string lpCaption, UInt32 uType)
		{
			long result = 0;
			try
			{
				Int64 hInstance = GetWindowLongPtrW(hWnd, GWLP_HINSTANCE);
				Int32 dwThreadID = GetCurrentThreadId();
				hParentWnd = hWnd;
				hMessageBoxHook = SetWindowsHookExW(WH_CBT, new(_HookProc), hInstance, dwThreadID);
				result = MessageBoxW(hWnd, lpText, lpCaption, uType);
			}
			catch (Exception)
			{
			}
			return result;
		}

		private static Int64 _HookProc(Int32 nCode, Int64 wParam, Int64 lParam)
		{
			if (nCode == HCBT_ACTIVATE)
			{
				RECT rcParent = new();
				RECT rcMsgBox = new();
				GetWindowRect(hParentWnd, ref rcParent);
				GetWindowRect(wParam, ref rcMsgBox);

				Int32 x = rcParent.Left + (((rcParent.Right - rcParent.Left) - (rcMsgBox.Right - rcMsgBox.Left)) / 2);
				Int32 y = rcParent.Top + (((rcParent.Bottom - rcParent.Top) - (rcMsgBox.Bottom - rcMsgBox.Top)) / 2);
				SetWindowPos(wParam, HWND_TOPMOST, x, y, (rcMsgBox.Right - rcMsgBox.Left), (rcMsgBox.Bottom - rcMsgBox.Top), (SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE));

				UnhookWindowsHookEx(hMessageBoxHook);
				hMessageBoxHook = (Int64)0;
			}
			return CallNextHookEx(hMessageBoxHook, nCode, wParam, lParam);
		}

#pragma warning disable SYSLIB1045
		public static bool CheckNumeric(String text, Int64 Min, Int64 Max)
		{
			if (text == null) return false;
			Regex regex = new("^[0-9]*$");
			Int64 i;
			if (text != null)
			{
				if (regex.IsMatch(text))
				{
					i = Convert.ToInt64(text);
					if ((Min <= i) && (i <= Max)) return true;
				}
			}
			return false;
		}

		public static bool CheckHexNumeric(String text, Int64 Min, Int64 Max)
		{
			if (text == null) return false;
			Regex regex = new("^[0-9a-fA-F]*$");
			Int64 i;
			if (text != null)
			{
				if (regex.IsMatch(text))
				{
					i = Convert.ToInt64(text, 16);
					if ((Min <= i) && (i <= Max)) return true;
				}
			}
			return false;
		}

		public static bool CheckBinaryNumeric(String text, Int64 Min, Int64 Max)
		{
			if (text == null) return false;
			Regex regex = new("^[01]*$");
			Int64 i;
			if (text != null)
			{
				if (regex.IsMatch(text))
				{
					i = Convert.ToInt64(text, 2);
					if ((Min <= i) && (i <= Max)) return true;
				}
			}
			return false;
		}
#pragma warning restore SYSLIB1045

		public static bool CheckHostname(String text)
		{
			if (Uri.CheckHostName(text) != UriHostNameType.Unknown) return true;
			return false;
		}
	}
}
#pragma warning restore IDE0079


/* = EOF = */