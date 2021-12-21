#include "pch.h"
#include "ProgressBar_test.h"
#include "afxdialogex.h"
#include "RightDlg.h"

IMPLEMENT_DYNAMIC(RightDlg, CDialogEx)

RightDlg::RightDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RIGHT_BT_DIALOG, pParent)
{
	m_pbgImage = NULL;

	m_nWidth = 0;
	m_nHeight = 0;

	m_nCount = 0;
}

RightDlg::~RightDlg()
{
	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void RightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_STATIC_PROGRESS_PNG, m_StcProgressPng);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_PNGA, m_StcProgressPngA);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_StcProgress);	
}

BEGIN_MESSAGE_MAP(RightDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL RightDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void RightDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);

	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void RightDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

BOOL RightDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadImage();

	InitControls();

	SetTimer(1, 1000, NULL);

	return TRUE;
}

void RightDlg::SetParent(CProgressBartestDlg* pParent)
{
	m_pParent = pParent;
}

void RightDlg::LoadImage()
{
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	if (!m_pbgImage)
	{
		m_pbgImage = new CGdiPlusBitmapResource;

		if (!m_pbgImage->Load(MAKEINTRESOURCE(IDB_PNG1_DLG), _T("PNG"), AfxGetApp()->m_hInstance))
			return;
	}

	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();
	MoveWindow(0, 0, x, y);

	m_nWidth = x;
	m_nHeight = y;
}

void RightDlg::DrawSkin(CDC* pDC)
{
	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();

	Graphics gps(pDC->GetSafeHdc());
	gps.DrawImage(m_pbgImage->m_pBitmap, Rect(0, 0, x, y), 0, 0, x, y, UnitPixel);
}

void RightDlg::InitControls()
{
	CRect rc;
	GetClientRect(&rc);

	m_StcProgressPng.SetNoImage();
	m_StcProgressPng.MoveWindow(10, 50, rc.Width() - 20, 20);
	m_StcProgressPng.SetRange(0, 100);
	m_StcProgressPng.SetPos(0);

	m_StcProgressPngA.SetImage(MAKEINTRESOURCE(IDB_PNG_BAR1), MAKEINTRESOURCE(IDB_PNG_BAR2), _T("PNG"), AfxGetApp()->m_hInstance);
	m_StcProgressPngA.MoveWindow(10, 80, rc.Width() - 20, 20);
	m_StcProgressPngA.SetRange(0, 100);
	m_StcProgressPngA.SetPos(0);

	m_StcProgress.MoveWindow(10, 110, rc.Width() - 20, 20);
	m_StcProgress.SetRange(0, 100);
	m_StcProgress.SetPos(0);
}

void RightDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_nCount = m_nCount + 10;
	if (m_nCount > 100)
		m_nCount = 0;

	m_StcProgressPng.SetPos(m_nCount);

	m_StcProgressPngA.SetPos(m_nCount);

	m_StcProgress.SetPos(m_nCount);

	CDialogEx::OnTimer(nIDEvent);
}