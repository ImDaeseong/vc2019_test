#include "pch.h"
#include "framework.h"
#include "Checkbox_test.h"
#include "Checkbox_testDlg.h"
#include "afxdialogex.h"
#include "EditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCheckboxtestDlg::CCheckboxtestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHECKBOX_TEST_DIALOG, pParent)
{
	m_pBackSkin = NULL;

	m_pEditDlg = NULL;
}

void CCheckboxtestDlg::CreateEditDlg()
{
	if (!m_pEditDlg)
	{
		m_pEditDlg = new EditDlg();
		m_pEditDlg->SetParent(this);
		m_pEditDlg->Create(IDD_EDIT_DIALOG, this);
		SetChildPos();
		m_pEditDlg->ShowWindow(SW_SHOW);
	}
	else
		m_pEditDlg->SetActiveWindow();
}

void CCheckboxtestDlg::DestroyEditDlg()
{
	if (m_pEditDlg)
	{
		m_pEditDlg->DestroyWindow();
		delete m_pEditDlg;
		m_pEditDlg = NULL;
	}
}

void CCheckboxtestDlg::SetChildPos()
{
	if (m_pEditDlg)
	{
		CRect rInfoWnd;
		GetWindowRect(rInfoWnd);

		int nX = rInfoWnd.left + 431;
		int nY = rInfoWnd.top + 264;
		m_pEditDlg->MoveWindow(nX, nY, 310, 30);
	}
}

LRESULT CCheckboxtestDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	SetChildPos();
	return 0;
}

void CCheckboxtestDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	SetChildPos();
}

void CCheckboxtestDlg::OnMouseMove(UINT nFlags, CPoint point)
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

void CCheckboxtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_BUTTON1, m_btnCheck1);
	DDX_Control(pDX, IDC_CHECK_STATIC1, m_StaticCheck1);
	DDX_Control(pDX, IDC_REMOVE_BUTTON, m_btnRemove);
	DDX_Control(pDX, IDC_CANCEL_BUTTON, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CCheckboxtestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_REMOVE_BUTTON, &CCheckboxtestDlg::OnBnClickedRemoveButton)
	ON_BN_CLICKED(IDC_CANCEL_BUTTON, &CCheckboxtestDlg::OnBnClickedCancelButton)
END_MESSAGE_MAP()

BOOL CCheckboxtestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CCheckboxtestDlg::OnInitDialog()
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

	CreateEditDlg();

	return TRUE;  
}

void CCheckboxtestDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawSkin(&dc);
}

void CCheckboxtestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	DestroyEditDlg();

	if (m_pBackSkin != NULL)
		delete m_pBackSkin;
	m_pBackSkin = NULL;
}

BOOL CCheckboxtestDlg::LoadSkin()
{
	m_pBackSkin = new CGdiPlusBitmapResource;
	if (!m_pBackSkin->Load(MAKEINTRESOURCE(IDB_BG_PNG), _T("PNG"), AfxGetApp()->m_hInstance))
	{
		delete m_pBackSkin;
		m_pBackSkin = NULL;
		return FALSE;
	}

	int cx = m_pBackSkin->m_pBitmap->GetWidth();
	int cy = m_pBackSkin->m_pBitmap->GetHeight();
	MoveWindow(0, 0, cx, cy);

	return TRUE;
}

void CCheckboxtestDlg::DrawSkin(CDC* pDC)
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

	if (m_btnCheck1.IsAvailableDraw())
		m_btnCheck1.OnDrawLayerdWindow(gps);

	m_StaticCheck1.OnDrawLayerdWindow(gps);


	if (m_btnRemove.IsAvailableDraw())
		m_btnRemove.OnDrawLayerdWindow(gps);

	if (m_btnCancel.IsAvailableDraw())
		m_btnCancel.OnDrawLayerdWindow(gps);

	
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

void CCheckboxtestDlg::InitControls()
{
	CRect rc;
	GetClientRect(&rc);

	m_btnCheck1.SetImage(MAKEINTRESOURCE(IDB_CHECK_PNG), _T("PNG"), AfxGetApp()->m_hInstance, 48, 139, 2);
	
	m_StaticCheck1.MoveWindow(46, 138, 254, 28);
	m_StaticCheck1.SetCheckIndex(1);


	m_btnRemove.SetImage(MAKEINTRESOURCE(IDB_REMOVE_PNG), _T("PNG"), AfxGetApp()->m_hInstance, 244, 396, 1);
	m_btnCancel.SetImage(MAKEINTRESOURCE(IDB_CANCEL_PNG), _T("PNG"), AfxGetApp()->m_hInstance, 404, 396, 1);
}

void CCheckboxtestDlg::SetCheckIndex(int nIndex)
{
	m_btnCheck1.SetCheckState();
}

void CCheckboxtestDlg::OnBnClickedRemoveButton()
{
	if (m_btnCheck1.GetCheckState())
	{
		OutputDebugString(_T("m_btnCheck1 check"));
	}

	if (m_pEditDlg)
	{
		CString strText = m_pEditDlg->GetEditText();
		OutputDebugString(strText);
	}
}

void CCheckboxtestDlg::OnBnClickedCancelButton()
{
	CDialogEx::OnOK();
}
