#pragma once

class EditDlg;
class CCheckboxtestDlg : public CDialogEx
{
public:
	CCheckboxtestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	EditDlg* m_pEditDlg;
	void CreateEditDlg();
	void DestroyEditDlg();
	void SetChildPos();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECKBOX_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedRemoveButton();
	afx_msg void OnBnClickedCancelButton();
	DECLARE_MESSAGE_MAP()

private:
	BOOL LoadSkin();
	void DrawSkin(CDC* pDC);
	void InitControls();

	CGdiPlusBitmapResource* m_pBackSkin;

public:
	CCheckButton m_btnCheck1;
	CCheckButton m_btnCheck2;
	CCheckStatic m_StaticCheck1;	
	CCheckStatic m_StaticCheck2;

	CSkinButton m_btnRemove;
	CSkinButton m_btnCancel;

	void SetCheckIndex(int nIndex);	
};
