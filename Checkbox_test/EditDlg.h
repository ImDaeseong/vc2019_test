#pragma once

class CCheckboxtestDlg;
class EditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EditDlg)

public:
	EditDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~EditDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

	void SetParent(CCheckboxtestDlg* pParent);
	CCheckboxtestDlg* m_pParent;
	CString GetEditText();

	void InitControls();
	CEditEx m_EdtText;
    CFont   m_FontText;
};
