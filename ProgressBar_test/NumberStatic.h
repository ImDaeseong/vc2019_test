#pragma once

class NumberStatic : public CStatic
{
public:
	NumberStatic();
	virtual ~NumberStatic();

public:
	void SetImagePath(CString strPath, int nDivide = 1);
	void SetImage(LPCTSTR pName, LPCTSTR pType, HMODULE hInst);

	BOOL IsAvailableDraw();
	void OnDrawLayerdWindow(Graphics& Gps);

	NumberStatic& SetPictureStaticClick(BOOL bClick = FALSE);
	void SetPictureStaticCursor(BOOL bCursor = FALSE);

	int GetImageWidth() const;
	int GetImageHeight() const;

	void IsShow(BOOL bShow);

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()

private:
	BOOL     m_bClick;
	BOOL     m_bCursor;
	CGdiPlusBitmapResource* m_pImage;
	int m_nDivide;
	BOOL     m_bShow;
};