#pragma once

class CgiftestDlg : public CDialogEx
{
public:
	CgiftestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GIF_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	DECLARE_MESSAGE_MAP()

private:
	CGifStatic m_gif;	
};
