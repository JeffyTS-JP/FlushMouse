// SynTP_Helper.Xaml.cs for FlushMouseUI3DLL
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
using Microsoft.UI.Xaml.Data;
using Microsoft.UI.Xaml.Input;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Navigation;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;

using System.Net;

using static FlushMouseUI3DLL.Settings;
using Windows.System;
using System.ComponentModel.Design;


namespace FlushMouseUI3DLL {
	public sealed partial class SynTP_Helper
	{
		private static bool m_Sentinel {  get; set; }
		   
		internal const Int32 SYNTPH_DISABLE = 0x00;
		internal const Int32 SYNTPH_SENDERIPV4 = 0x01;
		internal const Int32 SYNTPH_SENDERHOSNAMEIPV4 = 0x11;
		internal const Int32 SYNTPH_SENDERHOSNAMEIPV6 = 0x31;
		internal const Int32 SYNTPH_SENDERIPV4_START = 0x02;
		internal const Int32 SYNTPH_SENDERHOSNAMEIPV4_START = 0x12;
		internal const Int32 SYNTPH_SENDERHOSNAMEIPV6_START = 0x32;
		internal const Int32 SYNTPH_RECEIVERIPV4 = 0x03;
		internal const Int32 SYNTPH_RECEIVERIPV6 = 0x33;
		internal const Int32 SYNTPH_RECEIVERIPV4_START = 0x04;
		internal const Int32 SYNTPH_RECEIVERIPV6_START = 0x34;
	}

	public sealed partial class SynTP_Helper : Page
	{
		public SynTP_Helper()
		{
			m_Sentinel = false;
			InitializeComponent();
		}

		private void EnableDisableItems(object sender, RoutedEventArgs e)
		{
			if (e == null) { }
			if (sender != null) {
				switch (dwSynTPHelper1) {
					case SYNTPH_DISABLE:
						if (rb1 != null)    rb1.IsEnabled = true;
						if (rb2 != null)    rb2.IsEnabled = true;
						if (rb3 != null)    rb3.IsEnabled = true;
						if (rb4 != null)    rb4.IsEnabled = false;
						if (rb5 != null)    rb5.IsEnabled = false;

						if (nb1 != null)    nb1.IsEnabled = false;
						if (nb2 != null)    nb2.IsEnabled = false;
						if (nb3 != null)    nb3.IsEnabled = false;
						if (nb4 != null)    nb4.IsEnabled = false;
						if (nb5_1 != null)  nb5_1.IsEnabled = false;
						if (nb5_2 != null)  nb5_2.IsEnabled = false;
						if (nb5_3 != null)  nb5_3.IsEnabled = false;
						if (nb5_4 != null)  nb5_4.IsEnabled = false;
						if (nb6 != null)    nb6.IsEnabled = false;

						if (tb1 != null)    tb1.IsEnabled = false;
						if (cb1 != null)    cb1.IsEnabled = false;
						if (btn1 != null)	btn1.IsEnabled = false;
						if (btn2 != null)	btn2.IsEnabled = false;
						return;
					case SYNTPH_SENDERIPV4:
					case SYNTPH_SENDERHOSNAMEIPV4:
					case SYNTPH_SENDERIPV4_START:
					case SYNTPH_SENDERHOSNAMEIPV4_START:
						if (rb1 != null)    rb1.IsEnabled = true;
						if (rb2 != null)    rb2.IsEnabled = true;
						if (rb3 != null)    rb3.IsEnabled = true;
						if (rb4 != null)	rb4.IsEnabled = true;
						if (rb5 != null)	rb5.IsEnabled = true;

						if (nb1 != null)	nb1.IsEnabled = true;
						if (nb2 != null)	nb2.IsEnabled = true;
						if (nb3 != null)	nb3.IsEnabled = true;
						if (nb4 != null)	nb4.IsEnabled = true;
						if (nb6 != null)	nb6.IsEnabled = true;
						if ((dwSynTPHelper1 == SYNTPH_SENDERIPV4) || (dwSynTPHelper1 == SYNTPH_SENDERIPV4_START)) {
							if (nb5_1 != null)	nb5_1.IsEnabled = true;
							if (nb5_2 != null)	nb5_2.IsEnabled = true;
							if (nb5_3 != null)	nb5_3.IsEnabled = true;
							if (nb5_4 != null)	nb5_4.IsEnabled = true;
							if (tb1 != null)	tb1.IsEnabled = true;
						}
						else if ((dwSynTPHelper1 == SYNTPH_SENDERHOSNAMEIPV4) || (dwSynTPHelper1 == SYNTPH_SENDERHOSNAMEIPV4_START)) {
							if (nb5_1 != null)	nb5_1.IsEnabled = false;
							if (nb5_2 != null)	nb5_2.IsEnabled = false;
							if (nb5_3 != null)	nb5_3.IsEnabled = false;
							if (nb5_4 != null)	nb5_4.IsEnabled = false;
							if (tb1 != null)	tb1.IsEnabled = true;
						}
						if (cb1 != null)	cb1.IsEnabled = true;
						if ((dwSynTPHelper1 == SYNTPH_SENDERIPV4_START) || (dwSynTPHelper1 == SYNTPH_SENDERHOSNAMEIPV4_START)) {
							if (cb1 != null)	cb1.IsChecked = true;
						}
						else {
							if (cb1 != null)	cb1.IsChecked = false;
						}
						break;
					case SYNTPH_RECEIVERIPV4:
					case SYNTPH_RECEIVERIPV4_START:
						if (rb1 != null)    rb1.IsEnabled = true;
						if (rb2 != null)    rb2.IsEnabled = true;
						if (rb3 != null)    rb3.IsEnabled = true;
						if (rb4 != null)	rb4.IsEnabled = false;
						if (rb5 != null)	rb5.IsEnabled = false;

						if (nb1 != null)	nb1.IsEnabled = false;
						if (nb2 != null)	nb2.IsEnabled = false;
						if (nb3 != null)	nb3.IsEnabled = false;
						if (nb4 != null)	nb4.IsEnabled = false;
						if (nb5_1 != null)	nb5_1.IsEnabled = false;
						if (nb5_2 != null)	nb5_2.IsEnabled = false;
						if (nb5_3 != null)	nb5_3.IsEnabled = false;
						if (nb5_4 != null)	nb5_4.IsEnabled = false;
						if (nb6 != null)	nb6.IsEnabled = true;

						if (tb1 != null)	tb1.IsEnabled = false;
						if (cb1 != null)	cb1.IsEnabled = true;
						if (cb1 != null)	cb1.IsEnabled = true;
						if ((dwSynTPHelper1 == SYNTPH_RECEIVERIPV4_START)) {
							if (cb1 != null)	cb1.IsChecked = true;
						}
						else {
							if (cb1 != null)	cb1.IsChecked = false;
						}
						break;
				}
				if (bSynTPStarted1) {
					if (rb1 != null)	rb1.IsEnabled = false;	
					if (rb2 != null)	rb2.IsEnabled = false;	
					if (rb3 != null)	rb3.IsEnabled = false;	
					if (rb4 != null)    rb4.IsEnabled = false;
					if (rb5 != null)    rb5.IsEnabled = false;

					if (nb1 != null)    nb1.IsEnabled = false;
					if (nb2 != null)    nb2.IsEnabled = false;
					if (nb3 != null)    nb3.IsEnabled = false;
					if (nb4 != null)    nb4.IsEnabled = false;
					if (nb5_1 != null)  nb5_1.IsEnabled = false;
					if (nb5_2 != null)  nb5_2.IsEnabled = false;
					if (nb5_3 != null)  nb5_3.IsEnabled = false;
					if (nb5_4 != null)  nb5_4.IsEnabled = false;
					if (nb6 != null)    nb6.IsEnabled = false;

					if (tb1 != null)    tb1.IsEnabled = false;
					if (cb1 != null)    cb1.IsEnabled = false;
					Int64 _hWnd = FindWindowW(CLASS_FLUSHMOUSE, null);
					if (_hWnd != 0) {
						if (btn1 != null) btn1.IsEnabled = false;
						if (btn2 != null) btn2.IsEnabled = true;
					}
					else {
						if (btn1 != null) btn1.IsEnabled = false;
						if (btn2 != null) btn2.IsEnabled = false;
					}
				}
				else {
					if (rb1 != null)	rb1.IsEnabled = true;	
					if (rb2 != null)	rb2.IsEnabled = true;	
					if (rb3 != null)	rb3.IsEnabled = true;	
					Int64 _hWnd = FindWindowW(CLASS_FLUSHMOUSE, null);
					if (_hWnd != 0) {
						if (btn1 != null)	btn1.IsEnabled = true;
						if (btn2 != null)	btn2.IsEnabled = false;
					}
					else {
						if (btn1 != null) btn1.IsEnabled = false;
						if (btn2 != null) btn2.IsEnabled = false;
					}
				}
			}
		}

		private void SynTP_helper_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
			if (UpdateProfile(SETTINGSEX_SYNTP_IS_STARTED) == 0) {
				bSynTPStarted1 = false;
			}
			else {
				bSynTPStarted1 = true;
			}
			SetNumberBox();
			SetTextBox();
			SetCheckBox();
			SetRadioButton();
			m_Sentinel = true;
			EnableDisableItems(sender, e);
		}

		private void SetRadioButton()
		{
			if (rb1 == null) rb1 = new RadioButton();
			if (rb2 == null) rb2 = new RadioButton();
			if (rb3 == null) rb3 = new RadioButton();
			if (rb4 == null) rb4 = new RadioButton();
			if (rb5 == null) rb5 = new RadioButton();
			rb1.IsEnabled = true;
			rb2.IsEnabled = true;
			rb3.IsEnabled = true;
			rb4.IsEnabled = true;
			rb5.IsEnabled = true;
			switch (dwSynTPHelper1) {
				case SYNTPH_DISABLE:
					rb1.IsChecked = true;
					rb2.IsChecked = false;
					rb3.IsChecked = false;
					break;
				case SYNTPH_SENDERIPV4:
				case SYNTPH_SENDERIPV4_START:
					rb1.IsChecked = false;
					rb2.IsChecked = true;
					rb3.IsChecked = false;
					rb4.IsChecked = true;
					rb5.IsChecked = false;
					break;
				case SYNTPH_SENDERHOSNAMEIPV4:
				case SYNTPH_SENDERHOSNAMEIPV4_START:
					rb1.IsChecked = false;
					rb2.IsChecked = true;
					rb3.IsChecked = false;
					rb4.IsChecked = false;
					rb5.IsChecked = true;
					break;
				case SYNTPH_RECEIVERIPV4:
				case SYNTPH_RECEIVERIPV4_START:
					rb1.IsChecked = false;
					rb2.IsChecked = false;
					rb3.IsChecked = true;
					break;
			}
		}

		private void RadioButton_Checked(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			if (e == null) { }
			RadioButton rb = sender as RadioButton;
			if (rb != null) {
				if (rb.Name == "rb1") {
					dwSynTPHelper1 = SYNTPH_DISABLE;
					rb1.IsChecked = true;
					rb2.IsChecked = false;
					rb3.IsChecked = false;
				}
				else if (rb.Name == "rb2") {
					if (rb5.IsChecked == false) {
						if (cb1.IsChecked == true) dwSynTPHelper1 = SYNTPH_SENDERIPV4_START;
						else dwSynTPHelper1 = SYNTPH_SENDERIPV4;
						rb4.IsChecked = true;
						rb5.IsChecked = false;
					}
					else {
						if (cb1.IsChecked == true) dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4_START;
						else dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4;
						rb4.IsChecked = false;
						rb5.IsChecked = true;
					}
					rb1.IsChecked = false;
					rb2.IsChecked = true;
					rb3.IsChecked = false;
				}
				else if (rb.Name == "rb3") {
					if (cb1.IsChecked == true) dwSynTPHelper1 = SYNTPH_RECEIVERIPV4_START;
					else dwSynTPHelper1 = SYNTPH_RECEIVERIPV4;
					rb1.IsChecked = false;
					rb2.IsChecked = false;
					rb3.IsChecked = true;
				}
				else if (rb.Name == "rb4") {
					if (cb1.IsChecked == true) dwSynTPHelper1 = SYNTPH_SENDERIPV4_START;
					else dwSynTPHelper1 = SYNTPH_SENDERIPV4;
					rb4.IsChecked = true;
					rb5.IsChecked = false;
				}
				else if (rb.Name == "rb5") {
					if (cb1.IsChecked == true) dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4_START;
					else dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4;
					rb4.IsChecked = false;
					rb5.IsChecked = true;
				}
				EnableDisableItems(sender as RadioButton, e);
				UpdateProfile(SETTINGSEX_SYNTP_SETREGISRY);
			}
		}

		private void SetNumberBox()
		{
			if (nb1 == null)	nb1 = new NumberBox();
			if (nb2 == null)	nb2 = new NumberBox();
			if (nb3 == null)	nb3 = new NumberBox();
			if (nb4 == null)	nb4 = new NumberBox();
			if (nb5_1 == null)	nb5_1 = new NumberBox();
			if (nb5_2 == null)	nb5_2 = new NumberBox();
			if (nb5_3 == null)	nb5_3 = new NumberBox();
			if (nb5_4 == null)	nb5_4 = new NumberBox();
			if (nb6 == null)	nb6 = new NumberBox();
			nb1.Text = dwSynTPPadX.ToString();
			nb2.Text = dwSynTPPadY.ToString();
			nb3.Text = dwSynTPEdgeX.ToString();
			nb4.Text = dwSynTPEdgeY.ToString();
			nb5_1.Text = szSynTPSendIPAddr1_1;
			nb5_2.Text = szSynTPSendIPAddr1_2;
			nb5_3.Text = szSynTPSendIPAddr1_3;
			nb5_4.Text = szSynTPSendIPAddr1_4;
			nb6.Text = dwSynTPPortNo1.ToString();
		}

		private void nb_LostFocus(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			if (e == null) { }
			NumberBox nb = sender as NumberBox;
			if (nb != null) {
				if (nb == nb1) { if (nb1.Text == "") nb1.Text = "0"; dwSynTPPadX = Convert.ToInt32(nb1.Text); nb1.Text = dwSynTPPadX.ToString(); }
				else if (nb == nb2) { if (nb2.Text == "") nb2.Text = "0"; dwSynTPPadY = Convert.ToInt32(nb2.Text); nb2.Text = dwSynTPPadY.ToString(); }
				else if (nb == nb3) { if (nb3.Text == "") nb3.Text = "0"; dwSynTPEdgeX = Convert.ToInt32(nb3.Text); nb3.Text = dwSynTPEdgeX.ToString(); }
				else if (nb == nb4) { if (nb4.Text == "") nb4.Text = "0"; dwSynTPEdgeY = Convert.ToInt32(nb4.Text); nb4.Text = dwSynTPEdgeY.ToString(); }
				else if (nb == nb5_1) { if (nb5_1.Text == "") nb5_1.Text = "0"; nb5_1.Text = szSynTPSendIPAddr1_1 = Convert.ToInt32(nb5_1.Text).ToString(); }
				else if (nb == nb5_2) { if (nb5_2.Text == "") nb5_2.Text = "0"; nb5_2.Text = szSynTPSendIPAddr1_2 = Convert.ToInt32(nb5_2.Text).ToString(); }
				else if (nb == nb5_3) { if (nb5_3.Text == "") nb5_3.Text = "0"; nb5_3.Text = szSynTPSendIPAddr1_3 = Convert.ToInt32(nb5_3.Text).ToString(); }
				else if (nb == nb5_4) { if (nb5_4.Text == "") nb5_4.Text = "0"; nb5_4.Text = szSynTPSendIPAddr1_4 = Convert.ToInt32(nb5_4.Text).ToString(); }
				else if (nb == nb6) { if (nb6.Text == "") nb6.Text = "0"; dwSynTPPortNo1 = Convert.ToInt32(nb6.Text); nb6.Text = dwSynTPPortNo1.ToString(); }

				UpdateProfile(SETTINGSEX_SYNTP_SETREGISRY);
			}
		}

		private void nb_PreviewKeyDown(object sender, KeyRoutedEventArgs e)
		{
			if (sender == null) { }
			if (e != null) {
				if ((VirtualKey.Number0 <= e.Key) && (e.Key <= VirtualKey.Number9)
						 || (e.Key == VirtualKey.Delete) || (e.Key == VirtualKey.Back)
						 || (e.Key == VirtualKey.Left) || (e.Key == VirtualKey.Right)
						 || (e.Key == VirtualKey.Up) || (e.Key == VirtualKey.Down)
						 || (e.Key == VirtualKey.Tab)) {
				}
				else {
					e.Handled = true;
				}
			}
		}

		private void nb_PreviewKeyUp(object sender, KeyRoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			if (sender == null) { }
			if (e != null) {
				if ((VirtualKey.Number0 <= e.Key) && (e.Key <= VirtualKey.Number9)
						 || (e.Key == VirtualKey.Delete) || (e.Key == VirtualKey.Back)) {
					NumberBox nb = sender as NumberBox;
					if (nb == null) { return; }
				}
			}
		}

		private void nb_GotFocus(object sender, RoutedEventArgs e)
		{
			if (e == null) { }
			NumberBox nb = sender as NumberBox;
			if (nb == null) { return; }
		}

		private void SetTextBox()
		{
			if (tb1 == null)	tb1 = new TextBox();
			tb1.Text = szSynTPSendHostname1;
		}

		private void tb_LostFocus(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			if (e == null) { }
			TextBox tb = sender as TextBox;
			if (tb != null) {
				if (tb == tb1) { szSynTPSendHostname1 = tb1.Text; }
				UpdateProfile(SETTINGSEX_SYNTP_SETREGISRY);
			}
		}
		
		private void tb_GotFocus(object sender, RoutedEventArgs e)
		{
			if (e == null) { }
			TextBox tb = sender as TextBox;
			if (tb == null) { return; }
			tb.SelectAll();
		}


		private void SetCheckBox()
		{
			if (cb1 == null) cb1 = new CheckBox();
			switch (dwSynTPHelper1) {
				case SYNTPH_DISABLE:
					cb1.IsChecked = false;
					break;
				case SYNTPH_SENDERIPV4:
				case SYNTPH_SENDERHOSNAMEIPV4:
				case SYNTPH_RECEIVERIPV4:
					cb1.IsChecked = false;
					break;
				case SYNTPH_SENDERIPV4_START:
				case SYNTPH_SENDERHOSNAMEIPV4_START:
				case SYNTPH_RECEIVERIPV4_START:
					cb1.IsChecked = true;
					break;
			}

		}
		
		private void CheckBox_Click(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			if (e == null) { }
			CheckBox cb = sender as CheckBox;
			if (cb == null) { return; }
			if (cb.Name == "cb1") {
				if (cb.IsChecked == true) {
					if (rb2.IsChecked == true) {
						if (rb4.IsChecked == true) {
							dwSynTPHelper1 = SYNTPH_SENDERIPV4_START;
						}
						else {
							dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4_START;
						}
					}
					else if (rb3.IsChecked == true) {
						dwSynTPHelper1 = SYNTPH_RECEIVERIPV4_START;
					}
				}
				else {
					if (rb2.IsChecked == true) {
						if (rb4.IsChecked == true) {
							dwSynTPHelper1 = SYNTPH_SENDERIPV4;
						}
						else {
							dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4;
						}
					}
					else if (rb3.IsChecked == true) {
						dwSynTPHelper1 = SYNTPH_RECEIVERIPV4;
					}
				}
				EnableDisableItems(sender, e);
				UpdateProfile(SETTINGSEX_SYNTP_SETREGISRY);
			}
		}

		private void Button_Click(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			if (e == null) { }
			Button btn = sender as Button;
			if (btn == null) { return; }
			if (btn.Name == "btn1") {
				if (btn1 != null) btn1.IsEnabled = false;
				if (btn2 != null) btn2.IsEnabled = false;
				if (UpdateProfile(SETTINGSEX_SYNTP_START) != 0) {
					bSynTPStarted1 = true;
				}
				else {
					bSynTPStarted1 = false;
				}
			}
			else if (btn.Name == "btn2") {
				UpdateProfile(SETTINGSEX_SYNTP_STOP);
				bSynTPStarted1 = false;
			}
			EnableDisableItems(sender, e);
		 }
	}
}
