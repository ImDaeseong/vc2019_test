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
    CleanupBuffer();

    HDC hdcScreen = ::GetDC(NULL);
    m_hMemDC = CreateCompatibleDC(hdcScreen);

    if (m_pBitmap)
    {
        delete m_pBitmap;
        m_pBitmap = nullptr;
    }
    m_pBitmap = new Bitmap(width, height, PixelFormat32bppARGB);

    HBITMAP hBitmap = NULL;
    m_pBitmap->GetHBITMAP(Color(0, 0, 0, 0), &hBitmap);

    m_oldBitmap = (HBITMAP)SelectObject(m_hMemDC, hBitmap);
    m_hBitmap = hBitmap;

    if (m_pGraphics)
    {
        delete m_pGraphics;
        m_pGraphics = nullptr;
    }
    m_pGraphics = new Graphics(m_hMemDC);
    m_pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
    m_pGraphics->SetCompositingQuality(CompositingQualityHighQuality);

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
            SelectObject(m_hMemDC, m_oldBitmap); // 이전 비트맵 복원
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
        SetTimer(1, 30, nullptr);
    }
}

void CProgressBarWnd::Stop()
{
    if (m_bRunning)
    {
        m_bRunning = FALSE;
        KillTimer(1);
    }
}

BOOL CProgressBarWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
    // 깜박임 방지: 배경 지우지 않음
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

        if (m_hMemDC == NULL || m_pBitmap == nullptr ||
            width != m_pBitmap->GetWidth() || height != m_pBitmap->GetHeight())
        {
            CreateBuffer(width, height);
        }

        m_nAngle = (m_nAngle + 5) % 360;

        DrawProgressBar();

        POINT ptSrc = { 0, 0 };
        POINT ptWndPos;
        RECT rcWnd;
        GetWindowRect(&rcWnd);
        ptWndPos.x = rcWnd.left;
        ptWndPos.y = rcWnd.top;
        SIZE sizeWnd = { width, height };

        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

        HDC hdcScreen = ::GetDC(NULL);
        ::UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWndPos, &sizeWnd, m_hMemDC, &ptSrc, 0, &blend, ULW_ALPHA);
        ::ReleaseDC(NULL, hdcScreen);
    }

    CWnd::OnTimer(nIDEvent);
}

void CProgressBarWnd::DrawProgressBar()
{
    if (!m_pGraphics)
        return;

    CRect rc;
    GetClientRect(&rc);
    int cx = rc.Width();
    int cy = rc.Height();

    m_pGraphics->Clear(Color(0, 0, 0, 0)); // 완전 투명 배경

    int size = min(cx, cy) - 10;
    if (size <= 0)
        return;

    Rect rect((cx - size) / 2, (cy - size) / 2, size, size);

    Pen bgPen(Color(255, 255, 255, 255), 8.0f);
    bgPen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
    m_pGraphics->DrawArc(&bgPen, rect, 0, 360);

    Pen fgPen(Color(255, 255, 153, 0), 6.0f);
    fgPen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);

    if (m_bRunning)
    {
        m_pGraphics->DrawArc(&fgPen, rect, (REAL)m_nAngle, 90);
    }
    else if (m_fProgress > 0.0f)
    {
        REAL sweepAngle = 360.0f * m_fProgress;
        m_pGraphics->DrawArc(&fgPen, rect, -90, sweepAngle);
    }
}
