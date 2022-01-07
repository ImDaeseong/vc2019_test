#include "pch.h"
#include "framework.h"
#include "ProgressBar_test.h"
#include "ProgressBar_testDlg.h"
#include "afxdialogex.h"

#include "LeftLayerDlg.h"
#include "LeftBottomDlg.h"
#include "RightLayerDlg.h"
#include "RightDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CProgressBartestDlg::CProgressBartestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROGRESSBAR_TEST_DIALOG, pParent)
{
	m_pbgImage = NULL;

	m_pLeftLayerDlg = NULL;
	m_pLeftBottomDlg = NULL;

	m_pRightLayerDlg = NULL;
	m_pRightDlg = NULL;
}

void CProgressBartestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_Btnclose);
	DDX_Control(pDX, IDC_BUTTON_CLICK, m_Btnclick);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_StcMsg);
}

BEGIN_MESSAGE_MAP(CProgressBartestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()	
	ON_WM_MOVE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CProgressBartestDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_CLICK, &CProgressBartestDlg::OnBnClickedButtonClick)
END_MESSAGE_MAP()

BOOL CProgressBartestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	
	LoadImage();

	InitControls();

	CenterWindow(GetDesktopWindow());

	return TRUE; 
}

void CProgressBartestDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

void CProgressBartestDlg::InitControls()
{
	m_Btnclose.SetImage(MAKEINTRESOURCE(IDB_PNG_4CLOSE), _T("PNG"), AfxGetApp()->m_hInstance, 120, 5, 4);

	m_Btnclick.SetImage(MAKEINTRESOURCE(IDB_PNG_CLICK), _T("PNG"), AfxGetApp()->m_hInstance, 61, 192, 1);

	CRect rc;
	GetClientRect(&rc);
	m_StcMsg.MoveWindow(10, 260, rc.Width() - 20, 40);
	m_StcMsg.SetMesssageFontSize(14);
	m_StcMsg.SetMesssageText(_T("화면테스트"));	
}

void CProgressBartestDlg::LoadImage()
{
	if (!m_pbgImage)
	{
		m_pbgImage = new CGdiPlusBitmapResource;

		if (!m_pbgImage->Load(MAKEINTRESOURCE(IDB_PNG_DLG), _T("PNG"), AfxGetApp()->m_hInstance))
			return;
	}

	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();
	MoveWindow(0, 0, x, y);
}

void CProgressBartestDlg::DrawSkin(CDC* pDC)
{
	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();

	Graphics gps(pDC->GetSafeHdc());
	gps.DrawImage(m_pbgImage->m_pBitmap, Rect(0, 0, x, y), 0, 0, x, y, UnitPixel);
}

HBRUSH CProgressBartestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CProgressBartestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	DestroyLeftLayerDlg();

	DestroyRightLayerDlg();

	DestroyRightDlg();

	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void CProgressBartestDlg::OnMouseMove(UINT nFlags, CPoint point)
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

void CProgressBartestDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	SetChildPos();
}

LRESULT CProgressBartestDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	SetChildPos();
	return 0;
}

BOOL CProgressBartestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CProgressBartestDlg::OnBnClickedButtonClose()
{
	CDialogEx::OnOK();
}

void CProgressBartestDlg::OnBnClickedButtonClick()
{
	CreateLeftLayerDlg();
	CreateLeftBottomDlg();
	CreateRightLayerDlg();
	CreateRightDlg();
}

void CProgressBartestDlg::CreateLeftLayerDlg()
{
	if (!m_pLeftLayerDlg)
	{
		m_pLeftLayerDlg = new LeftLayerDlg();
		m_pLeftLayerDlg->SetParent(this);
		m_pLeftLayerDlg->Create(IDD_LEFT_DIALOG, this);
		SetChildPos();
		m_pLeftLayerDlg->ShowWindow(SW_SHOW);
	}
	else
		m_pLeftLayerDlg->SetActiveWindow();
}

void CProgressBartestDlg::DestroyLeftLayerDlg()
{
	if (m_pLeftLayerDlg)
	{
		m_pLeftLayerDlg->DestroyWindow();
		delete m_pLeftLayerDlg;
		m_pLeftLayerDlg = NULL;
	}
}

void CProgressBartestDlg::CreateLeftBottomDlg()
{
	if (!m_pLeftBottomDlg)
	{
		m_pLeftBottomDlg = new LeftBottomDlg();
		m_pLeftBottomDlg->SetParent(this);
		m_pLeftBottomDlg->Create(IDD_LEFT_BT_DIALOG, this);
		SetChildPos();
		m_pLeftBottomDlg->ShowWindow(SW_SHOW);
	}
	else
		m_pLeftBottomDlg->SetActiveWindow();
}

void CProgressBartestDlg::DestroyLeftBottomDlg()
{
	if (m_pLeftBottomDlg)
	{
		m_pLeftBottomDlg->DestroyWindow();
		delete m_pLeftBottomDlg;
		m_pLeftBottomDlg = NULL;
	}
}

void CProgressBartestDlg::CreateRightLayerDlg()
{
	if (!m_pRightLayerDlg)
	{
		m_pRightLayerDlg = new RightLayerDlg();
		m_pRightLayerDlg->SetParent(this);
		m_pRightLayerDlg->Create(IDD_RIGHT_DIALOG, this);
		SetChildPos();
		m_pRightLayerDlg->ShowWindow(SW_SHOW);
	}
	else
		m_pRightLayerDlg->SetActiveWindow();
}

void CProgressBartestDlg::DestroyRightLayerDlg()
{
	if (m_pRightLayerDlg)
	{
		m_pRightLayerDlg->DestroyWindow();
		delete m_pRightLayerDlg;
		m_pRightLayerDlg = NULL;
	}
}

void CProgressBartestDlg::CreateRightDlg()
{
	if (!m_pRightDlg)
	{
		m_pRightDlg = new RightDlg();
		m_pRightDlg->SetParent(this);
		m_pRightDlg->Create(IDD_RIGHT_BT_DIALOG, this);
		SetChildPos();
		m_pRightDlg->ShowWindow(SW_SHOW);
	}
	else
		m_pRightDlg->SetActiveWindow();
}

void CProgressBartestDlg::DestroyRightDlg()
{
	if (m_pRightDlg)
	{
		m_pRightDlg->DestroyWindow();
		delete m_pRightDlg;
		m_pRightDlg = NULL;
	}
}

void CProgressBartestDlg::SetChildPos()
{
	if (m_pLeftLayerDlg)
	{
		CRect rc;
		GetWindowRect(rc);

		int nX = rc.left - m_pLeftLayerDlg->m_nWidth;
		int nY = rc.top;
		m_pLeftLayerDlg->MoveWindow(nX, nY, m_pLeftLayerDlg->m_nWidth, m_pLeftLayerDlg->m_nHeight);
	}

	if (m_pLeftBottomDlg)
	{
		CRect rc;
		GetWindowRect(rc);

		int nX = rc.left - m_pLeftBottomDlg->m_nWidth;
		int nY = rc.top + m_pLeftBottomDlg->m_nHeight;
		m_pLeftBottomDlg->MoveWindow(nX, nY, m_pLeftBottomDlg->m_nWidth, m_pLeftBottomDlg->m_nHeight);
	}

	if (m_pRightLayerDlg)
	{
		CRect rc;
		GetWindowRect(rc);

		int nX = rc.right;
		int nY = rc.top;
		m_pRightLayerDlg->MoveWindow(nX, nY, m_pRightLayerDlg->m_nWidth, m_pRightLayerDlg->m_nHeight);
	}	

	if (m_pRightDlg)
	{
		CRect rc;
		GetWindowRect(rc);

		int nX = rc.right;
		int nY = rc.top + m_pRightDlg->m_nHeight;
		m_pRightDlg->MoveWindow(nX, nY, m_pRightDlg->m_nWidth, m_pRightDlg->m_nHeight);
	}
}

void CProgressBartestDlg::MainCallFunc()
{

}