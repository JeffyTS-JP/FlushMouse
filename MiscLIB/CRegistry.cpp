//
// CRegstry.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/05/23  JeffyTS  	New edit.
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
// bGetSetRegValueDWORD()
//
BOOL CRegistry::bGetSetRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData, DWORD dwInitialData)
{
	LSTATUS lStatus = 0;
	HKEY hKeySub = NULL;
	BOOL bRet = FALSE;
	DWORD dwLen = sizeof(DWORD);
	DWORD dwData = dwInitialData;
	DWORD dwDisposition = 0;

	if (lpdwData == NULL) return FALSE;

	lStatus = RegCreateKeyEx(hKey, lpszSubKey, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_READ | KEY_WRITE | KEY_WOW64_64KEY, NULL, &hKeySub, &dwDisposition);
	if (lStatus != ERROR_SUCCESS) {
		return FALSE;
	}

	if (dwDisposition == REG_OPENED_EXISTING_KEY) {
		lStatus = RegQueryValueEx(hKeySub, szValue, NULL, NULL, reinterpret_cast<LPBYTE>(&dwData), &dwLen);
		if (lStatus == ERROR_SUCCESS) {
			bRet = TRUE;
		}
		else if (lStatus == ERROR_FILE_NOT_FOUND || lStatus == ERROR_PATH_NOT_FOUND) {
			lStatus = RegSetValueEx(hKeySub, szValue, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&dwData), dwLen);
			if (lStatus == ERROR_SUCCESS) bRet = TRUE;
		}
	}
	else {
		lStatus = RegSetValueEx(hKeySub, szValue, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&dwData), dwLen);
		if (lStatus == ERROR_SUCCESS) bRet = TRUE;
	}

	RegCloseKey(hKeySub);
	*lpdwData = dwData;
	return bRet;
}

//
// bReadRegValueDWORD()
//
BOOL CRegistry::bReadRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData)
{
	if (lpdwData == NULL) return FALSE;
	*lpdwData = 0;

	LSTATUS lStatus = 0;
	HKEY hkResult = NULL;
	DWORD dwLen = sizeof(DWORD);

	lStatus = RegOpenKeyEx(hKey, lpszSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &hkResult);
	if (lStatus != ERROR_SUCCESS) {
		return FALSE;
	}

	lStatus = RegQueryValueEx(hkResult, szValue, NULL, NULL, reinterpret_cast<LPBYTE>(lpdwData), &dwLen);
	RegCloseKey(hkResult);

	if (lStatus == ERROR_SUCCESS) return TRUE;
	return FALSE;
}

//
// bReadSystemRegValueDWORD()
//
BOOL CRegistry::bReadSystemRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData)
{
	return bReadRegValueDWORD(hKey, lpszSubKey, szValue, lpdwData);
}

//
// bSetRegValueDWORD()
//
BOOL CRegistry::bSetRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, DWORD dwData)
{
	LSTATUS lStatus = 0;
	HKEY hKeySub = NULL;
	BOOL bRet = FALSE;
	DWORD dwDisposition = 0;
	DWORD dwLen = sizeof(DWORD);

	lStatus = RegCreateKeyEx(hKey, lpszSubKey, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_WRITE | KEY_WOW64_64KEY, NULL, &hKeySub, &dwDisposition);
	if (lStatus != ERROR_SUCCESS) return FALSE;

	lStatus = RegSetValueEx(hKeySub, szValue, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&dwData), dwLen);
	if (lStatus == ERROR_SUCCESS) bRet = TRUE;

	RegCloseKey(hKeySub);
	return bRet;
}

//
// bGetSetRegValueDWORDasBOOL()
//
BOOL CRegistry::bGetSetRegValueDWORDasBOOL(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPBOOL lpbData, BOOL bInitialData)
{
	if (lpbData == NULL) return FALSE;
	DWORD dw = (DWORD)(*lpbData ? 1 : 0);
	BOOL bRet = bGetSetRegValueDWORD(hKey, lpszSubKey, szValue, &dw, (DWORD)(bInitialData ? 1 : 0));
	*lpbData = (dw != 0) ? TRUE : FALSE;
	return bRet;
}

//
// bReadRegValueDWORDasBOOL()
//
BOOL CRegistry::bReadRegValueDWORDasBOOL(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPBOOL lpbData)
{
	if (lpbData == NULL) return FALSE;
	DWORD dw = 0;
	BOOL bRet = bReadRegValueDWORD(hKey, lpszSubKey, szValue, &dw);
	*lpbData = (dw != 0) ? TRUE : FALSE;
	return bRet;
}

//
// bSetRegValueDWORDasBOOL()
//
BOOL CRegistry::bSetRegValueDWORDasBOOL(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, BOOL bData)
{
	DWORD dw = (DWORD)(bData ? 1 : 0);
	return bSetRegValueDWORD(hKey, lpszSubKey, szValue, dw);
}

//
// bReadRegValueString()
//
BOOL CRegistry::bReadRegValueString(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize)
{
	if ((szData == NULL) || (dwDataSize == 0)) return FALSE;

	LSTATUS lStatus = 0;
	HKEY hKeySub = NULL;
	DWORD dwType = 0;
	DWORD cbSize = 0;

	lStatus = RegOpenKeyEx(hKey, lpszSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &hKeySub);
	if (lStatus != ERROR_SUCCESS) return FALSE;

	lStatus = RegQueryValueEx(hKeySub, szValue, NULL, &dwType, NULL, &cbSize);
	if (lStatus != ERROR_SUCCESS) {
		RegCloseKey(hKeySub);
		return FALSE;
	}

	if (cbSize == 0 || (cbSize / sizeof(TCHAR) + 1) > dwDataSize) {
		RegCloseKey(hKeySub);
		return FALSE;
	}

	ZeroMemory(szData, dwDataSize * sizeof(TCHAR));
	lStatus = RegQueryValueEx(hKeySub, szValue, NULL, NULL, reinterpret_cast<LPBYTE>(szData), &cbSize);
	RegCloseKey(hKeySub);

	if (lStatus == ERROR_SUCCESS) return TRUE;
	return FALSE;
}

BOOL		CRegistry::bGetSetRegValueString(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize)
{
	LSTATUS	lStatus = 0;
	HKEY	hKeySub = NULL;
	BOOL	bRet = FALSE;
	if (dwDataSize <= 1024) {
		if ((lStatus = RegOpenKeyEx(hKey, lpszSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &hKeySub)) == ERROR_SUCCESS) {
			DWORD	dwLen = 0;
			if ((lStatus = RegQueryValueEx(hKeySub, szValue, NULL, NULL, NULL, &dwLen)) == ERROR_SUCCESS) {
				if ((dwLen <= 1024) && ((dwLen / sizeof(TCHAR)) <= dwDataSize)) {
					ZeroMemory(szData, dwDataSize);
					if ((lStatus = RegQueryValueEx(hKeySub, szValue, NULL, NULL, (LPBYTE)szData, &dwLen)) == ERROR_SUCCESS) {
						bRet = TRUE;
					}
				}
			}
			RegCloseKey(hKeySub);
		}
		if (lStatus == ERROR_FILE_NOT_FOUND)	bRet = TRUE;
	}
	return	bRet;
}

//
// bSetRegValueString()
//
BOOL CRegistry::bSetRegValueString(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize)
{
	if ((szData == NULL) || (dwDataSize == 0)) return FALSE;

	LSTATUS lStatus = 0;
	HKEY hKeySub = NULL;
	BOOL bRet = FALSE;

	size_t  cbData = (size_t )(wcsnlen_s(szData, dwDataSize) + 1) * sizeof(TCHAR);
	if (cbData == 0 || cbData > (1024 * sizeof(TCHAR))) return FALSE;

	lStatus = RegCreateKeyEx(hKey, lpszSubKey, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_WRITE | KEY_WOW64_64KEY, NULL, &hKeySub, NULL);
	if (lStatus != ERROR_SUCCESS) return FALSE;

	lStatus = RegSetValueEx(hKeySub, szValue, 0, REG_SZ, reinterpret_cast<const BYTE*>(szData), (DWORD)cbData);
	if (lStatus == ERROR_SUCCESS) bRet = TRUE;

	RegCloseKey(hKeySub);
	return bRet;
}

//
// bDeleteRegValue
//
BOOL CRegistry::bDeleteRegValue(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue)
{
	LSTATUS lStatus = 0;
	HKEY hKeySub = NULL;
	BOOL bRet = FALSE;

	lStatus = RegOpenKeyEx(hKey, lpszSubKey, 0, KEY_SET_VALUE | KEY_WOW64_64KEY, &hKeySub);
	if (lStatus != ERROR_SUCCESS) return FALSE;

	lStatus = RegDeleteValue(hKeySub, szValue);
	if (lStatus == ERROR_SUCCESS) bRet = TRUE;

	RegCloseKey(hKeySub);
	return bRet;
}


/* = EOF = */
