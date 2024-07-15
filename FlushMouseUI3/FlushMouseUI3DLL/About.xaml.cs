// About.Xaml.cs for FlushMouseUI3DLL
//      Copyright (C) 2024 JeffyTS
//
//    
// No.    Date          Name            Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000    2024/07/15  JeffyTS     New edit.
//

using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System;
using System.Diagnostics;
using System.Reflection;

namespace FlushMouseUI3DLL {
	public sealed partial class About : Page {

		public About()
		{
			InitializeComponent();
		}

		private void About_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
			SetTextBlock();
		}

		private void SetTextBlock()
		{
			Assembly assembly = Assembly.GetExecutingAssembly();
			AssemblyProductAttribute asmProduct = (AssemblyProductAttribute)Attribute.GetCustomAttribute(assembly, typeof(AssemblyProductAttribute));

			tb1.Text = asmProduct.Product;
		}

		private void Copyright_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }
			DateTime currentDay = DateTime.Now;
			Copyright.Text = currentDay.Year.ToString();
		}
		
		private void Version_Loaded(object sender, RoutedEventArgs e)
		{
			if (sender == null) { }
			if (e == null) { }

			Assembly assembly = Assembly.GetExecutingAssembly();
			AssemblyFileVersionAttribute asmFileVersion = (AssemblyFileVersionAttribute)Attribute.GetCustomAttribute(assembly, typeof(AssemblyFileVersionAttribute));
			
			Version.Text = asmFileVersion.Version;
		}
	}
}
