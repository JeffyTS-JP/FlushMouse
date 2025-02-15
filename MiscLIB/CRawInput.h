// CRawInput.h
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

//
// class CRawInput
//
class CRawInput
{
	public:
		CRawInput();
		virtual	~CRawInput() {};

		BOOL			bRegisterRawInputDevices(PCRAWINPUTDEVICE pRawInputDevices, UINT uiNumDevices) const;
		void			vRawInputDevicesHandler(HWND hWnd, DWORD dwFlags, HRAWINPUT hRawInput);
		void			vInputDeviceChange(HWND hWnd, WPARAM wParam, HANDLE hDevice);
		virtual void	vRawInputMouseHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput);
		virtual void	vRawInputKeyboardHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput);
		virtual void	vRawInputHIDHandler(HWND hWnd, DWORD dwFlags, LPRAWINPUT lpRawInput);

	private:

	protected:
		void			vGetRawInputDeviceList();
		BOOL			bGetRawInputDeviceInfo(HANDLE hDevice);

};

/* = EOF = */