//
// CResource.h
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
#include <memory>

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
		std::unique_ptr<TCHAR[]> _lpszResFile;
		HMODULE	hModRes;
		int		iResourceLoadCount;
};


/* = EOF = */
