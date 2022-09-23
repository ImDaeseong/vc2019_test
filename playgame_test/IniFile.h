#pragma once

class CIniFile
{
public:
	CIniFile();
	virtual ~CIniFile();
	
	BOOL    CreateIniFile(CString strIniPath);

	BOOL    SetTotalCount(CString strCount);
	int     GetTotalCount();

	BOOL    SetInfo(CString strIndex, CString strName);
	CString GetInfo(CString strIndex);


private:
	void    SetFilePath(LPCTSTR lpFileFullPath);
	LPCTSTR GetFilePath();

	DWORD   GetProfileString(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize);
	BOOL    SetProfileString(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPCTSTR lpString);

	TCHAR mFilePath[MAX_PATH];
};
