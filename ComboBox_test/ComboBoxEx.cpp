#include "pch.h"
#include "ComboBoxEx.h"
#include "Resource.h"

IMPLEMENT_DYNAMIC(ComboBoxEx, CComboBox)

ComboBoxEx::ComboBoxEx()
{
	m_strText = "";

	m_bImg = FALSE;
	m_pbtnCombo = new CGdiPlusBitmapResource;
	m_pbtnCombo->Load(MAKEINTRESOURCE(IDB_PNG_COMBOBOX), _T("PNG"), AfxGetApp()->m_hInstance);
}

ComboBoxEx::~ComboBoxEx()
{
}

BEGIN_MESSAGE_MAP(ComboBoxEx, CComboBox)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &ComboBoxEx::OnCbnSelchange)
END_MESSAGE_MAP()

void ComboBoxEx::IsArrowImage(BOOL bImg)
{
	m_bImg = bImg;
}

void ComboBoxEx::OnPaint()
{
	CPaintDC dc(this); 
		
	Graphics graphics(dc.GetSafeHdc());

	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	Color bgColor(Gdiplus::Color(255, 255, 255, 255));
	graphics.Clear(bgColor);

	CRect rc;
	GetClientRect(&rc);

	//사각형 라인
	DrawBorder(graphics, rc);

	//클릭 버튼 라인
	DrawArrowBorder(graphics, rc);
	           
	//선택된 텍스트
   	DrawSelectText(graphics, rc);
}

void ComboBoxEx::OnDrawLayerdWindow(Graphics& Gps)
{
	USES_CONVERSION;

	Gps.SetSmoothingMode(SmoothingModeHighQuality);
	Gps.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	CRect rc;
	GetWindowRect(rc);
	GetOwner()->ScreenToClient(rc);

	//사각형 라인
	DrawBorder(Gps, rc);

	//클릭 버튼 라인
	DrawArrowBorder(Gps, rc);

	//선택된 텍스트
	DrawSelectText(Gps, rc);
}

void ComboBoxEx::DrawBorder(Graphics& Gps, CRect rc)
{
	RectF destRect(REAL(rc.left), REAL(rc.top), REAL(rc.Width()-1), REAL(rc.Height()-1));
			
	//사각형 라인
	Pen pen(Color(255, 122, 122, 122), 1);
	pen.SetDashStyle(Gdiplus::DashStyle::DashStyleSolid);
	Gps.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gps.DrawRectangle(&pen, destRect);	
}

void ComboBoxEx::DrawArrowBorder(Graphics& Gps, CRect rc)
{
	int buttonWidth = GetSystemMetrics(SM_CXVSCROLL);
	rc.right -= 2;
	rc.left = rc.right - buttonWidth;


	RectF rectF(REAL(rc.left), REAL(rc.top), REAL(rc.Width() +1 ), REAL(rc.Height() - 1));

	//사각형 라인
	/*
	Pen pen(Color(255, 0, 0, 255), 1);
	pen.SetDashStyle(Gdiplus::DashStyle::DashStyleSolid);
	Gps.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	Gps.DrawRectangle(&pen, rectF);
	*/

	//사각형 채우기
	/*
	Gdiplus::SolidBrush brush(Color(255, 255, 255, 255));
	Gps.FillRectangle(&brush, rectF);
	*/

	//라인 긋기
	/*
	Gdiplus::Pen pen({ 122, 122, 122 }, 0.3);
	Gps.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	Gps.DrawLine(&pen, rc.left - 2, rc.top + 6, rc.left - 2, rc.bottom - 6);
	*/

	//클릭 버튼 이미지 혹은 텍스트
	if (m_bImg)
	{
		//클릭 버튼 이미지
		if (m_pbtnCombo->m_pBitmap != NULL) {
			Gps.DrawImage(m_pbtnCombo->m_pBitmap, Rect(rc.left+1, rc.top + 6, 15, 11), 0, 0, 15, 11, UnitPixel);
		}
	}
	else
	{		
		//클릭 버튼 텍스트
		Gdiplus::Font font(L"Tahoma", 11, FontStyleRegular, UnitPixel);
		SolidBrush brush(Color(122, 122, 122));
		StringFormat stringFormat(StringFormatFlagsDisplayFormatControl);
		stringFormat.SetAlignment(StringAlignmentCenter);
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		Gps.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
		Gps.DrawString(_T("v"), -1, &font, rectF, &stringFormat, &brush);
	}
}

void ComboBoxEx::DrawSelectText(Graphics& Gps, CRect rc)
{
	int buttonWidth = GetSystemMetrics(SM_CXVSCROLL) + 4;
	
	//선택된 데이터
	int nIndex = GetCurSel();
	if (nIndex != -1)
	{
		GetLBText(nIndex, m_strText);
	}

	Gdiplus::Font font(L"Tahoma", 13, FontStyleRegular, UnitPixel);
	SolidBrush brush(Color(122, 122, 122));

	RectF rectF(REAL(rc.left), REAL(rc.top), REAL(rc.Width() - buttonWidth), REAL(rc.Height() - 1));

	//영역 확인
	//Gdiplus::SolidBrush brush1(Color(255, 255, 255, 255));
	//Gps.FillRectangle(&brush1, rectF);

	StringFormat stringFormat(StringFormatFlagsDisplayFormatControl);
	stringFormat.SetAlignment(StringAlignmentCenter);//stringFormat.SetAlignment(StringAlignmentNear);
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);//stringFormat.SetLineAlignment(StringAlignmentNear);
	Gps.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	Gps.DrawString(m_strText, -1, &font, rectF, &stringFormat, &brush);
}

void ComboBoxEx::OnCbnSelchange()
{
	Invalidate(TRUE);

	//투명 원도우일 경우 강제로 OnDrawLayerdWindow 태우기 위해 호출(투명원도우 일때만 필요)
	::SendMessage(GetParent()->m_hWnd, WM_PAINT, 0, 0);
}