#include "pch.h"
#include "framework.h"
#include "vcdefine_test.h"
#include "vcdefine_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CvcdefinetestDlg::CvcdefinetestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VCDEFINE_TEST_DIALOG, pParent)
{
}

void CvcdefinetestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CvcdefinetestDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CvcdefinetestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strPath1 = EXE1_PATH;
	CString strPath2 = EXE2_PATH;
	CString strUrl = URL_PATH;

	OutputDebugString(strPath1 + _T("\r\n"));
	OutputDebugString(strPath2 + _T("\r\n"));
	OutputDebugString(strUrl + _T("\r\n"));

	return TRUE;  
}

void CvcdefinetestDlg::OnPaint()
{
	CPaintDC dc(this);
}
