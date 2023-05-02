// CRegstry.h
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

//
// class CRegistry
//
class CRegistry
{
public:
	CRegistry();
	~CRegistry();

public:
	BOOL		bGetRegValueDWORD(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData, DWORD dwInitialData);
	BOOL		bSetRegValueDWORD(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, DWORD dwData);
	BOOL		bGetRegValueDWORDasBOOL(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPBOOL lpbData, BOOL bInitialData);
	BOOL		bSetRegValueDWORDasBOOL(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, BOOL bData);
	BOOL		bGetRegValueString(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize);
	BOOL		bSetRegValueString(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize);

	BOOL		bReadRegValueDWORD(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPDWORD lpdwData);
	BOOL		bReadRegValueDWORDasBOOL(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPBOOL lpbData);
	BOOL		bReadRegValueString(HKEY hkey, LPCTSTR lpszSubKey, LPCTSTR szValue, LPTSTR szData, DWORD dwDataSize);

private:

};

/* = EOF = */