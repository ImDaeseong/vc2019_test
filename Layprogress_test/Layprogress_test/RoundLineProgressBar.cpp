#include "pch.h"
#include "RoundLineProgressBar.h"

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