// CRegstry.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	022/05/23  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "CRegistry.h"

//
// Define
// 

//
// class CRegistry
//
CRegistry::CRegistry()
{
}

CRegistry::~CRegistry()
{
}

//
// bGetRegValueDWORD()
//
BOOL		CRegistry::bGetRegValueDWORD(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData, DWORD dwInitialData)
{
	LSTATUS	lStatus = 0;
	HKEY		hKeySub = 0;
	BOOL		bRet = FALSE;
	DWORD	dwDisposition = 0;
	DWORD	dwLen = sizeof(DWORD);
	DWORD	dwData = dwInitialData;
	if ((lStatus = RegCreateKeyEx(hkey, lpszSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeySub, &dwDisposition)) == ERROR_SUCCESS) {
		if (dwDisposition == KEY_QUERY_VALUE) {	
			if ((lStatus = RegSetValueEx(hKeySub, szValue, NULL, REG_DWORD, reinterpret_cast<BYTE*>(&dwData), dwLen)) == ERROR_SUCCESS) {
				bRet = TRUE;
			}
		}
		else if (dwDisposition == REG_OPENED_EXISTING_KEY) {	
			if ((lStatus = RegQueryValueEx(hKeySub, szValue, NULL, NULL, (LPBYTE)&dwData, &dwLen)) == ERROR_SUCCESS) {
				bRet = TRUE;
			}
			else if (lStatus == ERROR_FILE_NOT_FOUND) {
				if ((lStatus = RegSetValueEx(hKeySub, szValue, NULL, REG_DWORD, reinterpret_cast<BYTE*>(&dwData), dwLen)) == ERROR_SUCCESS) {
					bRet = TRUE;
				}
			}
			else {
			}
		}
		else {
		}
		RegCloseKey(hKeySub);
	}
	*lpdwData = dwData;
	return	bRet;
}

//
// bReadRegValueDWORD()
//
BOOL		CRegistry::bReadRegValueDWORD(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData)
{
	LSTATUS	lStatus = 0;
	HKEY	hkResult = 0;
	BOOL	bRet = FALSE;
	DWORD	dwData = 0;
	DWORD	dwLen = sizeof(DWORD);
	if ((lStatus = RegOpenKeyEx(hkey, lpszSubKey, 0, KEY_READ, &hkResult)) == ERROR_SUCCESS) {
		if ((lStatus = RegGetValue(hkey, lpszSubKey, szValue, RRF_RT_REG_DWORD, NULL, &dwData, &dwLen)) == ERROR_SUCCESS) {
			bRet = TRUE;
		}
		else {
			_Post_equals_last_error_ DWORD err = GetLastError();
			if (err == ERROR_FILE_NOT_FOUND) {
				bRet = TRUE;
			}
		}
		RegCloseKey(hkResult);
	}
	else {
		// Unknown error
#ifdef _DEBUG
#pragma warning(push)
#pragma warning(disable : 4189)
		{_Post_equals_last_error_ DWORD err = GetLastError(); }
#pragma warning(pop)
#endif // _DEBUG
	}
	*lpdwData = dwData;
	return	bRet;
}

//
// bSetRegValueDWORD()
//
BOOL		CRegistry::bSetRegValueDWORD(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, DWORD dwData)
{
	LSTATUS	lStatus = 0;
	HKEY		hKeySub = 0;
	BOOL		bRet = FALSE;
	DWORD	dwDisposition = 0;
	if ((lStatus = RegCreateKeyEx(hkey, lpszSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeySub, &dwDisposition)) == ERROR_SUCCESS) {
		DWORD	dwLen = sizeof(DWORD);
		if ((lStatus = RegSetValueEx(hKeySub, szValue, NULL, REG_DWORD, reinterpret_cast<BYTE*>(&dwData), dwLen)) == ERROR_SUCCESS) {
			bRet = TRUE;
		}
		else {
		}
		RegCloseKey(hKeySub);
	}
	else {
	}
	return	bRet;
}

//
// bReadRegValueDWORDasBOOL()
//
BOOL		CRegistry::bReadRegValueDWORDasBOOL(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPBOOL lpbData)
{
	BOOL	bRet = FALSE;
	DWORD	dwData = (DWORD)*lpbData;
	bRet = bReadRegValueDWORD(hkey, lpszSubKey, szValue, &dwData);
	if (dwData == 0)		*lpbData = FALSE;	else *lpbData = TRUE;
	return bRet;
}

//
// bGetRegValueDWORDasBOOL()
//
BOOL		CRegistry::bGetRegValueDWORDasBOOL(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPBOOL lpbData, BOOL bInitialData)
{
	BOOL		bRet = FALSE;
	DWORD	dwData = (DWORD)*lpbData;
	bRet = bGetRegValueDWORD(hkey, lpszSubKey, szValue, &dwData, (DWORD)bInitialData);
	if (dwData == 0)		*lpbData = FALSE;	else *lpbData = TRUE;
	return bRet;
}
//
// bGetRegValueString()
//
BOOL		CRegistry::bReadRegValueString(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize)
{
	LSTATUS	lStatus = 0;
	HKEY		hKeySub = 0;
	BOOL		bRet = FALSE;
	if ((lStatus = RegOpenKeyEx(hkey, lpszSubKey, 0, KEY_READ, &hKeySub)) == ERROR_SUCCESS) {
		DWORD	dwLen = 0;
		if ((lStatus = RegQueryValueEx(hKeySub, szValue, NULL, NULL, NULL, &dwLen)) == ERROR_SUCCESS) {
			if ((dwLen <= (_MAX_PATH * sizeof(TCHAR)) && (dwLen <= dwDataSize))) {	// この処理では_MAX_PATHまでしか扱わない
				if ((lStatus = RegQueryValueEx(hKeySub, szValue, NULL, NULL, (LPBYTE)szData, &dwLen)) == ERROR_SUCCESS) {
					bRet = TRUE;
				}
			}
		}
		RegCloseKey(hKeySub);
	}
	if (lStatus == ERROR_FILE_NOT_FOUND)		bRet = TRUE;		// エラーの理由がValueがない時はTRUEで返る
	return	bRet;
}

//
// bSetRegValueString()
//
BOOL		CRegistry::bSetRegValueDWORDasBOOL(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, BOOL bData)
{
	DWORD	dwData = (DWORD)bData;
	return bSetRegValueDWORD(hkey, lpszSubKey, szValue, dwData);
}

//
// bGetRegValueString()
//
BOOL		CRegistry::bGetRegValueString(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize)
{
	LSTATUS	lStatus = 0;
	HKEY		hKeySub = 0;
	BOOL		bRet = FALSE;
	if ((lStatus = RegOpenKeyEx(hkey, lpszSubKey, 0, KEY_READ, &hKeySub)) == ERROR_SUCCESS) {
		DWORD	dwLen = 0;
		if ((lStatus = RegQueryValueEx(hKeySub, szValue, NULL, NULL, NULL, &dwLen)) == ERROR_SUCCESS) {
			if ((dwLen <= (_MAX_PATH * sizeof(TCHAR)) && (dwLen <= dwDataSize))) {
				if ((lStatus = RegQueryValueEx(hKeySub, szValue, NULL, NULL, (LPBYTE)szData, &dwLen)) == ERROR_SUCCESS) {
					bRet = TRUE;
				}
			}
		}
		RegCloseKey(hKeySub);
	}
	if (lStatus == ERROR_FILE_NOT_FOUND)		bRet = TRUE;	
	return	bRet;
}

//
// bSetRegValueString()
//
BOOL		CRegistry::bSetRegValueString(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize)
{
	LSTATUS	lStatus = 0;
	HKEY		hKeySub = 0;
	BOOL		bRet = FALSE;
	DWORD	dwDisposition = 0;
	if ((lStatus = RegCreateKeyEx(hkey, lpszSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeySub, &dwDisposition)) == ERROR_SUCCESS) {
		if (dwDataSize <= _MAX_PATH) {
			if ((lStatus = RegSetValueEx(hKeySub, szValue, NULL, REG_SZ, (LPBYTE)&szData, dwDataSize)) == ERROR_SUCCESS) {
				bRet = TRUE;
			}
		}
		RegCloseKey(hKeySub);
	}
	return	bRet;
}

/* = EOF = */