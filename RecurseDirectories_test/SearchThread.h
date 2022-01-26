#pragma once

class CRecurseDirectoriestestDlg;
class SearchThread : public CWinThread
{
	DECLARE_DYNCREATE(SearchThread)

public:
	SearchThread();
	virtual ~SearchThread();

protected:
	CWnd* m_pWnd;
	CRecurseDirectoriestestDlg* m_pParentDlg;
public:
	void SetThread(CWnd* pWnd) { m_pWnd = pWnd; }
	void ExitThread();
	void RunProcess();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};

