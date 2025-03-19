#include "pch.h"
#include "tstringutil.h"

tstring tstringutil::Mid(const tstring& str, size_t off, size_t count)
{
    return str.substr(off, count);
}

tstring tstringutil::Left(const tstring& str, size_t count)
{
    return str.substr(0, count);
}

tstring tstringutil::Right(const tstring& str, size_t count)
{
    if (count > str.size()) {
        count = str.size(); // ���ڿ� ���̺��� ū ���� ���� ��� ó��
    }
    return str.substr(str.size() - count, count);
}

tstring tstringutil::MakeLower(const tstring& str)
{
    tstring result;
    result.reserve(str.size()); // �̸� �޸� �Ҵ����� ���� ���
    std::transform(str.begin(), str.end(), std::back_inserter(result), ::_totlower);
    return result;
}

tstring tstringutil::MakeUpper(const tstring& str)
{
    tstring result;
    result.reserve(str.size()); // �̸� �޸� �Ҵ����� ���� ���
    std::transform(str.begin(), str.end(), std::back_inserter(result), ::_totupper);
    return result;
}

size_t tstringutil::Find(const tstring& str, const tstring& strsub, size_t off)
{
    return str.find(strsub, off);
}

size_t tstringutil::Find(const tstring& str, TCHAR ch, size_t off)
{
    return str.find(ch, off);
}

size_t tstringutil::ReserveFind(const tstring& str, const tstring& strsub)
{
    return str.rfind(strsub);
}

size_t tstringutil::ReserveFind(const tstring& str, TCHAR ch)
{
    return str.rfind(ch);
}

size_t tstringutil::Split(const tstring& str, const tstring& delimiter, std::vector<tstring>& result)
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
    tstring temp = str; // ���� ���ڿ� ����

    while ((found = temp.find(delimiter, pos)) != tstring::npos) {
        result.push_back(temp.substr(pos, found - pos));
        pos = found + delimiter.length();
    }

    // ������ ���� �߰�
    if (pos <= temp.length()) {
        result.push_back(temp.substr(pos));
    }

    return result.size();
}

tstring tstringutil::Trim(const tstring& str)
{
    return TrimRight(TrimLeft(str));
}

tstring tstringutil::TrimLeft(const tstring& str)
{
    tstring result = str;
    size_t pos = result.find_first_not_of(_T(" \t\r\n"));

    if (pos == tstring::npos) {
        return _T(""); // ��� ������ ���
    }

    result.erase(0, pos);
    return result;
}

tstring tstringutil::TrimRight(const tstring& str)
{
    tstring result = str;
    size_t pos = result.find_last_not_of(_T(" \t\r\n"));

    if (pos == tstring::npos) {
        return _T(""); // ��� ������ ���
    }

    result.erase(pos + 1);
    return result;
}

tstring tstringutil::Reserve(const tstring& str)
{
    tstring result = str;
    std::reverse(result.begin(), result.end()); // ǥ�� �˰��� ���
    return result;
}

tstring tstringutil::Replace(const tstring& str, const tstring& search, const tstring& replace)
{
    if (search.empty()) {
        return str; // �˻� ���ڿ��� ��������� ���� ��ȯ
    }

    tstring result = str;
    size_t pos = 0;

    while ((pos = result.find(search, pos)) != tstring::npos) {
        result.replace(pos, search.length(), replace);
        pos += replace.length();
    }

    return result;
}

tstring tstringutil::GetFilePath(const tstring& str)
{
    size_t pos = str.find_last_of(_T("\\/"));
    if (pos != tstring::npos) {
        return str.substr(0, pos);
    }
    return _T("");
}

tstring tstringutil::GetFileName(const tstring& str)
{
    size_t pos = str.find_last_of(_T("\\/"));
    if (pos != tstring::npos) {
        return str.substr(pos + 1);
    }
    return str;
}

tstring tstringutil::RemoveExt(const tstring& str)
{
    size_t pos = str.find_last_of(_T("."));
    if (pos != tstring::npos) {
        return str.substr(0, pos);
    }
    return str;
}

tstring tstringutil::GetFileExt(const tstring& str)
{
    size_t pos = str.find_last_of(_T("."));
    if (pos != tstring::npos) {
        return str.substr(pos + 1);
    }
    return _T("");
}

tstring tstringutil::GetFindStr(const tstring& str, const tstring& left, const tstring& right)
{
    size_t leftPos = str.find(left);
    if (leftPos == tstring::npos) {
        return _T("");
    }

    leftPos += left.length(); // left ���ڿ� �������� ����
    size_t rightPos = str.find(right, leftPos);

    if (rightPos == tstring::npos) {
        return _T("");
    }

    return str.substr(leftPos, rightPos - leftPos);
}