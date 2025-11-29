//
// SynTP.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
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

//
// Define
//
#define	RECIEVEPACKETTHREADID		5
#define	RECIEVEPACKETTHREADNAME		_T("ReceivePacketThread")
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
CSynTP::CSynTP(DWORD dwSynTPPadX, DWORD dwSynTPPadY, DWORD dwSynTPEdgeX, DWORD dwSynTPEdgeY)
	: HIDRawInput(NULL), lpSynTPData(new SYNTPDATA[sizeof(SYNTPDATA)]), uuTickCount64(0), Sender(NULL), Receiver(NULL), ReceivePacketThread(NULL)
{
	if (lpSynTPData) {
		ZeroMemory(lpSynTPData, sizeof(SYNTPDATA));
		lpSynTPData->rcSynTP.left = 0;
		lpSynTPData->rcSynTP.top = dwSynTPPadY;
		lpSynTPData->rcSynTP.right = dwSynTPPadX;
		lpSynTPData->rcSynTP.bottom = 0;
		lpSynTPData->sTouchZoneX = (SHORT)dwSynTPEdgeX;
		lpSynTPData->sTouchZoneY = (SHORT)dwSynTPEdgeY;
	}
}

CSynTP::CSynTP(const CSynTP& other)
	: HIDRawInput(NULL), lpSynTPData(new SYNTPDATA[sizeof(SYNTPDATA)]), uuTickCount64(0), Sender(NULL), Receiver(NULL), ReceivePacketThread(NULL)
{
	if (lpSynTPData != NULL) {
		*lpSynTPData = *other.lpSynTPData;
	}
}

CSynTP& CSynTP::operator = (const CSynTP& other)
{
	if (this != &other) {
		HIDRawInput = NULL;
		if (lpSynTPData != NULL) {
			delete[] lpSynTPData;
		}
		lpSynTPData = new SYNTPDATA;
		if (lpSynTPData != NULL) {
			*lpSynTPData = *other.lpSynTPData;
		}
		uuTickCount64 = 0;
		Sender = NULL;
		Receiver = NULL;
		ReceivePacketThread = NULL;
	}
	return *this;
}

CSynTP::~CSynTP()
{
	if (hGetHWND()) {
		SendMessage(hGetHWND(), WM_DESTROY, 0, 0);
	}
	if (Sender)	delete Sender;
	Sender = NULL;
	if (ReceivePacketThread != NULL) {
		ReceivePacketThread->bSetSentinel(FALSE);
		SleepEx(10, FALSE);
		delete	ReceivePacketThread;
		ReceivePacketThread = NULL;
	}
	if (Receiver)	delete Receiver;
	Receiver = NULL;
	if (lpSynTPData)	delete [] lpSynTPData;
	lpSynTPData = NULL;
	if (HIDRawInput)	delete HIDRawInput;
	HIDRawInput = NULL;
}

//
// bStartReceiver()
//
BOOL		CSynTP::bStartReceiver(HWND hWnd, int iPort)
{
	if ((hWnd == NULL) || (iPort == 0))	return FALSE;
	if (hGetHWND() == NULL) {
		if (!bRegister((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), WINDOWCLASS)) {
			return FALSE;
		}
	}
	BOOL	bRet = FALSE;
	if (Receiver == NULL)	Receiver = new CTCPIP;
	if (ReceivePacketThread == NULL)	ReceivePacketThread = new CThread;
	if (Receiver && ReceivePacketThread) {
		if (!Receiver->bOpenPortForReceiveUDPv4(iPort)) goto Cleanup;
		if (ReceivePacketThread) {
			if (!ReceivePacketThread->bRegister(RECIEVEPACKETTHREADNAME, RECIEVEPACKETTHREADID,
				(LPTHREAD_START_ROUTINE)&bReceivePacketThreadRoutine, this, 0))	{
				goto Cleanup;
			}
			ReceivePacketThread->bSetSentinel(TRUE);
			if (!ReceivePacketThread->bStart()) {
				goto Cleanup;
			}
			bRet = TRUE;
		}
	}

Cleanup:
	if (!bRet) {
		if (ReceivePacketThread != NULL) {
			ReceivePacketThread->bSetSentinel(FALSE);
			delete	ReceivePacketThread;
			ReceivePacketThread = NULL;
		}
		if (Receiver != NULL) {
			delete Receiver;
			Receiver = NULL;
		}
	}
	return bRet;
}

//
// bStopReceiver()
//
void		CSynTP::vStopReceiver()
{
	if (hGetHWND()) {
		SendMessage(hGetHWND(), WM_DESTROY, 0, 0);
	}
	if (ReceivePacketThread != NULL) {
		ReceivePacketThread->bSetSentinel(FALSE);
		SleepEx(10, FALSE);
		delete	ReceivePacketThread;
		ReceivePacketThread = NULL;
	}
	if (Receiver)	delete Receiver;
	Receiver = NULL;
}

//
// bStartSender()
//
BOOL		CSynTP::bStartSender(HWND hWnd, LPCTSTR szIPAddress, int iPort)
{
	if ((hWnd == NULL) || (szIPAddress == NULL) || (iPort == 0))	return FALSE;
	if (Sender == NULL)	Sender = new CTCPIP;
	if (Sender) {
		if (!Sender->bOpenPortForSendUDPv4(szIPAddress, iPort)) {
			delete Sender;
			Sender = NULL;
			return FALSE;
		}

		if (hGetHWND() == NULL) {
			if (!bRegister((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), WINDOWCLASS)) {
				if (Sender)	delete Sender;
				Sender = NULL;
				return FALSE;
			}
		}
	}
	return TRUE;
}

//
// bStopSender()
//
void		CSynTP::bStopSender()
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

#define CLASSSTYLE (CS_NOCLOSE)
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

#define		EX_WINDOWSTYLE	(WS_EX_NOACTIVATE)
#define		WINDOWSTYLE		(WS_DISABLED)
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
// WndProc()
//
LRESULT CALLBACK CSynTP::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#define HANDLE_WM_INPUT(hWnd, wParam, lParam, fn) ((fn)((hWnd), (DWORD)(GET_RAWINPUT_CODE_WPARAM(wParam)), (HRAWINPUT)(lParam)), 0L)
#define HANDLE_WM_INPUT_DEVICE_CHANGE(hWnd, wParam, lParam, fn) ((fn)((hWnd), (WPARAM)(wParam), (HANDLE)(lParam)), 0L)


	switch (message) {
		HANDLE_MSG(hWnd, WM_CREATE, Cls_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, Cls_OnDestroy);
		HANDLE_MSG(hWnd, WM_INPUT, Cls_OnInput);
		HANDLE_MSG(hWnd, WM_INPUT_DEVICE_CHANGE, Cls_OnInputDeviceChange);
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

	if (ReceivePacketThread != NULL) {
		ReceivePacketThread->bSetSentinel(FALSE);
		SleepEx(10, FALSE);
		delete	ReceivePacketThread;
		ReceivePacketThread = NULL;
	}

	if (HIDRawInput != NULL) {
		delete HIDRawInput;
		HIDRawInput = NULL;
	}
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
	if (!FindWindowEx(NULL, NULL, L"VMUIFrame", NULL) && !FindWindowEx(NULL, NULL, L"VMPlayerFrame", NULL) && !FindWindowEx(NULL, NULL, L"TscShellContainerClass", NULL)) {
		return;
	}
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
#define	makeAxis(hi, lo)	((SHORT)(((DWORD)hi << 8) & 0xff00) | (((DWORD)lo & 0x00ff)))
#define STOP			0
#define UP				1
#define DOWN			(-1)
#define LEFT			1
#define RIGHT			(-1)

	if (lpSynTPData && Report) {
		LPSYNTPRAWDATA	lpSynTPRawData = reinterpret_cast<LPSYNTPRAWDATA>(Report);

		SHORT	newX = makeAxis(lpSynTPRawData->stFinger1.hiX, lpSynTPRawData->stFinger1.loX);
		SHORT	newY = makeAxis(lpSynTPRawData->stFinger1.hiY, lpSynTPRawData->stFinger1.loY);
		if ((lpSynTPData->rcSynTP.top < newY) || (lpSynTPData->rcSynTP.bottom > newY)
			|| (lpSynTPData->rcSynTP.left > newX) || (lpSynTPData->rcSynTP.right < newX))	return;

		SHORT	deltaX = newX - makeAxis(lpSynTPData->stSynRawData.stFinger1.hiX, lpSynTPData->stSynRawData.stFinger1.loX);
		SHORT	deltaY = newY - makeAxis(lpSynTPData->stSynRawData.stFinger1.hiY, lpSynTPData->stSynRawData.stFinger1.loY);

		if (lpSynTPRawData->stFinger1.bTouched == 0) {
			ZeroMemory(&(lpSynTPData->stSynRawData), sizeof(SYNTPRAWDATA));
			lpSynTPData->iYStart = STOP;
			lpSynTPData->iXStart = STOP;
			lpSynTPData->iWheelStart = STOP;
		}
		else if ((lpSynTPRawData->stFinger1.bTouched != 0) && ((lpSynTPRawData->stFinger2.bTouched != 0) || (lpSynTPRawData->stFinger3.bTouched != 0))) {
			if ((lpSynTPData->iYStart == STOP) && (lpSynTPData->iXStart == STOP)) {
#define	TOUCH_MARGIN	2
				if ((abs(deltaY) + TOUCH_MARGIN) >= abs(deltaX)) {
					if (deltaY < 0) {
						lpSynTPData->iYStart = UP;
						vSynTPSendMouseData(WM_MOUSEWHEEL, (+WHEEL_DELTA));
					}
					else if (deltaY > 0) {
						lpSynTPData->iYStart = DOWN;
						vSynTPSendMouseData(WM_MOUSEWHEEL, (-WHEEL_DELTA));
					}
				}
				else {
					if (deltaX < 0) {
						lpSynTPData->iXStart = LEFT;
						vSynTPSendMouseData(WM_MOUSEHWHEEL, (+WHEEL_DELTA));
					}
					else if (deltaX > 0) {
						lpSynTPData->iXStart = RIGHT;
						vSynTPSendMouseData(WM_MOUSEHWHEEL, (-WHEEL_DELTA));
					}
				}
				return;
			}
		}
		if (lpSynTPRawData->stFinger1.bTouched != 0) {
			if (lpSynTPData->iYStart == UP) {
				if (newY < (lpSynTPData->rcSynTP.bottom + lpSynTPData->sTouchZoneY)) {
					vSynTPSendMouseData(WM_MOUSEWHEEL, (+WHEEL_DELTA) / 3);
				}
				else if (deltaY != 0) {
					if (deltaY < 0) {
						vSynTPSendMouseData(WM_MOUSEWHEEL, (+WHEEL_DELTA));
					}
					else if (deltaY > 0) {
						lpSynTPData->iYStart = DOWN;
					}
				}
			}
			else if (lpSynTPData->iYStart == DOWN) {
				if (newY > (lpSynTPData->rcSynTP.top - lpSynTPData->sTouchZoneY)) {
					vSynTPSendMouseData(WM_MOUSEWHEEL, (-WHEEL_DELTA) / 3);
				}
				else if (deltaY != 0) {
					if (deltaY < 0) {
						lpSynTPData->iYStart = UP;
					}
					else if (deltaY > 0) {
						vSynTPSendMouseData(WM_MOUSEWHEEL, (-WHEEL_DELTA));
					}
				}
			}
			else if (lpSynTPData->iXStart == LEFT) {
				if (newX < (lpSynTPData->rcSynTP.left + lpSynTPData->sTouchZoneX)) {
					vSynTPSendMouseData(WM_MOUSEHWHEEL, (+WHEEL_DELTA) / 3);
				}
				else if (deltaX != 0) {
					if (deltaX < 0) {
						vSynTPSendMouseData(WM_MOUSEHWHEEL, (+WHEEL_DELTA));
					}
					else if (deltaX > 0) {
						lpSynTPData->iXStart = RIGHT;
					}
				}
			}
			else if (lpSynTPData->iXStart == RIGHT) {
				if (newX > (lpSynTPData->rcSynTP.right - lpSynTPData->sTouchZoneX)) {
					vSynTPSendMouseData(WM_MOUSEHWHEEL, (-WHEEL_DELTA) / 3);
				}
				else if (deltaX != 0) {
					if (deltaX < 0) {
						lpSynTPData->iXStart = LEFT;
					}
					else if (deltaX > 0) {
						vSynTPSendMouseData(WM_MOUSEHWHEEL, (-WHEEL_DELTA));
					}
				}
			}
			else if (lpSynTPData->iWheelStart == UP) {
				SHORT	centerX = (SHORT)(lpSynTPData->rcSynTP.right - lpSynTPData->rcSynTP.left) / 2;
				if ((centerX >= newX) && (deltaY < 0))		lpSynTPData->iWheelStart = DOWN;
				else if ((centerX < newX) && (deltaY > 0))	lpSynTPData->iWheelStart = DOWN;
				else vSynTPSendMouseData(WM_MOUSEWHEEL, (-WHEEL_DELTA));
			}
			else if (lpSynTPData->iWheelStart == DOWN) {
				SHORT	centerX = (SHORT)(lpSynTPData->rcSynTP.right - lpSynTPData->rcSynTP.left) / 2;
				if ((centerX <= newX) && (deltaY < 0))		lpSynTPData->iWheelStart = UP;
				else if ((centerX > newX) && (deltaY > 0))	lpSynTPData->iWheelStart = UP;
				else vSynTPSendMouseData(WM_MOUSEWHEEL, (+WHEEL_DELTA));
			}
			else if ((lpSynTPRawData->stFinger1.bTouched != 0) && ((lpSynTPRawData->stFinger2.bTouched == 0) && (lpSynTPRawData->stFinger3.bTouched == 0))) {
#define		TOUCH_AREA_MARGIN	5
				if (((lpSynTPData->rcSynTP.right - lpSynTPData->sTouchZoneX) < newX)
					&& ((((lpSynTPData->rcSynTP.top - lpSynTPData->rcSynTP.bottom) / 2) - TOUCH_AREA_MARGIN) < newY)
					&& ((((lpSynTPData->rcSynTP.top - lpSynTPData->rcSynTP.bottom) / 2) + TOUCH_AREA_MARGIN) > newY)) {
					if (deltaY < 0) {
						lpSynTPData->iWheelStart = DOWN;
					}
					else if (deltaY > 0) {
						lpSynTPData->iWheelStart = UP;
					}
				}
			}
		}
		memcpy_s(&(lpSynTPData->stSynRawData), sizeof(SynTPRawData), Report, sizeof(SynTPRawData));
	}
}

//
// bReceivePacketThreadRoutine()
//
BOOL		CSynTP::bReceivePacketThreadRoutine(LPVOID lpvParam)
{
	if (lpvParam == NULL)	return FALSE;
	CSynTP	*This = reinterpret_cast<CSynTP*>(lpvParam);

#define RECEIVE_LAP_TIME	1
	ULONGLONG	uuTickCount64 = 0, _uuTickCount64 = 0;
	CHAR	szReceiveData[SYNTP_DATA_BUFFER_SIZE]{};

	if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST)) {
		return FALSE;
	}
	do {
		ZeroMemory(szReceiveData, sizeof(szReceiveData));
		if (This->Receiver)	This->Receiver->bReceivePacket(szReceiveData, sizeof(szReceiveData));
		if (szReceiveData[0] == '\0')	continue;
		_uuTickCount64 = GetTickCount64();
		if ((_uuTickCount64 - uuTickCount64) > RECEIVE_LAP_TIME) {
			uuTickCount64 = _uuTickCount64;
			size_t	size = strnlen_s(szReceiveData, SYNTP_DATA_BUFFER_SIZE);
			if (size < SYNTP_DATA_BUFFER_SIZE)	szReceiveData[size] = NULL;
			POINT	pt{};
			GetCursorPos(&pt);
			int	iMouseData = atoi(&szReceiveData[2]);
			if (iMouseData == 0)	continue;
			if (szReceiveData[0] == 'Y') {
				This->bSendInput(MOUSEEVENTF_WHEEL, pt.x, pt.y, iMouseData);
			}
			else if (szReceiveData[0] == 'X') {
				This->bSendInput(MOUSEEVENTF_HWHEEL, pt.x, pt.y, iMouseData);
			}
			else continue;
		}
	} while (This->ReceivePacketThread->bCheckSentinel());
	return TRUE;
}

//
// vSynTPSendMouseData()
//
void		CSynTP::vSynTPSendMouseData(UINT message, SHORT zDelta)
{
	UNREFERENCED_PARAMETER(message);

#define SEND_LAP_TIME	150
	ULONGLONG	_uuTickCount64= GetTickCount64();
	if ((_uuTickCount64 - uuTickCount64) > SEND_LAP_TIME) {
		uuTickCount64 = _uuTickCount64;
		CHAR	szSendData[SYNTP_DATA_BUFFER_SIZE]{};
		if (message == WM_MOUSEWHEEL) {
			_snprintf_s(szSendData, sizeof(szSendData), "Y %+05d", (SHORT)zDelta);
		}
		else if (message == WM_MOUSEHWHEEL) {
			_snprintf_s(szSendData, sizeof(szSendData), "X %+05d", (SHORT)zDelta);
		}
		else return;

		size_t	cbSize = strnlen_s(szSendData, SYNTP_DATA_BUFFER_SIZE);
		if (Sender)	Sender->bSendPacket((LPCSTR)szSendData, (int)((cbSize + 1) * sizeof(CHAR)));
	}
}


/* = EOF = */
