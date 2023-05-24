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
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <shellapi.h>
#include <shellscalingapi.h>
#pragma comment(lib, "Shcore.lib")
	#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")
#include <imm.h>
#include <ime_cmodes.h>
#pragma comment(lib, "Imm32.lib")

#endif //PCH_H
