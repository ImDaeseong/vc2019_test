#pragma once

class ComboBoxEx : public CComboBox
{
	DECLARE_DYNAMIC(ComboBoxEx)

public:
	ComboBoxEx();
	virtual ~ComboBoxEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchange();

public:
	void OnDrawLayerdWindow(Graphics& Gps);
	void IsArrowImage(BOOL bImg);

private:	
	CString m_strText;

	BOOL m_bImg;
	CGdiPlusBitmapResource* m_pbtnCombo;
		
	void DrawBorder(Graphics& Gps, CRect rc);
	void DrawArrowBorder(Graphics& Gps, CRect rc);
	void DrawSelectText(Graphics& Gps, CRect rc);	
};


