#include "pch.h"
#include "framework.h"
#include "Tray_test.h"
#include "Tray_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTraytestDlg::CTraytestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAY_TEST_DIALOG, pParent)
{
	m_pWebBrowser = nullptr;
	m_pbgImage = NULL;

	m_nWidth = 0;
	m_nHeight = 0;
}

void CTraytestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTraytestDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_MOVE()
END_MESSAGE_MAP()

BOOL CTraytestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CTraytestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		KillTimer(1);
		SetTimer(2, 2000, NULL);
		ShowTray();
		ResizeWebBrowser();
	}
	else if (nIDEvent == 2)
	{
		KillTimer(2);
		HideTray();
		CDialog::OnOK();
	}
}

void CTraytestDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	//ResizeWebBrowser();
}

void CTraytestDlg::OnDestroy()
{
	KillTimer(1);
	KillTimer(2);

	if (m_pbgImage != NULL)
		delete m_pbgImage;
	m_pbgImage = NULL;

	CoUninitialize();
}

BOOL CTraytestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CoInitialize(NULL);

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	BringToForeground();

	LoadImage();

	MoveWindow(-3000, -3000, 0, 0);

	InitWebBrowser();

	SetTimer(1, 2000, NULL);

	return TRUE;
}

void CTraytestDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

void CTraytestDlg::LoadImage()
{
	if (!m_pbgImage)
	{
		m_pbgImage = new CGdiPlusBitmapResource;
		if (!m_pbgImage->Load(MAKEINTRESOURCE(IDB_PNG_BG), _T("PNG"), AfxGetApp()->m_hInstance))
			return;
	}

	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();
	MoveWindow(0, 0, x, y);

	m_nWidth = x;
	m_nHeight = y;
}

void CTraytestDlg::DrawSkin(CDC* pDC)
{
	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();

	Graphics gps(pDC->GetSafeHdc());
	gps.DrawImage(m_pbgImage->m_pBitmap, Rect(0, 0, x, y), 0, 0, x, y, UnitPixel);
}

void CTraytestDlg::InitWebBrowser()
{
	//CRect rectClient;
	//GetClientRect(rectClient);
	CRect rectClient(-3000, -3000, 0, 0);

	m_pWebBrowser = std::make_unique<CWebBrowser>();
	if (m_pWebBrowser != nullptr)
	{
		m_pWebBrowser->CreateAsync(
			WS_VISIBLE | WS_CHILD,
			rectClient,
			this,
			1,
			[this]() {

				CString strParam("");
				CString content(strParam);
				CString headers(_T("Content-Type: application/x-www-form-urlencoded"));
				
				m_pWebBrowser->SetParentView(this);
				m_pWebBrowser->DisablePopups();
				m_pWebBrowser->NavigatePost(L"https://www.naver.com", content, headers, this->GetSafeHwnd());
				
				// 타이틀 변경 콜백 등록
				m_pWebBrowser->RegisterCallback(CWebBrowser::CallbackType::TitleChanged, [this]() {
					CString title = m_pWebBrowser->GetTitle();
					AfxGetMainWnd()->SetWindowText(title);
					});
			});
	}
}

void CTraytestDlg::ResizeWebBrowser()
{
	if (m_pWebBrowser != nullptr)
	{
		CRect rect;
		GetClientRect(&rect);
		m_pWebBrowser->MoveWindow(&rect, TRUE);
	}	
}

void CTraytestDlg::BringToForeground()
{
	if (::GetForegroundWindow() == m_hWnd)
		return;

	HWND hActiveWnd = ::GetForegroundWindow();
	if (!hActiveWnd)
		return;

	DWORD activeThreadId = GetWindowThreadProcessId(hActiveWnd, nullptr);
	DWORD currentThreadId = GetCurrentThreadId();

	if (currentThreadId != activeThreadId)
	{
		if (AttachThreadInput(currentThreadId, activeThreadId, TRUE))
		{
			BringWindowToTop();

			//포커스 필요시 사용
			//SetForegroundWindow();

			AttachThreadInput(currentThreadId, activeThreadId, FALSE);
		}
	}
}

void CTraytestDlg::ShowTray()
{
	int nScreenSizeX = GetSystemMetrics(SM_CXSCREEN);
	int nScreenSizeY = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(nScreenSizeX, nScreenSizeY, m_nWidth, m_nHeight);

	::ShowTray(this->m_hWnd, 1000, TRUE, TRUE);
}

void CTraytestDlg::HideTray()
{
	::ShowTray(this->m_hWnd, 500, FALSE, TRUE);
}
