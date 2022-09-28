#pragma once

class CCheckButton : public CButton
{
	DECLARE_DYNAMIC(CCheckButton)

public:
	CCheckButton();
	virtual ~CCheckButton();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

public:
	BOOL SetImage(UINT uid, LPCTSTR pType, HMODULE hInst, int nStartX, int nStartY, int nDivide = 2);
	BOOL SetImage(LPCTSTR pName, LPCTSTR pType, HMODULE hInst, int nStartX, int nStartY, int nDivide = 2);
	
	int GetImageWidth() const;
	int GetImageHeight() const;

	BOOL IsAvailableDraw();
	void OnDrawWindow(Graphics& Gps);
	void OnDrawLayerdWindow(Graphics& Gps);

	void SetCheckState();
	BOOL GetCheckState();
	
private:	
	CGdiPlusBitmapResource* m_pBGImage;

	int m_nStartX;
	int m_nStartY;
	int m_nDivide;
	BOOL m_Clicked;	
};

