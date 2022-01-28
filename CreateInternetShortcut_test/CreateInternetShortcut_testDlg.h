#pragma once


struct SHORTSYMBOL
{
	CString strName;
	int nIndex;
};

class CCreateInternetShortcuttestDlg : public CDialogEx
{
public:
	CCreateInternetShortcuttestDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATEINTERNETSHORTCUT_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	BOOL UnzipResource();
	void CreateDesktopIcon();
};
