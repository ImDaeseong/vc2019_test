#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

class CMsWebBrowsertestApp : public CWinApp
{
public:
	CMsWebBrowsertestApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

private:
	void RegisterWin();

	ULONG_PTR	m_gdiplusToken;

	HANDLE m_hMutex;

public:
	class CImpIDispatch* m_pDispOM;
};

extern CMsWebBrowsertestApp theApp;
