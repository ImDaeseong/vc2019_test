#include "StdAfx.h"
#include "RegInfo.h"

RegInfo::RegInfo(void)
{
}

RegInfo::~RegInfo(void)
{
}

CString RegInfo::GetModulePath()
{
	TCHAR tPath[MAX_PATH];
	::GetModuleFileName(NULL, tPath, MAX_PATH);

	CString strPath = tPath;
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
	return strPath;
}