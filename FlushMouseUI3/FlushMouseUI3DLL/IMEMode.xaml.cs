// IMEMode.Xaml.cs for FlushMouseUI3DLL
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
	public sealed partial class IMEMode
	{
		private static bool m_Sentinel {  get; set; }
	}

	public sealed partial class IMEMode : Page
	{

		public IMEMode()
		{
			m_Sentinel = false;
			InitializeComponent();
		}

		private void EnableDisableItems(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
			if (Button1_1 != null) {
				if ((!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp) || !bDisplayIMEModeIMEOFF) Button1_1.IsEnabled = false;
				else Button1_1.IsEnabled = true;
			}
			if (Button1_2 != null) {
				if (!bDrawNearCaret) Button1_2.IsEnabled = false;
				else Button1_2.IsEnabled = true;
			}
			if (Button1_3 != null) {
				if (!bDisplayIMEModeOnCursor || !bDisplayIMEModeByWindow || !bDisplayIMEModeIMEOFF) Button1_3.IsEnabled = false;
				else Button1_3.IsEnabled = true;
			}
			if (Button2_1 != null) {
				if (!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp) Button2_1.IsEnabled = false;
				else Button2_1.IsEnabled = true;
			}
			if (Button2_2 != null) {
				if (!bDrawNearCaret) Button2_2.IsEnabled = false;
				else Button2_2.IsEnabled = true;
			}
			if (Button2_3 != null) 	{
				if (!bDisplayIMEModeOnCursor || !bDisplayIMEModeByWindow) Button2_3.IsEnabled = false;
				else Button2_3.IsEnabled = true;
			}
			if (Button3_1 != null) {
				if (!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp) Button3_1.IsEnabled = false;
				else Button3_1.IsEnabled = true;
			}
			if (Button3_2 != null) {
				if (!bDrawNearCaret) Button3_2.IsEnabled = false;
				else Button3_2.IsEnabled = true;
			}
			if (Button3_3 != null) 	{
				if (!bDisplayIMEModeOnCursor || !bDisplayIMEModeByWindow) Button3_3.IsEnabled = false;
				else Button3_3.IsEnabled = true;
			}
			if (Button4_1 != null) {
				if (!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp) Button4_1.IsEnabled = false;
				else Button4_1.IsEnabled = true;
			}
			if (Button4_2 != null) {
				if (!bDrawNearCaret) Button4_2.IsEnabled = false;
				else Button4_2.IsEnabled = true;
			}
			if (Button4_3 != null) 	{
				if (!bDisplayIMEModeOnCursor || !bDisplayIMEModeByWindow) Button4_3.IsEnabled = false;
				else Button4_3.IsEnabled = true;
			}
			if (Button5_1 != null) {
				if (!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp) Button5_1.IsEnabled = false;
				else Button5_1.IsEnabled = true;
			}
			if (Button5_2 != null) {
				if (!bDrawNearCaret) Button5_2.IsEnabled = false;
				else Button5_2.IsEnabled = true;
			}
			if (Button5_3 != null) 	{
				if (!bDisplayIMEModeOnCursor || !bDisplayIMEModeByWindow) Button5_3.IsEnabled = false;
				else Button5_3.IsEnabled = true;
			}
			if (Button6_1 != null) {
				if (!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp)
				Button6_1.IsEnabled = false;
				else Button6_1.IsEnabled = true;
			}
			if (Button6_2 != null) {
				if (!bDrawNearCaret) Button6_2.IsEnabled = false;
				else Button6_2.IsEnabled = true;
			}
			if (Button6_3 != null) 	{
				if (!bDisplayIMEModeOnCursor || !bDisplayIMEModeByWindow) Button6_3.IsEnabled = false;
				else Button6_3.IsEnabled = true;
			}
		}

		private void IMEMode_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }

			SetColorButton();
			m_Sentinel = true;
			EnableDisableItems(sender, e);
		}

		// Color Button
		private void SetColorButton()
		{
			if (Button1_1 == null)    Button1_1 = new Button();
			Color clr = Color.FromArgb(dwNearDrawMouseIMEOFFColor);
			Button1_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button1_2 == null)    Button1_2 = new Button();
			clr = Color.FromArgb(dwNearDrawCaretIMEOFFColor);
			Button1_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button1_3 == null)    Button1_3 = new Button();
			clr = Color.FromArgb(dwNearDrawMouseByWndIMEOFFColor);
			Button1_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button2_1 == null)    Button2_1 = new Button();
			clr = Color.FromArgb(dwNearDrawMouseZENHIRA_IMEONColor);
			Button2_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button2_2 == null)    Button2_2 = new Button();
			clr = Color.FromArgb(dwNearDrawCaretZENHIRA_IMEONColor);
			Button2_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button2_3 == null)    Button2_3 = new Button();
			clr = Color.FromArgb(dwNearDrawMouseByWndZENHIRA_IMEONColor);
			Button2_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button3_1 == null)    Button3_1 = new Button();
			clr = Color.FromArgb(dwNearDrawMouseZENKANA_IMEONColor);
			Button3_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button3_2 == null)    Button3_2 = new Button();
			clr = Color.FromArgb(dwNearDrawCaretZENKANA_IMEONColor);
			Button3_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button3_3 == null)    Button3_3 = new Button();
			clr = Color.FromArgb(dwNearDrawMouseByWndZENKANA_IMEONColor);
			Button3_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button4_1 == null)    Button4_1 = new Button();
			clr = Color.FromArgb(dwNearDrawMouseZENEISU_IMEONColor);
			Button4_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button4_2 == null)    Button4_2 = new Button();
			clr = Color.FromArgb(dwNearDrawCaretZENEISU_IMEONColor);
			Button4_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button4_3 == null)    Button4_3 = new Button();
			clr = Color.FromArgb(dwNearDrawMouseByWndZENEISU_IMEONColor);
			Button4_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button5_1 == null)    Button5_1 = new Button();
			clr = Color.FromArgb(dwNearDrawMouseHANKANA_IMEONColor);
			Button5_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button5_2 == null)    Button5_2 = new Button();
			clr = Color.FromArgb(dwNearDrawCaretHANKANA_IMEONColor);
			Button5_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button5_3 == null)    Button5_3 = new Button();
			clr = Color.FromArgb(dwNearDrawMouseByWndHANKANA_IMEONColor);
			Button5_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button6_1 == null)    Button6_1 = new Button();
			clr = Color.FromArgb(dwNearDrawMouseHANEISU_IMEONColor);
			Button6_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button6_2 == null)    Button6_2 = new Button();
			clr = Color.FromArgb(dwNearDrawCaretHANEISU_IMEONColor);
			Button6_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Button6_3 == null)    Button6_3 = new Button();
			clr = Color.FromArgb(dwNearDrawMouseByWndHANEISU_IMEONColor);
			Button6_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
		}

		unsafe private void ColorButton_Click(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			if (e == null) { }
			Button btn = sender as Button;
			if (btn != null) {
				if (btn.Name == "Button1_1") {
					Color clr = Color.FromArgb(dwNearDrawMouseIMEOFFColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker1_1.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker1_1.PreviousColor = ColorPicker1_1.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button1_2") {
					Color clr = Color.FromArgb(dwNearDrawCaretIMEOFFColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker1_2.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker1_2.PreviousColor = ColorPicker1_2.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button1_3") {
					Color clr = Color.FromArgb(dwNearDrawMouseByWndIMEOFFColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker1_3.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker1_3.PreviousColor = ColorPicker1_3.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button2_1") {
					Color clr = Color.FromArgb(dwNearDrawMouseZENHIRA_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker2_1.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker2_1.PreviousColor = ColorPicker2_1.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button2_2") {
					Color clr = Color.FromArgb(dwNearDrawCaretZENHIRA_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker2_2.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker2_2.PreviousColor = ColorPicker2_2.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button2_3") {
					Color clr = Color.FromArgb(dwNearDrawMouseByWndZENHIRA_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker2_3.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker2_3.PreviousColor = ColorPicker2_3.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button3_1") {
					Color clr = Color.FromArgb(dwNearDrawMouseHANKANA_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker3_1.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker3_1.PreviousColor = ColorPicker3_1.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button3_2") {
					Color clr = Color.FromArgb(dwNearDrawCaretHANKANA_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker3_2.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker3_2.PreviousColor = ColorPicker3_2.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button3_3") {
					Color clr = Color.FromArgb(dwNearDrawMouseByWndHANKANA_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker3_3.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker3_3.PreviousColor = ColorPicker3_3.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button4_1") {
					Color clr = Color.FromArgb(dwNearDrawMouseZENEISU_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker4_1.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker4_1.PreviousColor = ColorPicker4_1.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button4_2") {
					Color clr = Color.FromArgb(dwNearDrawCaretZENEISU_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker4_2.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker4_2.PreviousColor = ColorPicker4_2.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button4_3") {
					Color clr = Color.FromArgb(dwNearDrawMouseByWndZENEISU_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker4_3.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker4_3.PreviousColor = ColorPicker4_3.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button5_1") {
					Color clr = Color.FromArgb(dwNearDrawMouseHANKANA_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker5_1.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker5_1.PreviousColor = ColorPicker5_1.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button5_2") {
					Color clr = Color.FromArgb(dwNearDrawCaretHANKANA_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker5_2.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker5_2.PreviousColor = ColorPicker5_2.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button5_3") {
					Color clr = Color.FromArgb(dwNearDrawMouseByWndHANKANA_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker5_3.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker5_3.PreviousColor = ColorPicker5_3.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button6_1") {
					Color clr = Color.FromArgb(dwNearDrawMouseHANEISU_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker6_1.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker6_1.PreviousColor = ColorPicker6_1.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button6_2") {
					Color clr = Color.FromArgb(dwNearDrawCaretHANEISU_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker6_2.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker6_2.PreviousColor = ColorPicker6_2.Color;
					FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
				}
				else if (btn.Name == "Button6_3") {
					Color clr = Color.FromArgb(dwNearDrawMouseByWndHANEISU_IMEONColor);
					byte A = (byte)(255 - clr.A);
					byte R = (byte)(clr.B);
					byte G = (byte)(clr.G);
					byte B = (byte)(clr.R);
					ColorPicker6_3.Color = Windows.UI.Color.FromArgb(A, R, G, B);
					ColorPicker6_3.PreviousColor = ColorPicker6_3.Color;
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
				if (btn.Name == "CP_Button1_1_1") {
					Windows.UI.Color wclr = ColorPicker1_1.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseIMEOFFColor;
					dwNearDrawMouseIMEOFFColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0);
					Flyout1_1.Hide();
					if (i != dwNearDrawMouseIMEOFFColor) {
						Color clr = Color.FromArgb(dwNearDrawMouseIMEOFFColor);
						Button1_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button1_1_2") {
					Flyout1_1.Hide();
				}
				else if (btn.Name == "CP_Button1_2_1") {
					Windows.UI.Color wclr = ColorPicker1_2.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawCaretIMEOFFColor;
					dwNearDrawCaretIMEOFFColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0);
					Flyout1_2.Hide();
					if (i != dwNearDrawCaretIMEOFFColor) {
						Color clr = Color.FromArgb(dwNearDrawCaretIMEOFFColor);
						Button1_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button1_2_2") {
					Flyout1_2.Hide();
				}
				else if (btn.Name == "CP_Button1_3_1") {
					Windows.UI.Color wclr = ColorPicker1_3.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseByWndIMEOFFColor;
					dwNearDrawMouseByWndIMEOFFColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0);
					Flyout1_3.Hide();
					if (i != dwNearDrawMouseByWndIMEOFFColor)
					{
						Color clr = Color.FromArgb(dwNearDrawMouseByWndIMEOFFColor);
						Button1_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button1_3_2") {
					Flyout1_3.Hide();
				}
				else if (btn.Name == "CP_Button2_1_1") {
					Windows.UI.Color wclr = ColorPicker2_1.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseZENHIRA_IMEONColor;
					dwNearDrawMouseZENHIRA_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout2_1.Hide();
					if (i != dwNearDrawMouseZENHIRA_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawMouseZENHIRA_IMEONColor);
						Button2_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button2_1_2") {
					Flyout2_1.Hide();
				}
				else if (btn.Name == "CP_Button2_2_1") {
					Windows.UI.Color wclr = ColorPicker2_2.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawCaretZENHIRA_IMEONColor;
					dwNearDrawCaretZENHIRA_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout2_2.Hide();
					if (i != dwNearDrawCaretZENHIRA_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawCaretZENHIRA_IMEONColor);
						Button2_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button2_2_2") {
					Flyout2_2.Hide();
				}
				else if (btn.Name == "CP_Button2_3_1") {
					Windows.UI.Color wclr = ColorPicker2_3.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseByWndZENHIRA_IMEONColor;
					dwNearDrawMouseByWndZENHIRA_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout2_3.Hide();
					if (i != dwNearDrawMouseByWndZENHIRA_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawMouseByWndZENHIRA_IMEONColor);
						Button2_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button2_3_2") {
					Flyout2_3.Hide();
				}
				else if (btn.Name == "CP_Button3_1_1") {
					Windows.UI.Color wclr = ColorPicker3_1.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseZENKANA_IMEONColor;
					dwNearDrawMouseZENKANA_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout3_1.Hide();
					if (i != dwNearDrawMouseZENKANA_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawMouseZENKANA_IMEONColor);
						Button3_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button3_1_2") {
					Flyout3_1.Hide();
				}
				else if (btn.Name == "CP_Button3_2_1") {
					Windows.UI.Color wclr = ColorPicker3_2.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawCaretZENKANA_IMEONColor;
					dwNearDrawCaretZENKANA_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout3_2.Hide();
					if (i != dwNearDrawCaretZENKANA_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawCaretZENKANA_IMEONColor);
						Button3_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button3_2_2") {
					Flyout3_2.Hide();
				}
				else if (btn.Name == "CP_Button3_3_1") {
					Windows.UI.Color wclr = ColorPicker3_3.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseByWndZENKANA_IMEONColor;
					dwNearDrawMouseByWndZENKANA_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout3_3.Hide();
					if (i != dwNearDrawMouseByWndZENKANA_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawMouseByWndZENKANA_IMEONColor);
						Button3_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button3_3_2") {
					Flyout3_3.Hide();
				}
				else if (btn.Name == "CP_Button4_1_1") {
					Windows.UI.Color wclr = ColorPicker4_1.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseZENEISU_IMEONColor;
					dwNearDrawMouseZENEISU_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout4_1.Hide();
					if (i != dwNearDrawMouseZENEISU_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawMouseZENEISU_IMEONColor);
						Button4_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button4_1_2") {
					Flyout4_1.Hide();
				}
				else if (btn.Name == "CP_Button4_2_1") {
					Windows.UI.Color wclr = ColorPicker4_2.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawCaretZENEISU_IMEONColor;
					dwNearDrawCaretZENEISU_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout4_2.Hide();
					if (i != dwNearDrawCaretZENEISU_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawCaretZENEISU_IMEONColor);
						Button4_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button4_2_2") {
					Flyout4_2.Hide();
				}
				else if (btn.Name == "CP_Button4_3_1") {
					Windows.UI.Color wclr = ColorPicker4_3.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseByWndZENEISU_IMEONColor;
					dwNearDrawMouseByWndZENEISU_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout4_3.Hide();
					if (i != dwNearDrawMouseByWndZENEISU_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawMouseByWndZENEISU_IMEONColor);
						Button4_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button4_3_2") {
					Flyout4_3.Hide();
				}
				else if (btn.Name == "CP_Button5_1_1") {
					Windows.UI.Color wclr = ColorPicker5_1.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseHANKANA_IMEONColor;
					dwNearDrawMouseHANKANA_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout5_1.Hide();
					if (i != dwNearDrawMouseHANKANA_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawMouseHANKANA_IMEONColor);
						Button5_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button5_1_2") {
					Flyout5_1.Hide();
				}
				else if (btn.Name == "CP_Button5_2_1") {
					Windows.UI.Color wclr = ColorPicker5_2.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawCaretHANKANA_IMEONColor;
					dwNearDrawCaretHANKANA_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout5_2.Hide();
					if (i != dwNearDrawCaretHANKANA_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawCaretHANKANA_IMEONColor);
						Button5_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button5_2_2") {
					Flyout5_2.Hide();
				}
				else if (btn.Name == "CP_Button5_3_1") {
					Windows.UI.Color wclr = ColorPicker5_3.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseByWndHANKANA_IMEONColor;
					dwNearDrawMouseByWndHANKANA_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout5_3.Hide();
					if (i != dwNearDrawMouseByWndHANKANA_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawMouseByWndHANKANA_IMEONColor);
						Button5_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button5_3_2") {
					Flyout5_3.Hide();
				}
				else if (btn.Name == "CP_Button6_1_1") {
					Windows.UI.Color wclr = ColorPicker6_1.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseHANEISU_IMEONColor;
					dwNearDrawMouseHANEISU_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout6_1.Hide();
					if (i != dwNearDrawMouseHANEISU_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawMouseHANEISU_IMEONColor);
						Button6_1.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button6_1_2") {
					Flyout6_1.Hide();
				}
				else if (btn.Name == "CP_Button6_2_1") {
					Windows.UI.Color wclr = ColorPicker6_2.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawCaretHANEISU_IMEONColor;
					dwNearDrawCaretHANEISU_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout6_2.Hide();
					if (i != dwNearDrawCaretHANEISU_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawCaretHANEISU_IMEONColor);
						Button6_2.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button6_2_2") {
					Flyout6_2.Hide();
				}
				else if (btn.Name == "CP_Button6_3_1") {
					Windows.UI.Color wclr = ColorPicker6_3.Color;
					byte A = (byte)(255 - wclr.A);
					byte R = wclr.B;
					byte G = wclr.G;
					byte B = wclr.R;
					Int32 i = dwNearDrawMouseByWndHANEISU_IMEONColor;
					dwNearDrawMouseByWndHANEISU_IMEONColor = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0); 
					Flyout6_3.Hide();
					if (i != dwNearDrawMouseByWndHANEISU_IMEONColor) {
						Color   clr = Color.FromArgb(dwNearDrawMouseByWndHANEISU_IMEONColor);
						Button6_3.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
						EnableDisableItems(sender, e);
						UpdateProfile(SETTINGSEX_RELOAD_CURSOR);
					}
				}
				else if (btn.Name == "CP_Button6_3_2") {
					Flyout6_3.Hide();
				}
			}
		}
	}
}


/* = EOF = */