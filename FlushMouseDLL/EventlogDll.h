//
// Eventlog.h
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the MIT License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2023/04/14  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "FlushMouseDll.h"
#include "EventlogData.h"

//
// Global Prototype Define
//
DLLEXPORT BOOL  __stdcall 	bReportEvent(DWORD dwEventID, WORD wCategory);

/* EOF */
