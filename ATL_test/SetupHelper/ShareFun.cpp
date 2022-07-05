// ShareFun.cpp : CShareFun 구현

#include "pch.h"
#include "ShareFun.h"
#include "Func.h"

// CShareFun

STDMETHODIMP CShareFun::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IShareFun
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

https://github.com/search?l=C%2B%2B&o=desc&p=3&q=AFX_MANAGE_STATE+pch.h&s=indexed&type=Code

STDMETHODIMP CShareFun::GetSystemPath(VARIANT* vtPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;

	CString strPath = Func::GetSystemFolderPath();		
	vtPath->vt = VT_BSTR;
	vtPath->bstrVal = strPath.AllocSysString();

	return S_OK;
}


STDMETHODIMP CShareFun::GetProgramFilesPath(VARIANT* vtPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;

	CString strPath = Func::GetProgramFilesFolderPath();
	vtPath->vt = VT_BSTR;
	vtPath->bstrVal = strPath.AllocSysString();

	return S_OK;
}
