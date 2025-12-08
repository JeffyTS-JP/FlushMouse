//
// CWindow.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/02/13  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "CWindow.h"

//
// Define
//
constexpr ULONGLONG CACHE_VALIDATE_MS = 5000;
constexpr ULONGLONG CLEANUP_THRESHOLD_MS = 30000;
constexpr size_t MAX_CACHE_SIZE = 100;

//
// Struct Define
//
typedef struct tagCacheEntry {
	HWND		hWnd;
	ULONGLONG	lastCheckedTick;
	ULONGLONG	lastAccessTick;
} CACHEENTRY, *pCACHEENTRY, *lpCACHEENTRY;

typedef struct tagCacheKey {
	HWND		hWndParent;
	std::basic_string<TCHAR> lpszClassName;

	bool operator==(const tagCacheKey& other) const {
		return hWndParent == other.hWndParent && lpszClassName == other.lpszClassName;
	}
} CACHEKEY, *pCACHEKEY, *lpCACHEKEY;

namespace std {
	template<>
	struct hash<CACHEKEY> {
		size_t operator()(const CACHEKEY& key) const {
			size_t h1 = std::hash<HWND>{}(key.hWndParent);
			size_t h2 = std::hash<std::basic_string<TCHAR>>{}(key.lpszClassName);
			return h1 ^ (h2 << 1);
		}
	};
}

//
// Local Data
//
static SRWLOCK						CacheLock = SRWLOCK_INIT;
static BOOL							bCacheInitialized = FALSE;
static std::unordered_map<CACHEKEY, CACHEENTRY>* lpszCacheMap = nullptr;
static std::unordered_map<CACHEKEY, CACHEENTRY>* lpszCacheExMap = nullptr;

//
// Local Prototype Define
//
static inline void	vCleanupOldCacheEntries();
static inline void	vCleanupOldCacheExEntries();

//
//class CWindow
//
CWindow::CWindow()
	: _hInstance(NULL), _hWnd(NULL), _lpWndClass(nullptr), _lpWndClassEx(nullptr)
{
}

CWindow::~CWindow()
{
	if (_lpWndClassEx)	delete _lpWndClassEx;
	_lpWndClassEx = NULL;
	if (_lpWndClass)	delete _lpWndClass;
	_lpWndClass = NULL;

	_hInstance = NULL;
	_hWnd = NULL;
}

//
// bRegisterClass()
//
BOOL	CWindow::bRegisterClass(_In_ LPWNDCLASS lpWndClass)
{
	if (!lpWndClass || !lpWndClass->hInstance)	return FALSE;
	_hInstance = lpWndClass->hInstance;

	_lpWndClass = new WNDCLASS;
	if (_lpWndClass == NULL)	return FALSE;
	memcpy(_lpWndClass, lpWndClass, sizeof(WNDCLASS));
	_lpWndClass->lpfnWndProc = (WNDPROC)_WndProc;
	ATOM atom = RegisterClass(_lpWndClass);
	if (!atom) {
		if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
			delete _lpWndClass;
			_lpWndClass = NULL;
			return FALSE;
		}
	}
	return TRUE;
}

//
// bRegisterClassEx()
//
BOOL	CWindow::bRegisterClassEx(_In_ LPWNDCLASSEX lpWndClassEx)
{
	if (!lpWndClassEx || !lpWndClassEx->hInstance)	return FALSE;
	_hInstance = lpWndClassEx->hInstance;

	_lpWndClassEx = new WNDCLASSEX;
	if (_lpWndClassEx == NULL)	return FALSE;
	memcpy(_lpWndClassEx, lpWndClassEx, sizeof(WNDCLASSEX));
	_lpWndClassEx->lpfnWndProc = (WNDPROC)_WndProc;

	ATOM atom = RegisterClassEx(_lpWndClassEx);
	if (!atom) {
		if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
			delete _lpWndClassEx;
			_lpWndClassEx = NULL;
			return FALSE;
		}
	}
	return TRUE;
}

//
// bCreateWindow()
//
BOOL	CWindow::bCreateWindow(_In_opt_ LPCTSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y,
	_In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu)
{
	if (!_lpWndClass || !_lpWndClass->lpszClassName)	return FALSE;

	_hWnd = CreateWindow(_lpWndClass->lpszClassName, lpWindowName, dwStyle, 
		X, Y, nWidth, nHeight, hWndParent, hMenu, _hInstance, this);
	return (_hWnd != NULL);
}

//
// bCreateWindowEx()
//
BOOL	CWindow::bCreateWindowEx(_In_ DWORD dwExStyle, _In_opt_ LPCTSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y,
	_In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu)
{
	if (!_lpWndClassEx || !_lpWndClassEx->lpszClassName)	return FALSE;

	_hWnd = CreateWindowEx(dwExStyle, _lpWndClassEx->lpszClassName, lpWindowName, dwStyle,
								X, Y, nWidth, nHeight, hWndParent, hMenu, _hInstance, this);
	return (_hWnd != NULL);
}

//
// _WndProc()
//
LRESULT CALLBACK CWindow::_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CWindow* pThis = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	
	if (!pThis && message == WM_CREATE) {
		LPCREATESTRUCT lpCreate = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = reinterpret_cast<CWindow*>(lpCreate->lpCreateParams);
		if (pThis) {
			pThis->vSetWindowLongPtr(hWnd);
		}
	}
	
	if (pThis && pThis->_hWnd == hWnd) {
		return pThis->WndProc(hWnd, message, wParam, lParam);
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// vSetWindowLongPtr()
//
void	CWindow::vSetWindowLongPtr(HWND hWnd)
{
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	_hWnd = hWnd;
}

//
// WndProc()
//
LRESULT CWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const CWindow* This = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (This != NULL) {
		switch (message) {
			case WM_CREATE:
				break;
			case WM_DESTROY:
				break;
			default:
				break;
		}
	}
	else {
		if (message == WM_CREATE) {
			This = reinterpret_cast<CWindow*>((reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams));
		}
		if (This != NULL) {
			vSetWindowLongPtr(hWnd);
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// bUpdateWindow()
//
BOOL	CWindow::bUpdateWindow() const
{
	return _hWnd ? UpdateWindow(_hWnd) : FALSE;
}

//
// bShowWindow()
//
BOOL	CWindow::bShowWindow(_In_ int nCmdShow) const
{
	if (!_hWnd)	return FALSE;
	ShowWindow(_hWnd, nCmdShow);
	return TRUE;
}

//
// bSetWindowText()
//
BOOL	CWindow::bSetWindowText(_In_opt_ LPCTSTR szTitle) const
{
	return (_hWnd && szTitle) ? SetWindowText(_hWnd, szTitle) : FALSE;
}

//
// bSetWindowPos()
//
BOOL	CWindow::bSetWindowPos(_In_opt_ HWND hWndInsertAfter, _In_ int X, _In_ int Y, _In_ int cx, _In_ int cy, _In_ UINT uFlags) const
{
	return _hWnd ? SetWindowPos(_hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags) : FALSE;
}

//
// bInvalidateRect()
//
BOOL	CWindow::bInvalidateRect(_In_opt_ LPCRECT lpRect, _In_ BOOL bErase) const
{
	return _hWnd ? InvalidateRect(_hWnd, lpRect, bErase) : FALSE;
}

//
// bSetWndThreadPriority()
//
BOOL	CWindow::bSetWndThreadPriority(_In_ int nPriority) const
{
	return _hWnd ? (SetThreadPriority(GetCurrentThread(), nPriority) != 0) : FALSE;
}

//
// bInitializeWindowCache()
//
BOOL	bInitializeWindowCache()
{
	if (bCacheInitialized)	return TRUE;
	AcquireSRWLockExclusive(&CacheLock);
	if (!bCacheInitialized) {
		try {
			lpszCacheMap = new std::unordered_map<CACHEKEY, CACHEENTRY>();
			lpszCacheMap->reserve(MAX_CACHE_SIZE);
			lpszCacheExMap = new std::unordered_map<CACHEKEY, CACHEENTRY>();
			lpszCacheExMap->reserve(MAX_CACHE_SIZE);
			bCacheInitialized = TRUE;
		}
		catch (...) {
			if (lpszCacheMap) {
				delete lpszCacheMap;
				lpszCacheMap = nullptr;
			}
			if (lpszCacheExMap) {
				delete lpszCacheExMap;
				lpszCacheExMap = nullptr;
			}
			ReleaseSRWLockExclusive(&CacheLock);
			return FALSE;
		}
	}
	ReleaseSRWLockExclusive(&CacheLock);
	return TRUE;
}

//
// vUninitializeWindowCache()
//
void	vUninitializeWindowCache()
{
	if (!bCacheInitialized)	return;
	AcquireSRWLockExclusive(&CacheLock);
	if (lpszCacheMap) {
		delete lpszCacheMap;
		lpszCacheMap = nullptr;
	}
	if (lpszCacheExMap) {
		delete lpszCacheExMap;
		lpszCacheExMap = nullptr;
	}
	bCacheInitialized = FALSE;
	ReleaseSRWLockExclusive(&CacheLock);
}

//
// hGetCachedWindowByClassName()
//
HWND	hGetCachedWindowByClassName(_In_z_ LPCTSTR lpszClassName)
{
	if (!lpszClassName || !bCacheInitialized || !lpszCacheMap)	return NULL;

	CACHEKEY searchKey {};
	searchKey.hWndParent = NULL;
	searchKey.lpszClassName = lpszClassName;
	ULONGLONG now = GetTickCount64();
	HWND hCached = NULL;
	BOOL bCacheHit = FALSE;
	BOOL bNeedAccessUpdate = FALSE;

	AcquireSRWLockShared(&CacheLock);
	auto it = lpszCacheMap->find(searchKey);
	if (it != lpszCacheMap->end()) {
		ULONGLONG age = now - it->second.lastCheckedTick;
		hCached = it->second.hWnd;
		if (age < CACHE_VALIDATE_MS) {
			BOOL bValid = TRUE;
			if (hCached != NULL) {
				BOOL isWindowResult = IsWindow(hCached);
				if (isWindowResult) {
					DWORD dwProcessId = 0;
					DWORD dwThreadId = GetWindowThreadProcessId(hCached, &dwProcessId);
					if (dwThreadId == 0) {
						bValid = FALSE;
					}
				}
				else {
					bValid = FALSE;
				}
			}
			if (bValid) {
				bCacheHit = TRUE;
				if ((now - it->second.lastAccessTick) > CLEANUP_THRESHOLD_MS) {
					bNeedAccessUpdate = TRUE;
				}
			}
		}
	}
	ReleaseSRWLockShared(&CacheLock);

	if (bCacheHit) {
		if (bNeedAccessUpdate) {
			AcquireSRWLockExclusive(&CacheLock);
			if (lpszCacheMap) {
				auto it2 = lpszCacheMap->find(searchKey);
				if (it2 != lpszCacheMap->end()) {
					it2->second.lastAccessTick = now;
				}
			}
			ReleaseSRWLockExclusive(&CacheLock);
		}
		return hCached;
	}
	HWND hResult = FindWindow(lpszClassName, NULL);
	AcquireSRWLockExclusive(&CacheLock);
	if (lpszCacheMap) {
		CACHEENTRY entry {};
		entry.hWnd = hResult;
		entry.lastCheckedTick = now;
		entry.lastAccessTick = now;
		(*lpszCacheMap)[searchKey] = entry;
		if (lpszCacheMap->size() > MAX_CACHE_SIZE) {
			vCleanupOldCacheEntries();
		}
	}
	ReleaseSRWLockExclusive(&CacheLock);
	return hResult;
}

//
// hGetCachedWindowExByClassName()
//
HWND	hGetCachedWindowExByClassName(_In_opt_ HWND hWndParent, _In_opt_ HWND hWndChildAfter, _In_z_ LPCTSTR lpszClassName)
{
	if (!lpszClassName || !bCacheInitialized || !lpszCacheExMap)	return NULL;

	if (hWndChildAfter != NULL)	return FindWindowEx(hWndParent, hWndChildAfter, lpszClassName, NULL);

	CACHEKEY searchKey {};
	searchKey.hWndParent = hWndParent;
	searchKey.lpszClassName = lpszClassName;
	ULONGLONG now = GetTickCount64();
	HWND hCached = NULL;
	BOOL bCacheHit = FALSE;
	BOOL bNeedAccessUpdate = FALSE;

	AcquireSRWLockShared(&CacheLock);
	auto it = lpszCacheExMap->find(searchKey);
	if (it != lpszCacheExMap->end()) {
		ULONGLONG age = now - it->second.lastCheckedTick;
		hCached = it->second.hWnd;
		if (age < CACHE_VALIDATE_MS) {
			BOOL bValid = TRUE;
			if (hCached != NULL) {
				BOOL isWindowResult = IsWindow(hCached);
				if (isWindowResult) {
					DWORD dwProcessId = 0;
					DWORD dwThreadId = GetWindowThreadProcessId(hCached, &dwProcessId);
					if (dwThreadId == 0) {
						bValid = FALSE;
					}
				}
				else {
					bValid = FALSE;
				}
			}
			if (bValid) {
				bCacheHit = TRUE;
				if ((now - it->second.lastAccessTick) > CLEANUP_THRESHOLD_MS) {
					bNeedAccessUpdate = TRUE;
				}
			}
		}
	}
	ReleaseSRWLockShared(&CacheLock);

	if (bCacheHit) {
		if (bNeedAccessUpdate) {
			AcquireSRWLockExclusive(&CacheLock);
			if (lpszCacheExMap) {
				auto it2 = lpszCacheExMap->find(searchKey);
				if (it2 != lpszCacheExMap->end()) {
					it2->second.lastAccessTick = now;
				}
			}
			ReleaseSRWLockExclusive(&CacheLock);
		}
		return hCached;
	}
	HWND hResult = FindWindowEx(hWndParent, NULL, lpszClassName, NULL);
	AcquireSRWLockExclusive(&CacheLock);
	if (lpszCacheExMap) {
		CACHEENTRY entry {};
		entry.hWnd = hResult;
		entry.lastCheckedTick = now;
		entry.lastAccessTick = now;
		(*lpszCacheExMap)[searchKey] = entry;
		if (lpszCacheExMap->size() > MAX_CACHE_SIZE) {
			vCleanupOldCacheExEntries();
		}
	}
	ReleaseSRWLockExclusive(&CacheLock);
	return hResult;
}

//
// vCleanupOldCacheEntries()
//
static inline void	vCleanupOldCacheEntries()
{
	if (!lpszCacheMap || lpszCacheMap->size() <= MAX_CACHE_SIZE)	return;

	size_t removeCount = lpszCacheMap->size() - MAX_CACHE_SIZE + 10;
	std::vector<std::pair<CACHEKEY, ULONGLONG>> entries;
	entries.reserve(lpszCacheMap->size());
	std::transform(lpszCacheMap->begin(), lpszCacheMap->end(), std::back_inserter(entries),
				   [](const auto& pair) {
					   return std::make_pair(pair.first, pair.second.lastAccessTick);
				   });
	std::partial_sort(entries.begin(), entries.begin() + removeCount, entries.end(),
					  [](const auto& a, const auto& b) { return a.second < b.second; });
	for (size_t i = 0; i < removeCount && i < entries.size(); ++i) {
		lpszCacheMap->erase(entries[i].first);
	}
}

//
// vCleanupOldCacheExEntries()
//
static inline void	vCleanupOldCacheExEntries()
{
	if (!lpszCacheExMap || lpszCacheExMap->size() <= MAX_CACHE_SIZE)	return;

	// LRU方式：最もアクセスされていないエントリを削除
	size_t removeCount = lpszCacheExMap->size() - MAX_CACHE_SIZE + 10;
	std::vector<std::pair<CACHEKEY, ULONGLONG>> entries;
	entries.reserve(lpszCacheExMap->size());
	std::transform(lpszCacheExMap->begin(), lpszCacheExMap->end(), std::back_inserter(entries),
				   [](const auto& pair) {
					   return std::make_pair(pair.first, pair.second.lastAccessTick);
				   });
	std::partial_sort(entries.begin(), entries.begin() + removeCount, entries.end(),
					  [](const auto& a, const auto& b) { return a.second < b.second; });
	for (size_t i = 0; i < removeCount && i < entries.size(); ++i) {
		lpszCacheExMap->erase(entries[i].first);
	}
}

//
// vInvalidateWindowCache()
//
void	vInvalidateWindowCache(_In_z_ LPCTSTR lpszClassName)
{
	if (!lpszClassName || !bCacheInitialized || !lpszCacheMap)	return;

	CACHEKEY key {};
	key.hWndParent = NULL;
	key.lpszClassName = lpszClassName;
	AcquireSRWLockExclusive(&CacheLock);
	if (lpszCacheMap) {
		lpszCacheMap->erase(key);
	}
	ReleaseSRWLockExclusive(&CacheLock);
}

//
// vInvalidateWindowExCache()
//
void	vInvalidateWindowExCache(_In_opt_ HWND hWndParent, _In_z_ LPCTSTR lpszClassName)
{
	if (!lpszClassName || !bCacheInitialized || !lpszCacheExMap)	return;

	CACHEKEY key {};
	key.hWndParent = hWndParent;
	key.lpszClassName = lpszClassName;
	AcquireSRWLockExclusive(&CacheLock);
	if (lpszCacheExMap) {
		lpszCacheExMap->erase(key);
	}
	ReleaseSRWLockExclusive(&CacheLock);
}

//
// vInvalidateAllWindowCache()
//
void	vInvalidateAllWindowCache()
{
	if (!bCacheInitialized)	return;

	AcquireSRWLockExclusive(&CacheLock);
	if (lpszCacheMap) {
		lpszCacheMap->clear();
	}
	if (lpszCacheExMap) {
		lpszCacheExMap->clear();
	}
	ReleaseSRWLockExclusive(&CacheLock);
}


/* = EOF = */
