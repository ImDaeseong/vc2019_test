#include "pch.h"
#include "framework.h"
#include "DragStatic_test.h"
#include "DragStatic_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDragStatictestDlg::CDragStatictestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAGSTATIC_TEST_DIALOG, pParent)
{
	m_pBackSkin = NULL;

	nStart1 = 1;
	nStart2 = 2;
	nStart3 = 3;
	nStart4 = 4;
	nSelectIndex = 0;
}

void CDragStatictestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DRAG1, m_ctlDrag1);
	DDX_Control(pDX, IDC_STATIC_DRAG2, m_ctlDrag2);
	DDX_Control(pDX, IDC_STATIC_DRAG3, m_ctlDrag3);
	DDX_Control(pDX, IDC_STATIC_DRAG4, m_ctlDrag4);
	DDX_Control(pDX, IDC_STATIC_CURSOR, m_ctlCurosr);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
}

BEGIN_MESSAGE_MAP(CDragStatictestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER_DRAGCONTROL, DragControl)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDragStatictestDlg::OnBnClickedButtonClose)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BOOL CDragStatictestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDragStatictestDlg::swap(int *start, int *end)
{
	int temp = 0;
	temp = *start;
	*start = *end;
	*end = temp;
}

void CDragStatictestDlg::Num1(int nStart1)
{
	if (nStart1 == 1)
		m_ctlDrag1.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM1), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart1 == 2)
		m_ctlDrag1.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM2), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart1 == 3)
		m_ctlDrag1.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM3), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart1 == 4)
		m_ctlDrag1.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM4), _T("PNG"), AfxGetApp()->m_hInstance);
}

void CDragStatictestDlg::Num2(int nStart2)
{
	if (nStart2 == 1)
		m_ctlDrag2.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM1), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart2 == 2)
		m_ctlDrag2.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM2), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart2 == 3)
		m_ctlDrag2.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM3), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart2 == 4)
		m_ctlDrag2.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM4), _T("PNG"), AfxGetApp()->m_hInstance);
}

void CDragStatictestDlg::Num3(int nStart3)
{
	if (nStart3 == 1)
		m_ctlDrag3.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM1), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart3 == 2)
		m_ctlDrag3.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM2), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart3 == 3)
		m_ctlDrag3.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM3), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart3 == 4)
		m_ctlDrag3.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM4), _T("PNG"), AfxGetApp()->m_hInstance);
}

void CDragStatictestDlg::Num4(int nStart4)
{
	if (nStart4 == 1)
		m_ctlDrag4.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM1), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart4 == 2)
		m_ctlDrag4.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM2), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart4 == 3)
		m_ctlDrag4.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM3), _T("PNG"), AfxGetApp()->m_hInstance);
	else if (nStart4 == 4)
		m_ctlDrag4.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM4), _T("PNG"), AfxGetApp()->m_hInstance);
}

afx_msg LRESULT CDragStatictestDlg::DragControl(WPARAM wParam, LPARAM lParam)
{
	CString strMsg;

	switch (wParam) {
	case IDC_STATIC_DRAG1:

		if (lParam == 1)
		{
			nSelectIndex = 1;

			//strMsg.Format(_T("nStart1:%d"), nStart1);
			//OutputDebugString(strMsg);

			if (nStart1 == 1)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM1), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart1 == 2)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM2), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart1 == 3)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM3), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart1 == 4)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM4), _T("PNG"), AfxGetApp()->m_hInstance);
		}
		else if (lParam == 2)
		{	
			if (nSelectIndex == 1)
			{
				swap(&nStart1, &nStart1);

				Num1(nStart1);
			}
			else if (nSelectIndex == 2)
			{
				swap(&nStart2, &nStart1);

				Num2(nStart2);
				Num1(nStart1);
			}
			else if (nSelectIndex == 3)
			{
				swap(&nStart3, &nStart1);

				Num3(nStart3);
				Num1(nStart1);
			}
			else if (nSelectIndex == 4)
			{
				swap(&nStart4, &nStart1);

				Num4(nStart4);
				Num1(nStart1);
			}
		}
		
		OutputDebugString(m_ctlDrag1.m_strDragFile);
		break;

	case IDC_STATIC_DRAG2:

		if (lParam == 1)
		{
			nSelectIndex = 2;

			//strMsg.Format(_T("nStart2:%d"), nStart2);
			//OutputDebugString(strMsg);

			if (nStart2 == 1)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM1), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart2 == 2)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM2), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart2 == 3)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM3), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart2 == 4)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM4), _T("PNG"), AfxGetApp()->m_hInstance);
		}
		else if (lParam == 2)
		{			
			if (nSelectIndex == 1)
			{
				swap(&nStart1, &nStart2);

				Num1(nStart1);
				Num2(nStart2);				
			}
			else if (nSelectIndex == 2)
			{
				swap(&nStart2, &nStart2);

				Num2(nStart2);
			}
			else if (nSelectIndex == 3)
			{
				swap(&nStart3, &nStart2);

				Num3(nStart3);
				Num2(nStart2);
			}
			else if (nSelectIndex == 4)
			{
				swap(&nStart4, &nStart2);

				Num4(nStart4);
				Num2(nStart2);
			}
		}

		OutputDebugString(m_ctlDrag2.m_strDragFile);
		break;


	case IDC_STATIC_DRAG3:

		if (lParam == 1)
		{
			nSelectIndex = 3;

			//strMsg.Format(_T("nStart3:%d"), nStart3);
			//OutputDebugString(strMsg);

			if (nStart3 == 1)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM1), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart3 == 2)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM2), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart3 == 3)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM3), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart3 == 4)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM4), _T("PNG"), AfxGetApp()->m_hInstance);
		}
		else if (lParam == 2)
		{
			if (nSelectIndex == 1)
			{
				swap(&nStart1, &nStart3);

				Num1(nStart1);
				Num3(nStart3);
			}
			else if (nSelectIndex == 2)
			{
				swap(&nStart2, &nStart3);

				Num2(nStart2);
				Num3(nStart3);
			}
			else if (nSelectIndex == 3)
			{
				swap(&nStart3, &nStart3);

				Num3(nStart3);
			}
			else if (nSelectIndex == 4)
			{
				swap(&nStart4, &nStart3);

				Num4(nStart4);
				Num3(nStart3);
			}
		}

		OutputDebugString(m_ctlDrag3.m_strDragFile);
		break;

		
	case IDC_STATIC_DRAG4:

		if (lParam == 1)
		{
			nSelectIndex = 4;

			//strMsg.Format(_T("nStart4:%d"), nStart4);
			//OutputDebugString(strMsg);

			if (nStart4 == 1)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM1), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart4 == 2)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM2), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart4 == 3)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM3), _T("PNG"), AfxGetApp()->m_hInstance);
			else if (nStart4 == 4)
				m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM4), _T("PNG"), AfxGetApp()->m_hInstance);
		}
		else if (lParam == 2)
		{
			if (nSelectIndex == 1)
			{
				swap(&nStart1, &nStart4);

				Num1(nStart1);
				Num4(nStart4);
			}
			else if (nSelectIndex == 2)
			{
				swap(&nStart2, &nStart4);

				Num2(nStart2);
				Num4(nStart4);
			}
			else if (nSelectIndex == 3)
			{
				swap(&nStart3, &nStart4);

				Num3(nStart3);
				Num4(nStart4);
			}
			else if (nSelectIndex == 4)
			{
				swap(&nStart4, &nStart4);

				Num4(nStart4);
			}
		}
	
		OutputDebugString(m_ctlDrag4.m_strDragFile);
		break;

	default:
		m_ctlCurosr.MoveWindow(-100, -100, 35, 35);
		break;
	};

	return 0;
}

BOOL CDragStatictestDlg::OnInitDialog()
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

	CenterWindow(GetDesktopWindow());

	return TRUE;  
}

void CDragStatictestDlg::InitControls()
{
	CRect rClient;
	GetClientRect(&rClient);

	m_btnClose.SetImage(MAKEINTRESOURCE(IDB_PNG_CLOSE), _T("PNG"), AfxGetApp()->m_hInstance, (rClient.right - 35), 0, 4);

	//default 1
	m_ctlDrag1.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM1), _T("PNG"), AfxGetApp()->m_hInstance);
	m_ctlDrag1.MoveWindow(50, 80, 35, 35);

	//default 2
	m_ctlDrag2.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM2), _T("PNG"), AfxGetApp()->m_hInstance);
	m_ctlDrag2.MoveWindow(190, 80, 35, 35);

	//default 3
	m_ctlDrag3.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM3), _T("PNG"), AfxGetApp()->m_hInstance);
	m_ctlDrag3.MoveWindow(340, 80, 35, 35);

	//default 4
	m_ctlDrag4.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM4), _T("PNG"), AfxGetApp()->m_hInstance);
	m_ctlDrag4.MoveWindow(460, 80, 35, 35);
	
	//default cursor 1
	m_ctlCurosr.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM1), _T("PNG"), AfxGetApp()->m_hInstance);
	m_ctlCurosr.MoveWindow(-100, -100, 35, 35);
}

void CDragStatictestDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

void CDragStatictestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pBackSkin != NULL)
		delete m_pBackSkin;
	m_pBackSkin = NULL;
}

BOOL CDragStatictestDlg::LoadSkin()
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

void CDragStatictestDlg::DrawSkin(CDC* pDC)
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

	if (m_btnClose.IsAvailableDraw())
		m_btnClose.OnDrawLayerdWindow(gps);

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

void CDragStatictestDlg::SetMoveWindow(BOOL bInWindow)
{
	if (bInWindow)
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		m_ctlCurosr.MoveWindow(pt.x, pt.y, 35, 35);
	}
	else
	{
		m_ctlCurosr.MoveWindow(-100, -100, 35, 35);
	}
}

void CDragStatictestDlg::OnBnClickedButtonClose()
{
	CDialogEx::OnOK();
}

void CDragStatictestDlg::OnMouseMove(UINT nFlags, CPoint point)
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