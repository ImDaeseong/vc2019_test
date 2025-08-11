#include "pch.h"
#include "RoundLineProgressBar.h"


//------------ CRoundLineProgressBarEx
CRoundLineProgressBarEx::CRoundLineProgressBarEx()
{
    m_nMin = 0;
    m_nMax = 100;
    m_nPos = 0;
    m_barColor = RGB(255, 111, 0);      //��Ȳ��
    m_bgColor = RGB(255, 255, 255);     //��� ���
    m_borderColor = RGB(200, 200, 200); //�׵θ� ����
    m_borderWidth = 1.0f;               //�׵θ� �β�
    m_shadowColor = RGB(128, 128, 128); //�׸��� ����
    m_enableShadow = true;              //�׸��� Ȱ��ȭ
    m_enableGradient = true;            //�׶���Ʈ Ȱ��ȭ
}

CRoundLineProgressBarEx::~CRoundLineProgressBarEx()
{
}

BEGIN_MESSAGE_MAP(CRoundLineProgressBarEx, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CRoundLineProgressBarEx::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CRoundLineProgressBarEx::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    //���ػ� �������� ���� �����ϸ�
    float scale = max(1.0f, dc.GetDeviceCaps(LOGPIXELSX) / 96.0f);
    int scaledWidth = (int)(rect.Width() * scale);
    int scaledHeight = (int)(rect.Height() * scale);

    Bitmap memBitmap(scaledWidth, scaledHeight);
    Graphics memGraphics(&memBitmap);

    //��ǰ�� ������ ����
    memGraphics.SetSmoothingMode(SmoothingModeHighQuality);
    memGraphics.SetCompositingQuality(CompositingQualityHighQuality);
    memGraphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    memGraphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
    memGraphics.ScaleTransform(scale, scale);

    //��ü ��� ����
    memGraphics.Clear(Color(0, 0, 0, 0));

    DrawProgressBar(&memGraphics, RectF(0, 0, (REAL)rect.Width(), (REAL)rect.Height()));

    Graphics g(dc.GetSafeHdc());
    g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    g.DrawImage(&memBitmap, 0, 0, rect.Width(), rect.Height());
}

void CRoundLineProgressBarEx::OnDrawLayerdWindow(Graphics& Gps)
{
    CRect rect;
    GetWindowRect(&rect);
    GetParent()->ScreenToClient(&rect);

    int nWidth = rect.Width();
    int nHeight = rect.Height();

    //���ػ� �������� ���� �����ϸ�
    float scale = 2.0f; 
    int scaledWidth = (int)(nWidth * scale);
    int scaledHeight = (int)(nHeight * scale);

    Bitmap memBitmap(scaledWidth, scaledHeight);
    Graphics memGraphics(&memBitmap);

    //��ǰ�� ������ ����
    memGraphics.SetSmoothingMode(SmoothingModeHighQuality);
    memGraphics.SetCompositingQuality(CompositingQualityHighQuality);
    memGraphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    memGraphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
    memGraphics.ScaleTransform(scale, scale);

    //���� ���� ���
    memGraphics.Clear(Color(0, 0, 0, 0));

    DrawProgressBar(&memGraphics, RectF(0, 0, (REAL)nWidth, (REAL)nHeight));

    //���� Ȯ��
    /*
    SolidBrush bgBrush(Color(255, 255, 0, 0));
    memGraphics.FillRectangle(&bgBrush, 0, 0, rect.Width(), rect.Height());
    */

    Gps.TranslateTransform((REAL)rect.left, (REAL)rect.top);
    Gps.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    Gps.DrawImage(&memBitmap, 0, 0, nWidth, nHeight);
    Gps.ResetTransform();
}

void CRoundLineProgressBarEx::DrawProgressBar(Graphics* graphics, RectF rect)
{
    float radius = rect.Height / 2.0f;

    //�׸��� �׸���
    if (m_enableShadow)
    {
        RectF shadowRect = rect;
        shadowRect.Offset(1.0f, 1.0f);
        DrawRoundedRectangle(graphics, shadowRect, radius, Color(64, GetRValue(m_shadowColor), GetGValue(m_shadowColor), GetBValue(m_shadowColor)), nullptr, 0);
    }

    //��� �׸���
    Color bgColor(255, GetRValue(m_bgColor), GetGValue(m_bgColor), GetBValue(m_bgColor));
    Pen borderPen(Color(255, GetRValue(m_borderColor), GetGValue(m_borderColor), GetBValue(m_borderColor)), m_borderWidth);
    DrawRoundedRectangle(graphics, rect, radius, bgColor, &borderPen, m_borderWidth);

    //����� ���
    float percent = (float)(m_nPos - m_nMin) / (m_nMax - m_nMin);
    if (percent <= 0) return;

    //����� ���� ���
    float fillWidth = rect.Width * percent;
    if (fillWidth < radius * 2) 
    {
        //�ּ� �ʺ� ����
        fillWidth = radius * 2;
    }

    RectF fillRect(rect.X, rect.Y, fillWidth, rect.Height);

    // �׶���Ʈ �����
    if (m_enableGradient)
    {
        DrawGradientProgressBar(graphics, fillRect, radius);
    }
    else
    {
        Color barColor(255, GetRValue(m_barColor), GetGValue(m_barColor), GetBValue(m_barColor));
        DrawRoundedRectangle(graphics, fillRect, radius, barColor, nullptr, 0);
    }
}

void CRoundLineProgressBarEx::DrawRoundedRectangle(Graphics* graphics, RectF rect, float radius, Color fillColor, Pen* borderPen, float borderWidth)
{
    GraphicsPath path;

    //�ձ� �𼭸� ��� ����
    if (rect.Width <= radius * 2)
    {
        //���� ���������� �۰ų� ������ ��������
        path.AddEllipse(rect);
    }
    else
    {
        //���� �ݿ�
        path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 90, 180);

        //��� ����
        path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - radius, rect.Y);

        //������ �ݿ�
        path.AddArc(rect.X + rect.Width - radius * 2, rect.Y, radius * 2, radius * 2, 270, 180);

        //�ϴ� ���� (�ڵ����� ����)
        path.CloseFigure();
    }

    //ä���
    SolidBrush brush(fillColor);
    graphics->FillPath(&brush, &path);

    //�׵θ�
    if (borderPen)
    {
        graphics->DrawPath(borderPen, &path);
    }
}

void CRoundLineProgressBarEx::DrawGradientProgressBar(Graphics* graphics, RectF rect, float radius)
{
    //�׶���Ʈ ���� ���
    Color startColor(255, GetRValue(m_barColor), GetGValue(m_barColor), GetBValue(m_barColor));
    Color endColor(255, min(255, GetRValue(m_barColor) + 40), min(255, GetGValue(m_barColor) + 40), min(255, GetBValue(m_barColor) + 40));

    //���� �׶���Ʈ �귯��
    LinearGradientBrush gradientBrush(PointF(rect.X, rect.Y), PointF(rect.X, rect.Y + rect.Height), startColor, endColor);

    //�׶���Ʈ ��� ����
    GraphicsPath path;
    if (rect.Width <= radius * 2)
    {
        path.AddEllipse(rect);
    }
    else
    {
        path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 90, 180);
        path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - radius, rect.Y);
        path.AddArc(rect.X + rect.Width - radius * 2, rect.Y, radius * 2, radius * 2, 270, 180);
        path.CloseFigure();
    }

    graphics->FillPath(&gradientBrush, &path);

    //���̶���Ʈ ȿ�� (��ܿ� ���� ��)
    if (rect.Height > 4)
    {
        Color highlightColor(128, 255, 255, 255);
        Pen highlightPen(highlightColor, 1.0f);

        GraphicsPath highlightPath;
        if (rect.Width <= radius * 2)
        {
            // ������ ��� ��� ȣ��
            highlightPath.AddArc(rect.X + 1, rect.Y + 1, radius * 2 - 2, radius * 2 - 2, 135, 90);
        }
        else
        {
            // ��� ���θ�
            highlightPath.AddLine(rect.X + radius, rect.Y + 1, rect.X + rect.Width - radius, rect.Y + 1);
        }

        graphics->DrawPath(&highlightPen, &highlightPath);
    }
}

void CRoundLineProgressBarEx::SetRange(int min, int max)
{
    m_nMin = min;
    m_nMax = max;
    Invalidate();
}

void CRoundLineProgressBarEx::SetPos(int pos)
{
    m_nPos = max(min(pos, m_nMax), m_nMin);
    Invalidate();
}

void CRoundLineProgressBarEx::SetBarColor(COLORREF color)
{
    m_barColor = color;
    Invalidate();
}

void CRoundLineProgressBarEx::SetBgColor(COLORREF color)
{
    m_bgColor = color;
    Invalidate();
}

void CRoundLineProgressBarEx::SetBorderColor(COLORREF color)
{
    m_borderColor = color;
    Invalidate();
}

void CRoundLineProgressBarEx::SetBorderWidth(float width)
{
    m_borderWidth = width;
    Invalidate();
}

void CRoundLineProgressBarEx::SetShadowColor(COLORREF color)
{
    m_shadowColor = color;
    Invalidate();
}

void CRoundLineProgressBarEx::EnableShadow(bool enable)
{
    m_enableShadow = enable;
    Invalidate();
}

void CRoundLineProgressBarEx::EnableGradient(bool enable)
{
    m_enableGradient = enable;
    Invalidate();
}
//------------ CRoundLineProgressBarEx




//------------ CRoundLineProgressBar
CRoundLineProgressBar::CRoundLineProgressBar()
{
    m_nMin = 0;
    m_nMax = 100;
    m_nPos = 0;
    m_barColor = RGB(255, 111, 0);      // ��Ȳ��
    m_bgColor = RGB(255, 255, 255);     // ȸ�� ���
}

CRoundLineProgressBar::~CRoundLineProgressBar()
{
}

BEGIN_MESSAGE_MAP(CRoundLineProgressBar, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CRoundLineProgressBar::OnEraseBkgnd(CDC* pDC)
{
    // ������ ������ ���� ��� ����� ����
    return TRUE;
}

void CRoundLineProgressBar::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    // ���� ���۸��� ���� �޸� ��Ʈ�� ����
    Bitmap memBitmap(rect.Width(), rect.Height());
    Graphics memGraphics(&memBitmap);
    memGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
    memGraphics.SetCompositingQuality(CompositingQualityHighQuality);

    // ��ü ����� �θ� ��Ʈ�� �������� ä���
    SolidBrush bgBrush(Color(255, 72, 72, 72));
    memGraphics.FillRectangle(&bgBrush, 0, 0, rect.Width(), rect.Height());

    // ��� �׸���
    SolidBrush roundBgBrush(Color(255, GetRValue(m_bgColor), GetGValue(m_bgColor), GetBValue(m_bgColor)));
    GraphicsPath bgPath;
    bgPath.AddArc(rect.left, rect.top, rect.Height(), rect.Height(), 90, 180);
    bgPath.AddLine(rect.left + rect.Height() / 2, rect.top, rect.right - rect.Height() / 2, rect.top);
    bgPath.AddArc(rect.right - rect.Height(), rect.top, rect.Height(), rect.Height(), 270, 180);
    bgPath.CloseFigure();
    memGraphics.FillPath(&roundBgBrush, &bgPath);

    // ����� ���
    float percent = (float)(m_nPos - m_nMin) / (m_nMax - m_nMin);
    int fillWidth = (int)(rect.Width() * percent);
    if (fillWidth > 0)
    {
        GraphicsPath fillPath;
        fillPath.AddArc(rect.left, rect.top, rect.Height(), rect.Height(), 90, 180);
        fillPath.AddLine(rect.left + rect.Height() / 2, rect.top, rect.left + fillWidth - rect.Height() / 2, rect.top);
        fillPath.AddArc(rect.left + fillWidth - rect.Height(), rect.top, rect.Height(), rect.Height(), 270, 180);
        fillPath.CloseFigure();
        SolidBrush barBrush(Color(255, GetRValue(m_barColor), GetGValue(m_barColor), GetBValue(m_barColor)));
        memGraphics.FillPath(&barBrush, &fillPath);
    }

    // �޸� ��Ʈ���� ȭ�鿡 ���� (���� ���۸� �Ϸ�)
    Graphics g(dc.GetSafeHdc());
    g.DrawImage(&memBitmap, 0, 0, rect.Width(), rect.Height());
}

void CRoundLineProgressBar::OnDrawLayerdWindow(Graphics& Gps)
{
    CRect rect;
    GetWindowRect(&rect);
    GetParent()->ScreenToClient(&rect);

    int nWidth = rect.Width();
    int nHeight = rect.Height();

    // ���� ���۸��� �޸� ��Ʈ�� ����
    Bitmap memBitmap(nWidth, nHeight);
    Graphics memGraphics(&memBitmap);
    memGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
    memGraphics.SetCompositingQuality(CompositingQualityHighQuality);

    // ��ü ��� (���� ��� ���)
    SolidBrush bgBrush(Color(0, 255, 0, 0));  // ���� ���� ���
    memGraphics.FillRectangle(&bgBrush, 0, 0, nWidth, nHeight);

    // ��� ����
    SolidBrush roundBgBrush(Color(255, GetRValue(m_bgColor), GetGValue(m_bgColor), GetBValue(m_bgColor)));
    GraphicsPath bgPath;
    bgPath.AddArc(0, 0, nHeight, nHeight, 90, 180);
    bgPath.AddLine(nHeight / 2, 0, nWidth - nHeight / 2, 0);
    bgPath.AddArc(nWidth - nHeight, 0, nHeight, nHeight, 270, 180);
    bgPath.CloseFigure();
    memGraphics.FillPath(&roundBgBrush, &bgPath);

    // ����� ���
    float percent = (float)(m_nPos - m_nMin) / (m_nMax - m_nMin);
    int fillWidth = (int)(nWidth * percent);
    if (fillWidth > 0)
    {
        GraphicsPath fillPath;
        fillPath.AddArc(0, 0, nHeight, nHeight, 90, 180);
        fillPath.AddLine(nHeight / 2, 0, fillWidth - nHeight / 2, 0);
        fillPath.AddArc(fillWidth - nHeight, 0, nHeight, nHeight, 270, 180);
        fillPath.CloseFigure();
        SolidBrush barBrush(Color(255, GetRValue(m_barColor), GetGValue(m_barColor), GetBValue(m_barColor)));
        memGraphics.FillPath(&barBrush, &fillPath);
    }

    // ��ġ ������ ��ǥ�� �̵�
    Gps.TranslateTransform((REAL)rect.left, (REAL)rect.top);

    // �޸� ��Ʈ���� ȭ�鿡 ��� (���� ���۸� �Ϸ�)
    Gps.DrawImage(&memBitmap, 0, 0, nWidth, nHeight);

    // ��ǥ�� ��ȯ ����
    Gps.ResetTransform();
}

void CRoundLineProgressBar::SetRange(int min, int max)
{
    m_nMin = min;
    m_nMax = max;
    Invalidate();
}

void CRoundLineProgressBar::SetPos(int pos)
{
    m_nPos = max(min(pos, m_nMax), m_nMin);
    Invalidate();
}

void CRoundLineProgressBar::SetBarColor(COLORREF color)
{
    m_barColor = color;
    Invalidate();
}

void CRoundLineProgressBar::SetBgColor(COLORREF color)
{
    m_bgColor = color;
    Invalidate();
}
//------------ CRoundLineProgressBar