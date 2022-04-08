#pragma once
#include "afxdialogex.h"
#include "MsWebBrowser_testDlg.h"

class TopDlg : public CDialog
{
	DECLARE_DYNAMIC(TopDlg)

public:
	TopDlg(CWnd* pParent = nullptr);  
	virtual ~TopDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOP_DIALOG };
#endif

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

public:
	CTextStatic mDisplayText;

private:
	CGdiPlusBitmapResource* m_pbgImage;
	void LoadImage();
	void DrawSkin();

	CMsWebBrowsertestDlg* m_pParent;

public:
	void SetParent(CMsWebBrowsertestDlg* pParent);
	int GetWidth();
	int GetHeight();	
};
