#pragma once
#include <atlstr.h>
#include <vector>

class CZlibHelper
{
public:
    CZlibHelper();
    ~CZlibHelper();

    // 단일 파일을 ZIP으로 압축
    int CreateZip(const CString& zipPath, const CString& fileToAdd);

    // 여러 파일을 ZIP으로 압축
    int CreateZipMultipleFiles(const CString& zipPath, const std::vector<CString>& filesToAdd);

    // 폴더 전체를 ZIP으로 압축
    int CreateZipFromFolder(const CString& zipPath, const CString& folderPath);

    // ZIP 파일 압축 해제
    int ExtractZip(const CString& zipPath, const CString& extractFolder);

    // ZIP 파일 내용 목록 조회
    int GetZipFileList(const CString& zipPath, std::vector<CString>& fileList);

    // 특정 파일만 압축 해제
    int ExtractSpecificFile(const CString& zipPath, const CString& fileNameInZip, const CString& outputPath);

private:
    BOOL CreateDirectoriesRecursive(LPCTSTR lpszPath);
    int AddFileToZip(void* writer, const CString& filePath, const CString& fileNameInZip);
    int AddFolderToZip(void* writer, const CString& folderPath, const CString& basePath);
    void GetFilesInFolder(const CString& folderPath, std::vector<CString>& files, BOOL recursive = TRUE);
    CString GetRelativePath(const CString& fullPath, const CString& basePath);
};

