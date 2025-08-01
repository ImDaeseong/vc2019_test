#include "pch.h"
#include "CircularProgressBar.h"

BEGIN_MESSAGE_MAP(CircularProgressBar, CStatic)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CircularProgressBar::CircularProgressBar()
    : m_fProgress(0.0f), m_nAngle(0), m_bRunning(FALSE)
{   
}

CircularProgressBar::~CircularProgressBar()
{
    Stop();
}

void CircularProgressBar::SetProgress(float percent)
{
    //입력값을 0.0~1.0 범위로 제한
    m_fProgress = max(0.0f, min(1.0f, percent));

    if (GetSafeHwnd())
    {
        //배경 지우지 않음(깜박임 방지)
        Invalidate(FALSE);
    }
}

void CircularProgressBar::Start()
{
    if (!m_bRunning)
    {
        m_bRunning = TRUE;
        SetTimer(1, 30, nullptr); 
    }
}

void CircularProgressBar::Stop()
{
    if (m_bRunning)
    {
        m_bRunning = FALSE;
        KillTimer(1);

        if (GetSafeHwnd())
        {
            //배경 지우지 않음(깜박임 방지)
            Invalidate(FALSE);
        }
    }
}

void CircularProgressBar::OnPaint()
{
    CPaintDC dc(this);

    CRect rc;
    GetClientRect(&rc);

    //더블 버퍼링을 위한 메모리 비트맵 생성
    Bitmap memBitmap(rc.Width(), rc.Height());
    Graphics memGraphics(&memBitmap);
    memGraphics.SetSmoothingMode(SmoothingModeAntiAlias);
    memGraphics.SetCompositingQuality(CompositingQualityHighQuality);

    //배경색
    SolidBrush bgBrush(Color(255, 72,72, 72));
    memGraphics.FillRectangle(&bgBrush, 0, 0, rc.Width(), rc.Height());

    //프로그래스바 크기 설정
    int cx = rc.Width();
    int cy = rc.Height();
    int size = min(cx, cy) - 10;

    if (size > 0)
    {
        //원을 그릴 사각형 영역 계산(중앙 정렬)
        Rect rect((cx - size) / 2, (cy - size) / 2, size, size);

        //배경 원(흰색, 전체 360도)
        Pen bgPen(Color(255, 255, 255, 255), 8.0f);//흰색, 8픽셀 두께
        bgPen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);//둥근 끝
        memGraphics.DrawArc(&bgPen, rect, 0.0f, 360.0f);

        //진행률 원(주황색)
        Pen fgPen(Color(255, 255, 153, 0), 6.0f);//주황색, 6픽셀 두께
        fgPen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);//둥근 끝

        if (m_bRunning)
        {
            //회전 상태 - 90도 호를 회전시키며 그리기
            memGraphics.DrawArc(&fgPen, rect, (REAL)m_nAngle, 90.0f);
        }
        else
        {
            // 진행률 상태 - 설정된 진행률만큼 호 그리기
            if (m_fProgress > 0.0f)
            {
                REAL sweepAngle = 360.0f * m_fProgress;// 진행률을 각도로 변환
                memGraphics.DrawArc(&fgPen, rect, -90.0f, sweepAngle);// 12시 방향부터 시작
            }
        }
    }

    //메모리 비트맵을 화면에 복사 (더블 버퍼링 완료)
    Graphics g(dc.GetSafeHdc());
    g.DrawImage(&memBitmap, 0, 0, rc.Width(), rc.Height());
}

BOOL CircularProgressBar::OnEraseBkgnd(CDC* pDC)
{
    // 깜박임 방지를 위해 배경 지우기 차단
    return TRUE;
}

void CircularProgressBar::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1 && m_bRunning)
    {
        m_nAngle = (m_nAngle + 5) % 360;//각도를 5도씩 증가 (360도에서 0도로 순환)
        Invalidate(FALSE);//화면 다시 그리기 (배경 지우지 않음)
    }

    CStatic::OnTimer(nIDEvent);
}

