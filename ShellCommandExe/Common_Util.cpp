#include "pch.h"
#include "Common_Util.h"

CString Common_Util::GetCommand()
{
	CString strValue = _T("");
	HKEY hKey = NULL;
	if (RegOpenKey(HKEY_CLASSES_ROOT, _T("daeseong\\Shell\\Open"), &hKey) == ERROR_SUCCESS)
	{
		TCHAR tValue[MAX_PATH];
		LONG lSize = sizeof(tValue);

		if (RegQueryValue(hKey, _T("Command"), tValue, &lSize) == ERROR_SUCCESS)
		{
			strValue.Format(_T("%s\r\n"), tValue);
		}
		RegCloseKey(hKey);		
	}
	return strValue;
}

BOOL Common_Util::SetCommand(CString strExe)
{		
	HKEY hRootKey;
	HKEY hIconKey;
	HKEY hShellKey;
	HKEY hOpenKey;
	HKEY hCommandKey;
		

	//daeseong
	if (ERROR_SUCCESS != RegCreateKey(HKEY_CLASSES_ROOT, _T("daeseong"), &hRootKey))
	{
		return FALSE;
	}

	if (ERROR_SUCCESS != RegSetValueEx(hRootKey, _T("URL Protocol"), 0, REG_SZ, 0, 0))
	{
		return FALSE;
	}


	//DefaultIcon
	if (ERROR_SUCCESS != RegCreateKey(hRootKey, _T("DefaultIcon"), &hIconKey))
	{
		return FALSE;
	}

	if (ERROR_SUCCESS != RegSetValue(hIconKey, _T(""), REG_SZ, strExe, strExe.GetLength() + 1))
	{
		return FALSE;
	}


	//Shell
	if (ERROR_SUCCESS != RegCreateKey(hRootKey, _T("Shell"), &hShellKey))
	{
		return FALSE;
	}


	//Open
	if (ERROR_SUCCESS != RegCreateKey(hShellKey, _T("Open"), &hOpenKey))
	{
		return FALSE;
	}


	//Command
	if (ERROR_SUCCESS != RegCreateKey(hOpenKey, _T("Command"), &hCommandKey))
	{
		return FALSE;
	}


	CString strCommandExe;
	strCommandExe.Format(_T("%s \"%%1\""), strExe);

	if (ERROR_SUCCESS != RegSetValue(hCommandKey, _T(""), REG_SZ, strCommandExe, strCommandExe.GetLength() + 1))
	{
		return FALSE;
	}

	RegCloseKey(hCommandKey);
	RegCloseKey(hOpenKey);
	RegCloseKey(hShellKey);
	RegCloseKey(hIconKey);
	RegCloseKey(hRootKey);

	return TRUE;
}

BOOL RegDeleteMainKey(HKEY hKey, CString szSubKey)
{
	LONG lRet = 0;
	TCHAR szBuffer[256];
	TCHAR szPath[256];
	DWORD dwIndex = 0;
	HKEY hOpenKey;

	int len = 0;
	lRet = RegOpenKey(hKey, szSubKey, &hOpenKey);
	if (lRet == ERROR_SUCCESS)
	{
		lRet = RegEnumKey(hOpenKey, dwIndex, szBuffer, sizeof(szBuffer));

		lstrcpy(szPath, szSubKey);

		if (RegDeleteKey(hKey, szPath) == ERROR_SUCCESS)
		{

		}
		else
		{
		}
	}
	RegCloseKey(hOpenKey);
	return lRet;
}

int RegDeleteSubKey(HKEY hKey, CString szSubKey)
{
	LONG lRet = 0;
	TCHAR szBuffer[256];
	TCHAR szPath[256];
	DWORD dwIndex = 0;
	HKEY hOpenKey;

	int len = 0;
	lRet = RegOpenKey(hKey, szSubKey, &hOpenKey);
	while (lRet == ERROR_SUCCESS)
	{
		lRet = RegEnumKey(hOpenKey, dwIndex, szBuffer, sizeof(szBuffer));

		if (lRet == ERROR_NO_MORE_ITEMS) return ERROR_SUCCESS;

		if (lRet != ERROR_SUCCESS) return lRet;

		lstrcpy(szPath, szSubKey);

		lstrcat(szPath, _T("\\"));

		lstrcat(szPath, szBuffer);

		if (RegDeleteSubKey(hKey, szPath) == ERROR_SUCCESS)
		{
			RegDeleteKey(hKey, szPath);
		}
		else
		{
			dwIndex++;
		}
	}
	RegCloseKey(hOpenKey);

	return lRet;
}

void Common_Util::DeleteCommand()
{
	RegDeleteSubKey(HKEY_CLASSES_ROOT, _T("daeseong"));
	RegDeleteMainKey(HKEY_CLASSES_ROOT, _T("daeseong"));
}

CString Common_Util::GetModulePath()
{
	TCHAR tPath[MAX_PATH];
	::GetModuleFileName(NULL, tPath, MAX_PATH);

	CString strPath = tPath;
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
	return strPath;
}