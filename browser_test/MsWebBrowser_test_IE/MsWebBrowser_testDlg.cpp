#include "pch.h"
#include "framework.h"
#include "MsWebBrowser_test.h"
#include "MsWebBrowser_testDlg.h"
#include "afxdialogex.h"
#include "TopDlg.h"
#include "CenterDlg.h"
#include "BottomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMsWebBrowsertestDlg::CMsWebBrowsertestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MSWEBBROWSER_TEST_DIALOG, pParent)
{
	m_pTopDlg = NULL;
	m_pCenterDlg = NULL;
	m_pBottomDlg = NULL;

	m_pbgImage = NULL;
	m_LoadedFlag = FALSE;
}

void CMsWebBrowsertestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_WebBrowser);
}

BEGIN_MESSAGE_MAP(CMsWebBrowsertestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
END_MESSAGE_MAP()

void CMsWebBrowsertestDlg::CreateTopDlg()
{
	if (!m_pTopDlg)
	{
		m_pTopDlg = new TopDlg();
		m_pTopDlg->SetParent(this);
		m_pTopDlg->Create(IDD_TOP_DIALOG, this);
		SetChildPos();
		m_pTopDlg->ShowWindow(SW_SHOW);
	}
}

void CMsWebBrowsertestDlg::DestroyTopDlg()
{
	if (m_pTopDlg)
	{
		m_pTopDlg->DestroyWindow();
		delete m_pTopDlg;
		m_pTopDlg = NULL;
	}
}

void CMsWebBrowsertestDlg::CreateCenterDlg()
{
	if (!m_pCenterDlg)
	{
		m_pCenterDlg = new CenterDlg();
		m_pCenterDlg->SetParent(this);
		m_pCenterDlg->Create(IDD_CENTER_DIALOG, this);
		SetChildPos();
		m_pCenterDlg->ShowWindow(SW_SHOW);
	}
}

void CMsWebBrowsertestDlg::DestroyCenterDlg()
{
	if (m_pCenterDlg)
	{
		m_pCenterDlg->DestroyWindow();
		delete m_pCenterDlg;
		m_pCenterDlg = NULL;
	}
}

void CMsWebBrowsertestDlg::CreateBottomDlg()
{
	if (!m_pBottomDlg)
	{
		m_pBottomDlg = new BottomDlg();
		m_pBottomDlg->SetParent(this);
		m_pBottomDlg->Create(IDD_BOTTOM_DIALOG, this);
		SetChildPos();
		m_pBottomDlg->SetStartTimer(10);
		m_pBottomDlg->ShowWindow(SW_SHOW);
	}
}

void CMsWebBrowsertestDlg::DestroyBottomDlg()
{
	if (m_pBottomDlg)
	{
		m_pBottomDlg->DestroyWindow();
		delete m_pBottomDlg;
		m_pBottomDlg = NULL;
	}
}

void CMsWebBrowsertestDlg::SetChildPos()
{
	CRect rc;
	GetWindowRect(rc);

	if (m_pTopDlg)
	{
		int nX = rc.right - (m_pTopDlg->GetWidth() + 30);
		int nY = rc.top + 30;
		m_pTopDlg->MoveWindow(nX, nY, m_pTopDlg->GetWidth(), m_pTopDlg->GetHeight());
	}

	if (m_pCenterDlg)
	{
		int nX = rc.left + 5;
		int nY = rc.top + 5;
		m_pCenterDlg->MoveWindow(nX, nY, 588, 344);
	}

	if (m_pBottomDlg)
	{
		int nX = rc.right - (m_pTopDlg->GetWidth() + 30);
		int nY = rc.bottom - 90;
		m_pBottomDlg->MoveWindow(nX, nY, m_pBottomDlg->GetWidth(), m_pBottomDlg->GetHeight());
	}
}

void CMsWebBrowsertestDlg::CallMain()
{
	
}

BOOL CMsWebBrowsertestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Common_Util::SetBrowserEmulation(_T("MsWebBrowser_test.exe"));

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	LoadImage();

	InitWebBrowser();

	CenterWindow(GetDesktopWindow());

	return TRUE; 
}

void CMsWebBrowsertestDlg::InitWebBrowser()
{
	CRect rc;
	GetClientRect(&rc);

	m_WebBrowser.MoveWindow(rc.left + 5, rc.top + 5, rc.Width() - 10, rc.Height() - 10);

	CString strurl;
	strurl.Format(_T("E:\\vc2019_test\\MsWebBrowser_test\\html\\index.html"));
	NavigateA(strurl);
}

void CMsWebBrowsertestDlg::NavigateA(CString strURL)
{
	VARIANT vtHeader, vtTarget, vtEmpty;
	CString strHeader, strTarget;

	strHeader = "Content-Type: application/x-www-form-urlencoded\r\n";

	strTarget = "_parent";

	::VariantInit(&vtHeader);
	::VariantInit(&vtTarget);
	::VariantInit(&vtEmpty);

	vtHeader.vt = VT_BSTR;

	vtHeader.bstrVal = strHeader.AllocSysString();

	vtTarget.vt = VT_BSTR;

	vtTarget.bstrVal = strTarget.AllocSysString();

	m_WebBrowser.Navigate(strURL, &vtEmpty, &vtTarget, &vtEmpty, &vtHeader);

	SysFreeString(vtHeader.bstrVal);
	SysFreeString(vtTarget.bstrVal);
}

void CMsWebBrowsertestDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

void CMsWebBrowsertestDlg::LoadImage()
{
	if (!m_pbgImage)
	{
		m_pbgImage = new CGdiPlusBitmapResource;

		if (!m_pbgImage->Load(MAKEINTRESOURCE(IDB_PNG_BG), _T("PNG"), AfxGetApp()->m_hInstance))
			return;
	}

	int nResizeWidth = 588;
	int nResizeHeight = 344;
	m_pbgImage->m_pBitmap = Common_Util::ResizeBitmap(m_pbgImage->m_pBitmap, nResizeWidth, nResizeHeight);
	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();
	MoveWindow(0, 0, x, y);
}

void CMsWebBrowsertestDlg::DrawSkin(CDC* pDC)
{
	int x = m_pbgImage->m_pBitmap->GetWidth();
	int y = m_pbgImage->m_pBitmap->GetHeight();

	Graphics gps(pDC->GetSafeHdc());
	gps.DrawImage(m_pbgImage->m_pBitmap, Rect(0, 0, x, y), 0, 0, x, y, UnitPixel);
}

BOOL CMsWebBrowsertestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CMsWebBrowsertestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CMsWebBrowsertestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	DestroyTopDlg();
	DestroyCenterDlg();
	DestroyBottomDlg();

	SAFE_DELETE(m_pbgImage);
}

BOOL CMsWebBrowsertestDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMsWebBrowsertestDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, 0, 0);
	}
	else
	{
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, 0, 0);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CMsWebBrowsertestDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);
	SetChildPos();
}

LRESULT CMsWebBrowsertestDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	SetChildPos();
	return 0;
}

BEGIN_EVENTSINK_MAP(CMsWebBrowsertestDlg, CDialogEx)
	ON_EVENT(CMsWebBrowsertestDlg, IDC_EXPLORER1, 104, CMsWebBrowsertestDlg::OnDownloadcompleteExplorer1, VTS_NONE)
	ON_EVENT(CMsWebBrowsertestDlg, IDC_EXPLORER1, 259, CMsWebBrowsertestDlg::OnDocumentcompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

void CMsWebBrowsertestDlg::OnDownloadcompleteExplorer1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMsWebBrowsertestDlg::OnDocumentcompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	CreateTopDlg();
	CreateCenterDlg();
	CreateBottomDlg();

	HRESULT   hr;
	LPUNKNOWN lpUnknown;
	LPUNKNOWN lpUnknownWB = NULL;
	LPUNKNOWN lpUnknownDC = NULL;
	lpUnknown = m_WebBrowser.GetControlUnknown();

	if (lpUnknown)
	{
		hr = lpUnknown->QueryInterface(IID_IUnknown, (LPVOID*)&lpUnknownWB);
		ASSERT(SUCCEEDED(hr)); if (FAILED(hr))return;

		hr = pDisp->QueryInterface(IID_IUnknown, (LPVOID*)&lpUnknownDC);
		ASSERT(SUCCEEDED(hr));

		if (SUCCEEDED(hr) && lpUnknownWB == lpUnknownDC)
		{
			IWebBrowser2* thisBrowser = NULL;
			HRESULT hr;

			hr = pDisp->QueryInterface(IID_IWebBrowser2, reinterpret_cast<void**>(&thisBrowser));

			if (SUCCEEDED(hr)) {
				thisBrowser->put_Silent(TRUE); // 이거해야 자바스크립트 오류 안뜬다!!		
			}

			if (m_LoadedFlag)
			{
				if (thisBrowser)thisBrowser->Release();
				if (thisBrowser)thisBrowser = NULL;

				if (lpUnknownWB)lpUnknownWB->Release();
				if (lpUnknownDC)lpUnknownDC->Release();
				return;
			}

			m_LoadedFlag = TRUE;

			if (thisBrowser)thisBrowser->Release();
			if (thisBrowser)thisBrowser = NULL;

			if (lpUnknownWB)lpUnknownWB->Release();
			if (lpUnknownDC)lpUnknownDC->Release();
		}
	}
}
