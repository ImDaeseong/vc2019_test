#include "pch.h"
#include "CheckButton.h"

#define WM_BTN_PAINT WM_USER + 100
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

IMPLEMENT_DYNAMIC(CCheckButton, CButton) 
CCheckButton::CCheckButton() : m_Clicked(FALSE)
{
	m_nStartX = 0;
	m_nStartY = 0;
	m_pBGImage = NULL;
	m_nDivide = 2;
}

CCheckButton::~CCheckButton()
{
	SAFE_DELETE(m_pBGImage);
}

BEGIN_MESSAGE_MAP(CCheckButton, CButton)
	ON_WM_LBUTTONUP()	
END_MESSAGE_MAP()

void CCheckButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CCheckButton::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
}

void CCheckButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Clicked = !m_Clicked;
	//RedrawWindow();
	GetOwner()->SendMessage(WM_PAINT, 0, 0);
	//Invalidate();

	CButton::OnLButtonUp(nFlags, point);	
}

int CCheckButton::GetImageWidth() const
{
	int res = 0;
	if (m_pBGImage != NULL && m_pBGImage->m_pBitmap != NULL)
	{
		res = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
	}
	return res;
}

int CCheckButton::GetImageHeight() const
{
	int res = 0;
	if (m_pBGImage != NULL && m_pBGImage->m_pBitmap != NULL)
	{
		res = m_pBGImage->m_pBitmap->GetHeight();
	}
	return res;
}

void CCheckButton::OnDrawWindow(Graphics& gps)
{
	CRect rc;
	GetWindowRect(rc);
	GetOwner()->ScreenToClient(rc);

	int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
	int nY = m_pBGImage->m_pBitmap->GetHeight();

	if(m_Clicked)
		gps.DrawImage(m_pBGImage->m_pBitmap, Rect(rc.left, rc.top, nX, nY), nX * 1, 0, nX, nY, UnitPixel);
	else
		gps.DrawImage(m_pBGImage->m_pBitmap, Rect(rc.left, rc.top, nX, nY), nX * 0, 0, nX, nY, UnitPixel);	
}

void CCheckButton::OnDrawLayerdWindow(Graphics& Gps)
{
	CRect rc;
	GetWindowRect(rc);
	GetOwner()->ScreenToClient(rc);

	CSize szFrame = (GetStyle() & WS_THICKFRAME) ?
		CSize(GetSystemMetrics(SM_CXSIZEFRAME), GetSystemMetrics(SM_CYSIZEFRAME)) :
		CSize(GetSystemMetrics(SM_CXFIXEDFRAME), GetSystemMetrics(SM_CYFIXEDFRAME));

	rc.OffsetRect(szFrame.cy, szFrame.cy);

	int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
	int nY = m_pBGImage->m_pBitmap->GetHeight();

	if (m_Clicked)
		Gps.DrawImage(m_pBGImage->m_pBitmap, Rect(rc.left, rc.top, nX, nY), nX * 1, 0, nX, nY, UnitPixel);
	else
		Gps.DrawImage(m_pBGImage->m_pBitmap, Rect(rc.left, rc.top, nX, nY), nX * 0, 0, nX, nY, UnitPixel);
}

BOOL CCheckButton::SetImage(LPCTSTR pName, LPCTSTR pType, HMODULE hInst, int nStartX, int nStartY, int nDivide)
{
	if (!m_pBGImage)
	{
		m_pBGImage = new CGdiPlusBitmapResource;
		if (!m_pBGImage->Load(pName, pType, hInst))
		{
			SAFE_DELETE(m_pBGImage);
			return FALSE;
		}

		m_nDivide = nDivide;
		m_nStartX = nStartX;
		m_nStartY = nStartY;
		int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
		int nY = m_pBGImage->m_pBitmap->GetHeight();
		MoveWindow(m_nStartX, m_nStartY, nX, nY);

		return TRUE;
	}
	return FALSE;
}

BOOL CCheckButton::SetImage(UINT uid, LPCTSTR pType, HMODULE hInst, int nStartX, int nStartY, int nDivide)
{
	if (!m_pBGImage)
	{
		m_pBGImage = new CGdiPlusBitmapResource;

		if (!m_pBGImage->Load(uid, pType, hInst))
		{
			SAFE_DELETE(m_pBGImage);
			return FALSE;
		}

		m_nDivide = nDivide;
		m_nStartX = nStartX;
		m_nStartY = nStartY;
		int nX = m_pBGImage->m_pBitmap->GetWidth() / m_nDivide;
		int nY = m_pBGImage->m_pBitmap->GetHeight();
		MoveWindow(m_nStartX, m_nStartY, nX, nY);

		return TRUE;
	}
	return FALSE;
}

BOOL CCheckButton::IsAvailableDraw()
{
	if (!m_pBGImage)
		return FALSE;
	return TRUE;
}

void CCheckButton::SetCheckState()
{
	m_Clicked = !m_Clicked;
	//RedrawWindow();
	GetOwner()->SendMessage(WM_PAINT, 0, 0);
	//Invalidate();
}

BOOL CCheckButton::GetCheckState()
{
	return m_Clicked;
}