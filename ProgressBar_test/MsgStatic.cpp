#include "pch.h"
#include "MsgStatic.h"

IMPLEMENT_DYNAMIC(CMsgStatic, CStatic)

CMsgStatic::CMsgStatic()
{
	m_nFontSize = 13;
}

CMsgStatic::~CMsgStatic()
{	
}

BEGIN_MESSAGE_MAP(CMsgStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CMsgStatic::OnPaint()
{
	USES_CONVERSION;

	CPaintDC dc(this); 

	Gdiplus::Graphics graphics(dc.GetSafeHdc());

	CRect rect;
	GetClientRect(&rect);
	RectF rectF(rect.left, rect.top, (float)rect.Width(), (float)rect.Height());

	Gdiplus::Font font(L"Tahoma", m_nFontSize, FontStyleBold, UnitPixel);	
	SolidBrush brush(Color(92, 92, 92));

	StringFormat stringFormat(StringFormatFlagsDisplayFormatControl);
	stringFormat.SetAlignment(StringAlignmentCenter);//stringFormat.SetAlignment(StringAlignmentNear);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);//stringFormat.SetLineAlignment(StringAlignmentNear);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	graphics.DrawString(m_strMessage, -1, &font, rectF, &stringFormat, &brush);

	//배경 설정
	RectF destRect(rect.left, rect.top, (float)rect.Width() - 2, (float)rect.Height() - 2);

	//배경 색상 처리
	/* 
	SolidBrush bgbrush(Color(255, 0, 0));
	graphics.FillRectangle(&bgbrush, destRect);
	*/

	//배경 라인 처리
	/*
	Pen pen(Color(255, 0, 0, 0), 1);
	pen.SetDashStyle(Gdiplus::DashStyle::DashStyleDash);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	graphics.DrawRectangle(&pen, destRect);
	*/

	//라운드 라인 처리
	//DrawRoundRect::OnDrawRoundRect(graphics, destRect, Gdiplus::Color(0, 0, 0), 4);
	DrawRoundRect::OnDrawRoundRectLine(graphics, destRect, Gdiplus::Color(0, 0, 0), 4);
}

void CMsgStatic::OnDrawLayerdWindow(Graphics& Gps)
{
	CRect rc;
	GetWindowRect(rc);
	GetOwner()->ScreenToClient(rc);	
	RectF rectF(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));

	Gdiplus::Font font(L"Tahoma", m_nFontSize, FontStyleBold, UnitPixel);
	SolidBrush brush(Color(92, 92, 92));

	StringFormat stringFormat(StringFormatFlagsDisplayFormatControl);
	stringFormat.SetAlignment(StringAlignmentCenter);//stringFormat.SetAlignment(StringAlignmentNear);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);//stringFormat.SetLineAlignment(StringAlignmentNear);
	Gps.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	Gps.DrawString(m_strMessage, -1, &font, rectF, &stringFormat, &brush);

	//배경 설정
	RectF destRect(rc.left, rc.top, (float)rc.Width() - 2, (float)rc.Height() - 2);

	//배경 색상 처리
	/*
	SolidBrush bgbrush(Color(255, 0, 0));
	Gps.FillRectangle(&bgbrush, destRect);
	*/

	//배경 라인 처리
	/*
	Pen pen(Color(255, 0, 0, 0), 1);
	pen.SetDashStyle(Gdiplus::DashStyle::DashStyleDash);
	Gps.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gps.DrawRectangle(&pen, destRect);
	*/

	//라운드 라인 처리
	//DrawRoundRect::OnDrawRoundRect(Gps, destRect, Gdiplus::Color(0, 0, 0), 4);
	DrawRoundRect::OnDrawRoundRectLine(Gps, destRect, Gdiplus::Color(0, 0, 0), 4);
}

void CMsgStatic::SetMesssageText(CString sMessage)
{
	m_strMessage = sMessage;
}

void CMsgStatic::SetMesssageFontSize(int nFontSize)
{
	m_nFontSize = nFontSize;
}