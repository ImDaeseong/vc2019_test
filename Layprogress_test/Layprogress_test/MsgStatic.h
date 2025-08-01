#pragma once

class CMsgStatic : public CStatic
{
	DECLARE_DYNAMIC(CMsgStatic)

public:
	CMsgStatic();
	virtual ~CMsgStatic();

	void SetMesssageText(CString sMessage);
	void SetMesssageFontSize(int nFontSize);
	void OnDrawLayerdWindow(Graphics& Gps);

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

private:
	CString m_strMessage;	
	int m_nFontSize;
};