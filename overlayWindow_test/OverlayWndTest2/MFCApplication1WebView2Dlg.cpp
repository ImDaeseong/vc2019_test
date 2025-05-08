#include "pch.h"
#include "framework.h"
#include "MFCApplication1WebView2.h"
#include "MFCApplication1WebView2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMFCApplication1WebView2Dlg::CMFCApplication1WebView2Dlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MFCAPPLICATION1WEBVIEW2_DIALOG, pParent)
{
	m_pWebBrowser = nullptr;
	m_overWnd = nullptr;
}

void CMFCApplication1WebView2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1WebView2Dlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_DESTROY()
	ON_MESSAGE(WM_USER_WEBDATA1, OnMessage1)
	ON_MESSAGE(WM_USER_WEBDATA2, OnMessage2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1WebView2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1WebView2Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CMFCApplication1WebView2Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

	CoInitialize(nullptr);

    return TRUE;
}

void CMFCApplication1WebView2Dlg::OnPaint()
{
    CPaintDC dc(this);
}

void CMFCApplication1WebView2Dlg::OnDestroy()
{
    CDialogEx::OnDestroy();

	destroyOverlay();
	destroyWebBrowser();
	CoUninitialize();
}

void CMFCApplication1WebView2Dlg::setBrowserMode(BOOL bShow)
{
	CRect rRect;
	GetClientRect(&rRect);

	if (bShow)
	{
		CRect rRc(10, 10, rRect.Width() - 100, rRect.Height() - 40);

		if (m_pWebBrowser == nullptr)
		{
			m_pWebBrowser = std::make_unique<CWebBrowser>();
			if (m_pWebBrowser != nullptr)
			{
				m_pWebBrowser->CreateAsync(
					WS_VISIBLE | WS_CHILD,
					rRc,
					this,
					1,
					[this]() {
						CString strParam("");
						CString content(strParam);
						CString headers(_T("Content-Type: application/x-www-form-urlencoded"));
						m_pWebBrowser->SetParentView(this);
						m_pWebBrowser->DisablePopups();
						m_pWebBrowser->NavigatePost(L"https://www.naver.com", content, headers, this->GetSafeHwnd());
						m_pWebBrowser->RegisterCallback(CWebBrowser::CallbackType::TitleChanged, [this]() {
							CString title = m_pWebBrowser->GetTitle();
							AfxGetMainWnd()->SetWindowText(title);
							});
					});
			}
		}
		else
		{
			m_pWebBrowser->MoveWindow(&rRc);
		}

		if (m_pWebBrowser != nullptr)
		{
			CRect rcBrowser;
			m_pWebBrowser->GetWindowRect(&rcBrowser);
			CRect rcWeb(rcBrowser.left + 10, rcBrowser.top + 10, rcBrowser.left + 400, rcBrowser.top + 100);

			if (m_overWnd == NULL)
			{
				//정적 변수로 한 번만 등록
				static LPCTSTR s_overlayClass = AfxRegisterWndClass(0);

				m_overWnd = new COverlayWnd();
				m_overWnd->CreateEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
					s_overlayClass, NULL, WS_POPUP | WS_VISIBLE, rcWeb, this, 0);
				
				//마우스 이벤트 무시하게
				::SetWindowLong(m_overWnd->GetSafeHwnd(), GWL_EXSTYLE, ::GetWindowLong(m_overWnd->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_TRANSPARENT);

				m_overWnd->UpdateDrawBackGround();
				//m_overWnd->MoveWindow(rcWeb);
			}
			else
			{
				m_overWnd->MoveWindow(rcWeb);
			}
		}
	}
	else
	{
		destroyOverlay();
		destroyWebBrowser();
	}	
}

void CMFCApplication1WebView2Dlg::destroyOverlay()
{
	if (m_overWnd)
	{
		if (m_overWnd->GetSafeHwnd())
		{
			m_overWnd->DestroyWindow();
		}
		delete m_overWnd;
		m_overWnd = nullptr;
	}
}

void CMFCApplication1WebView2Dlg::destroyWebBrowser()
{
	if (m_pWebBrowser)
	{
		if (m_pWebBrowser->GetSafeHwnd())
		{
			m_pWebBrowser->DestroyWindow();
		}
		m_pWebBrowser = nullptr;
	}
}

LRESULT CMFCApplication1WebView2Dlg::OnMessage1(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

LRESULT CMFCApplication1WebView2Dlg::OnMessage2(WPARAM wParam, LPARAM lParam)
{
	if (m_pWebBrowser != nullptr)
	{
		CRect rectClient;
		GetClientRect(rectClient);
		m_pWebBrowser->MoveWindow(10, 10, rectClient.Width() - 100, rectClient.Height() - 40);
	}

	return 0;
}

void CMFCApplication1WebView2Dlg::OnBnClickedButton1()
{
	setBrowserMode(TRUE);
}

void CMFCApplication1WebView2Dlg::OnBnClickedButton2()
{
	setBrowserMode(FALSE);
}
