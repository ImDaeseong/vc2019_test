#include "pch.h"
#include "TextStatic.h"
#include "MsWebBrowser_testDlg.h"

IMPLEMENT_DYNAMIC(CTextStatic, CStatic)

CTextStatic::CTextStatic()
{
	m_nFontSize = 20;
	m_bClick = FALSE;
}

CTextStatic::~CTextStatic()
{
}

BEGIN_MESSAGE_MAP(CTextStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

void CTextStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bClick)
	{
		CMsWebBrowsertestDlg* pMain = (CMsWebBrowsertestDlg*)AfxGetMainWnd();
		pMain->CallMain();
	}
}

void CTextStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	InvalidateRect(NULL);
}

BOOL CTextStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(::LoadCursor(0, MAKEINTRESOURCE(IDC_HAND)));
	return TRUE;
}

void CTextStatic::SetBannerClick(BOOL bClick)
{
	m_bClick = bClick;
	ModifyStyle(0, SS_NOTIFY);
}

void CTextStatic::OnPaint()
{
	USES_CONVERSION;

	CPaintDC dc(this);

	Gdiplus::Graphics graphics(dc.GetSafeHdc());

	CRect rect;
	GetClientRect(&rect);
	RectF rectF(rect.left, rect.top, (float)rect.Width(), (float)rect.Height());

	Gdiplus::Font font(L"Tahoma", m_nFontSize, FontStyleBold, UnitPixel);
	Gdiplus::SolidBrush brush(Gdiplus::Color(254, (BYTE)255, (BYTE)214, (BYTE)50));

	StringFormat stringFormat(StringFormatFlagsDisplayFormatControl);
	stringFormat.SetAlignment(StringAlignmentCenter);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	graphics.DrawString(m_strMessage, -1, &font, rectF, &stringFormat, &brush);
}

void CTextStatic::OnDrawLayerdWindow(Graphics& Gps)
{
	CRect rc;
	GetWindowRect(rc);
	GetOwner()->ScreenToClient(rc);
	RectF rectF(REAL(rc.left), REAL(rc.top), REAL(rc.Width()), REAL(rc.Height()));

	Gdiplus::Font font(L"Tahoma", m_nFontSize, FontStyleBold, UnitPixel);
	Gdiplus::SolidBrush brush(Gdiplus::Color(254, (BYTE)255, (BYTE)214, (BYTE)50));

	StringFormat stringFormat(StringFormatFlagsDisplayFormatControl);
	stringFormat.SetAlignment(StringAlignmentCenter);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	Gps.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	Gps.DrawString(m_strMessage, -1, &font, rectF, &stringFormat, &brush);

	/*
	//배경 설정
	RectF destRect(rc.left, rc.top, (float)rc.Width() - 2, (float)rc.Height() - 2);

	//배경 색상 처리
	SolidBrush bgbrush(Color(255, 0, 0));
	Gps.FillRectangle(&bgbrush, destRect);
	*/
}

void CTextStatic::SetMesssageText(CString sMessage)
{
	m_strMessage = sMessage;
	ModifyStyle(0, SS_NOTIFY);
}

void CTextStatic::SetMesssageFontSize(int nFontSize)
{
	m_nFontSize = nFontSize;
}