// FlushMouse32.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/04/06  JeffyTS  	New edit.
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

//
// Global Data
//

//
// class CFocusEvent
//
class CFocusEvent
{
public:
	CFocusEvent();
	~CFocusEvent();
	BOOL		bEventSet();
	BOOL		bEventUnset();

private:
	static void CALLBACK vHandleEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

private:
	HWINEVENTHOOK	hEventHook;
};

/* EOF */