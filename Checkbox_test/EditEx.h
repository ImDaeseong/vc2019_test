#pragma once

//ÇÑ±Û
#include <imm.h>
#pragma comment(lib, "imm32")

class CEditEx : public CEdit
{
public:
	CEditEx();
	virtual ~CEditEx();

	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnKillfocus();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

private:
	COLORREF m_clrText;
	COLORREF m_clrBack;
	CBrush   m_brBack;
	CString  m_strPlaceholder;
	CString  m_strInput;
};

