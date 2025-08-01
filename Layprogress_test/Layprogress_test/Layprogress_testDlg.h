#pragma once

class LayerDlg;
class NormalDlg;
class CLayprogresstestDlg : public CDialogEx
{
public:
	CLayprogresstestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	LayerDlg* m_pLayerDlg;
	void CreateLayerDlg();
	void DestroyLayerDlg();

	NormalDlg* m_pNormalDlg;
	void CreateNormalDlg();
	void DestroyNormalDlg();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAYPROGRESS_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	DECLARE_MESSAGE_MAP()
};
