#include "pch.h"
#include "framework.h"
#include "RecurseDirectories_test.h"
#include "RecurseDirectories_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRecurseDirectoriestestDlg::CRecurseDirectoriestestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RECURSEDIRECTORIES_TEST_DIALOG, pParent)
{
	m_pThread = NULL;
}

void CRecurseDirectoriestestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mlist1);
	DDX_Control(pDX, IDC_LIST2, mlist2);
}

BEGIN_MESSAGE_MAP(CRecurseDirectoriestestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CRecurseDirectoriestestDlg::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CRecurseDirectoriestestDlg::OnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CRecurseDirectoriestestDlg::OnSelchangeList2)
END_MESSAGE_MAP()

BOOL CRecurseDirectoriestestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  
}

void CRecurseDirectoriestestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CRecurseDirectoriestestDlg::OnDestroy()
{
	CloseThread();

	CDialog::OnDestroy();
}

int getcpucount()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return (int)info.dwNumberOfProcessors;
}

uint64_t file_time_2_utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;

	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;

	return li.QuadPart;
}

int getProcesscpu()
{
	static int nCount = -1;

	static int64_t last_time_ = 0;
	static int64_t last_system_time_ = 0;

	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	int64_t time;
	int64_t system_time;
	int64_t system_time_delta;
	int64_t time_delta;

	if (nCount == -1)
		nCount = getcpucount();
	
	GetSystemTimeAsFileTime(&now);

	if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time, &kernel_time, &user_time))
		return -1;
	
	system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / nCount;
	time = file_time_2_utc(&now);

	if ((last_system_time_ == 0) || (last_time_ == 0))
	{
		last_system_time_ = system_time;
		last_time_ = time;
		return -1;
	}

	system_time_delta = system_time - last_system_time_;
	time_delta = time - last_time_;

	if (time_delta == 0)
		return -1;
	
	int nCpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
	last_system_time_ = system_time;
	last_time_ = time;

	return nCpu;
}

CString CRecurseDirectoriestestDlg::GetFilePath(CString strFilename)
{
	int nPos = strFilename.ReverseFind('\\');
	if (nPos > 0)
		return strFilename.Left(nPos);

	return strFilename;
}

//드라이브 개수
void CRecurseDirectoriestestDlg::getDriveList(CStringArray& drivelist)
{
	CString sDrive = "";
	for (int i = 0; i < 26; i++)
	{
		sDrive = "";
		sDrive.Format("%c:", 'a' + i);

		if (::PathFileExists(sDrive))
		{
			drivelist.Add(sDrive);
		}
	}
}

void CRecurseDirectoriestestDlg::RecurseDirectories(CStringArray& folderList, CStringArray& fileList,  CString strDirectory, int nDepth)
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
		
		int nPercent = getProcesscpu();
		if (nPercent > 3)
		{
			Sleep(100);			
			/*
			CString strPath;
			strPath.Format(_T("getProcesscpu Percent = %d \n"), nPercent);
			OutputDebugString(strPath);
			*/
		}

		if (finder.IsDirectory())
		{
			//nDepth가 4 보다 작은 깊이의 폴더만 검사  
			if (nDepth > 4)
				continue;
			
			CString szFolderPath = finder.GetFilePath();
						
			//폴더 리스트			
			folderList.Add(szFolderPath);

			RecurseDirectories(folderList, fileList, szFolderPath, nDepth + 1);				
		}
		else
		{
			CString szFileName = finder.GetFileName();
			CString szFullPath = finder.GetFilePath();

			//찾은 파일
			if (lstrcmpi(szFileName, _T("lostark.exe")) == 0 || 
				lstrcmpi(szFileName, _T("LeagueClient.exe")) == 0 ||
				lstrcmpi(szFileName, _T("VALORANT.exe")) == 0 )
			{
				int nExist = _access(szFullPath, 0);
				if (nExist != -1)
				{
					fileList.Add(szFullPath);
				}
			}
		}
	}
	finder.Close();
}

void CRecurseDirectoriestestDlg::OnSelchangeList1()
{
	int nIndex = mlist1.GetCurSel();
	if (nIndex == -1)
		return;

	CString strFilePath;
	mlist1.GetText(nIndex, strFilePath);

	//선택된 리스트항목 폴더 오픈 
	HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), GetFilePath(strFilePath), NULL, NULL, SW_SHOW);
}

void CRecurseDirectoriestestDlg::OnSelchangeList2()
{
	int nIndex = mlist2.GetCurSel();
	if (nIndex == -1)
		return;

	CString strFilePath;
	mlist2.GetText(nIndex, strFilePath);

	//선택된 리스트항목 폴더 오픈 
	HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), GetFilePath(strFilePath), NULL, NULL, SW_SHOW);
}

void CRecurseDirectoriestestDlg::OnBnClickedButton1()
{	
	StartThread();
}

void CRecurseDirectoriestestDlg::StartThread()
{
	if (m_pThread == NULL)
	{
		m_pThread = new SearchThread;
		m_pThread->SetThread(this);
		m_pThread->CreateThread();
	}
}

void CRecurseDirectoriestestDlg::CloseThread()
{
	if (m_pThread)
	{
		m_pThread->ResumeThread();
		delete 	m_pThread;
	}
}

void CRecurseDirectoriestestDlg::runThread()
{
	//시작시간
	CTime tStart = CTime::GetCurrentTime();


	//리스트 초기화
	mlist1.ResetContent();
	mlist2.ResetContent();

	CStringArray folderList;
	CStringArray fileList;
	CStringArray drivelist;

	//존재하는 드라이브 조사
	getDriveList(drivelist);

	//존재하는 드라이브만큼 폴더 조사
	int nSize = drivelist.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strRootDir = drivelist.GetAt(i);
		RecurseDirectories(folderList, fileList, strRootDir, 0);
	}

	//--list 에 데이터 입력시 cpu 튄다
	///*
	//조사한 폴더 목록
	nSize = folderList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strDir = folderList.GetAt(i);
		mlist1.AddString(strDir);
	}

	//찾은 파일 목록
	nSize = fileList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strFile = fileList.GetAt(i);
		mlist2.AddString(strFile);
	}
	//*/

	//완료 시간
	CTime tEnd = CTime::GetCurrentTime();
	CTimeSpan tResult = tEnd - tStart;

	CString sMsg;
	sMsg += tResult.Format("%M:%S\n");
	SetDlgItemText(IDC_STATIC_TIME, sMsg);


	Sleep(100);
}