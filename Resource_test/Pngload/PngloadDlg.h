#pragma once

class CPngloadDlg : public CDialogEx
{
public:
	CPngloadDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PNGLOAD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClose();
	DECLARE_MESSAGE_MAP()

public:
	CSkinButton m_btnClose;

private:
	BOOL LoadSkin();
	void DrawSkin(CDC* pDC);
	void InitControls();

	CGdiPlusBitmapResource* m_pBackSkin;	
};
