#include "pch.h"
#include "ProgressBar_test.h"
#include "afxdialogex.h"
#include "RightLayerDlg.h"

IMPLEMENT_DYNAMIC(RightLayerDlg, CDialogEx)

RightLayerDlg::RightLayerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RIGHT_DIALOG, pParent)
{
	m_pbgImage = NULL;

	m_nWidth = 0;
	m_nHeight = 0;

	m_nCount = 0;
}

RightLayerDlg::~RightLayerDlg()
{
	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void RightLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_Btnclose);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_PNG, m_StcProgressPng);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_PNGA, m_StcProgressPngA);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_StcProgress);
}

BEGIN_MESSAGE_MAP(RightLayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &RightLayerDlg::OnBnClickedButtonClose)	
END_MESSAGE_MAP()

BOOL RightLayerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void RightLayerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);

	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void RightLayerDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin();
}

BOOL RightLayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadImage();

	InitControls();

	DrawSkin();

	SetTimer(1, 1000, NULL);

	return TRUE;
}

void RightLayerDlg::SetParent(CProgressBartestDlg* pParent)
{
	m_pParent = pParent;
}

void RightLayerDlg::LoadImage()
{
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW | WS_EX_LAYERED);

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

void RightLayerDlg::DrawSkin()
{
	int nWidth = m_pbgImage->m_pBitmap->GetWidth();
	int nHeight = m_pbgImage->m_pBitmap->GetHeight();

	HDC hDC = ::GetDC(GetSafeHwnd());
	HDC hMemoryDC = ::CreateCompatibleDC(hDC);
	HBITMAP hMemoryBmp = (HBITMAP)::CreateCompatibleBitmap(hDC, nWidth, nHeight);
	::SelectObject(hMemoryDC, hMemoryBmp);
	Graphics gps(hMemoryDC);

	gps.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	gps.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	gps.SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
	gps.DrawImage(m_pbgImage->m_pBitmap, 0, 0, nWidth, nHeight);

	if (m_Btnclose.IsAvailableDraw())
		m_Btnclose.OnDrawLayerdWindow(gps);

	if(m_StcProgressPng.IsAvailableDraw())
		m_StcProgressPng.OnDrawLayerdWindow(gps);

	if (m_StcProgressPngA.IsAvailableDraw())
		m_StcProgressPngA.OnDrawLayerdWindow(gps);

	m_StcProgress.OnDrawLayerdWindow(gps);

	RECT rcWindow;
	::GetWindowRect(GetSafeHwnd(), &rcWindow);

	POINT ptWinPos = { rcWindow.left, rcWindow.top };
	SIZE sizeWindow = { nWidth, nHeight };

	POINT ptSrc = { 0, 0 };

	BLENDFUNCTION bf;
	bf.BlendOp = 0x00;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0x01;
	bf.SourceConstantAlpha = 255;

	::UpdateLayeredWindow(GetSafeHwnd(), hDC, &ptWinPos, &sizeWindow, hMemoryDC, &ptSrc, 0, &bf, ULW_ALPHA);

	SAFE_DELETEDC(hMemoryDC);
	SAFE_DELETEOBJECT(hMemoryBmp);

	::ReleaseDC(m_hWnd, hDC);
}

void RightLayerDlg::InitControls()
{
	m_Btnclose.SetImage(MAKEINTRESOURCE(IDB_PNG_4CLOSE), _T("PNG"), AfxGetApp()->m_hInstance, 225, 5, 4);

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

void RightLayerDlg::OnBnClickedButtonClose()
{
	m_pParent->DestroyRightLayerDlg();
}

void RightLayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_nCount = m_nCount + 1;
	if (m_nCount > 100)
		m_nCount = 0;

	m_StcProgressPng.SetPos(m_nCount);

	m_StcProgressPngA.SetPos(m_nCount);

	m_StcProgress.SetPos(m_nCount);

	RedrawWindow();

	CDialogEx::OnTimer(nIDEvent);
}
