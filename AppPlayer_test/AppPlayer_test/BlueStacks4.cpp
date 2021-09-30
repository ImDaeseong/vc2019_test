#include "pch.h"
#include "BlueStacks4.h"

BlueStacks4::BlueStacks4()
{
}

BlueStacks4::~BlueStacks4()
{
}

CString BlueStacks4::getBlueStacks4Path()
{
	TCHAR tPath[MAX_PATH];
	ULONG size{ MAX_PATH };

	CString strPath;
	CRegKey regKey;

	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\BlueStacks"), KEY_READ) != ERROR_SUCCESS)
	{
		return _T("");
	}

	if (regKey.QueryStringValue(_T("InstallDir"), tPath, &size) == ERROR_SUCCESS)
	{
		strPath = tPath;
	}
	regKey.Close();

	return strPath;
}

CString BlueStacks4::getBlueStacks4Version()
{
	TCHAR tPath[MAX_PATH];
	ULONG size{ MAX_PATH };

	CString strPath;
	CRegKey regKey;

	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\BlueStacks"), KEY_READ) != ERROR_SUCCESS)
	{
		return _T("");
	}

	if (regKey.QueryStringValue(_T("Version"), tPath, &size) == ERROR_SUCCESS)
	{
		strPath = tPath;
	}
	regKey.Close();

	return strPath;
}

BOOL BlueStacks4::DefaultRunBlueStacks4()
{
	try
	{
		CString strBlueStacks4Path = Common::GetFilePath(getBlueStacks4Path());
		if (strBlueStacks4Path.IsEmpty())
			return FALSE;

		CString strAppExe;
		strAppExe.Format(_T("%s\\HD-RunApp.exe"), strBlueStacks4Path);

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

BOOL BlueStacks4::RunBlueStacks4(CString strPackage, CString strActivity)
{
	try
	{
		CString strBlueStacks4Path = Common::GetFilePath(getBlueStacks4Path());
		if (strBlueStacks4Path.IsEmpty())
			return FALSE;

		CString strAppExe;
		strAppExe.Format(_T("%s\\HD-RunApp.exe"), strBlueStacks4Path);

		CString strParams;
		strParams.Format(_T(" -p %s -a %s"), strPackage, strActivity);
		
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