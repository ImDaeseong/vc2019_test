#include "stdafx.h"
#include "vc08define_test.h"
#include "vc08define_testDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Cvc08define_testDlg::Cvc08define_testDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cvc08define_testDlg::IDD, pParent)
{
}

void Cvc08define_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cvc08define_testDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL Cvc08define_testDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strPath1 = EXE1_PATH;
	CString strPath2 = EXE2_PATH;
	CString strUrl = URL_PATH;

	return TRUE;  
}

void Cvc08define_testDlg::OnPaint()
{
	CPaintDC dc(this);
}