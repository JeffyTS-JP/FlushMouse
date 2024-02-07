//
// WinRT.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/02084  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime

#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.ApplicationModel.Activation.h"
#include "winrt/Windows.Storage.h"

#include "winrt/Microsoft.UI.Xaml.Data.h"
#include "winrt/Microsoft.UI.Xaml.Markup.h"

#include "winrt/Microsoft.UI.Dispatching.h"
#include "winrt/Microsoft.UI.Interop.h"
#include "winrt/Microsoft.UI.Windowing.h"

#include "Microsoft.UI.Xaml.Window.h"

#pragma pop_macro("GetCurrentTime")

/* = EOF = */