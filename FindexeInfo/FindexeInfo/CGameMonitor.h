#pragma once

typedef struct _PcGameList_Data_
{
	int m_nIndex;
	CString m_strExename;
	int m_nInstalled;
	CString m_strFullPath;
} ST_PCGAMELIST_DATA, * LPST_PCGAMELIST_DATA;

class CFindexeInfoDlg;
class CGameMonitor
{
public:
	CGameMonitor(void);
	virtual ~CGameMonitor(void);

protected:
	HANDLE m_hKillEvent;
	HANDLE m_hThread;
	DWORD m_dwThreadId;

	static DWORD WINAPI GameMonitorThread(LPVOID pParam);

public:
	void SetParent(CFindexeInfoDlg* pParent);	
	BOOL StartGameMonitor();
	BOOL StopGameMonitor();	
	UINT MonitorGamePrograms();
	void AddGameInfo(int nIndex, CString strExename);
	CString findResult();

private:
	void insertGameInfo(int nIndex, CString strExename);
	void findGameInfo(CString strExename, CString szFullPath);
	void removeAllGameInfo();	
	void searchDirectories(CString strDirectory, int nDepth);

	CStringArray drivelist;
	CPtrArray m_AryList;
	CFindexeInfoDlg* m_pParent;
};

