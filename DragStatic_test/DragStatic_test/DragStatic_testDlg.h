#pragma once

class CDragStatictestDlg : public CDialogEx
{
public:
	CDragStatictestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAGSTATIC_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT DragControl(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClose();

public:
	CSkinButton m_btnClose;

	DragStatic m_ctlDrag1;
	DragStatic m_ctlDrag2;
	DragStatic m_ctlDrag3;
	DragStatic m_ctlDrag4;
	CPictureStatic m_ctlCurosr;

	void SetMoveWindow(BOOL bInWindow);

private:
	BOOL LoadSkin();
	void DrawSkin(CDC* pDC);
	void InitControls();
	CGdiPlusBitmapResource* m_pBackSkin;

	int nStart1;
	int nStart2;
	int nStart3;
	int nStart4;
	int nSelectIndex;

	void swap(int* start, int* end);
	void Num1(int nStart1);
	void Num2(int nStart2);
	void Num3(int nStart3);
	void Num4(int nStart4);
};
