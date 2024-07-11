//
// KeyboardHook.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2022/02/12  JeffyTS  	New edit.
// #0001	2024/07/01  JeffyTS  	Move to exe from dll
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