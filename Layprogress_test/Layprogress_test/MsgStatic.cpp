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
    if (!memDC.CreateCompatibleDC(&dc)) return;

    CBitmap memBitmap;
    if (!memBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height())) return;

    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);
    if (!pOldBitmap) return;

    Gdiplus::Graphics graphics(memDC.GetSafeHdc());
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

    // 배경 색 설정
    graphics.Clear(Gdiplus::Color(255, 72, 72, 72));

    //영역 확인시
    /*
    //SolidBrush bgBrush(Color(255, 255, 0, 0));
    //graphics.FillRectangle(&bgBrush, rectF);
    */

    Gdiplus::RectF rectF(0, 0, (REAL)rect.Width(), (REAL)rect.Height());
    Gdiplus::Font font(L"Tahoma", (REAL)m_nFontSize, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255));

    Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsDisplayFormatControl);
    format.SetAlignment(Gdiplus::StringAlignmentCenter);
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    graphics.DrawString(m_strMessage, -1, &font, rectF, &format, &brush);

    dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

    // 리소스 해제
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

    Gdiplus::Font font(L"Tahoma", (REAL)m_nFontSize, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255));
    
    Gdiplus::StringFormat stringFormat(Gdiplus::StringFormatFlagsDisplayFormatControl);
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    Gps.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
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