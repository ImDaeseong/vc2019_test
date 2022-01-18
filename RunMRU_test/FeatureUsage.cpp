#include "pch.h"
#include "FeatureUsage.h"

FeatureUsage::FeatureUsage()
{
	aryList.RemoveAll();
}

FeatureUsage::~FeatureUsage()
{
	aryList.RemoveAll();
}

void FeatureUsage::getRunMRU()
{
	LPCTSTR lpkey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU");

	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, lpkey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwCount = 0;
		if (RegQueryInfoKey(hKey, NULL, 0, 0, NULL, NULL, NULL, &dwCount, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			for (DWORD i = 0; i < dwCount; i++)
			{
				TCHAR szBuffer[MAX_PATH], szValue[MAX_PATH];
				DWORD dwSize, dwSizeOfByte, dwType;
				dwSize = MAX_PATH;
				dwSizeOfByte = MAX_PATH;

				if (::RegEnumValue(hKey, i, szBuffer, &dwSize, NULL, &dwType, (LPBYTE)szValue, &dwSizeOfByte) == ERROR_SUCCESS)
				{
					CString strPath;
					strPath.Format(_T("%s"), szValue);
					OutputDebugString(strPath);					
				}
			}
		}
	}
	RegCloseKey(hKey);
}

void FeatureUsage::getAppLaunch()
{
	LPCTSTR lpkey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FeatureUsage\\AppLaunch");

	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, lpkey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwCount = 0;
		if (RegQueryInfoKey(hKey, NULL, 0, 0, NULL, NULL, NULL, &dwCount, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			for (DWORD i = 0; i < dwCount; i++)
			{
				TCHAR szBuffer[MAX_PATH], szValue[MAX_PATH];
				DWORD dwSize, dwSizeOfByte, dwType;
				dwSize = MAX_PATH;
				dwSizeOfByte = MAX_PATH;

				unsigned long lValue, lValueSize;
				lValueSize = sizeof(lValue);

				if (RegEnumValue(hKey, i, szBuffer, &dwSize, NULL, &dwType, NULL, NULL) == ERROR_SUCCESS)
				{
					if (dwType == REG_DWORD)
					{
						if (RegQueryValueEx(hKey, szBuffer, NULL, NULL, (LPBYTE)&lValue, &lValueSize) == ERROR_SUCCESS)
						{
							CString strPath;
							strPath.Format(_T("%s"), szBuffer);

							int nExist = _waccess(strPath, 0);
							if (nExist != -1)
							{
								aryList.Add(strPath);
							}
						}
					}
				}
			}

		}
	}

	RegCloseKey(hKey);
}

void FeatureUsage::getAppSwitched()
{
	LPCTSTR lpkey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FeatureUsage\\AppSwitched");

	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, lpkey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwCount = 0;
		if (RegQueryInfoKey(hKey, NULL, 0, 0, NULL, NULL, NULL, &dwCount, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			for (DWORD i = 0; i < dwCount; i++)
			{
				TCHAR szBuffer[MAX_PATH], szValue[MAX_PATH];
				DWORD dwSize, dwSizeOfByte, dwType;
				dwSize = MAX_PATH;
				dwSizeOfByte = MAX_PATH;

				unsigned long lValue, lValueSize;
				lValueSize = sizeof(lValue);

				if (RegEnumValue(hKey, i, szBuffer, &dwSize, NULL, &dwType, NULL, NULL) == ERROR_SUCCESS)
				{
					if (dwType == REG_DWORD)
					{
						if (RegQueryValueEx(hKey, szBuffer, NULL, NULL, (LPBYTE)&lValue, &lValueSize) == ERROR_SUCCESS)
						{
							CString strPath;
							strPath.Format(_T("%s"), szBuffer);

							int nExist = _waccess(strPath, 0);
							if (nExist != -1)
							{
								aryList.Add(strPath);
							}
						}
					}

				}
			}

		}
	}

	RegCloseKey(hKey);
}

void FeatureUsage::getShowJumpView()
{
	LPCTSTR lpkey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FeatureUsage\\ShowJumpView");

	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, lpkey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwCount = 0;
		if (RegQueryInfoKey(hKey, NULL, 0, 0, NULL, NULL, NULL, &dwCount, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			for (DWORD i = 0; i < dwCount; i++)
			{
				TCHAR szBuffer[MAX_PATH], szValue[MAX_PATH];
				DWORD dwSize, dwSizeOfByte, dwType;
				dwSize = MAX_PATH;
				dwSizeOfByte = MAX_PATH;

				unsigned long lValue, lValueSize;
				lValueSize = sizeof(lValue);

				if (RegEnumValue(hKey, i, szBuffer, &dwSize, NULL, &dwType, NULL, NULL) == ERROR_SUCCESS)
				{
					if (dwType == REG_DWORD)
					{
						if (RegQueryValueEx(hKey, szBuffer, NULL, NULL, (LPBYTE)&lValue, &lValueSize) == ERROR_SUCCESS)
						{
							CString strPath;
							strPath.Format(_T("%s"), szBuffer);

							int nExist = _waccess(strPath, 0);
							if (nExist != -1)
							{
								aryList.Add(strPath);
							}
						}
					}

				}
			}

		}
	}

	RegCloseKey(hKey);
}

CString FeatureUsage::GetFileName(CString strFilename)
{
	int nPos = strFilename.ReverseFind('\\');
	if (nPos > 0)
		return strFilename.Mid(nPos + 1);

	return strFilename;
}

void FeatureUsage::InitLoad()
{
	//getRunMRU();
	getAppLaunch();
	getAppSwitched();
	getShowJumpView();
}

void FeatureUsage::clear()
{
	aryList.RemoveAll();
}

void FeatureUsage::FindExe(CString strFileName)
{
	int nSize = aryList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		//파일이름만 가져온다
		CString strValue = GetFileName(aryList.GetAt(i));

		if (_stricmp((CStringA)strValue, (CStringA)strFileName) == 0)
		{
			//파일을 실행한다
			HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), aryList.GetAt(i), NULL, NULL, SW_SHOW);
		}		
	}	
}

void FeatureUsage::FindExe(CString strFileName, CStringArray& list)
{
	int nSize = aryList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		//파일이름만 가져온다
		CString strValue = GetFileName(aryList.GetAt(i));

		if (_stricmp((CStringA)strValue, (CStringA)strFileName) == 0)
		{
			list.Add(aryList.GetAt(i));
		}
	}
}