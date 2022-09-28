#include "pch.h"
#include "EditEx.h"

CEditEx::CEditEx()
{
	m_clrText = RGB(0, 0, 0);//텍스트 색상
	m_clrBack = RGB(244, 245, 247);//에디트 배경 색상

	m_strPlaceholder = _T("30자까지 등록가능합니다.");	

	m_strInput = _T("");
}

CEditEx::~CEditEx()
{
}

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_NCPAINT()
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CEditEx::OnEnSetfocus)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &CEditEx::OnEnKillfocus)
	ON_WM_CHAR()
END_MESSAGE_MAP()

HBRUSH CEditEx::CtlColor(CDC* pDC, UINT nCtlColor)
{
	m_brBack.DeleteObject();

	m_brBack.CreateSolidBrush(m_clrBack);

	pDC->SetBkColor(m_clrBack);

	pDC->SetTextColor(m_clrText);

	return (HBRUSH)m_brBack;
}

void CEditEx::OnNcPaint()
{
	CDC* pDC = GetWindowDC();

	CRect rect;
	GetWindowRect(&rect);
	rect.OffsetRect(-rect.left, -rect.top);

	//border line 배경색과 동일한 색상으로 변경해서 border line 을 숨김
	CBrush brush(RGB(244, 245, 247));
	pDC->FrameRect(&rect, &brush);
	ReleaseDC(pDC);
}

void CEditEx::OnEnSetfocus()
{
	//한글 입력
	if( GetSafeHwnd() != NULL )
	{
		HIMC himc = ImmGetContext(GetSafeHwnd());	
		ImmSetConversionStatus(himc, IME_CMODE_NATIVE, IME_SMODE_CONVERSATION);
		ImmReleaseContext(GetSafeHwnd(), himc);
	}

	if (m_strInput.GetLength() > 0)
	{
		//SetWindowText(m_strInput);

		//커서를 문자열 제일 끝으로 이동
		//SetSel(-1, -1);

		SetSel(0, -1);
		ReplaceSel(m_strInput, FALSE);
	}
	else
	{
		SetWindowText(_T(""));
	}	
}

void CEditEx::OnEnKillfocus()
{
	if (GetWindowTextLength() == 0) {

		SetWindowText(m_strPlaceholder);

		m_strInput = _T("");
	}
	else
	{
		GetWindowText(m_strInput);
		OutputDebugString(m_strInput + _T("\r\n"));
	}
}

void CEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//GetWindowText(m_strInput);
	//OutputDebugString(m_strInput + _T("\r\n"));
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
