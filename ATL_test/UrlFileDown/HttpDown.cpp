// HttpDown.cpp : CHttpDown 구현

#include "pch.h"
#include "HttpDown.h"


// CHttpDown

STDMETHODIMP CHttpDown::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IHttpDown
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CHttpDown::GetDownloadFile(VARIANT vtUrl, VARIANT vtFile, VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;


	VARIANT lc_vtURL;
	VariantInit(&lc_vtURL);

	if (vtUrl.vt == (VT_BYREF | VT_VARIANT))
		HRESULT hr = VariantCopy(&lc_vtURL, vtUrl.pvarVal);
	else
		HRESULT hr = VariantCopy(&lc_vtURL, &vtUrl);

	char* szURL;
	switch (lc_vtURL.vt)
	{
	case VT_BSTR:
		szURL = W2A(lc_vtURL.bstrVal);
		break;
	case VT_BSTR | VT_BYREF:
		szURL = W2A(lc_vtURL.pvarVal->bstrVal);
		break;
	default:
		::MessageBox(0, _T("형식이 일치 하지 않습니다."), _T("Error Message"), MB_ICONSTOP);
		return S_OK;
		break;
	}


	VARIANT lc_vtFile;
	VariantInit(&lc_vtFile);

	if (vtFile.vt == (VT_BYREF | VT_VARIANT))
		HRESULT hr = VariantCopy(&lc_vtFile, vtFile.pvarVal);
	else
		HRESULT hr = VariantCopy(&lc_vtFile, &vtFile);

	char* szFile;
	switch (lc_vtFile.vt)
	{
	case VT_BSTR:
		szFile = W2A(lc_vtFile.bstrVal);
		break;
	case VT_BSTR | VT_BYREF:
		szFile = W2A(lc_vtFile.pvarVal->bstrVal);
		break;
	default:
		::MessageBox(0, _T("형식이 일치 하지 않습니다."), _T("Error Message"), MB_ICONSTOP);
		return S_OK;
		break;
	}

	//#define VARIANT_TRUE ((VARIANT_BOOL)-1)
	//#define VARIANT_FALSE ((VARIANT_BOOL)0)

	if (HTTPDown(szURL, szFile))
	{
		*pVal = VARIANT_TRUE;
	}
	else
	{
		*pVal = VARIANT_FALSE;
	}

	return S_OK;
}

BOOL CHttpDown::HTTPDown(char* szUrl, char* szLocal, DWORD dwFileLen)
{
	HINTERNET hInternet, hURL;
	HANDLE hFile;
	DWORD Size;
	DWORD dwRead, dwWritten, dwRecv = 0;
	char state[256] = { 0 };
	char buf[15872];

	hInternet = InternetOpen(_T("http"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
	{
		return FALSE;
	}

	hURL = InternetOpenUrl(hInternet, szUrl, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hURL == NULL)
	{
		InternetCloseHandle(hInternet);
		return FALSE;
	}

	DWORD dwSize = sizeof(state);
	BOOL bQuery = HttpQueryInfo(hURL, HTTP_QUERY_STATUS_CODE, state, &dwSize, NULL);
	if (!bQuery)
	{
		InternetCloseHandle(hURL);
		InternetCloseHandle(hInternet);
		return FALSE;
	}

	if (!(atoi(state) != 200 || atoi(state) != 201))
	{
		InternetCloseHandle(hURL);
		InternetCloseHandle(hInternet);
		return FALSE;
	}

	hFile = CreateFile(szLocal, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	int nPersent = 0;
	CString strText;

	do
	{
		InternetQueryDataAvailable(hURL, &Size, 0, 0);
		InternetReadFile(hURL, buf, Size, &dwRead);
		WriteFile(hFile, buf, dwRead, &dwWritten, NULL);
	} while (dwRead != 0);

	InternetCloseHandle(hURL);
	InternetCloseHandle(hInternet);
	CloseHandle(hFile);

	return TRUE;
}