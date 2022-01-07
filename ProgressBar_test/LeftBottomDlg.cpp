#include "pch.h"
#include "ProgressBar_test.h"
#include "afxdialogex.h"
#include "LeftBottomDlg.h"

IMPLEMENT_DYNAMIC(LeftBottomDlg, CDialogEx)

LeftBottomDlg::LeftBottomDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEFT_BT_DIALOG, pParent)
{
	m_pbgImage = NULL;

	m_nWidth = 0;
	m_nHeight = 0;

	m_nCount = 0;
}

LeftBottomDlg::~LeftBottomDlg()
{
	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void LeftBottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(LeftBottomDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL LeftBottomDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void LeftBottomDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);

	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void LeftBottomDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

BOOL LeftBottomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadImage();

	InitControls();

	SetTimer(1, 1000, NULL);

	return TRUE;
}

void LeftBottomDlg::SetParent(CProgressBartestDlg* pParent)
{
	m_pParent = pParent;
}

void LeftBottomDlg::LoadImage()
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

void LeftBottomDlg::DrawSkin(CDC* pDC)
{
	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();

	Graphics gps(pDC->GetSafeHdc());
	gps.DrawImage(m_pbgImage->m_pBitmap, Rect(0, 0, x, y), 0, 0, x, y, UnitPixel);
}

void LeftBottomDlg::InitControls()
{
	CRect rc;
	GetClientRect(&rc);
}

void LeftBottomDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_nCount = m_nCount + 10;
	if (m_nCount > 100)
		m_nCount = 0;


	CDialogEx::OnTimer(nIDEvent);
}