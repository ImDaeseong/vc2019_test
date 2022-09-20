#pragma once

class converUtil
{
public:
	static char* ConvertUnicodeToMultybyte(CString strUnicode);
	static CString ConvertMultibyteToUnicode(char* pMultibyte);
};
