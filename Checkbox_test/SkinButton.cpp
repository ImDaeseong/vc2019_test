#include "pch.h"
#include "SkinButton.h"

IMPLEMENT_DYNAMIC(CSkinButton, CButton)

CSkinButton::CSkinButton()
{
	m_pBGImage = NULL;
	m_Tracking = FALSE;
	m_Hovering = FALSE;
	m_Disabled = FALSE;

	m_nStartX = 0;
	m_nStartY = 0;
	m_nDivide = 1;
	m_Status = 0;
}

CSkinButton::~CSkinButton()
{
	Free();
}

BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

void CSkinButton::Free()
{
	if (m_pBGImage != NULL)
		delete m_pBGImage;
	m_pBGImage = NULL;
}

void CSkinButton::Disable(BOOL Disabled)
{
	m_Disabled = Disabled;
	RedrawWindow();
}

BOOL CSkinButton::Disabled()
{
	return m_Disabled;
}

void CSkinButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

BOOL CSkinButton::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	return CButton::PreTranslateMessage(pMsg);
}

void CSkinButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_Disabled) return;
	if (!m_Tracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_Tracking = _TrackMouseEvent(&tme);
		Invalidate(FALSE);
	}

	if (m_Hovering)
		::SetCursor(::LoadCursor(0, MAKEINTRESOURCE(IDC_HAND)));

	CButton::OnMouseMove(nFlags, point);
}

LRESULT CSkinButton::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	if (m_Disabled) return NULL;
	m_Hovering = TRUE;
	Invalidate(FALSE);
	return NULL;
}

LRESULT CSkinButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (m_Disabled) return NULL;
	m_Tracking = FALSE;
	m_Hovering = FALSE;
	Invalidate(FALSE);
	return NULL;
}

void CSkinButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_Disabled) return;
	CButton::OnLButtonDown(nFlags, point);
}

void CSkinButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_Disabled) return;
	CButton::OnLButtonUp(nFlags, point);
}

void CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{	
}

BOOL CSkinButton::SetImage(LPCTSTR pName, LPCTSTR pType, HMODULE hInst, int nStartX, int nStartY, int nDivide)
{
	if (m_pBGImage == NULL)
		m_pBGImage = new CGdiPlusBitmapResource;

	if (!m_pBGImage->Load(pName, pType, hInst))
	{
		Free();
		return FALSE;
	}

	m_nDivide = nDivide;
	m_nStartX = nStartX;
	m_nStartY = nStartY;

	int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
	int nY = m_pBGImage->m_pBitmap->GetHeight();
	MoveWindow(nStartX, nStartY, nX, nY);

	return TRUE;
}

BOOL CSkinButton::SetImagePath(CString strPath, int nStartX, int nStartY, int nDivide)
{
	if (m_pBGImage == NULL)
		m_pBGImage = new CGdiPlusBitmapResource;

	if (!m_pBGImage->LoadPath(strPath))
	{
		Free();
		return FALSE;
	}

	m_nDivide = nDivide;
	m_nStartX = nStartX;
	m_nStartY = nStartY;

	int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
	int nY = m_pBGImage->m_pBitmap->GetHeight();
	MoveWindow(m_nStartX, nStartY, nX, nY);

	return TRUE;
}

BOOL CSkinButton::IsAvailableDraw()
{
	if (!m_pBGImage)
		return FALSE;
	return TRUE;
}

void CSkinButton::OnDrawLayerdWindow(Graphics& Gps)
{
	CRect rc;
	GetWindowRect(rc);
	GetOwner()->ScreenToClient(rc);

	CSize szFrame = (GetStyle() & WS_THICKFRAME) ? CSize(GetSystemMetrics(SM_CXSIZEFRAME), GetSystemMetrics(SM_CYSIZEFRAME)) : CSize(GetSystemMetrics(SM_CXFIXEDFRAME), GetSystemMetrics(SM_CYFIXEDFRAME));
	rc.OffsetRect(szFrame.cy, szFrame.cy);

	int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
	int nY = m_pBGImage->m_pBitmap->GetHeight();

	Gps.DrawImage(m_pBGImage->m_pBitmap, Rect(rc.left, rc.top, nX, nY), 0, 0, nX, nY, UnitPixel);
}