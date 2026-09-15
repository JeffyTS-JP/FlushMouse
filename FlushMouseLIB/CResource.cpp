//
// CResource.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/09/09  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "CResource.h"

//
// Define
//

//
// Global Data
//

//
// Local Data
//
static CRITICAL_SECTION	csResource;
static std::once_flag	csInitFlag;

//
// Local Prototype Define
//

//
// Class CResource
//
CResource::CResource(LPCTSTR lpszResFile)
	: _lpszResFile(new TCHAR[_MAX_PATH]()), hModRes(NULL), iResourceLoadCount(0)
{
	if (_lpszResFile) {
		_tcsncpy_s(_lpszResFile.get(), _MAX_PATH, lpszResFile, _TRUNCATE);
	}
	std::call_once(csInitFlag, []() {
		InitializeCriticalSection(&csResource);
	});
}

CResource::~CResource()
{
	(void)bUnload();
	_lpszResFile = NULL;
	iResourceLoadCount = 0;
}

//
// hLoad()
//
HMODULE		CResource::hLoad()
{
	if (!_lpszResFile) return NULL;

	std::call_once(csInitFlag, []() {
		InitializeCriticalSection(&csResource);
	});

	EnterCriticalSection(&csResource);

	HMODULE hRes = NULL;
	if (hModRes == NULL) {
		hRes = LoadLibraryEx(_lpszResFile.get(), NULL, (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE));
		if (hRes == NULL) {
			LeaveCriticalSection(&csResource);
			return NULL;
		}
		hModRes = hRes;
		iResourceLoadCount = 0;
	}

	++iResourceLoadCount;

	hRes = hModRes;
	LeaveCriticalSection(&csResource);
	return hRes;
}

//
// bUnload()
//
BOOL		CResource::bUnload()
{
	BOOL bRet = TRUE;
	std::call_once(csInitFlag, []() {
		InitializeCriticalSection(&csResource);
				   });
	EnterCriticalSection(&csResource);
	if (hModRes != NULL) {
		while (iResourceLoadCount > 0) iResourceLoadCount--;
		if (!FreeLibrary(hModRes)) {
			bRet = FALSE;
		}
		hModRes = NULL;
	}
	LeaveCriticalSection(&csResource);
	return bRet;
}



/* = EOF = */
