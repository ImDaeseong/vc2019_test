#include "pch.h"
#include "framework.h"
#include "ShellCommandExe.h"
#include "ShellCommandExeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CShellCommandExeDlg::CShellCommandExeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHELLCOMMANDEXE_DIALOG, pParent)
{
}

void CShellCommandExeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShellCommandExeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CShellCommandExeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShellCommandExeDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CShellCommandExeDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

BOOL CShellCommandExeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
		
	CString strCommand = Common_Util::GetCommand();
	OutputDebugString(strCommand);

	return TRUE;  
}

void CShellCommandExeDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CShellCommandExeDlg::OnBnClickedButton1()
{
	//html 파일에서 exe 실행 파일 호출을 위한 레지스트리 등록 
	Common_Util::SetCommand(_T("C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe"));
}

void CShellCommandExeDlg::OnBnClickedButton2()
{
	//등록 정보 삭제
	Common_Util::DeleteCommand();
}

void CShellCommandExeDlg::OnBnClickedButton3()
{
	//html 실행
	CString strPath;
	strPath.Format(_T(".\\html\\index.htm"));
	::ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOW);
}
