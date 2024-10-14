// pch.h: プリコンパイル済みヘッダー ファイルです。
// 次のファイルは、その後のビルドのビルド パフォーマンスを向上させるため 1 回だけコンパイルされます。
// コード補完や多くのコード参照機能などの IntelliSense パフォーマンスにも影響します。
// ただし、ここに一覧表示されているファイルは、ビルド間でいずれかが更新されると、すべてが再コンパイルされます。
// 頻繁に更新するファイルをここに追加しないでください。追加すると、パフォーマンス上の利点がなくなります。

#ifndef PCH_H
#define PCH_H

// プリコンパイルするヘッダーをここに追加します
#define OEMRESOURCE
#include "framework.h"
#include "targetver.h"
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

#include <windowsx.h>
#include <tchar.h>
#include <shellapi.h>
#include <shellscalingapi.h>
#pragma comment(lib, "Shcore.lib")
#include <imm.h>
#include <ime_cmodes.h>
#pragma comment(lib, "Imm32.lib")
#include <oleacc.h>
#pragma comment(lib, "Oleacc.lib")
#ifndef USHORT
typedef unsigned short USHORT;
#endif
#include <hidusage.h>
#include <hidpi.h>
#pragma comment(lib, "hid.lib")
#include <cstdlib>
#include <stdio.h>
#include <future>

#include <psapi.h>
#include <strsafe.h>

#endif //PCH_H
