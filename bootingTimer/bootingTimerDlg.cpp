#include "pch.h"
#include "framework.h"
#include "bootingTimer.h"
#include "bootingTimerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CbootingTimerDlg::CbootingTimerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BOOTINGTIMER_DIALOG, pParent)
{
}

void CbootingTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CbootingTimerDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CbootingTimerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	BootTimerManager obj1;
	CString strResult = obj1.GetBootTime();
	AfxMessageBox(strResult);

	return TRUE; 
}

void CbootingTimerDlg::OnPaint()
{
	CPaintDC dc(this);
}
