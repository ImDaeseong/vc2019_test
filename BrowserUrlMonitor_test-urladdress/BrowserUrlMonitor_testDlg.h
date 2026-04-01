#pragma once
#include "Resource.h"

class CBrowserUrlMonitor;
class CBrowserUrlMonitortestDlg : public CDialogEx
{
public:
	CBrowserUrlMonitortestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BROWSERURLMONITOR_TEST_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL DestroyWindow();
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    CBrowserUrlMonitor* m_pUrlMonitor;
};
