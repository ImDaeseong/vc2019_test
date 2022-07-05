#include "pch.h"
#include "Func.h"

Func::Func()
{
}
Func::~Func()
{
}

CString Func::GetSystemFolderPath()
{
	TCHAR tPath[MAX_PATH];
	::SHGetSpecialFolderPath(NULL, tPath, CSIDL_SYSTEM, FALSE);
	return (CString)tPath;
}

CString Func::GetProgramFilesFolderPath()
{
	TCHAR tPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, tPath, CSIDL_PROGRAM_FILES, FALSE);
	return (CString)tPath;
}

