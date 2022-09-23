#pragma once

struct GameInfo
{
	CString strGameID;
	CString strGameTitle;
	CString strGameClass;
};

typedef struct _Game_info_
{
	CString m_sGameID;//게임 ID
	CString m_sTitle;//게임 타이틀
	CString m_sClass;//게임 클레스	
	DWORD m_dwProcessID;//게임프로세스
	COleDateTime m_startTm;//시작시간	
	COleDateTime m_EndTm;//종료시간

} ST_GAME_INFO, * LPST_GAME_INFO;


class CplaygametestDlg : public CDialogEx
{
public:
	CplaygametestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLAYGAME_TEST_DIALOG };
#endif

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	DECLARE_MESSAGE_MAP()

private:
	CListCtrl m_ListCtrl;

	void InitList();
	void InsertList(BOOL bFind, CString strGameID, CString strClass, CString strTitle, int dwProcessID, COleDateTime startTm, COleDateTime EndTm);

public:
	void UpdateGameInfo(CString strGameID, CString strClass, CString strTitle,  DWORD dwProcessID);
	void RemoveAllGameInfo();
	void DeleteGameInfo(CString strGameID, CString strClass, CString strTitle, DWORD dwProcessID);

private:
	std::vector<GameInfo> m_GameInfo;
	
	CPtrArray mAryGameInfo;
	void setInifile();
	void getInifile();	
};
