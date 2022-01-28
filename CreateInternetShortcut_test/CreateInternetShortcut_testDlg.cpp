#include "pch.h"
#include "framework.h"
#include "CreateInternetShortcut_test.h"
#include "CreateInternetShortcut_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SHORTSYMBOL m_gameInfo[] =
{
	{ _T("던파"), 1 },
	{ _T("디아블로3"),	2 },
	{ _T("리그오브레전드"), 3 },
	{ _T("피파4"), 4 },
	{ _T("배틀그라운드"), 5 },
	{ _T("WOW"), 6 },
	{ NULL,	0 }
};

CCreateInternetShortcuttestDlg::CCreateInternetShortcuttestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CREATEINTERNETSHORTCUT_TEST_DIALOG, pParent)
{
}

void CCreateInternetShortcuttestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCreateInternetShortcuttestDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL CCreateInternetShortcuttestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CreateDesktopIcon();
	
	return TRUE;  
}

void CCreateInternetShortcuttestDlg::OnPaint()
{
	CPaintDC dc(this);
}

//resource file unzip
BOOL CCreateInternetShortcuttestDlg::UnzipResource()
{
	CString strCurrentPath = DesktopIcon::GetModulePath();

	CString strIcon;
	strIcon.Format(_T("%s\\Icon.zip"), strCurrentPath);

	if (DesktopIcon::GetReourceFiles(strIcon, _T("RC"), IDR_RC1))
	{
		if (DesktopIcon::UnZipFolder(strCurrentPath + CString(_T("\\TempIcon")), strIcon))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CCreateInternetShortcuttestDlg::CreateDesktopIcon()
{
	int nExist = -1;
	CString strIcon;
	CString strTxt;
	CString strShortcutPath;

	CString strCurrentPath = DesktopIcon::GetModulePath();
	CString strDesktopPath = DesktopIcon::GetDesktopPath();

	if (UnzipResource())
	{
		for (int i = 0; i < 6; i++)
		{
			//바로가기 존재 여부 확인
			strShortcutPath.Format(_T("%s\\%s.url"), strDesktopPath, m_gameInfo[i].strName);
			nExist = _waccess(strShortcutPath, 0);
			if (nExist != -1)
			{
				::DeleteFile(strShortcutPath);
			}

			strTxt.Format(_T("%sTempIcon\\%d.txt"), strCurrentPath, i+1);
			strIcon.Format(_T("%sTempIcon\\%d.ico"), strCurrentPath, i+1);

			WritePrivateProfileString(_T("InternetShortcut"), _T("IconFile"), strIcon, strTxt);

			nExist = _waccess(strTxt, 0);
			if (nExist != -1)
			{				
				DesktopIcon::FileCopy(strTxt, strShortcutPath);
			}
		}

		//아이콘 새로고침
		//SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	}

	//리소스 폴더 삭제
	DesktopIcon::DeleteFolder(strCurrentPath + CString(_T("\\TempIcon")));
}