#include "pch.h"
#include "framework.h"
#include "FindProcesschild.h"
#include "FindProcesschildDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRITICAL_SECTION CFindProcesschildDlg::m_Log;

struct EnumParam
{
	DWORD pid;
	CFindProcesschildDlg* pDlg;
};

void SearchDialogWindows(HWND hWnd, EnumParam* param)
{
	DWORD pid = 0;
	GetWindowThreadProcessId(hWnd, &pid);

	if (pid == param->pid)
	{
		TCHAR className[256] = { 0 };
		GetClassName(hWnd, className, _countof(className));

		if (_tcscmp(className, _T("#32770")) == 0)
		{
			TCHAR title[256] = { 0 };
			GetWindowText(hWnd, title, _countof(title));

			CString strMsg;
			if (_tcslen(title) > 0)
			{
				strMsg.Format(_T("다이얼로그 타이틀: %s"), title);
			}
			else
			{
				strMsg = _T("다이얼로그 타이틀: 없음");
			}

			param->pDlg->setListCtrlData(strMsg);
			param->pDlg->WriteLog(_T("윈도우 탐색"), _T("%s"), strMsg);
		}
	}

	//자식 윈도우 재귀 탐색
	EnumChildWindows(hWnd, [](HWND childHwnd, LPARAM lParam) -> BOOL {		
		SearchDialogWindows(childHwnd, reinterpret_cast<EnumParam*>(lParam));		
		return TRUE;		
		}, reinterpret_cast<LPARAM>(param));
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	//현재창 및 자식창 모두 탐색
	EnumParam* param = reinterpret_cast<EnumParam*>(lParam);
	SearchDialogWindows(hWnd, param);
	return TRUE;
}

DWORD GetProcessIdByName(LPCTSTR processName)
{
	DWORD pid = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe = { 0 };
		pe.dwSize = sizeof(pe);

		if (Process32First(hSnapshot, &pe))
		{
			do
			{
				if (_tcsicmp(pe.szExeFile, processName) == 0)
				{
					pid = pe.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnapshot, &pe));
		}
		CloseHandle(hSnapshot);
	}
	return pid;
}

void CFindProcesschildDlg::FindWindowTitles()
{
	DWORD pid = GetProcessIdByName(_T("원도우프로그램.exe"));
	if (pid == 0) return;

	EnumParam param;
	param.pid = pid;
	param.pDlg = this;
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&param));
}

CFindProcesschildDlg::CFindProcesschildDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINDPROCESSCHILD_DIALOG, pParent)
{
	::InitializeCriticalSection(&m_Log);
}

void CFindProcesschildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListLog);
}

BEGIN_MESSAGE_MAP(CFindProcesschildDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CFindProcesschildDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	initListCtrl();

	CRect rc;
	GetWindowRect(&rc);

	int nScreenSizeX = GetSystemMetrics(SM_CXSCREEN);
	int nScreenSizeY = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow((nScreenSizeX - rc.Width()), (nScreenSizeY - rc.Height()), rc.Width(), rc.Height());

	//m_timer = SetTimer(1, 2000, NULL);

	return TRUE; 
}

void CFindProcesschildDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CFindProcesschildDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_timer != 0)
	{
		KillTimer(m_timer);
		m_timer = 0;
	}

	::DeleteCriticalSection(&m_Log);
}

void CFindProcesschildDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		FindWindowTitles();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CFindProcesschildDlg::initListCtrl()
{
	m_ListLog.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListLog.InsertColumn(0, _T("시간"), LVCFMT_LEFT, 80);
	m_ListLog.InsertColumn(1, _T("메시지"), LVCFMT_LEFT, 400);
}

void CFindProcesschildDlg::setListCtrlData(CString strTitle)
{
	CTime tTime = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%.2d:%.2d:%.2d"), tTime.GetHour(), tTime.GetMinute(), tTime.GetSecond());

	int nIndex = m_ListLog.InsertItem(m_ListLog.GetItemCount(), strTime);
	m_ListLog.SetItemText(nIndex, 1, strTitle);  
	
	m_ListLog.EnsureVisible(nIndex, FALSE);
}

void CFindProcesschildDlg::WriteLog(CString strFunc, LPCTSTR szFmt, ...)
{
	Synchronized lock(m_Log);

	TCHAR szBuffer[4096] = { 0 };
	va_list marker;
	va_start(marker, szFmt);
	_vsnwprintf(szBuffer, 4096 - 1, szFmt, marker); 
	va_end(marker);

	WriteLogFile(strFunc, szBuffer);
}

void CFindProcesschildDlg::WriteLogFile(CString strFunc, TCHAR* szBuffer)
{
	CTime tTime = CTime::GetCurrentTime();
	CString strFile = GetFileName(tTime.GetDay());

	static int nHour = -1;

	if (nHour != tTime.GetHour())
	{
		CTimeSpan tTimeSpan(1, 0, 0, 0);
		CString strExpiredFile = GetFileName((tTime + tTimeSpan).GetDay());

		if (_waccess(strExpiredFile, 0) != -1)
		{
			TRY
			{
				CFile::Remove(strExpiredFile);
			}
			CATCH(CFileException, e)
			{
			}
			END_CATCH
		}
		nHour = tTime.GetHour();
	}

	CFile file;
	CFileException ex;

	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite, &ex))
	{
		return;
	}

	file.SeekToEnd();

	CString logLine;
	logLine.Format(_T("%.2d:%.2d:%.2d - %s : %s\r\n"),
		tTime.GetHour(), tTime.GetMinute(), tTime.GetSecond(), strFunc, szBuffer);

	if (file.GetLength() == 0)
	{
		USHORT bom = 0xfeff;
		file.Write(&bom, sizeof(bom));
	}

	file.Write(logLine, logLine.GetLength() * sizeof(TCHAR));
	file.Close();
}

CString CFindProcesschildDlg::GetFileName(int nDay)
{
	CString strTemp;
	strTemp.Format(_T("%.2d"), nDay);

	CString strDir;
	strDir.Format(_T("%s\\Log"), GetModulePath());

	if (_waccess(strDir, 0) == -1)
		::CreateDirectory(strDir, NULL);

	return strDir + _T("\\Log") + strTemp + _T(".txt");
}

CString CFindProcesschildDlg::GetModulePath()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	if (GetModuleFileName(AfxGetInstanceHandle(), szPath, MAX_PATH) > 0)
	{
		CString strPath(szPath);
		int nPos = strPath.ReverseFind('\\');
		if (nPos != -1)
			strPath = strPath.Left(nPos);
		return strPath;
	}
	return _T(".");
}