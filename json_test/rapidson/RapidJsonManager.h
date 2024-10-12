#pragma once
#include <afx.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <vector>
#include <string>
#include <type_traits>

class CRapidJsonManager
{
public:
    CRapidJsonManager();
    ~CRapidJsonManager();

    bool LoadFromString(const CString& jsonString);
    bool RemoveKey(const CString& key);

    CString GetValueAsCString(const CString& key) const;
    int GetValueAsInt(const CString& key) const;
    double GetValueAsDouble(const CString& key) const;
    bool GetValueAsBool(const CString& key) const;

    template<typename T>
    bool SetValue(const CString& key, const T& value)
    {
        std::string utf8Key = CStringToUtf8(key);

        if constexpr (std::is_convertible_v<T, CString>)
        {
            CString strValue = value;  
            std::string utf8Value = CStringToUtf8(strValue);
            m_document.RemoveMember(utf8Key.c_str());
            m_document.AddMember(rapidjson::Value(utf8Key.c_str(), m_document.GetAllocator()).Move(),rapidjson::Value(utf8Value.c_str(), m_document.GetAllocator()).Move(),m_document.GetAllocator() );
        }
        else if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, bool>)
        {
            m_document.RemoveMember(utf8Key.c_str());
            m_document.AddMember(rapidjson::Value(utf8Key.c_str(), m_document.GetAllocator()).Move(), rapidjson::Value(value), m_document.GetAllocator() );
        }
        else
        {
            return false;
        }

        return true;
    }

    bool SetJsonArray(const CString& key, const std::vector<rapidjson::Value*>& array);
    std::vector<const rapidjson::Value*> GetJsonArray(const CString& key) const;

    CString GetJsonString() const;

    rapidjson::Document::AllocatorType& GetAllocator() { return m_document.GetAllocator(); }
    rapidjson::Value* CreateValue();

    static std::string CStringToUtf8(const CString& str);
    static CString Utf8ToCString(const std::string& str);

private:
    rapidjson::Document m_document;
    std::vector<std::unique_ptr<rapidjson::Value>> m_allocatedValues;
};