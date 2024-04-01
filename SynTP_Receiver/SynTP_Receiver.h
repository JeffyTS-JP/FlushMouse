//
// SynTP_Receiver.h
//		Copyright (C) 2024 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/03/26  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "..\FlushMouseLIB\SynTP.h"
#include "..\MiscLIB\CWindow.h"

//
// Define
//

//
// Global Prototype Define
//

//
// Global Data
//

class CSynTP_Receiver : private CWindow
{
public:
	CSynTP_Receiver();
	~CSynTP_Receiver();

public:
	BOOL		bRegister(HINSTANCE hInstance, LPCTSTR szWindowClass);

private:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	BOOL		Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
	void		Cls_OnDestroy(HWND hWnd);

public:

private:
	CSynTP		*SynTP;

};


/* = EOF = */