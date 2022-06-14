#include "pch.h"
#include "framework.h"
#include "searchInstallinfo_test.h"
#include "searchInstallinfo_testDlg.h"
#include "afxdialogex.h"

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
	searchReg(TRUE);
	searchReg(FALSE);
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

//프로그램 설치 항목을 레지스트리에서 검색
void CsearchInstallinfotestDlg::searchReg(BOOL bWOW)
{
	LPCTSTR lpkey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

	if(bWOW)
		lpkey = _T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

	HKEY hKey = NULL;
	HKEY hSubKey = NULL;

	TCHAR tKeyName[MAX_PATH] = { 0 };
	TCHAR tDisplayName[MAX_PATH] = { 0 };
	DWORD dSize = MAX_PATH;

	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpkey, 0, KEY_ALL_ACCESS, &hKey))
		return;

	DWORD dwIndex = 0;
	CString strSubkey;

	CString strkey;
	CString strDisplayName;

	while (RegEnumKey(hKey, dwIndex++, tKeyName, dSize) != ERROR_NO_MORE_ITEMS)
	{
		strSubkey = "";
		strSubkey.Format(L"%s\\%s", lpkey, tKeyName);

		if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubkey, 0, KEY_QUERY_VALUE, &hSubKey))
			continue;

		if (ERROR_SUCCESS != RegQueryValueEx(hSubKey, L"DisplayName", 0, 0, (LPBYTE)tDisplayName, &dSize))
			continue;

		strkey.Format(L"%s", tKeyName);
		strDisplayName.Format(L"%s",tDisplayName);
		AddSetupInfo(strkey, strDisplayName);

		dSize = MAX_PATH;
		::ZeroMemory(tKeyName, MAX_PATH);
		::ZeroMemory(tDisplayName, MAX_PATH);

		::RegCloseKey(hSubKey);
	}
	::RegCloseKey(hKey);
}

void CsearchInstallinfotestDlg::GetSetupInfoList()
{
	m_ListCtrl.DeleteAllItems();

	for (int i = 0; i < m_SetupInfo.size(); i++)
	{
		InsertListView(m_SetupInfo[i].strkey, m_SetupInfo[i].strDisplayName);
	}
}