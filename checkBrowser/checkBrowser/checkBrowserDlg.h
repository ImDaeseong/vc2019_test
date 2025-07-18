#pragma once

class CcheckBrowserDlg : public CDialogEx
{
public:
	CcheckBrowserDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECKBROWSER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	void FindBrowserPIDs();
	void FindBrowserPIDsTime();
	DWORD FindBrowserPIDsLast();

	UINT_PTR m_timer;

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
