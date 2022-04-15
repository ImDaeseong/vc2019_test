#pragma once

class CvcdefinetestDlg : public CDialogEx
{
public:
	CvcdefinetestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VCDEFINE_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
