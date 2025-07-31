#include "pch.h"
#include "framework.h"
#include "DragDropStatic_test.h"
#include "DragDropStatic_testDlg.h"
#include "afxdialogex.h"

#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>

using namespace Microsoft::WRL;


CString GetHtmlContent()
{
    CString html;

    html += _T(R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Drag and Drop MD5</title>
    <style>
        body {
            font-family: sans-serif;
            margin: 0;
            padding: 0;
            background-color: #ffffff;
        }
        #resultContainer {
            margin: 20px 30px;
            padding: 20px;
            background-color: #fafafa;
            border: 1px solid #ddd;
            border-radius: 8px;
            overflow-x: auto;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            table-layout: fixed;
            min-width: 800px;
        }
        col {
            width: 200px;
        }
        th, td {
            border: 1px solid #ccc;
            padding: 8px;
            position: relative;
            text-align: left;
            overflow: hidden;
            white-space: nowrap;
            text-overflow: ellipsis;
        }
        th {
            background-color: #f0f0f0;
        }
        .resizer {
            position: absolute;
            right: 0;
            top: 0;
            width: 5px;
            height: 100%;
            cursor: col-resize;
            user-select: none;
        }
    </style>
</head>
<body>
    <div id="resultContainer">
        <h3>MD5 결과</h3>
        <table id="fileTable">
            <colgroup>
                <col id="col-name">
                <col id="col-md5">
                <col id="col-path">
            </colgroup>
            <thead>
                <tr>
                    <th>파일 이름<div class="resizer"></div></th>
                    <th>파일 경로<div class="resizer"></div></th>
                    <th>파일 정보<div class="resizer"></div></th>
                </tr>
            </thead>
            <tbody id="fileTableBody">
                <!-- 데이터가 여기에 추가됩니다 -->
            </tbody>
        </table>
    </div>

    <script>
        const colGroup = document.querySelectorAll("col");
        const headers = document.querySelectorAll("th");
        const fileTableBody = document.getElementById("fileTableBody");

        // Resizer 이벤트 등록
        headers.forEach((th, index) => {
            const resizer = th.querySelector(".resizer");
            if (!resizer) return;

            let startX, startWidth;

            resizer.addEventListener("mousedown", (e) => {
                startX = e.pageX;
                const col = colGroup[index];
                startWidth = parseInt(window.getComputedStyle(col).width, 10);

                const onMouseMove = (e) => {
                    const newWidth = startWidth + (e.pageX - startX);
                    if (newWidth > 50) {
                        col.style.width = newWidth + "px";
                    }
                };

                const onMouseUp = () => {
                    document.removeEventListener("mousemove", onMouseMove);
                    document.removeEventListener("mouseup", onMouseUp);
                };

                document.addEventListener("mousemove", onMouseMove);
                document.addEventListener("mouseup", onMouseUp);
            });
        });

        // 중복 체크 및 행 추가
        function addFileToTable(name, path, info) {
            for (const row of fileTableBody.rows) {
                if (row.cells[2] && row.cells[2].textContent === path) {
                    return; // 이미 추가된 경로는 무시
                }
            }

            const row = document.createElement("tr");
            row.innerHTML = `
                <td title="${name}">${name}</td>
                <td title="${path}">${path}</td>
                <td title="${info}">${info}</td>
            `;
            fileTableBody.appendChild(row);
        }
    </script>
</body>
</html>
    )");

    return html;
}


//IDC_DROP_STATIC 사이즈 설정
static int nHeight = 100;
static int nMargin = 20;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDragDropStatictestDlg::CDragDropStatictestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAGDROPSTATIC_TEST_DIALOG, pParent)
{
}

void CDragDropStatictestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDragDropStatictestDlg, CDialogEx)
	ON_WM_PAINT()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

BOOL CDragDropStatictestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))
        return FALSE;

    InitWebView();

    InitStatic();

	return TRUE;  
}

void CDragDropStatictestDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CDragDropStatictestDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    if (m_webView && m_navigationCompletedToken.value != 0)
    {
        m_webView->remove_NavigationCompleted(m_navigationCompletedToken);
        m_navigationCompletedToken.value = 0;
    }

    m_webViewController = nullptr;
    m_webView = nullptr;
    m_webViewEnvironment = nullptr;

    CoUninitialize();
}

void CDragDropStatictestDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    resizeControls();
}

void CDragDropStatictestDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    //최소 크기 설정
    lpMMI->ptMinTrackSize.x = 800;
    lpMMI->ptMinTrackSize.y = 800;

    CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CDragDropStatictestDlg::resizeControls()
{
    CRect rc;
    GetClientRect(&rc);

    //IDC_DROP_STATIC
    if (m_dropStatic.GetSafeHwnd())
    {
        CRect dropRect(rc.left + nMargin, rc.top + nMargin, rc.right - nMargin, rc.top + nMargin + nHeight);
        m_dropStatic.MoveWindow(dropRect);
    }

    //WebView 
    if (m_webViewController)
    {
        int webViewTop = nMargin * 2 + nHeight;
        CRect webViewRect(rc.left + nMargin, rc.top + webViewTop, rc.right - nMargin, rc.bottom - nMargin);
        m_webViewController->put_Bounds(webViewRect);
    }
}

void CDragDropStatictestDlg::InitStatic()
{
    m_dropStatic.SubclassDlgItem(IDC_DROP_STATIC, this);
    m_dropStatic.DragAcceptFiles(TRUE);

    CRect rc;
    GetClientRect(&rc);

    CRect dropRect(rc.left + nMargin, rc.top + nMargin, rc.right - nMargin, rc.top + nMargin + nHeight);
    m_dropStatic.MoveWindow(dropRect);
}

void CDragDropStatictestDlg::InitWebView()
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
                            CRect rc;
                            GetClientRect(&rc);
                            
                            int webViewTop = nMargin * 2 + nHeight;
                            CRect webViewRect(rc.left + nMargin, rc.top + webViewTop, rc.right - nMargin, rc.bottom - nMargin);
                            m_webViewController->put_Bounds(webViewRect);


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

                                        return S_OK;

                                    }).Get(), nullptr);

                            return S_OK;

                        }).Get());
            }).Get());
}

CString CDragDropStatictestDlg::EscapeForJS(const CString& str)
{
    CString escaped;
    for (int i = 0; i < str.GetLength(); ++i)
    {
        TCHAR ch = str[i];
        if (ch == '\\')       escaped += _T("\\\\");
        else if (ch == '\'')  escaped += _T("\\'");
        else if (ch == '\"')  escaped += _T("\\\"");
        else if (ch == '\r')  escaped += _T("\\r");
        else if (ch == '\n')  escaped += _T("\\n");
        else                  escaped += ch;
    }
    return escaped;
}

CString CDragDropStatictestDlg::GetFileVersionString(const CString& strFilePath)
{
    DWORD handle = 0;
    DWORD size = GetFileVersionInfoSize(strFilePath, &handle);
    if (size == 0)
        return _T(""); // 버전 정보 없음

    std::vector<BYTE> buffer(size);
    if (!GetFileVersionInfo(strFilePath, handle, size, buffer.data()))
        return _T("");

    VS_FIXEDFILEINFO* fileInfo = nullptr;
    UINT len = 0;
    if (!VerQueryValue(buffer.data(), _T("\\"), (LPVOID*)&fileInfo, &len))
        return _T("");

    if (fileInfo == nullptr)
        return _T("");

    // 버전 조합: major.minor.build.revision
    CString version;
    version.Format(_T("%d.%d.%d.%d"),
        HIWORD(fileInfo->dwFileVersionMS),
        LOWORD(fileInfo->dwFileVersionMS),
        HIWORD(fileInfo->dwFileVersionLS),
        LOWORD(fileInfo->dwFileVersionLS));

    return version;
}

void CDragDropStatictestDlg::setData(const std::vector<CString>& strFilePaths)
{
    for (const auto& path : strFilePaths)
    {
        CString strFilePath(path);
        CString strFileName = strFilePath.Mid(strFilePath.ReverseFind(L'\\') + 1);
        CString strFileInfo = GetFileVersionString(strFilePath);

        CString js;
        js.Format(
            _T("addFileToTable(\"%s\", \"%s\", \"%s\");"),
            EscapeForJS(strFileName),
            EscapeForJS(strFilePath),
            EscapeForJS(strFileInfo)
        );

        if (m_webView)
        {
            m_webView->ExecuteScript(js.GetString(), nullptr);
        }
    }
}
