#include "pch.h"
#include "BlueStacks5.h"

BlueStacks5::BlueStacks5()
{
}

BlueStacks5::~BlueStacks5()
{
}

CString BlueStacks5::getBlueStacks5Path()
{
	TCHAR tPath[MAX_PATH];
	ULONG size{ MAX_PATH };

	CString strPath;
	CRegKey regKey;

	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\BlueStacks_nxt"), KEY_READ) != ERROR_SUCCESS)
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

CString BlueStacks5::getBlueStacks5Version()
{
	TCHAR tPath[MAX_PATH];
	ULONG size{ MAX_PATH };

	CString strPath;
	CRegKey regKey;

	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\BlueStacks_nxt"), KEY_READ) != ERROR_SUCCESS)
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

BOOL BlueStacks5::DefaultRunBlueStacks5()
{
	try
	{
		CString strBlueStacks5Path = Common::GetFilePath(getBlueStacks5Path());
		if (strBlueStacks5Path.IsEmpty())
			return FALSE;

		CString strAppExe;
		strAppExe.Format(_T("%s\\HD-Player.exe"), strBlueStacks5Path);

		if (!::PathFileExists(strAppExe))
			return FALSE;

		CString strParams;
		strParams.Format(_T("--instance Nougat32"));

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

BOOL BlueStacks5::RunBlueStacks5(CString strPackage, CString strActivity)
{
	try
	{
		CString strBlueStacks5Path = Common::GetFilePath(getBlueStacks5Path());
		if (strBlueStacks5Path.IsEmpty())
			return FALSE;

		CString strAppExe;
		strAppExe.Format(_T("%s\\HD-Player.exe"), strBlueStacks5Path);

		CString strParams;
		strParams.Format(_T("--instance Nougat32"));

		//파라미터 정보 변경됨 게임 호출 불가
		//BlueStacks_nxt\Engine\Nougat32\AppCache => 설치 프로그램 정보
		//strParams.Format(_T(" -p %s -a %s"), strPackage, strActivity);
		//strParams.Format(_T("--instance Nougat32 -p:%s -a:%s"), strPackage, strActivity);
		//strParams.Format(_T("--instance Nougat32 package=%s activity=%s"), strPackage, strActivity);

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