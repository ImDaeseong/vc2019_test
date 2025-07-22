#pragma once
#include "resource.h"   
#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>     
#include <WebView2EnvironmentOptions.h>

class CFinderAllFilesDlg : public CDialogEx
{
public:
	CFinderAllFilesDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINDERALLFILES_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	void SearchAllFiles(CString strDirectory);

	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	CString SelectFolder(HWND hWnd, const CString& strInitialDir);
	CString GetModulePath();
	CString GetRelativePath(const CString& fullPath, const CString& basePath);

	CString m_strFolderPath;

private:
	wil::com_ptr<ICoreWebView2Controller> m_webViewController;
	wil::com_ptr<ICoreWebView2> m_webView;
	wil::com_ptr<ICoreWebView2Environment> m_webViewEnvironment;

	EventRegistrationToken m_navigationCompletedToken = {};

	void InitWebVeiw();
};
