#pragma once

struct InstallInfo
{
	CString strkey;
	CString strDisplayName;
};

class CsearchInstallinfotestDlg : public CDialogEx
{
public:
	CsearchInstallinfotestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCHINSTALLINFO_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

private:
	std::vector<InstallInfo> m_SetupInfo;

	void AddSetupInfo(CString strkey, CString strDisplayName);

	void ClearSetupInfo();
	void searchReg(BOOL bWOW);
	void GetSetupInfoList();

	void InitCtrl();
	void InsertListView(CString strkey, CString strDisplayName);
	CListCtrl m_ListCtrl;
};
