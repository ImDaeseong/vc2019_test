#pragma once

//------------ CRoundLineProgressBarEx
class CRoundLineProgressBarEx : public CStatic
{
public:
    CRoundLineProgressBarEx();
    virtual ~CRoundLineProgressBarEx();

    void SetRange(int min, int max);
    void SetPos(int pos);
    void SetBarColor(COLORREF color);
    void SetBgColor(COLORREF color);
    void OnDrawLayerdWindow(Graphics& Gps);

    void SetBorderColor(COLORREF color);
    void SetBorderWidth(float width);
    void SetShadowColor(COLORREF color);
    void EnableShadow(bool enable);
    void EnableGradient(bool enable);

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

    COLORREF m_borderColor;
    float m_borderWidth;
    COLORREF m_shadowColor;
    bool m_enableShadow;
    bool m_enableGradient;

    void DrawProgressBar(Graphics* graphics, RectF rect);
    void DrawRoundedRectangle(Graphics* graphics, RectF rect, float radius, Color fillColor, Pen* borderPen = nullptr, float borderWidth = 0);
    void DrawGradientProgressBar(Graphics* graphics, RectF rect, float radius);
};
//------------ CRoundLineProgressBarEx


//------------ CRoundLineProgressBar
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
//------------ CRoundLineProgressBar