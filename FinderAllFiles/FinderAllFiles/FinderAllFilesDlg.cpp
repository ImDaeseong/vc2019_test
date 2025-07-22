#include "pch.h"
#include "framework.h"
#include "FinderAllFiles.h"
#include "FinderAllFilesDlg.h"
#include "afxdialogex.h"

#include "JsonManager.h"

#include <string>
#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>

// WebView2Loader.dll을 로드하고 환경 옵션을 사용하기 위한 헤더
#include <WebView2EnvironmentOptions.h>

using namespace Microsoft::WRL;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString GetHtmlContent()
{
    CString html;
    html += "<html>\n";
    html += "<head>\n";
    html += "<meta charset=\"UTF-8\">\n";
    html += "<style>\n";
    html += "table { border-collapse: collapse; width: 100%; table-layout: fixed; }\n";
    html += "th, td { border: 1px solid #ccc; padding: 4px; text-align: left; overflow: hidden; position: relative; }\n";
    html += "th { background-color: #f2f2f2; user-select: none; }\n";
    html += "th .resizer {\n";
    html += "  position: absolute;\n";
    html += "  right: 0;\n";
    html += "  top: 0;\n";
    html += "  width: 5px;\n";
    html += "  height: 100%;\n";
    html += "  cursor: col-resize;\n";
    html += "  user-select: none;\n";
    html += "}\n";
    html += "</style>\n";
    html += "</head>\n";
    html += "<body>\n";
    html += "<h3>파일 탐색기</h3>\n";
    html += "<button onclick=\"requestFolder()\">폴더 선택</button>\n";
    html += "<button onclick=\"saveChanges()\">저장</button>\n"; 
    html += "<div id=\"selectedFolder\" style=\"margin:10px 0; font-weight:bold;\"></div>\n";
    html += "<table id=\"fileTable\">\n";
    html += "<thead>\n";
    html += "<tr>\n";
    html += "<th>파일 이름<div class=\"resizer\"></div></th>\n";
    html += "<th>파일 경로<div class=\"resizer\"></div></th>\n";
    html += "<th>파일 타입<div class=\"resizer\"></div></th>\n";
    html += "</tr>\n";
    html += "</thead>\n";
    html += "<tbody id=\"fileTableBody\">\n";
    html += "</tbody>\n";
    html += "</table>\n";
    html += "<script>\n";
    html += "function addFileRow(name, path, type) {\n";
    html += "  const tbody = document.getElementById('fileTableBody');\n";
    html += "  const row = tbody.insertRow();\n";
    html += "  const cell1 = row.insertCell(0);\n";
    html += "  const cell2 = row.insertCell(1);\n";
    html += "  const cell3 = row.insertCell(2);\n";
    html += "  cell1.textContent = name;\n";
    html += "  cell2.textContent = path;\n";
    html += "  cell3.textContent = type;\n";
    html += "  cell3.contentEditable = 'true';\n";  // 수정 가능하게 설정
    html += "}\n";
    html += "function requestFolder() {\n";
    html += "  if (window.chrome && window.chrome.webview) {\n";
    html += "    window.chrome.webview.postMessage({ action: 'selectFolder' });\n";
    html += "  }\n";
    html += "}\n";
    html += "function showSelectedFolder(path) {\n";
    html += "  document.getElementById('selectedFolder').textContent = '선택된 폴더: ' + path;\n";
    html += "  document.getElementById('fileTableBody').innerHTML = '';\n";
    html += "}\n";

    // 저장 버튼 클릭 시 테이블 내용 읽어 C++로 전달
    html += "function saveChanges() {\n";
    html += "  const tbody = document.getElementById('fileTableBody');\n";
    html += "  const rows = tbody.rows;\n";
    html += "  let data = [];\n";
    html += "  for (let i = 0; i < rows.length; i++) {\n";
    html += "    const name = rows[i].cells[0].textContent;\n";
    html += "    const path = rows[i].cells[1].textContent;\n";
    html += "    const type = rows[i].cells[2].textContent;\n";
    html += "    data.push({ name: name, path: path, type: type });\n";
    html += "  }\n";
    html += "  if (window.chrome && window.chrome.webview) {\n";
    html += "    window.chrome.webview.postMessage({ action: 'saveChanges', files: data });\n";
    html += "  }\n";
    html += "}\n";

    // 컬럼 리사이즈 스크립트
    html += "const table = document.getElementById('fileTable');\n";
    html += "const cols = table.querySelectorAll('th');\n";
    html += "let startX, startWidth, currentCol;\n";
    html += "cols.forEach(function(th) {\n";
    html += "  const resizer = th.querySelector('.resizer');\n";
    html += "  resizer.addEventListener('mousedown', initResize);\n";
    html += "  function initResize(e) {\n";
    html += "    currentCol = th;\n";
    html += "    startX = e.pageX;\n";
    html += "    startWidth = currentCol.offsetWidth;\n";
    html += "    document.addEventListener('mousemove', resizeColumn);\n";
    html += "    document.addEventListener('mouseup', stopResize);\n";
    html += "  }\n";
    html += "  function resizeColumn(e) {\n";
    html += "    const newWidth = startWidth + (e.pageX - startX);\n";
    html += "    if (newWidth > 30) {\n";
    html += "      currentCol.style.width = newWidth + 'px';\n";
    html += "    }\n";
    html += "  }\n";
    html += "  function stopResize(e) {\n";
    html += "    document.removeEventListener('mousemove', resizeColumn);\n";
    html += "    document.removeEventListener('mouseup', stopResize);\n";
    html += "  }\n";
    html += "});\n";

    html += "</script>\n";
    html += "</body>\n";
    html += "</html>\n";

    return html;
}

CString EscapeForJS(const CString& str)
{
    CString escaped;
    for (int i = 0; i < str.GetLength(); ++i)
    {
        TCHAR ch = str[i];
        if (ch == '\\')
            escaped += _T("\\\\");  // 백슬래시 2개로 변환
        else if (ch == '\'')
            escaped += _T("\\'");   // 작은 따옴표 escape
        else if (ch == '\"')
            escaped += _T("\\\"");  // 큰 따옴표 escape
        else if (ch == '\r')
            escaped += _T("\\r");
        else if (ch == '\n')
            escaped += _T("\\n");
        else
            escaped += ch;
    }
    return escaped;
}

CFinderAllFilesDlg::CFinderAllFilesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINDERALLFILES_DIALOG, pParent)
{
}

void CFinderAllFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFinderAllFilesDlg, CDialogEx)
	ON_WM_PAINT()
    ON_WM_DESTROY()
    ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CFinderAllFilesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
    ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

    // COM 초기화: WebView2 사용을 위해 반드시 필요
    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))
    {
        return FALSE;
    }

    InitWebVeiw();

	return TRUE;  
}

void CFinderAllFilesDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CFinderAllFilesDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    // 이벤트 핸들러 등록 해제
    if (m_webView && m_navigationCompletedToken.value != 0)
    {
        m_webView->remove_NavigationCompleted(m_navigationCompletedToken);
    }

    m_webViewController = nullptr;
    m_webView = nullptr;
    m_webViewEnvironment = nullptr;

    CoUninitialize();
}

void CFinderAllFilesDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (m_webViewController)
    {
        CRect clientRect;
        GetClientRect(&clientRect);
        m_webViewController->put_Bounds(clientRect);
    }
}

void CFinderAllFilesDlg::InitWebVeiw()
{
    // CreateCoreWebView2EnvironmentWithOptions를 사용하여 WebView2 환경 생성
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

                if (FAILED(result))
                {
                    // 환경 생성 실패 처리
                    return result;
                }                    

                // 환경 객체를 멤버 변수에 저장
                this->m_webViewEnvironment = env;

                return env->CreateCoreWebView2Controller(this->m_hWnd,
                    Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {

                            if (FAILED(result))
                            {
                                // 환경 생성 실패 처리
                                return result;
                            }
                                
                            // 컨트롤러 객체를 멤버 변수에 저장
                            this->m_webViewController = controller;

                            //웹뷰 숨기기(웹페이지 로드전이므로 숨김)
                            m_webViewController->put_IsVisible(FALSE);

                            wil::com_ptr<ICoreWebView2> webview;
                            controller->get_CoreWebView2(&webview);

                            // 코어 WebView2 객체를 멤버 변수에 저장
                            m_webView = webview;

                            // 다이얼로그 클라이언트 영역에 WebView2 배치
                            CRect clientRect;
                            GetClientRect(&clientRect);
                            m_webViewController->put_Bounds(clientRect);

                            // HTML 로드
                            m_webView->NavigateToString(GetHtmlContent());

                            // NavigationCompleted 이벤트 핸들러 등록
                            m_webView->add_NavigationCompleted(
                                Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                    [this](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {

                                        //웹페이지 Navigation 완료

                                        //웹뷰 보이게(웹페이지 로드 완료시 보임)
                                        m_webViewController->put_IsVisible(TRUE);

                                        return S_OK;

                                    }).Get(), &m_navigationCompletedToken);

                            //웹페이지에서 이벤트 들어옴
                            m_webView->add_WebMessageReceived(
                                Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [this](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {

                                        wil::unique_cotaskmem_string message;
                                        args->get_WebMessageAsJson(&message);
                                                                               
                                        CStringA json(message.get());

                                        //웹페이지에서 호출한 window.chrome.webview.postMessage({ action: 'selectFolder' }); 처리
                                        if (json.Find("\"action\":\"selectFolder\"") >= 0)
                                        {
                                            CString path = SelectFolder(GetSafeHwnd(), GetModulePath());
                                            if (!path.IsEmpty())
                                            {
                                                m_strFolderPath = path;

                                                //선택된 폴더 경로 html 에 보여줌
                                                CString js;
                                                js.Format(L"showSelectedFolder(`%s`);", path);
                                                m_webView->ExecuteScript(js, nullptr);

                                                //폴더 검색
                                                SearchAllFiles(m_strFolderPath);
                                            }
                                        }

                                        //웹페이지에서 호출한 window.chrome.webview.postMessage({ action: 'saveChanges', files: data }); 처리
                                        else if (json.Find("\"action\":\"saveChanges\"") >= 0)
                                        {
                                            CStringA msg(message.get());

                                            CJsonManager man;
                                            if (man.LoadFromString(CString(msg)))
                                            {
                                                std::vector<Json::Value> files = man.GetJsonArray(_T("files"));
                                            
                                                for (const auto& fileObj : files)
                                                {
                                                    CString name = CJsonManager::ConvertUtf8ToCString(fileObj["name"].asString());
                                                    CString path = CJsonManager::ConvertUtf8ToCString(fileObj["path"].asString());
                                                    CString type = CJsonManager::ConvertUtf8ToCString(fileObj["type"].asString());

                                                    CString strMsg;
                                                    strMsg.Format(L"파일명: %s, 경로: %s, 타입: %s \r\n", name, path, type);
                                                    OutputDebugString(strMsg);
                                                }
                                            }
                                            
                                        }

                                        return S_OK;

                                    }).Get(), nullptr);

                            return S_OK;

                        }).Get());
            }).Get());
}

//선택된 폴더 안에 전체 파일 검색(하위 폴더 포함)
void CFinderAllFilesDlg::SearchAllFiles(CString strDirectory)
{
    CString strFilter = strDirectory + _T("\\*.*");

    CFileFind finder;
    BOOL bSearch = finder.FindFile(strFilter);

    while (bSearch)
    {
        bSearch = finder.FindNextFile();

        if (finder.IsDots())
            continue;

        CString strFullPath = finder.GetFilePath();

        if (finder.IsDirectory())
        {
            // 하위 폴더를 재귀적으로 탐색
            SearchAllFiles(strFullPath);
        }
        else
        {
            // 파일명
            CString strFileName = finder.GetFileName();
            
            //파일 경로
            CString strType = GetRelativePath(strFullPath, m_strFolderPath);

            CString escapedFileName = EscapeForJS(strFileName);
            CString escapedFullPath = EscapeForJS(strFullPath);
            CString escapedType = EscapeForJS(strType);

            CString js;
            js.Format(L"addFileRow(`%s`, `%s`, `%s`);", escapedFileName, escapedFullPath, escapedType);

            if (m_webView)
                m_webView->ExecuteScript(js, nullptr);
        }
    }

    finder.Close();
}

int CALLBACK CFinderAllFilesDlg::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if (uMsg == BFFM_INITIALIZED)
    {
        if (lpData != 0)
        {
            ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
        }
    }
    return 0;
}

CString CFinderAllFilesDlg::SelectFolder(HWND hWnd, const CString& strInitialDir)
{
    CString sFolder;

    BROWSEINFO bi = { 0 };
    bi.hwndOwner = hWnd;
    bi.lpszTitle = _T("폴더선택");
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;

    bi.lpfn = nullptr;
    bi.lParam = 0;

    if (!strInitialDir.IsEmpty())
    {
        bi.lpfn = BrowseCallbackProc;
        bi.lParam = (LPARAM)(LPCTSTR)strInitialDir;  // 초기 경로 지정
    }

    LPITEMIDLIST pidlSelected = SHBrowseForFolder(&bi);

    if (pidlSelected)
    {
        TCHAR szPath[MAX_PATH] = { 0 };
        if (SHGetPathFromIDList(pidlSelected, szPath))
        {
            sFolder = szPath;
        }
        CoTaskMemFree(pidlSelected);
    }

    return sFolder;
}

CString CFinderAllFilesDlg::GetModulePath()
{
    TCHAR tPath[MAX_PATH];
    ::GetModuleFileName(NULL, tPath, MAX_PATH);

    CString strPath = tPath;
    strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
    return strPath;
}

CString CFinderAllFilesDlg::GetRelativePath(const CString& fullPath, const CString& basePath)
{
    int baseLen = basePath.GetLength();
    CString base = basePath;
    if (!base.IsEmpty() && base[baseLen - 1] != '\\')
    {
        base += '\\';
        baseLen++;
    }

    if (fullPath.Left(baseLen).CompareNoCase(base) == 0)
    {
        return fullPath.Mid(baseLen - 1);
    }

    return fullPath;
}