#include "pch.h"
#include "framework.h"
#include "RecurseDirectories_test.h"
#include "RecurseDirectories_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRecurseDirectoriestestDlg::CRecurseDirectoriestestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RECURSEDIRECTORIES_TEST_DIALOG, pParent)
{
	m_pThread = NULL;
}

void CRecurseDirectoriestestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mlist1);
	DDX_Control(pDX, IDC_LIST2, mlist2);
}

BEGIN_MESSAGE_MAP(CRecurseDirectoriestestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CRecurseDirectoriestestDlg::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CRecurseDirectoriestestDlg::OnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CRecurseDirectoriestestDlg::OnSelchangeList2)
END_MESSAGE_MAP()

BOOL CRecurseDirectoriestestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  
}

void CRecurseDirectoriestestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CRecurseDirectoriestestDlg::OnDestroy()
{
	CloseThread();

	CDialog::OnDestroy();
}

void CRecurseDirectoriestestDlg::OnSelchangeList1()
{
	int nIndex = mlist1.GetCurSel();
	if (nIndex == -1)
		return;

	CString strFilePath;
	mlist1.GetText(nIndex, strFilePath);

	//선택된 리스트항목 폴더 오픈 
	HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), Searchfilegame::GetFilePath(strFilePath), NULL, NULL, SW_SHOW);
}

void CRecurseDirectoriestestDlg::OnSelchangeList2()
{
	int nIndex = mlist2.GetCurSel();
	if (nIndex == -1)
		return;

	CString strFilePath;
	mlist2.GetText(nIndex, strFilePath);

	//선택된 리스트항목 폴더 오픈 
	HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), Searchfilegame::GetFilePath(strFilePath), NULL, NULL, SW_SHOW);
}

void CRecurseDirectoriestestDlg::OnBnClickedButton1()
{	
	StartThread();
}

void CRecurseDirectoriestestDlg::StartThread()
{
	if (m_pThread == NULL)
	{
		m_pThread = new SearchThread;
		m_pThread->SetThread(this);
		m_pThread->CreateThread();
	}
}

void CRecurseDirectoriestestDlg::CloseThread()
{
	if (m_pThread)
	{
		m_pThread->ResumeThread();
		delete 	m_pThread;
	}
}

void CRecurseDirectoriestestDlg::runThread()
{
	//시작시간
	CTime tStart = CTime::GetCurrentTime();


	//리스트 초기화
	mlist1.ResetContent();
	mlist2.ResetContent();


	CStringArray drivelist;
	CStringArray folderList;
	CStringArray fileList;
	

	//존재하는 드라이브 조사	
	Searchfilegame::getDriveList(drivelist);	
	int nSize = drivelist.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strRootDir = drivelist.GetAt(i);
		Searchfilegame::RecurseDirectories(folderList, fileList, strRootDir, 0);
	}



	//--list 에 데이터 입력시 cpu 튄다	
	
	//조사한 폴더 목록
	nSize = folderList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strDir = folderList.GetAt(i);
		mlist1.AddString(strDir);
	}

	//찾은 파일 목록
	nSize = fileList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strFile = fileList.GetAt(i);
		mlist2.AddString(strFile);
	}

	//--list 에 데이터 입력시 cpu 튄다



	//완료 시간
	CTime tEnd = CTime::GetCurrentTime();
	CTimeSpan tResult = tEnd - tStart;


	UpdateData();

	CString sMsg;
	sMsg += tResult.Format("%M:%S\n");
	SetDlgItemText(IDC_STATIC_TIME, sMsg);
}