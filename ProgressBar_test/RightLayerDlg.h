#pragma once
#include "afxdialogex.h"
#include "ProgressBar_testDlg.h"

#define SAFE_DELETEDC( hdc ) if( hdc ){ ::DeleteDC( hdc ); hdc = NULL; }
#define SAFE_DELETEOBJECT( hobj ) if( hobj ){ ::DeleteObject( hobj ); hobj = NULL; }


class RightLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RightLayerDlg)

public:
	RightLayerDlg(CWnd* pParent = nullptr);   
	virtual ~RightLayerDlg();

	void SetParent(CProgressBartestDlg* pParent);
	CProgressBartestDlg* m_pParent;

	int m_nWidth;
	int m_nHeight;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RIGHT_DIALOG };
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
	void DrawSkin();
	void InitControls();	

	int      m_nCount;

public:
	CSkinButtonLayer m_Btnclose;	
	ProgressbarPngStatic m_StcProgressPng;
	ProgressbarPngStatic m_StcProgressPngA;
	ProgressbarStatic m_StcProgress;
	CMsgStatic m_StcMsg;
};
