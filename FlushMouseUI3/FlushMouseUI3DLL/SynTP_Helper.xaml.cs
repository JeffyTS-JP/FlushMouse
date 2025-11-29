//
// SynTP_Helper.Xaml.cs for FlushMouseUI3DLL
//
//      Copyright (C) 1993- JeffyTS. All rights reserved.
//      Licensed under the GPL-2.0 License.
//
// No.    Date          Name            Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000    2024/07/15  JeffyTS     New edit.
//

using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Input;

using System;
using Windows.System;

using static FlushMouseUI3DLL.Settings;
using static FlushMouseUI3DLL.Miscs;

namespace FlushMouseUI3DLL {
    public sealed partial class Settings
    {
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

    public sealed partial class SynTP_Helper
    {
        private static bool m_Sentinel {  get; set; }

        internal const Int32 SYNTPH_DISABLE = 0x00;                 // disable
        internal const Int32 SYNTPH_SENDERIPV4 = 0x01;              // sender (IPv4)
        internal const Int32 SYNTPH_SENDERHOSNAMEIPV4 = 0x11;       // sender (Hosstname IPV4)
        internal const Int32 SYNTPH_SENDERHOSNAMEIPV6 = 0x31;       // sender (Hosstname IPV6)
        internal const Int32 SYNTPH_SENDERIPV4_START = 0x02;        // sender (always start IPv4)
        internal const Int32 SYNTPH_SENDERHOSNAMEIPV4_START = 0x12; // sender (always start Hosstname IPV4)
        internal const Int32 SYNTPH_SENDERHOSNAMEIPV6_START = 0x32; // sender (always start Hosstname IPV6)
        internal const Int32 SYNTPH_RECEIVERIPV4 = 0x03;            // receiver (IPv4)
        internal const Int32 SYNTPH_RECEIVERIPV6 = 0x33;            // receiver (IPv6)
        internal const Int32 SYNTPH_RECEIVERIPV4_START = 0x04;      // receiver (always start IPv4)
        internal const Int32 SYNTPH_RECEIVERIPV6_START = 0x34;      // receiver (always start IPv6)
    }

    public sealed partial class SynTP_Helper : Page
    {
        public SynTP_Helper() {
            m_Sentinel = false;
            InitializeComponent();
        }

        private void EnableDisableItems(object sender, RoutedEventArgs e) {
            if (e == null) { }
            if (sender != null) {
                switch (dwSynTPHelper1) {
                    case SYNTPH_DISABLE:
                        if (rb1 != null)    rb1.IsEnabled = true;
                        if (rb2 != null)    rb2.IsEnabled = true;
                        if (rb3 != null)    rb3.IsEnabled = true;
                        if (rb4 != null)    rb4.IsEnabled = false;
                        if (rb5 != null)    rb5.IsEnabled = false;

                        if (tb1 != null)    tb1.IsEnabled = false;
                        if (tb2 != null)    tb2.IsEnabled = false;
                        if (tb3 != null)    tb3.IsEnabled = false;
                        if (tb4 != null)    tb4.IsEnabled = false;
                        if (tb5_1 != null)  tb5_1.IsEnabled = false;
                        if (tb5_2 != null)  tb5_2.IsEnabled = false;
                        if (tb5_3 != null)  tb5_3.IsEnabled = false;
                        if (tb5_4 != null)  tb5_4.IsEnabled = false;
                        if (tb6 != null)    tb6.IsEnabled = false;
                        if (tb7 != null)    tb7.IsEnabled = false;

                        if (cb1 != null)    cb1.IsEnabled = false;
                        if (btn1 != null)   btn1.IsEnabled = false;
                        if (btn2 != null)   btn2.IsEnabled = false;
                        return;
                    case SYNTPH_SENDERIPV4:
                    case SYNTPH_SENDERHOSNAMEIPV4:
                    case SYNTPH_SENDERIPV4_START:
                    case SYNTPH_SENDERHOSNAMEIPV4_START:
                        if (rb1 != null)    rb1.IsEnabled = true;
                        if (rb2 != null)    rb2.IsEnabled = true;
                        if (rb3 != null)    rb3.IsEnabled = true;
                        if (rb4 != null)    rb4.IsEnabled = true;
                        if (rb5 != null)    rb5.IsEnabled = true;

                        if (tb1 != null)    tb1.IsEnabled = true;
                        if (tb2 != null)    tb2.IsEnabled = true;
                        if (tb3 != null)    tb3.IsEnabled = true;
                        if (tb4 != null)    tb4.IsEnabled = true;
                        if (tb7 != null)    tb7.IsEnabled = true;
                        if ((dwSynTPHelper1 == SYNTPH_SENDERIPV4) || (dwSynTPHelper1 == SYNTPH_SENDERIPV4_START)) {
                            if (tb5_1 != null)  tb5_1.IsEnabled = true;
                            if (tb5_2 != null)  tb5_2.IsEnabled = true;
                            if (tb5_3 != null)  tb5_3.IsEnabled = true;
                            if (tb5_4 != null)  tb5_4.IsEnabled = true;
                            if (tb6 != null)    tb6.IsEnabled = false;
                        }
                        else if ((dwSynTPHelper1 == SYNTPH_SENDERHOSNAMEIPV4) || (dwSynTPHelper1 == SYNTPH_SENDERHOSNAMEIPV4_START)) {
                            if (tb5_1 != null)  tb5_1.IsEnabled = false;
                            if (tb5_2 != null)  tb5_2.IsEnabled = false;
                            if (tb5_3 != null)  tb5_3.IsEnabled = false;
                            if (tb5_4 != null)  tb5_4.IsEnabled = false;
                            if (tb6 != null)    tb6.IsEnabled = true;
                        }
                        if (cb1 != null)    cb1.IsEnabled = true;
                        if ((dwSynTPHelper1 == SYNTPH_SENDERIPV4_START) || (dwSynTPHelper1 == SYNTPH_SENDERHOSNAMEIPV4_START)) {
                            if (cb1 != null)    cb1.IsChecked = true;
                        }
                        else {
                            if (cb1 != null)    cb1.IsChecked = false;
                        }
                        break;
                    case SYNTPH_RECEIVERIPV4:
                    case SYNTPH_RECEIVERIPV4_START:
                        if (rb1 != null)    rb1.IsEnabled = true;
                        if (rb2 != null)    rb2.IsEnabled = true;
                        if (rb3 != null)    rb3.IsEnabled = true;
                        if (rb4 != null)    rb4.IsEnabled = false;
                        if (rb5 != null)    rb5.IsEnabled = false;

                        if (tb1 != null)    tb1.IsEnabled = false;
                        if (tb2 != null)    tb2.IsEnabled = false;
                        if (tb3 != null)    tb3.IsEnabled = false;
                        if (tb4 != null)    tb4.IsEnabled = false;
                        if (tb5_1 != null)  tb5_1.IsEnabled = false;
                        if (tb5_2 != null)  tb5_2.IsEnabled = false;
                        if (tb5_3 != null)  tb5_3.IsEnabled = false;
                        if (tb5_4 != null)  tb5_4.IsEnabled = false;
                        if (tb6 != null)    tb6.IsEnabled = false;
                        if (tb7 != null)    tb7.IsEnabled = true;

                        if (cb1 != null)    cb1.IsEnabled = true;
                        if (cb1 != null)    cb1.IsEnabled = true;
                        if ((dwSynTPHelper1 == SYNTPH_RECEIVERIPV4_START)) {
                            if (cb1 != null)    cb1.IsChecked = true;
                        }
                        else {
                            if (cb1 != null)    cb1.IsChecked = false;
                        }
                        break;
                }
                if (bSynTPStarted1) {
                    if (rb1 != null)    rb1.IsEnabled = false;
                    if (rb2 != null)    rb2.IsEnabled = false;
                    if (rb3 != null)    rb3.IsEnabled = false;
                    if (rb4 != null)    rb4.IsEnabled = false;
                    if (rb5 != null)    rb5.IsEnabled = false;

                    if (tb1 != null)    tb1.IsEnabled = false;
                    if (tb2 != null)    tb2.IsEnabled = false;
                    if (tb3 != null)    tb3.IsEnabled = false;
                    if (tb4 != null)    tb4.IsEnabled = false;
                    if (tb5_1 != null)  tb5_1.IsEnabled = false;
                    if (tb5_2 != null)  tb5_2.IsEnabled = false;
                    if (tb5_3 != null)  tb5_3.IsEnabled = false;
                    if (tb5_4 != null)  tb5_4.IsEnabled = false;
                    if (tb6 != null)    tb6.IsEnabled = false;
                    if (tb7 != null)    tb7.IsEnabled = false;

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
                    if (rb1 != null)    rb1.IsEnabled = true;
                    if (rb2 != null)    rb2.IsEnabled = true;
                    if (rb3 != null)    rb3.IsEnabled = true;
                    Int64 _hWnd = FindWindowW(CLASS_FLUSHMOUSE, null);
                    if (_hWnd != 0) {
                        if (btn1 != null)   btn1.IsEnabled = true;
                        if (btn2 != null)   btn2.IsEnabled = false;
                    }
                    else {
                        if (btn1 != null) btn1.IsEnabled = false;
                        if (btn2 != null) btn2.IsEnabled = false;
                    }
                }
            }
        }

        private void SynTP_helper_Loaded(object sender, RoutedEventArgs e) {
            if (sender == null) { }
            if (e == null) { }
            Int64 _hWnd = FindWindowW(CLASS_FLUSHMOUSE, null);
            if (_hWnd != 0) {
                if (UpdateProfile(SETTINGSEX_SYNTP_IS_STARTED) == 0) {
                    bSynTPStarted1 = false;
                }
                else {
                    bSynTPStarted1 = true;
                }
            }
            else {
                bSynTPStarted1 = false;
            }

            SetTextBox();
            SetCheckBox();
            SetRadioButton();
            m_Sentinel = true;
            EnableDisableItems(sender, e);
        }

        private void SetRadioButton() {
            if (rb1 == null) rb1 = new RadioButton();
            if (rb2 == null) rb2 = new RadioButton();
            if (rb3 == null) rb3 = new RadioButton();
            if (rb4 == null) rb4 = new RadioButton();
            if (rb5 == null) rb5 = new RadioButton();
            rb1.IsEnabled = true;   rb2.IsEnabled = true;   rb3.IsEnabled = true;
            rb4.IsEnabled = true;   rb5.IsEnabled = true;
            switch (dwSynTPHelper1) {
                case SYNTPH_DISABLE:
                    rb1.IsChecked = true;   rb2.IsChecked = false;  rb3.IsChecked = false;
                    break;
                case SYNTPH_SENDERIPV4:
                case SYNTPH_SENDERIPV4_START:
                    rb1.IsChecked = false;  rb2.IsChecked = true;   rb3.IsChecked = false;
                    rb4.IsChecked = true;   rb5.IsChecked = false;
                    break;
                case SYNTPH_SENDERHOSNAMEIPV4:
                case SYNTPH_SENDERHOSNAMEIPV4_START:
                    rb1.IsChecked = false;  rb2.IsChecked = true;   rb3.IsChecked = false;
                    rb4.IsChecked = false;  rb5.IsChecked = true;
                    break;
                case SYNTPH_RECEIVERIPV4:
                case SYNTPH_RECEIVERIPV4_START:
                    rb1.IsChecked = false;  rb2.IsChecked = false;  rb3.IsChecked = true;
                    break;
            }
        }

        private void RadioButton_Checked(object sender, RoutedEventArgs e) {
            if (m_Sentinel == false) return;
            if (e == null) { }
            RadioButton rb = sender as RadioButton;
            if (rb != null) {
                if (rb.Name == "rb1") {
                    dwSynTPHelper1 = SYNTPH_DISABLE;
                    rb1.IsChecked = true;   rb2.IsChecked = false;  rb3.IsChecked = false;
                }
                else if (rb.Name == "rb2") {
                    if (rb5.IsChecked == false) {
                        if (cb1.IsChecked == true) dwSynTPHelper1 = SYNTPH_SENDERIPV4_START;
                        else dwSynTPHelper1 = SYNTPH_SENDERIPV4;
                        rb4.IsChecked = true;   rb5.IsChecked = false;
                    }
                    else {
                        if (cb1.IsChecked == true) dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4_START;
                        else dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4;
                        rb4.IsChecked = false;  rb5.IsChecked = true;
                    }
                    rb1.IsChecked = false;  rb2.IsChecked = true;   rb3.IsChecked = false;
                }
                else if (rb.Name == "rb3") {
                    if (cb1.IsChecked == true) dwSynTPHelper1 = SYNTPH_RECEIVERIPV4_START;
                    else dwSynTPHelper1 = SYNTPH_RECEIVERIPV4;
                    rb1.IsChecked = false;  rb2.IsChecked = false;  rb3.IsChecked = true;
                }
                else if (rb.Name == "rb4") {
                    if (cb1.IsChecked == true) dwSynTPHelper1 = SYNTPH_SENDERIPV4_START;
                    else dwSynTPHelper1 = SYNTPH_SENDERIPV4;
                    rb4.IsChecked = true;   rb5.IsChecked = false;
                }
                else if (rb.Name == "rb5") {
                    if (cb1.IsChecked == true) dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4_START;
                    else dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4;
                    rb4.IsChecked = false;  rb5.IsChecked = true;
                }
                EnableDisableItems(sender as RadioButton, e);
                UpdateProfile(SETTINGSEX_SYNTP_SETREGISRY);
            }
        }

        private void tb_PreviewKeyDown(object sender, KeyRoutedEventArgs e) {
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

        private void tb_PreviewKeyUp(object sender, KeyRoutedEventArgs e) {
            if (m_Sentinel == false) return;
            if (e == null) { }
            if (sender != null) { }
        }

        private void SetTextBox() {
            if (tb1 == null)    tb1 = new TextBox();
            if (tb2 == null)    tb2 = new TextBox();
            if (tb3 == null)    tb3 = new TextBox();
            if (tb4 == null)    tb4 = new TextBox();
            if (tb5_1 == null)  tb5_1 = new TextBox();
            if (tb5_2 == null)  tb5_2 = new TextBox();
            if (tb5_3 == null)  tb5_3 = new TextBox();
            if (tb5_4 == null)  tb5_4 = new TextBox();
            if (tb6 == null)    tb6 = new TextBox();
            if (tb7 == null)    tb7 = new TextBox();
            tb1.Text = dwSynTPPadX.ToString();  tb2.Text = dwSynTPPadY.ToString();
            tb3.Text = dwSynTPEdgeX.ToString(); tb4.Text = dwSynTPEdgeY.ToString();
            tb5_1.Text = szSynTPSendIPAddr1_1;  tb5_2.Text = szSynTPSendIPAddr1_2;
            tb5_3.Text = szSynTPSendIPAddr1_3;  tb5_4.Text = szSynTPSendIPAddr1_4;
            InputScopeName scopeName = new() { NameValue = InputScopeNameValue.AlphanumericHalfWidth };
            InputScope scope = new();   scope.Names.Add(scopeName); tb6.InputScope = scope;
            tb6.Text = szSynTPSendHostname1;
            tb7.Text = dwSynTPPortNo1.ToString();
        }

        private void tb_LostFocus(object sender, RoutedEventArgs e) {
            if (m_Sentinel == false) return;
            if (e == null) { }
            TextBox tb = sender as TextBox;
            if (tb != null) {
                if (tb == tb1) {
                    if (tb.Text.Length == 0) { MessageBeep(MB_ICONWARNING); tb.Text = dwSynTPPadX.ToString(); tb.Focus(FocusState.Programmatic); }
                    if (CheckNumeric(tb.Text, 0, 9999) == false) { MessageBeep(MB_ICONWARNING); tb.Undo(); tb.Focus(FocusState.Programmatic); return; }
                    else dwSynTPPadX = Convert.ToInt32(tb.Text);
                }
                else if (tb == tb2) {
                    if (tb.Text.Length == 0) { MessageBeep(MB_ICONWARNING); tb.Text = dwSynTPPadY.ToString(); tb.Focus(FocusState.Programmatic); }
                    if (CheckNumeric(tb.Text, 0, 9999) == false) { MessageBeep(MB_ICONWARNING); tb.Undo(); tb.Focus(FocusState.Programmatic); return; }
                    else dwSynTPPadY = Convert.ToInt32(tb.Text);
                }
                else if (tb == tb3) {
                    if (tb.Text.Length == 0) { MessageBeep(MB_ICONWARNING); tb.Text = dwSynTPEdgeX.ToString(); tb.Focus(FocusState.Programmatic); }
                    if (CheckNumeric(tb.Text, 0, 9999) == false) { MessageBeep(MB_ICONWARNING); tb.Undo(); tb.Focus(FocusState.Programmatic); return; }
                    else dwSynTPEdgeX = Convert.ToInt32(tb.Text);
                }
                else if (tb == tb4) {
                    if (tb.Text.Length == 0) { MessageBeep(MB_ICONWARNING); tb.Text = dwSynTPEdgeY.ToString(); tb.Focus(FocusState.Programmatic); }
                    if (CheckNumeric(tb.Text, 0, 9999) == false) { MessageBeep(MB_ICONWARNING); tb.Undo(); tb.Focus(FocusState.Programmatic); return; }
                    else dwSynTPEdgeY = Convert.ToInt32(tb.Text);
                }
                else if (tb == tb5_1) {
                    if (tb.Text.Length == 0) { MessageBeep(MB_ICONWARNING); if (szSynTPSendIPAddr1_1 == null) tb.Text = "0"; else tb.Text = szSynTPSendIPAddr1_1; tb.Focus(FocusState.Programmatic);}
                    if (CheckNumeric(tb.Text, 0, 255) == false) { MessageBeep(MB_ICONWARNING); tb.Undo(); tb.Focus(FocusState.Programmatic); return; }
                    else szSynTPSendIPAddr1_1 = tb.Text;
                }
                else if (tb == tb5_2) {
                    if (tb.Text.Length == 0) { MessageBeep(MB_ICONWARNING); if (szSynTPSendIPAddr1_2 == null) tb.Text = "0"; else tb.Text = szSynTPSendIPAddr1_2; tb.Focus(FocusState.Programmatic); }
                    if (CheckNumeric(tb.Text, 0, 255) == false) { MessageBeep(MB_ICONWARNING); tb.Undo(); tb.Focus(FocusState.Programmatic); return; }
                    else szSynTPSendIPAddr1_2 = tb.Text;
                }
                else if (tb == tb5_3) {
                    if (tb.Text.Length == 0) { MessageBeep(MB_ICONWARNING); if (szSynTPSendIPAddr1_3 == null) tb.Text = "0"; else tb.Text = szSynTPSendIPAddr1_3; tb.Focus(FocusState.Programmatic); }
                    if (CheckNumeric(tb.Text, 0, 255) == false) { MessageBeep(MB_ICONWARNING); tb.Undo(); tb.Focus(FocusState.Programmatic); return; }
                    else szSynTPSendIPAddr1_3 = tb.Text;
                }
                else if (tb == tb5_4) {
                    if (tb.Text.Length == 0) { MessageBeep(MB_ICONWARNING); if (szSynTPSendIPAddr1_4 == null) tb.Text = "0"; else tb.Text = szSynTPSendIPAddr1_4; tb.Focus(FocusState.Programmatic); }
                    if (CheckNumeric(tb.Text, 0, 255) == false) { MessageBeep(MB_ICONWARNING); tb.Undo(); tb.Focus(FocusState.Programmatic); return; }
                    else szSynTPSendIPAddr1_4 = tb.Text;
                }
                else if (tb == tb6) {
                    if (tb.Text.Length == 0) { if ((szSynTPSendHostname1 != null) && (szSynTPSendHostname1.Length != 0)) { tb.Text = szSynTPSendHostname1; tb.Focus(FocusState.Programmatic); } return; }
                    if (CheckHostname(tb.Text) == false) { MessageBeep(MB_ICONWARNING); tb.Undo(); tb.Focus(FocusState.Programmatic); return; }
                    else szSynTPSendHostname1 = tb.Text;
                }
                else if (tb == tb7) {
                    if (tb.Text.Length == 0) { MessageBeep(MB_ICONWARNING); tb.Text = dwSynTPPortNo1.ToString(); tb.Focus(FocusState.Programmatic); }
                    if (CheckNumeric(tb.Text, 50000, 59999) == false) { MessageBeep(MB_ICONWARNING); tb.Undo(); tb.Focus(FocusState.Programmatic); return; }
                    else dwSynTPPortNo1 = Convert.ToInt32(tb.Text);
                }
                UpdateProfile(SETTINGSEX_SYNTP_SETREGISRY);
            }
        }

        private void tb_GotFocus(object sender, RoutedEventArgs e) {
            if (e == null) { }
            TextBox tb = sender as TextBox;
            if (tb == null) { return; }
            tb.SelectAll();
        }

        private void SetCheckBox() {
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

        private void CheckBox_Click(object sender, RoutedEventArgs e) {
            if (m_Sentinel == false) return;
            if (e == null) { }
            CheckBox cb = sender as CheckBox;
            if (cb == null) { return; }
            if (cb.Name == "cb1") {
                if (cb.IsChecked == true) {
                    if (rb2.IsChecked == true) {
                        if (rb4.IsChecked == true)  dwSynTPHelper1 = SYNTPH_SENDERIPV4_START;
                        else dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4_START;
                    }
                    else if (rb3.IsChecked == true) dwSynTPHelper1 = SYNTPH_RECEIVERIPV4_START;
                }
                else {
                    if (rb2.IsChecked == true) {
                        if (rb4.IsChecked == true)  dwSynTPHelper1 = SYNTPH_SENDERIPV4;
                        else dwSynTPHelper1 = SYNTPH_SENDERHOSNAMEIPV4;
                    }
                    else if (rb3.IsChecked == true) dwSynTPHelper1 = SYNTPH_RECEIVERIPV4;
                    }
                EnableDisableItems(sender, e);
                UpdateProfile(SETTINGSEX_SYNTP_SETREGISRY);
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e) {
            if (m_Sentinel == false) return;
            if (e == null) { }
            Button btn = sender as Button;
            if (btn == null) { return; }
            if (btn.Name == "btn1") {
                if (btn1 != null) btn1.IsEnabled = false;
                if (btn2 != null) btn2.IsEnabled = false;
                if (UpdateProfile(SETTINGSEX_SYNTP_START) != 0) bSynTPStarted1 = true;
                else bSynTPStarted1 = false;
            }
            else if (btn.Name == "btn2") {
                UpdateProfile(SETTINGSEX_SYNTP_STOP);
                bSynTPStarted1 = false;
            }
            EnableDisableItems(sender, e);
         }
    }
}
