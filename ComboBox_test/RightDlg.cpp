#include "pch.h"
#include "ComboBox_test.h"
#include "afxdialogex.h"
#include "RightDlg.h"

IMPLEMENT_DYNAMIC(RightDlg, CDialogEx)

RightDlg::RightDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RIGHT_DIALOG, pParent)
{
	m_pbgImage = NULL;

	m_nWidth = 0;
	m_nHeight = 0;
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
	DDX_Control(pDX, IDC_COMBO1, m_cbo1);
	DDX_Control(pDX, IDC_COMBO2, m_cbo2);
	DDX_Control(pDX, IDC_COMBO3, m_cbo3);
	DDX_Control(pDX, IDC_COMBO4, m_cbo4);
}

BEGIN_MESSAGE_MAP(RightDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
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

	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void RightDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin();
}

BOOL RightDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadImage();

	InitControls();

	InitData();

	DrawSkin();

	return TRUE;
}

void RightDlg::InitData()
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

void RightDlg::InitControls()
{
	CRect rc;
	GetClientRect(&rc);
}

void RightDlg::SetParent(CComboBoxtestDlg* pParent)
{
	m_pParent = pParent;
}

void RightDlg::LoadImage()
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

void RightDlg::DrawSkin()
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

	m_cbo1.OnDrawLayerdWindow(gps);
	m_cbo2.OnDrawLayerdWindow(gps);

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