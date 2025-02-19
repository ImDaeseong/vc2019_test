#pragma once

class CWindowsSettingsURItestDlg : public CDialogEx
{
public:
	CWindowsSettingsURItestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWSSETTINGSURI_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()	
};
