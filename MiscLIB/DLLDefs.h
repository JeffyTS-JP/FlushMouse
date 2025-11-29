//
// DLLDefs.h
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/02/08  JeffyTS		New edit.
//

//
// Include
//
#pragma once

//
// Define
//
#ifdef _USRDLL
#define DLLAPI  __declspec(dllexport)
#else
#define DLLAPI  __declspec(dllimport)
#endif

#define DLLEXPORT extern "C" DLLAPI
#define DLLEXPORT_CLS		 DLLAPI

/* EOF */
