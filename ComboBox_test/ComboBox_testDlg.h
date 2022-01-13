#pragma once

class LeftDlg;
class RightDlg;

class CComboBoxtestDlg : public CDialogEx
{
public:
	CComboBoxtestDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMBOBOX_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonClick();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	void InitControls();
	void InitData();

	LeftDlg* m_pLeftDlg;
	void CreateLeftDlg();
	void DestroyLeftDlg();

	RightDlg* m_pRightDlg;
	void CreateRightDlg();
	void DestroyRightDlg();

	void SetChildPos();

public:
	CSkinButtonNormal m_Btnclick;
		
	ComboBoxEx m_cbo1;
	ComboBoxEx m_cbo2;
	CComboBox m_cbo3;
	CComboBox m_cbo4;
};
