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
	else
		strPath.Format(_T("%s"), strChromeExtension);		
	
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

	SetDlgItemText(IDC_STATIC_CHROME, _T("chrome path - ") + strChromeExtension);
	SetDlgItemText(IDC_STATIC_EDGE, _T("edge path - ") + strEdgeExtension);
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