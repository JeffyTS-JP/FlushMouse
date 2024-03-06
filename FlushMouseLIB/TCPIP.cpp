//
// TCPIP.cpp
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
#include "TCPIP.h"
#include <commctrl.h>
#include <Mstcpip.h>
#include <Inaddr.h>
#pragma comment(lib, "ntdll.lib")

//
// Define
//

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
// dwGetString2IPAddr()
// 
DWORD		dwGetString2IPv4Addr(LPCTSTR szIPAddr)
{
	LPCTSTR	Terminator = NULL;
	IN_ADDR	Addr{};
	if (RtlIpv4StringToAddress(szIPAddr, FALSE, &Terminator, &Addr) == 0) {
		return (DWORD)MAKEIPADDRESS(Addr.S_un.S_un_b.s_b1, Addr.S_un.S_un_b.s_b2, Addr.S_un.S_un_b.s_b3, Addr.S_un.S_un_b.s_b4);
	}
	return (DWORD)(-1);
}

//
//class CTCPIP
//
CTCPIP::CTCPIP()
	:	lpWSAData(), pSocket(), lpszIPAddr(), iPortNo(), SockAddr_IN()
{
}

CTCPIP::~CTCPIP()
{

	if (pSocket)	closesocket(pSocket);
	pSocket = NULL;
	int iRet = WSACleanup();
	if (iRet != 0) {
	}
	if (lpWSAData)	delete [] lpWSAData;
	lpWSAData = NULL;
	if (lpszIPAddr)	delete [] lpszIPAddr;
	lpszIPAddr = NULL;
}

//
// bOpenPortForReceiveUDPv4()
//
BOOL		CTCPIP::bOpenPortForReceiveUDPv4(LPCTSTR lpszIPAddress, int Port)
{
	if ((Port == 0))	return FALSE;

	if (lpszIPAddr == NULL)	lpszIPAddr = new TCHAR[MAX_LOADSTRING];
	if (lpszIPAddr) {
		ZeroMemory(lpszIPAddr, (MAX_LOADSTRING * sizeof(TCHAR)));
		if (lpszIPAddress)	wcsncpy_s(lpszIPAddr, MAX_LOADSTRING, lpszIPAddress, MAX_LOADSTRING);
	}

	if (lpWSAData == NULL)	lpWSAData = new WSADATA[sizeof(WSADATA)];
	if (lpWSAData)	ZeroMemory(lpWSAData, sizeof(WSADATA));
	else return FALSE;
	
	if (WSAStartup(MAKEWORD(2, 0), lpWSAData) != 0)	return FALSE;
	if ((pSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
		WSACleanup();
		return FALSE;
	}
	int	iRet = 0;
	iPortNo = Port;
	SockAddr_IN.sin_family = AF_INET;
	if (WSAHtons(pSocket, (USHORT)iPortNo, (PUSHORT)&SockAddr_IN.sin_port) == SOCKET_ERROR) {
		WSACleanup();
		return FALSE;
	}
	u_long	arg = 1;
	iRet = ioctlsocket(pSocket, FIONBIO, &arg);
	if (iRet != 0) {
	}
	TIMEVAL	tv{};
	tv.tv_sec = 0;
	tv.tv_usec = 10;
	iRet = setsockopt(pSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(TIMEVAL));
	if (iRet != 0) {
	}
	if (lpszIPAddress == NULL) {
		SockAddr_IN.sin_addr.S_un.S_addr = INADDR_ANY;
	}
	else {
		iRet = InetPton(SockAddr_IN.sin_family, lpszIPAddr, &SockAddr_IN.sin_addr.S_un.S_addr);
		if (iRet == 0) {
			ADDRINFOW	pHints{};	
			PADDRINFOW	ppResult = NULL;
			SecureZeroMemory(&pHints, sizeof(ADDRINFOW));
			pHints.ai_family = PF_INET;
			pHints.ai_socktype = SOCK_STREAM;
			iRet = GetAddrInfo(lpszIPAddr, NULL, &pHints, &ppResult);
			if (iRet != 0) {
				FreeAddrInfo(ppResult);
				WSACleanup();
				return FALSE;
			}
			SockAddr_IN.sin_addr.S_un = ((LPSOCKADDR_IN)(ppResult->ai_addr))->sin_addr.S_un;
			FreeAddrInfo(ppResult);
		}
		else if (iRet == (-1)) {
			WSACleanup();
			return FALSE;
		}
	}
	iRet = bind(pSocket, (LPSOCKADDR)&SockAddr_IN, sizeof(SockAddr_IN));
	if (iRet != 0) {
		WSACleanup();
		return FALSE;
	}

	return TRUE;
}

//
// bReceivePackaet()
//
BOOL		CTCPIP::bReceivePackaet(LPVOID lpData, int cbSize)
{
	if ((pSocket == NULL) || (lpData == NULL) || (cbSize == 0))	return FALSE;

	int			iRet = 0;
	WSABUF		wsaBuf{};	wsaBuf.len = cbSize;	wsaBuf.buf = (LPSTR)lpData;
	DWORD		NumberOfBytesRecvd = 0;
	DWORD		Flags = 0;
	char		*_lpData = (char *)lpData;
	if (lpszIPAddr[0] == '\0')	iRet = recv(pSocket, _lpData, cbSize, 0);
	else iRet = WSARecvFrom(pSocket, &wsaBuf, 1, &NumberOfBytesRecvd, &Flags, NULL, 0, NULL, NULL);
	if (iRet == SOCKET_ERROR) {
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK)	return TRUE;
		return FALSE;
	}
	return TRUE;
}

//
// bOpenPortForSendUDPv4()
//
BOOL		CTCPIP::bOpenPortForSendUDPv4(LPCTSTR lpszIPAddress, int Port)
{
	if ((lpszIPAddress == NULL) || (Port == 0))	return FALSE;

	if (lpszIPAddr == NULL)	lpszIPAddr = new TCHAR[MAX_LOADSTRING];
	if (lpszIPAddr) {
		ZeroMemory(lpszIPAddr, (MAX_LOADSTRING * sizeof(TCHAR)));
		if (lpszIPAddress)	wcsncpy_s(lpszIPAddr, MAX_LOADSTRING, lpszIPAddress, MAX_LOADSTRING);
	}

	if (lpWSAData == NULL)	lpWSAData = new WSADATA[sizeof(WSADATA)];
	if (lpWSAData)	ZeroMemory(lpWSAData, sizeof(WSADATA));
	else return FALSE;
	
	if (WSAStartup(MAKEWORD(2, 0), lpWSAData) != 0)	return FALSE;
	if ((pSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
		WSACleanup();
		return FALSE;
	}
	int	iRet = 0;
	iPortNo = Port;
	SockAddr_IN.sin_port = htons((USHORT)iPortNo);
	SockAddr_IN.sin_family = AF_INET;
	iRet = InetPton(SockAddr_IN.sin_family, lpszIPAddr, &SockAddr_IN.sin_addr.S_un.S_addr);
	if (iRet == 0) {
		ADDRINFOW	pHints{};	
		PADDRINFOW	ppResult = NULL;
		SecureZeroMemory(&pHints, sizeof(ADDRINFOW));
		pHints.ai_family = PF_INET;
		pHints.ai_socktype = SOCK_STREAM;
		iRet = GetAddrInfo(lpszIPAddr, NULL, &pHints, &ppResult);
		if (iRet != 0) {
			FreeAddrInfo(ppResult);
			WSACleanup();
			return FALSE;
		}
		SockAddr_IN.sin_addr.S_un = ((LPSOCKADDR_IN)(ppResult->ai_addr))->sin_addr.S_un;
		FreeAddrInfo(ppResult);
	}
	else if (iRet == (-1)) {
		WSACleanup();
		return FALSE;
	}

	return TRUE;
}

//
// bSendPacket()
//
BOOL		CTCPIP::bSendPacket(LPCSTR lpszSendData, int cbSize)
{
	if ((pSocket == NULL) || (lpszSendData == NULL) || (cbSize == 0))	return FALSE;
	int	iRet = 0;
	iRet = sendto(pSocket, lpszSendData, cbSize, 0, (LPSOCKADDR)&SockAddr_IN, sizeof(SockAddr_IN));
	if (iRet ==SOCKET_ERROR) {
		return FALSE;
	}
	return TRUE;
}


/* = EOF = */