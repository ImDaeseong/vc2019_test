#include "pch.h"
#include "JsonManager.h"

CJsonManager::CJsonManager()
{
    m_json = nlohmann::json::object();
}

CJsonManager::~CJsonManager()
{
    m_json.clear();
}

bool CJsonManager::LoadFromString(const CString& jsonString)
{
    try {
        m_json = nlohmann::json::parse(CStringToUtf8(jsonString));
        return true;
    }
    catch (const std::exception& e) {
        CString sErr = _T("LoadFromString error: ") + Utf8ToCString(e.what());
        return false;
    }
}

bool CJsonManager::RemoveKey(const CString& key)
{
    return m_json.erase(CStringToUtf8(key)) > 0;
}

CString CJsonManager::GetValueAsCString(const CString& key) const
{
    auto it = m_json.find(CStringToUtf8(key));
    if (it != m_json.end() && it->is_string()) {
        return Utf8ToCString(it->get<std::string>());
    }
    return CString();
}

int CJsonManager::GetValueAsInt(const CString& key) const
{
    auto it = m_json.find(CStringToUtf8(key));
    if (it != m_json.end() && it->is_number_integer()) {
        return it->get<int>();
    }
    return 0;
}

double CJsonManager::GetValueAsDouble(const CString& key) const
{
    auto it = m_json.find(CStringToUtf8(key));
    if (it != m_json.end() && it->is_number()) {
        return it->get<double>();
    }
    return 0.0;
}

bool CJsonManager::GetValueAsBool(const CString& key) const
{
    auto it = m_json.find(CStringToUtf8(key));
    if (it != m_json.end() && it->is_boolean()) {
        return it->get<bool>();
    }
    return false;
}

bool CJsonManager::SetValueAsCString(const CString& key, const CString& value)
{
    m_json[CStringToUtf8(key)] = CStringToUtf8(value);
    return true;
}

bool CJsonManager::SetValue(const CString& key, int value)
{
    m_json[CStringToUtf8(key)] = value;
    return true;
}

bool CJsonManager::SetValue(const CString& key, double value)
{
    m_json[CStringToUtf8(key)] = value;
    return true;
}

bool CJsonManager::SetValue(const CString& key, bool value)
{
    m_json[CStringToUtf8(key)] = value;
    return true;
}

bool CJsonManager::SetJsonArray(const CString& key, const std::vector<nlohmann::json>& array)
{
    try {
        nlohmann::json utf8Array;
        for (const auto& item : array) {
            utf8Array.push_back(item);
        }
        m_json[CStringToUtf8(key)] = utf8Array;
        return true;
    }
    catch (const std::exception& e) {
        CString sErr = _T("SetJsonArray error: ") + Utf8ToCString(e.what());
        return false;
    }
}

std::vector<nlohmann::json> CJsonManager::GetJsonArray(const CString& key) const
{
    std::vector<nlohmann::json> result;
    auto it = m_json.find(CStringToUtf8(key));
    if (it != m_json.end() && it->is_array()) {
        for (const auto& item : *it) {
            result.push_back(item);
        }
    }
    return result;
}

CString CJsonManager::GetJsonString() const
{
    try 
    {
        return Utf8ToCString(m_json.dump(4));
    }
    catch (const std::exception& e)
    {
        CString sErr = _T("GetJsonString error: ") + Utf8ToCString(e.what());
        return CString();
    }
}

nlohmann::json CJsonManager::CreateJsonObject()
{
    return nlohmann::json::object();
}

void CJsonManager::AddToJsonObject(nlohmann::json& obj, const CString& key, const CString& value)
{
    obj[CStringToUtf8(key)] = CStringToUtf8(value);
}

void CJsonManager::AddToJsonObject(nlohmann::json& obj, const CString& key, int value)
{
    obj[CStringToUtf8(key)] = value;
}

void CJsonManager::AddToJsonObject(nlohmann::json& obj, const CString& key, double value)
{
    obj[CStringToUtf8(key)] = value;
}

void CJsonManager::AddToJsonObject(nlohmann::json& obj, const CString& key, bool value)
{
    obj[CStringToUtf8(key)] = value;
}

std::string CJsonManager::CStringToUtf8(const CString& str)
{
    if (str.IsEmpty()) return std::string();

    int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
    std::string utf8(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, str, -1, &utf8[0], len, nullptr, nullptr);
    return utf8;
}

CString CJsonManager::Utf8ToCString(const std::string& str)
{
    if (str.empty()) return CString();

    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    CString wstr;
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr.GetBuffer(len), len);
    wstr.ReleaseBuffer();
    return wstr;
}

nlohmann::json CJsonManager::ConvertToUtf8Json(const nlohmann::json& input)
{
    nlohmann::json result;
    for (auto it = input.begin(); it != input.end(); ++it) {
        if (it.value().is_string()) {
            result[it.key()] = CStringToUtf8(Utf8ToCString(it.value().get<std::string>()));
        }
        else if (it.value().is_object() || it.value().is_array()) {
            result[it.key()] = ConvertToUtf8Json(it.value());
        }
        else {
            result[it.key()] = it.value();
        }
    }
    return result;
}

nlohmann::json CJsonManager::ConvertFromUtf8Json(const nlohmann::json& input)
{
    nlohmann::json result;
    for (auto it = input.begin(); it != input.end(); ++it) {
        if (it.value().is_string()) {
            result[it.key()] = CStringToUtf8(Utf8ToCString(it.value().get<std::string>()));
        }
        else if (it.value().is_object() || it.value().is_array()) {
            result[it.key()] = ConvertFromUtf8Json(it.value());
        }
        else {
            result[it.key()] = it.value();
        }
    }
    return result;
}

CString CJsonManager::ConvertUtf8ToCString(const std::string& str)
{
    return Utf8ToCString(str);
}