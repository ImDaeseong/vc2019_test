#pragma once

#include "SearchThread.h"

class CRecurseDirectoriestestDlg : public CDialogEx
{
public:
	CRecurseDirectoriestestDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECURSEDIRECTORIES_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeList1();
	afx_msg void OnSelchangeList2();
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

public:
	CListBox mlist1;
	CListBox mlist2;

	CString GetFilePath(CString strFilename);
	void getDriveList(CStringArray& drivelist);
	void RecurseDirectories(CStringArray& folderList, CStringArray& fileList, CString strDirectory, int nDepth);

	
	SearchThread* GetThread() { return m_pThread; }
	void SetThread() { m_pThread = NULL; }
	
	void StartThread();
	void CloseThread();
	void runThread();
	SearchThread* m_pThread;
};
