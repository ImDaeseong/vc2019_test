#include "pch.h"
#include "framework.h"
#include "htmlUpdateDlg.h"
#include "htmlUpdateDlgDlg.h"
#include "afxdialogex.h"

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

CString GetProgressHtmlContent()
{
    // HTML 내용을 CString으로 반환
    return LR"(
<html>
<head>
<meta charset="UTF-8">
<style>
body {
    font-family: 'Malgun Gothic', 'Arial', sans-serif;
    text-align:center;
    margin:0;
    padding:0;
    height:100vh;
    display:flex;
    justify-content:center;
    align-items:center;
    background:#f0f0f0;
    overflow:hidden; /* 스크롤바 방지 */
}
#overlay {
    position:absolute;
    top:0;
    left:0;
    width:100%;
    height:100%;
    background:rgba(0,0,0,0.5); /* 반투명 배경 */
    display:flex;
    justify-content:center;
    align-items:center;
}
#progress-container {
    width:500px; /* 기존 300px에서 500px로 넓이 증가 */
    height:auto; /* 내용에 따라 높이 자동 조절 */
    background:#fff;
    border-radius:12px; /* 모서리 둥글기 증가 */
    padding:30px; /* 패딩 증가 */
    box-shadow: 0 6px 20px rgba(0,0,0,0.3); /* 그림자 효과 강화 */
    text-align:center;
}
#message {
    margin-bottom: 25px; /* 여백 증가 */
    font-size: 1.5em; /* 폰트 크기 증가 */
    color: #333;
    font-weight: bold; /* 텍스트 굵게 */
}
#bar-container {
    width: 100%;
    background: #e0e0e0; /* 바 배경 색상 */
    border-radius: 8px; /* 둥글기 증가 */
    overflow: hidden; /* 바가 컨테이너 밖으로 넘어가지 않도록 */
    height: 15px; /* 바 높이 증가 */
    margin-top: 20px; /* 여백 증가 */
}
#bar {
    width:0%;
    height:100%;
    background:linear-gradient(to right, #ffa07a, #ff6347); /* 그라데이션 색상 */
    border-radius:8px; /* 둥글기 증가 */
    transition: width 0.4s ease-out; /* 너비 변경 시 부드러운 애니메이션 */
}
#percent {
    margin-top:20px; /* 여백 증가 */
    font-weight:bold;
    color:#ff6347; /* 백분율 텍스트 색상 */
    font-size: 1.8em; /* 폰트 크기 증가 */
}
</style>
</head>
<body>
<div id="overlay">
  <div id="progress-container">
    <div id="message">업데이트 중입니다. 잠시만 기다려 주세요.</div>
    <div id="bar-container">
      <div id="bar"></div>
    </div>
    <div id="percent">0%</div>
  </div>
</div>
<script>
function updateProgress(pct) {
  const bar = document.getElementById('bar');
  const percentText = document.getElementById('percent');

  // 값이 유효한지 확인하고 적용
  if (pct >= 0 && pct <= 100) {
    bar.style.width = pct + '%';
    percentText.innerText = Math.round(pct) + '%';
  }
}
</script>
</body>
</html>
)";
}

ChtmlUpdateDlgDlg::ChtmlUpdateDlgDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_HTMLUPDATEDLG_DIALOG, pParent)
{
}

void ChtmlUpdateDlgDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ChtmlUpdateDlgDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL ChtmlUpdateDlgDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

    // COM 초기화: WebView2 사용을 위해 반드시 필요
    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)))
    {
        return FALSE;
    }

    InitWebVeiw();

    SetTimer(1, 500, nullptr);

    return TRUE;
}
void ChtmlUpdateDlgDlg::OnPaint()
{
    CPaintDC dc(this);
}

void ChtmlUpdateDlgDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    KillTimer(1);

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

void ChtmlUpdateDlgDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (m_webViewController)
    {
        CRect clientRect;
        GetClientRect(&clientRect);
        m_webViewController->put_Bounds(clientRect);
    }
}

void ChtmlUpdateDlgDlg::OnTimer(UINT_PTR nIDEvent)
{
    m_nProgress += 1;

    if (m_nProgress <= 100)
    {
        // 웹뷰에 진행률 업데이트
        UpdateProgress(m_nProgress);
    }
    else
    {
        //완료시 종료
        KillTimer(1);
        OnOK();
    }

    CDialogEx::OnTimer(nIDEvent);
}

void ChtmlUpdateDlgDlg::InitWebVeiw()
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

                // 컨트롤러 생성
                return env->CreateCoreWebView2Controller(this->m_hWnd,
                    Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {

                            if (FAILED(result)) 
                            {
                                // 컨트롤러 생성 실패 처리                            
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
                            //m_webViewController->put_IsVisible(TRUE); // 기본적으로 TRUE

                            // HTML 로드
                            m_webView->NavigateToString(GetProgressHtmlContent());

                            // NavigationCompleted 이벤트 핸들러 등록
                            m_webView->add_NavigationCompleted(
                                Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                    [this](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {

                                        //웹페이지 Navigation 완료
                                                                                
                                        //웹뷰 보이게(웹페이지 로드 완료시 보임)
                                        m_webViewController->put_IsVisible(TRUE);

                                        //디폴트 배경이미지 숨기기
                                        
                                        return S_OK;

                                    }).Get(), &m_navigationCompletedToken); 

                            return S_OK;

                        }).Get());
            }).Get());

    if (FAILED(hr))
    {
        // CreateCoreWebView2EnvironmentWithOptions 호출 자체 실패 시        
    }
}

void ChtmlUpdateDlgDlg::UpdateProgress(int percent)
{
    if (!m_webView) return;

    CString js;
    js.Format(L"updateProgress(%d);", percent);
    m_webView->ExecuteScript(js, nullptr);
}