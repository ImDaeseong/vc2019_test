#include "pch.h"
#include "Layprogress_test.h"
#include "afxdialogex.h"
#include "LayerDlg.h"

IMPLEMENT_DYNAMIC(LayerDlg, CDialogEx)

LayerDlg::LayerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LAYER, pParent)
{
	progressBarWnd = NULL;

	m_pbgImage = NULL;

	m_nCount = 0;
}

LayerDlg::~LayerDlg()
{
	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void LayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(LayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
END_MESSAGE_MAP()

BOOL LayerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void LayerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);

	if (progressBarWnd)
	{
		if (progressBarWnd->GetSafeHwnd())
		{
			progressBarWnd->DestroyWindow();
			delete progressBarWnd;
			progressBarWnd = NULL;
		}
	}

	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void LayerDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

void LayerDlg::OnMouseMove(UINT nFlags, CPoint point)
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

void LayerDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	SetProPos();
}

LRESULT LayerDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	SetProPos();
	return 0;
}

BOOL LayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadSkin();

	InitControls();

	CenterWindow(GetDesktopWindow());

	CDC* pDC = GetDC();
	DrawSkin(pDC);
	ReleaseDC(pDC);

	SetTimer(1, 1000, NULL);

	return TRUE;
}

void LayerDlg::SetParent(CLayprogresstestDlg* pParent)
{
	m_pParent = pParent;
}

void LayerDlg::LoadSkin()
{
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	if (!m_pbgImage)
	{
		m_pbgImage = new CGdiPlusBitmapResource;

		if (!m_pbgImage->Load(MAKEINTRESOURCE(IDB_PNG1), _T("PNG"), AfxGetApp()->m_hInstance))
			return;
	}

	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();
	MoveWindow(0, 0, x, y);
}

void LayerDlg::DrawSkin(CDC* pDC)
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
		m_pbgImage->m_pBitmap,
		Rect(0, 0, m_pbgImage->m_pBitmap->GetWidth(), m_pbgImage->m_pBitmap->GetHeight()),
		0,
		0,
		m_pbgImage->m_pBitmap->GetWidth(),
		m_pbgImage->m_pBitmap->GetHeight(),
		UnitPixel);
		
	//m_RoundLineBar.OnDrawLayerdWindow(gps);
	m_RoundLineBarEx.OnDrawLayerdWindow(gps);

	m_MsgMessage1.OnDrawLayerdWindow(gps);
	m_MsgMessage2.OnDrawLayerdWindow(gps);
	
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

void LayerDlg::SetProPos()
{
	if (progressBarWnd)
	{
		CRect rcWnd;
		GetWindowRect(&rcWnd);

		CPoint ptPos = rcWnd.TopLeft();
		ptPos.Offset(690, 380);

		progressBarWnd->MoveWindow(ptPos.x, ptPos.y, 60, 60);
	}
}

void LayerDlg::InitControls()
{	
	static LPCTSTR s_overlayClass = AfxRegisterWndClass(0);
	progressBarWnd = new CProgressBarWnd();
	progressBarWnd->CreateEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, s_overlayClass, NULL, WS_POPUP | WS_VISIBLE, CRect(0,0,0,0), this, 0);
	SetProPos();
	progressBarWnd->Start();
		
	/*
	m_RoundLineBar.SubclassDlgItem(IDC_STATIC_LINE1, this);
	m_RoundLineBar.MoveWindow(522, 552, 396, 27);
	m_RoundLineBar.SetRange(0, 100);//범위 설정 (예: 0~100)	
	m_RoundLineBar.SetPos(0);// 진행률 설정 (예: 30%)
	*/

	m_RoundLineBarEx.SubclassDlgItem(IDC_STATIC_LINE1, this);
	m_RoundLineBarEx.MoveWindow(522, 552, 396, 27);
	m_RoundLineBarEx.SetRange(0, 100);//범위 설정 (예: 0~100)	
	m_RoundLineBarEx.SetPos(0);// 진행률 설정 (예: 30%)
    m_RoundLineBarEx.EnableGradient(true);
	m_RoundLineBarEx.EnableShadow(true);
	m_RoundLineBarEx.SetBorderColor(RGB(180, 180, 180));
	m_RoundLineBarEx.SetBorderWidth(0.5f);

	m_MsgMessage1.SubclassDlgItem(IDC_STATIC_MESSAGE1, this);
	m_MsgMessage1.MoveWindow(582, 458, 300, 70);
	m_MsgMessage1.SetMesssageFontSize(26);
	m_MsgMessage1.SetMesssageText(_T("업데이트 중입니다.\r\n잠시만 기다려 주세요."));
	
	m_MsgMessage2.SubclassDlgItem(IDC_STATIC_MESSAGE2, this);
	m_MsgMessage2.MoveWindow(925, 551, 75, 27);
	m_MsgMessage2.SetMesssageFontSize(20);
	m_MsgMessage2.SetMesssageText(_T("0%"));
}

void LayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_nCount += 10;
	if (m_nCount > 100)
		m_nCount = 0;

	//m_RoundLineBar.SetPos(m_nCount); // 진행률: 0.0 ~ 1.0

	m_RoundLineBarEx.SetPos(m_nCount); // 진행률: 0.0 ~ 1.0

	CString strMsg;
	strMsg.Format(_T("%d%%"), m_nCount);
	m_MsgMessage2.SetMesssageText(strMsg);	
			
	Invalidate();
	
	/*
	CDC* pDC = GetDC();
	DrawSkin(pDC);
	ReleaseDC(pDC);
	*/

	CDialogEx::OnTimer(nIDEvent);
}