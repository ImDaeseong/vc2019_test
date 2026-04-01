#include "pch.h"
#include "framework.h"
#include "BrowserUrlMonitor_test.h"
#include "BrowserUrlMonitor_testDlg.h"
#include "afxdialogex.h"
#include "BrowserUrlMonitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBrowserUrlMonitortestDlg::CBrowserUrlMonitortestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BROWSERURLMONITOR_TEST_DIALOG, pParent)
{
}

void CBrowserUrlMonitortestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBrowserUrlMonitortestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CBrowserUrlMonitortestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetTimer(1, 1000, NULL);

	return TRUE;  
}

void CBrowserUrlMonitortestDlg::OnPaint()
{
	CPaintDC dc(this);
}

BOOL CBrowserUrlMonitortestDlg::DestroyWindow()
{
	CBrowserUrlMonitor::ReleaseInstance();

	return CDialog::DestroyWindow();
}

void CBrowserUrlMonitortestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		HWND hForeground = ::GetForegroundWindow();
		if (hForeground != NULL)
		{
			CBrowserUrlMonitor* pMonitor = CBrowserUrlMonitor::GetInstance();

			if (pMonitor && pMonitor->IsSupportedBrowser(hForeground))
			{
				CString szUrl;

				if (pMonitor->GetLastKnownUrl(hForeground, szUrl))
				{
					CString strMsg;
					strMsg.Format(_T("감지된 URL: %s\n"), szUrl);

					SetDlgItemText(IDC_EDIT1, _T(""));
					SetDlgItemText(IDC_EDIT1, strMsg);

				}
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}
