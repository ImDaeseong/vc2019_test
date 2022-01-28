#include "pch.h"
#include "DesktopIcon.h"

DesktopIcon::DesktopIcon()
{
}

DesktopIcon::~DesktopIcon()
{
}

CString DesktopIcon::GetDesktopPath()
{
	TCHAR szPath[256];

	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_DESKTOP, FALSE);

	return (CString)szPath;
}

CString DesktopIcon::GetModulePath()
{
	TCHAR tPath[MAX_PATH];
	::GetModuleFileName(NULL, tPath, MAX_PATH);

	CString strPath = tPath;
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
	return strPath;
}

BOOL DesktopIcon::GetReourceFiles(CString strFileName, CString strResourceID, UINT nID)
{
	HINSTANCE hInst = AfxGetInstanceHandle();

	HRSRC hExeFile;
	hExeFile = ::FindResource(hInst, MAKEINTRESOURCE(nID), strResourceID);
	if (hExeFile == NULL)
		return FALSE;

	HGLOBAL hRes = ::LoadResource(hInst, hExeFile);
	if (hRes != NULL)
	{
		DWORD dwSize = ::SizeofResource(hInst, hExeFile);

		UINT FAR* lpnRes = (UINT FAR*)::LockResource(hRes);

		TRY
		{
			CFile file(strFileName, CFile::modeCreate | CFile::modeWrite);
			file.Write(lpnRes, dwSize);
			file.Flush();
		}
			CATCH(CFileException, e)
		{
		}
		END_CATCH

		::FreeResource(hRes);

		return TRUE;
	}

	return FALSE;
}

BOOL DesktopIcon::UnZipFolder(CString strCurrentPath, CString strFilePath)
{
	HZIP hZip;
	ZIPENTRY zipEntry;
	ZIPENTRY zipItem;
	CString strOutputFile;

	hZip = OpenZip(strFilePath, "");
	if (!hZip) return FALSE;

	GetZipItem(hZip, -1, &zipEntry);
	for (int i = 0; i < zipEntry.index; i++)
	{
		GetZipItem(hZip, i, &zipItem);

		strOutputFile.Format(_T("%s\\%s"), strCurrentPath, CString(zipItem.name));

		UnzipItem(hZip, i, strOutputFile);
	}
	CloseZip(hZip);

	::DeleteFile(strFilePath);

	return TRUE;
}

int DesktopIcon::FileCopy(CString strFrom, CString strTo)
{
	SHFILEOPSTRUCT    shfo;
	WORD        wfunc;
	TCHAR        pszTo[1024] = { 0 };
	TCHAR        pszFrom[1024] = { 0 };
	TCHAR        pszTitle[MAX_PATH] = { 0 };

	wfunc = FO_COPY;

	memset(pszTo, 0, sizeof(pszTo));
	memset(pszFrom, 0, sizeof(pszFrom));

	wcscpy(pszTitle, _T("파일복사"));
	wcscpy(pszTo, strTo);
	wcscpy(pszFrom, strFrom);

	ZeroMemory(&shfo, sizeof(shfo));

	shfo.hwnd = NULL;
	shfo.wFunc = wfunc;
	shfo.lpszProgressTitle = pszTitle;
	shfo.pTo = pszTo;

	shfo.pFrom = pszFrom;
	shfo.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;

	int ret = SHFileOperation(&shfo);

	return shfo.fAnyOperationsAborted;
}

BOOL DesktopIcon::DeleteFolder(CString strFolderPath)
{
    WIN32_FIND_DATA  FindData;

    HANDLE FindHandle;
    TCHAR temp1[MAX_PATH];
    int lpFolder_Len;

    TCHAR lpFolder[256];
    _stprintf(lpFolder, _T("%s"), strFolderPath);

    if (lpFolder == NULL)
    {
        return FALSE;
    }

    if (*lpFolder == '\0')
    {
        return FALSE;
    }

    lpFolder_Len = wcslen(lpFolder);

    if (lpFolder[lpFolder_Len - 1] == '\\')
    {
        _stprintf(temp1, _T("%s*.*"), lpFolder);
    }
    else
    {
        _stprintf(temp1, _T("%s\\*.*"), lpFolder);
    }

    FindHandle = FindFirstFile(temp1, &FindData);

    if (INVALID_HANDLE_VALUE != FindHandle)
    {
        while (1)
        {
            if (lpFolder[lpFolder_Len - 1] == '\\')
            {
                _stprintf(temp1, _T("%s%s"), lpFolder, FindData.cFileName);
            }
            else
            {
                _stprintf(temp1, _T("%s\\%s"), lpFolder, FindData.cFileName);
            }

            if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                //Dir
                if (lstrcmpi(FindData.cFileName, _T(".")) != 0 && lstrcmpi(FindData.cFileName, _T("..")) != 0)
                {          
                    DeleteFolder(temp1);

                    if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) || (FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
                    {
                        SetFileAttributes(temp1, FILE_ATTRIBUTE_NORMAL);
                    }

                    RemoveDirectory(temp1);
                }
            }
            else
            {
                //File
                if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) || (FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
                {
                    SetFileAttributes(temp1, FILE_ATTRIBUTE_NORMAL);
                }

                DeleteFile(temp1);
            }

            if (!FindNextFile(FindHandle, &FindData))
            {
                break;
            }
        }
    }

    FindClose(FindHandle);

    SetFileAttributes(lpFolder, FILE_ATTRIBUTE_NORMAL);

    RemoveDirectory(lpFolder);

    return TRUE;
}