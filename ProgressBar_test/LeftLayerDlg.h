#pragma once
#include "afxdialogex.h"
#include "ProgressBar_testDlg.h"

class LeftLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LeftLayerDlg)

public:
	LeftLayerDlg(CWnd* pParent = nullptr);   
	virtual ~LeftLayerDlg();

	void SetParent(CProgressBartestDlg* pParent);
	CProgressBartestDlg* m_pParent;

	int m_nWidth;
	int m_nHeight;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEFT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonClose();

private:
	CGdiPlusBitmapResource* m_pbgImage;

	void LoadImage();
	void DrawSkin(CDC* pDC);
	void InitControls();

	int      m_nCount;

public:	
	CSkinButtonLayer m_Btnclose;	
	NumberStatic m_StcNum0;
	NumberStatic m_StcNum1;
	CMsgStatic m_StcMsg;
};
