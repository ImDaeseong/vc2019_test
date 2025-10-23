#include "pch.h"
#include "framework.h"
#include "BrowserUrlMonitor_test.h"
#include "BrowserUrlMonitor_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBrowserUrlMonitortestDlg::CBrowserUrlMonitortestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BROWSERURLMONITOR_TEST_DIALOG, pParent)
{
	m_pUrlMonitor = NULL;
}

void CBrowserUrlMonitortestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBrowserUrlMonitortestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(WM_URL_DETECTED, &CBrowserUrlMonitortestDlg::OnUrlDetected)
END_MESSAGE_MAP()

BOOL CBrowserUrlMonitortestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitUrlMonitor();

	return TRUE;  
}

void CBrowserUrlMonitortestDlg::OnPaint()
{
	CPaintDC dc(this);
}

BOOL CBrowserUrlMonitortestDlg::DestroyWindow()
{
	ReleaseUrlMonitor();

	return CDialog::DestroyWindow();
}

LRESULT CBrowserUrlMonitortestDlg::OnUrlDetected(WPARAM, LPARAM lParam)
{
	BrowserUrlInfo* pInfo = (BrowserUrlInfo*)lParam;
	if (pInfo)
	{
		CString strUrl = pInfo->strUrl;
		HWND hwnd = pInfo->hwndBrowser;

		wchar_t szTitle[MAX_PATH] = { 0 };
		::GetWindowTextW(hwnd, szTitle, MAX_PATH);

		CString strMsg;
		strMsg.Format(_T("브라우저 : %p\r\n제목 : %s\r\n주소 : %s\n"), hwnd, szTitle, strUrl);
		OutputDebugString(strMsg);

		SetDlgItemText(IDC_EDIT1, _T(""));
		SetDlgItemText(IDC_EDIT1, strMsg);

		delete pInfo;
	}
	return 0;
}

BOOL CBrowserUrlMonitortestDlg::InitUrlMonitor()
{
	m_pUrlMonitor = new CBrowserUrlMonitor();
	if (!m_pUrlMonitor->StartMonitor())
	{
		delete m_pUrlMonitor;
		m_pUrlMonitor = NULL;
		return FALSE;
	}
	return TRUE;
}

void CBrowserUrlMonitortestDlg::ReleaseUrlMonitor()
{
	if (m_pUrlMonitor)
	{
		m_pUrlMonitor->StopMonitor();
		delete m_pUrlMonitor;
		m_pUrlMonitor = NULL;
	}
}