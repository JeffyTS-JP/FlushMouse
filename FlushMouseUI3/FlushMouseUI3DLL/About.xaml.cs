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
using System.Net.Http;
using System.Net.Http.Headers;
using System.Reflection;
using System.Threading.Tasks;
using static FlushMouseUI3DLL.Settings;

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

		private async void Button_Click(object sender, RoutedEventArgs e)
		{
			if (e == null) { }
			Button btn = sender as Button;
			if (btn == null) { return; }
			if (btn.Name == "btn1") {
				string urlExperimentGroups = "JeffyTS-JP/FlushMouse/releases/latest";
				Task<string> task = Task<string>.Run(() => GetReleaseVersionOnGitHub(urlExperimentGroups));
				string result = await task;
				if (result != null) {
					string tag = "\"tag_name\":\"";
					int foundIndex1 = result.IndexOf(tag, StringComparison.OrdinalIgnoreCase);
					int nextIndex = foundIndex1 + tag.Length;
					if (nextIndex < result.Length) {
						int foundIndex2 = result.IndexOf("\",", nextIndex);
						string gitVersion = "";
						for (int i = nextIndex; i < foundIndex2; i++) {
							gitVersion += result[i].ToString();
						}
						Assembly assembly = Assembly.GetExecutingAssembly();
						AssemblyFileVersionAttribute asmFileVersion = (AssemblyFileVersionAttribute)Attribute.GetCustomAttribute(assembly, typeof(AssemblyFileVersionAttribute));
						string currentVersion = asmFileVersion.Version;
						if (Compare(gitVersion, currentVersion) >= 1) {
							try {
								String lpCaption = "FlushMouse";
								String lpText = "FlushMouse の新しいバージョンがあります\nVer. " + gitVersion;
								MessageBox(g_hMainWnd, lpText, lpCaption, (MB_OK | MB_ICONINFORMATION| MB_TOPMOST));
								return;
							}
							catch (Exception) {
							}
						}
						else {
							try {
								String lpCaption = "FlushMouse";
								String lpText = "FlushMouse の新しいバージョンは見つかりませんでした";
								MessageBox(g_hMainWnd, lpText, lpCaption, (MB_OK | MB_ICONINFORMATION| MB_TOPMOST));
								return;
							}
							catch (Exception) {
							}
						}
					}
				}
			}
		}

		private static int Compare(string current, string target)
		{
			if (string.IsNullOrEmpty(current) || string.IsNullOrEmpty(target)) {
				return 0;
			}
			try {
				var currentVersion = new System.Version(current);
				var targetVersion = new System.Version(target);
				return currentVersion.CompareTo(targetVersion);
			}
			catch (Exception) {
				return 0;
			}
		}

		static readonly HttpClient client = new();

		static async Task<string> GetReleaseVersionOnGitHub(string urlExperimentGroups)
		{
			if (urlExperimentGroups == null) { return null; }
			try {
				client.DefaultRequestHeaders.Clear();
				if (client.BaseAddress == null) client.BaseAddress = new Uri(@"https://api.github.com/repos/");
				if (client.DefaultRequestHeaders == null) {
					client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
				}
				client.DefaultRequestHeaders.Add("User-Agent", "node.js");
				using HttpResponseMessage response = await client.GetAsync(urlExperimentGroups);
				response.EnsureSuccessStatusCode();
				string responseBody = await response.Content.ReadAsStringAsync();
				return responseBody;
			}
			catch (HttpRequestException) {
			}
			return null;
		}
	}
}

/* = EOF = */
