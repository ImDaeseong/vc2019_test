#include "pch.h"
#include "GifOverlayWnd.h"

CGifOverlayWnd::CGifOverlayWnd()
    : m_pImage(nullptr), m_nFrameCount(0), m_nCurrentFrame(0), m_pFrameDelays(nullptr), m_nTimerID(0)
{
}

CGifOverlayWnd::~CGifOverlayWnd()
{
    Stop();
    ClearGifData();
}

BOOL CGifOverlayWnd::LoadGif(LPCTSTR lpszPath)
{
    Stop();
    ClearGifData();

    m_pImage = Image::FromFile(lpszPath);
    if (!m_pImage || m_pImage->GetLastStatus() != Ok)
    {
        ClearGifData();
        return FALSE;
    }

    m_guid = FrameDimensionTime;
    m_nFrameCount = m_pImage->GetFrameCount(&m_guid);

    LoadFrameDelays();

    return m_nFrameCount > 0 && m_pFrameDelays;
}

BOOL CGifOverlayWnd::LoadGifResource(UINT nResourceID, LPCTSTR lpszResourceType)
{
    Stop();
    ClearGifData();

    HINSTANCE hInstance = AfxGetResourceHandle();

    HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(nResourceID), lpszResourceType);
    if (!hRes) return FALSE;

    HGLOBAL hGlobal = LoadResource(hInstance, hRes);
    if (!hGlobal) return FALSE;

    DWORD dwSize = SizeofResource(hInstance, hRes);
    if (dwSize == 0) return FALSE;

    LPVOID pData = LockResource(hGlobal);
    if (!pData) return FALSE;

    HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, dwSize);
    if (!hBuffer) return FALSE;

    void* pBuffer = GlobalLock(hBuffer);
    if (!pBuffer)
    {
        GlobalFree(hBuffer);
        return FALSE;
    }

    memcpy(pBuffer, pData, dwSize);
    GlobalUnlock(hBuffer);

    IStream* pStream = nullptr;
    HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);
    if (FAILED(hr))
    {
        GlobalFree(hBuffer);
        return FALSE;
    }

    m_pImage = Gdiplus::Image::FromStream(pStream);
    pStream->Release();

    if (!m_pImage || m_pImage->GetLastStatus() != Gdiplus::Ok)
    {
        ClearGifData();
        return FALSE;
    }

    m_guid = FrameDimensionTime;
    m_nFrameCount = m_pImage->GetFrameCount(&m_guid);

    LoadFrameDelays();

    if (m_nFrameCount == 0 || m_pFrameDelays == nullptr)
    {
        ClearGifData();
        return FALSE;
    }

    return TRUE;
}

void CGifOverlayWnd::Start()
{
    if (!m_pImage || m_nFrameCount <= 1 || !m_pFrameDelays) return;

    m_nCurrentFrame = 0;
    UpdateFrame();
    m_nTimerID = SetTimer(1, m_pFrameDelays[0] * 10, nullptr);
}

void CGifOverlayWnd::Stop()
{
    if (m_nTimerID != 0)
    {
        KillTimer(m_nTimerID);
        m_nTimerID = 0;
    }
}

void CGifOverlayWnd::LoadFrameDelays()
{
    if (!m_pImage) return;

    UINT size = m_pImage->GetPropertyItemSize(PropertyTagFrameDelay);
    if (size == 0) return;

    PropertyItem* pItem = reinterpret_cast<PropertyItem*>(new BYTE[size]);
    if (!pItem) return;

    if (m_pImage->GetPropertyItem(PropertyTagFrameDelay, size, pItem) == Ok)
    {
        m_pFrameDelays = new UINT[m_nFrameCount];
        memcpy(m_pFrameDelays, pItem->value, sizeof(UINT) * m_nFrameCount);
    }

    delete[] reinterpret_cast<BYTE*>(pItem);
}

void CGifOverlayWnd::ClearGifData()
{
    delete[] m_pFrameDelays;
    m_pFrameDelays = nullptr;

    delete m_pImage;
    m_pImage = nullptr;

    m_nFrameCount = 0;
    m_nCurrentFrame = 0;
}

void CGifOverlayWnd::UpdateFrame()
{
    if (!m_pImage) return;

    m_pImage->SelectActiveFrame(&m_guid, m_nCurrentFrame);

    CRect rClient;
    GetClientRect(&rClient);

    CClientDC dcScreen(nullptr);

    CDC memDC;
    memDC.CreateCompatibleDC(&dcScreen);

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = rClient.Width();
    bmi.bmiHeader.biHeight = -rClient.Height();
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pBits = nullptr;
    HBITMAP hBmp = CreateDIBSection(dcScreen, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
    if (!hBmp) return;

    CBitmap bmp;
    bmp.Attach(hBmp);
    CBitmap* pOldBmp = memDC.SelectObject(&bmp);

    Graphics graphics(memDC);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

    graphics.Clear(Color(0, 0, 0, 0)); // 완전 투명

    graphics.DrawImage(m_pImage, 0, 0, rClient.Width(), rClient.Height());

    POINT ptSrc = { 0, 0 };
    SIZE size = { rClient.Width(), rClient.Height() };
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    ::UpdateLayeredWindow(GetSafeHwnd(), dcScreen.GetSafeHdc(), nullptr, &size,
        memDC.GetSafeHdc(), &ptSrc, 0, &blend, ULW_ALPHA);

    memDC.SelectObject(pOldBmp);
    ::DeleteObject(bmp.Detach());
}

BEGIN_MESSAGE_MAP(CGifOverlayWnd, CWnd)
    ON_WM_TIMER()
END_MESSAGE_MAP()

void CGifOverlayWnd::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent != 1) return;

    m_nCurrentFrame = (m_nCurrentFrame + 1) % m_nFrameCount;
    UpdateFrame();

    KillTimer(1);
    m_nTimerID = SetTimer(1, m_pFrameDelays[m_nCurrentFrame] * 10, nullptr);
}
