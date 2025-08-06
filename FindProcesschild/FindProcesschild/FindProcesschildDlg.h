#pragma once
#include "Synchronized.h"

class CFindProcesschildDlg : public CDialogEx
{
public:
	CFindProcesschildDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINDPROCESSCHILD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

	UINT_PTR m_timer;
	static CRITICAL_SECTION m_Log;

	CListCtrl m_ListLog;
	void initListCtrl();	

public:
	void FindWindowTitles();

	void setListCtrlData(CString strTitle);

	void WriteLogFile(CString strFunc, TCHAR* szBuffer);
	void WriteLog(CString strFunc, LPCTSTR szFmt, ...);
	CString GetFileName(int nDay);
	CString GetModulePath();
};
