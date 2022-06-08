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
	BOOL						bThreadSentinel;
} THREAD_DATA, *PTHREAD_DATA, *LPTHREAD_DATA;

//
// Class Define
//
// Thred
class CThread
{
public:
			CThread();
			~CThread();
public:
	BOOL 	bRegister(LPCTSTR lpszThreadName, DWORD dwThreadID, LPTHREAD_START_ROUTINE lpbCallbackRoutine, LPVOID lParamOption, DWORD dwSleepTime);
	BOOL 	bStart();
	VOID 	vUnregister();

private:
	static DWORD WINAPI	dwThreadProc(LPTHREAD_DATA lpstThreadData);

private:
	LPTHREAD_DATA	lpstThreadData;
};

