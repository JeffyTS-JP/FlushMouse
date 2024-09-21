﻿//
// CResource.cpp
//		Copyright (C) 2022 JeffyTS
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

//
// Local Prototype Define
//

//
// Class CResource
//
CResource::CResource()
{
	hModRes = NULL;
	iResourceLoadCount = 0;
	szResFile = NULL;
}

CResource::CResource(LPCTSTR lpszResFile)
{
	hModRes = NULL;
	iResourceLoadCount = 0;
	szResFile = new TCHAR[_MAX_PATH];
	if (szResFile) {
		ZeroMemory(szResFile, sizeof(TCHAR) * _MAX_PATH);
		_tcsncpy_s(szResFile, _MAX_PATH, lpszResFile, _TRUNCATE);
	}
}

CResource::~CResource()
{
	bUnload();
	hModRes = NULL;
	iResourceLoadCount = 0;
	if (szResFile)	delete []	szResFile;
	szResFile = NULL;
}

//
// hLoad()
//
HMODULE		CResource::hLoad()
{
	if ((hModRes == NULL) && (iResourceLoadCount == 0)) {
		if ((hModRes = LoadLibraryEx(szResFile, NULL, (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE))) == NULL) {
			return NULL;
		}
	}
	++iResourceLoadCount;
	return hModRes;
}

//
// bUnload()
//
BOOL		CResource::bUnload()
{
	if ((hModRes != NULL) && (iResourceLoadCount == 1)) {
		if (!FreeLibrary(hModRes)) {
			return FALSE;
		}
	}
	--iResourceLoadCount;
	return TRUE;
}



/* = EOF = */