#include "pch.h"
#include "ProgressBar_test.h"
#include "afxdialogex.h"
#include "LeftBottomDlg.h"

IMPLEMENT_DYNAMIC(LeftBottomDlg, CDialogEx)

LeftBottomDlg::LeftBottomDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEFT_BT_DIALOG, pParent)
{
	m_nWidth = 268;
	m_nHeight = 150;

	m_nCount = 0;
}

LeftBottomDlg::~LeftBottomDlg()
{
}

void LeftBottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_PNG, m_StcProgressPng);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_PNGA, m_StcProgressPngA);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_StcProgress);
	DDX_Control(pDX, IDC_STATIC_MSG, m_StcMsg);
}

BEGIN_MESSAGE_MAP(LeftBottomDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL LeftBottomDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void LeftBottomDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
}

void LeftBottomDlg::OnPaint()
{
	CPaintDC dc(this);
}

BOOL LeftBottomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitControls();

	SetTimer(1, 1000, NULL);

	return TRUE;
}

void LeftBottomDlg::SetParent(CProgressBartestDlg* pParent)
{
	m_pParent = pParent;
}

void LeftBottomDlg::InitControls()
{
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	MoveWindow(0, 0, m_nWidth, m_nHeight);

	m_StcProgressPng.SetNoImage();
	m_StcProgressPng.MoveWindow(10, 50, m_nWidth - 20, 20);
	m_StcProgressPng.SetRange(0, 100);
	m_StcProgressPng.SetPos(0);

	m_StcProgressPngA.SetImage(MAKEINTRESOURCE(IDB_PNG_BAR1), MAKEINTRESOURCE(IDB_PNG_BAR2), _T("PNG"), AfxGetApp()->m_hInstance);
	m_StcProgressPngA.MoveWindow(10, 80, m_nWidth - 20, 20);
	m_StcProgressPngA.SetRange(0, 100);
	m_StcProgressPngA.SetPos(0);

	m_StcProgress.MoveWindow(10, 110, m_nWidth - 20, 20);
	m_StcProgress.SetRange(0, 100);
	m_StcProgress.SetPos(0);

	m_StcMsg.MoveWindow(10, 20, m_nWidth - 20, 20);
	m_StcMsg.SetMesssageFontSize(14);
	m_StcMsg.SetMesssageText(_T("LeftBottomDlg"));
}

void LeftBottomDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_nCount = m_nCount + 10;
	if (m_nCount > 100)
		m_nCount = 0;

	m_StcProgressPng.SetPos(m_nCount);

	m_StcProgressPngA.SetPos(m_nCount);

	m_StcProgress.SetPos(m_nCount);

	CDialogEx::OnTimer(nIDEvent);
}