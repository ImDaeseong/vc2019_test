#include "pch.h"
#include "ProgressBarWnd.h"

BEGIN_MESSAGE_MAP(CProgressBarWnd, CWnd)
    ON_WM_TIMER()
    ON_WM_ERASEBKGND()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

CProgressBarWnd::CProgressBarWnd()
    : m_fProgress(0.0f), m_nAngle(0), m_bRunning(FALSE),
    m_pBitmap(nullptr), m_hBitmap(NULL), m_oldBitmap(NULL), m_hMemDC(NULL), m_pGraphics(nullptr)
{
}

CProgressBarWnd::~CProgressBarWnd()
{
    Stop();
    CleanupBuffer();
}

void CProgressBarWnd::CreateBuffer(int width, int height)
{
    if (width <= 0 || height <= 0)
        return;

    CleanupBuffer();

    HDC hdcScreen = ::GetDC(NULL);
    if (!hdcScreen)
        return;

    m_hMemDC = CreateCompatibleDC(hdcScreen);
    if (!m_hMemDC)
    {
        ::ReleaseDC(NULL, hdcScreen);
        return;
    }

    if (m_pBitmap)
    {
        delete m_pBitmap;
        m_pBitmap = nullptr;
    }
    m_pBitmap = new Bitmap(width, height, PixelFormat32bppARGB);

    if (!m_pBitmap || m_pBitmap->GetLastStatus() != Ok)
    {
        CleanupBuffer();
        ::ReleaseDC(NULL, hdcScreen);
        return;
    }

    HBITMAP hBitmap = NULL;
    Status status = m_pBitmap->GetHBITMAP(Color(0, 0, 0, 0), &hBitmap);
    if (status != Ok || !hBitmap)
    {
        CleanupBuffer();
        ::ReleaseDC(NULL, hdcScreen);
        return;
    }

    m_oldBitmap = (HBITMAP)SelectObject(m_hMemDC, hBitmap);
    m_hBitmap = hBitmap;

    if (m_pGraphics)
    {
        delete m_pGraphics;
        m_pGraphics = nullptr;
    }
    m_pGraphics = new Graphics(m_hMemDC);

    if (m_pGraphics)
    {
        //향상된 렌더링 품질
        m_pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
        m_pGraphics->SetCompositingQuality(CompositingQualityHighQuality);
        m_pGraphics->SetPixelOffsetMode(PixelOffsetModeHighQuality);
    }

    ::ReleaseDC(NULL, hdcScreen);
}

void CProgressBarWnd::CleanupBuffer()
{
    if (m_pGraphics)
    {
        delete m_pGraphics;
        m_pGraphics = nullptr;
    }

    if (m_pBitmap)
    {
        delete m_pBitmap;
        m_pBitmap = nullptr;
    }

    if (m_hBitmap)
    {
        if (m_hMemDC && m_oldBitmap)
        {
            SelectObject(m_hMemDC, m_oldBitmap);
            m_oldBitmap = NULL;
        }
        DeleteObject(m_hBitmap);
        m_hBitmap = NULL;
    }

    if (m_hMemDC)
    {
        DeleteDC(m_hMemDC);
        m_hMemDC = NULL;
    }
}

void CProgressBarWnd::SetProgress(float percent)
{
    m_fProgress = max(0.0f, min(1.0f, percent));
    Invalidate(FALSE);
}

void CProgressBarWnd::Start()
{
    if (!m_bRunning)
    {
        m_bRunning = TRUE;
        if (::IsWindow(m_hWnd))
        {
            SetTimer(1, 30, nullptr);
        }
    }
}

void CProgressBarWnd::Stop()
{
    if (m_bRunning)
    {
        m_bRunning = FALSE;
        if (::IsWindow(m_hWnd))
        {
            KillTimer(1);
        }
    }
}

BOOL CProgressBarWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}

void CProgressBarWnd::OnDestroy()
{
    Stop();
    CleanupBuffer();

    CWnd::OnDestroy();
}

void CProgressBarWnd::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1 && m_bRunning)
    {
        CRect rc;
        GetClientRect(&rc);
        int width = rc.Width();
        int height = rc.Height();

        if ( m_hMemDC == NULL || 
             m_pBitmap == nullptr ||
             width != (int)m_pBitmap->GetWidth() || 
             height != (int)m_pBitmap->GetHeight() )
        {
            CreateBuffer(width, height);
        }

        if (m_pGraphics)
        {
            //부드러운 애니메이션 (기본 속도)
            m_nAngle = (m_nAngle + 4) % 360;

            DrawProgressBar();
            UpdateLayeredWindowBuffer();
        }

    }

    CWnd::OnTimer(nIDEvent);
}

void CProgressBarWnd::UpdateLayeredWindowBuffer()
{
    if (!m_hMemDC || !::IsWindow(m_hWnd))
        return;

    CRect rc;
    GetClientRect(&rc);

    POINT ptSrc = { 0, 0 };
    POINT ptWndPos;
    RECT rcWnd;
    GetWindowRect(&rcWnd);
    ptWndPos.x = rcWnd.left;
    ptWndPos.y = rcWnd.top;
    SIZE sizeWnd = { rc.Width(), rc.Height() };

    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    HDC hdcScreen = ::GetDC(NULL);
    if (hdcScreen)
    {
        ::UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWndPos, &sizeWnd,
            m_hMemDC, &ptSrc, 0, &blend, ULW_ALPHA);
        ::ReleaseDC(NULL, hdcScreen);
    }
}

void CProgressBarWnd::DrawProgressBar()
{
    if (!m_pGraphics)
        return;

    CRect rc;
    GetClientRect(&rc);
    int cx = rc.Width();
    int cy = rc.Height();

    if (cx <= 0 || cy <= 0)
        return;

    m_pGraphics->Clear(Color(0, 0, 0, 0));

    //여백
    int size = min(cx, cy) - 16; 
    if (size <= 10)
        return;

    Rect rect((cx - size) / 2, (cy - size) / 2, size, size);

    //배경 원
    Pen bgPen(Color(255, 255, 255, 255), 8.0f);
    bgPen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
    m_pGraphics->DrawEllipse(&bgPen, rect);

    //진행률/회전 표시
    Pen fgPen(Color(255, 255, 153, 0), 6.0f);
    fgPen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);

    if (m_bRunning)
    {
        //회전 애니메이션
        m_pGraphics->DrawArc(&fgPen, rect, (REAL)m_nAngle, 90);
    }
    else if (m_fProgress > 0.0f)
    {
        //정적 진행률 표시
        REAL sweepAngle = 360.0f * m_fProgress;
        m_pGraphics->DrawArc(&fgPen, rect, -90, sweepAngle);
    }
}