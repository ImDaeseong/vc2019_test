#pragma once
#include <functional> 

class CButtonEx : public CWnd
{
public:
	CButtonEx();
	virtual ~CButtonEx();

	BOOL Create(HWND hParent, CRect rRect, const CString& strText);
	void SetText(const CString& strText);
    void SetOnClick(void (*handler)());

    void SetOnClick(std::function<void()> handler);

protected:
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()

    void UpdateLayered();

private:
    CString m_strText;
    
    std::function<void()> m_clickHandler;
};
