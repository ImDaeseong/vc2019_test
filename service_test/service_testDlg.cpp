#include "pch.h"
#include "framework.h"
#include "service_test.h"
#include "service_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CservicetestDlg::CservicetestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVICE_TEST_DIALOG, pParent)
{
	strServicePath = _T("E:\\Bin\\Release\\DaeSeongService.exe");	
}

void CservicetestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CservicetestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CservicetestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CservicetestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CservicetestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CservicetestDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

BOOL CservicetestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SetDlgItemText(IDC_STATIC_TEXT, _T(""));

	return TRUE; 
}

void CservicetestDlg::OnPaint()
{
	CPaintDC dc(this);

}

//설치/삭제
void CservicetestDlg::OnBnClickedButton1()
{
	CheckServiceManager obj;

	if (obj.IsInstallService())
	{
		BOOL bUnInstall = obj.UninstallService();

		strMsg.Format(_T("UninstallService:%d"), bUnInstall);
		SetDlgItemText(IDC_STATIC_TEXT, strMsg);
	}
	else
	{
		BOOL bInstall = obj.InstallIService(strServicePath);

		strMsg.Format(_T("InstallIService:%d"), bInstall);
		SetDlgItemText(IDC_STATIC_TEXT, strMsg);
	}	
}

//실행
void CservicetestDlg::OnBnClickedButton2()
{		
	CheckServiceManager obj;

	if (!obj.IsRunService())
	{
		if (!obj.StartService())
		{
			strMsg.Format(_T("GetServiceStatus StartService failed"));
			SetDlgItemText(IDC_STATIC_TEXT, strMsg);
		}
	}
	
	DWORD dwStatus = obj.GetServiceStatus();
	strMsg.Format(_T("GetServiceStatus:%d"), dwStatus);
	SetDlgItemText(IDC_STATIC_TEXT, strMsg);
}

//중지
void CservicetestDlg::OnBnClickedButton3()
{
	CheckServiceManager obj;

	if (obj.IsRunService())
	{
		obj.StopService();
	}
	
	DWORD dwStatus = obj.GetServiceStatus();
	strMsg.Format(_T("GetServiceStatus:%d"), dwStatus);
	SetDlgItemText(IDC_STATIC_TEXT, strMsg);
}

//재실행 - 서비스 시작 실패시 서비스 삭제후 재설치후 실행
void CservicetestDlg::OnBnClickedButton4()
{
	CheckServiceManager obj;

	if (obj.IsRunService())
	{
		obj.StopService();		
	}
	else
	{
		if (!obj.StartService())
		{
			BOOL bUnInstall = obj.UninstallService();
			if (bUnInstall)
			{
				BOOL bInstall = obj.InstallIService(strServicePath);
				if (bInstall)
				{
					obj.StartService();
				}
			}
		}		
	}
}
