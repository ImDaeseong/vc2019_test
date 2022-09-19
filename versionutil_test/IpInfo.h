#pragma once

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")


class IpInfo
{
public:
	static CString GetIPAddress();
};

