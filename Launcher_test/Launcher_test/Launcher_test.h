#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CLaunchertestApp : public CWinApp
{
public:
	CLaunchertestApp();

public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

protected:
	ULONG_PTR m_gdiplusToken;
	void SetClassName();
};

extern CLaunchertestApp theApp;
