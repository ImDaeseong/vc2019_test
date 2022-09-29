#pragma once

struct UnInstallInfo
{
	CString strkey;
	CString strDisplayName;
	CString strUninstallString;
};

class CuninstallInfotestDlg : public CDialogEx
{
public:
	CuninstallInfotestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNINSTALLINFO_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	std::vector<UnInstallInfo> m_Info;

	void AddInfo(CString strkey, CString strDisplayName, CString strUninstallString);
	void ClearInfo();
	void searchRegInfo();
	CString searchSubRegInfo(CString strSubkey);
	void SearchInfoList();
	
	void InitCtrl();
	void InsertListView(CString strkey, CString strDisplayName, CString strUninstallString);
	CListCtrl m_ListCtrl;	
};
