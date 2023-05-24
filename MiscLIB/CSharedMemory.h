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
#include "pch.h"

//
// Define
//

//
// Class CSharedMemory
//
class CSharedMemory
{
public:
	CSharedMemory(LPCTSTR szSharedMemoryName, DWORD dwDataSize);
	~CSharedMemory();

public:
	LPVOID	lpvSharedMemoryRead();
	BOOL	bSharedMemoryWrite(LPVOID lpSharedData);

private:
	BOOL		bSharedMemoryClose(LPVOID lpvSharedMem);

private:
	HANDLE	hSharedMem;
	LPTSTR	lpszSharedMemoryName;
	DWORD	dwDataByteSize;
};

/* EOF */