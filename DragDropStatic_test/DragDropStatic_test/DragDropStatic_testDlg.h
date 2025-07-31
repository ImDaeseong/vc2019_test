#pragma once
#include "resource.h"   
#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>     
#include <WebView2EnvironmentOptions.h>

class CDragDropStatictestDlg : public CDialogEx
{
public:
	CDragDropStatictestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAGDROPSTATIC_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	DECLARE_MESSAGE_MAP()


private:
	//브라우저
	wil::com_ptr<ICoreWebView2Controller> m_webViewController;
	wil::com_ptr<ICoreWebView2> m_webView;
	wil::com_ptr<ICoreWebView2Environment> m_webViewEnvironment;

	EventRegistrationToken m_navigationCompletedToken = {};

	void resizeControls();
	void InitStatic();
	void InitWebView();
	CString EscapeForJS(const CString& str);

public:
	void setData(const std::vector<CString>& strFilePaths);

private:
	CDragDropStatic m_dropStatic;
};
