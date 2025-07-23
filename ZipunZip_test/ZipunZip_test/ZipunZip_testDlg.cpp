#include "pch.h"
#include "framework.h"
#include "ZipunZip_test.h"
#include "ZipunZip_testDlg.h"
#include "afxdialogex.h"
#include "ZlibHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CZipunZiptestDlg::CZipunZiptestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ZIPUNZIP_TEST_DIALOG, pParent)
{
}

void CZipunZiptestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CZipunZiptestDlg, CDialogEx)
	ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BUTTON1, &CZipunZiptestDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CZipunZiptestDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CZipunZiptestDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

BOOL CZipunZiptestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  
}
void CZipunZiptestDlg::OnPaint()
{
    CPaintDC dc(this);
}

void zip1()
{
    //압축
    CString zipPath = L"C:\\temp\\NeoPicker.zip";
    CString fileToAdd = L"E:\\Util\\NeoPicker.exe";

    CZlibHelper zipHelper;
    int result = zipHelper.CreateZip(zipPath, fileToAdd);
    if (result == 0)
    {
        OutputDebugString(L"압축 성공\n");
    }
    else
    {
        OutputDebugString(L"압축 실패\n");
    }
}

void unzip1()
{
    //압축 해제
    CString zipPath_unzip = L"C:\\temp\\NeoPicker.zip"; // 압축 해제할 ZIP 파일 경로
    CString extractFolder = L"C:\\temp"; // 압축 해제될 폴더 경로

    CZlibHelper zipHelper;
    int result = zipHelper.ExtractZip(zipPath_unzip, extractFolder);
    if (result == 0)
    {
        OutputDebugString(L"압축 해제 성공\n");
    }
    else
    {
        OutputDebugString(L"압축 해제 실패\n");
    }
}

void CZipunZiptestDlg::OnBnClickedButton1()
{
    //압축
    zip1();
    
    //압축 해제
    unzip1();
}

void zipMulit()
{
    //여러개 파일 압축
    CString zipPath = L"C:\\temp\\Multifile.zip";
    CString fileToAdd1 = L"E:\\Util\\NeoPicker.exe";
    CString fileToAdd2 = L"E:\\Util\\7z2409-x64.exe";
    
    std::vector<CString> files;
    files.push_back(fileToAdd1);
    files.push_back(fileToAdd2);
       
    CZlibHelper zipHelper;
    int result = zipHelper.CreateZipMultipleFiles(zipPath, files);
    if (result == 0)
    {
        OutputDebugString(L"여러개 압축 성공\n");
    }
    else
    {
        OutputDebugString(L"여러개 압축 실패\n");
    }
}

void unzipMulit()
{
    //압축 해제
    CString zipPath_unzip = L"C:\\temp\\Multifile.zip"; // 압축 해제할 ZIP 파일 경로
    CString extractFolder = L"C:\\temp"; // 압축 해제될 폴더 경로

    CZlibHelper zipHelper;

    std::vector<CString> fileList;
    zipHelper.GetZipFileList(zipPath_unzip, fileList);

    for (const auto& file : fileList)
    {
        //wprintf(L"파일: %s\n", file.GetString());

        CString strPath = extractFolder + L"\\" + file;

        int result = zipHelper.ExtractSpecificFile(zipPath_unzip, file, strPath);
        if (result == 0)
        {
            OutputDebugString(L"여러개 압축 해제 성공\n");
        }
        else
        {
            OutputDebugString(L"여러개 압축 해제 실패\n");
        }
    }
}

void CZipunZiptestDlg::OnBnClickedButton2()
{
    //여러개 파일 압축
    zipMulit();
   
    //압축 해제
    unzipMulit();   
}

void zipFolder()
{
    //폴더 압축
    CString zipPath = L"C:\\temp\\folder.zip";
    CString fileToAdd = L"E:\\Util\\Fiddler";

    CZlibHelper zipHelper;
    int result = zipHelper.CreateZipFromFolder(zipPath, fileToAdd);
    if (result == 0)
    {
        OutputDebugString(L"폴더 압축 성공\n");
    }
    else
    {
        OutputDebugString(L"폴더 압축 실패\n");
    }
}

void unzipFolder()
{
    //압축 해제
    CString zipPath_unzip = L"C:\\temp\\folder.zip"; // 압축 해제할 ZIP 파일 경로
    CString extractFolder = L"C:\\temp\\Fiddler"; // 압축 해제될 폴더 경로

    CZlibHelper zipHelper;
    int result = zipHelper.ExtractZip(zipPath_unzip, extractFolder);
    if (result == 0)
    {
        OutputDebugString(L"폴더 압축 해제 성공\n");
    }
    else
    {
        OutputDebugString(L"폴더 압축 해제 실패\n");
    }
}

void CZipunZiptestDlg::OnBnClickedButton3()
{
    //폴더 압축
    zipFolder();

    //폴더 압축 해제
    unzipFolder();
}
