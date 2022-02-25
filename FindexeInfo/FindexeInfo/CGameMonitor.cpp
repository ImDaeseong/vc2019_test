#include "pch.h"
#include "CGameMonitor.h"
#include "FindexeInfoDlg.h"

static bool m_bcheckThread = false;

void CGameMonitor::insertGameInfo(int nIndex, CString strExename)
{
	ST_PCGAMELIST_DATA* pData = new ST_PCGAMELIST_DATA;
	pData->m_nIndex = nIndex;
	pData->m_strExename = strExename;
	pData->m_nInstalled = 0;
	pData->m_strFullPath = "";
	m_AryList.Add(pData);
}

void CGameMonitor::findGameInfo(CString strExename, CString szFullPath)
{
	int nCount = (int)m_AryList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		ST_PCGAMELIST_DATA* pData = (ST_PCGAMELIST_DATA*)m_AryList.GetAt(i);

		if (lstrcmpi(pData->m_strExename, strExename) == 0)
		{
			int nExist = _waccess(szFullPath, 0);
			if (nExist != -1)
			{
				pData->m_nInstalled = 1;
				pData->m_strFullPath = szFullPath;
				break;
			}
		}
	}
}

void CGameMonitor::removeAllGameInfo()
{
	int nCount = (int)m_AryList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		ST_PCGAMELIST_DATA* pData = (ST_PCGAMELIST_DATA*)m_AryList.GetAt(i);
		if (pData)delete pData;
		pData = NULL;
	}
	m_AryList.RemoveAll();
}

CString CGameMonitor::findResult()
{
	CString strResult = _T("");

	int nCount = (int)m_AryList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		ST_PCGAMELIST_DATA* pData = (ST_PCGAMELIST_DATA*)m_AryList.GetAt(i);
		CString strTemp = _T("");

		if (i == (nCount - 1))
			strTemp.Format(_T("[%d] %s (%s):%d"), pData->m_nIndex, pData->m_strExename, pData->m_strFullPath, pData->m_nInstalled);
		else
			strTemp.Format(_T("[%d] %s (%s):%d|"), pData->m_nIndex, pData->m_strExename, pData->m_strFullPath, pData->m_nInstalled);

		strResult += strTemp;
	}

	return strResult;
}

void CGameMonitor::searchDirectories(CString strDirectory, int nDepth)
{
	CString strFilter = strDirectory + _T("\\*.*");

	double start = 0;
	double stop = 0;

	CFileFind finder;
	BOOL bSearch = finder.FindFile(strFilter, 0);

	while (bSearch)
	{
		bSearch = finder.FindNextFile();

		if (finder.IsDots() || finder.IsSystem() || finder.IsTemporary())//if (finder.IsDots() || finder.IsSystem() || finder.IsHidden() || finder.IsTemporary())
			continue;

		int nPercent = Searchfilegame::getProcesscpu();
		if (nPercent > 4)
		{
			Sleep(100);
		}

		if (finder.IsDirectory())
		{
			//nDepth가 4 보다 작은 깊이의 폴더만 검사  
			if (nDepth > 4)
				continue;

			CString szFolderPath = finder.GetFilePath();
			searchDirectories(szFolderPath, nDepth + 1);
		}
		else
		{
			CString szFileName = finder.GetFileName();
			CString szFullPath = finder.GetFilePath();
			findGameInfo(szFileName, szFullPath);
		}
	}
	finder.Close();
}

CGameMonitor::CGameMonitor()
{
	m_hKillEvent = NULL;
	m_hThread = NULL;
	m_dwThreadId = 0;
	removeAllGameInfo();
	drivelist.RemoveAll();
}

CGameMonitor::~CGameMonitor()
{
	if (m_hKillEvent)
		CloseHandle(m_hKillEvent);

	if (m_hThread)
		CloseHandle(m_hThread);

	m_dwThreadId = 0;
	removeAllGameInfo();
	drivelist.RemoveAll();
}

void CGameMonitor::SetParent(CFindexeInfoDlg* pParent)
{
	m_pParent = pParent;
}

BOOL CGameMonitor::StartGameMonitor()
{
	if (m_bcheckThread)
	{
		//OutputDebugString(_T("스레드 구동중"));
		return TRUE;
	}

	m_bcheckThread = true;
	m_hKillEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hThread = CreateThread(NULL, 0, GameMonitorThread, this, 0, &m_dwThreadId);
	if (m_hThread == NULL)
	{
		//OutputDebugString(_T("스레드 구동 실패"));
		m_bcheckThread = false;
		return FALSE;
	}
	return TRUE;
}

BOOL CGameMonitor::StopGameMonitor()
{
	m_bcheckThread = false;
	DWORD dwExitCode;
	if (m_hThread != NULL && ::GetExitCodeThread(m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
	{
		::SetEvent(m_hKillEvent);

		::WaitForSingleObject(m_hThread, INFINITE);

		m_hThread = NULL;

		m_hKillEvent = NULL;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

DWORD CGameMonitor::GameMonitorThread(LPVOID pParam)
{
	CGameMonitor* pThis = (CGameMonitor*)pParam;
	while (m_bcheckThread != false)
	{
		if (::WaitForSingleObject(pThis->m_hKillEvent, 1000) == WAIT_OBJECT_0)
		{
			m_bcheckThread = false;
			break;
		}
		else
		{
			//파일 검색
			pThis->MonitorGamePrograms();
		}
	}
	return 1;
}

UINT CGameMonitor::MonitorGamePrograms()
{
	//시작시간
	//CTime tStart = CTime::GetCurrentTime();


	//존재하는 드라이브 조사
	Searchfilegame::getDriveList(drivelist);

	//존재하는 드라이브만큼 폴더 조사
	int nSize = drivelist.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strRootDir = drivelist.GetAt(i);
		searchDirectories(strRootDir, 0);
	}

	//완료 시간
	//CTime tEnd = CTime::GetCurrentTime();
	//CTimeSpan tResult = tEnd - tStart;
	//OutputDebugString(tResult.Format("%M:%S\n"));

	//메인으로 결과 전달
	//m_pParent->ResultGameThread();

	//메인으로 결과 전달 - SendMessageTimeout
	DWORD dwResult;
	if (0 == ::SendMessageTimeout(m_pParent->m_hWnd, WM_GAMEINFO_RESULT, 0, 0, SMTO_NORMAL, 3000, (PDWORD_PTR)&dwResult))
	{
		int nLastErr = GetLastError();
		if (ERROR_TIMEOUT == nLastErr)
		{
		}
	}

	return 0;
}

void CGameMonitor::AddGameInfo(int nIndex, CString strExename)
{
	insertGameInfo(nIndex, strExename);
}