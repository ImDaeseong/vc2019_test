#include "pch.h"
#include "SkinButtonNormal.h"

#define WM_BTN_PAINT WM_USER + 100
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

IMPLEMENT_DYNAMIC(CSkinButtonNormal, CButton)
CSkinButtonNormal::CSkinButtonNormal()
{
	m_pBGImage = NULL;
	m_Tracking = FALSE;
	m_Hovering = FALSE;
	m_Disabled = FALSE;

	m_nStartX = 0;
	m_nStartY = 0;
	m_nDivide = 4;
}

CSkinButtonNormal::~CSkinButtonNormal()
{
	Free();
}

BEGIN_MESSAGE_MAP(CSkinButtonNormal, CButton)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

void CSkinButtonNormal::Free()
{
	if(m_pBGImage != NULL)
		delete m_pBGImage;
	m_pBGImage = NULL;
}

void CSkinButtonNormal::Disable(BOOL Disabled)
{
	m_Disabled = Disabled;
	RedrawWindow();
}

BOOL CSkinButtonNormal::Disabled()
{
	return m_Disabled;
}

void CSkinButtonNormal::InvalidateParent()
{
	RedrawWindow();
}

void CSkinButtonNormal::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW, 0);
	CButton::PreSubclassWindow();
}

BOOL CSkinButtonNormal::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	return CButton::PreTranslateMessage(pMsg);
}

HBRUSH CSkinButtonNormal::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetBkMode(TRANSPARENT);
	return static_cast<HBRUSH>(::GetStockObject(NULL_BRUSH));
}

BOOL CSkinButtonNormal::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSkinButtonNormal::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_Disabled) return;
	if (!m_Tracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_Tracking = _TrackMouseEvent(&tme);
	}

	if(m_Hovering)
		::SetCursor(::LoadCursor(0, MAKEINTRESOURCE(IDC_HAND)));

	CButton::OnMouseMove(nFlags, point);
}

LRESULT CSkinButtonNormal::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	if (m_Disabled) return NULL;
	m_Hovering = TRUE;
	InvalidateParent();
	return NULL;
}

LRESULT CSkinButtonNormal::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (m_Disabled) return NULL;
	m_Tracking = FALSE;
	m_Hovering = FALSE;
	InvalidateParent();
	return NULL;
}

void CSkinButtonNormal::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_Disabled) return;
	//InvalidateParent();
	CButton::OnLButtonDown(nFlags, point);
}

void CSkinButtonNormal::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_Disabled) return;
	InvalidateParent();
	CButton::OnLButtonUp(nFlags, point);
}

void CSkinButtonNormal::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{	
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	Graphics gps(dc);

	UINT state = lpDrawItemStruct->itemState;
	
	int nStatus = 0;

	if ((state & ODS_SELECTED))
	{	
		if(m_pBGImage != NULL)
		{			
			int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
			int nY = m_pBGImage->m_pBitmap->GetHeight();

			int nStatus = 2;
			CRect rBtn;
			rBtn = CRect((nX * nStatus), 0, nX, nY);
			gps.DrawImage(m_pBGImage->m_pBitmap, 0, 0, rBtn.left, rBtn.top, nX, nY, UnitPixel);
		}
	}
	else if (m_Disabled)
	{
		if(m_pBGImage != NULL)
		{
			int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
			int nY = m_pBGImage->m_pBitmap->GetHeight();
			
			int nStatus = 1;
			CRect rBtn;
			rBtn = CRect((nX * nStatus), 0, nX, nY);
			gps.DrawImage(m_pBGImage->m_pBitmap, 0, 0, rBtn.left, rBtn.top, nX, nY, UnitPixel);
		}
	}
	else if (m_Hovering)
	{
		if(m_pBGImage != NULL)
		{
			int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
			int nY = m_pBGImage->m_pBitmap->GetHeight();
			
			int nStatus = 2;
			CRect rBtn;
			rBtn = CRect((nX * nStatus), 0, nX, nY);
			gps.DrawImage(m_pBGImage->m_pBitmap, 0, 0, rBtn.left, rBtn.top, nX, nY, UnitPixel);
		}
	}
	else
	{	
		if(m_pBGImage != NULL)
		{
			int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
			int nY = m_pBGImage->m_pBitmap->GetHeight();

			int nStatus = 0;
			CRect rBtn;
			rBtn = CRect((nX * nStatus), 0, nX, nY);
			gps.DrawImage(m_pBGImage->m_pBitmap, 0, 0, rBtn.left, rBtn.top, nX, nY, UnitPixel);
		}
	}
	dc.Detach();	
}

BOOL CSkinButtonNormal::SetImage(LPCTSTR pName, LPCTSTR pType, HMODULE hInst, int nStartX, int nStartY, int nDivide)
{
	if (!m_pBGImage)
	{
		m_pBGImage = new CGdiPlusBitmapResource;
		if (!m_pBGImage->Load(pName, pType, hInst))
		{
			Free();
			return FALSE;
		}
	}

	m_nDivide = nDivide;
	m_nStartX = nStartX;
	m_nStartY = nStartY;

	int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
	int nY = m_pBGImage->m_pBitmap->GetHeight();
	MoveWindow(nStartX, nStartY, nX, nY);	

	return TRUE;
}

BOOL CSkinButtonNormal::SetImg(LPCTSTR pName, int nStartX, int nStartY, int nDivide)
{
	if(!m_pBGImage)
	{
		m_pBGImage = new CGdiPlusBitmapResource;
		if(!m_pBGImage->Load(pName))
		{
			Free();
			return FALSE;
		}
	}	
	
	m_nDivide = nDivide;
	m_nStartX = nStartX;
	m_nStartY = nStartY;

	int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
	int nY = m_pBGImage->m_pBitmap->GetHeight();
	MoveWindow(m_nStartX, nStartY, nX, nY);	

	return TRUE;
}