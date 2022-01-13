#include "pch.h"
#include "ComboBox_test.h"
#include "afxdialogex.h"
#include "LeftDlg.h"

IMPLEMENT_DYNAMIC(LeftDlg, CDialogEx)

LeftDlg::LeftDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEFT_DIALOG, pParent)
{
	m_pbgImage = NULL;

	m_nWidth = 0;
	m_nHeight = 0;
}

LeftDlg::~LeftDlg()
{
	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void LeftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbo1);
	DDX_Control(pDX, IDC_COMBO2, m_cbo2);
	DDX_Control(pDX, IDC_COMBO3, m_cbo3);
	DDX_Control(pDX, IDC_COMBO4, m_cbo4);
}

BEGIN_MESSAGE_MAP(LeftDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL LeftDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void LeftDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void LeftDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

BOOL LeftDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadImage();

	InitControls();

	InitData();

	return TRUE;
}

void LeftDlg::InitData()
{
	m_cbo1.IsArrowImage(TRUE);

	//ComboBoxEx Dropdown
	m_cbo1.AddString(_T("test1"));
	m_cbo1.AddString(_T("test2"));
	m_cbo1.AddString(_T("test3"));
	m_cbo1.AddString(_T("test4"));
	m_cbo1.AddString(_T("test5"));

	//ComboBoxEx Drop List
	m_cbo2.AddString(_T("test1"));
	m_cbo2.AddString(_T("test2"));
	m_cbo2.AddString(_T("test3"));
	m_cbo2.AddString(_T("test4"));
	m_cbo2.AddString(_T("test5"));

	//ComboBox Dropdown
	m_cbo3.AddString(_T("test1"));
	m_cbo3.AddString(_T("test2"));
	m_cbo3.AddString(_T("test3"));
	m_cbo3.AddString(_T("test4"));
	m_cbo3.AddString(_T("test5"));

	//ComboBox Drop List
	m_cbo4.AddString(_T("test1"));
	m_cbo4.AddString(_T("test2"));
	m_cbo4.AddString(_T("test3"));
	m_cbo4.AddString(_T("test4"));
	m_cbo4.AddString(_T("test5"));

	m_cbo1.SetCurSel(0);
	m_cbo2.SetCurSel(0);
	m_cbo3.SetCurSel(0);
	m_cbo4.SetCurSel(0);
}

void LeftDlg::InitControls()
{
	CRect rc;
	GetClientRect(&rc);
}

void LeftDlg::SetParent(CComboBoxtestDlg* pParent)
{
	m_pParent = pParent;
}

void LeftDlg::LoadImage()
{
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	if (!m_pbgImage)
	{
		m_pbgImage = new CGdiPlusBitmapResource;

		if (!m_pbgImage->Load(MAKEINTRESOURCE(IDB_PNG1_DLG), _T("PNG"), AfxGetApp()->m_hInstance))
			return;
	}

	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();
	MoveWindow(0, 0, x, y);

	m_nWidth = x;
	m_nHeight = y;
}

void LeftDlg::DrawSkin(CDC* pDC)
{
	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();

	Graphics gps(pDC->GetSafeHdc());
	gps.DrawImage(m_pbgImage->m_pBitmap, Rect(0, 0, x, y), 0, 0, x, y, UnitPixel);
}
