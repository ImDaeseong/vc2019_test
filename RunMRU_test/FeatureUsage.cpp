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

void FeatureUsage::getRecentDocs()
{
	//전체 서브 항목별 전체 데이터 조회 - 서브 항목 제목별
	LPCTSTR lpkey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs");

	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, lpkey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwSubCount = 0;
		if (RegQueryInfoKey(hKey, NULL, 0, 0, &dwSubCount, NULL, NULL, NULL, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{			
			for (int i = 0; i < dwSubCount; i++)
			{
				TCHAR szSubKey[MAX_PATH];
				DWORD dwSize;
				dwSize = MAX_PATH;

				if (RegEnumKeyEx(hKey, i, szSubKey, &dwSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
				{
					HKEY hSubKey;
					CString strSubKey;
					strSubKey.Format(_T("%s\\%s"), lpkey, szSubKey);

					if (RegOpenKeyEx(HKEY_CURRENT_USER, strSubKey, 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS)
					{
						DWORD dwCount = 0;
						if (RegQueryInfoKey(hSubKey, NULL, 0, 0, NULL, NULL, NULL, &dwCount, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
						{
							for (DWORD i = 0; i < dwCount; i++)
							{
								TCHAR szKey[MAX_PATH], szValue[MAX_PATH];
								DWORD dwSize, dwSizeOfByte, dwType;
								dwSize = MAX_PATH;
								dwSizeOfByte = MAX_PATH;

								if (::RegEnumValue(hSubKey, i, szKey, &dwSize, NULL, &dwType, (LPBYTE)szValue, &dwSizeOfByte) == ERROR_SUCCESS)
								{
									CString strPath;
									strPath.Format(_T("[%s] %s \n"), szKey, szValue);
									OutputDebugString(strPath);
								}
							}
						}
					}
					RegCloseKey(hSubKey);
				}
			}
		}		
	}
	RegCloseKey(hKey);


	//전체 서브까지 전체 데이터 조회 - 데이터만 조회
	/*
	LPCTSTR lpkey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs");

	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, lpkey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwCount = 0;
		if (RegQueryInfoKey(hKey, NULL, 0, 0, NULL, NULL, NULL, &dwCount, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			for (DWORD i = 0; i < dwCount; i++)
			{
				TCHAR szKey[MAX_PATH], szValue[MAX_PATH];
				DWORD dwSize, dwSizeOfByte, dwType;
				dwSize = MAX_PATH;
				dwSizeOfByte = MAX_PATH;

				if (::RegEnumValue(hKey, i, szKey, &dwSize, NULL, &dwType, (LPBYTE)szValue, &dwSizeOfByte) == ERROR_SUCCESS)
				{
					CString strPath;
					strPath.Format(_T("RecentDocs[%s] %s \n"), szKey, szValue);
					OutputDebugString(strPath);
				}
			}
		}		
	}
	RegCloseKey(hKey);
	*/	
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
				TCHAR szKey[MAX_PATH], szValue[MAX_PATH];
				DWORD dwSize, dwSizeOfByte, dwType;
				dwSize = MAX_PATH;
				dwSizeOfByte = MAX_PATH;

				if (::RegEnumValue(hKey, i, szKey, &dwSize, NULL, &dwType, (LPBYTE)szValue, &dwSizeOfByte) == ERROR_SUCCESS)
				{
					CString strPath;
					strPath.Format(_T("RunMRU[%s] %s \n"), szKey, szValue);
					OutputDebugString(strPath);					
				}
			}
		}
	}
	RegCloseKey(hKey);
}

void FeatureUsage::getAppBadgeUpdated()
{
	LPCTSTR lpkey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FeatureUsage\\AppBadgeUpdated");

	HKEY hKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, lpkey, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwCount = 0;
		if (RegQueryInfoKey(hKey, NULL, 0, 0, NULL, NULL, NULL, &dwCount, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			for (DWORD i = 0; i < dwCount; i++)
			{
				TCHAR szKey[MAX_PATH], szValue[MAX_PATH];
				DWORD dwSize, dwSizeOfByte, dwType;
				dwSize = MAX_PATH;
				dwSizeOfByte = MAX_PATH;

				unsigned long lValue, lValueSize;
				lValueSize = sizeof(lValue);

				if (RegEnumValue(hKey, i, szKey, &dwSize, NULL, &dwType, NULL, NULL) == ERROR_SUCCESS)
				{
					if (dwType == REG_DWORD)
					{
						if (RegQueryValueEx(hKey, szKey, NULL, NULL, (LPBYTE)&lValue, &lValueSize) == ERROR_SUCCESS)
						{
							int nExist = 0;
							CString strPath;
							strPath.Format(_T("%s"), szKey);
														
							CString strChange = FindPathString(strPath);
							if (strChange != "")
							{
								CString strTemp1, strTemp2;
								strTemp1.Format(_T("%s\\%s"), GetProgramFilesx86Path(), strChange);
								strTemp2.Format(_T("%s\\%s"), GetProgramFilesPath(), strChange);

								nExist = _waccess(strTemp1, 0);
								if (nExist != -1)
								{
									aryList.Add(strTemp1);
								}

								nExist = _waccess(strTemp2, 0);
								if (nExist != -1)
								{
									aryList.Add(strTemp2);
								}
							}
							else
							{
								nExist = _waccess(strPath, 0);
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
				TCHAR szKey[MAX_PATH], szValue[MAX_PATH];
				DWORD dwSize, dwSizeOfByte, dwType;
				dwSize = MAX_PATH;
				dwSizeOfByte = MAX_PATH;

				unsigned long lValue, lValueSize;
				lValueSize = sizeof(lValue);

				if (RegEnumValue(hKey, i, szKey, &dwSize, NULL, &dwType, NULL, NULL) == ERROR_SUCCESS)
				{
					if (dwType == REG_DWORD)
					{
						if (RegQueryValueEx(hKey, szKey, NULL, NULL, (LPBYTE)&lValue, &lValueSize) == ERROR_SUCCESS)
						{
							int nExist = 0;
							CString strPath;
							strPath.Format(_T("%s"), szKey);

							CString strChange = FindPathString(strPath);
							if (strChange != "")
							{
								CString strTemp1, strTemp2;
								strTemp1.Format(_T("%s\\%s"), GetProgramFilesx86Path(), strChange);
								strTemp2.Format(_T("%s\\%s"), GetProgramFilesPath(), strChange);

								nExist = _waccess(strTemp1, 0);
								if (nExist != -1)
								{
									aryList.Add(strTemp1);
								}

								nExist = _waccess(strTemp2, 0);
								if (nExist != -1)
								{
									aryList.Add(strTemp2);
								}
							}
							else
							{
								nExist = _waccess(strPath, 0);
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
				TCHAR szKey[MAX_PATH], szValue[MAX_PATH];
				DWORD dwSize, dwSizeOfByte, dwType;
				dwSize = MAX_PATH;
				dwSizeOfByte = MAX_PATH;

				unsigned long lValue, lValueSize;
				lValueSize = sizeof(lValue);

				if (RegEnumValue(hKey, i, szKey, &dwSize, NULL, &dwType, NULL, NULL) == ERROR_SUCCESS)
				{
					if (dwType == REG_DWORD)
					{
						if (RegQueryValueEx(hKey, szKey, NULL, NULL, (LPBYTE)&lValue, &lValueSize) == ERROR_SUCCESS)
						{
							int nExist = 0;
							CString strPath;
							strPath.Format(_T("%s"), szKey);

							CString strChange = FindPathString(strPath);
							if (strChange != "")
							{
								CString strTemp1, strTemp2;
								strTemp1.Format(_T("%s\\%s"), GetProgramFilesx86Path(), strChange);
								strTemp2.Format(_T("%s\\%s"), GetProgramFilesPath(), strChange);

								nExist = _waccess(strTemp1, 0);
								if (nExist != -1)
								{
									aryList.Add(strTemp1);
								}

								nExist = _waccess(strTemp2, 0);
								if (nExist != -1)
								{
									aryList.Add(strTemp2);
								}
							}
							else
							{
								nExist = _waccess(strPath, 0);
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
				TCHAR szKey[MAX_PATH], szValue[MAX_PATH];
				DWORD dwSize, dwSizeOfByte, dwType;
				dwSize = MAX_PATH;
				dwSizeOfByte = MAX_PATH;

				unsigned long lValue, lValueSize;
				lValueSize = sizeof(lValue);

				if (RegEnumValue(hKey, i, szKey, &dwSize, NULL, &dwType, NULL, NULL) == ERROR_SUCCESS)
				{
					if (dwType == REG_DWORD)
					{
						if (RegQueryValueEx(hKey, szKey, NULL, NULL, (LPBYTE)&lValue, &lValueSize) == ERROR_SUCCESS)
						{
							int nExist = 0;
							CString strPath;
							strPath.Format(_T("%s"), szKey);

							CString strChange = FindPathString(strPath);
							if (strChange != "")
							{
								CString strTemp1, strTemp2;
								strTemp1.Format(_T("%s\\%s"), GetProgramFilesx86Path(), strChange);
								strTemp2.Format(_T("%s\\%s"), GetProgramFilesPath(), strChange);

								nExist = _waccess(strTemp1, 0);
								if (nExist != -1)
								{
									aryList.Add(strTemp1);
								}

								nExist = _waccess(strTemp2, 0);
								if (nExist != -1)
								{
									aryList.Add(strTemp2);
								}
							}
							else
							{
								nExist = _waccess(strPath, 0);
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
	}

	RegCloseKey(hKey);
}

CString FeatureUsage::GetSystem32Path()
{
	TCHAR szPath[MAX_PATH];
	GetSystemDirectory(szPath, MAX_PATH);	
	CString strFilesPath = szPath;
	return strFilesPath;
}

CString FeatureUsage::GetProgramFilesPath()
{
	CString strFilesPath;
	strFilesPath.Format(_T("%s:\\Program Files"), GetSystem32Path().Left(1));
	return strFilesPath;
}

CString FeatureUsage::GetProgramFilesx86Path()
{	
	CString strFilesPath;
	strFilesPath.Format(_T("%s:\\Program Files (x86)"), GetSystem32Path().Left(1));
	return strFilesPath;
}

CString FeatureUsage::FindPathString(CString strPath)
{
	CString lpProgramFilesX86 = _T("{6D809377-6AF0-444B-8957-A3773F02200E}");
	CString lpProgramFiles = _T("{7C5A40EF-A0FB-4BFC-874A-C0F2E0B9FA8E}");

	CString strReturn = _T("");

	int nIndex = strPath.Find(lpProgramFilesX86, 0);
	if (nIndex != -1)
	{
		strReturn = strPath.Mid(nIndex + lpProgramFilesX86.GetLength());
	}
	else
	{
		nIndex = strPath.Find(lpProgramFiles, 0);
		if (nIndex != -1)
		{
			strReturn = strPath.Mid(nIndex + lpProgramFiles.GetLength());
		}
	}
	return strReturn;
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
	getRecentDocs();
	getRunMRU();
	getAppBadgeUpdated();
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

void FeatureUsage::FindExe(CStringArray& list)
{
	int nSize = aryList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		//CString strValue = GetFileName(aryList.GetAt(i));
		list.Add(aryList.GetAt(i));
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

void FeatureUsage::FindExe(CStringArray& strFileNamelist, CStringArray& list)
{
	int kSize = strFileNamelist.GetSize();
	int nSize = aryList.GetSize();

	for (int i = 0; i < nSize; i++)
	{
		//파일이름만 가져온다
		CString strValue = GetFileName(aryList.GetAt(i));

		//찾아야할 항목 리스트
		for (int k = 0; k < kSize; k++)
		{
			CString strItem = GetFileName(strFileNamelist.GetAt(k));

			if (_stricmp((CStringA)strValue, (CStringA)strItem) == 0)
			{
				list.Add(aryList.GetAt(i));
			}
		}		
	}
}