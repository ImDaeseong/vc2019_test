#include "pch.h"
#include "LDPlayer4.h"

LDPlayer4::LDPlayer4()
{
}

LDPlayer4::~LDPlayer4()
{
}

CString LDPlayer4::getLDPlayer4Path()
{
	TCHAR tPath[MAX_PATH];
	ULONG size{ MAX_PATH };

	CString strPath;
	CRegKey regKey;

	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\LDPlayer4"), KEY_READ) != ERROR_SUCCESS)
	{
		return _T("");
	}

	if (regKey.QueryStringValue(_T("UninstallString"), tPath, &size) == ERROR_SUCCESS)
	{
		strPath = tPath;
	}
	regKey.Close();

	return strPath;

	/*
	TCHAR tPath[MAX_PATH];
	ULONG size{ MAX_PATH };

	CString strPath;
	CRegKey regKey;

	if (regKey.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\XuanZhi\\LDPlayer"), KEY_READ) != ERROR_SUCCESS)
	{
		return _T("");
	}

	if (regKey.QueryStringValue(_T("InstallDir"), tPath, &size) == ERROR_SUCCESS)
	{
		strPath = tPath;
	}
	regKey.Close();

	return strPath;
	*/
}

CString LDPlayer4::getLDPlayer4Version()
{
	TCHAR tPath[MAX_PATH];
	ULONG size{ MAX_PATH };

	CString strPath;
	CRegKey regKey;

	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\LDPlayer4"), KEY_READ) != ERROR_SUCCESS)
	{
		return _T("");
	}

	if (regKey.QueryStringValue(_T("DisplayVersion"), tPath, &size) == ERROR_SUCCESS)
	{
		strPath = tPath;
	}
	regKey.Close();

	return strPath;

	/*
	TCHAR tPath[MAX_PATH];
	ULONG size{ MAX_PATH };

	CString strPath;
	CRegKey regKey;

	if (regKey.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\XuanZhi\\LDPlayer"), KEY_READ) != ERROR_SUCCESS)
	{
		return _T("");
	}

	if (regKey.QueryStringValue(_T("Version"), tPath, &size) == ERROR_SUCCESS)
	{
		strPath = tPath;
	}
	regKey.Close();

	return strPath;
	*/
}

BOOL LDPlayer4::DefaultRunLDPlayer4()
{
	try
	{
		CString strLDPlayer4Path = Common::GetFilePath(getLDPlayer4Path());
		if (strLDPlayer4Path.IsEmpty())
			return FALSE;

		CString strAppExe;
		strAppExe.Format(_T("%s\\dnplayer.exe"), strLDPlayer4Path);

		if (!::PathFileExists(strAppExe))
			return FALSE;

		HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), strAppExe, NULL, NULL, SW_HIDE);
		if (hInstance <= (HINSTANCE)32)
			return FALSE;

		return TRUE;
	}
	catch (...)
	{
	}

	return FALSE;
}

BOOL LDPlayer4::RunLDPlayer4(CString strPackage)
{
	try
	{
		CString strLDPlayer4Path = Common::GetFilePath(getLDPlayer4Path());
		if (strLDPlayer4Path.IsEmpty())
			return FALSE;

		CString strAppExe;
		strAppExe.Format(_T("%s\\dnplayer.exe"), strLDPlayer4Path);

		CString strParams;
		strParams.Format(_T("package=%s"), strPackage);

		if (!::PathFileExists(strAppExe))
			return FALSE;

		HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), strAppExe, strParams, NULL, SW_HIDE);
		if (hInstance <= (HINSTANCE)32)
			return FALSE;

		return TRUE;
	}
	catch (...)
	{
	}

	return FALSE;
}
