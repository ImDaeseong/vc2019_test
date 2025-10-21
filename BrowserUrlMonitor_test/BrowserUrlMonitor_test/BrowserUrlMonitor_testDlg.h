#pragma once
#include "Resource.h"
#include "BrowserUrlMonitor.h"

#define WM_URL_DETECTED (WM_USER + 100)

class CBrowserUrlMonitortestDlg : public CDialogEx
{
public:
	CBrowserUrlMonitortestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BROWSERURLMONITOR_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
	afx_msg LRESULT OnUrlDetected(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CBrowserUrlMonitor* m_pUrlMonitor;
	BOOL InitUrlMonitor();
	void ReleaseUrlMonitor();
};
