// CThread.cpp
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/03  JeffyTS  	New edit.
//

#pragma once
#include "pch.h"
#include <process.h>
#include "CThread.h"

//
// CThread::
//
CThread::CThread()
{
	if ((lpstThreadData = new THREAD_DATA[sizeof(THREAD_DATA)]) != NULL)	ZeroMemory(lpstThreadData, sizeof(THREAD_DATA));
	if ((lpstThreadData->lpstSA = new SECURITY_ATTRIBUTES[sizeof(SECURITY_ATTRIBUTES)]) != NULL)	ZeroMemory(lpstThreadData->lpstSA, sizeof(SECURITY_ATTRIBUTES));

	lpstThreadData->lpstSA->nLength = sizeof(SECURITY_ATTRIBUTES);
	lpstThreadData->lpstSA->lpSecurityDescriptor = NULL;
	lpstThreadData->lpstSA->bInheritHandle = TRUE;
}

CThread::~CThread()
{
	vUnregister();
	if (lpstThreadData) {
		if (lpstThreadData->lpstSA != NULL)	delete[]	lpstThreadData->lpstSA;
		delete[]	lpstThreadData;
	}
	lpstThreadData = NULL;
}

//
// bResister()
//
BOOL 	CThread::bRegister(LPCTSTR lpszThreadName, DWORD dwThreadID, LPTHREAD_START_ROUTINE lpbCallbackRoutine, LPVOID lParamOption, DWORD dwSleepTime)
{
	if ((lpstThreadData == NULL) || (lpstThreadData->lpstSA == NULL))	return FALSE;
	lpstThreadData->lpszThreadName = lpszThreadName;				lpstThreadData->dwThreadID = dwThreadID;
	lpstThreadData->lpbCallbackRoutine = lpbCallbackRoutine;		lpstThreadData->lParamOption = lParamOption;
	lpstThreadData->dwSleepTime = dwSleepTime;						lpstThreadData->bThreadSentinel = TRUE;
	if ((lpstThreadData->hEvent = CreateEvent(NULL, TRUE, FALSE, lpstThreadData->lpszThreadName)) != NULL) {
		if ((lpstThreadData->hThread = (HANDLE)_beginthreadex(lpstThreadData->lpstSA, 0, &uThreadProc, lpstThreadData, CREATE_SUSPENDED, (UINT*)&lpstThreadData->dwThreadID)) != 0) {
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
	if ((lpstThreadData == NULL) || (lpstThreadData->lpstSA == NULL) || (lpstThreadData->hThread == NULL))	return FALSE;
	lpstThreadData->bThreadSentinel = TRUE;
	DWORD	dwExitCode = 0;
	if (GetExitCodeThread((HANDLE)lpstThreadData->hThread, &dwExitCode)) {
		if (dwExitCode == STILL_ACTIVE) {
			if (ResumeThread(lpstThreadData->hThread) != -1) {
				if (lpstThreadData->hEvent != NULL) {
					if (SetEvent(lpstThreadData->hEvent)) {
						return TRUE;
					}
				}
			}
		}
		else {
			vUnregister();
			if (bRegister(lpstThreadData->lpszThreadName, lpstThreadData->dwThreadID,
				lpstThreadData->lpbCallbackRoutine, lpstThreadData->lParamOption, lpstThreadData->dwSleepTime)) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

//
// bCheckSentinel
//
BOOL	CThread::bCheckSentinel() const
{
	if ((lpstThreadData == NULL) || (lpstThreadData->lpstSA == NULL) || (lpstThreadData->hEvent == NULL) || (lpstThreadData->hThread == NULL))	return FALSE;		// error
	return lpstThreadData->bThreadSentinel;
}

//
// bSetSentinel()
//
BOOL	CThread::bSetSentinel(BOOL bSentinel)
{
	if ((lpstThreadData == NULL) || (lpstThreadData->lpstSA == NULL) || (lpstThreadData->hEvent == NULL) || (lpstThreadData->hThread == NULL))	return FALSE;		// error
	BOOL	bRet = lpstThreadData->bThreadSentinel;
	lpstThreadData->bThreadSentinel = bSentinel;
	return bRet;
}

//
// bSetThreadPriority()
//
BOOL	CThread::bSetThreadPriority(int nPriority) const
{
	if ((lpstThreadData == NULL) || (lpstThreadData->lpstSA == NULL) || (lpstThreadData->hEvent == NULL) || (lpstThreadData->hThread == NULL))	return FALSE;		// error
	if (SetThreadPriority(lpstThreadData->hThread, nPriority) != 0)	return TRUE;
	return FALSE;
}

//
// vUnregister()
//
VOID 	CThread::vUnregister()
{
#define	TIMEOUT	1000 
	if ((lpstThreadData == NULL) || (lpstThreadData->lpstSA == NULL) || (lpstThreadData->hEvent == NULL) || (lpstThreadData->hThread == NULL))	return;
	lpstThreadData->bThreadSentinel = FALSE;
	DWORD	dwExitCode = 0;
	if (GetExitCodeThread((HANDLE)lpstThreadData->hThread, &dwExitCode)) {
		if (dwExitCode == STILL_ACTIVE) {
			if (ResumeThread(lpstThreadData->hThread) != -1) {
				if (SetEvent(lpstThreadData->hEvent)) {
					DWORD	dwRet = WaitForSingleObject((HANDLE)lpstThreadData->hThread, TIMEOUT);
					switch (dwRet) {
					case WAIT_OBJECT_0:
						break;
					case WAIT_ABANDONED:
					case WAIT_TIMEOUT:
					case WAIT_FAILED:
					default:
#pragma warning(push)
#pragma warning(disable : 6258)
						if (!TerminateThread(lpstThreadData->hThread, 2)) {
						}
#pragma warning(pop)
						break;
					}
				}
			}
		}
	}
	if (lpstThreadData->hThread != NULL)	CloseHandle(lpstThreadData->hThread);
	if (lpstThreadData->hEvent != NULL)		CloseHandle(lpstThreadData->hEvent);
	return;
}

//
// uThreadProc()
//
unsigned __stdcall	CThread::uThreadProc(void* pArguments)
{
	LPTHREAD_DATA lpstThreadData = (LPTHREAD_DATA)pArguments;
	do {
		if ((lpstThreadData != NULL) && (lpstThreadData->lpstSA != NULL) && (lpstThreadData->hEvent != NULL) && (lpstThreadData->hThread != NULL)) {
			DWORD	dwRet = WaitForSingleObject(lpstThreadData->hEvent, INFINITE);
			switch (dwRet) {
			case WAIT_OBJECT_0:	
				break;
			case WAIT_ABANDONED:
			case WAIT_TIMEOUT:
			case WAIT_FAILED:
			default:
				break;
			}
			if (!lpstThreadData->bThreadSentinel)	break;
			if (!(lpstThreadData->lpbCallbackRoutine)((LPVOID)lpstThreadData->lParamOption)) {
				_endthreadex((unsigned)-1);	
				return FALSE;
			}
			if (lpstThreadData->dwSleepTime != 0)	Sleep(lpstThreadData->dwSleepTime);
			if (SuspendThread(lpstThreadData->hThread) == -1) {
				_endthreadex((unsigned)-2);
				return FALSE;
			}
		}
		else {
			_endthreadex((unsigned)-3);
			return FALSE;
		}
	} while (lpstThreadData->bThreadSentinel);
	_endthreadex(0);
	return TRUE;
}

/* = EOF = */