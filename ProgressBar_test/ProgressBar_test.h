﻿#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

class CProgressBartestApp : public CWinApp
{
public:
	CProgressBartestApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

protected:
	ULONG_PTR m_gdiplusToken;
	void SetClassName();
};

extern CProgressBartestApp theApp;
