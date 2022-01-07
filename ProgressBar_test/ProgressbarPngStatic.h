#pragma once

class ProgressbarPngStatic : public CStatic
{
public:
	ProgressbarPngStatic();
	virtual ~ProgressbarPngStatic();

public:
	void SetImage(LPCTSTR pImgLeft, LPCTSTR pImgRight, LPCTSTR pType, HMODULE hInst);
	void SetNoImage();

	void SetRange(int nMin, int nMax);
	int SetPos(int nPos);

	BOOL IsAvailableDraw();
	void OnDrawLayerdWindow(Graphics& Gps);

protected:
	virtual void PreSubclassWindow();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	CGdiPlusBitmapResource* m_pImgLeft;
	CGdiPlusBitmapResource* m_pImgRight;

	int m_nMin;
	int m_nMax;
	int m_nPos;

	void DrawLeft(Graphics& Gps, CRect rc);
	void DrawRight(Graphics& Gps, CRect rc);
	void DrawPercent(Graphics& Gps, CRect rc, CString strPercent);
	void DrawBorder(Graphics& Gps, CRect rc);
	void DrawBorderNormal(Graphics& Gps, CRect rc);

	BOOL       m_bGradient;
	COLORREF   m_crGrBG1;
	COLORREF   m_crGrBG2;
};
