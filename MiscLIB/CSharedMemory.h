// CSharedMemory.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/02/13  JeffyTS  	New edit.
//

//
// Include
//
#pragma once

//
// Define
//

//
// Global Prototype Define
//
extern BOOL		bSharedMemoryCreate(HANDLE handle, LPCTSTR szSharedMemoryName, DWORD dwDataByteSize);
extern BOOL		bSharedMemoryDelete(LPCTSTR szSharedMemoryName);
extern LPVOID	lpvSharedMemoryOpen(LPCTSTR szSharedMemoryName, DWORD dwDataByteSize);
extern BOOL		bSharedMemoryClose(LPVOID lpvSharedMem);
extern BOOL		bSharedMemoryRead(LPCTSTR szSharedMemoryName, LPBYTE lpbSharedData, DWORD dwSharedDataByteSize);
extern BOOL		bSharedMemoryWrite(LPCTSTR szSharedMemoryName, LPBYTE lpbSharedData, DWORD dwSharedDataByteSize);

/* EOF */