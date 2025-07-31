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
	m_overWnd = NULL;
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
	ON_BN_CLICKED(IDC_BUTTON5, &CgiftestDlg::OnBnClickedButton5)
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
	
	if (m_overWnd)
	{
		if (m_overWnd->GetSafeHwnd())
		{
			m_overWnd->DestroyWindow();
			delete m_overWnd;
			m_overWnd = NULL;
		}
	}

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

void CgiftestDlg::OnBnClickedButton5()
{
	CRect rc;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_GIF);
	pWnd->GetWindowRect(&rc);

	int nWidth = rc.Width();
	int nHeight = rc.Height();
	CRect rcWeb(rc.left, rc.bottom + 10, rc.left + nWidth, rc.bottom + 10 + nHeight);

	if (m_overWnd == NULL)
	{
		m_overWnd = new CGifOverlayWnd();
		m_overWnd->CreateEx(WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
			AfxRegisterWndClass(0), NULL, WS_POPUP | WS_VISIBLE, rcWeb, this, 0);

		//경로
		//m_overWnd->LoadGif(_T("..\\a.gif"));

		//리소스
		m_overWnd->LoadGifResource(IDR_MY_GIF2, RT_RCDATA);

		m_overWnd->ShowWindow(SW_SHOW);
		m_overWnd->Start();
	}
	else
	{
		m_overWnd->MoveWindow(rcWeb);
	}	
}
