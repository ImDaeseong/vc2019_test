#include "pch.h"
#include "ProgressbarPngStatic.h"

ProgressbarPngStatic::ProgressbarPngStatic()
{
	m_pImgLeft = NULL;
	m_pImgRight = NULL;

	m_nMin = 0;
	m_nMax = 100;
	m_nPos = 0;

	m_bGradient = FALSE;
	m_crGrBG1 = RGB(255, 0, 0);
	m_crGrBG2 = RGB(255, 255, 255);
}

ProgressbarPngStatic::~ProgressbarPngStatic()
{
	if (m_pImgLeft != NULL)
		delete m_pImgLeft;
	m_pImgLeft = NULL;

	if (m_pImgRight != NULL)
		delete m_pImgRight;
	m_pImgRight = NULL;
}

BEGIN_MESSAGE_MAP(ProgressbarPngStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void ProgressbarPngStatic::PreSubclassWindow()
{
	ModifyStyle(0, SS_OWNERDRAW | SS_NOTIFY);

	CStatic::PreSubclassWindow();
}

void ProgressbarPngStatic::SetImage(LPCTSTR pImgLeft, LPCTSTR pImgRight, LPCTSTR pType, HMODULE hInst)
{
	if (m_pImgLeft == NULL)
		m_pImgLeft = new CGdiPlusBitmapResource;

	if (!m_pImgLeft->Load(pImgLeft, pType, hInst))
	{
		delete m_pImgLeft;
		m_pImgLeft = NULL;
		return;
	}

	if (m_pImgRight == NULL)
		m_pImgRight = new CGdiPlusBitmapResource;

	if (!m_pImgRight->Load(pImgRight, pType, hInst))
	{
		delete m_pImgRight;
		m_pImgRight = NULL;
		return;
	}

	InvalidateRect(NULL);
}

void ProgressbarPngStatic::SetNoImage()
{
	CRect rc;
	GetClientRect(&rc);

	Gdiplus::Bitmap* bmp1 = new Gdiplus::Bitmap(rc.Width(), rc.Height(), PixelFormat32bppARGB);
	Gdiplus::Bitmap* bmp2 = new Gdiplus::Bitmap(rc.Width(), rc.Height(), PixelFormat32bppARGB);

	Graphics graph1(bmp1);

	if (m_bGradient)
	{
		LinearGradientBrush	brush1(Rect(0, 0, rc.Width(), rc.Height()), Color(255, GetRValue(m_crGrBG1), GetGValue(m_crGrBG1), GetBValue(m_crGrBG1)), Color(255, GetRValue(m_crGrBG2), GetGValue(m_crGrBG2), GetBValue(m_crGrBG2)), LinearGradientModeVertical);
		graph1.FillRectangle(&brush1, 0, 0, bmp1->GetWidth(), bmp1->GetHeight());
	}
	else
	{
		Gdiplus::SolidBrush brush1(Gdiplus::Color(255, 255, 0, 0));
		graph1.FillRectangle(&brush1, 0, 0, bmp1->GetWidth(), bmp1->GetHeight());
	}
	
	Graphics graph2(bmp2);
	Gdiplus::SolidBrush brush2(Gdiplus::Color(255, 255, 255, 255));
	graph2.FillRectangle(&brush2, 0, 0, bmp2->GetWidth(), bmp2->GetHeight());


	if (m_pImgLeft == NULL)
		m_pImgLeft = new CGdiPlusBitmapResource;

	if (!m_pImgLeft->LoadBmp(bmp1))
	{
		delete m_pImgLeft;
		m_pImgLeft = NULL;
		return;
	}

	if (m_pImgRight == NULL)
		m_pImgRight = new CGdiPlusBitmapResource;

	if (!m_pImgRight->LoadBmp(bmp2))
	{
		delete m_pImgRight;
		m_pImgRight = NULL;
		return;
	}

	InvalidateRect(NULL);
}

void ProgressbarPngStatic::SetRange(int nMin, int nMax)
{
	if (nMin <= nMax)
	{
		m_nMin = nMin;
		m_nMax = nMax;
	}
}

int ProgressbarPngStatic::SetPos(int nPos)
{
	if (m_nMin <= nPos && nPos <= m_nMax)
	{
		m_nPos = nPos;
		Invalidate();
	}

	return m_nPos;
}

BOOL ProgressbarPngStatic::IsAvailableDraw()
{
	if (m_pImgLeft == NULL || m_pImgRight == NULL)
		return FALSE;
	return TRUE;
}

void ProgressbarPngStatic::OnPaint()
{
	if (m_pImgLeft == NULL || m_pImgRight == NULL) return;

	CPaintDC dc(this);

	Graphics graphics(dc.GetSafeHdc());
	
	CRect rc;
	GetClientRect(&rc);

	Color bgColor(Gdiplus::Color(255, 255, 255, 255));
	graphics.Clear(bgColor);

	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);


	double Fraction = (double)(m_nPos - m_nMin) / ((double)(m_nMax - m_nMin));

	CRect rcLeft, rcRight;
	rcLeft = rcRight = rc;

	//왼쪽 사이즈
	rcLeft.right = rcLeft.left + (int)((rcLeft.right - rcLeft.left) * Fraction);

	//오른쪽 사이즈
	rcRight.left = rcLeft.right;


	DrawLeft(graphics, rcLeft);

	DrawRight(graphics, rcRight);


	//현재 진행 퍼센트
	CString strPercent;
	strPercent.Format(_T("%d%%"), (int)(Fraction * 100.0));
	DrawPercent(graphics, rc, strPercent);

	//영역 라인
	DrawBorderNormal(graphics, rc);
}

void ProgressbarPngStatic::OnDrawLayerdWindow(Graphics& Gps)
{
	if (m_pImgLeft == NULL || m_pImgRight == NULL) return;

	USES_CONVERSION;

	CRect rc;
	GetWindowRect(rc);
	GetOwner()->ScreenToClient(rc);

	//Color bgColor(Gdiplus::Color(255, 255, 255, 255));
	//Gps.Clear(bgColor);

	Gps.SetSmoothingMode(SmoothingModeHighQuality);
	Gps.SetInterpolationMode(InterpolationModeHighQualityBicubic);


	double Fraction = (double)(m_nPos - m_nMin) / ((double)(m_nMax - m_nMin));

	CRect rcLeft, rcRight;
	rcLeft = rcRight = rc;

	//왼쪽 사이즈
	rcLeft.right = rcLeft.left + (int)((rcLeft.right - rcLeft.left) * Fraction);

	//오른쪽 사이즈
	rcRight.left = rcLeft.right;


	DrawLeft(Gps, rcLeft);

	DrawRight(Gps, rcRight);

	//현재 진행 퍼센트
	CString strPercent;
	strPercent.Format(_T("%d%%"), (int)(Fraction * 100.0));
	DrawPercent(Gps, rc, strPercent);

	//영역 라인
	DrawBorder(Gps, rc);
}

void ProgressbarPngStatic::DrawLeft(Graphics& Gps, CRect rc)
{
	RectF destRect(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));

	Unit units;
	RectF srcRect;
	m_pImgLeft->m_pBitmap->GetBounds(&srcRect, &units);
	Gps.DrawImage(m_pImgLeft->m_pBitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel, NULL);
}

void ProgressbarPngStatic::DrawRight(Graphics& Gps, CRect rc)
{
	RectF destRect(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));

	Unit units;
	RectF srcRect;
	m_pImgRight->m_pBitmap->GetBounds(&srcRect, &units);
	Gps.DrawImage(m_pImgRight->m_pBitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel, NULL);
}

void ProgressbarPngStatic::DrawPercent(Graphics& Gps, CRect rc, CString strPercent)
{
	RectF destRect(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));

	//퍼센트 글자
	Gdiplus::Font font(L"Tahoma", 12, FontStyle::FontStyleRegular, UnitPixel);
	SolidBrush brush(Color(0, 0, 0));
	StringFormat stringFormat(StringFormatFlagsDisplayFormatControl);
	stringFormat.SetAlignment(StringAlignmentCenter);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	Gps.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	Gps.DrawString(strPercent, -1, &font, destRect, &stringFormat, &brush);
}

void ProgressbarPngStatic::DrawBorder(Graphics& Gps, CRect rc)
{
	RectF destRect(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));
		
	//사각형 라인
	Pen pen(Color(255, 0, 0, 0), 1);
	pen.SetDashStyle(Gdiplus::DashStyle::DashStyleDash);
	Gps.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gps.DrawRectangle(&pen, destRect);

	//색을 채운 사각형
	//Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0));
	//Gps.FillRectangle(&brush, destRect);
}

void ProgressbarPngStatic::DrawBorderNormal(Graphics& Gps, CRect rc)
{
	RectF destRect(REAL(rc.left), REAL(rc.top), REAL(rc.Width() - 1), REAL(rc.Height() - 1));

	//사각형 라인
	Pen pen(Color(255, 0, 0, 0), 1);
	pen.SetDashStyle(Gdiplus::DashStyle::DashStyleDash);
	Gps.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gps.DrawRectangle(&pen, destRect);

	//색을 채운 사각형
	//Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0));
	//Gps.FillRectangle(&brush, destRect);
}