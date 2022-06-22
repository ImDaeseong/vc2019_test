#include "pch.h"
#include "framework.h"
#include "Pngload.h"
#include "PngloadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPngloadDlg::CPngloadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PNGLOAD_DIALOG, pParent)
{
	m_pBackSkin = NULL;
}

void CPngloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
}

BEGIN_MESSAGE_MAP(CPngloadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CPngloadDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()

BOOL CPngloadDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CPngloadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	if (!LoadSkin())
	{
		CDialogEx::OnCancel();
		return FALSE;
	}

	InitControls();

	CDC* pDC = GetDC();
	DrawSkin(pDC);
	ReleaseDC(pDC);

	return TRUE; 
}

void CPngloadDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

void CPngloadDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pBackSkin != NULL)
		delete m_pBackSkin;
	m_pBackSkin = NULL;
}

BOOL CPngloadDlg::LoadSkin()
{
	m_pBackSkin = new CGdiPlusBitmapResource;
	if (!m_pBackSkin->LoadPath(_T("")))
	{
		if (!m_pBackSkin->Load(MAKEINTRESOURCE(IDB_PNG1), _T("PNG"), ((CPngloadApp*)AfxGetApp())->m_hResourceDll))
		{
			delete m_pBackSkin;
			m_pBackSkin = NULL;
			return FALSE;
		}
	}

	int cx = m_pBackSkin->m_pBitmap->GetWidth();
	int cy = m_pBackSkin->m_pBitmap->GetHeight();
	MoveWindow(0, 0, cx, cy);
		
	return TRUE;
}

void CPngloadDlg::DrawSkin(CDC* pDC)
{
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

	//CSkinButton
	if (m_btnClose.IsAvailableDraw())
		m_btnClose.OnDrawLayerdWindow(gps);

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

void CPngloadDlg::InitControls()
{
	CRect rClient;
	GetClientRect(&rClient);

	//CSkinButton
	m_btnClose.SetImage(MAKEINTRESOURCE(IDB_PNG2), _T("PNG"), ((CPngloadApp*)AfxGetApp())->m_hResourceDll, (rClient.right - 35), 0, 4);
}

void CPngloadDlg::OnBnClickedButtonClose()
{
	CDialogEx::OnOK();
}
