#include "pch.h"
#include "Searchfilegame.h"

void funcTest()
{
	//시작시간
	CTime tStart = CTime::GetCurrentTime();

	CStringArray drivelist;
	CStringArray folderList;
	CStringArray fileList;

	//존재하는 드라이브 조사	
	Searchfilegame::getDriveList(drivelist);
	int nSize = drivelist.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strRootDir = drivelist.GetAt(i);
		Searchfilegame::RecurseDirectories(folderList, fileList, strRootDir, 0);
	}

	//조사한 폴더 목록
	nSize = folderList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strDir = folderList.GetAt(i);
		OutputDebugString(strDir);
	}

	//찾은 파일 목록
	nSize = fileList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strFile = fileList.GetAt(i);
		OutputDebugString(strFile);
	}

	//완료 시간
	CTime tEnd = CTime::GetCurrentTime();
	CTimeSpan tResult = tEnd - tStart;
	OutputDebugString(tResult.Format("%M:%S\n"));
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

int Searchfilegame::getProcesscpu()
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

CString Searchfilegame::GetFilePath(CString strFilename)
{
	int nPos = strFilename.ReverseFind('\\');
	if (nPos > 0)
		return strFilename.Left(nPos);

	return strFilename;
}

//드라이브 개수
void Searchfilegame::getDriveList(CStringArray& drivelist)
{
	CString sDrive = _T("");
	for (int i = 0; i < 26; i++)
	{
		sDrive = "";
		sDrive.Format(_T("%c:"), 'a' + i);

		if (::PathFileExists(sDrive))
		{
			drivelist.Add(sDrive);
		}
	}
}

void Searchfilegame::RecurseDirectories(CStringArray& folderList, CStringArray& fileList, CString strDirectory, int nDepth)
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
				lstrcmpi(szFileName, _T("VALORANT.exe")) == 0)
			{
				int nExist = _waccess(szFullPath, 0);
				if (nExist != -1)
				{
					fileList.Add(szFullPath);
				}
			}
		}
	}
	finder.Close();
}
