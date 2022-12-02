#pragma once

struct ExtensionInfo
{
	CString strItem;
	CString strDisplayName;
};

class CFindBrowserExtensionsDlg : public CDialogEx
{
public:
	CFindBrowserExtensionsDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINDBROWSEREXTENSIONS_DIALOG };
#endif

    public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:

	void InitPath();
	CString GetModulePath();
	CString GetFolderName(CString strFolderName);
	void searchExtension(CString strDirectory, BOOL bEdge = FALSE);
	void searchExtensionFireFox(CString strDirectory, BOOL bFirefox = FALSE);
	CString strChromeExtension;
	CString strEdgeExtension;
	CString strFirefoxExtension;

	std::vector<ExtensionInfo> m_Extensions;
	void AddExtensionInfo(CString strItem, CString strDisplayName);
	void ClearExtensionInfo();

	void InitCtrl();
	void InsertListView(CString strItem, CString strDisplayName);
	void ViewListInfo();
	CListCtrl m_ListCtrl;	
};
