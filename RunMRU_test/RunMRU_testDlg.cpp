#include "pch.h"
#include "framework.h"
#include "RunMRU_test.h"
#include "RunMRU_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRunMRUtestDlg::CRunMRUtestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RUNMRU_TEST_DIALOG, pParent)
{	
}

void CRunMRUtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mlist);
}

BEGIN_MESSAGE_MAP(CRunMRUtestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CRunMRUtestDlg::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CRunMRUtestDlg::OnSelchangeList1)
END_MESSAGE_MAP()

BOOL CRunMRUtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  
}

void CRunMRUtestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CRunMRUtestDlg::OnDestroy()
{

	CDialog::OnDestroy();
}

CString GetFilePath(CString strFilename)
{
	int nPos = strFilename.ReverseFind('\\');
	if (nPos > 0)
		return strFilename.Left(nPos);

	return strFilename;
}

void CRunMRUtestDlg::OnSelchangeList1()
{
	int nIndex = mlist.GetCurSel();
	if (nIndex == -1)
		return;

	CString strFilePath;
	mlist.GetText(nIndex, strFilePath);

	//선택된 리스트항목 폴더 오픈 
	HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), GetFilePath(strFilePath), NULL, NULL, SW_SHOW);
}

void CRunMRUtestDlg::OnBnClickedButton1()
{
	//리스트 초기화
	mlist.ResetContent();

	//레지스트리 조회
	objusage.InitLoad();

	//레지스트리 데이터 읽기
	CStringArray aryList;
	objusage.FindExe(aryList);

	//찾은 데이터 리스트 등록
	int nSize = aryList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strValue = aryList.GetAt(i);
		mlist.AddString(strValue);
	}
	aryList.RemoveAll();


	/*
	//입력값
	CString strInput;
	GetDlgItemText(IDC_EDIT1, strInput);

	//데이터 찾기
	CStringArray aryList;
	objusage.FindExe(strInput, aryList);

	//찾은 데이터 리스트 등록
	int nSize = aryList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strValue = aryList.GetAt(i);
		mlist.AddString(strValue);
	}
	aryList.RemoveAll();
	*/


	//테스트
	/* 
	//찾을 항목
	CStringArray strFileNamelist;
	strFileNamelist.Add(_T("LOSTARK.exe"));
	strFileNamelist.Add(_T("liteide.exe"));
	strFileNamelist.Add(_T("pycharm64.exe"));
	strFileNamelist.Add(_T("GoClean.exe"));
	strFileNamelist.Add(_T("studio64.exe"));


	CStringArray aryList;
	objusage.FindExe(strFileNamelist, aryList);
	strFileNamelist.RemoveAll();

	//찾은 데이터 리스트 등록
	int nSize = aryList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strValue = aryList.GetAt(i);
		mlist.AddString(strValue);
	}
	aryList.RemoveAll();
	*/


	//테스트
	/*
	//objusage.FindExe(_T("LOSTARK.exe"));
	//objusage.FindExe(_T("lostark.EXE"));
	
	CStringArray aryList;
	objusage.FindExe(_T("LOSTARK.exe"), aryList);

	int nSize = aryList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		mlist.AddString(strValue);
	}
	aryList.RemoveAll();
	*/
}

