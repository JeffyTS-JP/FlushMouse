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

#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime

#include "winrt/Microsoft.UI.Xaml.h"
#include "winrt/Microsoft.UI.Xaml.Controls.h"
#include "winrt/Microsoft.UI.Xaml.Controls.Primitives.h"
#include "winrt/Microsoft.UI.Xaml.XamlTypeInfo.h"

#include "winrt/Microsoft.Windows.AppLifecycle.h"

#include "winrt/FlushMouseUI3DLL.h"
#include "winrt/FlushMouseUI3DLL.FlushMouseUI3DLL_XamlTypeInfo.h"

#pragma pop_macro("GetCurrentTime")

/* = EOF = */