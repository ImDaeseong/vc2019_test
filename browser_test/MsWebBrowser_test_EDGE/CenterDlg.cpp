#include "pch.h"
#include "MsWebBrowser_test.h"
#include "afxdialogex.h"
#include "CenterDlg.h"
#include "MsWebBrowser_testDlg.h"

IMPLEMENT_DYNAMIC(CenterDlg, CDialog)

CenterDlg::CenterDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CENTER_DIALOG, pParent)
{
}

CenterDlg::~CenterDlg()
{
}

void CenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CenterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CenterDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CenterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOPMOST);
	SetLayeredWindowAttributes(0, 1, LWA_ALPHA);

	return TRUE; 
}

void CenterDlg::OnPaint()
{
	CPaintDC dc(this); 
}

void CenterDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

void CenterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pParent->CallMain();
	m_pParent->CreateTopDlg();
	m_pParent->CreateBottomDlg();
	CDialog::OnLButtonDown(nFlags, point);
}

void CenterDlg::SetParent(CMsWebBrowsertestDlg* pParent)
{
	m_pParent = pParent;
}