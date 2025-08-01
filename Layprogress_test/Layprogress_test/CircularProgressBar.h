#pragma once

class CircularProgressBar : public CStatic
{
public:
    CircularProgressBar();
    virtual ~CircularProgressBar();

    void SetProgress(float percent); // 0.0 ~ 1.0
    void Start(); // ȸ�� ����
    void Stop();  // ȸ�� ����

protected:
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    DECLARE_MESSAGE_MAP()

private:
    float m_fProgress;
    int   m_nAngle;
    BOOL  m_bRunning;
};
