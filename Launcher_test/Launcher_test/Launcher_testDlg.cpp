#include "pch.h"
#include "framework.h"
#include "Launcher_test.h"
#include "Launcher_testDlg.h"
#include "afxdialogex.h"
#include "LeftDlg.h"
#include "RightDlg.h"
#include "DragDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLaunchertestDlg::CLaunchertestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LAUNCHER_TEST_DIALOG, pParent)
{
	m_pBackSkin = NULL;

	m_pDragDlg = NULL;

	m_pLeftDlg = NULL;
	m_pRightDlg = NULL;

	m_bSlideStyle = TRUE;
	m_bLocation = TRUE;
}

void CLaunchertestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_Picturestatic);
}

BEGIN_MESSAGE_MAP(CLaunchertestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_WM_MOVE()
	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CLaunchertestDlg::OnClickedButtonClose)
END_MESSAGE_MAP()

BOOL CLaunchertestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CLaunchertestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW | WS_EX_LAYERED);

	if (!LoadSkin())
	{
		CDialogEx::OnCancel();
		return FALSE;
	}

	InitControls();

	CDC* pDC = GetDC();
	DrawSkin(pDC);
	ReleaseDC(pDC);
		
	SetTimer(TIMER_ID_LOCATION_CHANGE, 1000, NULL);

	return TRUE;  
}

void CLaunchertestDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

void CLaunchertestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	DestroyDragDlg();

	DestroyLeftDlg();
	DestroyRightDlg();

	if (m_pBackSkin != NULL)
		delete m_pBackSkin;
	m_pBackSkin = NULL;

	KillTimer(TIMER_ID_LOCATION_CHANGE);
	KillTimer(TIMER_ID_LEFTSLIDE_OPEN);
	KillTimer(TIMER_ID_LEFTSLIDE_CLOSE);
	KillTimer(TIMER_ID_RIGHTSLIDE_OPEN);
	KillTimer(TIMER_ID_RIGHTSLIDE_CLOSE);
}

void CLaunchertestDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CLaunchertestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType == SIZE_MAXIMIZED || nType == SIZE_RESTORED)
	{
		changeLocationDialog();
		//Invalidate();
	}
}

LRESULT CLaunchertestDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{	
	changeLocationDialog();
	SetChildPos();

	return 0;
}

void CLaunchertestDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	SetChildPos();
}

void CLaunchertestDlg::OnMouseMove(UINT nFlags, CPoint point)
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

void CLaunchertestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CLaunchertestDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CLaunchertestDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CLaunchertestDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID_LOCATION_CHANGE)
	{
		changeLocationDialog();
	}
	else if (nIDEvent == TIMER_ID_LEFTSLIDE_OPEN)
	{
		//왼쪽으로 열림
		slideLeftOpen();
	}
	else if (nIDEvent == TIMER_ID_LEFTSLIDE_CLOSE)
	{
		//왼쪽으로 닫힘
		slideLeftClose();
	}
	else if (nIDEvent == TIMER_ID_RIGHTSLIDE_OPEN)
	{
		//오른쪽으로 열림
		slideRightOpen();
	}
	else if (nIDEvent == TIMER_ID_RIGHTSLIDE_CLOSE)
	{
		//오른쪽으로 닫힘
		slideRightClose();
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CLaunchertestDlg::LoadSkin()
{
	m_pBackSkin = new CGdiPlusBitmapResource;
	if (!m_pBackSkin->LoadPath(_T("")))
	{
		if (!m_pBackSkin->Load(MAKEINTRESOURCE(IDB_PNG_BG), _T("PNG"), AfxGetApp()->m_hInstance))
		{
			delete m_pBackSkin;
			m_pBackSkin = NULL;
			return FALSE;
		}
	}
		
	MoveLocationDialog();

	return TRUE;
}

void CLaunchertestDlg::DrawSkin(CDC* pDC)
{
	CDC memDC;
	CBitmap bmp;
	CBitmap* bmp_old;

	CRect rect;
	GetWindowRect(&rect);

	int cx = rect.Width();
	int cy = rect.Height();

	memDC.CreateCompatibleDC(pDC);

	BITMAPINFO bmInfo;
	memset(&bmInfo, 0x00, sizeof(BITMAPINFO));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = cx;
	bmInfo.bmiHeader.biHeight = cy;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 32;
	bmInfo.bmiHeader.biCompression = BI_RGB;
	bmInfo.bmiHeader.biSizeImage = 0;
	bmInfo.bmiHeader.biClrUsed = 0;
	bmInfo.bmiHeader.biClrImportant = 0;

	LPBYTE pbmpBits = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(pDC->GetSafeHdc(), (LPBITMAPINFO)&bmInfo, DIB_RGB_COLORS, (void**)&pbmpBits, NULL, 0);
	if (hBitmap == NULL)
		bmp.CreateCompatibleBitmap(pDC, cx, cy);
	else
		bmp.Attach(hBitmap);

	bmp_old = memDC.SelectObject(&bmp);
	Graphics gps(memDC.GetSafeHdc());

	gps.DrawImage(
		m_pBackSkin->m_pBitmap,
		Rect(0, 0, m_pBackSkin->m_pBitmap->GetWidth(), m_pBackSkin->m_pBitmap->GetHeight()),
		0,
		0,
		m_pBackSkin->m_pBitmap->GetWidth(),
		m_pBackSkin->m_pBitmap->GetHeight(),
		UnitPixel);


	//CPictureStatic
	if (m_Picturestatic.IsAvailableDraw())
		m_Picturestatic.OnDrawLayerdWindow(gps);

	//CSkinButton
	if (m_btnClose.IsAvailableDraw())
		m_btnClose.OnDrawLayerdWindow(gps);


	CPoint ptWindowScreenPosition(rect.TopLeft());
	CSize szWindow(cx, cy);

	BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	CPoint ptSrc(0, 0);

	HDC hdc = gps.GetHDC();

	BOOL bRet = ::UpdateLayeredWindow(GetSafeHwnd(),
		pDC->GetSafeHdc(),
		&ptWindowScreenPosition,
		&szWindow,
		hdc,
		&ptSrc,
		0,
		&blendPixelFunction,
		ULW_ALPHA);

	gps.ReleaseHDC(hdc);

	memDC.SelectObject(bmp_old);
	bmp.DeleteObject();
	memDC.DeleteDC();
}

void CLaunchertestDlg::InitControls()
{
	CRect rClient;
	GetClientRect(&rClient);

	//CSkinButton
	m_btnClose.SetImage(MAKEINTRESOURCE(IDB_PNG_CLOSE), _T("PNG"), AfxGetApp()->m_hInstance, (rClient.right - 35), 0, 4);

	//CPictureStatic
	int nSize = 40;
	int nLeft = (rClient.Width() - nSize) / 2;
	int nTop = (rClient.Height() - nSize) / 2;
	m_Picturestatic.SetImage(MAKEINTRESOURCE(IDB_PNG_CLICK), _T("PNG"), AfxGetApp()->m_hInstance);
	m_Picturestatic.MoveWindow(nLeft, nTop, nSize, nSize);
	m_Picturestatic.SetPictureStaticClick(TRUE);
	m_Picturestatic.SetPictureStaticCursor(TRUE);
}

int CLaunchertestDlg::MonitorWidth()
{
	/*
	//주 모니터 해상도
	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);//가로 해상도
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);//세로 해상도

	//전체 모니터 해상도
	int nScreenX = GetSystemMetrics(SM_CXVIRTUALSCREEN);//전체 가로 해상도
	int nScreenY = GetSystemMetrics(SM_CYVIRTUALSCREEN);//전체 세로 해상도
	*/

	//주 모니터 해상도
	MONITORINFO mInfo;
	mInfo.cbSize = sizeof(mInfo);
	if (GetMonitorInfo(MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY), &mInfo) )
	{
		return (mInfo.rcMonitor.right - mInfo.rcMonitor.left);
	}
	
	return ::GetSystemMetrics(SM_CXSCREEN);
}

int CLaunchertestDlg::MonitorHeight()
{
	/*
	//주 모니터 해상도
	int nScreenX = ::GetSystemMetrics(SM_CXSCREEN);//가로 해상도
	int nScreenY = ::GetSystemMetrics(SM_CYSCREEN);//세로 해상도

	//전체 모니터 해상도
	int nScreenX = GetSystemMetrics(SM_CXVIRTUALSCREEN);//전체 가로 해상도
	int nScreenY = GetSystemMetrics(SM_CYVIRTUALSCREEN);//전체 세로 해상도
	*/

	//주 모니터 해상도
	MONITORINFO mInfo;
	mInfo.cbSize = sizeof(mInfo);
	if (GetMonitorInfo(MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY), &mInfo))
	{
		return (mInfo.rcMonitor.bottom - mInfo.rcMonitor.top);
	}

	return ::GetSystemMetrics(SM_CYSCREEN);
}

void CLaunchertestDlg::MoveLocationDialog()
{
	if (m_pBackSkin == NULL) return;

	int cx = m_pBackSkin->m_pBitmap->GetWidth();
	int cy = m_pBackSkin->m_pBitmap->GetHeight();
	
	int nScreenX = MonitorWidth();
	int nScreenY = MonitorHeight();

	RECT rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);

	//오른쪽
	MoveWindow(nScreenX - cx - 50, nScreenY - cy - 100, cx, cy);

	//왼쪽
	//MoveWindow(rcWorkArea.left + 50, nScreenY - cy - 100, cx, cy);
}

void CLaunchertestDlg::changeLocationDialog()
{
	if (m_pBackSkin == NULL) return;

	int cx = m_pBackSkin->m_pBitmap->GetWidth();
	int cy = m_pBackSkin->m_pBitmap->GetHeight();

	int nScreenX = MonitorWidth();
	int nScreenY = MonitorHeight();

	RECT rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);

	int nLocation = (nScreenX) / 2;

	CRect rect;
	GetWindowRect(rect);

	if (nLocation > rect.left)
	{
		//OutputDebugString(_T("왼쪽 \n"));

		m_bLocation = FALSE;

		//다이얼로그
		MoveWindow(rcWorkArea.left + 50, nScreenY - cy - 100, cx, cy);	

		CRect rClient;
		GetClientRect(&rClient);

		//종료버튼
		m_btnClose.MoveWindow((rClient.left), 0, m_btnClose.GetImageWidth(), m_btnClose.GetImageHeight());

		//왼쪽에 서브 다이얼로그 있다면 종료 
		DestroyLeftDlg();
	}
	else
	{
		//OutputDebugString(_T("오른쪽 \n"));

		m_bLocation = TRUE;

		//다이얼로그
		MoveWindow(nScreenX - cx - 50, nScreenY - cy - 100, cx, cy);	

		CRect rClient;
		GetClientRect(&rClient);

		//종료버튼
		m_btnClose.MoveWindow((rClient.right - 35), 0, m_btnClose.GetImageWidth(), m_btnClose.GetImageHeight());

		//오른쪽에 서브 다이얼로그 있다면 종료
		DestroyRightDlg();
	}
}

void CLaunchertestDlg::CreateLeftDlg()
{
	if (!m_pLeftDlg)
	{
		m_pLeftDlg = new LeftDlg();
		m_pLeftDlg->SetParent(this);
		m_pLeftDlg->Create(IDD_LEFT_DIALOG, this);
		SetChildPos();
		m_pLeftDlg->ShowWindow(SW_SHOW);
	}
}

void CLaunchertestDlg::DestroyLeftDlg()
{
	if (m_pLeftDlg)
	{
		m_pLeftDlg->DestroyWindow();
		delete m_pLeftDlg;
		m_pLeftDlg = NULL;
	}
}

void CLaunchertestDlg::CreateRightDlg()
{
	if (!m_pRightDlg)
	{
		m_pRightDlg = new RightDlg();
		m_pRightDlg->SetParent(this);
		m_pRightDlg->Create(IDD_RIGHT_DIALOG, this);
		SetChildPos();
		m_pRightDlg->ShowWindow(SW_SHOW);
	}
}

void CLaunchertestDlg::DestroyRightDlg()
{
	if (m_pRightDlg)
	{
		m_pRightDlg->DestroyWindow();
		delete m_pRightDlg;
		m_pRightDlg = NULL;
	}
}

void CLaunchertestDlg::SetChildPos()
{
	if (m_pLeftDlg)
	{
		CRect rect;
		GetWindowRect(rect);

		int nX = rect.left - m_pLeftDlg->GetWidth();
		int nY = rect.top;
		m_pLeftDlg->MoveWindow(nX, nY, m_pLeftDlg->GetWidth(), m_pLeftDlg->GetHeight());
	}

	if (m_pRightDlg)
	{
		CRect rect;
		GetWindowRect(rect);

		int nX = rect.right;
		int nY = rect.top;
		m_pRightDlg->MoveWindow(nX, nY, m_pRightDlg->GetWidth(), m_pRightDlg->GetHeight());
	}
}

void CLaunchertestDlg::SetLaunchersub()
{
	if (m_bSlideStyle)
	{
		//슬라이드 스타일
		if (m_bLocation)
		{
			//OutputDebugString(_T("오른쪽 \n"));

			if (!m_pLeftDlg)
			{
				m_pLeftDlg = new LeftDlg();
				m_pLeftDlg->SetParent(this);
				m_pLeftDlg->Create(IDD_LEFT_DIALOG, this);

				SetTimer(TIMER_ID_LEFTSLIDE_OPEN, 10, NULL);

				nWidth = 0;
				nX = 0;
				nLeft = 0;
				nTop = 0;
			}
			else
			{
				SetTimer(TIMER_ID_LEFTSLIDE_CLOSE, 10, NULL);

				nWidth = m_pLeftDlg->GetWidth();
				nX = 0;
				nLeft = 0;
				nTop = 0;
			}
		}
		else
		{
			//OutputDebugString(_T("왼쪽 \n"));

			if (!m_pRightDlg)
			{
				m_pRightDlg = new RightDlg();
				m_pRightDlg->SetParent(this);
				m_pRightDlg->Create(IDD_RIGHT_DIALOG, this);

				SetTimer(TIMER_ID_RIGHTSLIDE_OPEN, 10, NULL);

				nWidth = 0;
				nX = 0;
				nLeft = 0;
				nTop = 0;
			}
			else
			{
				SetTimer(TIMER_ID_RIGHTSLIDE_CLOSE, 10, NULL);

				nWidth = m_pRightDlg->GetWidth();
				nX = 0;
				nLeft = 0;
				nTop = 0;
			}
		}
	}
	else
	{
		//일반 스타일
		DestroyLeftDlg();
		DestroyRightDlg();

		if (m_bLocation)
		{
			//OutputDebugString(_T("오른쪽 \n"));
			CreateLeftDlg();
		}
		else
		{
			//OutputDebugString(_T("왼쪽 \n"));
			CreateRightDlg();
		}
	}	
}

void CLaunchertestDlg::slideLeftOpen()
{
	if (m_pLeftDlg == NULL) return;

	nWidth += 10;
	nX += 10;

	if (nWidth >= m_pLeftDlg->GetWidth())
	{
		KillTimer(TIMER_ID_LEFTSLIDE_OPEN);
		m_pLeftDlg->MoveWindow(nLeft - (nWidth - m_pLeftDlg->GetWidth()), nTop, m_pLeftDlg->GetWidth(), m_pLeftDlg->GetHeight());
		return;
	}

	CRect rect;
	GetWindowRect(rect);

	nLeft = rect.left - nX;
	nTop = rect.top;

	m_pLeftDlg->ShowWindow(SW_SHOWNOACTIVATE);
	m_pLeftDlg->MoveWindow(nLeft, nTop, nWidth, m_pLeftDlg->GetHeight());
}

void CLaunchertestDlg::slideLeftClose()
{
	if (m_pLeftDlg == NULL) return;

	nWidth -= 10;
	nX += 10;

	if (nWidth <= 0)
	{
		KillTimer(TIMER_ID_LEFTSLIDE_CLOSE);
		DestroyLeftDlg();
		return;
	}

	CRect rect;
	GetWindowRect(rect);

	nLeft = (rect.left - m_pLeftDlg->GetWidth()) + nX;
	nTop = rect.top;
	m_pLeftDlg->MoveWindow(nLeft, nTop, nWidth, m_pLeftDlg->GetHeight());
}

void CLaunchertestDlg::slideRightOpen()
{
	if (m_pRightDlg == NULL) return;

	nWidth += 10;

	if (nWidth >= m_pRightDlg->GetWidth())
	{
		KillTimer(TIMER_ID_RIGHTSLIDE_OPEN);
		m_pRightDlg->MoveWindow(nLeft, nTop, m_pRightDlg->GetWidth(), m_pRightDlg->GetHeight());
		return;
	}

	CRect rect;
	GetWindowRect(rect);

	nLeft = rect.right;
	nTop = rect.top;

	m_pRightDlg->ShowWindow(SW_SHOWNOACTIVATE);
	m_pRightDlg->MoveWindow(nLeft, nTop, nWidth, m_pRightDlg->GetHeight());
}

void CLaunchertestDlg::slideRightClose()
{
	if (m_pRightDlg == NULL) return;

	nWidth -= 10;

	if (nWidth <= 0)
	{
		KillTimer(TIMER_ID_RIGHTSLIDE_CLOSE);
		DestroyRightDlg();
		return;
	}

	CRect rect;
	GetWindowRect(rect);

	nLeft = rect.right;
	nTop = rect.top;

	m_pRightDlg->MoveWindow(nLeft, nTop, nWidth, m_pRightDlg->GetHeight());
}

void CLaunchertestDlg::CreateDragDlg()
{
	if (!m_pDragDlg)
	{
		m_pDragDlg = new DragDlg();
		m_pDragDlg->SetParent(this);
		m_pDragDlg->Create(IDD_DRAG_DIALOG, this);
		m_pDragDlg->ShowWindow(SW_SHOW);
	}
}

void CLaunchertestDlg::DestroyDragDlg()
{
	if (m_pDragDlg)
	{
		m_pDragDlg->DestroyWindow();
		delete m_pDragDlg;
		m_pDragDlg = NULL;
	}
}

void CLaunchertestDlg::OnClickedButtonClose()
{
	if (m_pDragDlg)
	{
		DestroyDragDlg();
	}
	else
	{
		CreateDragDlg();
	}
}
