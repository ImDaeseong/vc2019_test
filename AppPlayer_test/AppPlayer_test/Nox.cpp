#include "pch.h"
#include "Nox.h"

Nox::Nox()
{
}

Nox::~Nox()
{
}

CString Nox::getNoxPath()
{
	TCHAR tPath[MAX_PATH];
	ULONG size{ MAX_PATH };

	CString strPath;
	CRegKey regKey;

	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Nox"), KEY_READ) != ERROR_SUCCESS)
	{
		return _T("");
	}

	if (regKey.QueryStringValue(_T("UninstallString"), tPath, &size) == ERROR_SUCCESS)
	{
		strPath = tPath;
	}
	regKey.Close();

	return strPath;
}

CString Nox::getNoxVersion()
{
	TCHAR tPath[MAX_PATH];
	ULONG size{ MAX_PATH };

	CString strPath;
	CRegKey regKey;

	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Nox"), KEY_READ) != ERROR_SUCCESS)
	{
		return _T("");
	}

	if (regKey.QueryStringValue(_T("DisplayVersion"), tPath, &size) == ERROR_SUCCESS)
	{
		strPath = tPath;
	}
	regKey.Close();

	return strPath;
}

BOOL Nox::DefaultRunNox()
{
	try
	{
		CString strNoxPath = Common::GetFilePath(getNoxPath());
		if (strNoxPath.IsEmpty())
			return FALSE;

		CString strAppExe;
		strAppExe.Format(_T("%s\\Nox.exe"), strNoxPath);

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

BOOL Nox::RunNox(CString strPackage)
{
	try
	{
		CString strNoxPath = Common::GetFilePath(getNoxPath());
		if (strNoxPath.IsEmpty())
			return FALSE;

		CString strAppExe;
		strAppExe.Format(_T("%s\\Nox.exe"), strNoxPath);

		CString strParams;
		strParams.Format(_T("-package:%s"), strPackage);

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