#include "pch.h"
#include "framework.h"
#include "versionutil_test.h"
#include "versionutil_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CversionutiltestDlg::CversionutiltestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VERSIONUTIL_TEST_DIALOG, pParent)
{
}

void CversionutiltestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CversionutiltestDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CversionutiltestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strTime = tiemInfo::GetCurrentTime();
	//AfxMessageBox(strTime);

	CString strFileversion = fileversionInfo::GetFileVersion(_T("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe"));
	//AfxMessageBox(strFileversion);

	CString strLocaIP = IpInfo::GetIPAddress();
	//AfxMessageBox(strLocaIP);

	bool bResponse = requestInfo::GetPageUrl(_T("https://www.naver.com/"));

	return TRUE;  
}

void CversionutiltestDlg::OnPaint()
{
	CPaintDC dc(this);
}
