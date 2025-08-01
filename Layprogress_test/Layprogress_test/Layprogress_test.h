#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

class CLayprogresstestApp : public CWinApp
{
public:
	CLayprogresstestApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()

protected:
	ULONG_PTR m_gdiplusToken;
};

extern CLayprogresstestApp theApp;
