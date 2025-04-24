﻿#include "pch.h"
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
	m_overWndEx = nullptr;
	m_bShow = TRUE;
}

void CMFCApplication1WebView2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1WebView2Dlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1WebView2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1WebView2Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CMFCApplication1WebView2Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

	//setBrowserMode(TRUE);
	setBrowserModeEx(TRUE);

    return TRUE;
}

void CMFCApplication1WebView2Dlg::OnPaint()
{
    CPaintDC dc(this);
}

void CMFCApplication1WebView2Dlg::OnDestroy()
{
    CDialogEx::OnDestroy();

	closeBrowserMode();
	closeBrowserModeEx();
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
			//1번째 방법
			m_overWnd = new COverlayWnd();
			m_overWnd->CreateEx(WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
				NULL, _T("title"), WS_POPUP | WS_VISIBLE, rRc, NULL, 0);

			m_overWnd->setDrawBg();
			m_overWnd->setDrawText(_T("최상위 비활성화 원도우"));
			m_overWnd->setDrawFont(_T("돋움"));
			m_overWnd->setDrawColor(RGB(255, 255, 0));
			m_overWnd->UpdateDrawText();

			//2번째 방법
			/*
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
			*/
		}
		else
		{
			m_overWnd->MoveWindow(rRc);
		}
	}
	else
	{
		closeBrowserMode();
	}	
}

void CMFCApplication1WebView2Dlg::closeBrowserMode()
{
	if (m_overWnd)
	{
		if (m_overWnd->GetSafeHwnd())
		{
			m_overWnd->DestroyWindow();
			delete m_overWnd;
			m_overWnd = nullptr;
		}
	}
}

void CMFCApplication1WebView2Dlg::setBrowserModeEx(BOOL bShow)
{
	m_bShow = bShow;

	CRect rRect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rRect, 0);

	if (bShow)
	{
		int nWidth = 460;
		int nHeight = 80;

		//중앙 위치
		int nLeft = rRect.left + (rRect.Width() - nWidth) / 2;
		int nTop = rRect.top + 10;//(rRect.Height() - nHeight) / 2;
		CRect rRc(nLeft, nTop, nLeft + nWidth, nTop + nHeight);

		if (m_overWndEx == NULL)
		{
			//1번째 방법
			m_overWndEx = new COverlayWndEx();
			m_overWndEx->CreateEx(WS_EX_NOACTIVATE | WS_EX_TRANSPARENT  | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
				NULL, _T("title"), WS_POPUP | WS_VISIBLE, rRc, NULL, 0);

			m_overWndEx->setDrawBg();
			m_overWndEx->setDrawText(_T("최상위 비활성화 "), _T("최상위 비활성화"));
			m_overWndEx->setDrawFont(_T("돋움"), _T("돋움"));
			m_overWndEx->setDrawColor(RGB(255, 0, 0), RGB(0, 255, 0));
			m_overWndEx->UpdateDrawText();

			//윈도우 투명도 설정
			m_overWndEx->SetLayeredWindowAttributes(RGB(0, 0, 0), 0, LWA_COLORKEY);

			m_overWndEx->ShowWindow(SW_SHOWNORMAL);
			m_overWndEx->UpdateWindow();

			//2번째 방법
			/*
			//정적 변수로 한 번만 등록
			static LPCTSTR s_overlayClass = AfxRegisterWndClass(0);

			m_overWndEx = new COverlayWndEx();
			m_overWndEx->CreateEx(WS_EX_NOACTIVATE | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
				s_overlayClass, NULL, WS_POPUP | WS_VISIBLE, rRc, NULL, 0);

			m_overWndEx->setDrawBg();
			m_overWndEx->setDrawText(_T("최상위 비활성화 "), _T("최상위 비활성화"));
			m_overWndEx->setDrawFont(_T("돋움"), _T("돋움"));
			m_overWndEx->setDrawColor(RGB(255, 0, 0), RGB(0, 255, 0));
			m_overWndEx->UpdateDrawText();
			m_overWndEx->MoveWindow(rRc);
			*/
		}
		else
		{
			m_overWndEx->MoveWindow(rRc);
		}
	}
	else
	{
		closeBrowserModeEx();
	}
}

void CMFCApplication1WebView2Dlg::closeBrowserModeEx()
{
	if (m_overWndEx)
	{
		if (m_overWndEx->GetSafeHwnd())
		{
			m_overWndEx->DestroyWindow();
			delete m_overWndEx;
			m_overWndEx = nullptr;
		}
	}
}

void CMFCApplication1WebView2Dlg::OnBnClickedButton1()
{
	//보이기
	if (m_overWndEx)
	{
		m_overWndEx->SetLayeredWindowAttributes(RGB(0, 0, 0), 0, LWA_COLORKEY);
	}

	//setBrowserMode(TRUE);
	//setBrowserModeEx(TRUE);
}

void CMFCApplication1WebView2Dlg::OnBnClickedButton2()
{
	//숨기기
	if (m_overWndEx)
	{
		m_overWndEx->SetLayeredWindowAttributes(0, 0, LWA_ALPHA);
	}

	//setBrowserMode(FALSE);
	//setBrowserModeEx(FALSE);
}
