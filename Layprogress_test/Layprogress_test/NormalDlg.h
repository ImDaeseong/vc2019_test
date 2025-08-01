#pragma once
#include "afxdialogex.h"
#include "Layprogress_testDlg.h"

class NormalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NormalDlg)

	void SetParent(CLayprogresstestDlg* pParent);
	CLayprogresstestDlg* m_pParent;

public:
	NormalDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~NormalDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NORMAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	CGdiPlusBitmapResource* m_pbgImage;
	void LoadSkin();
	void DrawSkin(CDC* pDC);
	void InitControls();

private:
	CircularProgressBar m_CircularBar;
	CRoundLineProgressBar m_RoundLineBar;
	int m_nCount;

	CMsgStatic m_MsgMessage1;
	CMsgStatic m_MsgMessage2;
};
