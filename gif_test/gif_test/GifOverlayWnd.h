#pragma once

class CGifOverlayWnd : public CWnd
{
public:
    CGifOverlayWnd();
    virtual ~CGifOverlayWnd();

    BOOL LoadGif(LPCTSTR lpszPath);
    BOOL LoadGifResource(UINT nResourceID, LPCTSTR lpszResourceType);
    void Start();
    void Stop();

protected:
    Image* m_pImage;
    GUID m_guid;
    UINT m_nFrameCount;
    UINT m_nCurrentFrame;
    UINT* m_pFrameDelays;
    UINT_PTR m_nTimerID;

    void ClearGifData();
    void LoadFrameDelays();
    void UpdateFrame();

    afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()
};

