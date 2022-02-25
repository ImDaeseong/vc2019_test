#include "pch.h"
#include "framework.h"
#include "FindexeInfo.h"
#include "FindexeInfoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFindexeInfoDlg::CFindexeInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINDEXEINFO_DIALOG, pParent)
{
	m_pGameMonitor = NULL;
}

void CFindexeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFindexeInfoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_GAMEINFO_RESULT, OnResultGame)
END_MESSAGE_MAP()

BOOL CFindexeInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFindexeInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MoveWindow(-3000, -3000, 0, 0);

	initGameThread();

	return TRUE;  
}

void CFindexeInfoDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CFindexeInfoDlg::OnDestroy()
{
	StopGameThread();

	CDialog::OnDestroy();
}

void CFindexeInfoDlg::initGameThread()
{
	if (m_pGameMonitor)
	{
		m_pGameMonitor->StopGameMonitor();
		delete m_pGameMonitor;
		m_pGameMonitor = NULL;
	}

	m_pGameMonitor = new CGameMonitor;
	m_pGameMonitor->SetParent(this);

	//검색할 항목
	m_pGameMonitor->AddGameInfo(1, _T("lostark.exe"));
	m_pGameMonitor->AddGameInfo(2, _T("LeagueClient.exe"));
	m_pGameMonitor->AddGameInfo(3, _T("VALORANT.exe"));
	
	//검색 시작
	if (!m_pGameMonitor->StartGameMonitor())
	{
		m_pGameMonitor->StopGameMonitor();
		delete m_pGameMonitor;
		m_pGameMonitor = NULL;
	}
}

void CFindexeInfoDlg::StopGameThread()
{
	if (m_pGameMonitor)
	{
		m_pGameMonitor->StopGameMonitor();
		delete m_pGameMonitor;
		m_pGameMonitor = NULL;
	}
}

void CFindexeInfoDlg::ResultGameThread()
{
	OutputDebugString(m_pGameMonitor->findResult());

	//AfxMessageBox(m_pGameMonitor->findResult());

	exit(1);

	CDialog::OnOK();
}

LRESULT CFindexeInfoDlg::OnResultGame(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(m_pGameMonitor->findResult());

	//AfxMessageBox(m_pGameMonitor->findResult());

	exit(1);

	CDialog::OnOK();

	return 0L;
}