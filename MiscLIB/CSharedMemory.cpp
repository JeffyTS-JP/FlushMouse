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
#define	DEFAULT_SHAREDMEMORYNAME	_T("DefaultSharedMemory")
#ifdef _WIN64
typedef     ULONGLONG	QWORD;
typedef     ULONGLONG	*LPQWORD;
#define	GUARDBANDSIZE	sizeof(QWORD)
#else
#define	GUARDBANDSIZE	sizeof(DWORD)
#endif // _WIN64

//
// bSharedMemoryCreate()
//
BOOL		bSharedMemoryCreate(HANDLE handle, LPCTSTR szSharedMemoryName, DWORD dwDataByteSize)
{
	UNREFERENCED_PARAMETER(handle);
	SECURITY_ATTRIBUTES FileMappingAttributes{
											sizeof(SECURITY_ATTRIBUTES),
											NULL,
											TRUE };
	HANDLE hSharedMem = CreateFileMapping(INVALID_HANDLE_VALUE,	
		&FileMappingAttributes,
		PAGE_READWRITE,
		0,
		dwDataByteSize + GUARDBANDSIZE,
		szSharedMemoryName);
	if (hSharedMem != NULL) {
		LPBYTE lpSharedMem = (LPBYTE)MapViewOfFile(hSharedMem, FILE_MAP_ALL_ACCESS, 0, 0, dwDataByteSize + GUARDBANDSIZE);
		if (lpSharedMem != NULL) {
			ZeroMemory(lpSharedMem, dwDataByteSize + GUARDBANDSIZE);	
			UnmapViewOfFile(lpSharedMem);
			return TRUE;
		}
		CloseHandle(hSharedMem);
	}
	return FALSE;
}

//
// bSharedMemoryDelete()
//
BOOL		bSharedMemoryDelete(LPCTSTR szSharedMemoryName)
{
	HANDLE hSharedMem = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szSharedMemoryName);
	if (hSharedMem != NULL) {
		if (CloseHandle(hSharedMem))		return TRUE;	
	}
	return FALSE;
}

//
// lpvSharedMemoryOpen()
//
LPVOID	lpvSharedMemoryOpen(LPCTSTR szSharedMemoryName, DWORD dwDataByteSize)
{
	HANDLE hSharedMem = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szSharedMemoryName);	
	if (hSharedMem != NULL) {
		LPVOID lpvSharedMem = MapViewOfFile(hSharedMem, FILE_MAP_ALL_ACCESS, 0, 0, dwDataByteSize + GUARDBANDSIZE);
		if (lpvSharedMem != NULL) {
			return lpvSharedMem;
		}
		CloseHandle(hSharedMem);
	}
	return NULL;
}

//
// bSharedMemoryClose()
//
BOOL		bSharedMemoryClose(LPVOID lpvSharedMem)
{
	if (!UnmapViewOfFile(lpvSharedMem)) {
		return FALSE;
	}
	return TRUE;
}

//
// bSharedMemoryRead()
//
BOOL		bSharedMemoryRead(LPCTSTR szSharedMemoryName, LPBYTE lpbSharedData, DWORD dwSharedDataByteSize)
{
#ifdef _WIN64
	LPQWORD lpqwSharedMem = NULL;
	LPQWORD	lpqwSharedData = (LPQWORD)lpbSharedData;
	if ((lpqwSharedMem = (LPQWORD)lpvSharedMemoryOpen(szSharedMemoryName, dwSharedDataByteSize + GUARDBANDSIZE)) != NULL) {
		for (DWORD index = 0; index < (dwSharedDataByteSize / sizeof(QWORD)); index++) {	
			lpqwSharedData[index] = lpqwSharedMem[index];
		}
		return TRUE;
	}
#else
	LPDWORD lpqwSharedMem = NULL;
	LPDWORD	lpqwSharedData = (LPDWORD)lpbSharedData;

	if ((lpqwSharedMem = (LPDWORD)lpvSharedMemoryOpen(szSharedMemoryName, dwSharedDataByteSize + GUARDBANDSIZE)) != NULL) {
		for (DWORD index = 0; index < (dwSharedDataByteSize / sizeof(DWORD)); index++) {	
			lpqwSharedData[index] = lpqwSharedMem[index];
		}
		return TRUE;
	}
#endif // _WIN64
	return FALSE;
}

//
// bSharedMemoryWrite()
//
BOOL		bSharedMemoryWrite(LPCTSTR lpszSharedMemoryName, LPBYTE lpbSharedData, DWORD dwSharedDataByteSize)
{
#ifdef _WIN64
	LPQWORD lpqwSharedMem = NULL;
	LPQWORD	lpqwSharedData = (LPQWORD)lpbSharedData;

	if ((lpqwSharedMem = (LPQWORD)lpvSharedMemoryOpen(lpszSharedMemoryName, dwSharedDataByteSize)) != NULL) {
		for (DWORD index = 0; index < (dwSharedDataByteSize / sizeof(QWORD)); index++) {	
			lpqwSharedMem[index] = lpqwSharedData[index];
		}
		return bSharedMemoryClose(lpqwSharedMem);
	}
#else
	LPDWORD lpqwSharedMem = NULL;
	LPDWORD	lpqwSharedData = (LPDWORD)lpbSharedData;

	if ((lpqwSharedMem = (LPDWORD)lpvSharedMemoryOpen(lpszSharedMemoryName, dwSharedDataByteSize)) != NULL) {
		for (DWORD index = 0; index < (dwSharedDataByteSize / sizeof(DWORD)); index++) {	
			lpqwSharedMem[index] = lpqwSharedData[index];
		}
		return bSharedMemoryClose(lpqwSharedMem);
	}
#endif // _WIN64
	return FALSE;
}

/* == EOF == */