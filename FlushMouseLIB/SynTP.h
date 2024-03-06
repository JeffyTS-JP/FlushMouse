//
// SynTP.h
//		Copyright (C) 2022 JeffyTS
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/03/21  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "FlushMouseSub.h"
#include "TCPIP.h"
#include "..\MiscLIB\CWindow.h"
#include "..\MiscLIB\CThread.h"

//
// Define
// 
#define	DATA_BUFFER_SIZE		16					// "X -0000 0000" + α

//
// Struct Define
//

//
// Global Data
//

//
// Global Prototype Define
//


//
// Class SynTP
//
typedef struct SynTPFingerData
{
	BYTE	bTouched;
	BYTE	loX;
	BYTE	hiX;
	BYTE	loY;
	BYTE	hiY;
	BYTE	Area;
	BYTE	Unknown1;
	BYTE	Unknown2;
} SYNTPFINGERDATA, *PSYNTPFINGERDATA, *LPSYNTPFINGERDATA;

typedef struct SynTPRawData
{
	BYTE			bID;
	BYTE			bLength;
	SYNTPFINGERDATA	stFinger1;
	SYNTPFINGERDATA	stFinger2;
	SYNTPFINGERDATA	stFinger3;
} SYNTPRAWDATA, *PSYNTPRAWDATA, *LPSYNTPRAWDATA;

typedef struct SynTPData
{
	SYNTPRAWDATA stSynRawData;

	int		iXStart;
	int		iYStart;
	int		iWheelStart;

	RECT	rcSynTP;
	SHORT	sTouchZoneX;
	SHORT	sTouchZoneY;

} SYNTPDATA, *PSYNTPDATA, *LPSYNTPDATA;

class CSynTP : private CWindow, private CRawInput
{
public:
	CSynTP();
	~CSynTP();

public:
	BOOL		bStartReceiver(HWND hWnd, LPCTSTR szIPAddress, int iPort);
	void		vStopReceiver();
	BOOL		bStartSender(HWND hWnd, LPCTSTR szIPAddress, int iPort);
	void		vStoptSender();

private:
	BOOL		bRegister(HINSTANCE hInstance, LPCTSTR szWindowClass);
	BOOL		bGetSetSynTPSpecFromReg();
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	BOOL		Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
	void		Cls_OnDestroy(HWND hWnd);
	void		Cls_OnInput(HWND hWnd, DWORD dwFlags, HRAWINPUT hRawInput);
	void		Cls_OnInputDeviceChange(HWND hWnd, WPARAM wParam, HANDLE hDevice);
	void		Cls_OnMouseWheelEx(HWND hWnd, int xPos, int yPos, int zDelta, UINT fwKeys);
	void		Cls_OnMouseHWheelEx(HWND hWnd, int xPos, int yPos, int zDelta, UINT fwKeys);

	BOOL		bSendInput(DWORD dwFlags, int xPos, int yPos, int zDelta);
	void		vRawInputHIDHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput);
	void		vSynTPMouseData(PCHAR Report);
	void		vSynTPSendMouseData(UINT message, int iDelta);
	static BOOL WINAPI	bReceivePacketTheadRoutine(LPVOID lpvParam);

public:

private:
	CRawInput		*HIDRawInput;

	LPSYNTPDATA		lpSynTPData;
	ULONGLONG		uuTickCount64;

	CTCPIP			*Sender;
	CTCPIP			*Receiver;

	CThread			*ReceivePacketThead;
};


/* = EOF = */