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
	ON_BN_CLICKED(IDC_BUTTON2, &CFindBrowserExtensionsDlg::OnBnClickedButton2)
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

	InitPath();
}

void CFindBrowserExtensionsDlg::InsertListView(CString strItem, CString strDisplayName)
{
	int nIndex = m_ListCtrl.InsertItem(0, strItem);
	m_ListCtrl.SetItemText(nIndex, 1, strDisplayName);
}

void CFindBrowserExtensionsDlg::ViewListInfo()
{
	m_ListCtrl.DeleteAllItems();

	for (int i = 0; i < m_Extensions.size(); i++)
	{
		InsertListView(m_Extensions[i].strItem, m_Extensions[i].strDisplayName);
	}
}

void CFindBrowserExtensionsDlg::OnBnClickedButton1()
{
	ClearExtensionInfo();
	searchExtension(strChromeExtension);
	searchExtension(strEdgeExtension, TRUE);
	searchExtensionFireFox(strFirefoxExtension, TRUE);
	ViewListInfo();
}

void CFindBrowserExtensionsDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CString strItem = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	CString strDisplayName = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 1);

	CString strPath;

	if (strDisplayName == _T("edge"))
		strPath.Format(_T("%s"), strEdgeExtension);
	else if (strDisplayName == _T("chrome"))
		strPath.Format(_T("%s"), strChromeExtension);
	else if (strDisplayName == _T("firefox"))
		strPath.Format(_T("%s"), strFirefoxExtension);
	
	ShellExecute(GetSafeHwnd(), NULL, strPath, NULL, NULL, SW_SHOW);

	*pResult = 0;
}

void CFindBrowserExtensionsDlg::InitPath()
{
	TCHAR szPath[MAX_PATH];

	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	strChromeExtension.Format(_T("%s\\Google\\Chrome\\User Data\\Default\\Extensions"), szPath);

	ZeroMemory(szPath, MAX_PATH);
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	strEdgeExtension.Format(_T("%s\\Microsoft\\Edge\\User Data\\Default\\Extensions"), szPath);

	ZeroMemory(szPath, MAX_PATH);
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	strFirefoxExtension.Format(_T("%s\\Mozilla\\Firefox\\Profiles\\"), szPath);

	SetDlgItemText(IDC_STATIC_CHROME, _T("chrome path - ") + strChromeExtension);
	SetDlgItemText(IDC_STATIC_EDGE, _T("edge path - ") + strEdgeExtension);
	SetDlgItemText(IDC_STATIC_FIREFOX, _T("Firefox path - ") + strFirefoxExtension + _T("\\전체폴더검색\\extensions"));
}

CString CFindBrowserExtensionsDlg::GetModulePath()
{
	TCHAR tPath[MAX_PATH];
	::GetModuleFileName(NULL, tPath, MAX_PATH);

	CString strPath = tPath;
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
	return strPath;
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
				AddExtensionInfo(strItem, _T("edge"));
			else
				AddExtensionInfo(strItem, _T("chrome"));
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
				AddExtensionInfo(strFileName, _T("firefox"));
			}
		}
	}
}

void CFindBrowserExtensionsDlg::AddExtensionInfo(CString strItem, CString strDisplayName)
{
	ExtensionInfo info;
	info.strItem = strItem;
	info.strDisplayName = strDisplayName;
	m_Extensions.push_back(info);
}

void CFindBrowserExtensionsDlg::ClearExtensionInfo()
{
	m_Extensions.clear();
}

void CFindBrowserExtensionsDlg::OnBnClickedButton2()
{
	CString strFilePath;
	CString strIndex;
	CString strCount;

	//경로
	strFilePath.Format(_T("%s\\exetensionInfo.ini"), GetModulePath());

	//총개수
	strCount.Format(_T("%d"), m_Extensions.size());

	CIniFile ini;
	ini.CreateIniFile(strFilePath);
	ini.SetTotalCount(strCount);

	//정보
	int nIndex = 0;
	for (int i = 0; i < m_Extensions.size(); i++)
	{
		nIndex ++;
		strIndex.Format(_T("%d"), nIndex);

		ini.SetInfoType(strIndex, m_Extensions[i].strDisplayName);
		ini.SetInfo(strIndex, m_Extensions[i].strItem);
	}
}
