// CThread.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/03  JeffyTS  	New edit.
//

#pragma once
#include <Windows.h>
#include "CThread.h"

//
// CThred::
//
CThread::CThread()
{
	lpstThreadData = new THREAD_DATA[sizeof(THREAD_DATA)];
	ZeroMemory(lpstThreadData, sizeof(THREAD_DATA));
}

CThread::~CThread()
{
	vUnregister();
	delete []	lpstThreadData;
}

//
// bResister()
//
BOOL 	CThread::bRegister(LPCTSTR lpszThreadName, DWORD dwThreadID, LPTHREAD_START_ROUTINE lpbCallbackRoutine, LPVOID lParamOption, DWORD dwSleepTime)
{
	lpstThreadData->lpszThreadName = lpszThreadName;				lpstThreadData->dwThreadID = dwThreadID;
	lpstThreadData->lpbCallbackRoutine = lpbCallbackRoutine;		lpstThreadData->lParamOption = lParamOption;
	lpstThreadData->dwSleepTime = dwSleepTime;
	if ((lpstThreadData->hEvent = CreateEvent(NULL, TRUE, FALSE, lpstThreadData->lpszThreadName)) != NULL) {
		lpstThreadData->bThreadSentinel = TRUE;
		if ((lpstThreadData->hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dwThreadProc, lpstThreadData, 0, &lpstThreadData->dwThreadID)) != NULL) {
			return TRUE;	
		}
		CloseHandle(lpstThreadData->hEvent);
	}
	return FALSE;
}

//
// bStart()
//
BOOL 	CThread::bStart()
{
	if ((lpstThreadData->hEvent == NULL) || (lpstThreadData->hThread == NULL)) {
		return FALSE;
	}
	lpstThreadData->bThreadSentinel = TRUE;
	DWORD	dwExitCode = 0;
	if (GetExitCodeThread(lpstThreadData->hThread, &dwExitCode)) {
		if (dwExitCode == STILL_ACTIVE) {
			if (SetEvent(lpstThreadData->hEvent)) {
				return TRUE;
			}
		}
	}
	else {
		vUnregister();
		if (bRegister(lpstThreadData->lpszThreadName, lpstThreadData->dwThreadID,
						lpstThreadData->lpbCallbackRoutine, lpstThreadData->lParamOption, lpstThreadData->dwSleepTime)) {
			SetEvent(lpstThreadData->hEvent);
			return TRUE;
		}
	}
	return FALSE;
}

//
// vUnregister()
//
VOID 	CThread::vUnregister()
{
#define	TIMEOUT	1000 
	if ((lpstThreadData->hEvent == NULL) || (lpstThreadData->hThread == NULL)) {
		return;
	}
	lpstThreadData->bThreadSentinel = FALSE;
	DWORD	dwExitCode = 0;
	if (!GetExitCodeThread(lpstThreadData->hThread, &dwExitCode)) {
	}
	if (dwExitCode == STILL_ACTIVE) {
		if (!SetEvent(lpstThreadData->hEvent)) {
		}
	}
	DWORD	dwRet = 0;
	dwRet = WaitForSingleObject(lpstThreadData->hThread, TIMEOUT);
	switch (dwRet) {
		case WAIT_OBJECT_0:
			break;
		case WAIT_ABANDONED:
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		default:
#pragma warning(push)
#pragma warning(disable : 6258)
			if (!TerminateThread(lpstThreadData->hThread, 0)) {
#pragma warning(pop)
				;
			}
	}
	CloseHandle(lpstThreadData->hThread);
	CloseHandle(lpstThreadData->hEvent);
}

//
// dwThreadProc()
//
DWORD WINAPI		CThread::dwThreadProc(LPTHREAD_DATA lpstThreadData)
{
	HANDLE	hEvent = NULL;
	if ((hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCTSTR)lpstThreadData->lpszThreadName)) != NULL) {
		do {
			if (WaitForSingleObject(hEvent, INFINITE) == WAIT_FAILED) {
			}
			if (!lpstThreadData->bThreadSentinel)	break;
			if (!(lpstThreadData->lpbCallbackRoutine)((LPVOID)lpstThreadData->lParamOption)) {
				CloseHandle(hEvent);
				ExitThread(FALSE);
			}
			if (lpstThreadData->dwSleepTime != 0)	Sleep(lpstThreadData->dwSleepTime);
			ResetEvent(hEvent);
		} while (lpstThreadData->bThreadSentinel);
		CloseHandle(hEvent);
		ExitThread(TRUE);
	}
	ExitThread(FALSE);
}

