#include "pch.h"
#include "framework.h"
#include "gif_test.h"
#include "gif_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CgiftestDlg::CgiftestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GIF_TEST_DIALOG, pParent)
{
}

void CgiftestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CgiftestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CgiftestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CgiftestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CgiftestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CgiftestDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

BOOL CgiftestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_gif.SubclassDlgItem(IDC_STATIC_GIF, this);
	
	//경로
	m_gif.LoadGif(_T("..\\a.gif"));
	
	//리소스
	//m_gif.LoadGifResource(IDR_MY_GIF1, RT_RCDATA);

	m_gif.Start();
		
	return TRUE;  
}

void CgiftestDlg::OnPaint()
{
	CPaintDC dc(this);
}

BOOL CgiftestDlg::DestroyWindow()
{
	m_gif.Stop();               
	m_gif.UnsubclassWindow();   
	
	return CDialogEx::DestroyWindow();
}

void CgiftestDlg::OnBnClickedButton1()
{
	//리소스
	m_gif.LoadGifResource(IDR_MY_GIF2, RT_RCDATA);
	m_gif.Start();
}

void CgiftestDlg::OnBnClickedButton2()
{
	//경로
	m_gif.LoadGif(_T("..\\a.gif"));
	m_gif.Start();
}

void CgiftestDlg::OnBnClickedButton3()
{
	//경로
	m_gif.LoadGif(_T("..\\b.gif"));
	m_gif.Start();	
}

void CgiftestDlg::OnBnClickedButton4()
{
	//리소스
	m_gif.LoadGifResource(IDR_MY_GIF1, RT_RCDATA);
	m_gif.Start();
}
