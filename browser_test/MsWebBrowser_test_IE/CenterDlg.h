#pragma once
#include "afxdialogex.h"
#include "MsWebBrowser_testDlg.h"

class CenterDlg : public CDialog
{
	DECLARE_DYNAMIC(CenterDlg)

public:
	CenterDlg(CWnd* pParent = nullptr);
	virtual ~CenterDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CENTER_DIALOG	};
#endif

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	CMsWebBrowsertestDlg* m_pParent;

public:
	void SetParent(CMsWebBrowsertestDlg* pParent);
};
