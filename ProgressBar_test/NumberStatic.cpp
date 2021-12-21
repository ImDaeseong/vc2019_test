#include "pch.h"
#include "NumberStatic.h"

NumberStatic::NumberStatic()
{
	m_pImage = NULL;
	m_nDivide = 1;
	m_bClick = FALSE;
	m_bCursor = FALSE;
	m_bShow = TRUE;
}

NumberStatic::~NumberStatic()
{
	if (m_pImage != NULL)
		delete m_pImage;
	m_pImage = NULL;
}

BEGIN_MESSAGE_MAP(NumberStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

NumberStatic& NumberStatic::SetPictureStaticClick(BOOL bClick)
{
	m_bClick = bClick;

	if (bClick)
		ModifyStyle(0, SS_NOTIFY);
	else
		ModifyStyle(SS_NOTIFY, 0);

	return *this;
}

void NumberStatic::SetPictureStaticCursor(BOOL bCursor)
{
	m_bCursor = bCursor;
}

BOOL NumberStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bCursor == TRUE)
	{
		::SetCursor(::LoadCursor(0, MAKEINTRESOURCE(IDC_HAND)));
		return TRUE;
	}
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void NumberStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bClick == TRUE)
	{		
		return;
	}
}

void NumberStatic::SetImagePath(CString strPath, int nDivide)
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

void NumberStatic::SetImage(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
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

void NumberStatic::OnPaint()
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

BOOL NumberStatic::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL NumberStatic::IsAvailableDraw()
{
	if (!m_pImage)
		return FALSE;
	return TRUE;
}

void NumberStatic::OnDrawLayerdWindow(Graphics& Gps)
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
}

int NumberStatic::GetImageWidth() const
{
	int res = 0;
	if (m_pImage != NULL && m_pImage->m_pBitmap != NULL)
	{
		res = m_pImage->m_pBitmap->GetWidth() / m_nDivide;
	}
	return res;
}

int NumberStatic::GetImageHeight() const
{
	int res = 0;
	if (m_pImage != NULL && m_pImage->m_pBitmap != NULL)
	{
		res = m_pImage->m_pBitmap->GetHeight();
	}
	return res;
}

void NumberStatic::IsShow(BOOL bShow)
{
	m_bShow = bShow;
}