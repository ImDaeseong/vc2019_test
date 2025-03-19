#include "pch.h"
#include "wstringutil.h"

std::wstring wstringutil::Mid(const std::wstring& str, size_t off, size_t count)
{
    return str.substr(off, count);
}

std::wstring wstringutil::Left(const std::wstring& str, size_t count)
{
    return str.substr(0, count);
}

std::wstring wstringutil::Right(const std::wstring& str, size_t count)
{
    if (count > str.size()) {
        count = str.size(); // 문자열 길이보다 큰 값이 들어올 경우 처리
    }
    return str.substr(str.size() - count, count);
}

std::wstring wstringutil::MakeLower(const std::wstring& str)
{
    std::wstring result;
    result.reserve(str.size()); // 미리 메모리 할당으로 성능 향상
    std::transform(str.begin(), str.end(), std::back_inserter(result), ::towlower);
    return result;
}

std::wstring wstringutil::MakeUpper(const std::wstring& str)
{
    std::wstring result;
    result.reserve(str.size()); // 미리 메모리 할당으로 성능 향상
    std::transform(str.begin(), str.end(), std::back_inserter(result), ::towupper);
    return result;
}

size_t wstringutil::Find(const std::wstring& str, const std::wstring& strsub, size_t off)
{
    return str.find(strsub, off);
}

size_t wstringutil::Find(const std::wstring& str, wchar_t ch, size_t off)
{
    return str.find(ch, off);
}

size_t wstringutil::ReserveFind(const std::wstring& str, const std::wstring& strsub)
{
    return str.rfind(strsub);
}

size_t wstringutil::ReserveFind(const std::wstring& str, wchar_t ch)
{
    return str.rfind(ch);
}

size_t wstringutil::Split(const std::wstring& str, const std::wstring& delimiter, std::vector<std::wstring>& result)
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
    std::wstring temp = str; // 원본 문자열 보존

    while ((found = temp.find(delimiter, pos)) != std::wstring::npos) {
        result.push_back(temp.substr(pos, found - pos));
        pos = found + delimiter.length();
    }

    // 마지막 조각 추가
    if (pos <= temp.length()) {
        result.push_back(temp.substr(pos));
    }

    return result.size();
}

std::wstring wstringutil::Trim(const std::wstring& str)
{
    return TrimRight(TrimLeft(str));
}

std::wstring wstringutil::TrimLeft(const std::wstring& str)
{
    std::wstring result = str;
    size_t pos = result.find_first_not_of(L" \t\r\n");

    if (pos == std::wstring::npos) {
        return L""; // 모두 공백인 경우
    }

    result.erase(0, pos);
    return result;
}

std::wstring wstringutil::TrimRight(const std::wstring& str)
{
    std::wstring result = str;
    size_t pos = result.find_last_not_of(L" \t\r\n");

    if (pos == std::wstring::npos) {
        return L""; // 모두 공백인 경우
    }

    result.erase(pos + 1);
    return result;
}

std::wstring wstringutil::Reserve(const std::wstring& str)
{
    std::wstring result = str;
    std::reverse(result.begin(), result.end()); // 표준 알고리즘 사용
    return result;
}

std::wstring wstringutil::Replace(const std::wstring& str, const std::wstring& search, const std::wstring& replace)
{
    if (search.empty()) {
        return str; // 검색 문자열이 비어있으면 원본 반환
    }

    std::wstring result = str;
    size_t pos = 0;

    while ((pos = result.find(search, pos)) != std::wstring::npos) {
        result.replace(pos, search.length(), replace);
        pos += replace.length();
    }

    return result;
}

std::wstring wstringutil::GetFilePath(const std::wstring& str)
{
    size_t pos = str.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        return str.substr(0, pos);
    }
    return L"";
}

std::wstring wstringutil::GetFileName(const std::wstring& str)
{
    size_t pos = str.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        return str.substr(pos + 1);
    }
    return str;
}

std::wstring wstringutil::RemoveExt(const std::wstring& str)
{
    size_t pos = str.find_last_of(L".");
    if (pos != std::wstring::npos) {
        return str.substr(0, pos);
    }
    return str;
}

std::wstring wstringutil::GetFileExt(const std::wstring& str)
{
    size_t pos = str.find_last_of(L".");
    if (pos != std::wstring::npos) {
        return str.substr(pos + 1);
    }
    return L"";
}

std::wstring wstringutil::GetFindStr(const std::wstring& str, const std::wstring& left, const std::wstring& right)
{
    size_t leftPos = str.find(left);
    if (leftPos == std::wstring::npos) {
        return L"";
    }

    leftPos += left.length(); // left 문자열 다음부터 시작
    size_t rightPos = str.find(right, leftPos);

    if (rightPos == std::wstring::npos) {
        return L"";
    }

    return str.substr(leftPos, rightPos - leftPos);
}

/*
#include "pch.h"
#include "wstringutil.h"

std::wstring wstringutil::Mid(const std::wstring& _Str, size_t _Off, size_t _Count)
{
	return _Str.substr(_Off, _Count);
}

std::wstring wstringutil::Left(const std::wstring& _Str, size_t _Count)
{
	return _Str.substr(0, _Count);
}

std::wstring wstringutil::Right(const std::wstring& _Str, size_t _Count)
{
	return _Str.substr(_Str.size() - _Count, _Count);
}

std::wstring wstringutil::MakeLower(const std::wstring& _Str)
{
	std::wstring _DStr;
	std::transform(_Str.begin(), _Str.end(), std::back_inserter(_DStr), ::tolower);
	//std::transform(_Str.begin(), _Str.end(), _Str.begin(), ::tolower);
	return _DStr;
}

std::wstring wstringutil::MakeUpper(const std::wstring& _Str)
{
	std::wstring _DStr;
	std::transform(_Str.begin(), _Str.end(), std::back_inserter(_DStr), ::toupper);
	//std::transform(_Str.begin(), _Str.end(), _Str.begin(), ::toupper);
	return _DStr;
}

size_t wstringutil::Find(const std::wstring& _Str, const std::wstring& _Strsub, size_t _Off)
{
	return _Str.find(_Strsub, _Off);
}

size_t wstringutil::Find(const std::wstring& _Str, char ch, size_t _Off)
{
	return _Str.find(ch, _Off);
}

size_t wstringutil::ReserveFind(const std::wstring& _Str, const std::wstring& _Strsub)
{
	return _Str.rfind(_Strsub);
}

size_t wstringutil::ReserveFind(const std::wstring& _Str, char ch)
{
	return _Str.rfind(ch);
}

size_t wstringutil::Split(std::wstring& _Str, std::wstring& _Strsub, std::vector<std::wstring>& _Vec)
{
	_Vec.clear();

	std::wstring::size_type pos;

	pos = _Str.find(_Strsub);
	while (pos != std::string::npos) {

		_Vec.push_back(_Str.substr(0, pos));

		_Str = _Str.substr(pos + 1);
		pos = _Str.find(_Strsub);
	}

	_Vec.push_back(_Str);

	return _Vec.size();
}

std::wstring wstringutil::Trim(const std::wstring& _Str)
{
	std::wstring _DStr{ _Str };

	_DStr = TrimLeft(_DStr);
	_DStr = TrimRight(_DStr);

	return _DStr;
}

std::wstring wstringutil::TrimLeft(const std::wstring& _Str)
{
	std::wstring _DStr{ _Str };

	size_t nPos = _DStr.find_first_not_of(L" \t\r");
	if (nPos > 0)
		_DStr = _DStr.substr(nPos);

	return _DStr;
}

std::wstring wstringutil::TrimRight(const std::wstring& _Str)
{
	std::wstring _DStr{ _Str };

	size_t nPos = _DStr.find_last_not_of(L" \t\r");

	if (nPos > 0)
		_DStr = _DStr.substr(0, nPos + 1);
	return _DStr;
}

std::wstring wstringutil::Reserve(const std::wstring& _Str)
{
	std::wstring _DStr = { };

	for (int i = _Str.size() - 1; i >= 0; --i) {
		_DStr += _Str[i];
	}

	return _DStr;
}

std::wstring wstringutil::Replace(const std::wstring& _Str, const std::wstring& _Search, const std::wstring& _Replace)
{
	std::wstring _DStr = { _Str };

	size_t pos = 0;
	while ((pos = _DStr.find(_Search, pos)) != std::string::npos)
	{
		_DStr.replace(pos, _Search.length(), _Replace);

		pos += _Replace.length();
	}
	return _DStr;
}

std::wstring wstringutil::GetFilePath(const std::wstring& _Str)
{
	size_t nPos = _Str.find_last_of(L"\\");
	if (nPos > 0)
		return Left(_Str, nPos);
	return _Str;

	//size_t nPos = _Str.rfind(L"\\");
	//if (nPos > 0)
	//	return Left(_Str, nPos);
	//
	//return _Str;
}

std::wstring wstringutil::GetFileName(const std::wstring& _Str)
{
	size_t nPos = _Str.find_last_of(L"\\");
	if (nPos > 0)
		return _Str.substr(nPos + 1);

	return _Str;

	//size_t nPos = _Str.rfind(L"\\");
	//if (nPos > 0)
	//	return _Str.substr(nPos + 1);
	//
	//return _Str;
}

std::wstring wstringutil::RemoveExt(const std::wstring& _Str)
{
	return _Str.substr(0, _Str.find_last_of(L"."));
}

std::wstring wstringutil::GetFileExt(const std::wstring& _Str)
{
	size_t nPos = _Str.find_last_of(L'.');
	if (nPos > 0)
		return _Str.substr(nPos + 1);
	return _Str;

	//size_t nPos = _Str.rfind(L'.');
	//if (nPos > 0)
	//	return _Str.substr(nPos + 1);
	//
	//return _Str;
}

std::wstring wstringutil::GetFindStr(const std::wstring& _Str, const std::wstring& _left, const std::wstring& _right)
{
	size_t nleft = _Str.find(_left);
	size_t nright = _Str.find(_right);

	if (nleft > 0 && nright > 0)
		return Mid(_Str, nleft, (nright - nleft));//return Mid(_Str, nleft + 1, (nright - nleft - 1));

	return _Str;
}
*/