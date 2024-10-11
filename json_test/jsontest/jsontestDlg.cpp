#include "pch.h"
#include "framework.h"
#include "jsontest.h"
#include "jsontestDlg.h"
#include "afxdialogex.h"
#include "JsonManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CjsontestDlg::CjsontestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JSONTEST_DIALOG, pParent)
{
}

void CjsontestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CjsontestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void addGameToList(std::vector<Json::Value>& gameList, int no, bool bFree, const CString& gameName, const CString& exeName) {
    
    auto gameObject = CJsonManager::CreateJsonObject();

    CJsonManager::AddToJsonObject(gameObject, _T("NO"), no);
    CJsonManager::AddToJsonObject(gameObject, _T("FREE"), bFree);
    CJsonManager::AddToJsonObject(gameObject, _T("GAME"), gameName);
    CJsonManager::AddToJsonObject(gameObject, _T("EXE"), exeName);

    gameList.push_back(gameObject);
}

void createJson()
{
    CJsonManager man;

    // 기본정보 설정
    man.SetValueAsCString(_T("GameType"), _T("RPG"));
    man.SetValueAsCString(_T("GameCompany"), _T("Blizzard"));
    man.SetValue(_T("GameValue"), 54000);
    man.SetValue(_T("GameFree"), false);

    // 게임 리스트 설정
    std::vector<Json::Value> newGameList;
    addGameToList(newGameList, 1, true, _T("디아블로1"), _T("Diablo I.exe"));
    addGameToList(newGameList, 2, false, _T("디아블로2"), _T("Diablo II.exe"));
    addGameToList(newGameList, 3, true, _T("디아블로3"), _T("Diablo III.exe"));
    addGameToList(newGameList, 4, false, _T("디아블로4"), _T("Diablo IV.exe"));
    man.SetJsonArray(_T("NewGameList"), newGameList);

    // 기본정보 가져오기
    CString sNewGameType = man.GetValueAsCString(_T("GameType"));
    CString sNewGameCompany = man.GetValueAsCString(_T("GameCompany"));
    int nGameValue = man.GetValueAsInt(_T("GameValue"));
    bool bGameFree = man.GetValueAsBool(_T("GameFree"));

    // 게임 리스트 가져오기
    std::vector<Json::Value> retrievedGameList = man.GetJsonArray(_T("NewGameList"));

    // 1번째 데이터 가져오기
    if (!retrievedGameList.empty()) {
        const Json::Value& firstGame = retrievedGameList[0];
        int nNo = firstGame["NO"].asInt();
        bool bFree = firstGame["FREE"].asBool();
        CString sGAME = CJsonManager::ConvertUtf8ToCString(firstGame["GAME"].asString());
        CString sEXE = CJsonManager::ConvertUtf8ToCString(firstGame["EXE"].asString());
    }

    // 전체 리스트 출력
    for (const auto& game : retrievedGameList) {
        int nNo = game["NO"].asInt();
        bool bFree = game["FREE"].asBool();
        CString sGAME = CJsonManager::ConvertUtf8ToCString(game["GAME"].asString());
        CString sEXE = CJsonManager::ConvertUtf8ToCString(game["EXE"].asString());
    }

    CString sResult = man.GetJsonString();
    AfxMessageBox(sResult);
}

BOOL CjsontestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    createJson();

	return TRUE;  
}

void CjsontestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CjsontestDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

