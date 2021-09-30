#include "pch.h"
#include "DragStatic.h"
#include "Resource.h"
#include "DragStatic_testDlg.h"

DragStatic::DragStatic()
{
	m_pImage = NULL;

	m_bCapture = FALSE;
}

DragStatic::~DragStatic()
{
	if (m_pImage != NULL)
		delete m_pImage;
	m_pImage = NULL;
}
BEGIN_MESSAGE_MAP(DragStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_DROPFILES()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void DragStatic::SetImage(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
{
	if (m_pImage == NULL)
		m_pImage = new CGdiPlusBitmapResource;

	if (!m_pImage->Load(pName, pType, hInst))
	{
		delete m_pImage;
		m_pImage = NULL;
		return;
	}
	InvalidateRect(NULL);
}

void DragStatic::OnPaint()
{
	CPaintDC dc(this);

	Graphics graphics(dc.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	CRect rect;
	GetClientRect(&rect);
	RectF destRect(REAL(rect.left), REAL(rect.top), REAL(rect.Width()), REAL(rect.Height()));

	if (m_pImage)
	{
		Unit units;
		RectF srcRect;
		m_pImage->m_pBitmap->GetBounds(&srcRect, &units);
		graphics.DrawImage(m_pImage->m_pBitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel, NULL);
	}
}

BOOL DragStatic::IsAvailableDraw()
{
	if (!m_pImage)
		return FALSE;
	return TRUE;
}

void DragStatic::OnDrawLayerdWindow(Graphics& Gps)
{
	if (m_pImage)
	{
		Gps.SetSmoothingMode(SmoothingModeHighQuality);
		Gps.SetInterpolationMode(InterpolationModeHighQualityBicubic);

		CRect rc;
		GetWindowRect(rc);
		GetOwner()->ScreenToClient(rc);
		RectF destRect(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));

		Unit units;
		RectF srcRect;
		m_pImage->m_pBitmap->GetBounds(&srcRect, &units);
		Gps.DrawImage(m_pImage->m_pBitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel, NULL);
	}
}

void DragStatic::OnDropFiles(HDROP hDropInfo)
{
	if (::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0) == 1) {

		TCHAR tFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, 0, tFileName, _MAX_PATH);
		m_strDragFile = tFileName;

		GetParent()->SendMessage(WM_USER_DRAGCONTROL, (WPARAM)GetDlgCtrlID());
	}
	::DragFinish(hDropInfo);

	CStatic::OnDropFiles(hDropInfo);
}

void DragStatic::PreSubclassWindow()
{
	DragAcceptFiles();

	ModifyStyle(0, SS_NOTIFY);

	CStatic::PreSubclassWindow();
}

void DragStatic::OnLButtonDown(UINT nFlags, CPoint point)
{	
	((CDragStatictestDlg*)GetParent())->SetMoveWindow(FALSE);

	GetCursorPos(&m_pPos);
	HWND hWnd = ::WindowFromPoint(m_pPos);
	CWnd* pWnd = CWnd::FromHandle(hWnd);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_DRAG1)
	{
		GetParent()->SendMessage(WM_USER_DRAGCONTROL, (WPARAM)IDC_STATIC_DRAG1, (LPARAM)1);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_DRAG2)
	{
		GetParent()->SendMessage(WM_USER_DRAGCONTROL, (WPARAM)IDC_STATIC_DRAG2, (LPARAM)1);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_DRAG3)
	{
		GetParent()->SendMessage(WM_USER_DRAGCONTROL, (WPARAM)IDC_STATIC_DRAG3, (LPARAM)1);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_DRAG4)
	{
		GetParent()->SendMessage(WM_USER_DRAGCONTROL, (WPARAM)IDC_STATIC_DRAG4, (LPARAM)1);
	}
	
	m_bCapture = TRUE;
	SetCapture();
		
	CStatic::OnLButtonDown(nFlags, point);
}

void DragStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	((CDragStatictestDlg*)GetParent())->SetMoveWindow(FALSE);
		
	GetCursorPos(&m_pPos);
	HWND hWnd = ::WindowFromPoint(m_pPos);
	CWnd* pWnd = CWnd::FromHandle(hWnd);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_DRAG1)
	{
		GetParent()->SendMessage(WM_USER_DRAGCONTROL, (WPARAM)IDC_STATIC_DRAG1, (LPARAM)2);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_DRAG2)
	{
		GetParent()->SendMessage(WM_USER_DRAGCONTROL, (WPARAM)IDC_STATIC_DRAG2, (LPARAM)2);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_DRAG3)
	{
		GetParent()->SendMessage(WM_USER_DRAGCONTROL, (WPARAM)IDC_STATIC_DRAG3, (LPARAM)2);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_DRAG4)
	{
		GetParent()->SendMessage(WM_USER_DRAGCONTROL, (WPARAM)IDC_STATIC_DRAG4, (LPARAM)2);
	}
	
	ReleaseCapture();
	m_bCapture = FALSE;

	CStatic::OnLButtonUp(nFlags, point);
}

void DragStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	::SetCursor(::LoadCursor(NULL, IDC_HAND));

	if (m_bCapture)
	{		
		((CDragStatictestDlg*)GetParent())->SetMoveWindow(TRUE);
	}

	CStatic::OnMouseMove(nFlags, point);
}
