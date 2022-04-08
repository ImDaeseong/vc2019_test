#pragma once
#include "WebBrowser2.h"

class TopDlg;
class CenterDlg;
class BottomDlg;
class CMsWebBrowsertestDlg : public CDialogEx
{
public:
	CMsWebBrowsertestDlg(CWnd* pParent = nullptr);	

	void CallMain();

	void SetChildPos();

	TopDlg* m_pTopDlg;
	void CreateTopDlg();
	void DestroyTopDlg();

	CenterDlg* m_pCenterDlg;
	void CreateCenterDlg();
	void DestroyCenterDlg();

	BottomDlg* m_pBottomDlg;
	void CreateBottomDlg();
	void DestroyBottomDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MSWEBBROWSER_TEST_DIALOG };
#endif

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnPaint();
	afx_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
	void OnDownloadcompleteExplorer1();
	void OnDocumentcompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);

private:
	CGdiPlusBitmapResource* m_pbgImage;
	void LoadImage();
	void DrawSkin(CDC* pDC);

	BOOL m_LoadedFlag;
	CWebBrowser2 m_WebBrowser;
	void InitWebBrowser();
	void NavigateA(CString strURL);
};
