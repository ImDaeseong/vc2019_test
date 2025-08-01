#include "pch.h"
#include "Layprogress_test.h"
#include "afxdialogex.h"
#include "NormalDlg.h"

IMPLEMENT_DYNAMIC(NormalDlg, CDialogEx)

NormalDlg::NormalDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_NORMAL, pParent)
{
	m_pbgImage = NULL;

	m_nCount = 0;
}

NormalDlg::~NormalDlg()
{
	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void NormalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(NormalDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL NormalDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void NormalDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);

	if (m_pbgImage)
		delete m_pbgImage;
	m_pbgImage = NULL;
}

void NormalDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

BOOL NormalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadSkin();

	InitControls();

	CenterWindow(GetDesktopWindow());

	CDC* pDC = GetDC();
	DrawSkin(pDC);
	ReleaseDC(pDC);

	SetTimer(1, 1000, NULL);

	return TRUE;
}

void NormalDlg::SetParent(CLayprogresstestDlg* pParent)
{
	m_pParent = pParent;
}

void NormalDlg::LoadSkin()
{
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	if (!m_pbgImage)
	{
		m_pbgImage = new CGdiPlusBitmapResource;

		if (!m_pbgImage->Load(MAKEINTRESOURCE(IDB_PNG1), _T("PNG"), AfxGetApp()->m_hInstance))
			return;
	}

	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();
	MoveWindow(0, 0, x, y);
}

void NormalDlg::DrawSkin(CDC* pDC)
{
	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();

	Graphics gps(pDC->GetSafeHdc());
	gps.DrawImage(m_pbgImage->m_pBitmap, Rect(0, 0, x, y), 0, 0, x, y, UnitPixel);
}

void NormalDlg::InitControls()
{
	m_CircularBar.SubclassDlgItem(IDC_STATIC_CIRCUR1, this);
	m_CircularBar.MoveWindow(690, 382, 60, 60);
	m_CircularBar.Start();

	m_RoundLineBar.SubclassDlgItem(IDC_STATIC_LINE1, this);
	m_RoundLineBar.MoveWindow(522, 552, 396, 27);
	m_RoundLineBar.SetRange(0, 100);//범위 설정 (예: 0~100)	
	m_RoundLineBar.SetPos(0);// 진행률 설정 (예: 30%)

	m_MsgMessage1.SubclassDlgItem(IDC_STATIC_MESSAGE1, this);
	m_MsgMessage1.MoveWindow(582, 458, 300, 70);
	m_MsgMessage1.SetMesssageFontSize(26);
	m_MsgMessage1.SetMesssageText(_T("업데이트 중입니다.\r\n잠시만 기다려 주세요."));

	m_MsgMessage2.SubclassDlgItem(IDC_STATIC_MESSAGE2, this);
	m_MsgMessage2.MoveWindow(925, 551, 75, 27);
	m_MsgMessage2.SetMesssageFontSize(20);
	m_MsgMessage2.SetMesssageText(_T("0%"));
}

void NormalDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_nCount += 10;
	if (m_nCount > 100)
		m_nCount = 0;

	m_RoundLineBar.SetPos(m_nCount); // 진행률: 0.0 ~ 1.0

	CString strMsg;
	strMsg.Format(_T("%d%%"), m_nCount);
	m_MsgMessage2.SetMesssageText(strMsg);
	m_MsgMessage2.Invalidate();  
	
	CDialogEx::OnTimer(nIDEvent);
}