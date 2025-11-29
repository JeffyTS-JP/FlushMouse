//
// CRawInput.cpp
//
//		Copyright (C) 1993- JeffyTS. All rights reserved.
//		Licensed under the GPL-2.0 License.
//
// No.      Date		    Name		    Reason & Document
// -------+-----------+-----------+-------------------------------------------- -
// #0000	2024/03/20  JeffyTS  	New edit.
//

//
// Include
//
#pragma once
#include "pch.h"
#include "CRawInput.h"

//
// Define
//

//
// class CRawInput
//
CRawInput::CRawInput()
{
}

//
// bRegisterRawInputDevices()
//
BOOL		CRawInput::bRegisterRawInputDevices(PCRAWINPUTDEVICE pRawInputDevices, UINT uiNumDevices) const
{
	if (!RegisterRawInputDevices(pRawInputDevices, uiNumDevices, sizeof(RAWINPUTDEVICE))) {
		return FALSE;
	}
	return TRUE;
}

//
// vRawInputDevicesHandler()
//
void		CRawInput::vRawInputDevicesHandler(HWND hWnd, DWORD dwFlags, HRAWINPUT hRawInput)
{
	UINT	uBufferSize = 0;
	UINT	cbSize = 0;
	if ((uBufferSize = GetRawInputData(hRawInput, RID_INPUT, NULL, &cbSize, sizeof(RAWINPUTHEADER))) != (-1)) {
		HANDLE	hRawInputHeap = GetProcessHeap();
		if (hRawInputHeap != NULL) {
			LPRAWINPUT	lpRawInput = (LPRAWINPUT)HeapAlloc(hRawInputHeap, 0, cbSize);
			if (lpRawInput != NULL) {
				ZeroMemory(lpRawInput, cbSize);
				if ((uBufferSize = GetRawInputData(hRawInput, RID_INPUT, lpRawInput, &cbSize, sizeof(RAWINPUTHEADER))) != (-1)) {
					switch (lpRawInput->header.dwType) {
					case RIM_TYPEMOUSE:
						vRawInputMouseHandler(hWnd, dwFlags, lpRawInput);
						break;
					case RIM_TYPEKEYBOARD:
						vRawInputKeyboardHandler(hWnd, dwFlags, lpRawInput);
						break;
					case RIM_TYPEHID:
						vRawInputHIDHandler(hWnd, dwFlags, lpRawInput);
						break;
					default:
						break;
					}

				}
				HeapFree(hRawInputHeap, 0, lpRawInput);
			}
		}
	}
}

void	CRawInput::vInputDeviceChange(HWND hWnd, WPARAM wParam, HANDLE hDevice)
{
	UNREFERENCED_PARAMETER(hWnd);
	switch (wParam) {
	case GIDC_ARRIVAL:
		if (hDevice != NULL)	bGetRawInputDeviceInfo(hDevice);
		break;
	case GIDC_REMOVAL:
		break;
	}
}

//
// bGetRawInputDeviceList()
//
void		CRawInput::vGetRawInputDeviceList()
{
	UINT uiNumDevices = 0;
	GetRawInputDeviceList(NULL, &uiNumDevices, sizeof(RAWINPUTDEVICELIST));
	PRAWINPUTDEVICELIST	pRawInputDeviceList = new RAWINPUTDEVICELIST[sizeof(RAWINPUTDEVICELIST) * uiNumDevices];
	if (pRawInputDeviceList) {
		ZeroMemory(pRawInputDeviceList, sizeof(RAWINPUTDEVICELIST) * uiNumDevices);
		UINT	uResult;
		if ((uResult = GetRawInputDeviceList(pRawInputDeviceList, &uiNumDevices, sizeof(RAWINPUTDEVICELIST))) != (UINT)(-1)) {
			for(UINT i = 0; i < uiNumDevices; i++ ) {
				if (pRawInputDeviceList[i].hDevice != NULL) {
					if (!bGetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice))	break;
				}
			}
		}
		if (pRawInputDeviceList)	delete [] pRawInputDeviceList;
	}
}

//
// bGetRawInputDeviceInfo()
//
BOOL		CRawInput::bGetRawInputDeviceInfo(HANDLE hDevice)
{
	if (hDevice == NULL)	return FALSE;
	BOOL	bRet = FALSE;
	UINT	uResult;
	UINT	cbSize = 0;
	uResult = GetRawInputDeviceInfo(hDevice, RIDI_DEVICENAME, NULL, &cbSize);
	if (uResult != (UINT)(-1)) {
		LPTSTR lpszDeviceName = new TCHAR[cbSize + 1];
		if (lpszDeviceName) {
			ZeroMemory(lpszDeviceName, ((cbSize + 1) * sizeof(TCHAR)) );
			uResult = GetRawInputDeviceInfo(hDevice, RIDI_DEVICENAME, lpszDeviceName, &cbSize);
			if ((uResult != 0) && (uResult != (UINT)(-1))) {
				LPRID_DEVICE_INFO lpRIDDeviceInfo = new RID_DEVICE_INFO[sizeof(RID_DEVICE_INFO)];
				if (lpRIDDeviceInfo) {
					ZeroMemory(lpRIDDeviceInfo, sizeof(RID_DEVICE_INFO));
					lpRIDDeviceInfo->cbSize = sizeof(RID_DEVICE_INFO);
					cbSize = lpRIDDeviceInfo->cbSize;
					uResult = GetRawInputDeviceInfo(hDevice, RIDI_DEVICEINFO, lpRIDDeviceInfo, &cbSize);
					if ((uResult != 0) && (uResult != (UINT)(-1))) {
						bRet = TRUE;
					}
					if (lpRIDDeviceInfo)	delete [] lpRIDDeviceInfo;
				}
			}
			if (lpszDeviceName)	delete [] lpszDeviceName;
		}
	}
	return bRet;
}

//
// vRawInputMouseHandler()
//
void	CRawInput::vRawInputMouseHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(dwFlags);
	UNREFERENCED_PARAMETER(lpRawInput);
}

//
// vRawInputKeyboardHandler()
//
void	CRawInput::vRawInputKeyboardHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(dwFlags);
	UNREFERENCED_PARAMETER(lpRawInput);
}

//
// vRawInputHIDHandler()
//
void	CRawInput::vRawInputHIDHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(dwFlags);
	UNREFERENCED_PARAMETER(lpRawInput);
}


/* = EOF = */
