#pragma once

class CRoundLineProgressBar : public CStatic
{
public:
    CRoundLineProgressBar();
    virtual ~CRoundLineProgressBar();

    void SetRange(int min, int max);
    void SetPos(int pos);
    void SetBarColor(COLORREF color);
    void SetBgColor(COLORREF color);
    void OnDrawLayerdWindow(Graphics& gps);

protected:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    DECLARE_MESSAGE_MAP()

private:
    int m_nMin;
    int m_nMax;
    int m_nPos;

    COLORREF m_barColor;
    COLORREF m_bgColor;
};