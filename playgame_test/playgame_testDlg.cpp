#include "pch.h"
#include "framework.h"
#include "playgame_test.h"
#include "playgame_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct _FIND_WINDOW_PARAM
{
	LPCTSTR pszClassName;
	LPCTSTR pszWindowName;
	DWORD dwProcessID;
	HWND hwnd;

} FIND_WINDOW_PARAM, * LPFIND_WINDOW_PARAM;

BOOL CALLBACK FindWindowLikeProc(HWND hwnd, LPARAM lParam)
{
	BOOL bRes = TRUE;
	DWORD dwProcessID = 0;

	LPFIND_WINDOW_PARAM findParam = (LPFIND_WINDOW_PARAM)lParam;

	TCHAR tClass[MAX_PATH] = { 0 };
	::GetClassName(hwnd, tClass, MAX_PATH);

	TCHAR tTitle[MAX_PATH] = { 0 };
	::GetWindowText(hwnd, tTitle, MAX_PATH);

	if (lstrcmpi(tClass, findParam->pszClassName) == 0)
	{
		CString szWindowTitle = tTitle;
		if (szWindowTitle.Find(findParam->pszWindowName) >= 0)
		{
			findParam->hwnd = hwnd;
			GetWindowThreadProcessId(hwnd, &findParam->dwProcessID);
			bRes = FALSE;
		}
	}

	return bRes;
}

HWND FindWindowLikeTitle(LPCTSTR pszClassName, LPCTSTR pszWindowName, LPDWORD pRetProcessID)
{
	HWND hwnd = NULL;
	FIND_WINDOW_PARAM param;
	param.pszClassName = pszClassName;
	param.pszWindowName = pszWindowName;
	param.dwProcessID = 0;
	param.hwnd = NULL;

	::EnumWindows(FindWindowLikeProc, (LPARAM)&param);

	if (pRetProcessID != NULL)
		*pRetProcessID = param.dwProcessID;

	return param.hwnd;
}

CplaygametestDlg::CplaygametestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLAYGAME_TEST_DIALOG, pParent)
{
}

void CplaygametestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CplaygametestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CplaygametestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CplaygametestDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CplaygametestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CplaygametestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	getInifile();

	InitList();

	SetTimer(1, 10000, NULL);
	
	return TRUE;  
}

void CplaygametestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CplaygametestDlg::OnDestroy()
{
	KillTimer(1);

	RemoveAllGameInfo();

	m_GameInfo.clear();

	CDialogEx::OnDestroy();
}

void CplaygametestDlg::OnBnClickedButton1()
{
	Util::runAppplayer();
}

void CplaygametestDlg::OnBnClickedButton2()
{
	setInifile();
}

void CplaygametestDlg::OnTimer(UINT_PTR nIDEvent)
{
	for (int i = 0; i < m_GameInfo.size(); i++)
	{
		DWORD dwProcessID = 0;
		HWND hwnd = NULL;

		hwnd = FindWindowLikeTitle(m_GameInfo[i].strGameClass, m_GameInfo[i].strGameTitle, &dwProcessID);
		if (hwnd)
		{
			UpdateGameInfo(m_GameInfo[i].strGameID, m_GameInfo[i].strGameClass, m_GameInfo[i].strGameTitle, dwProcessID);
		}
		else
		{
			DeleteGameInfo(m_GameInfo[i].strGameID, m_GameInfo[i].strGameClass, m_GameInfo[i].strGameTitle, dwProcessID);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CplaygametestDlg::UpdateGameInfo(CString strGameID, CString strClass, CString strTitle, DWORD dwProcessID)
{
	int nIndex = 0;
	COleDateTime startTm;
	BOOL bFind = FALSE;

	//실행중인 프로세스 확인
	int nCount = mAryGameInfo.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		ST_GAME_INFO* pInfo = (ST_GAME_INFO*)mAryGameInfo.GetAt(i);

		if (pInfo->m_dwProcessID == dwProcessID && pInfo->m_sGameID == strGameID)
		{
			nIndex = i;
			startTm = pInfo->m_startTm;
			bFind = TRUE;
			break;
		}
	}

	//데이터 입력
	COleDateTime tNow = COleDateTime::GetCurrentTime();

	if (bFind)
	{
		ST_GAME_INFO* pInfo = new ST_GAME_INFO;
		pInfo->m_sGameID = strGameID;
		pInfo->m_sClass = strClass;
		pInfo->m_sTitle = strTitle;
		pInfo->m_dwProcessID = dwProcessID;
		pInfo->m_startTm = startTm;
		pInfo->m_EndTm = tNow;
		mAryGameInfo.SetAt(nIndex, (LPVOID)pInfo);

		//리스트에 내용 업데이트
		InsertList(bFind, pInfo->m_sGameID, pInfo->m_sClass, pInfo->m_sTitle, pInfo->m_dwProcessID, pInfo->m_startTm, pInfo->m_EndTm);
	}
	else
	{
		ST_GAME_INFO* pInfo = new ST_GAME_INFO;
		pInfo->m_sGameID = strGameID;
		pInfo->m_sClass = strClass;
		pInfo->m_sTitle = strTitle;
		pInfo->m_dwProcessID = dwProcessID;
		pInfo->m_startTm = tNow;
		pInfo->m_EndTm = tNow;
		mAryGameInfo.Add(pInfo);

		//리스트에 내용 추가
		InsertList(bFind, pInfo->m_sGameID, pInfo->m_sClass, pInfo->m_sTitle, pInfo->m_dwProcessID, pInfo->m_startTm, pInfo->m_EndTm);
	}		
}

void CplaygametestDlg::RemoveAllGameInfo()
{
	int nCount = mAryGameInfo.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		ST_GAME_INFO* pInfo = (ST_GAME_INFO*)mAryGameInfo.GetAt(i);
		if (pInfo)delete pInfo;
		pInfo = NULL;
	}
	mAryGameInfo.RemoveAll();
}

void CplaygametestDlg::DeleteGameInfo(CString strGameID, CString strClass, CString strTitle, DWORD dwProcessID)
{	
	int nCount = mAryGameInfo.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		ST_GAME_INFO* pInfo = (ST_GAME_INFO*)mAryGameInfo.GetAt(i);

		if (pInfo->m_sTitle == strTitle)
		{
			delete pInfo;
			pInfo = NULL;
			mAryGameInfo.RemoveAt(i);

			m_ListCtrl.DeleteItem(i);
			break;
		}
	}
}

void CplaygametestDlg::setInifile()
{
	CString strFilePath;
	strFilePath.Format(_T("%s\\googlegameinfo.ini"), Util::GetModulePath());
		
	//파일 생성	
	CIniFile ini;
	ini.CreateIniFile(strFilePath);
	ini.SetTotalCount(_T("2"));
	ini.SetInfo(_T("1"), _T("벽돌깨기 퀘스트"));
	ini.SetInfo(_T("2"), _T("쿠키런: 킹덤"));
}

void CplaygametestDlg::getInifile()
{
	mAryGameInfo.RemoveAll();

	CString strFilePath;
	CString strIndex;
	CString strTitle;

	strFilePath.Format(_T("%s\\googlegameinfo.ini"), Util::GetModulePath());

	CIniFile ini;
	ini.CreateIniFile(strFilePath);
	int nCount = ini.GetTotalCount();
	for (int i = 1; i <= nCount; i++)
	{
		strIndex.Format(_T("%d"), i);
		strTitle = ini.GetInfo(strIndex);

		GameInfo Info;
		Info.strGameID = strIndex;
		Info.strGameTitle = strTitle;
		Info.strGameClass = _T("KIWICROSVM");
		m_GameInfo.push_back(Info);
	}
}

void CplaygametestDlg::InitList()
{
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.InsertColumn(0, _T("게임ID"), LVCFMT_CENTER, 100, -1);
	m_ListCtrl.InsertColumn(1, _T("게임명"), LVCFMT_LEFT, 240, -1);
	m_ListCtrl.InsertColumn(2, _T("시작시간"), LVCFMT_LEFT, 120, -1);
	m_ListCtrl.InsertColumn(3, _T("업데이트시간"), LVCFMT_LEFT, 120, -1);
	m_ListCtrl.InsertColumn(4, _T("업데이트초"), LVCFMT_LEFT, 120, -1);
}

void CplaygametestDlg::InsertList(BOOL bFind, CString strGameID, CString strClass, CString strTitle, int dwProcessID, COleDateTime startTm, COleDateTime EndTm)
{
	//시작시간
	CString strStartTime;
	strStartTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"), startTm.GetYear(), startTm.GetMonth(), startTm.GetDay(), startTm.GetHour(), startTm.GetMinute(), startTm.GetSecond());

	//최종 실행시간
	CString strEndTime;
	strEndTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"), EndTm.GetYear(), EndTm.GetMonth(), EndTm.GetDay(), EndTm.GetHour(), EndTm.GetMinute(), EndTm.GetSecond());

	//최종 실행시간(초)
	COleDateTimeSpan tm = EndTm - startTm;
	
	CString strTotalSecond;
	strTotalSecond.Format(_T("%d"), (int)tm.GetTotalSeconds());//strTotalSecond.Format(_T("%f"), tm.GetTotalSeconds());

	if (bFind)
	{
		for (int i = 0; i < m_ListCtrl.GetItemCount(); i++)
		{
			if (strTitle == m_ListCtrl.GetItemText(i, 1) )
			{
				m_ListCtrl.SetItemText(i, 0, strGameID);
				m_ListCtrl.SetItemText(i, 1, strTitle);
				m_ListCtrl.SetItemText(i, 2, strStartTime);
				m_ListCtrl.SetItemText(i, 3, strEndTime);
				m_ListCtrl.SetItemText(i, 4, strTotalSecond);
			}
		}
	}
	else
	{
		int nListIndex = m_ListCtrl.GetItemCount();
		m_ListCtrl.InsertItem(nListIndex, strGameID);
		m_ListCtrl.SetItemText(nListIndex, 1, strTitle);
		m_ListCtrl.SetItemText(nListIndex, 2, strStartTime);
		m_ListCtrl.SetItemText(nListIndex, 3, strEndTime);
		m_ListCtrl.SetItemText(nListIndex, 4, strTotalSecond);
	}
}

