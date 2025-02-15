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
#include <future>

//
// Define
//

//
// Global Data
//

//
// Local Data
//
static BOOL	bReturn_CheckExistHostname = FALSE;

//
// Local Prototype Define
//
static BOOL	bIsPrivateIPv4Addr(DWORD dwIPv4Addr);

//
// dwGetString2IPAddr()
// 
DWORD		dwGetString2IPv4Addr(LPCTSTR lpszIPAddress)
{
	if (lpszIPAddress == NULL)	return (DWORD)(-1);
	LPCTSTR	Terminator = NULL;
	IN_ADDR	Addr{};
	if (RtlIpv4StringToAddress(lpszIPAddress, FALSE, &Terminator, &Addr) == 0) {
		return (DWORD)MAKEIPADDRESS(Addr.S_un.S_un_b.s_b1, Addr.S_un.S_un_b.s_b2, Addr.S_un.S_un_b.s_b3, Addr.S_un.S_un_b.s_b4);
	}
	return (DWORD)(-1);
}

//
// bGetString2IPv4Addr()
// 
BOOL		bGetString2IPv4Addr(LPCTSTR lpszIPAddress, LPTSTR addr1, LPTSTR addr2, LPTSTR addr3, LPTSTR addr4)
{
	if ((lpszIPAddress == NULL) || (addr1 == NULL) || (addr2 == NULL) || (addr3 == NULL) || (addr4 == NULL))	return FALSE;
	LPCTSTR	Terminator = NULL;
	IN_ADDR	Addr{};
	if (RtlIpv4StringToAddress(lpszIPAddress, FALSE, &Terminator, &Addr) == 0) {
		if (_itow_s(Addr.S_un.S_un_b.s_b1, addr1, 4, 10) != 0)	return FALSE;
		if (_itow_s(Addr.S_un.S_un_b.s_b2, addr2, 4, 10) != 0)	return FALSE;
		if (_itow_s(Addr.S_un.S_un_b.s_b3, addr3, 4, 10) != 0)	return FALSE;
		if (_itow_s(Addr.S_un.S_un_b.s_b4, addr4, 4, 10) != 0)	return FALSE;
		return TRUE;
	}
	return FALSE;
}

//
// bIsPrivateAddress()
//
BOOL		bIsPrivateAddress(LPCTSTR lpszIPAddress)
{
	if (lpszIPAddress == NULL)	return FALSE;
	DWORD	dwAddr = 0;
	if ((dwAddr = dwGetString2IPv4Addr(lpszIPAddress)) != (DWORD)(-1)) {
		return bIsPrivateIPv4Addr(dwAddr);
	}
	return FALSE;
}

//
// bIsPrivateIPv4Addr()
//
static BOOL	bIsPrivateIPv4Addr(DWORD dwIPv4Addr)
{
	if (((0x0a000000 <= dwIPv4Addr) && (dwIPv4Addr <=0x0affffff))		// Class A
		|| ((0xac100000 <= dwIPv4Addr) && (dwIPv4Addr <=0xac1fffff))	// Class B
		|| ((0xc0a80000 <= dwIPv4Addr) && (dwIPv4Addr <=0xc0a8ffff))) {	// Class C
		return TRUE;
	}
	return FALSE;
}

//
// bCheckExistHostnameIPv4() 
//
BOOL		bCheckExistHostnameIPv4(LPCTSTR lpszHostname, int iTimeOut)
{
	if (lpszHostname == NULL)	return FALSE;
	if (!(wcsnlen_s(lpszHostname, MAX_FQDN) > 0))	return FALSE;

	std::future<BOOL> _future;
	std::promise<BOOL> _promise;
	_future = _promise.get_future();
	try {
		std::thread([&](std::promise<BOOL> _promise) {
			LPWSADATA	lpWSAData = new WSADATA[sizeof(WSADATA)];
			if (lpWSAData)	ZeroMemory(lpWSAData, sizeof(WSADATA));
			else return FALSE;;

			if (WSAStartup(MAKEWORD(2, 2), lpWSAData) != 0) {
				delete []	lpWSAData;
				bReturn_CheckExistHostname = FALSE;
				return FALSE;
			}

			ADDRINFOW	pHints{};	
			SecureZeroMemory(&pHints, sizeof(ADDRINFOW));
			pHints.ai_flags  = AI_V4MAPPED;
			pHints.ai_family = PF_INET;
			pHints.ai_socktype = SOCK_STREAM;
			pHints.ai_protocol = IPPROTO_UDP;
			PADDRINFOW	ppResult = NULL;
			if (GetAddrInfo(lpszHostname, NULL, &pHints, &ppResult) != 0) {
				WSACleanup();
				if (ppResult)	FreeAddrInfo(ppResult);
				if (lpWSAData)	delete []	lpWSAData;
				bReturn_CheckExistHostname = FALSE;
				return FALSE;
			}
			if (ppResult && ppResult->ai_addr && (ppResult->ai_addr->sa_family == AF_INET) && (ppResult->ai_addrlen >= 10)) {
				SOCKADDR_IN		SockAddr_IN{};
				SockAddr_IN.sin_addr.S_un = ((LPSOCKADDR_IN)(ppResult->ai_addr))->sin_addr.S_un;
				if (!bIsPrivateIPv4Addr((DWORD)MAKEIPADDRESS(ppResult->ai_addr->sa_data[2], ppResult->ai_addr->sa_data[3], ppResult->ai_addr->sa_data[4], ppResult->ai_addr->sa_data[5]))) {
					WSACleanup();
					if (ppResult)	FreeAddrInfo(ppResult);
					if (lpWSAData)	delete []	lpWSAData;
					bReturn_CheckExistHostname = FALSE;
					return FALSE;
				}
			}
			WSACleanup();
			if (ppResult)	FreeAddrInfo(ppResult);
			if (lpWSAData)	delete []	lpWSAData;
			bReturn_CheckExistHostname = TRUE;
			return TRUE;
			}, std::move(_promise)).detach();

		if(std::future_status::ready == _future.wait_until(std::chrono::system_clock::now() + std::chrono::milliseconds(iTimeOut))) {
			return bReturn_CheckExistHostname;
		}
	}
	catch (...) {
	}
	return FALSE;
}

//
//class CTCPIP
//
CTCPIP::CTCPIP()
	: lpWSAData(NULL), lpszIPAddr(NULL),  pSocket(NULL), iPortNo(0), SockAddr_IN(0)
{
	
}

CTCPIP::~CTCPIP()
{
	if (pSocket) {
		closesocket(pSocket);
	}
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
BOOL		CTCPIP::bOpenPortForReceiveUDPv4(int Port)
{
	if ((Port == 0))	return FALSE;

	if (lpWSAData == NULL)	lpWSAData = new WSADATA[sizeof(WSADATA)];
	if (lpWSAData)	ZeroMemory(lpWSAData, sizeof(WSADATA));
	else return FALSE;
	
	if (WSAStartup(MAKEWORD(2, 2), lpWSAData) != 0)	return FALSE;
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
	TIMEVAL	tv{};
	tv.tv_sec = 0;
	tv.tv_usec = 10;
	iRet = setsockopt(pSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&tv), sizeof(TIMEVAL));
	if (iRet != 0) {
		WSACleanup();
		return FALSE;
	}
	SockAddr_IN.sin_addr.S_un.S_addr = INADDR_ANY;
	iRet = bind((SOCKET)pSocket, reinterpret_cast<const struct sockaddr *>(&SockAddr_IN), (int)sizeof(SockAddr_IN));
	if (iRet != 0) {
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

//
// bReceivePacket()
//
BOOL		CTCPIP::bReceivePacket(LPVOID lpData, int cbSize) const
{
	if ((pSocket == NULL) || (lpData == NULL) || (cbSize == 0))	return FALSE;

	int			iRet = 0;
	WSABUF		wsaBuf{};	wsaBuf.len = cbSize;	wsaBuf.buf = (LPSTR)lpData;
	char		*_lpData = reinterpret_cast<char *>(lpData);

	iRet = recv(pSocket, _lpData, cbSize, 0);
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

	int		iRet = 0;
	BOOL	bRet = FALSE;
	PADDRINFOW	ppResult = NULL;
	
	if (lpszIPAddress == NULL)	return FALSE;
	if (lpszIPAddr == NULL)	lpszIPAddr = new TCHAR[MAX_FQDN];
	if (lpszIPAddr) {
		ZeroMemory(lpszIPAddr, (MAX_FQDN * sizeof(TCHAR)));
		_tcsncpy_s(lpszIPAddr, MAX_FQDN, lpszIPAddress, _TRUNCATE);
	}
	else goto Cleanup;

	if (lpWSAData == NULL)	lpWSAData = new WSADATA[sizeof(WSADATA)];
	if (lpWSAData)	ZeroMemory(lpWSAData, sizeof(WSADATA));
	else goto Cleanup;

	if (WSAStartup(MAKEWORD(2, 2), lpWSAData) != 0)	goto Cleanup;

	if ((pSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
		goto Cleanup;
	}

	iPortNo = Port;
	SockAddr_IN.sin_port = htons((USHORT)iPortNo);
	SockAddr_IN.sin_family = AF_INET;
	iRet = InetPton(SockAddr_IN.sin_family, lpszIPAddr, &SockAddr_IN.sin_addr.S_un.S_addr);
	if (iRet == 0) {
		ADDRINFOW	pHints{};	
		SecureZeroMemory(&pHints, sizeof(ADDRINFOW));
		pHints.ai_flags  = AI_V4MAPPED;
		pHints.ai_family = PF_INET;
		pHints.ai_socktype = SOCK_STREAM;
		pHints.ai_protocol = IPPROTO_UDP;
		iRet = GetAddrInfo(lpszIPAddr, NULL, &pHints, &ppResult);
		if (iRet != 0) {
			WSACleanup();
			goto Cleanup;
		}
		if (ppResult && ppResult->ai_addr && (ppResult->ai_addr->sa_family == AF_INET) && (ppResult->ai_addrlen >= 10)) {
			SockAddr_IN.sin_addr.S_un = ((LPSOCKADDR_IN)(ppResult->ai_addr))->sin_addr.S_un;
			bRet = bIsPrivateIPv4Addr((DWORD)MAKEIPADDRESS(ppResult->ai_addr->sa_data[2], ppResult->ai_addr->sa_data[3], ppResult->ai_addr->sa_data[4], ppResult->ai_addr->sa_data[5]));
			if (!bRet)	WSACleanup();
			goto Cleanup;
		}
	}
	else if (iRet == 1) {
		bRet = TRUE;
	}
	else if (iRet == (-1)) {
		WSACleanup();
	}

Cleanup:
	if (ppResult)	FreeAddrInfo(ppResult);
	return bRet;
}

//
// bSendPacket()
//
BOOL		CTCPIP::bSendPacket(LPCSTR lpszSendData, int cbSize) const
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