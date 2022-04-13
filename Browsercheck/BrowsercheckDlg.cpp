#include "pch.h"
#include "framework.h"
#include "Browsercheck.h"
#include "BrowsercheckDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void sendKeyDown(unsigned char keyCode)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;

	input.ki.wVk = keyCode;
	input.ki.dwFlags = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	SendInput(1, &input, sizeof(INPUT));
}

void sendKeyUp(unsigned char keyCode)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;

	input.ki.wVk = keyCode;
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	SendInput(1, &input, sizeof(INPUT));
}

void refreshChrome_Widget(HWND hParent)
{
	if (hParent)
	{
		if (!IsWindowVisible(hParent))
		{
			return;
		}

		SetForegroundWindow(hParent);

		sendKeyDown(VK_CONTROL);
		sendKeyDown(VK_F5);
		sendKeyUp(VK_CONTROL);
		sendKeyUp(VK_F5);
	}
}

BOOL CALLBACK EnumWndChrome_Widget(HWND hWnd, DWORD lParam)
{
	TCHAR tClass[MAX_PATH] = { 0 };
	::GetClassName(hWnd, tClass, MAX_PATH);

	TCHAR tTitle[MAX_PATH] = { 0 };
	::GetWindowText(hWnd, tTitle, MAX_PATH);

	//크롬, 엣지 체크
	if (lstrcmpi(tClass, _T("Chrome_WidgetWin_1")) == 0)
	{
		if (lstrcmpi(tTitle, _T("")) != 0)
		{
			DWORD dwPID = 0;
			::GetWindowThreadProcessId(hWnd, &dwPID);

			CString strMsg;
			strMsg.Format(_T("%s[%ld]\r\n"), tTitle, dwPID);
			OutputDebugString(strMsg);

			//refreshChrome_Widget(hWnd);
		}
	}
	return true;
}

CBrowsercheckDlg::CBrowsercheckDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BROWSERCHECK_DIALOG, pParent)
{
}

void CBrowsercheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBrowsercheckDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CBrowsercheckDlg::OnPaint()
{
	CPaintDC dc(this);
}

BOOL CBrowsercheckDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);
	MoveWindow(-333, -333, 0, 0);
	
	SetTimer(1, 1000, NULL);

	return TRUE;  
}

void CBrowsercheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		EnumWindows((WNDENUMPROC)EnumWndChrome_Widget, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CBrowsercheckDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	KillTimer(1);
}