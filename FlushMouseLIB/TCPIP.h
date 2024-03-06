//
// TCPIP.h
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
#include <WinSock2.h>				// for Winsoc2.h
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

//
// Define
// 
#define MAX_LOADSTRING 100

// ICMP types and codes
#define ICMPV4_ECHO_REQUEST_TYPE	8
#define ICMPV4_ECHO_REQUEST_CODE	0
#define ICMPV4_ECHO_REPLY_TYPE		0
#define ICMPV4_ECHO_REPLY_CODE		0
#define ICMPV4_MINIMUM_HEADER		8

//
// Struct Define
//
typedef struct icmp_hdr
{
	unsigned char	icmp_type;
	unsigned char	icmp_code;
	unsigned short	icmp_checksum;
	unsigned short	icmp_id;
	unsigned short	icmp_sequence;
} ICMP_HDR, *PICMP_HDR, FAR *LPICMP_HDR;

//
// Global Data
//

//
// Global Prototype Define
//
extern	DWORD		dwGetString2IPv4Addr(LPCTSTR szIPAddr);

//
// class CTCPIP
//
class CTCPIP
{
	public:
		CTCPIP();
		~CTCPIP();

		BOOL		bOpenPortForReceiveUDPv4(LPCTSTR lpszIPAddress, int Port);
		BOOL		bReceivePackaet(LPVOID lpData, int cbSize);
		BOOL		bOpenPortForSendUDPv4(LPCTSTR lpszIPAddress, int Port);
		BOOL		bSendPacket(LPCSTR lpszSendData, int cbSize);

		//BOOL		bCheckHostByICMPv4(LPCTSTR lpszIPAddress);

	private:

	public:

	private:
		LPWSADATA		lpWSAData;
		LPTSTR			lpszIPAddr;
		SOCKET			pSocket;
		int				iPortNo;
		SOCKADDR_IN		SockAddr_IN;
};


/* = EOF = */