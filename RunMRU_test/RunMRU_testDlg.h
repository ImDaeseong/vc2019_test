#pragma once

class CRunMRUtestDlg : public CDialogEx
{
public:
	CRunMRUtestDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RUNMRU_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeList1();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

public:	
	FeatureUsage objusage;
	CListBox mlist;
};
