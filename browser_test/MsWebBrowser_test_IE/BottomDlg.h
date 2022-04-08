#pragma once
#include "afxdialogex.h"
#include "MsWebBrowser_testDlg.h"

class BottomDlg : public CDialog
{
	DECLARE_DYNAMIC(BottomDlg)

public:
	BottomDlg(CWnd* pParent = nullptr);   
	virtual ~BottomDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOTTOM_DIALOG };
#endif

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	CPngStatic m_PngNumF;
	CPngStatic m_PngNumE;
	CPngStatic m_PngNumBG;

private:
	CGdiPlusBitmapResource* m_pbgImage;
	void LoadImage();
	void DrawSkin();

	CMsWebBrowsertestDlg* m_pParent;

	int m_nCount;

private:
	void DrawSprite(Graphics& gps);
	Gdiplus::Bitmap* m_pSprite;
	int m_curIndex;
	int m_TotalIndex;

public:
	void SetParent(CMsWebBrowsertestDlg* pParent);
	int GetWidth();
	int GetHeight();
	void SetStartTimer(int nCount);		
};
