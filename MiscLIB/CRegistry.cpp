// CRegstry.cpp
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
	// NOP
}

CRegistry::~CRegistry()
{
	// NOP
}

//
// bGetSetRegValueDWORD()
//
BOOL		CRegistry::bGetSetRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData, DWORD dwInitialData)
{
	LSTATUS	lStatus = 0;
	HKEY	hKeySub = NULL;
	BOOL	bRet = FALSE;
	DWORD	dwDisposition = 0;
	DWORD	dwLen = sizeof(DWORD);
	DWORD	dwData = dwInitialData;
	if ((lStatus = RegCreateKeyEx(hKey, lpszSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &hKeySub, &dwDisposition)) == ERROR_SUCCESS) {
		if (dwDisposition == KEY_QUERY_VALUE) {
			if ((lStatus = RegSetValueEx(hKeySub, szValue, NULL, REG_DWORD, reinterpret_cast<BYTE*>(&dwData), dwLen)) == ERROR_SUCCESS) {
				bRet = TRUE;
			}
		}
		else if (dwDisposition == REG_OPENED_EXISTING_KEY) {
			if ((lStatus = RegQueryValueEx(hKeySub, szValue, NULL, NULL, reinterpret_cast<BYTE*>(&dwData), &dwLen)) == ERROR_SUCCESS) {
				bRet = TRUE;
			}
			else if (lStatus == ERROR_FILE_NOT_FOUND) {
				if ((lStatus = RegSetValueEx(hKeySub, szValue, NULL, REG_DWORD, reinterpret_cast<BYTE*>(&dwData), dwLen)) == ERROR_SUCCESS) {
					bRet = TRUE;
				}
			}
		}
		RegCloseKey(hKeySub);
	}
	*lpdwData = dwData;
	return	bRet;
}

//
// bReadRegValueDWORD()
//
BOOL		CRegistry::bReadRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData)
{
	LSTATUS	lStatus = 0;
	HKEY	hkResult = 0;
	BOOL	bRet = FALSE;
	DWORD	dwData = 0;
	DWORD	dwLen = sizeof(DWORD);
	if ((lStatus = RegOpenKeyEx(hKey, lpszSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &hkResult)) == ERROR_SUCCESS) {
		if ((lStatus = RegGetValue(hKey, lpszSubKey, szValue, RRF_RT_REG_DWORD | RRF_SUBKEY_WOW6464KEY, NULL, &dwData, &dwLen)) == ERROR_SUCCESS) {
			bRet = TRUE;
		}
		else {
			if (lStatus == ERROR_FILE_NOT_FOUND) {
				bRet = FALSE;
			}
		}
		RegCloseKey(hkResult);
	}
	*lpdwData = dwData;
	return	bRet;
}

//
// bReadSystemRegValueDWORD()
//
BOOL		CRegistry::bReadSystemRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData)
{
	LSTATUS	lStatus = 0;
	HKEY	hkResult = 0;
	BOOL	bRet = FALSE;
	DWORD	dwData = 0;
	DWORD	dwLen = sizeof(DWORD);
	if ((lStatus = RegOpenKeyEx(hKey, lpszSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &hkResult)) == ERROR_SUCCESS) {
		if ((lStatus = RegGetValue(hKey, lpszSubKey, szValue, RRF_RT_REG_DWORD | RRF_SUBKEY_WOW6464KEY, NULL, &dwData, &dwLen)) == ERROR_SUCCESS) {
			bRet = TRUE;
		}
		else {
			if (lStatus == ERROR_FILE_NOT_FOUND) {
				bRet = FALSE;
			}
		}
		RegCloseKey(hkResult);
	}
	*lpdwData = dwData;
	return	bRet;
}

//
// bSetRegValueDWORD()
//
BOOL		CRegistry::bSetRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, DWORD dwData)
{
	LSTATUS	lStatus = 0;
	HKEY	hKeySub = NULL;
	BOOL	bRet = FALSE;
	DWORD	dwDisposition = 0;
	if ((lStatus = RegCreateKeyEx(hKey, lpszSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &hKeySub, &dwDisposition)) == ERROR_SUCCESS) {
		DWORD	dwLen = sizeof(DWORD);
		if ((lStatus = RegSetValueEx(hKeySub, szValue, NULL, REG_DWORD, reinterpret_cast<BYTE*>(&dwData), dwLen)) == ERROR_SUCCESS) {
			bRet = TRUE;
		}
		RegCloseKey(hKeySub);
	}
	return	bRet;
}

//
// bGetSetRegValueDWORDasBOOL()
//
BOOL		CRegistry::bGetSetRegValueDWORDasBOOL(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPBOOL lpbData, BOOL bInitialData)
{
	BOOL	bRet = FALSE;
	DWORD	dwData = (DWORD)*lpbData;
	bRet = bGetSetRegValueDWORD(hKey, lpszSubKey, szValue, &dwData, (DWORD)bInitialData);
	if (dwData == 0)	*lpbData = FALSE;
	else				*lpbData = TRUE;
	return bRet;
}

//
// bReadRegValueDWORDasBOOL()
//
BOOL		CRegistry::bReadRegValueDWORDasBOOL(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPBOOL lpbData)
{
	BOOL	bRet = FALSE;
	DWORD	dwData = (DWORD)*lpbData;
	bRet = bReadRegValueDWORD(hKey, lpszSubKey, szValue, &dwData);
	if (dwData == 0)	*lpbData = FALSE;
	else				*lpbData = TRUE;
	return bRet;
}

//
// bSetRegValueDWORDasBOOL()
//
BOOL		CRegistry::bSetRegValueDWORDasBOOL(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, BOOL bData)
{
	DWORD	dwData = (DWORD)bData;
	return bSetRegValueDWORD(hKey, lpszSubKey, szValue, dwData);
}

//
// bReadRegValueString()
//
BOOL		CRegistry::bReadRegValueString(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize)
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
// bGetSetRegValueString()
//
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
BOOL		CRegistry::bSetRegValueString(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize)
{
	LSTATUS	lStatus = 0;
	HKEY	hKeySub = NULL;
	BOOL	bRet = FALSE;
	DWORD	dwDisposition = 0;
	DWORD	_dwDataSize = (DWORD)wcsnlen_s(szData, dwDataSize) * sizeof(TCHAR);
	if (_dwDataSize <= 1024) {
		if ((lStatus = RegCreateKeyEx(hKey, lpszSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &hKeySub, &dwDisposition)) == ERROR_SUCCESS) {
			if ((lStatus = RegSetValueEx(hKeySub, szValue, NULL, REG_SZ, (LPBYTE)szData, _dwDataSize)) == ERROR_SUCCESS) {
				bRet = TRUE;
			}
			RegCloseKey(hKeySub);
		}
	}
	return	bRet;
}

//
// bDeleteRegValue
//
BOOL		CRegistry::bDeleteRegValue(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue)
{
	LSTATUS	lStatus = 0;
	HKEY	hKeySub = NULL;
	BOOL	bRet = FALSE;
	if ((lStatus = RegOpenKeyEx(hKey, lpszSubKey, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKeySub)) == ERROR_SUCCESS) {
		if ((lStatus = RegDeleteValue(hKeySub, szValue)) == ERROR_SUCCESS) {
			bRet = TRUE;
		}
		RegCloseKey(hKeySub);
	}
	return	bRet;
}


/* = EOF = */