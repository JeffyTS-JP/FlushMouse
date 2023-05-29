// CRegistry.h
//		Copyright (C) 2022 JeffyTS
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

//
// Class CRegistry
//
class CRegistry
{
public:
	CRegistry();
	~CRegistry();

public:
	BOOL		bGetSetRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData, DWORD dwInitialData);
	BOOL		bSetRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, DWORD dwData);
	BOOL		bGetSetRegValueDWORDasBOOL(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPBOOL lpbData, BOOL bInitialData);
	BOOL		bSetRegValueDWORDasBOOL(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, BOOL bData);
	BOOL		bGetSetRegValueString(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize);
	BOOL		bSetRegValueString(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize);

	BOOL		bReadRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData);
	BOOL		bReadSystemRegValueDWORD(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData);
	BOOL		bReadRegValueDWORDasBOOL(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPBOOL lpbData);
	BOOL		bReadRegValueString(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize);

	BOOL		bDeleteRegValue(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR szValue);

private:

};

/* = EOF = */