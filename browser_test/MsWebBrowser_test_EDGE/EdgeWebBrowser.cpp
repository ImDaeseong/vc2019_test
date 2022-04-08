//-----------------------------------------참조 사이트-------------------------------------------------//
//https://mariusbancila.ro/blog/2021/01/27/using-microsoft-edge-in-a-native-windows-desktop-app-part-4/
//-----------------------------------------참조 사이트-------------------------------------------------//

#include "pch.h"
#include <wrl.h>
#include "wil/com.h"

using namespace Microsoft::WRL;

#include "EdgeWebBrowser.h"
//#include "Messages.h"

#include "WebView2.h"

#include <sstream>
#include <iomanip>

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"

#pragma comment(lib,"Version.lib")

#include <string>

#define CHECK_FAILURE_STRINGIFY(arg)         #arg
#define CHECK_FAILURE_FILE_LINE(file, line)  ([](HRESULT hr){ CheckFailure(hr, L"Failure at " CHECK_FAILURE_STRINGIFY(file) L"(" CHECK_FAILURE_STRINGIFY(line) L")"); })
#define CHECK_FAILURE                        CHECK_FAILURE_FILE_LINE(__FILE__, __LINE__)
#define CHECK_FAILURE_BOOL(value)            CHECK_FAILURE((value) ? S_OK : E_UNEXPECTED)

struct CWebBrowserImpl
{
   wil::com_ptr<ICoreWebView2Environment>    m_webViewEnvironment;
   wil::com_ptr<ICoreWebView2Environment2>   m_webViewEnvironment2;
   wil::com_ptr<ICoreWebView2>               m_webView;
   wil::com_ptr<ICoreWebView2_2>             m_webView2;
   wil::com_ptr<ICoreWebView2Controller>     m_webController;
   wil::com_ptr<ICoreWebView2Settings>       m_webSettings;
};

void ShowFailure(HRESULT hr, CString const & message)
{
   CString text;
   text.Format(L"%s (0x%08X)", (LPCTSTR)message, hr);

   ::MessageBox(nullptr, static_cast<LPCTSTR>(text), L"Failure", MB_OK);
}

void CheckFailure(HRESULT hr, CString const & message)
{
   if (FAILED(hr))
   {    
      CString text;
      text.Format(L"%s : 0x%08X", (LPCTSTR)message, hr);

      // TODO: log text
     
      std::exit(hr);
   }
}

/////////////////////////////////////////////////////////////////////////////
// CWebBrowser

IMPLEMENT_DYNCREATE(CWebBrowser, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CWebBrowser properties
BEGIN_MESSAGE_MAP(CWebBrowser, CWnd)
END_MESSAGE_MAP()

CWebBrowser::CWebBrowser():m_pImpl(new CWebBrowserImpl())
{
   m_callbacks[CallbackType::CreationCompleted] = nullptr;
   m_callbacks[CallbackType::NavigationCompleted] = nullptr;
}

CWebBrowser::~CWebBrowser()
{
   SetWindowLongPtr(m_hWnd, GWLP_USERDATA, 0);
   CloseWebView();
   delete m_pImpl;
}

BOOL CWebBrowser::CreateHostWindow(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
   if (lpszClassName == nullptr)
      lpszClassName = GetWindowClass();

   if (!CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID))
      return FALSE;

   ::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

   return TRUE;
}

BOOL CWebBrowser::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext*)
{
   if (!CreateHostWindow(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID))
      return FALSE;

   InitializeWebView();

   return TRUE;
}

BOOL CWebBrowser::CreateAsync(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CallbackFunc onCreated)
{
   if (!CreateHostWindow(nullptr, nullptr, dwStyle, rect, pParentWnd, nID))
      return FALSE;

   m_callbacks[CallbackType::CreationCompleted] = onCreated;

   InitializeWebView();

   return TRUE;
}

void CWebBrowser::RegisterCallback(CallbackType const type, CallbackFunc callback)
{
   m_callbacks[type] = callback;
}

void CWebBrowser::CloseWebView()
{
   if (m_pImpl->m_webView)
   {
      m_pImpl->m_webView->remove_NavigationCompleted(m_navigationCompletedToken);
      m_pImpl->m_webView->remove_NavigationStarting(m_navigationStartingToken);
      m_pImpl->m_webView->remove_DocumentTitleChanged(m_documentTitleChangedToken);

      m_pImpl->m_webView->remove_ContentLoading(m_contentLoadingToken);
      m_pImpl->m_webView->remove_WebMessageReceived(m_webMessageReceivedToken);
      m_pImpl->m_webView->remove_SourceChanged(m_sourceChangedToken);
      m_pImpl->m_webView->remove_HistoryChanged(m_historyChangedToken);
      m_pImpl->m_webView->remove_WebResourceRequested(m_webResourceRequestedToken);
           
      
      m_pImpl->m_webController->Close();

      m_pImpl->m_webController = nullptr;
      m_pImpl->m_webView = nullptr;
      m_pImpl->m_webView2 = nullptr;
      m_pImpl->m_webSettings = nullptr;
   }

   m_pImpl->m_webViewEnvironment2 = nullptr;
   m_pImpl->m_webViewEnvironment = nullptr;
}

void CWebBrowser::InitializeWebView()
{
   CloseWebView();

   CString subFolder = GetInstallPath();
   CString appData = GetUserDataFolder();
   ICoreWebView2EnvironmentOptions* options = nullptr;

   HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(subFolder, appData, options,
      Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &CWebBrowser::OnCreateEnvironmentCompleted).Get());

   if (!SUCCEEDED(hr))
   {
      CString text;
      if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
      {
         text = L"Cannot found the WebView2 component.";
      }
      else
      {
         text = L"Cannot create the webview environment.";
      }

      ShowFailure(hr, text);
   }
}

HRESULT CWebBrowser::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
   CHECK_FAILURE(result);

   if (!environment)
      return E_FAIL;

   CHECK_FAILURE(environment->QueryInterface(IID_PPV_ARGS(&m_pImpl->m_webViewEnvironment)));
   CHECK_FAILURE(environment->QueryInterface(IID_PPV_ARGS(&m_pImpl->m_webViewEnvironment2)));

   CHECK_FAILURE(m_pImpl->m_webViewEnvironment->CreateCoreWebView2Controller(m_hWnd, 
      Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this, &CWebBrowser::OnCreateWebViewControllerCompleted).Get()));

   return S_OK;
}

HRESULT CWebBrowser::OnCreateWebViewControllerCompleted(HRESULT result, ICoreWebView2Controller* controller)
{
   if (result == S_OK)
   {
      if (controller != nullptr)
      {
         m_pImpl->m_webController = controller;
         CHECK_FAILURE(controller->get_CoreWebView2(&m_pImpl->m_webView));

         if (!m_pImpl->m_webView)
            return E_FAIL;

         CHECK_FAILURE(m_pImpl->m_webView->QueryInterface(IID_PPV_ARGS(&m_pImpl->m_webView2)));

         CHECK_FAILURE(m_pImpl->m_webView->get_Settings(&m_pImpl->m_webSettings));


         //스크립트 사용 설정
         m_pImpl->m_webSettings->put_IsScriptEnabled(TRUE);
         m_pImpl->m_webSettings->put_AreDefaultScriptDialogsEnabled(TRUE);
         m_pImpl->m_webSettings->put_IsWebMessageEnabled(TRUE);


         RegisterEventHandlers();

         ResizeToClientArea();
      }

      auto callback = m_callbacks[CallbackType::CreationCompleted];
      if (callback != nullptr)
         RunAsync(callback);
   }
   else
   {
      ShowFailure(result, L"Cannot create webview environment.");
   }

   return S_OK;
}

void CWebBrowser::RegisterEventHandlers()
{

   // NavigationCompleted handler
   CHECK_FAILURE(m_pImpl->m_webView->add_NavigationCompleted(
      Callback<ICoreWebView2NavigationCompletedEventHandler>(
         [this](ICoreWebView2*, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
         {
            m_isNavigating = false;

            BOOL success;
            CHECK_FAILURE(args->get_IsSuccess(&success));

            if (!success)
            {
               COREWEBVIEW2_WEB_ERROR_STATUS webErrorStatus{};
               CHECK_FAILURE(args->get_WebErrorStatus(&webErrorStatus));
               if (webErrorStatus == COREWEBVIEW2_WEB_ERROR_STATUS_DISCONNECTED)
               {
                  // Do something here if you want to handle a specific error case.
                  // In most cases this isn't necessary, because the WebView will
                  // display its own error page automatically.
               }
            }

            wil::unique_cotaskmem_string uri;
            m_pImpl->m_webView->get_Source(&uri);

            if (wcscmp(uri.get(), L"about:blank") == 0)
            {
               uri = wil::make_cotaskmem_string(L"");
            }

            auto callback = m_callbacks[CallbackType::NavigationCompleted];
            if (callback != nullptr)
               RunAsync(callback);

            return S_OK;
         }).Get(), &m_navigationCompletedToken));


   // NavigationStarting handler
   CHECK_FAILURE(m_pImpl->m_webView->add_NavigationStarting(
      Callback<ICoreWebView2NavigationStartingEventHandler>(
         [this](ICoreWebView2*, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT
         {
            wil::unique_cotaskmem_string uri;
            CHECK_FAILURE(args->get_Uri(&uri));

            m_isNavigating = true;
                        
            return S_OK;
         }).Get(), &m_navigationStartingToken));


   // DocumentTitleChanged handler
   CHECK_FAILURE(m_pImpl->m_webView->add_DocumentTitleChanged(
      Callback<ICoreWebView2DocumentTitleChangedEventHandler>(
         [this](ICoreWebView2* sender, IUnknown* args) -> HRESULT {
            wil::unique_cotaskmem_string title;
            CHECK_FAILURE(sender->get_DocumentTitle(&title));

            m_strTitle = title.get();
            
            auto callback = m_callbacks[CallbackType::TitleChanged];
            if (callback != nullptr)
               RunAsync(callback);

            return S_OK;
         }).Get(), &m_documentTitleChangedToken));


   //ContentLoading handler
   CHECK_FAILURE(m_pImpl->m_webView->add_ContentLoading(
       Callback<ICoreWebView2ContentLoadingEventHandler>(
           [this](ICoreWebView2* sender, ICoreWebView2ContentLoadingEventArgs* args) -> HRESULT {
                   
               wil::unique_cotaskmem_string uri;
               CHECK_FAILURE(sender->get_Source(&uri));

               //CString strMsg;
               //strMsg.Format(_T("add_ContentLoading:%s"), uri.get());
               //OutputDebugString(strMsg);
                             
               return S_OK;
           }).Get(),  &m_contentLoadingToken));


   //WebMessageReceived handler
   CHECK_FAILURE(m_pImpl->m_webView->add_WebMessageReceived(
       Callback<ICoreWebView2WebMessageReceivedEventHandler>(
           [this](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args)->HRESULT {

               wil::unique_cotaskmem_string uri;
               CHECK_FAILURE(args->get_Source(&uri));

               wil::unique_cotaskmem_string messageRaw;
               CHECK_FAILURE(args->TryGetWebMessageAsString(&messageRaw));
               std::wstring message = messageRaw.get();
               //::SendMessage(m_hParent, WM_WEB_MESSAGERECEIVE, 0, 1);

               return S_OK;
           }).Get(), &m_webMessageReceivedToken));


   //SourceChanged handler
   CHECK_FAILURE(m_pImpl->m_webView->add_SourceChanged(
       Callback<ICoreWebView2SourceChangedEventHandler>(
           [this](ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args) -> HRESULT {
              
               wil::unique_cotaskmem_string uri;
               sender->get_Source(&uri);

               //CString strMsg;
               //strMsg.Format(_T("add_SourceChanged:%s"), uri.get());
               //OutputDebugString(strMsg);
                             
               return S_OK;
           }).Get(), &m_sourceChangedToken));


   //HistoryChanged handler
   CHECK_FAILURE(m_pImpl->m_webView->add_HistoryChanged(
       Callback<ICoreWebView2HistoryChangedEventHandler>(
           [this](ICoreWebView2* sender, IUnknown* args) -> HRESULT {

               BOOL canGoBack;
               BOOL canGoForward;
               sender->get_CanGoBack(&canGoBack);
               sender->get_CanGoForward(&canGoForward);

               //CString strMsg;
               //strMsg.Format(_T("canGoBack:%d canGoForward:%d"), canGoBack, canGoForward);
               //OutputDebugString(strMsg);
               
               return S_OK;
           }).Get(), &m_historyChangedToken));


   //WebResourceRequested handler
   CHECK_FAILURE(m_pImpl->m_webView2->add_WebResourceResponseReceived(
       Callback<ICoreWebView2WebResourceResponseReceivedEventHandler>(
           [this](ICoreWebView2* sender, ICoreWebView2WebResourceResponseReceivedEventArgs* args) {
                     
               wil::com_ptr<ICoreWebView2WebResourceRequest> request;
			   wil::com_ptr<ICoreWebView2WebResourceResponseView> response;
			   wil::unique_cotaskmem_string uri;
               CHECK_FAILURE(args->get_Request(&request));
               CHECK_FAILURE(request->get_Uri(&uri));
               CHECK_FAILURE(args->get_Response(&response));

               //CString strMsg;
               //strMsg.Format(_T("add_WebResourceResponseReceived:%s"), uri.get());
               //OutputDebugString(strMsg);
               //::SendMessage(m_hParent, WM_WEB_MESSAGERECEIVE, 0, 1);
             
               return S_OK;
       }).Get(), &m_webResourceRequestedToken));

}

void CWebBrowser::ResizeToClientArea()
{
   if (m_pImpl->m_webController)
   {
      RECT bounds;
      GetClientRect(&bounds);
      m_pImpl->m_webController->put_Bounds(bounds);
   }
}

RECT CWebBrowser::GetBounds()
{
   RECT rc{0,0,0,0};
   if (m_pImpl->m_webController)
   {
      m_pImpl->m_webController->get_Bounds(&rc);
   }

   return rc;
}

void CWebBrowser::Resize(LONG const width, LONG const height)
{
   SetWindowPos(nullptr, 0, 0, width, height, SWP_NOMOVE| SWP_NOREPOSITION);
}

CString CWebBrowser::GetLocationURL()
{
   CString url;
   if (m_pImpl->m_webView)
   {
      wil::unique_cotaskmem_string uri;
      m_pImpl->m_webView->get_Source(&uri);

      if (wcscmp(uri.get(), L"about:blank") == 0)
      {
         uri = wil::make_cotaskmem_string(L"");
      }

      url = uri.get();
   }

   return url;
}

CString CWebBrowser::NormalizeUrl(CString url)
{
   if (url.Find(_T("://")) < 0)
   {
      if (url.GetLength() > 1 && url[1] == ':')
         url = _T("file://") + url;
      else
         url = _T("http://") + url;
   }

   return url;
}

void CWebBrowser::NavigateTo(CString url)
{
   m_pImpl->m_webView->Navigate(NormalizeUrl(url));
}

void CWebBrowser::Navigate(CString const & url, CallbackFunc onComplete)
{
   if (m_pImpl->m_webView)
   {      
      m_callbacks[CallbackType::NavigationCompleted] = onComplete;
      NavigateTo(url);
   }
}

// The raw request header string delimited by CRLF(optional in last header).
void CWebBrowser::NavigatePost(CString const& url, CString const& content, CString const& headers, HWND hwnd, std::function<void()> onComplete)//void CWebBrowser::NavigatePost(CString const& url, CString const& content, CString const& headers, std::function<void()> onComplete)
{
   if (!m_pImpl->m_webView) return;

   m_hParent = hwnd;

   CString normalizedUrl{ NormalizeUrl(url) };

   m_callbacks[CallbackType::NavigationCompleted] = onComplete;

   wil::com_ptr<ICoreWebView2WebResourceRequest> webResourceRequest;
   wil::com_ptr<IStream> postDataStream = SHCreateMemStream(reinterpret_cast<const BYTE*>(static_cast<LPCTSTR>(content)), content.GetLength() + 1);

   CHECK_FAILURE(m_pImpl->m_webViewEnvironment2->CreateWebResourceRequest(
      CT2W(normalizedUrl),
      L"POST",
      postDataStream.get(),
      CT2W(headers),
      &webResourceRequest));

   CHECK_FAILURE(m_pImpl->m_webView2->NavigateWithWebResourceRequest(webResourceRequest.get()));
}

void CWebBrowser::PrintDocument()
{
   if (m_pImpl->m_webView)
   {
      m_pImpl->m_webView->ExecuteScript(L"window.print();", nullptr);
   }
}

void CWebBrowser::Stop()
{
   if (m_pImpl->m_webView)
   {
      m_pImpl->m_webView->Stop();
   }
}

void CWebBrowser::Reload()
{
   if (m_pImpl->m_webView)
   {
      m_pImpl->m_webView->Reload();
   }
}

void CWebBrowser::GoBack()
{
   if (m_pImpl->m_webView)
   {
      BOOL possible = FALSE;
      m_pImpl->m_webView->get_CanGoBack(&possible);
      if(possible)
         m_pImpl->m_webView->GoBack();
   }
}

void CWebBrowser::GoForward()
{
   if (m_pImpl->m_webView)
   {
      BOOL possible = FALSE;
      m_pImpl->m_webView->get_CanGoForward(&possible);
      if (possible)
         m_pImpl->m_webView->GoForward();
   }
}

void CWebBrowser::DisablePopups()
{
   if (m_pImpl->m_webSettings)
   {
      m_pImpl->m_webSettings->put_AreDefaultScriptDialogsEnabled(FALSE);
   }
}

PCTSTR CWebBrowser::GetWindowClass()
{
   static PCTSTR windowClass = []
   {
      static TCHAR const * className = L"EdgeBrowserHost";

      WNDCLASSEX wcex;
      wcex.cbSize = sizeof(WNDCLASSEX);

      wcex.style = CS_HREDRAW | CS_VREDRAW;
      wcex.lpfnWndProc = WndProcStatic;
      wcex.cbClsExtra = 0;
      wcex.cbWndExtra = 0;
      wcex.hInstance = AfxGetInstanceHandle();
      wcex.hIcon = nullptr;
      wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
      wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      wcex.lpszMenuName = nullptr;
      wcex.lpszClassName = className;
      wcex.hIconSm = nullptr;

      ATOM result = RegisterClassEx(&wcex);
      if (result == 0)
      {
         [[maybe_unused]] DWORD lastError = ::GetLastError();
      }

      return className;
   }();

   return windowClass;
}

LRESULT CALLBACK CWebBrowser::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   if (auto app = (CWebBrowser*)::GetWindowLongPtr(hWnd, GWLP_USERDATA))
   {
      LRESULT result = 0;
      if (app->HandleWindowMessage(hWnd, message, wParam, lParam, &result))
      {
         return result;
      }
   }

   return ::DefWindowProc(hWnd, message, wParam, lParam);
}

bool CWebBrowser::HandleWindowMessage(HWND, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result)
{
   *result = 0;
   
   switch (message)
   {
   case WM_SIZE:
   {
      if (lParam != 0)
      {
         ResizeToClientArea();
         return true;
      }
   }
   break;
   case MSG_RUN_ASYNC_CALLBACK:
   {
      auto* task = reinterpret_cast<CallbackFunc*>(wParam);
      (*task)();
      delete task;
      return true;
   }
   break;
   }

   return false;
}

void CWebBrowser::RunAsync(CallbackFunc callback)
{
   auto* task = new CallbackFunc(callback);
   PostMessage(MSG_RUN_ASYNC_CALLBACK, reinterpret_cast<WPARAM>(task), 0);
}

bool CWebBrowser::IsWebViewCreated() const
{
   return m_pImpl->m_webView != nullptr;
}

CString CWebBrowser::GetInstallPath()
{
    auto installPath = GetInstallPathFromRegistry();   // check registry for WebView2
    if (installPath.IsEmpty())
        installPath = GetInstallPathFromDisk();         // check disk for WebView2

    if (installPath.IsEmpty())
        installPath = GetInstallPathFromRegistry(false);// check registry for Edge

    if (installPath.IsEmpty())
        installPath = GetInstallPathFromDisk(false);    // check disk for Edge

    return installPath;
    /*
    static CString path = []
    { 
        auto installPath = GetInstallPathFromRegistry();   // check registry for WebView2
        if (installPath.IsEmpty())
            installPath = GetInstallPathFromDisk();         // check disk for WebView2
      
        if (installPath.IsEmpty())
            installPath = GetInstallPathFromRegistry(false);// check registry for Edge
      
        if (installPath.IsEmpty())
            installPath = GetInstallPathFromDisk(false);    // check disk for Edge
        
        return installPath;
   
    }();     
    return path;
    */
}

CString CWebBrowser::GetInstallPathFromRegistry(bool const searchWebView)
{
   CString path;

   HKEY handle = nullptr;

   LSTATUS result = ERROR_FILE_NOT_FOUND;

   if (searchWebView)
   {
      result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
         LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Microsoft EdgeWebView)",
         0,
         KEY_READ,
         &handle);

      if (result != ERROR_SUCCESS)
         result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
            LR"(SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Microsoft EdgeWebView)",
            0,
            KEY_READ,
            &handle);
   }
   else
   {
      result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
         LR"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Microsoft Edge)",
         0,
         KEY_READ,
         &handle);

      if (result != ERROR_SUCCESS)
         result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
            LR"(SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Microsoft Edge)",
            0,
            KEY_READ,
            &handle);
   }

   if (result == ERROR_SUCCESS)
   {
      TCHAR buffer[MAX_PATH + 1]{ 0 };
      DWORD type = REG_SZ;
      DWORD size = MAX_PATH;
      result = RegQueryValueEx(handle, L"InstallLocation", 0, &type, reinterpret_cast<LPBYTE>(buffer), &size);
      if (result == ERROR_SUCCESS) 
         path += CString{ buffer };

      TCHAR version[100]{ 0 };
      size = 100;
      result = RegQueryValueEx(handle, L"Version", 0, &type, reinterpret_cast<LPBYTE>(version), &size);
      if (result == ERROR_SUCCESS)
      {
         if (path.GetAt(path.GetLength() - 1) != L'\\')
            path += L"\\";
         path += CString{ version };
      }
      else
         path.Empty();

      RegCloseKey(handle);
   }

   return path;
}

CString CWebBrowser::GetInstallPathFromDisk(bool const searchWebView)
{
   CString path =
      searchWebView ?
      LR"(c:\Program Files (x86)\Microsoft\EdgeWebView\Application\)" :
      LR"(c:\Program Files (x86)\Microsoft\Edge\Application\)";
   CString pattern = path + L"*";

   WIN32_FIND_DATA ffd{ 0 };
   HANDLE hFind = FindFirstFile(pattern, &ffd);
   if (hFind == INVALID_HANDLE_VALUE)
   {
      [[maybe_unused]] DWORD error = ::GetLastError();
      return {};
   }

   do
   {
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
         CString name{ ffd.cFileName };
         int a, b, c, d;
         if (4 == swscanf_s(ffd.cFileName, L"%d.%d.%d.%d", &a, &b, &c, &d))
         {
            FindClose(hFind);
            return path + name;
         }
      }
   } while (FindNextFile(hFind, &ffd) != 0);

   FindClose(hFind);

   return {};
}

CString CWebBrowser::GetUserDataFolder()
{
   TCHAR szPath[MAX_PATH]{ 0 };
   ::SHGetFolderPath(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, szPath);
   ::PathAppend(szPath, LR"(\Demos\)");
   ::PathAppend(szPath, L"MfcEdgeDemo");
   return CString{ szPath };
}