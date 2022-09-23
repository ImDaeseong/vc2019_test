#include "pch.h"
#include "RegInfo.h"

CString RegInfo::IsExistGoogleplaygameInfo(LPCTSTR szKey)
{
	CString strValue;

	REGSAM sam = KEY_READ | KEY_WRITE;
	if (Util::IsWindows64Bit())
		sam |= KEY_WOW64_64KEY;
	else
		sam |= KEY_WOW64_32KEY;

	CRegKey regKey;

	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Google\\Play Games"), sam) == ERROR_SUCCESS)
	{
		ULONG uLen = 0;
		if (ERROR_SUCCESS == regKey.QueryStringValue(szKey, NULL, &uLen))
		{
			regKey.QueryStringValue(szKey, strValue.GetBuffer(uLen), &uLen);
			strValue.ReleaseBuffer();
		}
	}
	regKey.Close();

	return strValue;
}