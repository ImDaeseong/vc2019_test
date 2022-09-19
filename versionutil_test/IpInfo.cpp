#include "pch.h"
#include "IpInfo.h"

CString IpInfo::GetIPAddress()
{
	WORD wVersion;
	WSADATA wsaData;

	char chTemp[256];

	PHOSTENT phostinfo = NULL;

	CString strLocalIP = _T("");

	wVersion = MAKEWORD(2, 0);

	if (WSAStartup(wVersion, &wsaData) == 0)
	{
		if (gethostname(chTemp, sizeof(chTemp)) == 0)
		{
			if ((phostinfo = gethostbyname(chTemp)) != NULL)
			{
				strLocalIP = inet_ntoa(*(struct in_addr*)*phostinfo->h_addr_list);
				if (strLocalIP == "")
				{
					strLocalIP = "127.0.0.1";
				}
			}
		}
		WSACleanup();
	}

	return strLocalIP;
}
