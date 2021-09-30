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
	DECLARE_MESSAGE_MAP()

private:
	CGdiPlusBitmapResource* m_pImage;
};

