#include "pch.h"
#include "MsWebBrowser_test.h"
#include "afxdialogex.h"
#include "TopDlg.h"
#include "MsWebBrowser_testDlg.h"

IMPLEMENT_DYNAMIC(TopDlg, CDialog)

TopDlg::TopDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TOP_DIALOG, pParent)
{
	m_pbgImage = NULL;
}

TopDlg::~TopDlg()
{
	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void TopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TEXT, mDisplayText);
}

BEGIN_MESSAGE_MAP(TopDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL TopDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL TopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadImage();
	DrawSkin();

	return TRUE; 
}

void TopDlg::LoadImage()
{
	if (!m_pbgImage)
	{
		m_pbgImage = new CGdiPlusBitmapResource;
		if (!m_pbgImage->Load(MAKEINTRESOURCE(IDB_PNG_BG_TOP), _T("PNG"), AfxGetApp()->m_hInstance))
			return;
	}
	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();

	const DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED;
	ModifyStyleEx(0, dwExStyle);
	MoveWindow(0, 0, x, y);

	mDisplayText.MoveWindow(10, 10, x - 20, 40);
	mDisplayText.SetMesssageText(_T("유튜브 더보기"));
	mDisplayText.SetBannerClick(TRUE);
}

void TopDlg::DrawSkin()
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

	mDisplayText.OnDrawLayerdWindow(gps);

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

void TopDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin();
}

void TopDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void TopDlg::SetParent(CMsWebBrowsertestDlg* pParent)
{
	m_pParent = pParent;
}

int TopDlg::GetWidth()
{
	if (m_pbgImage)
	{
		return m_pbgImage->m_pBitmap->GetWidth();
	}
	return 0;
}

int TopDlg::GetHeight()
{
	if (m_pbgImage)
	{
		return m_pbgImage->m_pBitmap->GetHeight();
	}
	return 0;
}