#include "pch.h"
#include "DragDropStatic.h"
#include "DragDropStatic_testDlg.h"

BEGIN_MESSAGE_MAP(CDragDropStatic, CStatic)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_DROPFILES()
END_MESSAGE_MAP()

void CDragDropStatic::OnPaint()
{
    CPaintDC dc(this);
    Graphics graphics(dc.GetSafeHdc());

    CRect clientRect;
    GetClientRect(&clientRect);

    //배경색
    Color bgColor = m_bHighlight ? Color(200, 220, 255) : Color(240, 240, 240);
    SolidBrush brush(bgColor);
    graphics.FillRectangle(&brush, clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height());

    //테두리
    Pen pen(Color(58, 134, 255));
    pen.SetDashStyle(DashStyleDot);
    graphics.DrawRectangle(&pen, clientRect.left, clientRect.top, clientRect.Width() - 1, clientRect.Height() - 1);

    //텍스트
    CString text = _T("이 영역은 안내용입니다.\n파일을 이 영역으로 드래그하세요.");

    //폰트
    FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, 20.0f, FontStyleBold, UnitPixel);

    //텍스트색
    SolidBrush textBrush(Color(34, 34, 34));

    // 문자열 포맷: 가운데 정렬, 줄 바꿈, 수직 가운데 정렬
    StringFormat stringFormat;
    stringFormat.SetAlignment(StringAlignmentCenter);
    stringFormat.SetLineAlignment(StringAlignmentCenter);
    stringFormat.SetFormatFlags(StringFormatFlagsLineLimit);

    // 유니코드 변환
    USES_CONVERSION;
    LPCWSTR wText = T2CW(text);

    //텍스트 영역
    RectF layoutRect((REAL)clientRect.left + 10, (REAL)clientRect.top + 10, (REAL)(clientRect.Width() - 20), (REAL)(clientRect.Height() - 20));

    graphics.DrawString(wText, -1, &font, layoutRect, &stringFormat, &textBrush);
}

BOOL CDragDropStatic::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CDragDropStatic::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bHighlight)
    {
        m_bHighlight = TRUE;
        RedrawWindow();
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0 };
        ::TrackMouseEvent(&tme);
    }
    CStatic::OnMouseMove(nFlags, point);
}

LRESULT CDragDropStatic::OnMouseLeave(WPARAM, LPARAM)
{
    m_bHighlight = FALSE;
    RedrawWindow();
    return 0;
}

void CDragDropStatic::OnSize(UINT nType, int cx, int cy)
{
    CStatic::OnSize(nType, cx, cy);
    Invalidate();
    UpdateWindow();
}

void CDragDropStatic::OnDropFiles(HDROP hDropInfo)
{
    std::vector<CString> filePaths;

    UINT fileCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, nullptr, 0);
    for (UINT i = 0; i < fileCount; ++i)
    {
        TCHAR szFile[MAX_PATH];
        if (DragQueryFile(hDropInfo, i, szFile, MAX_PATH))
        {
            filePaths.push_back(CString(szFile));
        }
    }

    CDragDropStatictestDlg* pDlg = (CDragDropStatictestDlg*)AfxGetMainWnd();
    if (pDlg && !filePaths.empty())
        pDlg->setData(filePaths);

    DragFinish(hDropInfo);
}

