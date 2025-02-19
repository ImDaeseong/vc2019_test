#include "pch.h"
#include "framework.h"
#include "WindowsSettingsURI_test.h"
#include "WindowsSettingsURI_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWindowsSettingsURItestDlg::CWindowsSettingsURItestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINDOWSSETTINGSURI_TEST_DIALOG, pParent)
{
}

void CWindowsSettingsURItestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWindowsSettingsURItestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CWindowsSettingsURItestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CWindowsSettingsURItestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  
}

void CWindowsSettingsURItestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CWindowsSettingsURItestDlg::OnBnClickedButton1()
{
	//원도우 알림이 켬인 경우에만 
	if (Common_Util::GetToastEnabled())
	{
		Common_Util::OpenWindowsSettingURI(18);
	}	
}
