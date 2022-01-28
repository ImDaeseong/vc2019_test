#pragma once

class DesktopIcon
{
public:
	DesktopIcon();
	virtual ~DesktopIcon();

public:
	static CString GetDesktopPath();
	static CString GetModulePath();
	static BOOL GetReourceFiles(CString strFileName, CString strResourceID, UINT nID);
	static BOOL UnZipFolder(CString strCurrentPath, CString strFilePath = _T(""));
	static int FileCopy(CString strFrom, CString strTo);
	static BOOL DeleteFolder(CString strFolderPath);
};

