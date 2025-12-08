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
constexpr UINT MAX_RAWINPUT_SIZE = 1u << 20;

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
	UINT	cbSize = 0;
	if (GetRawInputData(hRawInput, RID_INPUT, NULL, &cbSize, sizeof(RAWINPUTHEADER)) == (UINT)(-1)) {
		return;
	}

	if (cbSize == 0 || cbSize > MAX_RAWINPUT_SIZE) {
		return;
	}

	std::vector<BYTE> buffer;
	try {
		buffer.resize(cbSize);
	}
	catch (...) {
		return;
	}

	if (GetRawInputData(hRawInput, RID_INPUT, buffer.data(), &cbSize, sizeof(RAWINPUTHEADER)) == (UINT)(-1)) {
		return;
	}

	LPRAWINPUT lpRawInput = reinterpret_cast<LPRAWINPUT>(buffer.data());
	if (!lpRawInput) return;

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
// vGetRawInputDeviceList()
//
void		CRawInput::vGetRawInputDeviceList()
{
	UINT uiNumDevices = 0;
	if (GetRawInputDeviceList(NULL, &uiNumDevices, sizeof(RAWINPUTDEVICELIST)) == (UINT)(-1)) {
		return;
	}
	if (uiNumDevices == 0) return;

	std::vector<RAWINPUTDEVICELIST> list;
	try {
		list.resize(uiNumDevices);
	}
	catch (...) {
		return;
	}

	if (GetRawInputDeviceList(list.data(), &uiNumDevices, sizeof(RAWINPUTDEVICELIST)) == (UINT)(-1)) {
		return;
	}

	for (UINT i = 0; i < uiNumDevices; i++) {
		if (list[i].hDevice != NULL) {
			(void)bGetRawInputDeviceInfo(list[i].hDevice);
		}
	}
}

//
// bGetRawInputDeviceInfo()
//
BOOL		CRawInput::bGetRawInputDeviceInfo(HANDLE hDevice)
{
	if (hDevice == NULL)	return FALSE;
	UINT	cbSize = 0;
	UINT uResult = GetRawInputDeviceInfo(hDevice, RIDI_DEVICENAME, NULL, &cbSize);
	if (uResult == (UINT)(-1) || cbSize == 0) {
		return FALSE;
	}

	std::vector<TCHAR> nameBuf;
	try {
		nameBuf.resize(static_cast<std::vector<TCHAR, std::allocator<wchar_t>>::size_type>(cbSize) + 1);
	}
	catch (...) {
		return FALSE;
	}
	ZeroMemory(nameBuf.data(), (cbSize + 1) * sizeof(TCHAR));

	uResult = GetRawInputDeviceInfo(hDevice, RIDI_DEVICENAME, nameBuf.data(), &cbSize);
	if (uResult == (UINT)(-1) || uResult == 0) {
		return FALSE;
	}

	RID_DEVICE_INFO info{};
	UINT infoSize = sizeof(RID_DEVICE_INFO);
	uResult = GetRawInputDeviceInfo(hDevice, RIDI_DEVICEINFO, &info, &infoSize);
	if (uResult == (UINT)(-1) || infoSize == 0) {
		return FALSE;
	}

	return TRUE;
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
