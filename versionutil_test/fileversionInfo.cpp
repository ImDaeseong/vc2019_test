#include "pch.h"
#include "fileversionInfo.h"

CString fileversionInfo::GetFileVersion(CString strFilePath)
{
	DWORD dwSize;
	DWORD dwMajorVer, dwMinorVer;
	CString strVersion = _T("");

	dwSize = GetFileVersionInfoSize(strFilePath, 0);
	if (dwSize)
	{
		TCHAR* buffer = new TCHAR[dwSize];
		memset(buffer, 0, dwSize);

		if (GetFileVersionInfo(strFilePath, 0, dwSize, buffer))
		{
			VS_FIXEDFILEINFO* pInfo;

			UINT uLen;
			BOOL bResult;

			bResult = VerQueryValue(buffer, _T("\\"), (void**)&pInfo, &uLen);
			if (bResult)
			{
				dwMajorVer = pInfo->dwFileVersionMS; // 주버전
				dwMinorVer = pInfo->dwFileVersionLS; // 보조버전

				DWORD dwMinorVer1, dwMinorVer2, dwMajorVer1, dwMajorVer2;

				dwMajorVer1 = (dwMajorVer / 65536);
				dwMajorVer2 = (dwMajorVer % 65536);
				dwMinorVer1 = (dwMinorVer / 65536);
				dwMinorVer2 = (dwMinorVer % 65536);
				strVersion.Format(_T("%d.%d.%d.%d"), dwMajorVer1, dwMajorVer2, dwMinorVer1, dwMinorVer2);
			}
		}
		delete[] buffer;
	}
	return strVersion;
}
