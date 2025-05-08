#include "pch.h"
#include "framework.h"
#include "LoL_test.h"
#include "LoL_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLoLtestDlg::CLoLtestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOL_TEST_DIALOG, pParent)
{
	m_lolManager = NULL;
}

void CLoLtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLoLtestDlg, CDialogEx)
	ON_WM_PAINT()	
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CLoLtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_lolManager = new CLoLManager();

	SetTimer(1, 10000, NULL);	

	return TRUE;  
}

void CLoLtestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);

	if (m_lolManager)
	{
		delete m_lolManager;
		m_lolManager = NULL;
	}
}

void CLoLtestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CLoLtestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		LoLGameInfo item = m_lolManager->GetDataInfo();
		CString strGameId = item.strGameId;
		CString strGameName = item.strGameName;
		CString strGameTag = item.strGameTag;
		OutputDebugString(strGameId + CString("\r\n"));
	}

	CDialogEx::OnTimer(nIDEvent);
}
