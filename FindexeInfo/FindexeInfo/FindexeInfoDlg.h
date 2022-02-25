#pragma once

class CFindexeInfoDlg : public CDialogEx
{
public:
	CFindexeInfoDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINDEXEINFO_DIALOG };
#endif
	
    public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnResultGame(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	CGameMonitor* m_pGameMonitor;
	void initGameThread();
	void StopGameThread();
	void ResultGameThread();
};
