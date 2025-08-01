#include "pch.h"
#include "framework.h"
#include "Layprogress_test.h"
#include "Layprogress_testDlg.h"
#include "afxdialogex.h"
#include "LayerDlg.h"
#include "NormalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLayprogresstestDlg::CLayprogresstestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LAYPROGRESS_TEST_DIALOG, pParent)
{
	m_pLayerDlg = NULL;
	m_pNormalDlg = NULL;
}

void CLayprogresstestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLayprogresstestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CLayprogresstestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLayprogresstestDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CLayprogresstestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rc;
	GetWindowRect(&rc);

	int nScreenSizeX = GetSystemMetrics(SM_CXSCREEN);
	int nScreenSizeY = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow( (nScreenSizeX - rc.Width()),(nScreenSizeY - rc.Height()), rc.Width(), rc.Height());

	return TRUE;  
}

void CLayprogresstestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	DestroyLayerDlg();
	DestroyNormalDlg();
}

void CLayprogresstestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CLayprogresstestDlg::OnBnClickedButton1()
{	
	DestroyLayerDlg();
	CreateNormalDlg();
}

void CLayprogresstestDlg::OnBnClickedButton2()
{
	DestroyNormalDlg();
	CreateLayerDlg();
}

void CLayprogresstestDlg::CreateLayerDlg()
{
	if (!m_pLayerDlg)
	{
		m_pLayerDlg = new LayerDlg();
		m_pLayerDlg->SetParent(this);
		m_pLayerDlg->Create(IDD_DIALOG_LAYER, this);
		m_pLayerDlg->ShowWindow(SW_SHOW);
	}
}

void CLayprogresstestDlg::DestroyLayerDlg()
{
	if (m_pLayerDlg)
	{
		m_pLayerDlg->DestroyWindow();
		delete m_pLayerDlg;
		m_pLayerDlg = NULL;
	}
}

void CLayprogresstestDlg::CreateNormalDlg()
{
	if (!m_pNormalDlg)
	{
		m_pNormalDlg = new NormalDlg();
		m_pNormalDlg->SetParent(this);
		m_pNormalDlg->Create(IDD_DIALOG_NORMAL, this);
		m_pNormalDlg->ShowWindow(SW_SHOW);
	}
}

void CLayprogresstestDlg::DestroyNormalDlg()
{
	if (m_pNormalDlg)
	{
		m_pNormalDlg->DestroyWindow();
		delete m_pNormalDlg;
		m_pNormalDlg = NULL;
	}
}
