//
// SynTP.cpp
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
#include "SynTP.h"
#include "CommonDef.h"
#include "..\MiscLIB\CRegistry.h"

//
// Define
//
#define	RECIEVEPACKETTHREADID		5
#define	RECIEVEPACKETTHREADNAME		_T("ReceivePacketThead")
#define	WINDOWCLASS					_T("FlushMouseSynTPWindow-{B61BCEB9-5EB4-4835-9882-BDDE47E425E3}")

//
// Global Data
//

//
// Local Data
//

//
// Local Prototype Define
//

//
//class CSynTP : private CWindow, private CRawInput
//
CSynTP::CSynTP()
	:	uuTickCount64(), Sender(), Receiver(), ReceivePacketThead()
{
	HIDRawInput = NULL;
	lpSynTPData = new SYNTPDATA[sizeof(SYNTPDATA)];
	if (lpSynTPData)	ZeroMemory(lpSynTPData, sizeof(SYNTPDATA));
}

CSynTP::~CSynTP()
{
	if (hGetHWND()) {
		SendMessage(hGetHWND(), WM_DESTROY, 0, 0);
	}
	if (Sender)		vStoptSender();
	if (Receiver)	vStopReceiver();
	if (lpSynTPData)	delete [] lpSynTPData;
	lpSynTPData = NULL;
	if (HIDRawInput)	delete HIDRawInput;
	HIDRawInput = NULL;
}

//
// bStartReceiver()
//
BOOL		CSynTP::bStartReceiver(HWND hWnd, LPCTSTR szIPAddress, int iPort)
{
	if (hGetHWND() == NULL) {
		if (!bRegister((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), WINDOWCLASS)) {
			return FALSE;
		}
	}
	if (Receiver == NULL)	Receiver = new CTCPIP;
	if (ReceivePacketThead == NULL)	ReceivePacketThead = new CThread;
	if (Receiver && ReceivePacketThead) {
		if (!Receiver->bOpenPortForReceiveUDPv4(szIPAddress, iPort)) return FALSE;
		if (ReceivePacketThead) {
			if (ReceivePacketThead->bRegister(RECIEVEPACKETTHREADNAME, RECIEVEPACKETTHREADID,
				(LPTHREAD_START_ROUTINE)&bReceivePacketTheadRoutine, this, 0)) {
				ReceivePacketThead->bSetSentinel(TRUE);
				if (!ReceivePacketThead->bStart()) {
					if (ReceivePacketThead != NULL) {
						delete	ReceivePacketThead;
						ReceivePacketThead = NULL;
					}
					ReceivePacketThead = new CThread;
					if (!ReceivePacketThead->bRegister(RECIEVEPACKETTHREADNAME, RECIEVEPACKETTHREADID,
						(LPTHREAD_START_ROUTINE)&bReceivePacketTheadRoutine, this, 0)) {
						return FALSE;
					}
				}
			}
			else {
				return FALSE;
			}
		}

	}
	return TRUE;

}

//
// bStopReceiver()
//
void		CSynTP::vStopReceiver()
{
	if (hGetHWND()) {
		SendMessage(hGetHWND(), WM_DESTROY, 0, 0);
	}
	if (ReceivePacketThead != NULL) {
		ReceivePacketThead->bSetSentinel(FALSE);
		Sleep(10);
		delete	ReceivePacketThead;
		ReceivePacketThead = NULL;
	}
	if (Receiver)	delete Receiver;
	Receiver = NULL;
}

//
// bStartSender()
//
BOOL		CSynTP::bStartSender(HWND hWnd, LPCTSTR szIPAddress, int iPort)
{
	if (lpSynTPData) {
		if (!bGetSetSynTPSpecFromReg())	return FALSE;
	}

	if (Sender == NULL)	Sender = new CTCPIP;
	if (Sender) {
		if (!Sender->bOpenPortForSendUDPv4(szIPAddress, iPort)) return FALSE;
	}
	
	if (hGetHWND() == NULL) {
		if (!bRegister((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), WINDOWCLASS)) {
			return FALSE;
		}
	}
	return TRUE;
}

//
// bStoptSender()
//
void		CSynTP::vStoptSender()
{
	if (hGetHWND()) {
		SendMessage(hGetHWND(), WM_DESTROY, 0, 0);
	}
	if (Sender)	delete Sender;
	Sender = NULL;

}

//
// bRegister()
//
BOOL		CSynTP::bRegister(HINSTANCE hInstance, LPCTSTR szWindowClassName)
{
	if (hInstance == NULL)	return FALSE;

#define CLASSSTYLE (CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE)
	WNDCLASSEX wcex{};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CLASSSTYLE;
		wcex.lpfnWndProc = NULL;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClassName;
		wcex.hIconSm = NULL;

	if (!bRegisterClassEx(&wcex))	return FALSE;

#define		EX_WINDOWSTYLE	(WS_EX_LAYERED | WS_EX_NOACTIVATE)
#define		WINDOWSTYLE		(WS_POPUP)
	if (!bCreateWindowEx(
						EX_WINDOWSTYLE,
						NULL,
						WINDOWSTYLE,
						0, 0,
						0, 0,
						NULL,
						NULL)
		)	return FALSE;


	if (hGetHWND() == NULL)	return FALSE;
	if (!bShowWindow(SW_HIDE))	return FALSE;
	if (!bUpdateWindow())	return FALSE;
	return TRUE;
}

//
// bGetSetSynTPSpecFromReg()
//
BOOL		CSynTP::bGetSetSynTPSpecFromReg()
{
#define SUBKEY		L"SOFTWARE\\Synaptics\\OEM\\TouchPad"
#define VALUE_X		L"MaxLogicalX"
#define VALUE_Y		L"MaxLogicalY"
#define VALUE_LX	L"MaxPhysicalX"
#define VALUE_LY	L"MaxPhysicalY"

	CRegistry	*CReg = new CRegistry;
	BOOL		bRet = TRUE;
	DWORD		dwLogicalX = 0, dwLogicalY = 0;
	DWORD		dwPhysicalX = 0, dwPhysicalY = 0;
	if (CReg) {
		if (!CReg->bReadSystemRegValueDWORD(HKEY_LOCAL_MACHINE, SUBKEY, VALUE_X, &dwLogicalX)) {
			bRet = FALSE;
			goto Cleanup;
		}
		if (!CReg->bReadSystemRegValueDWORD(HKEY_LOCAL_MACHINE, SUBKEY, VALUE_Y, &dwLogicalY)) {
			bRet = FALSE;
			goto Cleanup;
		}
		if (!CReg->bReadSystemRegValueDWORD(HKEY_LOCAL_MACHINE, SUBKEY, VALUE_LX, &dwPhysicalX)) {
			bRet = FALSE;
			goto Cleanup;
		}
		if (!CReg->bReadSystemRegValueDWORD(HKEY_LOCAL_MACHINE, SUBKEY, VALUE_LY, &dwPhysicalY)) {
			bRet = FALSE;
			goto Cleanup;
		}
	}
Cleanup:
	if (CReg)	delete	CReg;
	if (bRet) {
		lpSynTPData->rcSynTP.left = 0;
		lpSynTPData->rcSynTP.top = dwLogicalY;
		lpSynTPData->rcSynTP.right = dwLogicalX;
		lpSynTPData->rcSynTP.bottom = 0;
		lpSynTPData->sTouchZoneX = (SHORT)(dwLogicalX - dwPhysicalX);
		lpSynTPData->sTouchZoneY = (SHORT)(dwLogicalY - dwPhysicalY);
	}
	return bRet;
#undef SUBKEY
#undef VALUE_X
#undef VALUE_Y
#undef VALUE_LX
#undef VALUE_LY
}

//
// WndProc()
//
LRESULT CALLBACK CSynTP::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// void		Cls_OnInput(HWND hWnd, DWORD dwFlags, HRAWINPUT hRawInput);
#define HANDLE_WM_INPUT(hWnd, wParam, lParam, fn) ((fn)((hWnd), (DWORD)(GET_RAWINPUT_CODE_WPARAM(wParam)), (HRAWINPUT)(lParam)), 0L)

	// void		Cls_OnInputDeviceChange(HWND hWnd, WPARAM wParam, HANDLE hDevice);
#define HANDLE_WM_INPUT_DEVICE_CHANGE(hWnd, wParam, lParam, fn) ((fn)((hWnd), (WPARAM)(wParam), (HANDLE)(lParam)), 0L)


	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_INPUT, Cls_OnInput);
		HANDLE_MSG(hWnd, WM_INPUT_DEVICE_CHANGE, Cls_OnInputDeviceChange);
		HANDLE_MSG(hWnd, WM_MOUSEWHEELEX, Cls_OnMouseWheelEx);
		HANDLE_MSG(hWnd, WM_MOUSEHWHEELEX, Cls_OnMouseHWheelEx);
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//
// WM_CREATE
// Cls_OnCreate()
//
BOOL		CSynTP::Cls_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lpCreateStruct);

	if (Sender) {
#define	uiNumDevices	1
		RAWINPUTDEVICE	RawInputDevice[uiNumDevices]{};
		int	i = 0;
		RawInputDevice[i].hwndTarget = hWnd;	RawInputDevice[i].dwFlags = (RIDEV_INPUTSINK | RIDEV_DEVNOTIFY);
		RawInputDevice[i].usUsagePage =  HID_USAGE_PAGE_VENDOR_DEFINED_BEGIN;	RawInputDevice[i].usUsage = HID_USAGE_DIGITIZER_DIGITIZER;
		HIDRawInput = new CRawInput();
		if (HIDRawInput != NULL) {
			if (!HIDRawInput->bRegisterRawInputDevices(RawInputDevice, uiNumDevices)) {
				return FALSE;
			}
		}
#undef uiNumDevices
	}
	return TRUE;
}

//
// WM_DESTROY
// Cls_OnDestroy()
//
void		CSynTP::Cls_OnDestroy(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);

	if (ReceivePacketThead != NULL) {
		ReceivePacketThead->bSetSentinel(FALSE);
		Sleep(10);
		delete	ReceivePacketThead;
		ReceivePacketThead = NULL;
	}

	if (HIDRawInput != NULL) {
		delete HIDRawInput;
		HIDRawInput = NULL;
	}
}

//
// Cls_OnMouseWheelEx()
//
void		CSynTP::Cls_OnMouseWheelEx(HWND hWnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(fwKeys);

	if (zDelta == 0)	return;
	bSendInput(MOUSEEVENTF_WHEEL, xPos, yPos, zDelta);
	return;
}

//
// Cls_OnMouseWheelEx()
//
void		CSynTP::Cls_OnMouseHWheelEx(HWND hWnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(fwKeys);

	if (zDelta == 0)	return;
	bSendInput(MOUSEEVENTF_HWHEEL, xPos, yPos, zDelta);
	return;
}

//
// bSedInput()
//
BOOL		CSynTP::bSendInput(DWORD dwFlags, int xPos, int yPos, int zDelta)
{
	INPUT	Inputs[1]{};
	Inputs->type = INPUT_MOUSE;
	Inputs->mi.dx = xPos;
	Inputs->mi.dy = yPos;
	Inputs->mi.mouseData = zDelta;
	Inputs->mi.dwFlags = dwFlags;
	Inputs->mi.time = 0;
	Inputs->mi.dwExtraInfo = GetMessageExtraInfo();
	if (SendInput(1, Inputs, sizeof(INPUT)) == 1) {
		return TRUE;
	}
	return FALSE;
}

// 
// WM_INPUT
// Cls_OnInput()
//
void		CSynTP::Cls_OnInput(HWND hWnd, DWORD dwFlags, HRAWINPUT hRawInput)
{
	HWND	hPlayerWnd = FindWindowEx(NULL, NULL, L"VMPlayerFrame", NULL);	
	if (hPlayerWnd == NULL)	return;

	if (HIDRawInput)	vRawInputDevicesHandler(hWnd, dwFlags, hRawInput);
}

// 
// WM_INPUT_DEVICE_CHANGE
// Cls_OnInputDeviceChange()
//
void		CSynTP::Cls_OnInputDeviceChange(HWND hWnd, WPARAM wParam, HANDLE hDevice)
{
	if (HIDRawInput)	vInputDeviceChange(hWnd, wParam, hDevice);
}

void	CSynTP::vRawInputHIDHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(dwFlags);
	UNREFERENCED_PARAMETER(lpRawInput);

	RAWHID		RawHID = (RAWHID)lpRawInput->data.hid;
	NTSTATUS	ntStatus = 0;
	UINT		uPreparsedDataSize = 0;
	UINT		uResult;
	if((uResult = GetRawInputDeviceInfo(lpRawInput->header.hDevice, RIDI_PREPARSEDDATA, NULL, &uPreparsedDataSize)) == (UINT)(-1))	return;
	HANDLE	hPreparsedDataHeap = GetProcessHeap();
	PHIDP_PREPARSED_DATA pPreparsedData = (PHIDP_PREPARSED_DATA)HeapAlloc(hPreparsedDataHeap, 0, uPreparsedDataSize);
	if (pPreparsedData != NULL) {
		ZeroMemory(pPreparsedData, uPreparsedDataSize);
		uResult = GetRawInputDeviceInfo(lpRawInput->header.hDevice, RIDI_PREPARSEDDATA, pPreparsedData, &uPreparsedDataSize);
		if ((uResult != 0) && (uResult != (UINT)(-1))) {
			HIDP_CAPS	Capabilities{};
			ntStatus = HidP_GetCaps(pPreparsedData, &Capabilities);
			if (ntStatus == HIDP_STATUS_SUCCESS) {
				USHORT	ValueCapsLength = Capabilities.NumberInputValueCaps;
				HANDLE	hInputValueCapsHeap = GetProcessHeap();
				PHIDP_VALUE_CAPS pInputValueCaps = (PHIDP_VALUE_CAPS)HeapAlloc(hInputValueCapsHeap, 0, ValueCapsLength * sizeof(HIDP_VALUE_CAPS));
				if (pInputValueCaps != NULL) {
					ZeroMemory(pInputValueCaps, ValueCapsLength);
					ntStatus = HidP_GetValueCaps(HidP_Input, pInputValueCaps, &ValueCapsLength, pPreparsedData);
					if (ntStatus == HIDP_STATUS_SUCCESS) {
						DWORD	dwCount = lpRawInput->data.hid.dwCount;
						for (DWORD dwDataCount = 0; dwDataCount < dwCount ; dwDataCount++) {
							for (int i = 0; i < ValueCapsLength; i++) {
								PCHAR	Report = (PCHAR)(lpRawInput->data.hid.bRawData) + (dwDataCount * (lpRawInput->data.hid.dwSizeHid));
								ULONG	UsageValue = 0;
								ntStatus = HidP_GetUsageValue(HidP_Input, pInputValueCaps[i].UsagePage, pInputValueCaps[i].LinkCollection, pInputValueCaps[i].NotRange.Usage, &UsageValue, pPreparsedData, Report, lpRawInput->data.hid.dwSizeHid);
								if (ntStatus == HIDP_STATUS_SUCCESS) {
									vSynTPMouseData(Report);
								}
								else {
									continue;
								}
							}
						}
					}
					HeapFree(hInputValueCapsHeap, 0, pInputValueCaps);
				}
			}
		}
		HeapFree(hPreparsedDataHeap, 0, pPreparsedData);
	}
}

//
// vSynTPMouseData()
//
void		CSynTP::vSynTPMouseData(PCHAR Report)
{
#define	makeAxis(hi, lo)	(((DWORD)hi << 8) & 0xff00) | (((DWORD)lo & 0x00ff))
#define STOP	0
#define UP		1
#define DOWN	(-1)
#define LEFT	1
#define RIGHT	(-1)

	if (lpSynTPData && Report) {
		LPSYNTPRAWDATA	lpSynTPRawData = (LPSYNTPRAWDATA)Report;

		SHORT oldX = makeAxis(lpSynTPData->stSynRawData.stFinger1.hiX, lpSynTPData->stSynRawData.stFinger1.loX);
		SHORT oldY = makeAxis(lpSynTPData->stSynRawData.stFinger1.hiY, lpSynTPData->stSynRawData.stFinger1.loY);
		SHORT newX = makeAxis(lpSynTPRawData->stFinger1.hiX, lpSynTPRawData->stFinger1.loX);
		SHORT newY = makeAxis(lpSynTPRawData->stFinger1.hiY, lpSynTPRawData->stFinger1.loY);

		if (lpSynTPRawData->stFinger1.bTouched == 0) {
			ZeroMemory(&(lpSynTPData->stSynRawData), sizeof(SYNTPRAWDATA));
			lpSynTPData->iYStart = STOP;	lpSynTPData->iYStart = STOP;
			lpSynTPData->iXStart = STOP;	lpSynTPData->iXStart = STOP;
			lpSynTPData->iWheelStart = STOP;
		}
		else if (lpSynTPData->iYStart == UP) {
			if (newY < (lpSynTPData->rcSynTP.bottom + lpSynTPData->sTouchZoneY)) {
				vSynTPSendMouseData(WM_MOUSEWHEEL, (-20));
			}
			else if ((newY - oldY) != 0) {
				if ((newY - oldY) < 0) {
					vSynTPSendMouseData(WM_MOUSEWHEEL, (newY - oldY) * 5);
				}
				else if ((newY - oldY) > 0) {
					lpSynTPData->iYStart = DOWN;
				}
			}
		}
		else if (lpSynTPData->iYStart == DOWN) {
			if (newY > (lpSynTPData->rcSynTP.top - lpSynTPData->sTouchZoneY)) {
				vSynTPSendMouseData(WM_MOUSEWHEEL, (20));
			}
			else if ((newY - oldY) != 0) {
				if ((newY - oldY) < 0) {
					lpSynTPData->iYStart = UP;
				}
				else if ((newY - oldY) > 0) {
					vSynTPSendMouseData(WM_MOUSEWHEEL, (newY - oldY) * 5);
				}
			}
		}
		else if (lpSynTPData->iXStart == LEFT) {
			if (newX < (lpSynTPData->rcSynTP.left + lpSynTPData->sTouchZoneX)) {
				vSynTPSendMouseData(WM_MOUSEHWHEEL, (-3));
			}
			else if ((newX - oldX) != 0) {
				if ((newX - oldX) < 0) {
					vSynTPSendMouseData(WM_MOUSEHWHEEL, (newX - oldX));
				}
				else if ((newX - oldX) > 0) {
					lpSynTPData->iXStart = RIGHT;
				}
			}
		}
		else if (lpSynTPData->iXStart == RIGHT) {
			if (newX > (lpSynTPData->rcSynTP.right - lpSynTPData->sTouchZoneX)) {
				vSynTPSendMouseData(WM_MOUSEHWHEEL, (3));
			}
			else if ((newX - oldX) != 0) {
				if ((newX - oldX) < 0) {
					lpSynTPData->iXStart = LEFT;
				}
				else if ((newX - oldX) > 0) {
					vSynTPSendMouseData(WM_MOUSEHWHEEL, (newX - oldX));
				}
			}
		}
		else if (lpSynTPData->iWheelStart == UP) {
			SHORT	centerX = (SHORT)(lpSynTPData->rcSynTP.right - lpSynTPData->rcSynTP.left) / 2;
			if ((centerX > newX) && ((newY - oldY) < 0))	lpSynTPData->iWheelStart = DOWN;
			else if ((centerX < newX) && ((newY - oldY) > 0))	lpSynTPData->iWheelStart = DOWN;
			else vSynTPSendMouseData(WM_MOUSEWHEEL, (10));
		}
		else if (lpSynTPData->iWheelStart == DOWN) {
			SHORT	centerX = (SHORT)(lpSynTPData->rcSynTP.right - lpSynTPData->rcSynTP.left) / 2;
			if ((centerX < newX) && ((newY - oldY) < 0))	lpSynTPData->iWheelStart = UP;
			else if ((centerX > newX) && ((newY - oldY) > 0))	lpSynTPData->iWheelStart = UP;
			else vSynTPSendMouseData(WM_MOUSEWHEEL, (-10));
		}
		else if ((lpSynTPRawData->stFinger1.bTouched != 0) && ((lpSynTPRawData->stFinger2.bTouched != 0) || (lpSynTPRawData->stFinger3.bTouched != 0))) {
			if ((newY - oldY) < 0) {
				lpSynTPData->iYStart = UP;
			}
			else if ((newY - oldY) > 0) {
				lpSynTPData->iYStart = DOWN;
			}
		}
		else if ((lpSynTPRawData->stFinger1.bTouched != 0) && ((lpSynTPRawData->stFinger2.bTouched == 0) && (lpSynTPRawData->stFinger3.bTouched == 0))) {
#define	TOUCH_AREA_MARGIN	5
			if (((lpSynTPData->rcSynTP.right - lpSynTPData->sTouchZoneX) < newX)
					&& ((((lpSynTPData->rcSynTP.top - lpSynTPData->rcSynTP.bottom) / 2) - TOUCH_AREA_MARGIN) < newY)
					&& ((((lpSynTPData->rcSynTP.top - lpSynTPData->rcSynTP.bottom) / 2) + TOUCH_AREA_MARGIN) > newY)) {
				if ((newY - oldY) < 0) {
					lpSynTPData->iWheelStart = UP;
				}
				else if ((newY - oldY) > 0) {
					lpSynTPData->iWheelStart = DOWN;
				}
			}
		}
		errno_t err = 0;
		if ((err = memcpy_s(&(lpSynTPData->stSynRawData), sizeof(SynTPRawData), Report, sizeof(SynTPRawData))) != 0) {
		}
	}
}

//
// bReceivePacketTheadRoutine()
//
BOOL		CSynTP::bReceivePacketTheadRoutine(LPVOID lpvParam)
{
	if (lpvParam == NULL)	return FALSE;
	CSynTP	*This = reinterpret_cast<CSynTP*>(lpvParam);

	CHAR	szRecieveData[DATA_BUFFER_SIZE]{};
	do {
		if (This == NULL)	return FALSE;
		ZeroMemory(szRecieveData, sizeof(szRecieveData));
		if (This->Receiver)	This->Receiver->bReceivePackaet(szRecieveData, sizeof(szRecieveData));
		if (szRecieveData[0] == '\0')	continue;
		size_t	size = strnlen_s(szRecieveData, DATA_BUFFER_SIZE);
		if (size < DATA_BUFFER_SIZE)	szRecieveData[(size + 1)] = NULL;
		POINT	pt{};
		GetCursorPos(&pt);
		int	iMouseData = atoi(&szRecieveData[2]);
		if (iMouseData == 0)	continue;
		if (szRecieveData[0] == 'Y') {
			PostMessage(This->hGetHWND(), WM_MOUSEWHEELEX, MAKEWPARAM((0), (iMouseData)), MAKELPARAM((pt.x), (pt.y)));
		}
		else if (szRecieveData[0] == 'X') {
			PostMessage(This->hGetHWND(), WM_MOUSEHWHEELEX, MAKEWPARAM((0), (iMouseData)), MAKELPARAM((pt.x), (pt.y)));
		}
		else continue;
	} while (This->ReceivePacketThead->bCheckSentinel());
	return TRUE;
}

//
// vSynTPSendMouseData()
//
void		CSynTP::vSynTPSendMouseData(UINT message, int iDelta)
{
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(iDelta);

#define SEND_LAP_TIME	50

	ULONGLONG	_uuTickCount64 = GetTickCount64();
	if ((_uuTickCount64 - uuTickCount64) > SEND_LAP_TIME) {
		uuTickCount64 = _uuTickCount64;
		int	_zDelta = (0 - iDelta);

		CHAR	szSendData[DATA_BUFFER_SIZE]{};
		if (message == WM_MOUSEWHEEL) {
			_snprintf_s(szSendData, sizeof(szSendData), "Y %+05d", (SHORT)_zDelta);
		}
		else if (message == WM_MOUSEHWHEEL) {
			_snprintf_s(szSendData, sizeof(szSendData), "X %+05d", (SHORT)_zDelta);
		}
		else return;

		size_t	size = strnlen_s(szSendData, DATA_BUFFER_SIZE);
		if (Sender)	Sender->bSendPacket((LPCSTR)szSendData, (int)((size + 1) * sizeof(CHAR)));
	}
}


/* = EOF = */