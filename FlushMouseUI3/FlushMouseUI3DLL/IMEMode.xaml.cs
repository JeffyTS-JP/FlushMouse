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
using System.Text;

using static FlushMouseUI3DLL.Settings;

namespace FlushMouseUI3DLL {
	public sealed partial class Settings
	{
		public static Int32 iModeMouseSize { get; set; }
		public static Int32 iModeCaretSize { get; set; }
		public static Int32 iModeByWndSize { get; set; }
		public static Int32 iIMEModeDistance { get; set; }

		public static String szNearDrawMouseIMEOFFChar { get; set; }
		public static String szNearDrawMouseHANEISU_IMEONChar { get; set; }
		public static String szNearDrawMouseHANKANA_IMEONChar { get; set; }
		public static String szNearDrawMouseZENEISU_IMEONChar { get; set; }
		public static String szNearDrawMouseZENHIRA_IMEONChar { get; set; }
		public static String szNearDrawMouseZENKANA_IMEONChar { get; set; }
		public static String szNearDrawCaretIMEOFFChar { get; set; }
		public static String szNearDrawCaretHANEISU_IMEONChar { get; set; }
		public static String szNearDrawCaretHANKANA_IMEONChar { get; set; }
		public static String szNearDrawCaretZENEISU_IMEONChar { get; set; }
		public static String szNearDrawCaretZENHIRA_IMEONChar { get; set; }
		public static String szNearDrawCaretZENKANA_IMEONChar { get; set; }
		public static String szNearDrawMouseByWndIMEOFFChar { get; set; }
		public static String szNearDrawMouseByWndHANEISU_IMEONChar { get; set; }
		public static String szNearDrawMouseByWndHANKANA_IMEONChar { get; set; }
		public static String szNearDrawMouseByWndZENEISU_IMEONChar { get; set; }
		public static String szNearDrawMouseByWndZENHIRA_IMEONChar { get; set; }
		public static String szNearDrawMouseByWndZENKANA_IMEONChar { get; set; }

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
		
		public static String szNearDrawMouseFont { get; set; }
		public static String szNearDrawCaretFont { get; set; }
		public static String szNearDrawMouseByWndFont { get; set; }
	}

	public sealed partial class IMEMode
	{
		private static bool m_Sentinel {  get; set; }
		
		internal const String ComboItem1 = "Yu Gothic UI";
		internal const String ComboItem2 = "Meiryo UI";
		internal const String ComboItem3 = "MS Gothic";
		internal const String ComboItem4 = "Consolas";
	}

	public sealed partial class IMEMode : Page
	{

		public IMEMode()
		{
			m_Sentinel = false;
			InitializeComponent();
			Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
		}

		private void EnableDisableItems(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
			if ((!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp) || !bDisplayIMEModeIMEOFF) {
				Button1_1.IsEnabled = false;
				tb1_1.IsEnabled = false;
			}
			else {
				Button1_1.IsEnabled = true;
				tb1_1.IsEnabled = true;
			}
			if (!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp) {
				Combo1.IsEnabled = false;
				sl1.IsEnabled = false;
				Button1_2.IsEnabled = false;	Button1_3.IsEnabled = false;	Button1_4.IsEnabled = false;
				Button1_5.IsEnabled = false;	Button1_6.IsEnabled = false;
				tb1_2.IsEnabled = false;		tb1_3.IsEnabled = false;		tb1_4.IsEnabled = false;
				tb1_5.IsEnabled = false;		tb1_6.IsEnabled = false;
			}
			else {
				Combo1.IsEnabled = true;
				sl1.IsEnabled = true;
				Button1_2.IsEnabled = true;		Button1_3.IsEnabled = true;		Button1_4.IsEnabled = true;
				Button1_5.IsEnabled = true;		Button1_6.IsEnabled = true;
				tb1_2.IsEnabled = true;			tb1_3.IsEnabled = true;			tb1_4.IsEnabled = true;
				tb1_5.IsEnabled = true;			tb1_6.IsEnabled = true;
			}
			if ((!bDoModeDispByIMEKeyDown && !bDoModeDispByMouseBttnUp && !bDoModeDispByCtrlUp) || !bDrawNearCaret) {
				Combo2.IsEnabled = false;
				sl2.IsEnabled = false;
				Button2_1.IsEnabled = false;	Button2_2.IsEnabled = false;	Button2_3.IsEnabled = false;
				Button2_4.IsEnabled = false;	Button2_5.IsEnabled = false;	Button2_6.IsEnabled = false;
				tb2_1.IsEnabled = false;		tb2_2.IsEnabled = false;		tb2_3.IsEnabled = false;
				tb2_4.IsEnabled = false;		tb2_5.IsEnabled = false;		tb2_6.IsEnabled = false;
			}
			else {
				Combo2.IsEnabled = true;
				sl2.IsEnabled = true;
				Button2_1.IsEnabled = true;		Button2_2.IsEnabled = true;		Button2_3.IsEnabled = true;
				Button2_4.IsEnabled = true;		Button2_5.IsEnabled = true;		Button2_6.IsEnabled = true;
				tb2_1.IsEnabled = true;		tb2_2.IsEnabled = true;		tb2_3.IsEnabled = true;
				tb2_4.IsEnabled = true;		tb2_5.IsEnabled = true;		tb2_6.IsEnabled = true;
			}
			if (!bDisplayIMEModeOnCursor || !bDisplayIMEModeByWindow) {
				Combo3.IsEnabled = false;
				sl3.IsEnabled = false;	sl4.IsEnabled = false;
				Button3_1.IsEnabled = false;	Button3_2.IsEnabled = false;	Button3_3.IsEnabled = false;
				Button3_4.IsEnabled = false;	Button3_5.IsEnabled = false;	Button3_6.IsEnabled = false;
				tb3_1.IsEnabled = false;		tb3_2.IsEnabled = false;		tb3_3.IsEnabled = false;
				tb3_4.IsEnabled = false;		tb3_5.IsEnabled = false;		tb3_6.IsEnabled = false;
			}
			else {
				if (!bDisplayIMEModeIMEOFF) {
					Button3_1.IsEnabled = false;
					tb3_1.IsEnabled = false;
				}
				else {
					Button3_1.IsEnabled = true;
					tb3_1.IsEnabled = true;
				}
				Combo3.IsEnabled = true;
				sl3.IsEnabled = true;	sl4.IsEnabled = true;
				Button3_2.IsEnabled = true;		Button3_3.IsEnabled = true;		Button3_4.IsEnabled = true;
				Button3_5.IsEnabled = true;		Button3_6.IsEnabled = true;
				tb3_2.IsEnabled = true;			tb3_3.IsEnabled = true;			tb3_4.IsEnabled = true;
				tb3_5.IsEnabled = true;			tb3_6.IsEnabled = true;
			}
		}

		private void IMEMode_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
			SetColorButton();
			SetTextBox();
			SetComboBox();
			SetSlider();
			m_Sentinel = true;
			EnableDisableItems(sender, e);
		}

		private void SetSlider()
		{
			if (sl1 == null)	sl1 = new Slider();
			if (sl2 == null)	sl2 = new Slider();
			if (sl3 == null)	sl3 = new Slider();
			if (sl4 == null)	sl4 = new Slider();
			sl1.Value = iModeMouseSize;
			sl2.Value = iModeCaretSize;
			sl3.Value = iModeByWndSize;
			sl4.Value = iIMEModeDistance - 8;
		}

		private void Slider_ValueChanged(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			Slider sl = sender as Slider;
			if (sl != null) {
				if (sl.Name == "sl1")			iModeMouseSize = (Int32)sl.Value;
				else if (sl.Name == "sl2")		iModeCaretSize = (Int32)sl.Value;
				else if (sl.Name == "sl3")		iModeByWndSize = (Int32)sl.Value;
				else if (sl.Name == "sl4")		iIMEModeDistance = (Int32)sl.Value + 8;
				EnableDisableItems(sender, e);
				UpdateProfile(SETTINGSEX_SETTINGS_IMEMODE_SETREGISTRY);
			}
		}

		private void SetComboBox()
		{
			if (Combo1 == null) Combo1 = new ComboBox();
			if (Combo2 == null) Combo2 = new ComboBox();
			if (Combo3 == null) Combo3 = new ComboBox();
			
			Combo1.Items.Clear();
			Combo1.Items.Add(ComboItem1);	Combo1.Items.Add(ComboItem2);	Combo1.Items.Add(ComboItem3);	Combo1.Items.Add(ComboItem4);
			Combo2.Items.Clear();
			Combo2.Items.Add(ComboItem1);	Combo2.Items.Add(ComboItem2);	Combo2.Items.Add(ComboItem3);	Combo2.Items.Add(ComboItem4);
			Combo3.Items.Clear();
			Combo3.Items.Add(ComboItem1);	Combo3.Items.Add(ComboItem2);	Combo3.Items.Add(ComboItem3);	Combo3.Items.Add(ComboItem4);

			if (szNearDrawMouseFont == ComboItem1)		Combo1.SelectedItem = ComboItem1;
			else if (szNearDrawMouseFont == ComboItem2) Combo1.SelectedItem = ComboItem2;
			else if (szNearDrawMouseFont == ComboItem3) Combo1.SelectedItem = ComboItem3;
			else if (szNearDrawMouseFont == ComboItem4) Combo1.SelectedItem = ComboItem4;

			if (szNearDrawCaretFont == ComboItem1)		Combo2.SelectedItem = ComboItem1;
			else if (szNearDrawCaretFont == ComboItem2) Combo2.SelectedItem = ComboItem2;
			else if (szNearDrawCaretFont == ComboItem3) Combo2.SelectedItem = ComboItem3;
			else if (szNearDrawCaretFont == ComboItem4) Combo2.SelectedItem = ComboItem4;

			if (szNearDrawMouseByWndFont == ComboItem1)			Combo3.SelectedItem = ComboItem1;
			else if (szNearDrawMouseByWndFont == ComboItem2)	Combo3.SelectedItem = ComboItem2;
			else if (szNearDrawMouseByWndFont == ComboItem3)	Combo3.SelectedItem = ComboItem3;
			else if (szNearDrawMouseByWndFont == ComboItem4)	Combo3.SelectedItem = ComboItem4;
		}

		private void Combo_SelectionChanged(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			ComboBox combo = sender as ComboBox;
			if (combo != null) {
				if (combo.Name == "Combo1") {
					if (combo.SelectedItem != null) szNearDrawMouseFont = combo.SelectedItem.ToString();
					else combo.SelectedItem = szNearDrawMouseFont;
				}
				else if (combo.Name == "Combo2") {
					if (combo.SelectedItem != null) szNearDrawCaretFont = combo.SelectedItem.ToString();
					else combo.SelectedItem = szNearDrawCaretFont;
				}
				else if (combo.Name == "Combo3") {
					if (combo.SelectedItem != null) szNearDrawMouseByWndFont = combo.SelectedItem.ToString();
					else combo.SelectedItem = szNearDrawMouseByWndFont;
				}
				UpdateProfile(SETTINGSEX_SETTINGS_IMEMODE_SETREGISTRY);
				UpdateProfile(SETTINGSEX_RELOAD_MOUSECURSOR);
				SetColorButton();
				SetTextBox();
				EnableDisableItems(sender, e);
			}
		}
	
		private void SetTextBox()
		{
			if (tb1_1 == null)    tb1_1 = new TextBox();	if (tb1_2 == null)    tb1_2 = new TextBox();
			if (tb1_3 == null)    tb1_3 = new TextBox();	if (tb1_4 == null)    tb1_4 = new TextBox();
			if (tb1_5 == null)    tb1_5 = new TextBox();	if (tb1_6 == null)    tb1_6 = new TextBox();

			if (tb2_1 == null)    tb2_1 = new TextBox();	if (tb2_2 == null)    tb2_2 = new TextBox();
			if (tb2_3 == null)    tb2_3 = new TextBox();	if (tb2_4 == null)    tb2_4 = new TextBox();
			if (tb2_5 == null)    tb2_5 = new TextBox();	if (tb2_6 == null)    tb2_6 = new TextBox();

			if (tb3_1 == null)    tb3_1 = new TextBox();	if (tb3_2 == null)    tb3_2 = new TextBox();
			if (tb3_3 == null)    tb3_3 = new TextBox();	if (tb3_4 == null)    tb3_4 = new TextBox();
			if (tb3_5 == null)    tb3_5 = new TextBox();	if (tb3_6 == null)    tb3_6 = new TextBox();

			SetTextBox(tb1_1, szNearDrawMouseIMEOFFChar, dwNearDrawMouseIMEOFFColor, szNearDrawMouseFont);
			SetTextBox(tb1_2, szNearDrawMouseZENHIRA_IMEONChar, dwNearDrawMouseZENHIRA_IMEONColor, szNearDrawMouseFont);
			SetTextBox(tb1_3, szNearDrawMouseZENKANA_IMEONChar, dwNearDrawMouseZENKANA_IMEONColor, szNearDrawMouseFont);
			SetTextBox(tb1_4, szNearDrawMouseZENEISU_IMEONChar, dwNearDrawMouseZENEISU_IMEONColor, szNearDrawMouseFont);
			SetTextBox(tb1_5, szNearDrawMouseHANKANA_IMEONChar, dwNearDrawMouseHANKANA_IMEONColor, szNearDrawMouseFont);
			SetTextBox(tb1_6, szNearDrawMouseHANEISU_IMEONChar, dwNearDrawMouseHANEISU_IMEONColor, szNearDrawMouseFont);

			SetTextBox(tb2_1, szNearDrawCaretIMEOFFChar, dwNearDrawCaretIMEOFFColor, szNearDrawCaretFont);
			SetTextBox(tb2_2, szNearDrawCaretZENHIRA_IMEONChar, dwNearDrawCaretZENHIRA_IMEONColor, szNearDrawCaretFont);
			SetTextBox(tb2_3, szNearDrawCaretZENKANA_IMEONChar, dwNearDrawCaretZENKANA_IMEONColor, szNearDrawCaretFont);
			SetTextBox(tb2_4, szNearDrawCaretZENEISU_IMEONChar, dwNearDrawCaretZENEISU_IMEONColor, szNearDrawCaretFont);
			SetTextBox(tb2_5, szNearDrawCaretHANKANA_IMEONChar, dwNearDrawCaretHANKANA_IMEONColor, szNearDrawCaretFont);
			SetTextBox(tb2_6, szNearDrawCaretHANEISU_IMEONChar, dwNearDrawCaretHANEISU_IMEONColor, szNearDrawCaretFont);

			SetTextBox(tb3_1, szNearDrawMouseByWndIMEOFFChar, dwNearDrawMouseByWndIMEOFFColor, szNearDrawMouseByWndFont);
			SetTextBox(tb3_2, szNearDrawMouseByWndZENHIRA_IMEONChar, dwNearDrawMouseByWndZENHIRA_IMEONColor, szNearDrawMouseByWndFont);
			SetTextBox(tb3_3, szNearDrawMouseByWndZENKANA_IMEONChar, dwNearDrawMouseByWndZENKANA_IMEONColor, szNearDrawMouseByWndFont);
			SetTextBox(tb3_4, szNearDrawMouseByWndZENEISU_IMEONChar, dwNearDrawMouseByWndZENEISU_IMEONColor, szNearDrawMouseByWndFont);
			SetTextBox(tb3_5, szNearDrawMouseByWndHANKANA_IMEONChar, dwNearDrawMouseByWndHANKANA_IMEONColor, szNearDrawMouseByWndFont);
			SetTextBox(tb3_6, szNearDrawMouseByWndHANEISU_IMEONChar, dwNearDrawMouseByWndHANEISU_IMEONColor, szNearDrawMouseByWndFont);
		}

		private static void SetTextBox(TextBox Tbx, String Chr, Int32 Clr, String Font)
		{
			Tbx.Text = Chr;
			Color clr = Color.FromArgb(Clr);
			Tbx.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Font != null) Tbx.FontFamily = new FontFamily(Font);
		}
		
		private void tb_LostFocus(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			if (e == null) { }
			TextBox tb = sender as TextBox;
			if (tb != null) {
				string _text = CheckStrings(tb.Text);
				tb.Text = _text;
				if (tb == tb1_1)		szNearDrawMouseIMEOFFChar = _text;
				else if (tb == tb1_2)	szNearDrawMouseZENHIRA_IMEONChar = _text;
				else if (tb == tb1_3)	szNearDrawMouseZENKANA_IMEONChar = _text;
				else if (tb == tb1_4)	szNearDrawMouseZENEISU_IMEONChar = _text;
				else if (tb == tb1_5)	szNearDrawMouseHANKANA_IMEONChar = _text;
				else if (tb == tb1_6)	szNearDrawMouseHANEISU_IMEONChar = _text;
				else if (tb == tb2_1)	szNearDrawCaretIMEOFFChar = _text;
				else if (tb == tb2_2)	szNearDrawCaretZENHIRA_IMEONChar = _text;
				else if (tb == tb2_3)	szNearDrawCaretZENKANA_IMEONChar = _text;
				else if (tb == tb2_4)	szNearDrawCaretZENEISU_IMEONChar = _text;
				else if (tb == tb2_5)	szNearDrawCaretHANKANA_IMEONChar = _text;
				else if (tb == tb2_6)	szNearDrawCaretHANEISU_IMEONChar = _text;
				else if (tb == tb3_1)	szNearDrawMouseByWndIMEOFFChar = _text;
				else if (tb == tb3_2)	szNearDrawMouseByWndZENHIRA_IMEONChar = _text;
				else if (tb == tb3_3)	szNearDrawMouseByWndZENKANA_IMEONChar = _text;
				else if (tb == tb3_4)	szNearDrawMouseByWndZENEISU_IMEONChar = _text;
				else if (tb == tb3_5)	szNearDrawMouseByWndHANKANA_IMEONChar = _text;
				else if (tb == tb3_6)	szNearDrawMouseByWndHANEISU_IMEONChar = _text;

				UpdateProfile(SETTINGSEX_SETTINGS_IMEMODE_SETREGISTRY);
				UpdateProfile(SETTINGSEX_RELOAD_MOUSECURSOR);
				SetColorButton();
			}
		}
		
		private void tb_GotFocus(object sender, RoutedEventArgs e)
		{
			if (e == null) { }
			TextBox tb = sender as TextBox;
			if (tb == null) { return; }
			tb.SelectAll();
		}

		public static string CheckStrings(string text)
        {
			int len = text.Length;
			string _text = "";
			if (len <= 1) _text = text;
			else if (len >= 2) {
				_text = text[..2];
				Encoding enc = Encoding.GetEncoding("Shift_JIS");
				if (enc.GetByteCount(_text) != _text.Length) {
					_text = text[..1];
				}
			}
			return _text;
        }
		
		private void SetColorButton()
		{
			if (Button1_1 == null)    Button1_1 = new Button();
			if (Button1_2 == null)    Button1_2 = new Button();
			if (Button1_3 == null)    Button1_3 = new Button();
			if (Button1_4 == null)    Button1_4 = new Button();
			if (Button1_5 == null)    Button1_5 = new Button();
			if (Button1_6 == null)    Button1_6 = new Button();

			if (Button2_1 == null)    Button2_1 = new Button();
			if (Button2_2 == null)    Button2_2 = new Button();
			if (Button2_3 == null)    Button2_3 = new Button();
			if (Button2_4 == null)    Button2_4 = new Button();
			if (Button2_5 == null)    Button2_5 = new Button();
			if (Button2_6 == null)    Button2_6 = new Button();

			if (Button3_1 == null)    Button3_1 = new Button();
			if (Button3_2 == null)    Button3_2 = new Button();
			if (Button3_3 == null)    Button3_3 = new Button();
			if (Button3_4 == null)    Button3_4 = new Button();
			if (Button3_5 == null)    Button3_5 = new Button();
			if (Button3_6 == null)    Button3_6 = new Button();

			SetColorButton(Button1_1, szNearDrawMouseIMEOFFChar, dwNearDrawMouseIMEOFFColor, szNearDrawMouseFont);
			SetColorButton(Button1_2, szNearDrawMouseZENHIRA_IMEONChar, dwNearDrawMouseZENHIRA_IMEONColor, szNearDrawMouseFont);
			SetColorButton(Button1_3, szNearDrawMouseZENKANA_IMEONChar, dwNearDrawMouseZENKANA_IMEONColor, szNearDrawMouseFont);
			SetColorButton(Button1_4, szNearDrawMouseZENEISU_IMEONChar, dwNearDrawMouseZENEISU_IMEONColor, szNearDrawMouseFont);
			SetColorButton(Button1_5, szNearDrawMouseHANKANA_IMEONChar, dwNearDrawMouseHANKANA_IMEONColor, szNearDrawMouseFont);
			SetColorButton(Button1_6, szNearDrawMouseHANEISU_IMEONChar, dwNearDrawMouseHANEISU_IMEONColor, szNearDrawMouseFont);

			SetColorButton(Button2_1, szNearDrawCaretIMEOFFChar, dwNearDrawCaretIMEOFFColor, szNearDrawCaretFont);
			SetColorButton(Button2_2, szNearDrawCaretZENHIRA_IMEONChar, dwNearDrawCaretZENHIRA_IMEONColor, szNearDrawCaretFont);
			SetColorButton(Button2_3, szNearDrawCaretZENKANA_IMEONChar, dwNearDrawCaretZENKANA_IMEONColor, szNearDrawCaretFont);
			SetColorButton(Button2_4, szNearDrawCaretZENEISU_IMEONChar, dwNearDrawCaretZENEISU_IMEONColor, szNearDrawCaretFont);
			SetColorButton(Button2_5, szNearDrawCaretHANKANA_IMEONChar, dwNearDrawCaretHANKANA_IMEONColor, szNearDrawCaretFont);
			SetColorButton(Button2_6, szNearDrawCaretHANEISU_IMEONChar, dwNearDrawCaretHANEISU_IMEONColor, szNearDrawCaretFont);

			SetColorButton(Button3_1, szNearDrawMouseByWndIMEOFFChar, dwNearDrawMouseByWndIMEOFFColor, szNearDrawMouseByWndFont);
			SetColorButton(Button3_2, szNearDrawMouseByWndZENHIRA_IMEONChar, dwNearDrawMouseByWndZENHIRA_IMEONColor, szNearDrawMouseByWndFont);
			SetColorButton(Button3_3, szNearDrawMouseByWndZENKANA_IMEONChar, dwNearDrawMouseByWndZENKANA_IMEONColor, szNearDrawMouseByWndFont);
			SetColorButton(Button3_4, szNearDrawMouseByWndZENEISU_IMEONChar, dwNearDrawMouseByWndZENEISU_IMEONColor, szNearDrawMouseByWndFont);
			SetColorButton(Button3_5, szNearDrawMouseByWndHANKANA_IMEONChar, dwNearDrawMouseByWndHANKANA_IMEONColor, szNearDrawMouseByWndFont);
			SetColorButton(Button3_6, szNearDrawMouseByWndHANEISU_IMEONChar, dwNearDrawMouseByWndHANEISU_IMEONColor, szNearDrawMouseByWndFont);
		}

		private static void SetColorButton(Button Bttn, String Chr, Int32 Clr, String Font)
		{
			Bttn.Content = Chr;
			Color clr = Color.FromArgb(Clr);
			Bttn.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			if (Font != null) Bttn.FontFamily = new FontFamily(Font);
		}
		
		unsafe private void button_Click(object sender, RoutedEventArgs e)
		{
			if (m_Sentinel == false) return;
			if (e == null) { }
			Button btn = sender as Button;
			if (btn != null) {
				if (btn.Name == "Button1_1")	  OpenColorPicker(sender, dwNearDrawMouseIMEOFFColor, ColorPicker1_1);
				else if (btn.Name == "Button1_2") OpenColorPicker(sender, dwNearDrawMouseZENHIRA_IMEONColor, ColorPicker1_2);
				else if (btn.Name == "Button1_3") OpenColorPicker(sender, dwNearDrawMouseZENKANA_IMEONColor, ColorPicker1_3);
				else if (btn.Name == "Button1_4") OpenColorPicker(sender, dwNearDrawMouseZENEISU_IMEONColor, ColorPicker1_4);
				else if (btn.Name == "Button1_5") OpenColorPicker(sender, dwNearDrawMouseHANKANA_IMEONColor, ColorPicker1_5);
				else if (btn.Name == "Button1_6") OpenColorPicker(sender, dwNearDrawMouseHANEISU_IMEONColor, ColorPicker1_6);
				else if (btn.Name == "Button2_1") OpenColorPicker(sender, dwNearDrawCaretIMEOFFColor, ColorPicker2_1);
				else if (btn.Name == "Button2_2") OpenColorPicker(sender, dwNearDrawCaretZENHIRA_IMEONColor, ColorPicker2_2);
				else if (btn.Name == "Button2_3") OpenColorPicker(sender, dwNearDrawCaretZENKANA_IMEONColor, ColorPicker2_3);
				else if (btn.Name == "Button2_4") OpenColorPicker(sender, dwNearDrawCaretZENEISU_IMEONColor, ColorPicker2_4);
				else if (btn.Name == "Button2_5") OpenColorPicker(sender, dwNearDrawCaretHANKANA_IMEONColor, ColorPicker2_5);
				else if (btn.Name == "Button2_6") OpenColorPicker(sender, dwNearDrawCaretHANEISU_IMEONColor, ColorPicker2_6);
				else if (btn.Name == "Button3_1") OpenColorPicker(sender, dwNearDrawMouseByWndIMEOFFColor, ColorPicker3_1);
				else if (btn.Name == "Button3_2") OpenColorPicker(sender, dwNearDrawMouseByWndZENHIRA_IMEONColor, ColorPicker3_2);
				else if (btn.Name == "Button3_3") OpenColorPicker(sender, dwNearDrawMouseByWndZENKANA_IMEONColor, ColorPicker3_3);
				else if (btn.Name == "Button3_4") OpenColorPicker(sender, dwNearDrawMouseByWndZENEISU_IMEONColor, ColorPicker3_4);
				else if (btn.Name == "Button3_5") OpenColorPicker(sender, dwNearDrawMouseByWndHANKANA_IMEONColor, ColorPicker3_5);
				else if (btn.Name == "Button3_6") OpenColorPicker(sender, dwNearDrawMouseByWndHANEISU_IMEONColor, ColorPicker3_6);
				else if (btn.Name == "CP_Button1_1_1")	dwNearDrawMouseIMEOFFColor = CloseColorPicker(Flyout1_1, Button1_1, dwNearDrawMouseIMEOFFColor, ColorPicker1_1);
				else if (btn.Name == "CP_Button1_1_2") Flyout1_1.Hide();
				else if (btn.Name == "CP_Button1_2_1") dwNearDrawMouseZENHIRA_IMEONColor = CloseColorPicker(Flyout1_2, Button1_2, dwNearDrawMouseZENHIRA_IMEONColor, ColorPicker1_2);
				else if (btn.Name == "CP_Button1_2_2") Flyout1_2.Hide();
				else if (btn.Name == "CP_Button1_3_1") dwNearDrawMouseZENKANA_IMEONColor = CloseColorPicker(Flyout1_3, Button1_3, dwNearDrawMouseZENKANA_IMEONColor, ColorPicker1_3);
				else if (btn.Name == "CP_Button1_3_2") Flyout1_3.Hide();
				else if (btn.Name == "CP_Button1_4_1") dwNearDrawMouseZENEISU_IMEONColor = CloseColorPicker(Flyout1_4, Button1_4, dwNearDrawMouseZENEISU_IMEONColor, ColorPicker1_4);
				else if (btn.Name == "CP_Button1_4_2") Flyout1_4.Hide();
				else if (btn.Name == "CP_Button1_5_1") dwNearDrawMouseHANKANA_IMEONColor = CloseColorPicker(Flyout1_5, Button1_5, dwNearDrawMouseHANKANA_IMEONColor, ColorPicker1_5);
				else if (btn.Name == "CP_Button1_5_2") Flyout1_5.Hide();
				else if (btn.Name == "CP_Button1_6_1") dwNearDrawMouseHANEISU_IMEONColor = CloseColorPicker(Flyout1_6, Button1_6, dwNearDrawMouseHANEISU_IMEONColor, ColorPicker1_6);
				else if (btn.Name == "CP_Button1_6_2") Flyout1_6.Hide();
				else if (btn.Name == "CP_Button2_1_1") dwNearDrawCaretIMEOFFColor = CloseColorPicker(Flyout2_1, Button2_1, dwNearDrawCaretIMEOFFColor, ColorPicker2_1);
				else if (btn.Name == "CP_Button2_1_2") Flyout2_1.Hide();
				else if (btn.Name == "CP_Button2_2_1") dwNearDrawCaretZENHIRA_IMEONColor = CloseColorPicker(Flyout2_2, Button2_2, dwNearDrawCaretZENHIRA_IMEONColor, ColorPicker2_2);
				else if (btn.Name == "CP_Button2_2_2") Flyout2_2.Hide();
				else if (btn.Name == "CP_Button2_3_1") dwNearDrawCaretZENKANA_IMEONColor = CloseColorPicker(Flyout2_3, Button2_3, dwNearDrawCaretZENKANA_IMEONColor, ColorPicker2_3);
				else if (btn.Name == "CP_Button2_3_2") Flyout2_3.Hide();
				else if (btn.Name == "CP_Button2_4_1") dwNearDrawCaretZENEISU_IMEONColor = CloseColorPicker(Flyout2_4, Button2_4, dwNearDrawCaretZENEISU_IMEONColor, ColorPicker2_4);
				else if (btn.Name == "CP_Button2_4_2") Flyout2_4.Hide();
				else if (btn.Name == "CP_Button2_5_1") dwNearDrawCaretHANKANA_IMEONColor = CloseColorPicker(Flyout2_5, Button2_5, dwNearDrawCaretHANKANA_IMEONColor, ColorPicker2_5);
				else if (btn.Name == "CP_Button2_5_2") Flyout2_5.Hide();
				else if (btn.Name == "CP_Button2_6_1") dwNearDrawCaretHANEISU_IMEONColor = CloseColorPicker(Flyout2_6, Button2_6, dwNearDrawCaretHANEISU_IMEONColor, ColorPicker2_6);
				else if (btn.Name == "CP_Button2_6_2") Flyout2_6.Hide();
				else if (btn.Name == "CP_Button3_1_1") dwNearDrawMouseByWndIMEOFFColor = CloseColorPicker(Flyout3_1, Button3_1, dwNearDrawMouseByWndIMEOFFColor, ColorPicker3_1);
				else if (btn.Name == "CP_Button3_1_2") Flyout3_1.Hide();
				else if (btn.Name == "CP_Button3_2_1") dwNearDrawMouseByWndZENHIRA_IMEONColor = CloseColorPicker(Flyout3_2, Button3_2, dwNearDrawMouseByWndZENHIRA_IMEONColor, ColorPicker3_2);
				else if (btn.Name == "CP_Button3_2_2") Flyout3_2.Hide();
				else if (btn.Name == "CP_Button3_3_1") dwNearDrawMouseByWndZENKANA_IMEONColor = CloseColorPicker(Flyout3_3, Button3_3, dwNearDrawMouseByWndZENKANA_IMEONColor, ColorPicker3_3);
				else if (btn.Name == "CP_Button3_3_2") Flyout3_3.Hide();
				else if (btn.Name == "CP_Button3_4_1") dwNearDrawMouseByWndZENEISU_IMEONColor = CloseColorPicker(Flyout3_4, Button3_4, dwNearDrawMouseByWndZENEISU_IMEONColor, ColorPicker3_4);
				else if (btn.Name == "CP_Button3_4_2") Flyout3_4.Hide();
				else if (btn.Name == "CP_Button3_5_1") dwNearDrawMouseByWndHANKANA_IMEONColor = CloseColorPicker(Flyout3_5, Button3_5, dwNearDrawMouseByWndHANKANA_IMEONColor, ColorPicker3_5);
				else if (btn.Name == "CP_Button3_5_2") Flyout3_5.Hide();
				else if (btn.Name == "CP_Button3_6_1") dwNearDrawMouseByWndHANEISU_IMEONColor = CloseColorPicker(Flyout3_6, Button3_6, dwNearDrawMouseByWndHANEISU_IMEONColor, ColorPicker3_6);
				else if (btn.Name == "CP_Button3_6_2") Flyout3_6.Hide();
				
				UpdateProfile(SETTINGSEX_SETTINGS_IMEMODE_SETREGISTRY);
				UpdateProfile(SETTINGSEX_RELOAD_MOUSECURSOR);
				SetTextBox();
				EnableDisableItems(sender, e);
			}
		}

		private static void OpenColorPicker(object sender, Int32 color, ColorPicker colorPicker)
		{
			Color clr = Color.FromArgb(color);
			byte A = (byte)(255 - clr.A);
			byte R = (byte)(clr.B);
			byte G = (byte)(clr.G);
			byte B = (byte)(clr.R);
			colorPicker.Color = Windows.UI.Color.FromArgb(A, R, G, B);
			colorPicker.PreviousColor = colorPicker.Color;
			FlyoutBase.ShowAttachedFlyout((FrameworkElement)sender);
		}
		
		private static Int32 CloseColorPicker(Flyout flyout, Button button, Int32 color, ColorPicker colorPicker)
		{
			Windows.UI.Color wclr = colorPicker.Color;
			byte A = (byte)(255 - wclr.A);
			byte R = wclr.B;
			byte G = wclr.G;
			byte B = wclr.R;
			Int32 iRet = (A << 24) ^ (R << 16) ^ (G << 8) ^ (B << 0);
			flyout.Hide();
			if (iRet != color) {
				Color clr = Color.FromArgb(iRet);
				button.Foreground = new SolidColorBrush(Windows.UI.Color.FromArgb((byte)(255 - clr.A), clr.B, clr.G, clr.R));
			}
			return iRet;
		}
	}
}


/* = EOF = */