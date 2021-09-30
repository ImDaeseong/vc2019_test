#pragma once

#include "Launcher_testDlg.h"

class RightDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RightDlg)

public:
	RightDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~RightDlg();

	void SetParent(CLaunchertestDlg* pParent);
	int  GetWidth();
	int  GetHeight();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RIGHT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();

private:
	CLaunchertestDlg* m_pParent;

	BOOL LoadSkin();
	void DrawSkin(CDC* pDC);
	CGdiPlusBitmapResource* m_pBackSkin;
};
