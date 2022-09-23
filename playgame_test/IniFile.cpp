#include "pch.h"
#include "IniFile.h"

CIniFile::CIniFile()
{
	::memset(mFilePath, 0x00, MAX_PATH);
}

CIniFile::~CIniFile()
{
}

BOOL CIniFile::CreateIniFile(CString strIniPath)
{
	int nExist = _waccess((LPCTSTR)strIniPath, 0);
	if (nExist == -1)
	{
		FILE* outFile;
		fopen_s(&outFile, (LPSTR)(LPCTSTR)strIniPath, "wt");
		if (outFile == NULL)
		{
			return FALSE;
		}
		fclose(outFile);
	}

	SetFilePath(strIniPath);

	return TRUE;
}

BOOL CIniFile::SetTotalCount(CString strCount)
{
	if (!SetProfileString(_T("GameInfoList"), _T("GAMECOUNT"), strCount))
	{
		return FALSE;
	}
	return TRUE;
}

int CIniFile::GetTotalCount()
{
	int nCount = GetPrivateProfileInt(_T("GameInfoList"), _T("GAMECOUNT"), 0, mFilePath);
	return nCount;
}

BOOL CIniFile::SetInfo(CString strIndex, CString strName)
{
	if (!SetProfileString(strIndex, _T("GAMENAME"), strName))
	{
		return FALSE;
	}
	return TRUE;
}

CString CIniFile::GetInfo(CString strIndex)
{
	TCHAR tName[MAX_PATH] = { _T("") };
	CString strName = _T("");

	DWORD dwResult = GetProfileString(strIndex, _T("GAMENAME"), _T(""), tName, MAX_PATH);
	strName.Format(_T("%s"), tName);
	return strName;
}

void CIniFile::SetFilePath(LPCTSTR lpFileFullPath)
{
	::lstrcpy(mFilePath, lpFileFullPath);
}

LPCTSTR CIniFile::GetFilePath()
{
	return mFilePath;
}

DWORD CIniFile::GetProfileString(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize)
{
	return ::GetPrivateProfileString(lpszSection, lpKeyName, lpDefault, lpReturnedString, nSize, mFilePath);
}

BOOL CIniFile::SetProfileString(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	return ::WritePrivateProfileString(lpszSection, lpKeyName, lpString, mFilePath);
}
