#pragma once
#include "EdgeWebBrowser.h"

class CTraytestDlg : public CDialogEx
{
public:
	CTraytestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAY_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMove(int x, int y);
	DECLARE_MESSAGE_MAP()

private:
	CGdiPlusBitmapResource* m_pbgImage;
	void LoadImage();
	void DrawSkin(CDC* pDC);
	int m_nWidth;
	int m_nHeight;

	void BringToForeground();
	void ShowTray();
	void HideTray();
		
	void InitWebBrowser();
	void ResizeWebBrowser();
	std::unique_ptr<CWebBrowser> m_pWebBrowser{};		
};
