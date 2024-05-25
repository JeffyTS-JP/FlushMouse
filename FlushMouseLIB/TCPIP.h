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
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

//
// Define
// 
#define MAX_FQDN					253
#define MAX_IPV4_ADDRESS			16

// ICMP types and codes
#define ICMPV4_ECHO_REQUEST_TYPE	8
#define ICMPV4_ECHO_REQUEST_CODE	0
#define ICMPV4_ECHO_REPLY_TYPE		0
#define ICMPV4_ECHO_REPLY_CODE		0
#define ICMPV4_MINIMUM_HEADER		8

//
// Struct Define
//
#pragma pack(push,1)
typedef struct icmp_hdr
{
	unsigned char	type;
	unsigned char	code;
	unsigned short	checksum;
	unsigned short	id;
	unsigned short	sequence;
} ICMP_HDR, *PICMP_HDR, FAR *LPICMP_HDR;
#pragma pack(pop)

//
// Global Data
//

//
// Global Prototype Define
//
extern DWORD		dwGetString2IPv4Addr(LPCTSTR lpszIPAddress);
extern BOOL			bIsPrivateAddress(LPCTSTR lpszIPAddress);
extern BOOL			bCheckExistHostnameIPv4(LPCTSTR lpszHostname);

//
// class CTCPIP
//
class CTCPIP
{
	public:
		CTCPIP();
		~CTCPIP();

		BOOL		bOpenPortForReceiveUDPv4(int Port);
		BOOL		bReceivePackaet(LPVOID lpData, int cbSize) const;
		BOOL		bOpenPortForSendUDPv4(LPCTSTR lpszIPAddress, int Port);
		BOOL		bSendPacket(LPCSTR lpszSendData, int cbSize) const;

		BOOL		bCheckHostByICMPv4(LPCTSTR lpszIPAddress);

	private:
		PADDRINFOA	ResolveAddress(PCSTR addr, PCSTR port, int af, int type, int proto);
		USHORT		CalcChecksum(PUSHORT message, size_t size);
		BOOL		bSetTTL(SOCKET socket, int iAddressFamily, int ttl);
		void		InitIcmpHeader(char *buf, int datasize);

	public:

	private:
		LPWSADATA		lpWSAData;
		LPTSTR			lpszIPAddr;
		SOCKET			pSocket;
		int				iPortNo;
		SOCKADDR_IN		SockAddr_IN;
};


/* = EOF = */