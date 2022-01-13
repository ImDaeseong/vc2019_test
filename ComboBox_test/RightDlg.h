#pragma once
#include "afxdialogex.h"
#include "ComboBox_testDlg.h"

#define SAFE_DELETEDC( hdc ) if( hdc ){ ::DeleteDC( hdc ); hdc = NULL; }
#define SAFE_DELETEOBJECT( hobj ) if( hobj ){ ::DeleteObject( hobj ); hobj = NULL; }

class RightDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RightDlg)

public:
	RightDlg(CWnd* pParent = nullptr);  
	virtual ~RightDlg();

	void SetParent(CComboBoxtestDlg* pParent);
	CComboBoxtestDlg* m_pParent;

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

private:
	CGdiPlusBitmapResource* m_pbgImage;

	void LoadImage();
	void DrawSkin();
	void InitControls();
	void InitData();

public:

	ComboBoxEx m_cbo1;
	ComboBoxEx m_cbo2;
	CComboBox m_cbo3;
	CComboBox m_cbo4;
};
