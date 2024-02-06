// Eventlog.cpp
//		Copyright (C) 2022 JeffyTS
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
#include "CommonDef.h"

//
// bReportEvent()
//
BOOL		bReportEvent(DWORD dwEventID, WORD wCategory)
{
	BOOL	bRet = FALSE;
	HANDLE	hEvent = RegisterEventSource(NULL, FLUSHMOUSE);
	if (hEvent != NULL) {
		if (ReportEvent(hEvent, (0x0000000c & (dwEventID >> 28)), wCategory, dwEventID, NULL, 0, 0, NULL, NULL) != 0) {
			bRet = TRUE;
		}
		else {
		}
		if (DeregisterEventSource(hEvent) == 0) {
			bRet = FALSE;
		}
	}
	return bRet;
}

/* EOF */