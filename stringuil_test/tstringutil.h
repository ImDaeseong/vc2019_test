#pragma once

// TCHAR 기반 std::string 정의
typedef std::basic_string<TCHAR> tstring;

class tstringutil
{
public:
    static tstring Mid(const tstring& str, size_t off, size_t count);
    static tstring Left(const tstring& str, size_t count);
    static tstring Right(const tstring& str, size_t count);
    static tstring MakeLower(const tstring& str);
    static tstring MakeUpper(const tstring& str);
    static size_t Find(const tstring& str, const tstring& strsub, size_t off = 0);
    static size_t Find(const tstring& str, TCHAR ch, size_t off = 0);
    static size_t ReserveFind(const tstring& str, const tstring& strsub);
    static size_t ReserveFind(const tstring& str, TCHAR ch);
    static size_t Split(const tstring& str, const tstring& delimiter, std::vector<tstring>& result);
    static tstring Trim(const tstring& str);
    static tstring TrimLeft(const tstring& str);
    static tstring TrimRight(const tstring& str);
    static tstring Reserve(const tstring& str);
    static tstring Replace(const tstring& str, const tstring& search, const tstring& replace);
    static tstring GetFilePath(const tstring& str);
    static tstring GetFileName(const tstring& str);
    static tstring RemoveExt(const tstring& str);
    static tstring GetFileExt(const tstring& str);
    static tstring GetFindStr(const tstring& str, const tstring& left, const tstring& right);
};

