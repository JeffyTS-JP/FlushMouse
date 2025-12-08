//
// Eventlog.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
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
#include "..\FlushMouseDLL\EventlogData.h"

//
// bReportEvent()
//
BOOL		bReportEvent(DWORD dwEventID, WORD wCategory)
{
	BOOL	bRet = FALSE;
	HANDLE	hEvent = RegisterEventSource(NULL, FLUSHMOUSE);
	if (!hEvent)	return FALSE;

	DWORD	severity = (dwEventID >> 30) & 0x3;
	WORD	wType = EVENTLOG_SUCCESS;
	switch (severity)
	{
		case STATUS_SEVERITY_INFORMATIONAL:
			wType = EVENTLOG_INFORMATION_TYPE;
			break;
		case STATUS_SEVERITY_WARNING:
			wType = EVENTLOG_WARNING_TYPE;
			break;
		case STATUS_SEVERITY_ERROR:
			wType = EVENTLOG_ERROR_TYPE;
			break;
		default:
			break;
	}
	if (ReportEvent(hEvent, wType, wCategory, dwEventID, NULL, 0, 0, NULL, NULL) != 0) {
		bRet = TRUE;
	}
	if (DeregisterEventSource(hEvent) == 0) {
		bRet = FALSE;
	}
	return bRet;
}

/* = EOF = */
