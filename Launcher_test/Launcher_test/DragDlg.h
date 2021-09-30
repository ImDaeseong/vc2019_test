#pragma once

#include "Launcher_testDlg.h"

class DragDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DragDlg)

public:
	DragDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DragDlg();

	void SetParent(CLaunchertestDlg* pParent);
	int  GetWidth();
	int  GetHeight();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT DragControl(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();

public:
	DragStatic m_ctlDrag1;
	DragStatic m_ctlDrag2;
	DragStatic m_ctlDrag3;
	DragStatic m_ctlDrag4;
	CPictureStatic m_ctlCurosr;

private:
	CLaunchertestDlg* m_pParent;

	BOOL LoadSkin();
	void DrawSkin(CDC* pDC);
	CGdiPlusBitmapResource* m_pBackSkin;
};
