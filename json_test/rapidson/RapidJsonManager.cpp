#include "pch.h"
#include "RapidJsonManager.h"

CRapidJsonManager::CRapidJsonManager()
{
    m_document.SetObject();
}

CRapidJsonManager::~CRapidJsonManager()
{
}

bool CRapidJsonManager::LoadFromString(const CString& jsonString)
{
    std::string utf8String = CStringToUtf8(jsonString);
    return !m_document.Parse(utf8String.c_str()).HasParseError();
}

bool CRapidJsonManager::RemoveKey(const CString& key)
{
    std::string utf8Key = CStringToUtf8(key);
    return m_document.RemoveMember(utf8Key.c_str());
}

CString CRapidJsonManager::GetValueAsCString(const CString& key) const
{
    std::string utf8Key = CStringToUtf8(key);
    if (m_document.HasMember(utf8Key.c_str()) && m_document[utf8Key.c_str()].IsString()) {
        return Utf8ToCString(m_document[utf8Key.c_str()].GetString());
    }
    return CString();
}

int CRapidJsonManager::GetValueAsInt(const CString& key) const
{
    std::string utf8Key = CStringToUtf8(key);
    if (m_document.HasMember(utf8Key.c_str()) && m_document[utf8Key.c_str()].IsInt()) {
        return m_document[utf8Key.c_str()].GetInt();
    }
    return 0;
}

double CRapidJsonManager::GetValueAsDouble(const CString& key) const
{
    std::string utf8Key = CStringToUtf8(key);
    if (m_document.HasMember(utf8Key.c_str()) && m_document[utf8Key.c_str()].IsNumber()) {
        return m_document[utf8Key.c_str()].GetDouble();
    }
    return 0.0;
}

bool CRapidJsonManager::GetValueAsBool(const CString& key) const
{
    std::string utf8Key = CStringToUtf8(key);
    if (m_document.HasMember(utf8Key.c_str()) && m_document[utf8Key.c_str()].IsBool()) {
        return m_document[utf8Key.c_str()].GetBool();
    }
    return false;
}

bool CRapidJsonManager::SetValueAsCString(const CString& key, const CString& value)
{
    std::string utf8Key = CStringToUtf8(key);
    std::string utf8Value = CStringToUtf8(value);
    m_document.RemoveMember(utf8Key.c_str());
    m_document.AddMember(rapidjson::Value(utf8Key.c_str(), m_document.GetAllocator()).Move(), rapidjson::Value(utf8Value.c_str(), m_document.GetAllocator()).Move(), m_document.GetAllocator());
    return true;
}

bool CRapidJsonManager::SetValue(const CString& key, int value)
{
    std::string utf8Key = CStringToUtf8(key);
    m_document.RemoveMember(utf8Key.c_str());
    m_document.AddMember(rapidjson::Value(utf8Key.c_str(), m_document.GetAllocator()).Move(), value, m_document.GetAllocator());
    return true;
}

bool CRapidJsonManager::SetValue(const CString& key, double value)
{
    std::string utf8Key = CStringToUtf8(key);
    m_document.RemoveMember(utf8Key.c_str());
    m_document.AddMember(rapidjson::Value(utf8Key.c_str(), m_document.GetAllocator()).Move(), value, m_document.GetAllocator());
    return true;
}

bool CRapidJsonManager::SetValue(const CString& key, bool value)
{
    std::string utf8Key = CStringToUtf8(key);
    m_document.RemoveMember(utf8Key.c_str());
    m_document.AddMember(rapidjson::Value(utf8Key.c_str(), m_document.GetAllocator()).Move(), value, m_document.GetAllocator());
    return true;
}

bool CRapidJsonManager::SetJsonArray(const CString& key, const std::vector<rapidjson::Value*>& array)
{
    std::string utf8Key = CStringToUtf8(key);
    rapidjson::Value jsonArray(rapidjson::kArrayType);

    for (const auto& item : array) {
        jsonArray.PushBack(*item, m_document.GetAllocator());
    }
    
    if (m_document.HasMember(utf8Key.c_str())) {
        m_document[utf8Key.c_str()] = jsonArray;
    }
    else 
    {
        m_document.AddMember(rapidjson::Value(utf8Key.c_str(), m_document.GetAllocator()).Move(), jsonArray, m_document.GetAllocator());
    }
    return true;
}

std::vector<const rapidjson::Value*> CRapidJsonManager::GetJsonArray(const CString& key) const
{
    std::vector<const rapidjson::Value*> result;
    std::string utf8Key = CStringToUtf8(key);
    if (m_document.HasMember(utf8Key.c_str()) && m_document[utf8Key.c_str()].IsArray()) {
        const rapidjson::Value& jsonArray = m_document[utf8Key.c_str()];
        for (rapidjson::SizeType i = 0; i < jsonArray.Size(); i++) {
            result.push_back(&jsonArray[i]);
        }
    }
    return result;
}

CString CRapidJsonManager::GetJsonString() const
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    m_document.Accept(writer);
    return Utf8ToCString(buffer.GetString());
}

rapidjson::Value* CRapidJsonManager::CreateValue()
{
    m_allocatedValues.push_back(std::make_unique<rapidjson::Value>());
    return m_allocatedValues.back().get();
}

std::string CRapidJsonManager::CStringToUtf8(const CString& str)
{
    if (str.IsEmpty()) return std::string();

    int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
    std::string utf8(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, str, -1, &utf8[0], len, nullptr, nullptr);
    return utf8;
}

CString CRapidJsonManager::Utf8ToCString(const std::string& str)
{
    if (str.empty()) return CString();

    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    CString wstr;
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr.GetBuffer(len), len);
    wstr.ReleaseBuffer();
    return wstr;
}