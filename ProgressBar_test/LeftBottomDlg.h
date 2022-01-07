#pragma once
#include "afxdialogex.h"
#include "ProgressBar_testDlg.h"

class LeftBottomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LeftBottomDlg)

public:
	LeftBottomDlg(CWnd* pParent = nullptr);   
	virtual ~LeftBottomDlg();

	void SetParent(CProgressBartestDlg* pParent);
	CProgressBartestDlg* m_pParent;

	int m_nWidth;
	int m_nHeight;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEFT_BT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	void InitControls();

	int      m_nCount;

public:
	ProgressbarPngStatic m_StcProgressPng;
	ProgressbarPngStatic m_StcProgressPngA;
	ProgressbarStatic m_StcProgress;
	CMsgStatic m_StcMsg;
};
