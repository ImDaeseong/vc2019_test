#pragma once
#include "resource.h"   
#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>     
#include <WebView2EnvironmentOptions.h>

class ChtmlUpdateDlgDlg : public CDialogEx
{
public:
	ChtmlUpdateDlgDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	void UpdateProgress(int percent);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HTMLUPDATEDLG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	wil::com_ptr<ICoreWebView2Controller> m_webViewController;
	wil::com_ptr<ICoreWebView2> m_webView;
	wil::com_ptr<ICoreWebView2Environment> m_webViewEnvironment;

	EventRegistrationToken m_navigationCompletedToken = {};

	void InitWebVeiw();

	int m_nProgress = 0;
};
