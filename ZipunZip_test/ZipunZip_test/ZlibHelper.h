#pragma once
#include <atlstr.h>
#include <vector>

class CZlibHelper
{
public:
    CZlibHelper();
    ~CZlibHelper();

    // ���� ������ ZIP���� ����
    int CreateZip(const CString& zipPath, const CString& fileToAdd);

    // ���� ������ ZIP���� ����
    int CreateZipMultipleFiles(const CString& zipPath, const std::vector<CString>& filesToAdd);

    // ���� ��ü�� ZIP���� ����
    int CreateZipFromFolder(const CString& zipPath, const CString& folderPath);

    // ZIP ���� ���� ����
    int ExtractZip(const CString& zipPath, const CString& extractFolder);

    // ZIP ���� ���� ��� ��ȸ
    int GetZipFileList(const CString& zipPath, std::vector<CString>& fileList);

    // Ư�� ���ϸ� ���� ����
    int ExtractSpecificFile(const CString& zipPath, const CString& fileNameInZip, const CString& outputPath);

private:
    BOOL CreateDirectoriesRecursive(LPCTSTR lpszPath);
    int AddFileToZip(void* writer, const CString& filePath, const CString& fileNameInZip);
    int AddFolderToZip(void* writer, const CString& folderPath, const CString& basePath);
    void GetFilesInFolder(const CString& folderPath, std::vector<CString>& files, BOOL recursive = TRUE);
    CString GetRelativePath(const CString& fullPath, const CString& basePath);
};

