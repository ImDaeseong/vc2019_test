#pragma once

class DragStatic : public CStatic
{
public:
	DragStatic();
	virtual ~DragStatic();

	CString	m_strDragFile;

	void SetImage(LPCTSTR pName, LPCTSTR pType, HMODULE hInst);

	BOOL IsAvailableDraw();
	void OnDrawLayerdWindow(Graphics& Gps);

protected:
	afx_msg void OnPaint();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual void PreSubclassWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	CGdiPlusBitmapResource* m_pImage;

	BOOL m_bCapture;
	CPoint m_pPos;
};

