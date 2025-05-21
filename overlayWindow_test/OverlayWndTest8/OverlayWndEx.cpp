#include "pch.h"
#include "OverlayWndEx.h"
#include "resource.h"

COverlayWndEx::COverlayWndEx()
{
    m_strText1 = "";
    m_strText2 = "";

    m_strFont1 = _T("����");
    m_strFont2 = _T("����");

    m_textcolor1 = Color(255, 0, 0, 0);
    m_textcolor2 = Color(255, 0, 0, 0);

    m_pBitmap = NULL;
    m_hBuffer = NULL;

    m_Alpha = 255;

    //���� �� �׵θ� ����
    m_bgColor = Color(255, 0, 0, 0);
    m_borderColor = Color(255, 255, 0, 0);
    m_nBorder = 5;
    m_fRadius = 10.0f;
    m_nRound = FALSE;
}

COverlayWndEx::~COverlayWndEx()
{
    if (m_pBitmap)
    {
        delete m_pBitmap;
        m_pBitmap = NULL;
    }

    if (m_hBuffer)
    {
        ::GlobalFree(m_hBuffer);
        m_hBuffer = NULL;
    }
}

bool COverlayWndEx::LoadFromResource(LPCTSTR pName, LPCTSTR pType)
{
    if (m_pBitmap)
    {
        delete m_pBitmap;
        m_pBitmap = NULL;
    }

    if (m_hBuffer)
    {
        ::GlobalFree(m_hBuffer);
        m_hBuffer = NULL;
    }

    HMODULE hInst = AfxGetInstanceHandle();

    HRSRC hResource = ::FindResource(hInst, pName, pType);
    if (!hResource) return false;

    DWORD imageSize = ::SizeofResource(hInst, hResource);
    if (!imageSize) return false;

    const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
    if (!pResourceData) return false;

    m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
    if (!m_hBuffer) return false;

    void* pBuffer = ::GlobalLock(m_hBuffer);
    if (!pBuffer) return false;

    CopyMemory(pBuffer, pResourceData, imageSize);
    ::GlobalUnlock(m_hBuffer);

    IStream* pStream = NULL;
    if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
    {
        m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
        pStream->Release();

        if (m_pBitmap && m_pBitmap->GetLastStatus() == Gdiplus::Ok)
            return true;

        delete m_pBitmap;
        m_pBitmap = NULL;
    }

    ::GlobalFree(m_hBuffer);
    m_hBuffer = NULL;

    return false;
}

void COverlayWndEx::setDrawText(CString strText1, CString strText2)
{
    m_strText1 = strText1;
    m_strText2 = strText2;
}
void COverlayWndEx::setDrawFont(CString strFont1, CString strFont2)
{
    m_strFont1 = strFont1;
    m_strFont2 = strFont2;
}

void COverlayWndEx::setDrawColor(Gdiplus::Color textcolor1, Gdiplus::Color textcolor2)
{
    m_textcolor1 = textcolor1;
    m_textcolor2 = textcolor2;
}

void COverlayWndEx::setDrawColor(COLORREF color1, COLORREF color2)
{
    m_textcolor1 = Gdiplus::Color::MakeARGB(255, GetRValue(color1), GetGValue(color1), GetBValue(color1));
    m_textcolor2 = Gdiplus::Color::MakeARGB(255, GetRValue(color2), GetGValue(color2), GetBValue(color2));
}

void COverlayWndEx::setDrawBg()
{
    LoadFromResource(MAKEINTRESOURCE(IDB_PNG1), _T("PNG"));
}

void COverlayWndEx::SetWindowTransparency(BYTE alpha)
{
    m_Alpha = alpha;
    UpdateDrawText();
}

BEGIN_MESSAGE_MAP(COverlayWndEx, CWnd)
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void COverlayWndEx::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    if (nType != SIZE_MINIMIZED)
        UpdateDrawText();
}

BOOL COverlayWndEx::OnEraseBkgnd(CDC* pDC)
{
    // ��� �׸��� ����
    return TRUE;
}

void COverlayWndEx::DrawRoundedRect(Graphics& graphics, const RectF& fRect, float radius)
{
    GraphicsPath path;
    path.AddArc(fRect.X, fRect.Y, radius * 2, radius * 2, 180, 90);
    path.AddArc(fRect.X + fRect.Width - radius * 2, fRect.Y, radius * 2, radius * 2, 270, 90);
    path.AddArc(fRect.X + fRect.Width - radius * 2, fRect.Y + fRect.Height - radius * 2, radius * 2, radius * 2, 0, 90);
    path.AddArc(fRect.X, fRect.Y + fRect.Height - radius * 2, radius * 2, radius * 2, 90, 90);
    path.CloseFigure();

    SolidBrush brush(m_bgColor);
    Pen pen(m_borderColor, (REAL)m_nBorder);

    graphics.FillPath(&brush, &path);
    graphics.DrawPath(&pen, &path);
}

void COverlayWndEx::DrawRect(Graphics& graphics, const RectF& rect)
{
    SolidBrush brush(m_bgColor);
    Pen pen(m_borderColor, (REAL)m_nBorder);

    graphics.FillRectangle(&brush, rect);
    graphics.DrawRectangle(&pen, rect.X, rect.Y, rect.Width, rect.Height);
}

void COverlayWndEx::UpdateDrawText()
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

    if (m_nRound)
    {
        //���� �׵θ�
        DrawRoundedRect(graphics, Gdiplus::RectF(1, 1, rClient.Width() - 2, rClient.Height() - 2), m_fRadius);
    }
    else
    {
        //�簢�� �׵θ�
        DrawRect(graphics, Gdiplus::RectF(1, 1, rClient.Width() - 2, rClient.Height() - 2));
    }
    
    //�׵θ� ���� ���
    /*
    //�ؽ�Ʈ ��ġ
    RectF layoutRect1((REAL)rClient.left, (REAL)rClient.top, (REAL)rClient.Width(), (REAL)(rClient.Height() / 2));
    RectF layoutRect2((REAL)rClient.left, (REAL)(rClient.top + rClient.Height() / 2), (REAL)rClient.Width(), (REAL)(rClient.Height() / 2));
    */

    //�׵θ� ����
    float offset = static_cast<float>(m_nBorder);

    //��ü �������� �¿�/���� �׵θ�(offset)��ŭ ������ �ؽ�Ʈ ���� ũ�� ���
    float totalWidth = rClient.Width() - (offset * 2);
    float totalHeight = rClient.Height() - (offset * 2);

    //�ؽ�Ʈ ���� ����: ��� 60%, �ϴ� 40%
    float topHeight = totalHeight * 0.6f;
    float bottomHeight = totalHeight - topHeight;

    //��� �ؽ�Ʈ ���̾ƿ� ����
    RectF layoutRect1(offset, offset, totalWidth, topHeight);

    //�ϴ� �ؽ�Ʈ ���̾ƿ� ����
    RectF layoutRect2(offset, offset + topHeight, totalWidth, bottomHeight);


    DrawText(graphics, m_strText1, m_strFont1, 40, FontStyleBold,
        layoutRect1, m_textcolor1, Color(0, 0, 0, 0));

    DrawText(graphics, m_strText2, m_strFont1, 24, FontStyleRegular,
        layoutRect2, m_textcolor2, Color(0, 0, 0, 0));


    //Alpha ���� ó��
    POINT ptSrc = { 0, 0 };
    SIZE sizeWnd = { rClient.Width(), rClient.Height() };
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, m_Alpha, AC_SRC_ALPHA };

    ::UpdateLayeredWindow(GetSafeHwnd(), dcScreen.GetSafeHdc(), NULL, &sizeWnd,
        memDC.GetSafeHdc(), &ptSrc, 0, &blend, ULW_ALPHA);

    memDC.SelectObject(pOldBmp);
    ::DeleteObject(bmp.Detach());
}

void COverlayWndEx::DrawText(Graphics& graphics,
    CString strText,
    CString strFontName,
    REAL fontSize,
    INT fontStyle,
    RectF fRect,
    Color textcolor,
    Gdiplus::Color bgcolor)
{
    //USES_CONVERSION;

    //����
    SolidBrush backgroundBrush(bgcolor);
    graphics.FillRectangle(&backgroundBrush, fRect);

    Gdiplus::FontFamily fontFamily(strFontName);
    Gdiplus::Font font(&fontFamily, fontSize, fontStyle, UnitPixel);


    RectF textRect;
    graphics.MeasureString(strText, -1, &font, fRect, NULL, &textRect);

    float fCenterX = fRect.X + (fRect.Width - textRect.Width) / 2;
    float fCenterY = fRect.Y + (fRect.Height - textRect.Height) / 2;
    RectF drawRect(fCenterX, fCenterY, textRect.Width, textRect.Height);

    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentNear);

    Gdiplus::SolidBrush brush(textcolor);
    graphics.DrawString(strText, -1, &font, drawRect, &format, &brush);
}
