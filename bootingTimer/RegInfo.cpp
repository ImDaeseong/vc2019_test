#include "pch.h"
#include "RegInfo.h"

CString RegInfo::getBootingCount()
{
	CString strValue;

	REGSAM sam = KEY_READ;
	if (IsWindows64Bit())
		sam |= KEY_WOW64_64KEY;
	else
		sam |= KEY_WOW64_32KEY;

	CRegKey regKey;
	ULONG bootId = 0;

	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management\\PrefetchParameters"), sam) == ERROR_SUCCESS)
	{
		ULONG dataSize = sizeof(bootId);
		if (regKey.QueryDWORDValue(_T("BootId"), bootId) == ERROR_SUCCESS)
		{
			strValue.Format(_T("부팅 회수: %lu"), bootId);
		}		
	}
	regKey.Close();

	return strValue;
}

BOOL RegInfo::IsWindows64Bit()
{
	if (sizeof(int*) == 8) return TRUE;
	BOOL b = FALSE;
	IsWow64Process(GetCurrentProcess(), &b);
	return b;
}