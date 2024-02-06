#pragma once
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <sstream>
#include <strsafe.h>

#include <shellscalingapi.h>
#include <ShObjIdl.h>

// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime

#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.ApplicationModel.Activation.h"
#include "winrt/Windows.Storage.h"

#include "winrt/Microsoft.UI.Xaml.h"
#include "winrt/Microsoft.UI.Xaml.Controls.h"
#include "winrt/Microsoft.UI.Xaml.Controls.Primitives.h"
#include "winrt/Microsoft.UI.Xaml.Data.h"
#include "winrt/Microsoft.UI.Xaml.Markup.h"
#include "winrt/Microsoft.UI.Interop.h"						// For the WindowId struct and the GetWindowIdFromWindow function.
#include "winrt/Microsoft.UI.Dispatching.h"
#include "winrt/Microsoft.UI.Windowing.h"					// For AppWindow::GetFromWindowId

#include "Microsoft.UI.Xaml.Window.h"						// For the IWindowNative interface.

#include "winrt/Microsoft.Windows.AppLifecycle.h"


#include "winrt/Microsoft.UI.Xaml.XamlTypeInfo.h"
#include "winrt/FlushMouseUI3DLL.h"
#include "winrt/FlushMouseUI3DLL.FlushMouseUI3DLL_XamlTypeInfo.h"

#pragma pop_macro("GetCurrentTime")

/* = EOF = */