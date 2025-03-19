#pragma once

class stringutil
{
public:
    static std::string Mid(const std::string& str, size_t off, size_t count);
    static std::string Left(const std::string& str, size_t count);
    static std::string Right(const std::string& str, size_t count);
    static std::string MakeLower(const std::string& str);
    static std::string MakeUpper(const std::string& str);
    static size_t Find(const std::string& str, const std::string& strsub, size_t off = 0);
    static size_t Find(const std::string& str, char ch, size_t off = 0);
    static size_t ReserveFind(const std::string& str, const std::string& strsub);
    static size_t ReserveFind(const std::string& str, char ch);
    static size_t Split(const std::string& str, const std::string& delimiter, std::vector<std::string>& result);
    static std::string Trim(const std::string& str);
    static std::string TrimLeft(const std::string& str);
    static std::string TrimRight(const std::string& str);
    static std::string Reserve(const std::string& str);
    static std::string Replace(const std::string& str, const std::string& search, const std::string& replace);
    static std::string GetFilePath(const std::string& str);
    static std::string GetFileName(const std::string& str);
    static std::string RemoveExt(const std::string& str);
    static std::string GetFileExt(const std::string& str);
    static std::string GetFindStr(const std::string& str, const std::string& left, const std::string& right);
};

/*
#pragma once
class stringutil
{
public:
	static std::string Left(const std::string& _Str, size_t _Count);
	static std::string Right(const std::string& _Str, size_t _Count);
	static std::string Mid(const std::string& _Str, size_t _Off, size_t _Count);
	static std::string MakeLower(const std::string& _Str);
	static std::string MakeUpper(const std::string& _Str);
	static size_t Find(const std::string& _Str, const std::string& _Strsub, size_t _Off);
	static size_t Find(const std::string& _Str, char ch, size_t _Off);
	static size_t ReserveFind(const std::string& _Str, const std::string& _Strsub);
	static size_t ReserveFind(const std::string& _Str, char ch);
	static size_t Split(std::string& _Str, std::string& _Strsub, std::vector<std::string>& _Vec);
	static std::string Trim(const std::string& _Str);
	static std::string TrimLeft(const std::string& _Str);
	static std::string TrimRight(const std::string& _Str);
	static std::string Reserve(const std::string& _Str);
	static std::string Replace(const std::string& _Str, const std::string& _Search, const std::string& _Replace);

	static std::string GetFilePath(const std::string& _Str);
	static std::string GetFileName(const std::string& _Str);
	static std::string RemoveExt(const std::string& _Str);
	static std::string GetFileExt(const std::string& _Str);
	static std::string GetFindStr(const std::string& _Str, const std::string& _left, const std::string& _right);
};
*/
