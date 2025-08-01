#pragma once

class CProgressBarWnd : public CWnd
{
public:
    CProgressBarWnd();
    virtual ~CProgressBarWnd();

    void Start();
    void Stop();
    void SetProgress(float percent);

protected:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnDestroy();

    DECLARE_MESSAGE_MAP()

private:
    void CreateBuffer(int width, int height);
    void CleanupBuffer();
    void DrawProgressBar();

private:
    Bitmap* m_pBitmap = nullptr;
    HBITMAP m_hBitmap = NULL;
    HBITMAP m_oldBitmap = NULL;
    HDC m_hMemDC = NULL;
    Graphics* m_pGraphics = nullptr;

    float m_fProgress;
    int m_nAngle;
    BOOL m_bRunning;
};