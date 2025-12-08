//
// CThread.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
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
	: lpstThreadData(new THREAD_DATA())
{
	if (lpstThreadData) {
		ZeroMemory(lpstThreadData, sizeof(THREAD_DATA));
		lpstThreadData->lpstSA = new SECURITY_ATTRIBUTES();
		if (lpstThreadData->lpstSA == NULL) {
			delete lpstThreadData;
			lpstThreadData = NULL;
			return;
		}
		ZeroMemory(lpstThreadData->lpstSA, sizeof(SECURITY_ATTRIBUTES));
	}
	else return;

	lpstThreadData->lpstSA->nLength = sizeof(SECURITY_ATTRIBUTES);
	lpstThreadData->lpstSA->lpSecurityDescriptor = NULL;
	lpstThreadData->lpstSA->bInheritHandle = TRUE;
}

CThread::CThread(const CThread& other)
	: lpstThreadData(new THREAD_DATA())
{
	if (lpstThreadData != NULL && other.lpstThreadData != NULL) {
		*lpstThreadData = *other.lpstThreadData;
		if (other.lpstThreadData->lpstSA != NULL) {
			lpstThreadData->lpstSA = new SECURITY_ATTRIBUTES();
			if (lpstThreadData->lpstSA) {
				*(lpstThreadData->lpstSA) = *(other.lpstThreadData->lpstSA);
			}
		}
	}
}

CThread& CThread::operator = (const CThread& other)
{
	if (this != &other) {
		if (lpstThreadData != NULL) {
			if (lpstThreadData->lpstSA != NULL) delete lpstThreadData->lpstSA;
			delete lpstThreadData;
		}
		lpstThreadData = new THREAD_DATA();
		if (lpstThreadData != NULL && other.lpstThreadData != NULL) {
			*lpstThreadData = *other.lpstThreadData;
			if (other.lpstThreadData->lpstSA != NULL) {
				lpstThreadData->lpstSA = new SECURITY_ATTRIBUTES();
				if (lpstThreadData->lpstSA) {
					*(lpstThreadData->lpstSA) = *(other.lpstThreadData->lpstSA);
				}
			}
		}
	}
	return *this;
}

CThread::~CThread()
{
	vUnregister();
	if (lpstThreadData) {
		if (lpstThreadData->lpstSA != NULL) delete lpstThreadData->lpstSA;
		delete lpstThreadData;
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
	lpstThreadData->hEvent = CreateEvent(NULL, TRUE, FALSE, lpstThreadData->lpszThreadName);
	if (lpstThreadData->hEvent == NULL) return FALSE;

	uintptr_t threadHandle = _beginthreadex(lpstThreadData->lpstSA, 0, &uThreadProc, lpstThreadData, CREATE_SUSPENDED, reinterpret_cast<unsigned int*>(&lpstThreadData->dwThreadID));
	if (threadHandle == 0) {
		CloseHandle(lpstThreadData->hEvent);
		lpstThreadData->hEvent = NULL;
		return FALSE;
	}
	lpstThreadData->hThread = reinterpret_cast<HANDLE>(threadHandle);
	return TRUE;
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
			if (ResumeThread(lpstThreadData->hThread) != (DWORD)-1) {
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
	if ((lpstThreadData == NULL) || (lpstThreadData->lpstSA == NULL) || (lpstThreadData->hEvent == NULL) || (lpstThreadData->hThread == NULL))	return FALSE;
	return lpstThreadData->bThreadSentinel;
}

//
// bSetSentinel()
//
BOOL	CThread::bSetSentinel(BOOL bSentinel)
{
	if ((lpstThreadData == NULL) || (lpstThreadData->lpstSA == NULL) || (lpstThreadData->hEvent == NULL) || (lpstThreadData->hThread == NULL))	return FALSE;
	BOOL	bRet = lpstThreadData->bThreadSentinel;
	lpstThreadData->bThreadSentinel = bSentinel;
	if (lpstThreadData->hEvent != NULL) SetEvent(lpstThreadData->hEvent);
	return bRet;
}

//
// bSetThreadPriority()
//
BOOL	CThread::bSetThreadPriority(int nPriority) const
{
	if ((lpstThreadData == NULL) || (lpstThreadData->lpstSA == NULL) || (lpstThreadData->hEvent == NULL) || (lpstThreadData->hThread == NULL))	return FALSE;
	if (SetThreadPriority(lpstThreadData->hThread, nPriority) != 0)	return TRUE;
	return FALSE;
}

//
// hGetHandle()
//
HANDLE	CThread::hGetHandle() const
{
	if (lpstThreadData != NULL)	return lpstThreadData->hThread;
	else return NULL;
}

//
// vUnregister()
//
VOID 	CThread::vUnregister()
{
#define	TIMEOUT	1000
	if ((lpstThreadData == NULL) || (lpstThreadData->lpstSA == NULL) || (lpstThreadData->hEvent == NULL) || (lpstThreadData->hThread == NULL))	return;
	lpstThreadData->bThreadSentinel = FALSE;
	if (lpstThreadData->hEvent != NULL) SetEvent(lpstThreadData->hEvent);

	DWORD	dwExitCode = 0;
	if (GetExitCodeThread(reinterpret_cast<HANDLE>(lpstThreadData->hThread), &dwExitCode)) {
		if (dwExitCode == STILL_ACTIVE) {
			ResumeThread(lpstThreadData->hThread);
			DWORD	dwRet = WaitForSingleObject((HANDLE)lpstThreadData->hThread, TIMEOUT);
			if (dwRet != WAIT_OBJECT_0) {
#pragma warning(push)
#pragma warning(disable : 6258)
				TerminateThread(lpstThreadData->hThread, 2);
#pragma warning(pop)
			}
		}
	}
	if (lpstThreadData->hThread != NULL)	CloseHandle(lpstThreadData->hThread);
	lpstThreadData->hThread = NULL;
	if (lpstThreadData->hEvent != NULL)		CloseHandle(lpstThreadData->hEvent);
	lpstThreadData->hEvent = NULL;
	return;
}

//
// uThreadProc()
//
unsigned __stdcall	CThread::uThreadProc(void* pArguments)
{
	LPTHREAD_DATA lpstThreadData = reinterpret_cast<LPTHREAD_DATA>(pArguments);
	if (!lpstThreadData) {
		_endthreadex((unsigned)-3);
		return (unsigned)-3;
	}
	
	do {
		if ((lpstThreadData->lpstSA != NULL) && (lpstThreadData->hEvent != NULL) && (lpstThreadData->hThread != NULL)) {
			if (!lpstThreadData->bThreadSentinel) break;
			DWORD	dwRet = WaitForSingleObject(lpstThreadData->hEvent, INFINITE);
			(void)dwRet;
			if (lpstThreadData->lpbCallbackRoutine) {
				BOOL cbRet = lpstThreadData->lpbCallbackRoutine(lpstThreadData->lParamOption);
				if (!cbRet) {
					_endthreadex((unsigned)-1);
					return (unsigned)-1;
				}
			}
			if (!lpstThreadData->bThreadSentinel) break;
			if (lpstThreadData->dwSleepTime != 0) SleepEx(lpstThreadData->dwSleepTime, TRUE);
			if (SuspendThread(lpstThreadData->hThread) == (DWORD)-1) {
				_endthreadex((unsigned)-2);
				return (unsigned)-2;
			}
		}
		else {
			_endthreadex((unsigned)-3);
			return (unsigned)-3;
		}
		SleepEx(0, TRUE);
	} while (lpstThreadData->bThreadSentinel);
	_endthreadex(0);
	return 0;
}

/* = EOF = */
