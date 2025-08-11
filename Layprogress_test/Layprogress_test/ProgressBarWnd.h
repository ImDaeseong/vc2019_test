#pragma once

class CProgressBarWnd : public CWnd
{
public:
    CProgressBarWnd();
    virtual ~CProgressBarWnd();

    //기본 제어
    void Start();
    void Stop();
    void SetProgress(float percent);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnDestroy();

private:
    void CreateBuffer(int width, int height);
    void CleanupBuffer();
    void DrawProgressBar();
    void UpdateLayeredWindowBuffer();

    //상태 변수
    float m_fProgress;
    int m_nAngle;
    BOOL m_bRunning;

    //렌더링 관련
    Bitmap* m_pBitmap;
    HBITMAP m_hBitmap;
    HBITMAP m_oldBitmap;
    HDC m_hMemDC;
    Graphics* m_pGraphics;
};