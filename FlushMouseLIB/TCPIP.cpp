﻿//
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
// bCheckExistHostnameIPv4() 
//
BOOL		bCheckExistHostnameIPv4(LPCTSTR lpszHostname)
{
	BOOL		bRet = FALSE;
	LPWSADATA	_lpWSAData = new WSADATA[sizeof(WSADATA)];
	PSTR		_lpszIPAddress = new CHAR[MAX_FQDN];

	if (_lpWSAData) {
		ZeroMemory(_lpWSAData, sizeof(WSADATA));
		if (WSAStartup(MAKEWORD(2, 2), _lpWSAData) != 0)	goto Cleanup;
		if (_lpszIPAddress) {
			ZeroMemory(_lpszIPAddress, MAX_FQDN);
			if (WideCharToMultiByte(CP_ACP, 0, lpszHostname, -1, _lpszIPAddress, MAX_FQDN, NULL, NULL) == 0) {
				goto Cleanup;
			}
			ADDRINFOA	Hints{};
			Hints.ai_flags  = ((_lpszIPAddress) ? 0 : AI_PASSIVE);
			Hints.ai_family = AF_INET;
			Hints.ai_socktype = 0;
			Hints.ai_protocol = 0;
			PADDRINFOA	pResult = NULL;
			if (getaddrinfo(_lpszIPAddress, "0", &Hints, &pResult) != 0) {
				goto Cleanup;
			}
			else bRet = TRUE;
		}
	}
Cleanup:
	if (_lpszIPAddress)	delete [] _lpszIPAddress;
	if (_lpWSAData)	delete [] _lpWSAData;
	WSACleanup();

	return bRet;
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
	iRet = setsockopt(pSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(TIMEVAL));
	if (iRet != 0) {
		WSACleanup();
		return FALSE;
	}
	SockAddr_IN.sin_addr.S_un.S_addr = INADDR_ANY;
	iRet = bind((SOCKET)pSocket, (const struct sockaddr *)&SockAddr_IN, (int)sizeof(SockAddr_IN));
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
	char		*_lpData = (char *)lpData;
	
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

	if (lpszIPAddr == NULL)	lpszIPAddr = new TCHAR[MAX_FQDN];
	if (lpszIPAddr) {
		ZeroMemory(lpszIPAddr, (MAX_FQDN * sizeof(TCHAR)));
		if (lpszIPAddress)	wcsncpy_s(lpszIPAddr, MAX_FQDN, lpszIPAddress, MAX_FQDN);
	}

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

//
// bCheckHostByICMPv4()
//
BOOL		CTCPIP::bCheckHostByICMPv4(LPCTSTR lpszIPAddress)
{
	if (lpszIPAddress == NULL)	return FALSE;

	BOOL		bRet = FALSE;
	LPWSADATA	_lpWSAData = new WSADATA[sizeof(WSADATA)];
	PADDRINFOA	pDestAddrInfo = NULL;
	PADDRINFOA	pLocalAddrInfo = NULL;
	PSTR		pDest = new CHAR[MAX_FQDN];
	SOCKET		_pSocket = NULL;
	int			iPacketLen = 0;
	char		*bufICMP = NULL;

	if (_lpWSAData) {
		ZeroMemory(_lpWSAData, sizeof(WSADATA));
		if (WSAStartup(MAKEWORD(2, 2), _lpWSAData) != 0)	goto Cleanup;
		if (pDest) {
			ZeroMemory(pDest, MAX_FQDN);
			if (WideCharToMultiByte(CP_ACP, 0, lpszIPAddress, -1, pDest, MAX_FQDN, NULL, NULL) == 0) {
				goto Cleanup;
			}
			if ((pDestAddrInfo = ResolveAddress(pDest, "0", AF_INET, 0, 0)) == NULL)	goto Cleanup;
			if ((pLocalAddrInfo = ResolveAddress(NULL, "0", AF_INET, 0, 0)) == NULL)	goto Cleanup;
			if ((_pSocket = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
				goto Cleanup;
			}
#define DEFAULT_TTL            128
			if (!bSetTTL(_pSocket, AF_INET, 128)) {
				goto Cleanup;
			}
			iPacketLen += sizeof(ICMP_HDR);
#define DEFAULT_DATA_SIZE      32
			iPacketLen += DEFAULT_DATA_SIZE;
			if ((bufICMP = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, iPacketLen)) == NULL) {
				goto Cleanup;
			}
			InitIcmpHeader(bufICMP, DEFAULT_DATA_SIZE);
			if (bind(_pSocket, pLocalAddrInfo->ai_addr, (int)pLocalAddrInfo->ai_addrlen) != 0) {
				goto Cleanup;
			}
		}
	}

Cleanup:
	if (pDest)	delete [] pDest;
	if (_lpWSAData)	delete [] _lpWSAData;
	if (_pSocket)	closesocket(_pSocket);
	if (bufICMP)	HeapFree(GetProcessHeap(), 0, bufICMP);
	WSACleanup();

	return bRet;
}

//
// usCalcChecksum()
//
USHORT		CTCPIP::CalcChecksum(PUSHORT message, size_t size)
{
	ULONG	sum = 0;

	while (size > 1) {
		sum += *message++;
		size -= sizeof(USHORT);
	}
	if (size) {
		sum += *(UCHAR*)message;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >>16);
	return (USHORT)(~sum);
}

//
// ResolveAddress()
//
PADDRINFOA	CTCPIP::ResolveAddress(PCSTR addr, PCSTR port, int af, int type, int proto)
{
	ADDRINFOA	Hints{};
	Hints.ai_flags  = ((addr) ? 0 : AI_PASSIVE);
	Hints.ai_family = af;
	Hints.ai_socktype = type;
	Hints.ai_protocol = proto;
	PADDRINFOA	pResult = NULL;

	int iRet = 0;
	iRet = getaddrinfo(addr, port, &Hints, &pResult);
	if (iRet != 0) {
		return NULL;
	}
	return pResult;
}

//
// SetTtl()
//
BOOL	CTCPIP::bSetTTL(SOCKET socket, int iAddressFamily, int ttl)
{
	int	optlevel = 0,	option = 0;

	if (iAddressFamily == AF_INET) {
		optlevel = IPPROTO_IP;
		option   = IP_TTL;
	}
	else if (iAddressFamily == AF_INET6) {
		optlevel = IPPROTO_IPV6;
		option   = IPV6_UNICAST_HOPS;
	}
	else {
		return FALSE;
	}
	if (setsockopt(socket, optlevel, option, (char*)&ttl, sizeof(ttl)) != 0) {
		return FALSE;
	}
	return TRUE;
}

//
// InitIcmpHeader()
//
void	CTCPIP::InitIcmpHeader(char *buf, int datasize)
{
	ICMP_HDR   *icmp_hdr=NULL;
	char       *datapart=NULL;

	icmp_hdr = (ICMP_HDR *)buf;
	icmp_hdr->type     = ICMPV4_ECHO_REQUEST_TYPE;
	icmp_hdr->code     = ICMPV4_ECHO_REQUEST_CODE;
	icmp_hdr->id       = (USHORT)GetCurrentProcessId();
	icmp_hdr->checksum = 0;
	icmp_hdr->sequence = 0;

	datapart = buf + sizeof(ICMP_HDR);
	memset(datapart, 'E', datasize);
}


/* = EOF = */