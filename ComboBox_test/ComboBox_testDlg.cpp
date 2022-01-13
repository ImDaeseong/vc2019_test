#include "pch.h"
#include "framework.h"
#include "ComboBox_test.h"
#include "ComboBox_testDlg.h"
#include "afxdialogex.h"

#include "LeftDlg.h"
#include "RightDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CComboBoxtestDlg::CComboBoxtestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMBOBOX_TEST_DIALOG, pParent)
{
	m_pLeftDlg = NULL;
	m_pRightDlg = NULL;
}

void CComboBoxtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbo1);
	DDX_Control(pDX, IDC_COMBO2, m_cbo2);
	DDX_Control(pDX, IDC_COMBO3, m_cbo3);
	DDX_Control(pDX, IDC_COMBO4, m_cbo4);
	DDX_Control(pDX, IDC_BUTTON_CLICK, m_Btnclick);
}

BEGIN_MESSAGE_MAP(CComboBoxtestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_BN_CLICKED(IDC_BUTTON_CLICK, &CComboBoxtestDlg::OnBnClickedButtonClick)
END_MESSAGE_MAP()

BOOL CComboBoxtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitControls();

	InitData();	
		
	return TRUE;  
}

void CComboBoxtestDlg::InitData()
{
	m_cbo1.IsArrowImage(TRUE);

	//ComboBoxEx Dropdown	
	m_cbo1.AddString(_T("test1"));
	m_cbo1.AddString(_T("test2"));
	m_cbo1.AddString(_T("test3"));
	m_cbo1.AddString(_T("test4"));
	m_cbo1.AddString(_T("test5"));

	//ComboBoxEx Drop List
	m_cbo2.AddString(_T("test1"));
	m_cbo2.AddString(_T("test2"));
	m_cbo2.AddString(_T("test3"));
	m_cbo2.AddString(_T("test4"));
	m_cbo2.AddString(_T("test5"));

	//ComboBox Dropdown
	m_cbo3.AddString(_T("test1"));
	m_cbo3.AddString(_T("test2"));
	m_cbo3.AddString(_T("test3"));
	m_cbo3.AddString(_T("test4"));
	m_cbo3.AddString(_T("test5"));

	//ComboBox Drop List
	m_cbo4.AddString(_T("test1"));
	m_cbo4.AddString(_T("test2"));
	m_cbo4.AddString(_T("test3"));
	m_cbo4.AddString(_T("test4"));
	m_cbo4.AddString(_T("test5"));

	m_cbo1.SetCurSel(0);
	m_cbo2.SetCurSel(0);
	m_cbo3.SetCurSel(0);
	m_cbo4.SetCurSel(0);
}

void CComboBoxtestDlg::InitControls()
{
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	CRect rc;
	GetClientRect(&rc);

	m_Btnclick.SetImage(MAKEINTRESOURCE(IDB_PNG_CLICK), _T("PNG"), AfxGetApp()->m_hInstance, (rc.Width()/2) - 20, rc.Height() - 40, 1);
}

void CComboBoxtestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CComboBoxtestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	DestroyLeftDlg();
	DestroyRightDlg();
}

void CComboBoxtestDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, 0, 0);
	}
	else
	{
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, 0, 0);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CComboBoxtestDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	SetChildPos();
}

LRESULT CComboBoxtestDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	SetChildPos();
	return 0;
}

BOOL CComboBoxtestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CComboBoxtestDlg::SetChildPos()
{
	if (m_pLeftDlg)
	{
		CRect rc;
		GetWindowRect(rc);

		int nX = rc.left - m_pLeftDlg->m_nWidth;
		int nY = rc.top;
		m_pLeftDlg->MoveWindow(nX, nY, m_pLeftDlg->m_nWidth, m_pLeftDlg->m_nHeight);
	}
		
	if (m_pRightDlg)
	{
		CRect rc;
		GetWindowRect(rc);

		int nX = rc.right;
		int nY = rc.top;
		m_pRightDlg->MoveWindow(nX, nY, m_pRightDlg->m_nWidth, m_pRightDlg->m_nHeight);
	}
}

void CComboBoxtestDlg::CreateLeftDlg()
{
	if (!m_pLeftDlg)
	{
		m_pLeftDlg = new LeftDlg();
		m_pLeftDlg->SetParent(this);
		m_pLeftDlg->Create(IDD_LEFT_DIALOG, this);
		SetChildPos();
		m_pLeftDlg->ShowWindow(SW_SHOW);
	}
	else
		m_pLeftDlg->SetActiveWindow();
}

void CComboBoxtestDlg::DestroyLeftDlg()
{
	if (m_pLeftDlg)
	{
		m_pLeftDlg->DestroyWindow();
		delete m_pLeftDlg;
		m_pLeftDlg = NULL;
	}
}

void CComboBoxtestDlg::CreateRightDlg()
{
	if (!m_pRightDlg)
	{
		m_pRightDlg = new RightDlg();
		m_pRightDlg->SetParent(this);
		m_pRightDlg->Create(IDD_RIGHT_DIALOG, this);
		SetChildPos();
		m_pRightDlg->ShowWindow(SW_SHOW);
	}
	else
		m_pRightDlg->SetActiveWindow();
}

void CComboBoxtestDlg::DestroyRightDlg()
{
	if (m_pRightDlg)
	{
		m_pRightDlg->DestroyWindow();
		delete m_pRightDlg;
		m_pRightDlg = NULL;
	}
}

void CComboBoxtestDlg::OnBnClickedButtonClick()
{
	CreateLeftDlg();
	CreateRightDlg();
}