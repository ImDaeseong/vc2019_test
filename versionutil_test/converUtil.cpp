#include "pch.h"
#include "converUtil.h"

//유니코드 -> 멀티바이트
char* converUtil::ConvertUnicodeToMultybyte(CString strUnicode)
{
	int len = WideCharToMultiByte(CP_ACP, 0, strUnicode, strUnicode.GetLength(), NULL, 0, NULL, NULL);

	char* chData = new char[len + 1];

	WideCharToMultiByte(CP_ACP, 0, strUnicode, strUnicode.GetLength(), chData, len, NULL, NULL);

	chData[len] = '\0';
	return chData;
}

//멀티바이트 -> 유니코드
CString converUtil::ConvertMultibyteToUnicode(char* pMultibyte)
{
	int wLen = MultiByteToWideChar(CP_ACP, 0, pMultibyte, -1, NULL, 0);

	CString strUnicode;

	LPTSTR wStr = strUnicode.GetBufferSetLength(wLen - 1);
	MultiByteToWideChar(CP_ACP, 0, pMultibyte, -1, wStr, wLen - 1);

	return strUnicode;
}