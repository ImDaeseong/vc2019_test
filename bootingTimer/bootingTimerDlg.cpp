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
	CString strResult1 = obj1.GetBootTime();
	//AfxMessageBox(strResult1);

	//정확하지는 않음
	RegInfo reg;
	CString strResult2 = reg.getBootingCount();
	//AfxMessageBox(strResult2);

	return TRUE; 
}

void CbootingTimerDlg::OnPaint()
{
	CPaintDC dc(this);
}
