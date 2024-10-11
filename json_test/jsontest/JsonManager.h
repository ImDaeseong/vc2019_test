#pragma once

#include <afx.h>
#include <vector>
#include <string>
#include "./json/json.h"

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

    bool SetValueAsCString(const CString& key, const CString& value);
    bool SetValue(const CString& key, int value);
    bool SetValue(const CString& key, double value);
    bool SetValue(const CString& key, bool value);

    bool SetJsonArray(const CString& key, const std::vector<Json::Value>& array);
    std::vector<Json::Value> GetJsonArray(const CString& key) const;

    static Json::Value CreateJsonObject();
    static void AddToJsonObject(Json::Value& obj, const CString& key, const CString& value);
    static void AddToJsonObject(Json::Value& obj, const CString& key, int value);
    static void AddToJsonObject(Json::Value& obj, const CString& key, double value);
    static void AddToJsonObject(Json::Value& obj, const CString& key, bool value);

    static CString ConvertUtf8ToCString(const std::string& str);

    CString GetJsonString() const;

private:
    Json::Value m_json;

    static std::string CStringToUtf8(const CString& str);
    static CString Utf8ToCString(const std::string& str);
};