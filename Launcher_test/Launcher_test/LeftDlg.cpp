#include "pch.h"
#include "Launcher_test.h"
#include "LeftDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(LeftDlg, CDialogEx)

LeftDlg::LeftDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEFT_DIALOG, pParent)
{
	m_pBackSkin = NULL;
}

LeftDlg::~LeftDlg()
{
}

void LeftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(LeftDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL LeftDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL LeftDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	if (!LoadSkin())
	{
		CDialogEx::OnCancel();
		return FALSE;
	}

	CDC* pDC = GetDC();
	DrawSkin(pDC);
	ReleaseDC(pDC);

	return TRUE;  
}

void LeftDlg::OnPaint()
{
	CPaintDC dc(this); 
	DrawSkin(&dc);
}

void LeftDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pBackSkin != NULL)
		delete m_pBackSkin;
	m_pBackSkin = NULL;
}

BOOL LeftDlg::LoadSkin()
{
	m_pBackSkin = new CGdiPlusBitmapResource;
	if (!m_pBackSkin->LoadPath(_T("")))
	{
		if (!m_pBackSkin->Load(MAKEINTRESOURCE(IDB_PNG_BG), _T("PNG"), AfxGetApp()->m_hInstance))
		{
			delete m_pBackSkin;
			m_pBackSkin = NULL;
			return FALSE;
		}
	}

	return TRUE;
}

void LeftDlg::DrawSkin(CDC* pDC)
{
	/*
	if (m_pBackSkin->m_pBitmap == NULL)
		return;

	Graphics gps(pDC->GetSafeHdc());
	int x = m_pBackSkin->m_pBitmap->GetWidth();
	int y = m_pBackSkin->m_pBitmap->GetHeight();
	gps.DrawImage(m_pBackSkin->m_pBitmap, Rect(0, 0, x, y), 0, 0, x, y, UnitPixel);
	*/

	CDC memDC;
	CBitmap bmp;
	CBitmap* bmp_old;

	CRect rect;
	GetWindowRect(&rect);

	int cx = rect.Width();
	int cy = rect.Height();

	memDC.CreateCompatibleDC(pDC);

	BITMAPINFO bmInfo;
	memset(&bmInfo, 0x00, sizeof(BITMAPINFO));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = cx;
	bmInfo.bmiHeader.biHeight = cy;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 32;
	bmInfo.bmiHeader.biCompression = BI_RGB;
	bmInfo.bmiHeader.biSizeImage = 0;
	bmInfo.bmiHeader.biClrUsed = 0;
	bmInfo.bmiHeader.biClrImportant = 0;

	LPBYTE pbmpBits = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(pDC->GetSafeHdc(), (LPBITMAPINFO)&bmInfo, DIB_RGB_COLORS, (void**)&pbmpBits, NULL, 0);
	if (hBitmap == NULL)
		bmp.CreateCompatibleBitmap(pDC, cx, cy);
	else
		bmp.Attach(hBitmap);

	bmp_old = memDC.SelectObject(&bmp);
	Graphics gps(memDC.GetSafeHdc());

	gps.DrawImage(
		m_pBackSkin->m_pBitmap,
		Rect(0, 0, m_pBackSkin->m_pBitmap->GetWidth(), m_pBackSkin->m_pBitmap->GetHeight()),
		0,
		0,
		m_pBackSkin->m_pBitmap->GetWidth(),
		m_pBackSkin->m_pBitmap->GetHeight(),
		UnitPixel);

	CPoint ptWindowScreenPosition(rect.TopLeft());
	CSize szWindow(cx, cy);

	BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	CPoint ptSrc(0, 0);

	HDC hdc = gps.GetHDC();

	BOOL bRet = ::UpdateLayeredWindow(GetSafeHwnd(),
		pDC->GetSafeHdc(),
		&ptWindowScreenPosition,
		&szWindow,
		hdc,
		&ptSrc,
		0,
		&blendPixelFunction,
		ULW_ALPHA);

	gps.ReleaseHDC(hdc);

	memDC.SelectObject(bmp_old);
	bmp.DeleteObject();
	memDC.DeleteDC();
}

void LeftDlg::SetParent(CLaunchertestDlg* pParent)
{
	m_pParent = pParent;
}

int LeftDlg::GetWidth()
{
	return m_pBackSkin->m_pBitmap->GetWidth();
}

int LeftDlg::GetHeight()
{
	return m_pBackSkin->m_pBitmap->GetHeight();
}