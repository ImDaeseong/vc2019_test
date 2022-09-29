#include "pch.h"
#include "framework.h"
#include "searchInstallinfo_test.h"
#include "searchInstallinfo_testDlg.h"
#include "afxdialogex.h"


#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CsearchInstallinfotestDlg::CsearchInstallinfotestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEARCHINSTALLINFO_TEST_DIALOG, pParent)
{
}

void CsearchInstallinfotestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CsearchInstallinfotestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CsearchInstallinfotestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CsearchInstallinfotestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	InitCtrl();
	
	OnBnClickedButton1();

	return TRUE; 
}

void CsearchInstallinfotestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CsearchInstallinfotestDlg::OnDestroy()
{
	ClearSetupInfo();

	CDialog::OnDestroy();
}

void CsearchInstallinfotestDlg::OnBnClickedButton1()
{
	ClearSetupInfo();
	searchRegInfo();	
	GetSetupInfoList();
}

void CsearchInstallinfotestDlg::InitCtrl()
{
	CRect rect;
	m_ListCtrl.GetClientRect(&rect);
	m_ListCtrl.InsertColumn(0, _T("항목"), LVCFMT_CENTER, 700, -1);
	m_ListCtrl.InsertColumn(1, _T("설명"), LVCFMT_LEFT, rect.Width() - 700);
}

void CsearchInstallinfotestDlg::InsertListView(CString strkey, CString strDisplayName)
{
	int nIndex = m_ListCtrl.InsertItem(0, strkey);
	m_ListCtrl.SetItemText(nIndex, 1, strDisplayName);
}

void CsearchInstallinfotestDlg::AddSetupInfo(CString strkey, CString strDisplayName)
{
	InstallInfo info;
	info.strkey = strkey;
	info.strDisplayName = strDisplayName;
	m_SetupInfo.push_back(info);
}

void CsearchInstallinfotestDlg::ClearSetupInfo()
{
	m_SetupInfo.clear();
}

void CsearchInstallinfotestDlg::GetSetupInfoList()
{
	m_ListCtrl.DeleteAllItems();

	for (int i = 0; i < m_SetupInfo.size(); i++)
	{
		InsertListView(m_SetupInfo[i].strkey, m_SetupInfo[i].strDisplayName);
	}

	CString strMsg;
	strMsg.Format(_T("총개수:%d"), m_ListCtrl.GetItemCount());
	OutputDebugString(strMsg);
}

void CsearchInstallinfotestDlg::searchRegInfo()
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
		AddSetupInfo(strKeyName, strDisplayName);
		

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

CString CsearchInstallinfotestDlg::searchSubRegInfo(CString strSubkey)
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