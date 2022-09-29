#include "pch.h"
#include "framework.h"
#include "uninstallInfo_test.h"
#include "uninstallInfo_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CuninstallInfotestDlg::CuninstallInfotestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNINSTALLINFO_TEST_DIALOG, pParent)
{
}

void CuninstallInfotestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CuninstallInfotestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CuninstallInfotestDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CuninstallInfotestDlg::OnDblclkList1)
END_MESSAGE_MAP()

BOOL CuninstallInfotestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	InitCtrl();

	OnBnClickedButton1();

	return TRUE;  
}

void CuninstallInfotestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CuninstallInfotestDlg::OnDestroy()
{
	ClearInfo();

	CDialog::OnDestroy();
}

void CuninstallInfotestDlg::OnBnClickedButton1()
{
	ClearInfo();
	searchRegInfo();
	SearchInfoList();
}

void CuninstallInfotestDlg::InitCtrl()
{
	CRect rect;
	m_ListCtrl.GetClientRect(&rect);
	m_ListCtrl.InsertColumn(0, _T("항목"), LVCFMT_CENTER, 400, -1);
	m_ListCtrl.InsertColumn(1, _T("설명"), LVCFMT_LEFT, 200);
	m_ListCtrl.InsertColumn(2, _T("삭제 정보"), LVCFMT_LEFT, rect.Width() - 500);
}

void CuninstallInfotestDlg::InsertListView(CString strkey, CString strDisplayName, CString strUninstallString)
{
	int nIndex = m_ListCtrl.InsertItem(0, strkey);
	m_ListCtrl.SetItemText(nIndex, 1, strDisplayName);
	m_ListCtrl.SetItemText(nIndex, 2, strUninstallString);
}

void CuninstallInfotestDlg::AddInfo(CString strkey, CString strDisplayName, CString strUninstallString)
{
	UnInstallInfo info;
	info.strkey = strkey;
	info.strDisplayName = strDisplayName;
	info.strUninstallString = strUninstallString;
	m_Info.push_back(info);
}

void CuninstallInfotestDlg::ClearInfo()
{
	m_Info.clear();
}

void CuninstallInfotestDlg::SearchInfoList()
{
	m_ListCtrl.DeleteAllItems();

	for (int i = 0; i < m_Info.size(); i++)
	{
		InsertListView(m_Info[i].strkey, m_Info[i].strDisplayName, m_Info[i].strUninstallString);
	}

	CString strMsg;
	strMsg.Format(_T("총개수:%d"), m_ListCtrl.GetItemCount());
	OutputDebugString(strMsg);
}

void CuninstallInfotestDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CString strItem = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	CString strDisplayName = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 1);
	CString strUninstall = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 2);


	//삭제 프로그램 호출
	CString strSetup = _T("exe");

	if (strUninstall != "" && strUninstall.Find(strSetup) > 0)
	{
		int nExePath = strUninstall.Find(strSetup) + strSetup.GetLength();
		int nFullPath = strUninstall.GetLength();

		CString strExe = strUninstall.Left(nExePath);
		CString strParam = strUninstall.Right(nFullPath - nExePath - 1);
		strExe.Replace(_T("\""), _T(""));

		if (::PathFileExists(strExe))
		{
			HINSTANCE hInstance = ::ShellExecute(NULL, L"open", strExe, strParam, NULL, SW_SHOW);
		}
		else
		{
			HINSTANCE hInstance = ::ShellExecute(NULL, L"open", strExe, strParam, NULL, SW_SHOW);
		}
	}

	*pResult = 0;
}

void CuninstallInfotestDlg::searchRegInfo()
{
	LPCTSTR lpKey = _T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	CString strKey = _T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

	HKEY hKey = NULL;
	HKEY hSubKey = NULL;

	//requireAdministrator 권한 필요
	/*
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCTSTR)strKey, 0, KEY_ALL_ACCESS, &hKey))
		return;
	*/

	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpKey, 0, KEY_READ, &hKey))
		return;


	CString strSubkey = _T("");

	DWORD dwBufferSize = 0;
	DWORD dwIndex = 0;

	DWORD dwType = 0;
	TCHAR tKeyName[MAX_PATH] = { 0 };
	TCHAR tDisplayName[MAX_PATH] = { 0 };
	TCHAR tUninstallString[MAX_PATH] = { 0 };

	CString strKeyName = _T("");
	CString strDisplayName = _T("");
	CString strUninstallString = _T("");

	while (ERROR_SUCCESS == RegEnumKey(hKey, dwIndex, tKeyName, MAX_PATH))
	{
		strSubkey = _T("");
		strSubkey.Format(_T("%s\\%s"), lpKey, tKeyName);

		if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubkey, 0, KEY_READ, &hSubKey))
		{
			dwIndex++;

			dwType = 0;
			::ZeroMemory(tKeyName, MAX_PATH);
			dwBufferSize = MAX_PATH;
			strKeyName = _T("");
			continue;
		}

		strKeyName.Format(_T("%s"), tKeyName);


		//DisplayName
		dwType = 0;
		::ZeroMemory(tDisplayName, MAX_PATH);
		dwBufferSize = MAX_PATH;
		strDisplayName = _T("");

		if (ERROR_SUCCESS == RegQueryValueEx(hSubKey, _T("DisplayName"), NULL, &dwType, (LPBYTE)tDisplayName, &dwBufferSize))
		{
			strDisplayName.Format(_T("%s"), tDisplayName);
		}

		//UninstallString
		dwType = 0;
		::ZeroMemory(tUninstallString, MAX_PATH);
		dwBufferSize = MAX_PATH;
		strUninstallString = _T("");

		if (ERROR_SUCCESS == RegQueryValueEx(hSubKey, _T("UninstallString"), NULL, &dwType, (LPBYTE)tUninstallString, &dwBufferSize))
		{
			strUninstallString.Format(_T("%s"), tUninstallString);
		}
		else
		{
			strUninstallString = searchSubRegInfo(strSubkey);
		}

		//정보 저장
		AddInfo(strKeyName, strDisplayName, strUninstallString);

	
		//KeyName
		dwType = 0;
		::ZeroMemory(tKeyName, MAX_PATH);
		dwBufferSize = MAX_PATH;
		strKeyName = _T("");


		RegCloseKey(hSubKey);
		hSubKey = NULL;


		dwIndex++;
	}

	RegCloseKey(hKey);
	hKey = NULL;
}

CString CuninstallInfotestDlg::searchSubRegInfo(CString strSubkey)
{
	TCHAR tUninstallString[MAX_PATH] = { 0 };
	ULONG uBufferSize{ MAX_PATH };

	CString strUninstallString = _T("");

	CRegKey regKey;
	if (regKey.Open(HKEY_LOCAL_MACHINE, (LPCTSTR)strSubkey, KEY_READ) != ERROR_SUCCESS)
	{
		return _T("");
	}

	if (regKey.QueryStringValue(_T("UninstallString"), tUninstallString, &uBufferSize) == ERROR_SUCCESS)
	{
		strUninstallString.Format(_T("%s"), tUninstallString);
	}

	regKey.Close();

	return strUninstallString;
}