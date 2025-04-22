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
	m_overWnd = nullptr;
	m_bShow = TRUE;
}

void CMFCApplication1WebView2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1WebView2Dlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1WebView2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1WebView2Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CMFCApplication1WebView2Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

	setBrowserMode(TRUE);

	SetTimer(1, 5000, NULL);
	SetTimer(2, 1000, NULL);

    return TRUE;
}

void CMFCApplication1WebView2Dlg::OnPaint()
{
    CPaintDC dc(this);
}

void CMFCApplication1WebView2Dlg::OnDestroy()
{
    CDialogEx::OnDestroy();

	if (m_overWnd)
	{
		if (m_overWnd->GetSafeHwnd())
		{
			m_overWnd->DestroyWindow();
		}
		delete m_overWnd;
		m_overWnd = nullptr;
	}

	KillTimer(1);
	KillTimer(2);
}

void CMFCApplication1WebView2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		setBrowserMode(m_bShow);
		m_bShow = !m_bShow;
	}
	else if (nIDEvent == 2)
	{
		if (m_overWnd)
		{
			if (IsTopmostWindow(m_overWnd->GetSafeHwnd()))
			{
				//다른 TOPMOST 윈도우가 존재
			}
			else
			{
				//m_overWnd가 최상위 TOPMOST 윈도우
			}
		}
	}
}

BOOL CMFCApplication1WebView2Dlg::IsTopmostWindow(HWND hCheckWnd)
{
	HWND hTopWnd = ::GetTopWindow(NULL);
	while (hTopWnd)
	{
		if (hTopWnd == hCheckWnd)
			return FALSE;

		LONG_PTR exStyle = ::GetWindowLongPtr(hTopWnd, GWL_EXSTYLE);
		if ((exStyle & WS_EX_TOPMOST) && ::IsWindowVisible(hTopWnd))
			return TRUE;

		hTopWnd = ::GetNextWindow(hTopWnd, GW_HWNDNEXT);
	}
	return FALSE;
}

void CMFCApplication1WebView2Dlg::setBrowserMode(BOOL bShow)
{
	m_bShow = bShow;

	CRect rRect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rRect, 0);

	if (bShow)
	{
		int nWidth = 460;
		int nHeight = 40;

		//중앙 위치
		int nLeft = rRect.left + (rRect.Width() - nWidth) / 2;
		int nTop = rRect.top + 50;//(rRect.Height() - nHeight) / 2;
		CRect rRc(nLeft, nTop, nLeft + nWidth, nTop + nHeight);

		if (m_overWnd == nullptr)
		{
			//정적 변수로 한 번만 등록
			static LPCTSTR s_overlayClass = AfxRegisterWndClass(0);

			m_overWnd = new COverlayWnd();
			m_overWnd->CreateEx(WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
				s_overlayClass, NULL, WS_POPUP | WS_VISIBLE, rRc, NULL, 0);

			m_overWnd->setDrawBg();
			m_overWnd->setDrawText(_T("최상위 비활성화 원도우"));
			m_overWnd->setDrawFont(_T("돋움"));
			m_overWnd->setDrawColor(RGB(255, 255, 0));
			m_overWnd->UpdateDrawText();
			m_overWnd->MoveWindow(rRc);
		}
		else
		{
			m_overWnd->MoveWindow(rRc);
		}

		/*
		//FullScreen TOPMOST 원도우가 있는 경우 필요
		::SetWindowPos(m_overWnd->GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        */
	}
	else
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
}

void CMFCApplication1WebView2Dlg::OnBnClickedButton1()
{
	setBrowserMode(TRUE);
}

void CMFCApplication1WebView2Dlg::OnBnClickedButton2()
{
	setBrowserMode(FALSE);
}
