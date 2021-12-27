#include "pch.h"
#include "ProgressbarStatic.h"

ProgressbarStatic::ProgressbarStatic()
{
	m_nMin = 0;
	m_nMax = 100;
	m_nPos = 0;
}

ProgressbarStatic::~ProgressbarStatic()
{
}

BEGIN_MESSAGE_MAP(ProgressbarStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void ProgressbarStatic::PreSubclassWindow()
{
	ModifyStyle(0, SS_OWNERDRAW | SS_NOTIFY);

	CStatic::PreSubclassWindow();
}

void ProgressbarStatic::SetRange(int nMin, int nMax)
{
	if (nMin <= nMax)
	{
		m_nMin = nMin;
		m_nMax = nMax;
	}
}

int ProgressbarStatic::SetPos(int nPos)
{
	if (m_nMin <= nPos && nPos <= m_nMax)
	{
		m_nPos = nPos;
		Invalidate();
	}

	return m_nPos;
}

void ProgressbarStatic::OnPaint()
{
	CPaintDC dc(this);

	Graphics graphics(dc.GetSafeHdc());

	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	CRect rc;
	GetClientRect(&rc);

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

void ProgressbarStatic::OnDrawLayerdWindow(Graphics& Gps)
{
	USES_CONVERSION;

	Gps.SetSmoothingMode(SmoothingModeHighQuality);
	Gps.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	CRect rc;
	GetWindowRect(rc);
	GetOwner()->ScreenToClient(rc);

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

void ProgressbarStatic::DrawLeft(Graphics& Gps, CRect rc)
{
	RectF destRect(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));

	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 0, 0));
	Gps.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gps.FillRectangle(&brush, destRect);
}

void ProgressbarStatic::DrawRight(Graphics& Gps, CRect rc)
{
	RectF destRect(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));

	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));
	Gps.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gps.FillRectangle(&brush, destRect);
}

void ProgressbarStatic::DrawPercent(Graphics& Gps, CRect rc, CString strPercent)
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

void ProgressbarStatic::DrawBorder(Graphics& Gps, CRect rc)
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

void ProgressbarStatic::DrawBorderNormal(Graphics& Gps, CRect rc)
{
	RectF destRect(REAL(rc.left), REAL(rc.top), REAL(rc.Width() - 1), REAL(rc.Height() -1));

	//사각형 라인
	Pen pen(Color(255, 0, 0, 0), 1);
	pen.SetDashStyle(Gdiplus::DashStyle::DashStyleDash);
	Gps.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gps.DrawRectangle(&pen, destRect);

	//색을 채운 사각형
	//Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0));
	//Gps.FillRectangle(&brush, destRect);
}