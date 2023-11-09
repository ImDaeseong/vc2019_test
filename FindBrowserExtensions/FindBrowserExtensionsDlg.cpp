#include "pch.h"
#include "framework.h"
#include "FindBrowserExtensions.h"
#include "FindBrowserExtensionsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CFindBrowserExtensionsDlg::CFindBrowserExtensionsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINDBROWSEREXTENSIONS_DIALOG, pParent)
{
	ClearExtensionInfo();
}

void CFindBrowserExtensionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CFindBrowserExtensionsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CFindBrowserExtensionsDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CFindBrowserExtensionsDlg::OnNMDblclkList1)
END_MESSAGE_MAP()

BOOL CFindBrowserExtensionsDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFindBrowserExtensionsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	InitCtrl();

	OnBnClickedButton1();

	return TRUE;  
}

void CFindBrowserExtensionsDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CFindBrowserExtensionsDlg::OnDestroy()
{	
	ClearExtensionInfo();

	CDialog::OnDestroy();
}

void CFindBrowserExtensionsDlg::InitCtrl()
{
	CRect rect;
	m_ListCtrl.GetClientRect(&rect);
	m_ListCtrl.InsertColumn(0, _T("항목"), LVCFMT_CENTER, 300, -1);
	m_ListCtrl.InsertColumn(1, _T("설명"), LVCFMT_LEFT, rect.Width() - 300);
	m_ListCtrl.InsertColumn(2, _T(""), LVCFMT_LEFT, 0);

	InitPath();
}

void CFindBrowserExtensionsDlg::InsertListView(CString strItem, CString strDisplayName, CString strItemFullPath)
{
	int nIndex = m_ListCtrl.InsertItem(0, strItem);
	m_ListCtrl.SetItemText(nIndex, 1, strDisplayName);
	m_ListCtrl.SetItemText(nIndex, 2, strItemFullPath);
}

void CFindBrowserExtensionsDlg::ViewListInfo()
{
	m_ListCtrl.DeleteAllItems();

	for (int i = 0; i < m_Extensions.size(); i++)
	{
		InsertListView(m_Extensions[i].strItem, m_Extensions[i].strDisplayName, m_Extensions[i].strItemFullPath);
	}
}

void CFindBrowserExtensionsDlg::OnBnClickedButton1()
{
	FindExetions();
}

void CFindBrowserExtensionsDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CString strItem = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	CString strDisplayName = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 1);
	CString strItemFullPath = GetFolderPath(m_ListCtrl.GetItemText(pNMItemActivate->iItem, 2));

	ShellExecute(GetSafeHwnd(), NULL, strItemFullPath, NULL, NULL, SW_SHOW);

	*pResult = 0;
}

void CFindBrowserExtensionsDlg::FindExetions()
{
	ClearExtensionInfo();

	searchExtension(strChromeSearchPath1);
	searchExtension(strChromeSearchPath2);
	searchExtension(strEdgeExtension, TRUE);
	searchExtensionFireFox(strFirefoxExtension, TRUE);
	SearchPathAllProfile(strChromeSearchPathProfile);
	SearchPathAllNaverWhaleProfile(strNaverWhaleSearchPath);

	ViewListInfo();
}

void CFindBrowserExtensionsDlg::InitPath()
{
	TCHAR szPath[MAX_PATH];

	//Chrome1
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	strChromeSearchPath1.Format(_T("%s\\Google\\Chrome\\User Data\\Default\\Extensions"), szPath);

	//Chrome2
	ZeroMemory(szPath, MAX_PATH);
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	strChromeSearchPath2.Format(_T("%s\\Google\\Chrome\\User Data\\Default\\Sync Extension Settings"), szPath);

	//Chrome3
	ZeroMemory(szPath, MAX_PATH);
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	strChromeSearchPathProfile.Format(_T("%s\\Google\\Chrome\\User Data"), szPath);
	
	//Edge
	ZeroMemory(szPath, MAX_PATH);
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	strEdgeExtension.Format(_T("%s\\Microsoft\\Edge\\User Data\\Default\\Extensions"), szPath);

	//Firefox
	ZeroMemory(szPath, MAX_PATH);
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	strFirefoxExtension.Format(_T("%s\\Mozilla\\Firefox\\Profiles\\"), szPath);

	//naver whale
	ZeroMemory(szPath, MAX_PATH);
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	strNaverWhaleSearchPath.Format(_T("%s\\Naver\\Naver Whale\\User Data"), szPath);

	SetDlgItemText(IDC_STATIC_CHROME1, _T("chrome path1 - ") + strChromeSearchPath1);
	SetDlgItemText(IDC_STATIC_CHROME2, _T("chrome path2 - ") + strChromeSearchPath2);
	SetDlgItemText(IDC_STATIC_CHROME3, _T("chrome path3 - ") + strChromeSearchPathProfile + _T("\\profile 포함된 이름 전체 검색"));
	SetDlgItemText(IDC_STATIC_EDGE, _T("edge path - ") + strEdgeExtension);
	SetDlgItemText(IDC_STATIC_FIREFOX, _T("Firefox path - ") + strFirefoxExtension + _T("\\전체폴더검색\\extensions"));
	SetDlgItemText(IDC_STATIC_NAVERWHALE1, _T("naverwhale path1 - ") + strNaverWhaleSearchPath);
	SetDlgItemText(IDC_STATIC_NAVERWHALE2, _T("naverwhale path2 - ") + strNaverWhaleSearchPath);
}

BOOL CFindBrowserExtensionsDlg::DeleteFolderAll(CString strFolderPath)
{
	WIN32_FIND_DATA  FindData;

	HANDLE FindHandle;
	TCHAR temp1[MAX_PATH];
	int lpFolder_Len;

	TCHAR lpFolder[256];
	swprintf_s(lpFolder, _T("%s"), strFolderPath);//_stprintf(lpFolder, _T("%s"), strFolderPath);
	
	if (lpFolder == NULL)
	{
		return FALSE;
	}

	if (*lpFolder == '\0')
	{
		return FALSE;
	}

	lpFolder_Len = wcslen(lpFolder);

	if (lpFolder[lpFolder_Len - 1] == '\\')
	{
		swprintf_s(temp1, _T("%s*.*"), lpFolder);//_stprintf(temp1, _T("%s*.*"), lpFolder);
	}
	else
	{
		swprintf_s(temp1, _T("%s\\*.*"), lpFolder);//_stprintf(temp1, _T("%s\\*.*"), lpFolder);
	}

	FindHandle = FindFirstFile(temp1, &FindData);

	if (INVALID_HANDLE_VALUE != FindHandle)
	{
		while (1)
		{
			if (lpFolder[lpFolder_Len - 1] == '\\')
			{
				swprintf_s(temp1, _T("%s%s"), lpFolder, FindData.cFileName);//_stprintf(temp1, _T("%s%s"), lpFolder, FindData.cFileName);
			}
			else
			{
				swprintf_s(temp1, _T("%s\\%s"), lpFolder, FindData.cFileName);//_stprintf(temp1, _T("%s\\%s"), lpFolder, FindData.cFileName);
			}

			if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//Dir
				if (lstrcmpi(FindData.cFileName, _T(".")) != 0 && lstrcmpi(FindData.cFileName, _T("..")) != 0)
				{
					DeleteFolderAll(temp1);

					if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) || (FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
					{
						SetFileAttributes(temp1, FILE_ATTRIBUTE_NORMAL);
					}

					RemoveDirectory(temp1);
				}
			}
			else
			{
				//File
				if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) || (FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
				{
					SetFileAttributes(temp1, FILE_ATTRIBUTE_NORMAL);
				}

				DeleteFile(temp1);
			}

			if (!FindNextFile(FindHandle, &FindData))
			{
				break;
			}
		}
	}

	FindClose(FindHandle);

	SetFileAttributes(lpFolder, FILE_ATTRIBUTE_NORMAL);

	RemoveDirectory(lpFolder);

	return TRUE;
}

BOOL CFindBrowserExtensionsDlg::CloseAllProcess(CString strInputFileName)
{
	BOOL bResult = FALSE;
	CString strFileName = _T("");

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	::Process32First(snapshot, &pe32);	
	
	while (TRUE == ::Process32Next(snapshot, &pe32))
	{
		strFileName = pe32.szExeFile;
		strFileName.MakeLower();

		if (strFileName == strInputFileName.MakeLower())
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
			if (hProcess != NULL)
			{
				if (TerminateProcess(hProcess, 0))
				{
					bResult = TRUE;
				}
				::CloseHandle(hProcess);
			}
		}
	}
	::CloseHandle(snapshot);

	return TRUE;
}

CString CFindBrowserExtensionsDlg::GetFolderPath(CString strFolderPath)
{
	int nPos = strFolderPath.ReverseFind('\\');
	if (nPos > 0)
		return strFolderPath.Left(nPos);

	return strFolderPath;
}

CString CFindBrowserExtensionsDlg::GetFolderName(CString strFolderName)
{
	int nPos = strFolderName.ReverseFind('\\');
	if (nPos > 0)
		return strFolderName.Mid(nPos + 1);

	return strFolderName;
}

void CFindBrowserExtensionsDlg::searchExtension(CString strDirectory, BOOL bEdge)
{
	CString strFilter = strDirectory + _T("\\*.*");

	double start = 0;
	double stop = 0;

	CFileFind finder;
	BOOL bSearch = finder.FindFile(strFilter, 0);

	while (bSearch)
	{
		bSearch = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			CString strItem = GetFolderName(finder.GetFilePath());

			if (bEdge)
				AddExtensionInfo(strItem, _T("edge"), finder.GetFilePath());
			else
				AddExtensionInfo(strItem, _T("chrome"), finder.GetFilePath());


			//CloseAllProcess(_T("chrome.exe"));
			//DeleteFolderAll(finder.GetFilePath());
		}		
	}
	finder.Close();
}

void CFindBrowserExtensionsDlg::searchExtensionFireFox(CString strDirectory, BOOL bFirefox)
{
	CFileFind file;
	BOOL bFind = file.FindFile(strDirectory + _T("\\*.*"));
	CString strFolderItem, strFileExit, strTemp;

	while (bFind)
	{
		bFind = file.FindNextFile();
		if (file.IsDirectory() && !file.IsDots())
		{
			strFolderItem = file.GetFilePath();

			CString strExetension = GetFolderName(strFolderItem);
			if (lstrcmpi(strExetension, _T("extensions")) == 0)
			{
				//파일 경로를 찾았으므로 경로 업데이트
				strFirefoxExtension = strFolderItem;
				SetDlgItemText(IDC_STATIC_FIREFOX, _T("Firefox path - ") + strFirefoxExtension);

				searchExtensionFireFox(strFolderItem, true);
			}
			else
			{
				searchExtensionFireFox(strFolderItem, false);
			}
		}

		strFolderItem = file.GetFileName();
		if (!file.IsDots())
		{
			if (file.IsDirectory())
				continue;

			if (bFirefox)
			{
				CString strFileName;
				strFileName = strFolderItem.MakeLower();
				AddExtensionInfo(strFileName, _T("firefox"), file.GetFilePath());
			}
		}
	}
}

void CFindBrowserExtensionsDlg::SearchPathAllProfile(CString strDirectory)
{
	SearchChromeProfile(strDirectory, 0);
	AddChromeProfile();
}

void CFindBrowserExtensionsDlg::SearchChromeProfile(CString strDirectory, int nDepth)
{
	CString strFilter = strDirectory + _T("\\*.*");

	CFileFind finder;
	BOOL bSearch = finder.FindFile(strFilter, 0);
	while (bSearch)
	{
		bSearch = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			if (nDepth > 0)
				continue;

			CString szFolderPath = finder.GetFilePath();

			CString strTemp = szFolderPath.MakeLower();
			if (strTemp.Find(_T("profile")) > 0)
			{
				ArChromeExtensionProfile.Add(szFolderPath);
				SearchChromeProfile(szFolderPath, nDepth + 1);
			}
		}
	}
	finder.Close();
}

void CFindBrowserExtensionsDlg::AddChromeProfile()
{
	CString strFodler1, strFodler2;
	for (int i = 0; i < ArChromeExtensionProfile.GetSize(); i++)
	{
		strFodler1.Format(_T("%s\\Extensions"), ArChromeExtensionProfile.GetAt(i));
		strFodler2.Format(_T("%s\\Sync Extension Settings"), ArChromeExtensionProfile.GetAt(i));
		searchExtension(strFodler1);
		searchExtension(strFodler2);
	}
}

void CFindBrowserExtensionsDlg::SearchPathAllNaverWhaleProfile(CString strDirectory)
{
	ArNaverWhaleExtensionProfile.Add(strDirectory + CString("\\Default"));

	SearchNaverWhaleProfile(strDirectory, 0);
	AddNaverWhaleProfile();
}

void CFindBrowserExtensionsDlg::SearchNaverWhaleProfile(CString strDirectory, int nDepth)
{
	CString strFilter = strDirectory + _T("\\*.*");

	CFileFind finder;
	BOOL bSearch = finder.FindFile(strFilter, 0);
	while (bSearch)
	{
		bSearch = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			if (nDepth > 0)
				continue;

			CString szFolderPath = finder.GetFilePath();

			CString strTemp = szFolderPath.MakeLower();
			if (strTemp.Find(_T("profile")) > 0)
			{
				ArNaverWhaleExtensionProfile.Add(szFolderPath);

				SearchNaverWhaleProfile(szFolderPath, nDepth + 1);
			}
		}
	}
	finder.Close();
}

void CFindBrowserExtensionsDlg::AddNaverWhaleProfile()
{
	CString strFodler1, strFodler2;
	for (int i = 0; i < ArNaverWhaleExtensionProfile.GetSize(); i++)
	{
		strFodler1.Format(_T("%s\\Extensions"), ArNaverWhaleExtensionProfile.GetAt(i));
		strFodler2.Format(_T("%s\\Sync Extension Settings"), ArNaverWhaleExtensionProfile.GetAt(i));

		//파일 경로를 찾았으므로 경로 업데이트
		SetDlgItemText(IDC_STATIC_NAVERWHALE1, _T("naverwhale path1 - ") + strFodler1);
		SetDlgItemText(IDC_STATIC_NAVERWHALE2, _T("naverwhale path2 - ") + strFodler2);

		SearchPathNaverWhaleAll(strFodler1);
		SearchPathNaverWhaleAll(strFodler2);
	}
}

void CFindBrowserExtensionsDlg::SearchPathNaverWhaleAll(CString strDirectory)
{
	CString strFilter = strDirectory + _T("\\*.*");

	double start = 0;
	double stop = 0;

	CFileFind finder;
	BOOL bSearch = finder.FindFile(strFilter, 0);

	while (bSearch)
	{
		bSearch = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			CString strItem = GetFolderName(finder.GetFilePath());

			AddExtensionInfo(strItem, _T("NaverWhale"), finder.GetFilePath());
		}
	}
	finder.Close();
}

void CFindBrowserExtensionsDlg::AddExtensionInfo(CString strItem, CString strDisplayName, CString strItemFullPath)
{
	ExtensionInfo info;
	info.strItem = strItem;
	info.strDisplayName = strDisplayName;
	info.strItemFullPath = strItemFullPath;
	m_Extensions.push_back(info);
}

void CFindBrowserExtensionsDlg::ClearExtensionInfo()
{
	m_Extensions.clear();

	ArChromeExtensionProfile.RemoveAll();

	ArNaverWhaleExtensionProfile.RemoveAll();
}
