#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <cderr.h>
#include <dde.h>
#include <ddeml.h>
#include <dlgs.h>
#ifndef _MAC
#include <lzexpand.h>
#include <mmsystem.h>
#include <nb30.h>
#include <rpc.h>
#endif
#include <shellapi.h>
#ifndef _MAC
#include <winperf.h>
#endif
#ifndef NOCRYPT
#include <wincrypt.h>
#include <winefs.h>
#include <winscard.h>
#endif
#ifndef NOGDI
#ifndef _MAC
#include <winspool.h>
#ifdef INC_OLE1
#include <ole.h>
#else
#include <ole2.h>
#endif /* !INC_OLE1 */
#endif /* !MAC */
#include <commdlg.h>
#endif /* !NOGDI */
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

#include "winrt/Microsoft.Windows.AppLifecycle.h"
#include "winrt/Microsoft.UI.Xaml.XamlTypeInfo.h"
#include "winrt/FlushMouseUI3DLL.h"
#include "winrt/FlushMouseUI3DLL.FlushMouseUI3DLL_XamlTypeInfo.h"

#pragma pop_macro("GetCurrentTime")

/* = EOF = */