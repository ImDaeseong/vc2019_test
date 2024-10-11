#include "pch.h"
#include "framework.h"
#include "rapidson.h"
#include "rapidsonDlg.h"
#include "afxdialogex.h"
#include "RapidJsonManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CrapidsonDlg::CrapidsonDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RAPIDSON_DIALOG, pParent)
{
}

void CrapidsonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CrapidsonDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void addGameToList(std::vector<rapidjson::Value*>& gameList, CRapidJsonManager& man, int no, bool bFree, const CString& gameName, const CString& exeName) {
    
    rapidjson::Document::AllocatorType& allocator = man.GetAllocator();
    rapidjson::Value* gameObject = man.CreateValue();
    gameObject->SetObject();

    gameObject->AddMember("NO", no, allocator);
    gameObject->AddMember("FREE", bFree, allocator);
    gameObject->AddMember("GAME", rapidjson::Value(CRapidJsonManager::CStringToUtf8(gameName).c_str(), allocator).Move(), allocator);
    gameObject->AddMember("EXE", rapidjson::Value(CRapidJsonManager::CStringToUtf8(exeName).c_str(), allocator).Move(), allocator);

    gameList.push_back(gameObject);
}

void createJson()
{
    CRapidJsonManager man;

    // 기본 정보
    man.SetValueAsCString(_T("GameType"), _T("RPG"));
    man.SetValueAsCString(_T("GameCompany"), _T("Blizzard"));
    man.SetValue(_T("GameValue"), 54000);
    man.SetValue(_T("GameFree"), false);

    // 게임 리스트 설정
    std::vector<rapidjson::Value*> newGameList;
    addGameToList(newGameList, man, 1, true, _T("디아블로1"), _T("Diablo I.exe"));
    addGameToList(newGameList, man, 2, false, _T("디아블로2"), _T("Diablo II.exe"));
    addGameToList(newGameList, man, 3, true, _T("디아블로3"), _T("Diablo III.exe"));
    addGameToList(newGameList, man, 4, false, _T("디아블로4"), _T("Diablo IV.exe"));
    man.SetJsonArray(_T("NewGameList"), newGameList);

    // 기본 정보 가져오기
    CString sNewGameType = man.GetValueAsCString(_T("GameType"));
    CString sNewGameCompany = man.GetValueAsCString(_T("GameCompany"));
    int nGameValue = man.GetValueAsInt(_T("GameValue"));
    bool bGameFree = man.GetValueAsBool(_T("GameFree"));

    // 게임 리스트 가져오기
    std::vector<const rapidjson::Value*> retrievedGameList = man.GetJsonArray(_T("NewGameList"));

    // 첫 번째 데이터
    if (!retrievedGameList.empty()) {
        const rapidjson::Value* firstGame = retrievedGameList[0];
        int nNo = (*firstGame)["NO"].GetInt();
        bool bFree = (*firstGame)["FREE"].GetBool();
        CString sGAME = CRapidJsonManager::Utf8ToCString((*firstGame)["GAME"].GetString());
        CString sEXE = CRapidJsonManager::Utf8ToCString((*firstGame)["EXE"].GetString());
    }

    // 전체 리스트
    for (const auto& game : retrievedGameList) {
        int nNo = (*game)["NO"].GetInt();
        bool bFree = (*game)["FREE"].GetBool();
        CString sGAME = CRapidJsonManager::Utf8ToCString((*game)["GAME"].GetString());
        CString sEXE = CRapidJsonManager::Utf8ToCString((*game)["EXE"].GetString());
    }

    CString sResult = man.GetJsonString();
    AfxMessageBox(sResult);
}

BOOL CrapidsonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
		
    createJson();

	return TRUE;  
}

void CrapidsonDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CrapidsonDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

