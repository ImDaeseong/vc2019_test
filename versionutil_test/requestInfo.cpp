#include "pch.h"
#include "requestInfo.h"

bool requestInfo::GetPageUrl(CString strUrl)
{
	if (getWebPage(strUrl))
	{
		Sleep(100);
		return true;
	}
	return false;
}

bool requestInfo::getWebPage(CString strUrl)
{
	CInternetSession session;
	CInternetFile* pFile = NULL;

	try
	{
		pFile = (CInternetFile*)session.OpenURL(strUrl, 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_DONT_CACHE, NULL, 0);
	}
	catch (CInternetException* pEx)
	{
		pFile = NULL;
		pEx->Delete();
		return false;
	}

	CString szContent = _T("");

	if (pFile)
	{
		CString strData;
		pFile->SetReadBufferSize(4096);
		while (pFile->ReadString(strData))
		{
			szContent = szContent + CA2W((LPCSTR)strData.GetBuffer(), CP_UTF8) + _T("\r\n");
		}		
		pFile->Close();
		session.Close();
	}

	OutputDebugString(szContent);
	return true;
}
