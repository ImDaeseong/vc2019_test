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

	Gdiplus::Font font(L"Tahoma", m_nFontSize, FontStyleBold, UnitPixel);	
	SolidBrush brush(Color(92, 92, 92));

	StringFormat stringFormat(StringFormatFlagsDisplayFormatControl);
	stringFormat.SetAlignment(StringAlignmentCenter);//stringFormat.SetAlignment(StringAlignmentNear);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);//stringFormat.SetLineAlignment(StringAlignmentNear);
	RectF rectF(0, 0, (float)rect.Width(), (float)rect.Height());
	
	graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	graphics.DrawString(m_strMessage, -1, &font, rectF, &stringFormat, &brush);


	//��� ����
	RectF destRect(0, 0, (float)rect.Width() - 2, (float)rect.Height() - 2);

	//��� ���� ó��
	/* 
	SolidBrush bgbrush(Color(255, 0, 0));
	graphics.FillRectangle(&bgbrush, destRect);
	*/

	//��� ���� ó��
	///*
	Pen pen(Color(255, 0, 0, 0), 1);
	pen.SetDashStyle(Gdiplus::DashStyle::DashStyleDash);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	graphics.DrawRectangle(&pen, destRect);
	//*/
}

void CMsgStatic::SetMesssageText(CString sMessage)
{
	m_strMessage = sMessage;
}

void CMsgStatic::SetMesssageFontSize(int nFontSize)
{
	m_nFontSize = nFontSize;
}