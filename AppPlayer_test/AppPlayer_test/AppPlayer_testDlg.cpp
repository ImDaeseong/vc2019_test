#include "pch.h"
#include "framework.h"
#include "AppPlayer_test.h"
#include "AppPlayer_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAppPlayertestDlg::CAppPlayertestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_APPPLAYER_TEST_DIALOG, pParent)
{
}

void CAppPlayertestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAppPlayertestDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CAppPlayertestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//NoxInfo();
	//LDPlayer4Info();
	//BlueStacks4Info();
	BlueStacks5Info();
		
	return TRUE; 
}

void CAppPlayertestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CAppPlayertestDlg::NoxInfo()
{
	//Nox 설치경로
	CString strNoxPath = Common::GetFilePath(objNox.getNoxPath());
	
	//Nox 설치버전
	CString strNoxVersion = Common::GetFilePath(objNox.getNoxVersion());
	
	//Nox run
	//BOOL bRunNox = objNox.DefaultRunNox();
	
	//Nox game run 
	BOOL bRunNox = objNox.RunNox(_T("com.nexon.baram"));
}

void CAppPlayertestDlg::LDPlayer4Info()
{
	//LDPlayer4 설치경로
	CString strLDPlayer4Path = Common::GetFilePath(objLDPlayer4.getLDPlayer4Path());

	//LDPlayer4 설치버전
	CString strLDPlayer4Version = Common::GetFilePath(objLDPlayer4.getLDPlayer4Version());

	//LDPlayer4 run
	//BOOL bLDPlayer4 = objLDPlayer4.DefaultRunLDPlayer4();
	
	//LDPlayer4 game run 
	BOOL bLDPlayer4 = objLDPlayer4.RunLDPlayer4(_T("com.miHoYo.bh3korea"));
}

void CAppPlayertestDlg::BlueStacks4Info()
{
	//BlueStacks4 설치경로
	CString strBlueStacks5Path = Common::GetFilePath(objBlueStacks4.getBlueStacks4Path());

	//BlueStacks4 설치버전
	CString strBlueStacks5Version = Common::GetFilePath(objBlueStacks4.getBlueStacks4Version());

	//BlueStacks4 run
	//BOOL bRunBlueStacks4 = objBlueStacks4.DefaultRunBlueStacks4();

	//BlueStacks4 game run 
	BOOL bRunBlueStacks4 = objBlueStacks4.RunBlueStacks4(_T("com.nexon.baram"), _T("net.supercat.MainActivity"));
}

void CAppPlayertestDlg::BlueStacks5Info()
{
	//BlueStacks5 설치경로
	CString strBlueStacks5Path = Common::GetFilePath(objBlueStacks5.getBlueStacks5Path());

	//BlueStacks5 설치버전
	CString strBlueStacks5Version = Common::GetFilePath(objBlueStacks5.getBlueStacks5Version());

	//BlueStacks5 run
	//BOOL bRunBlueStacks5 = objBlueStacks5.DefaultRunBlueStacks5();
	 
	//BlueStacks5 game run 
	BOOL bRunBlueStacks5 = objBlueStacks5.RunBlueStacks5(_T("com.nexon.baram"), _T("net.supercat.MainActivity"));
}