#pragma once
#include "CEXPLORER1.h"

class COverlayWndTestDlg : public CDialogEx
{
public:
	COverlayWndTestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OVERLAYWNDTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	DECLARE_MESSAGE_MAP()

public:
	void setBrowserMode(BOOL bShow);

	CEXPLORER1 m_browser;
	COverlayWnd* m_overWnd;
};
