// CThread.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/03/03  JeffyTS  	New edit.
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
typedef struct tagTHREAD_DATA {
	LPCTSTR					lpszThreadName;
	DWORD					dwThreadID;
	LPTHREAD_START_ROUTINE	lpbCallbackRoutine;
	LPVOID					lParamOption;
	DWORD					dwSleepTime;
	HANDLE					hEvent;
	HANDLE					hThread;
	BOOL					bThreadSentinel;
	LPSECURITY_ATTRIBUTES	lpstSA;
} THREAD_DATA, * PTHREAD_DATA, * LPTHREAD_DATA;

//
// Class Define
//
// CThread
//
class CThread
{
public:
			CThread();
			~CThread();
public:
	BOOL 	bRegister(LPCTSTR lpszThreadName, DWORD dwThreadID, LPTHREAD_START_ROUTINE lpbCallbackRoutine, LPVOID lParamOption, DWORD dwSleepTime);
	BOOL 	bStart();
	BOOL	bCheckSentinel() const;
	BOOL	bSetSentinel(BOOL bSentinel);
	BOOL	bSetThreadPriority(int nPriority) const;
	HANDLE	hGetHandle() const;

private:
	VOID 	vUnregister();
	static unsigned __stdcall	uThreadProc(void* pArguments);

private:
	LPTHREAD_DATA	lpstThreadData;
};

/* == EOF == */