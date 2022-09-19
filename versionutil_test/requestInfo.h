#pragma once


#include <afxinet.h>

class requestInfo
{
public:
	static bool GetPageUrl(CString strUrl);

private:
	static bool getWebPage(CString strUrl);
};

