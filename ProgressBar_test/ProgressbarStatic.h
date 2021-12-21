#pragma once

class ProgressbarStatic : public CStatic
{
public:
	ProgressbarStatic();
	virtual ~ProgressbarStatic();

public:
	void SetRange(int nMin, int nMax);
	int SetPos(int nPos);

	void OnDrawLayerdWindow(Graphics& Gps);

protected:
	virtual void PreSubclassWindow();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	int m_nMin;
	int m_nMax;
	int m_nPos;

	void DrawLeft(Graphics& Gps, CRect rc);
	void DrawRight(Graphics& Gps, CRect rc);
	void DrawPercent(Graphics& Gps, CRect rc, CString strPercent);
	void DrawBorder(Graphics& Gps, CRect rc);
	void DrawBorderNormal(Graphics& Gps, CRect rc);
};

