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
	GetRegUninstallInfo();
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

void CuninstallInfotestDlg::GetRegUninstallInfo()
{
	LPCTSTR lpkey = _T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

	HKEY hKey = NULL;
	HKEY hSubKey = NULL;

	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpkey, 0, KEY_ALL_ACCESS, &hKey))
		return;

	DWORD dwIndex = 0;
	CString strSubkey;
	CString strkeyName;
	CString strDisplayName;
	CString strUninstallString;

	TCHAR tKeyName[MAX_PATH] = { 0 };
	TCHAR tDisplayName[MAX_PATH] = { 0 };
	TCHAR tUninstallString[MAX_PATH] = { 0 };
	DWORD dSize = MAX_PATH;

	while (RegEnumKey(hKey, dwIndex++, tKeyName, dSize) != ERROR_NO_MORE_ITEMS)
	{
		strSubkey = "";
		strSubkey.Format(L"%s\\%s", lpkey, tKeyName);
				
		if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubkey, 0, KEY_QUERY_VALUE, &hSubKey))
		{
			strkeyName = _T("");
		}
		else
		{
			strkeyName.Format(L"%s", tKeyName);
		}
		
		if (ERROR_SUCCESS != RegQueryValueEx(hSubKey, L"DisplayName", 0, 0, (LPBYTE)tDisplayName, &dSize))
		{
			strDisplayName = _T("");
		}
		else
		{
			strDisplayName.Format(L"%s", tDisplayName);
		}
		
		if (ERROR_SUCCESS != RegQueryValueEx(hSubKey, L"UninstallString", 0, 0, (LPBYTE)tUninstallString, &dSize))
		{
			strUninstallString = _T("");
		}
		else
		{
			strUninstallString.Format(L"%s", tUninstallString);
		}

		//정보 저장
		AddInfo(strkeyName, strDisplayName, strUninstallString);


		//초기화
		::RegCloseKey(hSubKey);

		::ZeroMemory(tKeyName, MAX_PATH);
		::ZeroMemory(tDisplayName, MAX_PATH);
		dSize = MAX_PATH;

	}
	::RegCloseKey(hKey);
}

void CuninstallInfotestDlg::SearchInfoList()
{
	m_ListCtrl.DeleteAllItems();

	for (int i = 0; i < m_Info.size(); i++)
	{
		InsertListView(m_Info[i].strkey, m_Info[i].strDisplayName, m_Info[i].strUninstallString);
	}
}

void CuninstallInfotestDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CString strItem = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 0);
	CString strDisplayName = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 1);
	CString strUninstallString = m_ListCtrl.GetItemText(pNMItemActivate->iItem, 2);

	AfxMessageBox(strUninstallString);

	*pResult = 0;
}
