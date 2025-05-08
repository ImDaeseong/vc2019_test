#include "../pch.h"
#include "LoLManager.h"

CLoLManager::CLoLManager()
{
}

CLoLManager::~CLoLManager()
{
}

//CString → std::string (UTF-8)
std::string  CLoLManager::CStringToUtf8(const CString& strInput)
{
    int nLen = WideCharToMultiByte(CP_UTF8, 0, strInput, -1, NULL, 0, NULL, NULL);
    if (nLen <= 1) return "";

    std::string result(nLen - 1, 0); // 널문자 제외
    WideCharToMultiByte(CP_UTF8, 0, strInput, -1, &result[0], nLen, NULL, NULL);
    return result;
}

//std::string(UTF - 8) → CString
CString CLoLManager::Utf8ToCString(const std::string& strInput)
{
    int nLen = MultiByteToWideChar(CP_UTF8, 0, strInput.c_str(), -1, NULL, 0);
    if (nLen <= 1) return _T("");

    CString strResult;
    MultiByteToWideChar(CP_UTF8, 0, strInput.c_str(), -1, strResult.GetBuffer(nLen), nLen);
    strResult.ReleaseBuffer();
    return strResult;
}

std::string CLoLManager::Base64Encode(const std::string& strInput)
{
    static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string output;
    int val = 0, valb = -6;

    for (size_t i = 0; i < strInput.size(); ++i)
    {
        unsigned char c = strInput[i];
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            output.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if (valb > -6)
        output.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);

    while (output.size() % 4)
        output.push_back('=');

    return output;
}

char* CLoLManager::Utf8ToChar(const char* chInput, size_t length)
{
    int nWideCharSize = MultiByteToWideChar(CP_UTF8, 0, chInput, static_cast<int>(length), NULL, 0);
    if (nWideCharSize == 0)
        return NULL;

    WCHAR* wideCharStr = new WCHAR[nWideCharSize + 1];
    ZeroMemory(wideCharStr, sizeof(WCHAR) * (nWideCharSize + 1));
    MultiByteToWideChar(CP_UTF8, 0, chInput, static_cast<int>(length), wideCharStr, nWideCharSize);

    int nCharSize = WideCharToMultiByte(CP_ACP, 0, wideCharStr, nWideCharSize, NULL, 0, NULL, NULL);
    if (nCharSize == 0)
    {
        delete[] wideCharStr;
        return NULL;
    }

    char* charStr = new char[nCharSize + 1];
    ZeroMemory(charStr, nCharSize + 1);
    WideCharToMultiByte(CP_ACP, 0, wideCharStr, nWideCharSize, charStr, nCharSize, NULL, NULL);

    delete[] wideCharStr;
    return charStr;
}

CString CLoLManager::GetJson(const CString& strUrl, const CString& strHeaders, unsigned short uPort)
{
    CString strServerDomainNoHttp;
    CString strServerDomain;
    CString strPageName;

    int nHttpIndex = -1;
    int nPlusIndex = 7;

    int nFind = strUrl.Find('/', 10);
    if (nFind < 0)
        return _T("");

    strServerDomain = strUrl.Left(nFind);
    strPageName = strUrl.Mid(nFind + 1);

    nHttpIndex = strServerDomain.Find(_T("http://"));
    if (nHttpIndex < 0)
    {
        nHttpIndex = strServerDomain.Find(_T("https://"));
        nPlusIndex++;
    }

    if (nHttpIndex != -1)
        strServerDomainNoHttp = strServerDomain.Mid(nHttpIndex + nPlusIndex);
    else
        strServerDomainNoHttp = strServerDomain;

    HINTERNET hInternet = InternetOpen(_T("HTTP"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet == NULL)
        return _T("");

    DWORD dwTimeout = 5000; // 5 seconds
    InternetSetOption(hInternet, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeout, sizeof(dwTimeout));
    InternetSetOption(hInternet, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwTimeout, sizeof(dwTimeout));
    InternetSetOption(hInternet, INTERNET_OPTION_SEND_TIMEOUT, &dwTimeout, sizeof(dwTimeout));

    HINTERNET hConnect = InternetConnect(hInternet, strServerDomainNoHttp, uPort, _T(""), _T(""), INTERNET_SERVICE_HTTP, 0, 0);
    if (hConnect == NULL)
    {
        InternetCloseHandle(hInternet);
        return _T("");
    }

    HINTERNET hRequest = HttpOpenRequest(hConnect, _T("GET"), strPageName, HTTP_VERSION, _T(""), NULL,
        INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
    if (hRequest == NULL)
    {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return _T("");
    }

    DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
        SECURITY_FLAG_IGNORE_REVOCATION |
        SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP |
        SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS |
        SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
        SECURITY_FLAG_IGNORE_CERT_CN_INVALID;

    InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

    HttpAddRequestHeaders(hRequest, strHeaders, strHeaders.GetLength(), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);

    BOOL bSend = HttpSendRequest(hRequest, NULL, 0, NULL, 0);
    if (!bSend)
    {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return _T("");
    }

    CString strReturn = _T("");
    char szBuff[10000] = { 0 };
    DWORD dwReadSize = 0;

    while (InternetReadFile(hRequest, szBuff, sizeof(szBuff) - 1, &dwReadSize) && dwReadSize > 0)
    {
        szBuff[dwReadSize] = '\0';
        char* pConverted = Utf8ToChar(szBuff, dwReadSize);
        if (pConverted)
        {
            strReturn += pConverted;
            delete[] pConverted;
        }
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return strReturn;
}

CString CLoLManager::GetLockFilePath()
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return _T("");

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe))
    {
        do
        {
            //OutputDebugString(pe.szExeFile);

            if (_tcsicmp(pe.szExeFile, _T("LeagueClientUx.exe")) == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
                if (hProcess != NULL)
                {
                    TCHAR tPath[MAX_PATH] = { 0 };
                    if (GetModuleFileNameEx(hProcess, NULL, tPath, MAX_PATH))
                    {
                        PathRemoveFileSpec(tPath);

                        CString strPath(tPath);
                        if (strPath.Right(1) != _T("\\"))
                            strPath += _T("\\");
                        strPath += _T("lockfile");

                        CloseHandle(hProcess);
                        CloseHandle(hSnapshot);
                        return CString(strPath);
                    }
                    CloseHandle(hProcess);
                }
            }
        } while (Process32Next(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);

    return _T("");
}

LockFileInfo CLoLManager::ParseLockFile(const CString& strPath)
{
    LockFileInfo lockFileInfo;

    CFile file;
    if (!file.Open(strPath, CFile::modeRead | CFile::shareDenyNone))
        return LockFileInfo();

    ULONGLONG length = file.GetLength();

    char* buffer = new char[(size_t)length + 1];
    memset(buffer, 0, (size_t)length + 1);

    file.Read(buffer, (UINT)length);
    buffer[length] = '\0';

    CString data(buffer);
    delete[] buffer;

    int pos = 0;
    lockFileInfo.strProcessName = data.Tokenize(_T(":"), pos);
    lockFileInfo.nPid = _ttoi(data.Tokenize(_T(":"), pos));
    lockFileInfo.nPort = _ttoi(data.Tokenize(_T(":"), pos));
    lockFileInfo.strPassword = data.Tokenize(_T(":"), pos);
    lockFileInfo.strProtocol = data.Tokenize(_T(":"), pos);

    file.Close();

    return lockFileInfo;
}

CString CLoLManager::MakeAuthHeader(const CString& strPassword)
{
    CString strHeader;
    strHeader.Format(_T("riot:%s"), strPassword);

    //UTF-8 문자열로 변환 후 Base64 인코딩
    std::string utf8Str = CStringToUtf8(strHeader);
    std::string encoded = Base64Encode(utf8Str);

    //다시 CString으로 변환하여 헤더 구성
    CString unicodeEncoded = Utf8ToCString(encoded);

    CString strResult;
    strResult.Format(_T("Authorization: Basic %s\r\n"), (LPCTSTR)unicodeEncoded);
    return strResult;
}

LoLDataInfo CLoLManager::GetGameDataInfo()
{
    LoLDataInfo item;

    CString strPath = GetLockFilePath();
    if (strPath.IsEmpty())
    {
        return item;
    }

    LockFileInfo lockFileInfo = ParseLockFile(strPath);
    if (lockFileInfo.strProtocol.IsEmpty() || lockFileInfo.strPassword.IsEmpty())
    {
        return item;
    }

    CString strHeaders = MakeAuthHeader(lockFileInfo.strPassword);

    CString strUrl;
    strUrl.Format(_T("%s://127.0.0.1/lol-chat/v1/me"), lockFileInfo.strProtocol.GetString());

    CString strJson = GetJson(strUrl, strHeaders, lockFileInfo.nPort);
    OutputDebugString(strJson);
   
    Json::Reader reader;
    Json::Value root;
    bool parsingRet = reader.parse(CStringToUtf8(strJson), root);
    if (!parsingRet)
    {
        return item;
    }
    
    if (root.isObject())
    {
        try
        {
            CString availability = Utf8ToCString(root["availability"].asString());
            CString gameName = Utf8ToCString(root["gameName"].asString());
            CString gameTag = Utf8ToCString(root["gameTag"].asString());
            CString id = Utf8ToCString(root["id"].asString());
            CString name = Utf8ToCString(root["name"].asString());
            CString patchline = Utf8ToCString(root["patchline"].asString());
            CString pid = Utf8ToCString(root["pid"].asString());
            CString platformId = Utf8ToCString(root["platformId"].asString());
            CString product = Utf8ToCString(root["product"].asString());
            CString productName = Utf8ToCString(root["productName"].asString());
            CString puuid = Utf8ToCString(root["puuid"].asString());
            CString statusMessage = Utf8ToCString(root["statusMessage"].asString());
            CString summary = Utf8ToCString(root["summary"].asString());
            CString summonerId = Utf8ToCString(root["summonerId"].asString());

            item.strGameName = gameName;
            item.strGameTag = gameTag;

            Json::Value lolInfo = root["lol"];
            if (lolInfo.isObject())
            {
                CString championId = Utf8ToCString(lolInfo["championId"].asString());
                CString companionId = Utf8ToCString(lolInfo["companionId"].asString());
                CString damageSkinId = Utf8ToCString(lolInfo["damageSkinId"].asString());
                CString gameId = Utf8ToCString(lolInfo["gameId"].asString());
                CString gameMode = Utf8ToCString(lolInfo["gameMode"].asString());
                CString gameQueueType = Utf8ToCString(lolInfo["gameQueueType"].asString());
                CString gameStatus = Utf8ToCString(lolInfo["gameStatus"].asString());
                CString iconOverride = Utf8ToCString(lolInfo["iconOverride"].asString());
                CString isObservable = Utf8ToCString(lolInfo["isObservable"].asString());
                CString legendaryMasteryScore = Utf8ToCString(lolInfo["legendaryMasteryScore"].asString());
                CString level = Utf8ToCString(lolInfo["level"].asString());
                CString mapId = Utf8ToCString(lolInfo["mapId"].asString());
                CString mapSkinId = Utf8ToCString(lolInfo["mapSkinId"].asString());
                CString puuid = Utf8ToCString(lolInfo["puuid"].asString());
                CString queueId = Utf8ToCString(lolInfo["queueId"].asString());
                CString rankedLeagueDivision = Utf8ToCString(lolInfo["rankedLeagueDivision"].asString());
                CString rankedLeagueQueue = Utf8ToCString(lolInfo["rankedLeagueQueue"].asString());
                CString rankedLeagueTier = Utf8ToCString(lolInfo["rankedLeagueTier"].asString());
                CString rankedLosses = Utf8ToCString(lolInfo["rankedLosses"].asString());
                CString rankedPrevSeasonDivision = Utf8ToCString(lolInfo["rankedPrevSeasonDivision"].asString());
                CString rankedPrevSeasonTier = Utf8ToCString(lolInfo["rankedPrevSeasonTier"].asString());
                CString rankedSplitRewardLevel = Utf8ToCString(lolInfo["rankedSplitRewardLevel"].asString());
                CString rankedWins = Utf8ToCString(lolInfo["rankedWins"].asString());
                CString regalia = Utf8ToCString(lolInfo["regalia"].asString());
                CString skinVariant = Utf8ToCString(lolInfo["skinVariant"].asString());
                CString skinname = Utf8ToCString(lolInfo["skinname"].asString());
                CString timeStamp = Utf8ToCString(lolInfo["timeStamp"].asString());

                item.strGameId = gameId;
            }
        }
        catch (...)
        {
        }
    }

    return item;
}

