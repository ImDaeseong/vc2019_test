#pragma once

class RightLayerDlg;
class LeftLayerDlg;
class RightDlg;

class CProgressBartestDlg : public CDialogEx
{
public:
	CProgressBartestDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESSBAR_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonClick();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CGdiPlusBitmapResource* m_pbgImage;

	void InitControls();
	void LoadImage();
	void DrawSkin(CDC* pDC);	

public:
	CSkinButtonNormal m_Btnclose;
	CSkinButtonNormal m_Btnclick;
	CMsgStatic m_StcMsg;

	RightLayerDlg* m_pRightLayerDlg;
	void CreateRightLayerDlg();
	void DestroyRightLayerDlg();

	LeftLayerDlg* m_pLeftLayerDlg;
	void CreateLeftLayerDlg();
	void DestroyLeftLayerDlg();

	RightDlg* m_pRightDlg;
	void CreateRightDlg();
	void DestroyRightDlg();

	void SetChildPos();
	void MainCallFunc();
};
