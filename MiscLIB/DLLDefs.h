// DLLDefs.h
//		Copyright (C) 2022 JeffyTS
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