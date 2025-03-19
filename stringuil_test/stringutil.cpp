#include "pch.h"
#include "stringutil.h"

std::string stringutil::Mid(const std::string& str, size_t off, size_t count)
{
    return str.substr(off, count);
}

std::string stringutil::Left(const std::string& str, size_t count)
{
    return str.substr(0, count);
}

std::string stringutil::Right(const std::string& str, size_t count)
{
    if (count > str.size()) {
        count = str.size(); // 문자열 길이보다 큰 값이 들어올 경우 처리
    }
    return str.substr(str.size() - count, count);
}

std::string stringutil::MakeLower(const std::string& str)
{
    std::string result;
    result.reserve(str.size()); // 미리 메모리 할당으로 성능 향상
    std::transform(str.begin(), str.end(), std::back_inserter(result), ::tolower);
    return result;
}

std::string stringutil::MakeUpper(const std::string& str)
{
    std::string result;
    result.reserve(str.size()); // 미리 메모리 할당으로 성능 향상
    std::transform(str.begin(), str.end(), std::back_inserter(result), ::toupper);
    return result;
}

size_t stringutil::Find(const std::string& str, const std::string& strsub, size_t off)
{
    return str.find(strsub, off);
}

size_t stringutil::Find(const std::string& str, char ch, size_t off)
{
    return str.find(ch, off);
}

size_t stringutil::ReserveFind(const std::string& str, const std::string& strsub)
{
    return str.rfind(strsub);
}

size_t stringutil::ReserveFind(const std::string& str, char ch)
{
    return str.rfind(ch);
}

size_t stringutil::Split(const std::string& str, const std::string& delimiter, std::vector<std::string>& result)
{
    result.clear();

    if (str.empty()) {
        return 0;
    }

    if (delimiter.empty()) {
        result.push_back(str);
        return 1;
    }

    size_t pos = 0;
    size_t found;
    std::string temp = str; // 원본 문자열 보존

    while ((found = temp.find(delimiter, pos)) != std::string::npos) {
        result.push_back(temp.substr(pos, found - pos));
        pos = found + delimiter.length();
    }

    // 마지막 조각 추가
    if (pos <= temp.length()) {
        result.push_back(temp.substr(pos));
    }

    return result.size();
}

std::string stringutil::Trim(const std::string& str)
{
    return TrimRight(TrimLeft(str));
}

std::string stringutil::TrimLeft(const std::string& str)
{
    std::string result = str;
    size_t pos = result.find_first_not_of(" \t\r\n");

    if (pos == std::string::npos) {
        return ""; // 모두 공백인 경우
    }

    result.erase(0, pos);
    return result;
}

std::string stringutil::TrimRight(const std::string& str)
{
    std::string result = str;
    size_t pos = result.find_last_not_of(" \t\r\n");

    if (pos == std::string::npos) {
        return ""; // 모두 공백인 경우
    }

    result.erase(pos + 1);
    return result;
}

std::string stringutil::Reserve(const std::string& str)
{
    std::string result = str;
    std::reverse(result.begin(), result.end()); // 표준 알고리즘 사용
    return result;
}

std::string stringutil::Replace(const std::string& str, const std::string& search, const std::string& replace)
{
    if (search.empty()) {
        return str; // 검색 문자열이 비어있으면 원본 반환
    }

    std::string result = str;
    size_t pos = 0;

    while ((pos = result.find(search, pos)) != std::string::npos) {
        result.replace(pos, search.length(), replace);
        pos += replace.length();
    }

    return result;
}

std::string stringutil::GetFilePath(const std::string& str)
{
    size_t pos = str.find_last_of("\\/");
    if (pos != std::string::npos) {
        return str.substr(0, pos);
    }
    return "";
}

std::string stringutil::GetFileName(const std::string& str)
{
    size_t pos = str.find_last_of("\\/");
    if (pos != std::string::npos) {
        return str.substr(pos + 1);
    }
    return str;
}

std::string stringutil::RemoveExt(const std::string& str)
{
    size_t pos = str.find_last_of(".");
    if (pos != std::string::npos) {
        return str.substr(0, pos);
    }
    return str;
}

std::string stringutil::GetFileExt(const std::string& str)
{
    size_t pos = str.find_last_of(".");
    if (pos != std::string::npos) {
        return str.substr(pos + 1);
    }
    return "";
}

std::string stringutil::GetFindStr(const std::string& str, const std::string& left, const std::string& right)
{
    size_t leftPos = str.find(left);
    if (leftPos == std::string::npos) {
        return "";
    }

    leftPos += left.length(); // left 문자열 다음부터 시작
    size_t rightPos = str.find(right, leftPos);

    if (rightPos == std::string::npos) {
        return "";
    }

    return str.substr(leftPos, rightPos - leftPos);
}

/*
#include "pch.h"
#include "stringutil.h"

std::string stringutil::Mid(const std::string& _Str, size_t _Off, size_t _Count)
{
	return _Str.substr(_Off, _Count);
}

std::string stringutil::Left(const std::string& _Str, size_t _Count)
{
	return _Str.substr(0, _Count);
}

std::string stringutil::Right(const std::string& _Str, size_t _Count)
{
	return _Str.substr(_Str.size() - _Count, _Count);
}

std::string stringutil::MakeLower(const std::string& _Str)
{
	std::string _DStr;
	std::transform(_Str.begin(), _Str.end(), std::back_inserter(_DStr), ::tolower);
	//std::transform(_Str.begin(), _Str.end(), _Str.begin(), ::tolower);
	return _DStr;
}

std::string stringutil::MakeUpper(const std::string& _Str)
{
	std::string _DStr;
	std::transform(_Str.begin(), _Str.end(), std::back_inserter(_DStr), ::toupper);
	//std::transform(_Str.begin(), _Str.end(), _Str.begin(), ::toupper);
	return _DStr;
}

size_t stringutil::Find(const std::string& _Str, const std::string& _Strsub, size_t _Off)
{
	return _Str.find(_Strsub, _Off);
}

size_t stringutil::Find(const std::string& _Str, char ch, size_t _Off)
{
	return _Str.find(ch, _Off);
}

size_t stringutil::ReserveFind(const std::string& _Str, const std::string& _Strsub)
{
	return _Str.rfind(_Strsub);
}

size_t stringutil::ReserveFind(const std::string& _Str, char ch)
{
	return _Str.rfind(ch);
}

size_t stringutil::Split(std::string& _Str, std::string& _Strsub, std::vector<std::string>& _Vec)
{
	_Vec.clear();

	std::string::size_type pos;

	pos = _Str.find(_Strsub);
	while (pos != std::string::npos) {

		_Vec.push_back(_Str.substr(0, pos));

		_Str = _Str.substr(pos + 1);
		pos = _Str.find(_Strsub);
	}

	_Vec.push_back(_Str);

	return _Vec.size();
}

std::string stringutil::Trim(const std::string& _Str)
{
	std::string _DStr{ _Str };

	_DStr = TrimLeft(_DStr);
	_DStr = TrimRight(_DStr);

	return _DStr;
}

std::string stringutil::TrimLeft(const std::string& _Str)
{
	std::string _DStr{ _Str };

	size_t nPos = _DStr.find_first_not_of(" \t\r");
	if (nPos > 0)
		_DStr = _DStr.substr(nPos);

	return _DStr;
}

std::string stringutil::TrimRight(const std::string& _Str)
{
	std::string _DStr{ _Str };

	size_t nPos = _DStr.find_last_not_of(" \t\r");

	if (nPos > 0)
		_DStr = _DStr.substr(0, nPos + 1);
	return _DStr;
}

std::string stringutil::Reserve(const std::string& _Str)
{
	std::string _DStr = { };

	for (int i = _Str.size() - 1; i >= 0; --i) {
		_DStr += _Str[i];
	}

	return _DStr;
}

std::string stringutil::Replace(const std::string& _Str, const std::string& _Search, const std::string& _Replace)
{
	std::string _DStr = { _Str };

	size_t pos = 0;
	while ((pos = _DStr.find(_Search, pos)) != std::string::npos)
	{
		_DStr.replace(pos, _Search.length(), _Replace);

		pos += _Replace.length();
	}
	return _DStr;
}

std::string stringutil::GetFilePath(const std::string& _Str)
{
	size_t nPos = _Str.rfind("\\");
	if (nPos > 0)
		return Left(_Str, nPos);

	return _Str;
}

std::string stringutil::GetFileName(const std::string& _Str)
{
	size_t nPos = _Str.rfind("\\");
	if (nPos > 0)
		return _Str.substr(nPos + 1);

	return _Str;
}

std::string stringutil::RemoveExt(const std::string& _Str)
{
	return _Str.substr(0, _Str.find_last_of("."));
}

std::string stringutil::GetFileExt(const std::string& _Str)
{
	size_t nPos = _Str.rfind('.');
	if (nPos > 0)
		return _Str.substr(nPos + 1);

	return _Str;
}

std::string stringutil::GetFindStr(const std::string& _Str, const std::string& _left, const std::string& _right)
{
	size_t nleft = _Str.find(_left);
	size_t nright = _Str.find(_right);

	if (nleft > 0 && nright > 0)
		return Mid(_Str, nleft, (nright - nleft));//return Mid(_Str, nleft + 1, (nright - nleft - 1));

	return _Str;
}
*/