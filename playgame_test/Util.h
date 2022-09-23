#pragma once

class Util
{
public:
	static CString GetModulePath();
	static BOOL IsWindows64Bit();
	static CString GetPrcessFileName(DWORD processID);
	static BOOL runAppplayer();
};
