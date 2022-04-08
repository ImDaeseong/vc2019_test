#pragma once

namespace Common_Util
{
	Bitmap* Load(LPCTSTR pName, LPCTSTR pType, HMODULE hInst);
	Bitmap* ResizeBitmap(Bitmap* pInImg, int nResizeWidth, int nResizeHeight);
	void SetBrowserEmulation(CString strFileName);
	BOOL IsWin7OsVersion();
	CString GetCurrentUserSID();
	HANDLE GetExplorerProcessHandle();
};

