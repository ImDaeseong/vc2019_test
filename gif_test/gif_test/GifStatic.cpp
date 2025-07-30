#include "pch.h"
#include "GifStatic.h"

CGifStatic::CGifStatic()
    : m_pImage(nullptr), m_nFrameCount(0), m_nCurrentFrame(0), m_pFrameDelays(nullptr), m_nTimerID(0)
{
}

CGifStatic::~CGifStatic()
{
    Stop();
    ClearGifData();
}

BOOL CGifStatic::LoadGif(LPCTSTR lpszPath)
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

    if (m_nFrameCount == 0 || m_pFrameDelays == nullptr)
    {
        ClearGifData();
        return FALSE;
    }

    return TRUE;
}

BOOL CGifStatic::LoadGifResource(UINT nResourceID, LPCTSTR lpszResourceType)
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

void CGifStatic::ClearGifData()
{
    if (m_pFrameDelays)
    {
        delete[] m_pFrameDelays;
        m_pFrameDelays = nullptr;
    }

    if (m_pImage)
    {
        delete m_pImage;
        m_pImage = nullptr;
    }

    m_nFrameCount = 0;
    m_nCurrentFrame = 0;
}

void CGifStatic::LoadFrameDelays()
{
    if (!m_pImage) return;

    UINT size = m_pImage->GetPropertyItemSize(PropertyTagFrameDelay);
    if (size == 0) return;

    PropertyItem* pItem = reinterpret_cast<PropertyItem*>(new(std::nothrow) BYTE[size]);
    if (!pItem) return;

    if (m_pImage->GetPropertyItem(PropertyTagFrameDelay, size, pItem) == Ok)
    {
        m_pFrameDelays = new(std::nothrow) UINT[m_nFrameCount];
        if (m_pFrameDelays)
        {
            memcpy(m_pFrameDelays, pItem->value, sizeof(UINT) * m_nFrameCount);
        }
    }

    delete[] reinterpret_cast<BYTE*>(pItem);
}

void CGifStatic::Start()
{
    if (!m_pImage || m_nFrameCount <= 1 || !m_pFrameDelays)
        return;

    Stop(); // 기존 타이머 제거

    m_nCurrentFrame = 0;
    m_pImage->SelectActiveFrame(&m_guid, m_nCurrentFrame);
    Invalidate();

    m_nTimerID = SetTimer(reinterpret_cast<UINT_PTR>(this), m_pFrameDelays[0] * 10, nullptr);
}

void CGifStatic::Stop()
{
    if (m_nTimerID != 0)
    {
        KillTimer(m_nTimerID);
        m_nTimerID = 0;
    }
}

BEGIN_MESSAGE_MAP(CGifStatic, CStatic)
    ON_WM_PAINT()
    ON_WM_TIMER()
END_MESSAGE_MAP()

void CGifStatic::OnPaint()
{
    CPaintDC dc(this);
    Graphics graphics(dc);
       
    if (m_pImage)
    {
        CRect rect;
        GetClientRect(&rect);
        graphics.DrawImage(m_pImage, rect.left, rect.top, rect.Width(), rect.Height());
    }
}

void CGifStatic::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent != reinterpret_cast<UINT_PTR>(this))
    {
        CStatic::OnTimer(nIDEvent);
        return;
    }

    if (!m_pImage || m_nFrameCount <= 1 || m_pFrameDelays == nullptr)
    {
        Stop();
        return;
    }

    m_nCurrentFrame = (m_nCurrentFrame + 1) % m_nFrameCount;
    m_pImage->SelectActiveFrame(&m_guid, m_nCurrentFrame);
    Invalidate();

    KillTimer(m_nTimerID);
    m_nTimerID = SetTimer(reinterpret_cast<UINT_PTR>(this), m_pFrameDelays[m_nCurrentFrame] * 10, nullptr);

    if (m_nTimerID == 0)
        Stop();
}