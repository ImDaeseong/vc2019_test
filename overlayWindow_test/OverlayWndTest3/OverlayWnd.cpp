#include "pch.h"
#include "OverlayWnd.h"

COverlayWnd::COverlayWnd()
{
    m_strText = "";

    m_strFont = L"����";

    m_textcolor = Color(255, 0, 0, 0);
}

void COverlayWnd::setDrawText(CString strText)
{
    m_strText = strText;
}
void COverlayWnd::setDrawFont(CString strFont)
{
    m_strFont = strFont;
}

void COverlayWnd::setDrawColor(Gdiplus::Color textcolor)
{
    m_textcolor = textcolor;
}

void COverlayWnd::setDrawColor(COLORREF color)
{
    m_textcolor = Gdiplus::Color::MakeARGB(255, GetRValue(color), GetGValue(color), GetBValue(color));
}

COverlayWnd::~COverlayWnd()
{
}

BEGIN_MESSAGE_MAP(COverlayWnd, CWnd)
    ON_WM_SIZE()
END_MESSAGE_MAP()

void COverlayWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    if (nType != SIZE_MINIMIZED)
        UpdateDrawText();
}

void COverlayWnd::UpdateDrawText()
{
    CRect rClient;
    GetClientRect(&rClient);

    //Create screen DC
    CClientDC dcScreen(NULL);

    //Memory DC ����
    CDC memDC;
    if (!memDC.CreateCompatibleDC(&dcScreen)) return;

    // Alpha ä�� ���Ե� DIBSection ��Ʈ�� ����
    BITMAPINFO bmpInfo = { 0 };
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = rClient.Width();
    bmpInfo.bmiHeader.biHeight = -rClient.Height(); // Top-down
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    void* pBits = NULL;
    HBITMAP hBmp = CreateDIBSection(dcScreen, &bmpInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
    if (!hBmp) return;

    CBitmap bmp;
    bmp.Attach(hBmp);
    CBitmap* pOldBmp = memDC.SelectObject(&bmp);

    //GDI+ �׷��� ��ü ����
    Graphics graphics(memDC);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

    //���� ���
    graphics.Clear(Color(0, 0, 0, 0));

    //������ ���
    //graphics.Clear(Color(255, 0, 0, 0));

    //�ؽ�Ʈ
    DrawText(graphics, m_strText, m_strFont, 40, FontStyleBold,
        RectF(0.0f, 0.0f, 460.0f, 40.0f), StringAlignmentFar,
        m_textcolor, Color(0, 0, 0, 0));


    //Alpha ���� ó��
    POINT ptSrc = { 0, 0 };
    SIZE sizeWnd = { rClient.Width(), rClient.Height() };
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    ::UpdateLayeredWindow(GetSafeHwnd(), dcScreen.GetSafeHdc(), NULL, &sizeWnd,
        memDC.GetSafeHdc(), &ptSrc, 0, &blend, ULW_ALPHA);

    memDC.SelectObject(pOldBmp);
    ::DeleteObject(bmp.Detach());
}

void COverlayWnd::DrawText(Graphics& graphics,
    CString strText,
    CString strFontName,
    REAL fontSize,
    INT fontStyle,
    RectF fRect,
    StringAlignment alignment,
    Color textcolor,
    Gdiplus::Color bgcolor)
{
    //USES_CONVERSION;

    //����
    //SolidBrush backgroundBrush(bgcolor);
    //graphics.FillRectangle(&backgroundBrush, fRect);

    Gdiplus::FontFamily fontFamily(strFontName);
    Gdiplus::Font font(&fontFamily, fontSize, fontStyle, UnitPixel);

    Gdiplus::StringFormat format;
    format.SetAlignment(alignment);
    format.SetLineAlignment(StringAlignmentNear); //���� ������ ��� ����

    Gdiplus::SolidBrush brush(textcolor);
    graphics.DrawString(strText, -1, &font, fRect, &format, &brush);
}
