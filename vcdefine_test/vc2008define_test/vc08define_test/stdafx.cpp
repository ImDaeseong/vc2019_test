#include "stdafx.h"

CString gGetModulePath()
{
	TCHAR tPath[MAX_PATH];
	::GetModuleFileName(NULL, tPath, MAX_PATH);

	CString strPath = tPath;
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
	return strPath;
}

CString gGetModulePath_RegInfo()
{
	RegInfo reginfo;
	return reginfo.GetModulePath();
}

CString gGetParamUrl()
{
	return "?where=nexearch&sm=top_hty&fbm=1&ie=utf8&query=movie";
}