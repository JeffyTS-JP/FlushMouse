//
// KeyboardHookDll.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000		2022/02/12  JeffyTS  	New edit.
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
extern BOOL		bKeyboardHookLLSet(HWND hWnd);
extern VOID		vKeyboardHookLLUnset();
extern BOOL		bSetEnableEPHelperLL64(BOOL bEPHelper);
extern BOOL		bSetEnableIMEModeForcedLL64(BOOL bIMEModeForced);


/* = EOF = */