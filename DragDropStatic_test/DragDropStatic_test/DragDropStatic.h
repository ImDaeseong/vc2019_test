#pragma once
class CDragDropStatic : public CStatic
{
public:
    BOOL m_bHighlight = FALSE;

protected:
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDropFiles(HDROP hDropInfo);
    DECLARE_MESSAGE_MAP()
};
