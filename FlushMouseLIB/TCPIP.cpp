//
// TCPIP.cpp
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
	if (iTimeOut <= 0) iTimeOut = 1000;

	auto task = std::async(std::launch::async, [lpszHostname]() -> BOOL {
		WSADATA wsaData{};
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			return FALSE;
		}

		ADDRINFOW hints{};
		hints.ai_flags  = AI_V4MAPPED;
		hints.ai_family = PF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;

		PADDRINFOW pResult = NULL;
		int rc = GetAddrInfo(lpszHostname, NULL, &hints, &pResult);
		if (rc != 0) {
			if (pResult) FreeAddrInfo(pResult);
			WSACleanup();
			return FALSE;
		}

		BOOL bFoundPrivate = FALSE;
		for (PADDRINFOW p = pResult; p != NULL; p = p->ai_next) {
			if (p->ai_addr && p->ai_addr->sa_family == AF_INET) {
				const BYTE *sa = reinterpret_cast<const BYTE*>(p->ai_addr->sa_data);
				DWORD addr = MAKEIPADDRESS(sa[2], sa[3], sa[4], sa[5]);
				if (bIsPrivateIPv4Addr(addr)) {
					bFoundPrivate = TRUE;
					break;
				}
			}
		}

		if (pResult) FreeAddrInfo(pResult);
		WSACleanup();
		return bFoundPrivate;
	});

	if (task.valid()) {
		std::future_status st = task.wait_for(std::chrono::milliseconds(iTimeOut));
		if (st == std::future_status::ready) {
			try {
				return task.get() ? TRUE : FALSE;
			}
			catch (...) {
				return FALSE;
			}
		}
	}
	return FALSE;
}

//
//class CTCPIP
//
CTCPIP::CTCPIP()
	: lpWSAData(NULL), lpszIPAddr(NULL),  pSocket(INVALID_SOCKET), iPortNo(0), SockAddr_IN{}
{

}

CTCPIP::~CTCPIP()
{
	if (pSocket != INVALID_SOCKET) {
		closesocket(pSocket);
		pSocket = INVALID_SOCKET;
	}
	if (lpWSAData) {
		WSACleanup();
		delete lpWSAData;
		lpWSAData = NULL;
	}
	if (lpszIPAddr)	delete [] lpszIPAddr;
	lpszIPAddr = NULL;
}

//
// bOpenPortForReceiveUDPv4()
//
BOOL		CTCPIP::bOpenPortForReceiveUDPv4(int Port)
{
	if ((Port == 0))	return FALSE;

	if (lpWSAData == NULL) lpWSAData = new WSADATA;
	if (lpWSAData)	ZeroMemory(lpWSAData, sizeof(WSADATA));
	else return FALSE;

	if (WSAStartup(MAKEWORD(2, 2), lpWSAData) != 0) {
		delete lpWSAData;
		lpWSAData = NULL;
		return FALSE;
	}

	pSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (pSocket == INVALID_SOCKET) {
		WSACleanup();
		delete lpWSAData;
		lpWSAData = NULL;
		return FALSE;
	}

	iPortNo = Port;
	SockAddr_IN.sin_family = AF_INET;
	SockAddr_IN.sin_port = htons((USHORT)iPortNo);
	SockAddr_IN.sin_addr.S_un.S_addr = INADDR_ANY;

	DWORD recvTimeout = 10;
	int optret = setsockopt((SOCKET)pSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&recvTimeout), sizeof(recvTimeout));
	if (optret != 0) {
		closesocket(pSocket);
		pSocket = INVALID_SOCKET;
		WSACleanup();
		delete lpWSAData;
		lpWSAData = NULL;
		return FALSE;
	}

	int iRet = bind((SOCKET)pSocket, reinterpret_cast<const struct sockaddr *>(&SockAddr_IN), (int)sizeof(SockAddr_IN));
	if (iRet != 0) {
		closesocket(pSocket);
		pSocket = INVALID_SOCKET;
		WSACleanup();
		delete lpWSAData;
		lpWSAData = NULL;
		return FALSE;
	}
	return TRUE;
}

//
// bReceivePacket()
//
BOOL		CTCPIP::bReceivePacket(LPVOID lpData, int cbSize) const
{
	if ((pSocket == INVALID_SOCKET) || (lpData == NULL) || (cbSize == 0))	return FALSE;

	char *_lpData = reinterpret_cast<char *>(lpData);
	int iRet = recv((SOCKET)pSocket, _lpData, cbSize, 0);
	if (iRet == SOCKET_ERROR) {
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK || err == WSAETIMEDOUT)	return TRUE; // no data available (non-fatal)
		return FALSE;
	}
	return	TRUE;
}

//
// bOpenPortForSendUDPv4()
//
BOOL		CTCPIP::bOpenPortForSendUDPv4(LPCTSTR lpszIPAddress, int Port)
{
	if ((lpszIPAddress == NULL) || (Port == 0))	return FALSE;

	BOOL	bRet = FALSE;
	PADDRINFOW ppResult = NULL;

	if (lpszIPAddr == NULL)	lpszIPAddr = new TCHAR[MAX_FQDN];
	if (lpszIPAddr) {
		ZeroMemory(lpszIPAddr, (MAX_FQDN * sizeof(TCHAR)));
		_tcsncpy_s(lpszIPAddr, MAX_FQDN, lpszIPAddress, _TRUNCATE);
	}
	else return FALSE;

	if (lpWSAData == NULL) lpWSAData = new WSADATA;
	if (lpWSAData)	ZeroMemory(lpWSAData, sizeof(WSADATA));
	else return FALSE;

	int inetRet = 0;
	if (WSAStartup(MAKEWORD(2, 2), lpWSAData) != 0) goto Cleanup;

	pSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (pSocket == INVALID_SOCKET) goto Cleanup;

	iPortNo = Port;
	SockAddr_IN.sin_port = htons((USHORT)iPortNo);
	SockAddr_IN.sin_family = AF_INET;

	inetRet = InetPton(SockAddr_IN.sin_family, lpszIPAddr, &SockAddr_IN.sin_addr.S_un.S_addr);
	if (inetRet == 0) {
		ADDRINFOW hints{};
		ZeroMemory(&hints, sizeof(ADDRINFOW));
		hints.ai_flags  = AI_V4MAPPED;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
		if (GetAddrInfo(lpszIPAddr, NULL, &hints, &ppResult) != 0)	goto Cleanup;
		if (ppResult && ppResult->ai_addr && (ppResult->ai_addr->sa_family == AF_INET) && (ppResult->ai_addrlen >= 10)) {
			SockAddr_IN.sin_addr.S_un = ((LPSOCKADDR_IN)(ppResult->ai_addr))->sin_addr.S_un;
			BOOL isPrivate = bIsPrivateIPv4Addr((DWORD)MAKEIPADDRESS(ppResult->ai_addr->sa_data[2], ppResult->ai_addr->sa_data[3], ppResult->ai_addr->sa_data[4], ppResult->ai_addr->sa_data[5]));
			if (!isPrivate) {
				WSACleanup();
				goto Cleanup;
			}
			bRet = TRUE;
		}
	}
	else if (inetRet == 1) {
		bRet = TRUE;
	}
	else {
		WSACleanup();
		goto Cleanup;
	}

Cleanup:
	if (ppResult)	FreeAddrInfo(ppResult);
	if (!bRet) {
		if (pSocket != INVALID_SOCKET) {
			closesocket(pSocket);
			pSocket = INVALID_SOCKET;
		}
		if (lpWSAData) {
			WSACleanup();
			delete lpWSAData;
			lpWSAData = NULL;
		}
	}
	return bRet;
}

//
// bSendPacket()
//
BOOL		CTCPIP::bSendPacket(LPCSTR lpszSendData, int cbSize) const
{
	if ((pSocket == INVALID_SOCKET) || (lpszSendData == NULL) || (cbSize == 0))	return FALSE;
	int	iRet = sendto((SOCKET)pSocket, lpszSendData, cbSize, 0, (LPSOCKADDR)&SockAddr_IN, sizeof(SockAddr_IN));
	if (iRet == SOCKET_ERROR) {
		return FALSE;
	}
	return TRUE;
}


/* = EOF = */
