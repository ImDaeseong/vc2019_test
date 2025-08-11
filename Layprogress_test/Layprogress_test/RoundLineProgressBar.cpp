#include "pch.h"
#include "RoundLineProgressBar.h"


//------------ CRoundLineProgressBarEx
CRoundLineProgressBarEx::CRoundLineProgressBarEx()
{
    m_nMin = 0;
    m_nMax = 100;
    m_nPos = 0;
    m_barColor = RGB(255, 111, 0);      //주황색
    m_bgColor = RGB(255, 255, 255);     //흰색 배경
    m_borderColor = RGB(200, 200, 200); //테두리 색상
    m_borderWidth = 1.0f;               //테두리 두께
    m_shadowColor = RGB(128, 128, 128); //그림자 색상
    m_enableShadow = true;              //그림자 활성화
    m_enableGradient = true;            //그라디언트 활성화
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

    //고해상도 렌더링을 위한 스케일링
    float scale = max(1.0f, dc.GetDeviceCaps(LOGPIXELSX) / 96.0f);
    int scaledWidth = (int)(rect.Width() * scale);
    int scaledHeight = (int)(rect.Height() * scale);

    Bitmap memBitmap(scaledWidth, scaledHeight);
    Graphics memGraphics(&memBitmap);

    //고품질 렌더링 설정
    memGraphics.SetSmoothingMode(SmoothingModeHighQuality);
    memGraphics.SetCompositingQuality(CompositingQualityHighQuality);
    memGraphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    memGraphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
    memGraphics.ScaleTransform(scale, scale);

    //전체 배경 투명
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

    //고해상도 렌더링을 위한 스케일링
    float scale = 2.0f; 
    int scaledWidth = (int)(nWidth * scale);
    int scaledHeight = (int)(nHeight * scale);

    Bitmap memBitmap(scaledWidth, scaledHeight);
    Graphics memGraphics(&memBitmap);

    //고품질 렌더링 설정
    memGraphics.SetSmoothingMode(SmoothingModeHighQuality);
    memGraphics.SetCompositingQuality(CompositingQualityHighQuality);
    memGraphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    memGraphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
    memGraphics.ScaleTransform(scale, scale);

    //완전 투명 배경
    memGraphics.Clear(Color(0, 0, 0, 0));

    DrawProgressBar(&memGraphics, RectF(0, 0, (REAL)nWidth, (REAL)nHeight));

    //영역 확인
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

    //그림자 그리기
    if (m_enableShadow)
    {
        RectF shadowRect = rect;
        shadowRect.Offset(1.0f, 1.0f);
        DrawRoundedRectangle(graphics, shadowRect, radius, Color(64, GetRValue(m_shadowColor), GetGValue(m_shadowColor), GetBValue(m_shadowColor)), nullptr, 0);
    }

    //배경 그리기
    Color bgColor(255, GetRValue(m_bgColor), GetGValue(m_bgColor), GetBValue(m_bgColor));
    Pen borderPen(Color(255, GetRValue(m_borderColor), GetGValue(m_borderColor), GetBValue(m_borderColor)), m_borderWidth);
    DrawRoundedRectangle(graphics, rect, radius, bgColor, &borderPen, m_borderWidth);

    //진행률 계산
    float percent = (float)(m_nPos - m_nMin) / (m_nMax - m_nMin);
    if (percent <= 0) return;

    //진행바 영역 계산
    float fillWidth = rect.Width * percent;
    if (fillWidth < radius * 2) 
    {
        //최소 너비 보장
        fillWidth = radius * 2;
    }

    RectF fillRect(rect.X, rect.Y, fillWidth, rect.Height);

    // 그라디언트 진행바
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

    //둥근 모서리 경로 생성
    if (rect.Width <= radius * 2)
    {
        //폭이 반지름보다 작거나 같으면 원형으로
        path.AddEllipse(rect);
    }
    else
    {
        //왼쪽 반원
        path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 90, 180);

        //상단 직선
        path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - radius, rect.Y);

        //오른쪽 반원
        path.AddArc(rect.X + rect.Width - radius * 2, rect.Y, radius * 2, radius * 2, 270, 180);

        //하단 직선 (자동으로 닫힘)
        path.CloseFigure();
    }

    //채우기
    SolidBrush brush(fillColor);
    graphics->FillPath(&brush, &path);

    //테두리
    if (borderPen)
    {
        graphics->DrawPath(borderPen, &path);
    }
}

void CRoundLineProgressBarEx::DrawGradientProgressBar(Graphics* graphics, RectF rect, float radius)
{
    //그라디언트 색상 계산
    Color startColor(255, GetRValue(m_barColor), GetGValue(m_barColor), GetBValue(m_barColor));
    Color endColor(255, min(255, GetRValue(m_barColor) + 40), min(255, GetGValue(m_barColor) + 40), min(255, GetBValue(m_barColor) + 40));

    //선형 그라디언트 브러시
    LinearGradientBrush gradientBrush(PointF(rect.X, rect.Y), PointF(rect.X, rect.Y + rect.Height), startColor, endColor);

    //그라디언트 경로 생성
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

    //하이라이트 효과 (상단에 밝은 줄)
    if (rect.Height > 4)
    {
        Color highlightColor(128, 255, 255, 255);
        Pen highlightPen(highlightColor, 1.0f);

        GraphicsPath highlightPath;
        if (rect.Width <= radius * 2)
        {
            // 원형의 경우 상단 호만
            highlightPath.AddArc(rect.X + 1, rect.Y + 1, radius * 2 - 2, radius * 2 - 2, 135, 90);
        }
        else
        {
            // 상단 라인만
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
    m_barColor = RGB(255, 111, 0);      // 주황색
    m_bgColor = RGB(255, 255, 255);     // 회색 배경
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
    // 깜박임 방지를 위해 배경 지우기 차단
    return TRUE;
}

void CRoundLineProgressBar::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    // 더블 버퍼링을 위한 메모리 비트맵 생성
    Bitmap memBitmap(rect.Width(), rect.Height());
    Graphics memGraphics(&memBitmap);
    memGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
    memGraphics.SetCompositingQuality(CompositingQualityHighQuality);

    // 전체 배경을 부모 컨트롤 색상으로 채우기
    SolidBrush bgBrush(Color(255, 72, 72, 72));
    memGraphics.FillRectangle(&bgBrush, 0, 0, rect.Width(), rect.Height());

    // 배경 그리기
    SolidBrush roundBgBrush(Color(255, GetRValue(m_bgColor), GetGValue(m_bgColor), GetBValue(m_bgColor)));
    GraphicsPath bgPath;
    bgPath.AddArc(rect.left, rect.top, rect.Height(), rect.Height(), 90, 180);
    bgPath.AddLine(rect.left + rect.Height() / 2, rect.top, rect.right - rect.Height() / 2, rect.top);
    bgPath.AddArc(rect.right - rect.Height(), rect.top, rect.Height(), rect.Height(), 270, 180);
    bgPath.CloseFigure();
    memGraphics.FillPath(&roundBgBrush, &bgPath);

    // 진행률 계산
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

    // 메모리 비트맵을 화면에 복사 (더블 버퍼링 완료)
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

    // 더블 버퍼링용 메모리 비트맵 생성
    Bitmap memBitmap(nWidth, nHeight);
    Graphics memGraphics(&memBitmap);
    memGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
    memGraphics.SetCompositingQuality(CompositingQualityHighQuality);

    // 전체 배경 (투명 배경 고려)
    SolidBrush bgBrush(Color(0, 255, 0, 0));  // 완전 투명 배경
    memGraphics.FillRectangle(&bgBrush, 0, 0, nWidth, nHeight);

    // 배경 색상
    SolidBrush roundBgBrush(Color(255, GetRValue(m_bgColor), GetGValue(m_bgColor), GetBValue(m_bgColor)));
    GraphicsPath bgPath;
    bgPath.AddArc(0, 0, nHeight, nHeight, 90, 180);
    bgPath.AddLine(nHeight / 2, 0, nWidth - nHeight / 2, 0);
    bgPath.AddArc(nWidth - nHeight, 0, nHeight, nHeight, 270, 180);
    bgPath.CloseFigure();
    memGraphics.FillPath(&roundBgBrush, &bgPath);

    // 진행률 계산
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

    // 위치 보정용 좌표계 이동
    Gps.TranslateTransform((REAL)rect.left, (REAL)rect.top);

    // 메모리 비트맵을 화면에 출력 (더블 버퍼링 완료)
    Gps.DrawImage(&memBitmap, 0, 0, nWidth, nHeight);

    // 좌표계 변환 복원
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