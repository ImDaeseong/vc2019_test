#pragma once

class CAppPlayertestDlg : public CDialogEx
{
public:
	CAppPlayertestDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APPPLAYER_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	void NoxInfo();
	Nox objNox;

	void LDPlayer4Info();
	LDPlayer4 objLDPlayer4;

	void BlueStacks4Info();
	BlueStacks4 objBlueStacks4;

	void BlueStacks5Info();
	BlueStacks5 objBlueStacks5;
};
