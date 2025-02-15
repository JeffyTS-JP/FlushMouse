//
// CResource.h
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

//
// Define
// 

//
// Struct Define
//

//
// Global Data
//

//
// Global Prototype Define
//

//
// Class CResource
//
class CResource
{
	public:
		explicit CResource(LPCTSTR lpszResFile);
		~CResource();

	public:
		HMODULE	hLoad();
		BOOL	bUnload();

	private:
		LPTSTR	szResFile;
		HMODULE	hModRes;
		int		iResourceLoadCount;
};


/* = EOF = */