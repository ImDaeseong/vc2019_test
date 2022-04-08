#include "pch.h"
#include "PngStatic.h"
#include "MsWebBrowser_testDlg.h"

CPngStatic::CPngStatic()
{
	m_pImage = NULL;
	m_nDivide = 1;
	m_bClick = FALSE;
	m_bCursor = FALSE;
	m_bShow = TRUE;
}

CPngStatic::~CPngStatic()
{
	if (m_pImage != NULL)
		delete m_pImage;
	m_pImage = NULL;
}

BEGIN_MESSAGE_MAP(CPngStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

CPngStatic& CPngStatic::SetPictureStaticClick(BOOL bClick)
{
	m_bClick = bClick;

	if (bClick)
		ModifyStyle(0, SS_NOTIFY);
	else
		ModifyStyle(SS_NOTIFY, 0);

	return *this;
}

void CPngStatic::SetPictureStaticCursor(BOOL bCursor)
{
	m_bCursor = bCursor;
}

BOOL CPngStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bCursor == TRUE)
	{
		::SetCursor(::LoadCursor(0, MAKEINTRESOURCE(IDC_HAND)));
		return TRUE;
	}
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CPngStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bClick == TRUE)
	{
		CMsWebBrowsertestDlg* pMain = (CMsWebBrowsertestDlg*)AfxGetMainWnd();
		pMain->CallMain();
	}
}

void CPngStatic::SetImagePath(CString strPath, int nDivide)
{
	if (m_pImage == NULL)
		m_pImage = new CGdiPlusBitmapResource;

	if (!m_pImage->LoadPath(strPath))
	{
		delete m_pImage;
		m_pImage = NULL;
		return;
	}

	m_nDivide = nDivide;

	InvalidateRect(NULL);
}

void CPngStatic::SetImage(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
{
	if (m_pImage == NULL)
		m_pImage = new CGdiPlusBitmapResource;

	if (!m_pImage->Load(pName, pType, hInst))
	{
		delete m_pImage;
		m_pImage = NULL;
		return;
	}

	InvalidateRect(NULL);
}

void CPngStatic::OnPaint()
{
	CPaintDC dc(this);

	Graphics graphics(dc.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	CRect rect;
	GetClientRect(&rect);
	RectF destRect(REAL(rect.left), REAL(rect.top), REAL(rect.Width()), REAL(rect.Height()));

	if (m_pImage)
	{
		Unit units;
		RectF srcRect;
		m_pImage->m_pBitmap->GetBounds(&srcRect, &units);
		graphics.DrawImage(m_pImage->m_pBitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width / m_nDivide, srcRect.Height, UnitPixel, NULL);
	}
}

BOOL CPngStatic::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CPngStatic::IsAvailableDraw()
{
	if (!m_pImage)
		return FALSE;
	return TRUE;
}

void CPngStatic::OnDrawLayerdWindow(Graphics& Gps)
{
	if (!m_bShow) return;

	if (m_pImage)
	{
		CRect rc;
		GetWindowRect(rc);
		GetOwner()->ScreenToClient(rc);
		RectF destRect(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));

		Unit units;
		RectF srcRect;
		m_pImage->m_pBitmap->GetBounds(&srcRect, &units);
		Gps.DrawImage(m_pImage->m_pBitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel, NULL);
	}

	/*
	CRect rc;
	GetWindowRect(rc);
	GetOwner()->ScreenToClient(rc);
	//배경 설정
	RectF destRect(rc.left, rc.top, (float)rc.Width() , (float)rc.Height());

	//배경 색상 처리
	SolidBrush bgbrush(Color(255, 0, 0));
	Gps.FillRectangle(&bgbrush, destRect);
	*/
}

int CPngStatic::GetImageWidth() const
{
	int res = 0;
	if (m_pImage != NULL && m_pImage->m_pBitmap != NULL)
	{
		res = m_pImage->m_pBitmap->GetWidth() / m_nDivide;
	}
	return res;
}

int CPngStatic::GetImageHeight() const
{
	int res = 0;
	if (m_pImage != NULL && m_pImage->m_pBitmap != NULL)
	{
		res = m_pImage->m_pBitmap->GetHeight();
	}
	return res;
}

void CPngStatic::IsBlueStacksShow(BOOL bShow)
{
	m_bShow = bShow;
}