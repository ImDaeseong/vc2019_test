// dllmain.cpp : DllMain의 구현입니다.

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "UrlFileDown_i.h"
#include "dllmain.h"
#include "xdlldata.h"

CUrlFileDownModule _AtlModule;

class CUrlFileDownApp : public CWinApp
{
public:

// 재정의입니다.
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CUrlFileDownApp, CWinApp)
END_MESSAGE_MAP()

CUrlFileDownApp theApp;

BOOL CUrlFileDownApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, nullptr))
		return FALSE;
#endif
	return CWinApp::InitInstance();
}

int CUrlFileDownApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
