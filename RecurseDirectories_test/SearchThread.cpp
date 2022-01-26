#include "pch.h"
#include "SearchThread.h"
#include "RecurseDirectories_testDlg.h"

IMPLEMENT_DYNCREATE(SearchThread, CWinThread)

SearchThread::SearchThread()
{
	m_pParentDlg = NULL;
}

SearchThread::~SearchThread()
{
}

BOOL SearchThread::InitInstance()
{
	RunProcess();
	return TRUE;
}

int SearchThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(SearchThread, CWinThread)
END_MESSAGE_MAP()

void SearchThread::ExitThread()
{
	DWORD exitCode;
	if (m_pParentDlg->GetThread())
	{
		GetExitCodeThread(m_hThread, &exitCode);

		if (exitCode == STILL_ACTIVE)
		{
			TerminateThread(m_hThread, 0);
		}
		delete m_pParentDlg->GetThread();

		m_pParentDlg->SetThread();
	}
}

void SearchThread::RunProcess()
{
	m_pParentDlg = (CRecurseDirectoriestestDlg*)m_pWnd;
	m_pParentDlg->runThread();
	ExitThread();
}
