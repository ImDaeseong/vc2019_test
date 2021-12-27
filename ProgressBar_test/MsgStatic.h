#pragma once

class CMsgStatic : public CStatic
{
	DECLARE_DYNAMIC(CMsgStatic)

public:
	CMsgStatic();
	virtual ~CMsgStatic();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();

	void SetMesssageText(CString sMessage);
	void SetMesssageFontSize(int nFontSize);
	CString m_strMessage;	
	int m_nFontSize;

	void OnDrawLayerdWindow(Graphics& Gps);
};