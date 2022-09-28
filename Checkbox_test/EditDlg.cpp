#include "pch.h"
#include "Checkbox_test.h"
#include "afxdialogex.h"
#include "EditDlg.h"

IMPLEMENT_DYNAMIC(EditDlg, CDialogEx)

EditDlg::EditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDIT_DIALOG, pParent)
{
}

EditDlg::~EditDlg()
{
	m_FontText.DeleteObject();
}

void EditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EdtText);
}

BEGIN_MESSAGE_MAP(EditDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL EditDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL EditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	InitControls();

	return TRUE;
}

void EditDlg::OnPaint()
{
	CPaintDC dc(this);
}

HBRUSH EditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr;
	if (nCtlColor == CTLCOLOR_DLG)
	{
		//hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	else
	{
		hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	return hbr;
}

void EditDlg::OnDestroy()
{	

	CDialogEx::OnDestroy();
}

void EditDlg::SetParent(CCheckboxtestDlg* pParent)
{
	m_pParent = pParent;
}

void EditDlg::InitControls()
{
	CRect rc;
	GetClientRect(&rc);

	m_EdtText.MoveWindow(2, 4, 306, 22);//m_EdtText.MoveWindow(2, 2, 306, 25);

	m_FontText.CreateFontW(
		18,
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		0,
		DEFAULT_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
		_T("Arial")
	);
	
	m_EdtText.SetFont(&m_FontText);

	//30자까지 입력 가능
	m_EdtText.SetLimitText(30);

	//Placeholder
	m_EdtText.SetWindowText(_T("30자까지 등록가능합니다."));
}

CString EditDlg::GetEditText()
{
	UpdateData(true);
	
	CString strText;
	GetDlgItemText(IDC_EDIT1, strText);

	return strText;
}