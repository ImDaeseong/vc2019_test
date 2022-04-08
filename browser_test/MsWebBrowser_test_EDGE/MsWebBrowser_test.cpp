#include "pch.h"
#include "framework.h"
#include "MsWebBrowser_test.h"
#include "MsWebBrowser_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMsWebBrowsertestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CMsWebBrowsertestApp::CMsWebBrowsertestApp()
{
	m_gdiplusToken = NULL;
}

CMsWebBrowsertestApp theApp;

BOOL CMsWebBrowsertestApp::InitInstance()
{
	CoInitialize(NULL);

	m_hMutex = ::CreateMutex(NULL, FALSE, _T("MsWebBrowsertestInstance"));
	if (ERROR_ALREADY_EXISTS == ::GetLastError())
		return FALSE;

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
		
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	RegisterWin();

	AfxEnableControlContainer();

	CShellManager *pShellManager = new CShellManager;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		
	CMsWebBrowsertestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}

	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	return FALSE;
}

int CMsWebBrowsertestApp::ExitInstance()
{
	if (m_gdiplusToken != NULL)
	{
		GdiplusShutdown(m_gdiplusToken);
		m_gdiplusToken = NULL;
	}

	CoUninitialize();

	return CWinApp::ExitInstance();
}

BOOL CMsWebBrowsertestApp::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F1)
		{
			return TRUE;
		}
	}
	else if (pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_F4)
		{
			return TRUE;
		}
	}

	return CWinApp::PreTranslateMessage(pMsg);
}

void CMsWebBrowsertestApp::RegisterWin()
{
	WNDCLASS wndclass;
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wndclass);
	CString strClassName = _T("MsWebBrowsertest");
	wndclass.lpszClassName = strClassName;
	BOOL bSuccess;
	bSuccess = AfxRegisterClass(&wndclass);
}