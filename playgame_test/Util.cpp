#include "pch.h"
#include "Util.h"

CString Util::GetModulePath()
{
	TCHAR tPath[MAX_PATH];
	::GetModuleFileName(NULL, tPath, MAX_PATH);

	CString strPath = tPath;
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
	return strPath;
}

BOOL Util::IsWindows64Bit()
{
	if (sizeof(int*) == 8) return TRUE;
	BOOL b = FALSE;
	IsWow64Process(GetCurrentProcess(), &b);
	return b;
}

CString Util::GetPrcessFileName(DWORD processID)
{
	CString strFileName;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap)
	{
		if (Process32First(hProcessSnap, &pe32))
		{
			do
			{
				if (pe32.th32ProcessID == processID)
				{
					strFileName.Format(_T("%s"), pe32.szExeFile);
					break;
				}

			} while (Process32Next(hProcessSnap, &pe32));
		}
	}

	CloseHandle(hProcessSnap);

	return strFileName;
}

BOOL Util::runAppplayer()
{
	//run C:\Program Files\Google\Play Games\Bootstrapper.exe

	CString strInstallPath = RegInfo::IsExistGoogleplaygameInfo(_T("InstallFolder"));
	if (strInstallPath.IsEmpty())
		return FALSE;

	CString strAppExe;
	strAppExe.Format(_T("%s\\Bootstrapper.exe"), strInstallPath);
	if (!::PathFileExists(strAppExe))
		return FALSE;

	HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), strAppExe, NULL, NULL, SW_SHOW);
	if (hInstance <= (HINSTANCE)32)
		return FALSE;

	return TRUE;
}