#include "pch.h"
#include "CheckStatic.h"
#include "Checkbox_testDlg.h"

CCheckStatic::CCheckStatic()
{
	m_nIndex = 0;

	//border = false 이면 투명 static
	m_bBorder = TRUE;
}

CCheckStatic::~CCheckStatic()
{
}

BEGIN_MESSAGE_MAP(CCheckStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CCheckStatic::PreSubclassWindow()
{
	ModifyStyle(0, SS_NOTIFY);

	CStatic::PreSubclassWindow();
}

void CCheckStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	CCheckboxtestDlg* pMain = (CCheckboxtestDlg*)AfxGetMainWnd();
	pMain->SetCheckIndex(m_nIndex);
}

void CCheckStatic::OnPaint()
{
	CPaintDC dc(this);
}

void CCheckStatic::OnDrawLayerdWindow(Graphics& Gps)
{
	USES_CONVERSION;

	Gps.SetSmoothingMode(SmoothingModeHighQuality);
	Gps.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	CRect rect;
	GetWindowRect(rect);
	GetOwner()->ScreenToClient(rect);

	RectF destRect(REAL(rect.left), REAL(rect.top), REAL(rect.Width()), REAL(rect.Height()));

	if (m_bBorder)
	{
		Pen dashedPen(Color(50, 50, 50, 0), 1);
		dashedPen.SetDashStyle(Gdiplus::DashStyle::DashStyleDash);
		Gps.DrawRectangle(&dashedPen, destRect.X + 1, destRect.Y + 1, destRect.Width - 2, destRect.Height - 2);
	}
}

void CCheckStatic::SetCheckIndex(int nIndex)
{
	m_nIndex = nIndex;
}
