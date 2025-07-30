#pragma once

class CGifStatic : public CStatic
{
public:
    CGifStatic();
    virtual ~CGifStatic();

    BOOL LoadGif(LPCTSTR lpszPath);
    BOOL LoadGifResource(UINT nResourceID, LPCTSTR lpszResourceType);
    void Start();
    void Stop();
    void ClearGifData();

protected:
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    void LoadFrameDelays();

    Image* m_pImage;
    UINT m_nFrameCount;
    UINT m_nCurrentFrame;
    UINT* m_pFrameDelays;
    GUID m_guid;
    UINT_PTR m_nTimerID;
};

