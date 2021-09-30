#pragma once

#define TIMER_ID_LOCATION_CHANGE 1
#define TIMER_ID_LEFTSLIDE_OPEN 2
#define TIMER_ID_LEFTSLIDE_CLOSE 3
#define TIMER_ID_RIGHTSLIDE_OPEN 4
#define TIMER_ID_RIGHTSLIDE_CLOSE 5

class DragDlg;
class LeftDlg;
class RightDlg;
class CLaunchertestDlg : public CDialogEx
{
public:
	CLaunchertestDlg(CWnd* pParent = nullptr);	// standard constructor

	DragDlg* m_pDragDlg;
	void CreateDragDlg();
	void DestroyDragDlg();

	LeftDlg* m_pLeftDlg;
	void CreateLeftDlg();
	void DestroyLeftDlg();

	RightDlg* m_pRightDlg;
	void CreateRightDlg();
	void DestroyRightDlg();

	void SetChildPos();
	void SetLaunchersub();

	void slideLeftOpen();
	void slideLeftClose();
	void slideRightOpen();
	void slideRightClose();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAUNCHER_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClickedButtonClose();

public:
	CSkinButton  m_btnClose;
	CPictureStatic  m_Picturestatic;

private:
	BOOL LoadSkin();
	void DrawSkin(CDC* pDC);	
	void InitControls();
	int MonitorWidth();
	int MonitorHeight();
	void MoveLocationDialog();
	void changeLocationDialog();

	CGdiPlusBitmapResource* m_pBackSkin;			

	BOOL m_bSlideStyle;
	BOOL m_bLocation;	

	int nWidth;
	int nX;
	int nLeft;
	int nTop;	
};
