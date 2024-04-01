// CSharedMemory.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/02/13  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "CSharedMemory.h"


//
// Define
//
#define MAX_LOADSTRING 100
#ifdef _WIN64
typedef     ULONGLONG	QWORD;
typedef     ULONGLONG	*LPQWORD;
#define	GUARDBANDSIZE	sizeof(QWORD)
#else
#define	GUARDBANDSIZE	sizeof(DWORD)
#endif // _WIN64

//
// Class CSharedMemory
//
CSharedMemory::CSharedMemory(LPCTSTR szSharedMemoryName, DWORD dwDataSize)
	: hSharedMem(), lpszSharedMemoryName(), dwDataByteSize()
{

	size_t	size = wcsnlen_s(szSharedMemoryName, MAX_LOADSTRING);
	if ((lpszSharedMemoryName = new TCHAR[size + 1]) == NULL)	return;
	ZeroMemory(lpszSharedMemoryName, sizeof(TCHAR) * (size + 1));
	_tcsncpy_s(lpszSharedMemoryName, size + 1, szSharedMemoryName, _TRUNCATE);
	dwDataByteSize = dwDataSize;

	SECURITY_ATTRIBUTES FileMappingAttributes{sizeof( SECURITY_ATTRIBUTES), NULL, TRUE };

	hSharedMem = CreateFileMapping(INVALID_HANDLE_VALUE,
		&FileMappingAttributes,
		PAGE_READWRITE,
		0,
		dwDataByteSize,
		lpszSharedMemoryName);
	if (hSharedMem != NULL) {
		LPBYTE lpSharedMem = (LPBYTE)MapViewOfFile(hSharedMem, FILE_MAP_ALL_ACCESS, 0, 0, dwDataByteSize);
		if (lpSharedMem != NULL) {
			ZeroMemory(lpSharedMem, dwDataByteSize + GUARDBANDSIZE);
			UnmapViewOfFile(lpSharedMem);
			return;
		}
		CloseHandle(hSharedMem);
		hSharedMem = NULL;
	}
	return;
}

CSharedMemory::~CSharedMemory()
{
	if (hSharedMem != NULL) {
		if (CloseHandle(hSharedMem)) {
			hSharedMem = NULL;
		}
	}
	hSharedMem = NULL;
	if (lpszSharedMemoryName != NULL)	delete[]	lpszSharedMemoryName;
	lpszSharedMemoryName = NULL;
	dwDataByteSize = 0;
	return;
}

//
// lpvSharedMemoryRead();
//
LPVOID	CSharedMemory::lpvSharedMemoryRead()
{
	if (hSharedMem != NULL) {
		LPVOID lpvSharedMem = MapViewOfFile(hSharedMem, FILE_MAP_ALL_ACCESS, 0, 0, dwDataByteSize);
		if (lpvSharedMem != NULL) {
			return lpvSharedMem;
		}
		CloseHandle(hSharedMem);
		hSharedMem = NULL;
	}
	return NULL;
}

//
// bSharedMemoryWrite()
//
BOOL	CSharedMemory::bSharedMemoryWrite(LPVOID lpSharedData)
{
	if (hSharedMem != NULL) {
		LPVOID lpvSharedMem = MapViewOfFile(hSharedMem, FILE_MAP_ALL_ACCESS, 0, 0, dwDataByteSize);
		if (lpvSharedMem != NULL) {
#ifdef _WIN64
			LPQWORD lpqwSharedMem = (LPQWORD)lpvSharedMem;
			LPQWORD	lpqwSharedData = (LPQWORD)lpSharedData;
			for (DWORD index = 0; index < (dwDataByteSize / sizeof(QWORD)); index++) {
				lpqwSharedMem[index] = lpqwSharedData[index];
			}
#else
			LPDWORD lpqwSharedMem = (LPDWORD)lpvSharedMem;
			LPDWORD	lpqwSharedData = (LPDWORD)lpSharedData;
			for (DWORD index = 0; index < (dwDataByteSize / sizeof(DWORD)); index++) {
				lpqwSharedMem[index] = lpqwSharedData[index];
			}
#endif // _WIN64
			if (FlushViewOfFile(lpvSharedMem, dwDataByteSize)) {
				return bSharedMemoryClose(lpqwSharedMem);
			}
		}
		CloseHandle(hSharedMem);
		hSharedMem = NULL;
	}
	return FALSE;
}

//
// bSharedMemoryClose
//
BOOL		CSharedMemory::bSharedMemoryClose(LPVOID lpvSharedMem)
{
	if (!UnmapViewOfFile(lpvSharedMem)) {
		return FALSE;
	}
	return TRUE;
}

/* == EOF == */