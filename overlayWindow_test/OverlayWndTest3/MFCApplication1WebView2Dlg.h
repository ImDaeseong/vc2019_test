#pragma once

class CMFCApplication1WebView2Dlg : public CDialogEx
{
public:
    CMFCApplication1WebView2Dlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MFCAPPLICATION1WEBVIEW2_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    DECLARE_MESSAGE_MAP()

private:
    BOOL IsTopmostWindow(HWND hCheckWnd);
    void setBrowserMode(BOOL bShow);
    BOOL m_bShow;

    COverlayWnd* m_overWnd;
};