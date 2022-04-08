#include "pch.h"
#include "MsWebBrowser_test.h"
#include "afxdialogex.h"
#include "BottomDlg.h"
#include "MsWebBrowser_testDlg.h"

typedef struct {
	int  nIndex;
	UINT uNum1;
	UINT uNum2;
} PngEnum;

#define sum 61
static const PngEnum PngNum[sum] =
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

#define TotalCount 14

typedef struct {
	int index;
	int x;
	int y;
	int widht;
	int height;
} Point_t;

static const Point_t SpritePoint[TotalCount] =
{
	{0, 23,  2, 80, 81},
	{1, 128, 4, 86, 78},
	{2, 242, 8, 80, 74},
	{3, 337, 11, 95, 69},
	{4, 440, 34, 104, 45},

	{5,  24, 135, 79, 64 },
	{6,  140, 134, 72,  68},
	{7,  251, 135, 71, 63},
	{8,  359, 135, 72, 58},
	{9,  468, 137, 73, 58},

	{10, 32, 241, 71, 56},
	{11, 133, 242, 80, 55},
	{12, 228, 242, 96, 45},
	{13, 333, 224, 99, 63}
};


IMPLEMENT_DYNAMIC(BottomDlg, CDialog)

BottomDlg::BottomDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_BOTTOM_DIALOG, pParent)
{
	m_pbgImage = NULL;

	m_nCount = 0;

	m_pSprite = NULL;
	m_curIndex = 0;
	m_TotalIndex = 0;
}

BottomDlg::~BottomDlg()
{
	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void BottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_NUMF, m_PngNumF);
	DDX_Control(pDX, IDC_STATIC_NUME, m_PngNumE);
	DDX_Control(pDX, IDC_STATIC_BG, m_PngNumBG);
}

BEGIN_MESSAGE_MAP(BottomDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL BottomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadImage();
	DrawSkin();

	return TRUE; 
}

void BottomDlg::LoadImage()
{
	if (!m_pbgImage)
	{
		m_pbgImage = new CGdiPlusBitmapResource;
		if (!m_pbgImage->Load(MAKEINTRESOURCE(IDB_PNG_BG_BOTTOM), _T("PNG"), AfxGetApp()->m_hInstance))
			return;
	}
	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();

	const DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED;
	ModifyStyleEx(0, dwExStyle);
	MoveWindow(0, 0, x, y);


	m_PngNumF.IsBlueStacksShow(false);
	m_PngNumF.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM1), _T("PNG"), AfxGetApp()->m_hInstance);
	m_PngNumF.MoveWindow(27, 20, m_PngNumF.GetImageWidth(), m_PngNumF.GetImageHeight());
	m_PngNumF.SetPictureStaticClick(FALSE);
	m_PngNumF.SetPictureStaticCursor(FALSE);

	m_PngNumE.IsBlueStacksShow(false);
	m_PngNumE.SetImage(MAKEINTRESOURCE(IDB_PNG_NUM0), _T("PNG"), AfxGetApp()->m_hInstance);
	m_PngNumE.MoveWindow(42, 20, m_PngNumE.GetImageWidth(), m_PngNumE.GetImageHeight());
	m_PngNumE.SetPictureStaticClick(FALSE);
	m_PngNumE.SetPictureStaticCursor(FALSE);

	m_PngNumBG.IsBlueStacksShow(false);
	m_PngNumBG.SetImage(MAKEINTRESOURCE(IDB_PNG_BG_TOP), _T("PNG"), AfxGetApp()->m_hInstance);
	m_PngNumBG.MoveWindow(0, 0, m_PngNumBG.GetImageWidth(), m_PngNumBG.GetImageHeight());
	m_PngNumBG.SetPictureStaticClick(FALSE);
	m_PngNumBG.SetPictureStaticCursor(FALSE);


	m_pSprite = Common_Util::Load(MAKEINTRESOURCE(IDB_PNG_SPRITE), _T("PNG"), AfxGetApp()->m_hInstance);
	m_TotalIndex = TotalCount;
}

void BottomDlg::DrawSkin()
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

	if (m_nCount > 0)
	{
		DrawSprite(gps);
	}		

	if (m_PngNumF.IsAvailableDraw())
		m_PngNumF.OnDrawLayerdWindow(gps);

	if (m_PngNumE.IsAvailableDraw())
		m_PngNumE.OnDrawLayerdWindow(gps);

	if (m_PngNumBG.IsAvailableDraw())
		m_PngNumBG.OnDrawLayerdWindow(gps);

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

void BottomDlg::DrawSprite(Graphics& gps)
{
	if (m_pSprite)
	{
		gps.SetSmoothingMode(SmoothingModeHighQuality);
		gps.SetInterpolationMode(InterpolationModeHighQualityBicubic);

		int nImgWidth = SpritePoint[m_curIndex].widht;
		int nImgHeight = SpritePoint[m_curIndex].height;
		int x = SpritePoint[m_curIndex].x;
		int y = SpritePoint[m_curIndex].y;

		
		CRect rc;
		m_PngNumBG.GetClientRect(&rc);
		Rect wndRect((rc.right - nImgWidth), ((rc.bottom - nImgHeight) / 2), nImgWidth, nImgHeight);

		//image size
		Rect ImgRect(x, y, nImgWidth, nImgWidth);
		gps.DrawImage(m_pSprite, wndRect, ImgRect.X, ImgRect.Y, ImgRect.Width, ImgRect.Height, UnitPixel);
	}
}

BOOL BottomDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void BottomDlg::OnDestroy()
{
	CDialog::OnDestroy();

	KillTimer(1);

	SAFE_DELETE(m_pSprite);

	SAFE_DELETE(m_pbgImage);
}

void BottomDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin();
}

void BottomDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonUp(nFlags, point);
}

void BottomDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		m_curIndex = (m_curIndex + 1) % m_TotalIndex;

		//CString strMsg;
		//strMsg.Format(_T("m_curIndex:%d\n"), m_curIndex);
		//OutputDebugString(strMsg);

		if (m_nCount > 0)
			--m_nCount;

		if (m_nCount == 0)
		{
			KillTimer(1);
		}

		if (m_nCount > 0)
		{
			m_PngNumF.IsBlueStacksShow(true);
			m_PngNumF.SetImage(MAKEINTRESOURCE(PngNum[m_nCount].uNum1), _T("PNG"), AfxGetApp()->m_hInstance);
			m_PngNumF.MoveWindow(27, 20, m_PngNumF.GetImageWidth(), m_PngNumF.GetImageHeight());

			m_PngNumE.IsBlueStacksShow(true);
			m_PngNumE.SetImage(MAKEINTRESOURCE(PngNum[m_nCount].uNum2), _T("PNG"), AfxGetApp()->m_hInstance);
			m_PngNumE.MoveWindow(42, 19, m_PngNumE.GetImageWidth(), m_PngNumE.GetImageHeight());

			m_PngNumBG.IsBlueStacksShow(true);
			m_PngNumBG.SetImage(MAKEINTRESOURCE(IDB_PNG_BG_TOP), _T("PNG"), AfxGetApp()->m_hInstance);
			m_PngNumBG.MoveWindow(0, 0, m_PngNumBG.GetImageWidth(), m_PngNumBG.GetImageHeight());
		}
		else
		{
			m_PngNumF.IsBlueStacksShow(false);
			m_PngNumF.MoveWindow(27, 20, m_PngNumF.GetImageWidth(), m_PngNumF.GetImageHeight());

			m_PngNumE.IsBlueStacksShow(false);
			m_PngNumE.MoveWindow(42, 19, m_PngNumE.GetImageWidth(), m_PngNumE.GetImageHeight());

			m_PngNumBG.IsBlueStacksShow(false);
			m_PngNumBG.MoveWindow(0, 0, m_PngNumBG.GetImageWidth(), m_PngNumBG.GetImageHeight());
		}
		RedrawWindow();
	}

	CDialog::OnTimer(nIDEvent);
}

void BottomDlg::SetParent(CMsWebBrowsertestDlg* pParent)
{
	m_pParent = pParent;
}

int BottomDlg::GetWidth()
{
	if (m_pbgImage)
	{
		int w = m_pbgImage->m_pBitmap->GetWidth();
		return m_pbgImage->m_pBitmap->GetWidth();
	}
	return 0;
}

int BottomDlg::GetHeight()
{
	if (m_pbgImage)
	{
		int h = m_pbgImage->m_pBitmap->GetHeight();
		return m_pbgImage->m_pBitmap->GetHeight();
	}
	return 0;
}

void BottomDlg::SetStartTimer(int nCount)
{
	m_nCount = nCount;

	if (m_nCount == 0)
	{
		m_PngNumF.IsBlueStacksShow(false);
		m_PngNumF.MoveWindow(27, 20, m_PngNumF.GetImageWidth(), m_PngNumF.GetImageHeight());
		
		m_PngNumE.IsBlueStacksShow(false);
		m_PngNumE.MoveWindow(42, 19, m_PngNumE.GetImageWidth(), m_PngNumE.GetImageHeight());
		
		m_PngNumBG.IsBlueStacksShow(false);
		m_PngNumBG.MoveWindow(0, 0, m_PngNumBG.GetImageWidth(), m_PngNumBG.GetImageHeight());
	}
	else
	{
		m_PngNumF.IsBlueStacksShow(true);
		m_PngNumF.SetImage(MAKEINTRESOURCE(PngNum[m_nCount].uNum1), _T("PNG"), AfxGetApp()->m_hInstance);
		m_PngNumF.MoveWindow(27, 20, m_PngNumF.GetImageWidth(), m_PngNumF.GetImageHeight());

		m_PngNumE.IsBlueStacksShow(true);
		m_PngNumE.SetImage(MAKEINTRESOURCE(PngNum[m_nCount].uNum2), _T("PNG"), AfxGetApp()->m_hInstance);
		m_PngNumE.MoveWindow(42, 19, m_PngNumE.GetImageWidth(), m_PngNumE.GetImageHeight());

		m_PngNumBG.IsBlueStacksShow(true);
		m_PngNumBG.SetImage(MAKEINTRESOURCE(IDB_PNG_BG_TOP), _T("PNG"), AfxGetApp()->m_hInstance);
		m_PngNumBG.MoveWindow(0, 0, m_PngNumBG.GetImageWidth(), m_PngNumBG.GetImageHeight());
	}

	SetTimer(1, 1000, NULL);
}