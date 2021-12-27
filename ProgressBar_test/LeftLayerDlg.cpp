#include "pch.h"
#include "ProgressBar_test.h"
#include "afxdialogex.h"
#include "LeftLayerDlg.h"
#include "LeftLayerDlg.h"

#define NumCount 61

typedef struct {
	int  nNumber;
	UINT uFirst;
	UINT uSecond;
} PicNumber_t;

static const PicNumber_t NumberPic[NumCount] =
{
	{0,  IDB_PNG_NUM0, IDB_PNG_NUM0 },
	{1,  IDB_PNG_NUM0, IDB_PNG_NUM1 },
	{2,  IDB_PNG_NUM0, IDB_PNG_NUM2 },
	{3,  IDB_PNG_NUM0, IDB_PNG_NUM3 },
	{4,  IDB_PNG_NUM0, IDB_PNG_NUM4 },
	{5,  IDB_PNG_NUM0, IDB_PNG_NUM5 },
	{6,  IDB_PNG_NUM0, IDB_PNG_NUM6 },
	{7,  IDB_PNG_NUM0, IDB_PNG_NUM7 },
	{8,  IDB_PNG_NUM0, IDB_PNG_NUM8 },
	{9,  IDB_PNG_NUM0, IDB_PNG_NUM9 },
	{10, IDB_PNG_NUM1, IDB_PNG_NUM0 },
	{11, IDB_PNG_NUM1, IDB_PNG_NUM1 },
	{12, IDB_PNG_NUM1, IDB_PNG_NUM2 },
	{13, IDB_PNG_NUM1, IDB_PNG_NUM3 },
	{14, IDB_PNG_NUM1, IDB_PNG_NUM4 },
	{15, IDB_PNG_NUM1, IDB_PNG_NUM5 },
	{16, IDB_PNG_NUM1, IDB_PNG_NUM6 },
	{17, IDB_PNG_NUM1, IDB_PNG_NUM7 },
	{18, IDB_PNG_NUM1, IDB_PNG_NUM8 },
	{19, IDB_PNG_NUM1, IDB_PNG_NUM9 },
	{20, IDB_PNG_NUM2, IDB_PNG_NUM0 },
	{21, IDB_PNG_NUM2, IDB_PNG_NUM1 },
	{22, IDB_PNG_NUM2, IDB_PNG_NUM2 },
	{23, IDB_PNG_NUM2, IDB_PNG_NUM3 },
	{24, IDB_PNG_NUM2, IDB_PNG_NUM4 },
	{25, IDB_PNG_NUM2, IDB_PNG_NUM5 },
	{26, IDB_PNG_NUM2, IDB_PNG_NUM6 },
	{27, IDB_PNG_NUM2, IDB_PNG_NUM7 },
	{28, IDB_PNG_NUM2, IDB_PNG_NUM8 },
	{29, IDB_PNG_NUM2, IDB_PNG_NUM9 },
	{30, IDB_PNG_NUM3, IDB_PNG_NUM0 },
	{31, IDB_PNG_NUM3, IDB_PNG_NUM1 },
	{32, IDB_PNG_NUM3, IDB_PNG_NUM2 },
	{33, IDB_PNG_NUM3, IDB_PNG_NUM3 },
	{34, IDB_PNG_NUM3, IDB_PNG_NUM4 },
	{35, IDB_PNG_NUM3, IDB_PNG_NUM5 },
	{36, IDB_PNG_NUM3, IDB_PNG_NUM6 },
	{37, IDB_PNG_NUM3, IDB_PNG_NUM7 },
	{38, IDB_PNG_NUM3, IDB_PNG_NUM8 },
	{39, IDB_PNG_NUM3, IDB_PNG_NUM9 },
	{40, IDB_PNG_NUM4, IDB_PNG_NUM0 },
	{41, IDB_PNG_NUM4, IDB_PNG_NUM1 },
	{42, IDB_PNG_NUM4, IDB_PNG_NUM2 },
	{43, IDB_PNG_NUM4, IDB_PNG_NUM3 },
	{44, IDB_PNG_NUM4, IDB_PNG_NUM4 },
	{45, IDB_PNG_NUM4, IDB_PNG_NUM5 },
	{46, IDB_PNG_NUM4, IDB_PNG_NUM6 },
	{47, IDB_PNG_NUM4, IDB_PNG_NUM7 },
	{48, IDB_PNG_NUM4, IDB_PNG_NUM8 },
	{49, IDB_PNG_NUM4, IDB_PNG_NUM9 },
	{50, IDB_PNG_NUM5, IDB_PNG_NUM0 },
	{51, IDB_PNG_NUM5, IDB_PNG_NUM1 },
	{52, IDB_PNG_NUM5, IDB_PNG_NUM2 },
	{53, IDB_PNG_NUM5, IDB_PNG_NUM3 },
	{54, IDB_PNG_NUM5, IDB_PNG_NUM4 },
	{55, IDB_PNG_NUM5, IDB_PNG_NUM5 },
	{56, IDB_PNG_NUM5, IDB_PNG_NUM6 },
	{57, IDB_PNG_NUM5, IDB_PNG_NUM7 },
	{58, IDB_PNG_NUM5, IDB_PNG_NUM8 },
	{59, IDB_PNG_NUM5, IDB_PNG_NUM9 },
	{60, IDB_PNG_NUM6, IDB_PNG_NUM0 }
};

IMPLEMENT_DYNAMIC(LeftLayerDlg, CDialogEx)

LeftLayerDlg::LeftLayerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEFT_DIALOG, pParent)
{
	m_pbgImage = NULL;

	m_nWidth = 0;
	m_nHeight = 0;

	m_nCount = 60;
}

LeftLayerDlg::~LeftLayerDlg()
{
	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void LeftLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_Btnclose);
	DDX_Control(pDX, IDC_STATIC_NUM1, m_StcNum0);
	DDX_Control(pDX, IDC_STATIC_NUM2, m_StcNum1);
	DDX_Control(pDX, IDC_STATIC_MSG, m_StcMsg);
}

BEGIN_MESSAGE_MAP(LeftLayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &LeftLayerDlg::OnBnClickedButtonClose)	
END_MESSAGE_MAP()

BOOL LeftLayerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void LeftLayerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);

	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void LeftLayerDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

BOOL LeftLayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadImage();

	InitControls();

	CDC* pDC = GetDC();
	DrawSkin(pDC);
	ReleaseDC(pDC);

	SetTimer(1, 1000, NULL);

	return TRUE;
}

void LeftLayerDlg::SetParent(CProgressBartestDlg* pParent)
{
	m_pParent = pParent;
}

void LeftLayerDlg::LoadImage()
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

void LeftLayerDlg::DrawSkin(CDC* pDC)
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

	if (m_Btnclose.IsAvailableDraw())
		m_Btnclose.OnDrawLayerdWindow(gps);

	if (m_StcNum0.IsAvailableDraw())
		m_StcNum0.OnDrawLayerdWindow(gps);

	if (m_StcNum1.IsAvailableDraw())
		m_StcNum1.OnDrawLayerdWindow(gps);

	m_StcMsg.OnDrawLayerdWindow(gps);

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

void LeftLayerDlg::InitControls()
{
	m_Btnclose.SetImage(MAKEINTRESOURCE(IDB_PNG_4CLOSE), _T("PNG"), AfxGetApp()->m_hInstance, 5, 5, 4);

	m_StcNum0.SetImage(MAKEINTRESOURCE(NumberPic[m_nCount].uFirst), _T("PNG"), AfxGetApp()->m_hInstance);
	m_StcNum0.MoveWindow(120, 60, m_StcNum0.GetImageWidth(), m_StcNum0.GetImageHeight());

	m_StcNum1.SetImage(MAKEINTRESOURCE(NumberPic[m_nCount].uSecond), _T("PNG"), AfxGetApp()->m_hInstance);
	m_StcNum1.MoveWindow(140, 60, m_StcNum1.GetImageWidth(), m_StcNum1.GetImageHeight());

	CRect rc;
	GetClientRect(&rc);
	m_StcMsg.MoveWindow(10, rc.Height() - 40, rc.Width() - 20, 20);
	m_StcMsg.SetMesssageFontSize(14);
	m_StcMsg.SetMesssageText(_T("LeftLayerDlg"));
}

void LeftLayerDlg::OnBnClickedButtonClose()
{
	m_pParent->DestroyLeftLayerDlg();
}

void LeftLayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_nCount > 0)
		--m_nCount;

	if (m_nCount == 0)
	{
		KillTimer(1);
	}

	if (m_nCount > 0)
	{
		m_StcNum0.IsShow(true);
		m_StcNum0.SetImage(MAKEINTRESOURCE(NumberPic[m_nCount].uFirst), _T("PNG"), AfxGetApp()->m_hInstance);
		m_StcNum0.MoveWindow(120, 60, m_StcNum0.GetImageWidth(), m_StcNum0.GetImageHeight());

		m_StcNum1.IsShow(true);
		m_StcNum1.SetImage(MAKEINTRESOURCE(NumberPic[m_nCount].uSecond), _T("PNG"), AfxGetApp()->m_hInstance);
		m_StcNum1.MoveWindow(140, 60, m_StcNum1.GetImageWidth(), m_StcNum1.GetImageHeight());
	}
	else
	{
		m_StcNum0.IsShow(false);
		m_StcNum0.SetImage(MAKEINTRESOURCE(NumberPic[m_nCount].uFirst), _T("PNG"), AfxGetApp()->m_hInstance);
		m_StcNum0.MoveWindow(-1000, -1000, m_StcNum0.GetImageWidth(), m_StcNum0.GetImageHeight());

		m_StcNum1.IsShow(false);
		m_StcNum1.SetImage(MAKEINTRESOURCE(NumberPic[m_nCount].uSecond), _T("PNG"), AfxGetApp()->m_hInstance);
		m_StcNum1.MoveWindow(-1000, -1000, m_StcNum1.GetImageWidth(), m_StcNum1.GetImageHeight());
	}

	RedrawWindow();

	CDialogEx::OnTimer(nIDEvent);
}
