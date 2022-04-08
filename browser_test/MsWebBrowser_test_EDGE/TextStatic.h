#pragma once
class CTextStatic : public CStatic
{
	DECLARE_DYNAMIC(CTextStatic)

public:
	CTextStatic();
	virtual ~CTextStatic();

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()

public:
	void SetMesssageText(CString sMessage);
	void SetMesssageFontSize(int nFontSize);
	void OnDrawLayerdWindow(Graphics& Gps);
	void SetBannerClick(BOOL bClick);

private:
	CString m_strMessage;
	int m_nFontSize;
	BOOL	m_bClick;
};

