#include "pch.h"
#include "Launcher_test.h"
#include "DragDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(DragDlg, CDialogEx)

DragDlg::DragDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAG_DIALOG, pParent)
{
	m_pBackSkin = NULL;
}

DragDlg::~DragDlg()
{
}

void DragDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DRAG1, m_ctlDrag1);
	DDX_Control(pDX, IDC_STATIC_DRAG2, m_ctlDrag2);
	DDX_Control(pDX, IDC_STATIC_DRAG3, m_ctlDrag3);
	DDX_Control(pDX, IDC_STATIC_DRAG4, m_ctlDrag4);
	DDX_Control(pDX, IDC_STATIC_CURSOR, m_ctlCurosr);
}

BEGIN_MESSAGE_MAP(DragDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER_DRAGCONTROL, DragControl)
END_MESSAGE_MAP()

BOOL DragDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

afx_msg LRESULT DragDlg::DragControl(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case IDC_STATIC_DRAG1:
		OutputDebugString(m_ctlDrag1.m_strDragFile);
		break;

	case IDC_STATIC_DRAG2:
		OutputDebugString(m_ctlDrag2.m_strDragFile);
		break;

	case IDC_STATIC_DRAG3:
		OutputDebugString(m_ctlDrag3.m_strDragFile);
		break;

	case IDC_STATIC_DRAG4:
		OutputDebugString(m_ctlDrag4.m_strDragFile);
		break;

	default:
		m_ctlCurosr.MoveWindow(-100, -100, 35, 35);
		break;
	};

	return 0;
}

BOOL DragDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	if (!LoadSkin())
	{
		CDialogEx::OnCancel();
		return FALSE;
	}

	m_ctlDrag1.SetImage(MAKEINTRESOURCE(IDB_PNG_IMG1), _T("PNG"), AfxGetApp()->m_hInstance);
	m_ctlDrag1.MoveWindow(25, 60, 35, 35);

	m_ctlDrag2.SetImage(MAKEINTRESOURCE(IDB_PNG_IMG2), _T("PNG"), AfxGetApp()->m_hInstance);
	m_ctlDrag2.MoveWindow(85, 60, 35, 35);

	m_ctlDrag3.SetImage(MAKEINTRESOURCE(IDB_PNG_IMG3), _T("PNG"), AfxGetApp()->m_hInstance);
	m_ctlDrag3.MoveWindow(145, 60, 35, 35);

	m_ctlDrag4.SetImage(MAKEINTRESOURCE(IDB_PNG_IMG4), _T("PNG"), AfxGetApp()->m_hInstance);
	m_ctlDrag4.MoveWindow(205, 60, 35, 35);
	
	m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_IMG4), _T("PNG"), AfxGetApp()->m_hInstance);
	m_ctlCurosr.MoveWindow(-100, -100, 35, 35);

	CDC* pDC = GetDC();
	DrawSkin(pDC);
	ReleaseDC(pDC);

	CenterWindow(GetDesktopWindow());

	return TRUE;
}

void DragDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

void DragDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pBackSkin != NULL)
		delete m_pBackSkin;
	m_pBackSkin = NULL;
}

BOOL DragDlg::LoadSkin()
{
	m_pBackSkin = new CGdiPlusBitmapResource;
	if (!m_pBackSkin->LoadPath(_T("")))
	{
		if (!m_pBackSkin->Load(MAKEINTRESOURCE(IDB_PNG_BG1), _T("PNG"), AfxGetApp()->m_hInstance))
		{
			delete m_pBackSkin;
			m_pBackSkin = NULL;
			return FALSE;
		}
	}

	return TRUE;
}

void DragDlg::DrawSkin(CDC* pDC)
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

	if (m_ctlDrag1.IsAvailableDraw())
		m_ctlDrag1.OnDrawLayerdWindow(gps);

	if (m_ctlDrag2.IsAvailableDraw())
		m_ctlDrag2.OnDrawLayerdWindow(gps);

	if (m_ctlDrag3.IsAvailableDraw())
		m_ctlDrag3.OnDrawLayerdWindow(gps);

	if (m_ctlDrag4.IsAvailableDraw())
		m_ctlDrag4.OnDrawLayerdWindow(gps);

	if (m_ctlCurosr.IsAvailableDraw())
		m_ctlCurosr.OnDrawLayerdWindow(gps);
	
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

void DragDlg::SetParent(CLaunchertestDlg* pParent)
{
	m_pParent = pParent;
}

int DragDlg::GetWidth()
{
	return m_pBackSkin->m_pBitmap->GetWidth();
}

int DragDlg::GetHeight()
{
	return m_pBackSkin->m_pBitmap->GetHeight();
}
