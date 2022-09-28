#pragma once
class CCheckStatic : public CStatic
{
public:
	CCheckStatic();
	virtual ~CCheckStatic();

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();
	DECLARE_MESSAGE_MAP()	

public:
	void OnDrawLayerdWindow(Graphics& Gps);

	void SetCheckIndex(int nIndex);

private:
	int m_nIndex;
	BOOL m_bBorder;
};

