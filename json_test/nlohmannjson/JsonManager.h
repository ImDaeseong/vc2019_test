#pragma once
#include <afx.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <type_traits>

class CJsonManager
{
public:
    CJsonManager();
    ~CJsonManager();
    bool LoadFromString(const CString& jsonString);
    bool RemoveKey(const CString& key);

    CString GetValueAsCString(const CString& key) const;
    int GetValueAsInt(const CString& key) const;
    double GetValueAsDouble(const CString& key) const;
    bool GetValueAsBool(const CString& key) const;

    template<typename T>
    bool SetValue(const CString& key, const T& value)
    {
        if constexpr (std::is_convertible_v<T, CString>)
        {
            m_json[CStringToUtf8(key)] = CStringToUtf8(static_cast<CString>(value));
        }
        else
        {
            m_json[CStringToUtf8(key)] = value;
        }
        return true;
    }

    bool SetJsonArray(const CString& key, const std::vector<nlohmann::json>& array);
    std::vector<nlohmann::json> GetJsonArray(const CString& key) const;
    static nlohmann::json CreateJsonObject();

    template<typename T>
    static void AddToJsonObject(nlohmann::json& obj, const CString& key, const T& value)
    {
        if constexpr (std::is_same_v<T, CString>)
        {
            obj[CStringToUtf8(key)] = CStringToUtf8(value);
        }
        else if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, bool>)
        {
            obj[CStringToUtf8(key)] = value;
        }
        else
        {
            static_assert(always_false<T>, "Unsupported type for AddToJsonObject");
        }
    }

    static CString ConvertUtf8ToCString(const std::string& str);
    CString GetJsonString() const;

private:
    nlohmann::json m_json;

    static std::string CStringToUtf8(const CString& str);
    static CString Utf8ToCString(const std::string& str);
    static nlohmann::json ConvertToUtf8Json(const nlohmann::json& input);
    static nlohmann::json ConvertFromUtf8Json(const nlohmann::json& input);
};