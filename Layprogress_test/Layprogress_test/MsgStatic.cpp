#include "pch.h"
#include "MsgStatic.h"

IMPLEMENT_DYNAMIC(CMsgStatic, CStatic)

CMsgStatic::CMsgStatic() : m_nFontSize(13)
{
}

CMsgStatic::~CMsgStatic()
{	
}

BEGIN_MESSAGE_MAP(CMsgStatic, CStatic)
	ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CMsgStatic::OnEraseBkgnd(CDC* pDC)
{
    // 깜빡임 방지용 배경 지우기 생략
    return TRUE;
}

void CMsgStatic::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    CDC memDC;
    if (!memDC.CreateCompatibleDC(&dc))
        return;

    CBitmap memBitmap;
    if (!memBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height()))
        return;

    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);
    if (!pOldBitmap)
        return;

    Gdiplus::Graphics graphics(memDC.GetSafeHdc());

    // 고품질 텍스트 렌더링 설정
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);

    //배경으로 클리어
    graphics.Clear(Gdiplus::Color(255, 72, 72, 72));

    Gdiplus::RectF rectF(0, 0, (REAL)rect.Width(), (REAL)rect.Height());

    //영역 확인시
    /*
    SolidBrush bgBrush(Color(255, 255, 0, 0));
    graphics.FillRectangle(&bgBrush, rectF);
    */
    
    //폰트 설정
    Gdiplus::Font font(L"맑은 고딕", (REAL)m_nFontSize, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));

    Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsDisplayFormatControl);
    format.SetAlignment(Gdiplus::StringAlignmentCenter);
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    graphics.DrawString(m_strMessage, -1, &font, rectF, &format, &brush);

    dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOldBitmap);
}

void CMsgStatic::OnDrawLayerdWindow(Graphics& Gps)
{
    CRect rect;
    GetWindowRect(&rect);
    GetParent()->ScreenToClient(&rect);

    Gdiplus::RectF rectF((REAL)rect.left, (REAL)rect.top, (REAL)rect.Width(), (REAL)rect.Height());

    //영역 확인시
    /*
    SolidBrush bgBrush(Color(255, 255, 0, 0));
    Gps.FillRectangle(&bgBrush, rectF);
    */
    
    //고품질 텍스트 렌더링 설정
    Gps.SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
    Gps.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    Gps.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);

    //폰트 설정
    Gdiplus::Font font(L"맑은 고딕", (REAL)m_nFontSize, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));

    Gdiplus::StringFormat stringFormat(Gdiplus::StringFormatFlagsDisplayFormatControl);
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    Gps.DrawString(m_strMessage, -1, &font, rectF, &stringFormat, &brush);    
}

void CMsgStatic::SetMesssageText(CString sMessage)
{
	m_strMessage = sMessage;
	if (GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CMsgStatic::SetMesssageFontSize(int nFontSize)
{
	m_nFontSize = nFontSize;
	if (GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}