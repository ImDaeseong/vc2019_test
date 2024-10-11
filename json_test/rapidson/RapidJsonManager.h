#pragma once

#include <afx.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <vector>
#include <string>

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

    bool SetValueAsCString(const CString& key, const CString& value);
    bool SetValue(const CString& key, int value);
    bool SetValue(const CString& key, double value);
    bool SetValue(const CString& key, bool value);

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