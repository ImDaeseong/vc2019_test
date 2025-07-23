#include "pch.h"
#include "ZlibHelper.h"

#include <atlconv.h>
#include <iostream>

#include <minizip-ng/mz.h>
#include <minizip-ng/mz_strm.h>
#include <minizip-ng/mz_strm_buf.h>
#include <minizip-ng/mz_strm_os.h>
#include <minizip-ng/mz_zip.h>
#include <minizip-ng/mz_zip_rw.h>

//추가포함 디렉토리
//C:\dev\vcpkg\installed\x86-windows\include

//추가 라이브러리 디렉토리
//C:\dev\vcpkg\installed\x86-windows\lib

//추가종속성
//zlib.lib
//minizip - ng.lib
//bcrypt.lib


CZlibHelper::CZlibHelper()
{
}

CZlibHelper::~CZlibHelper()
{
}

int CZlibHelper::CreateZip(const CString& zipPath, const CString& fileToAdd)
{
    std::vector<CString> files;
    files.push_back(fileToAdd);
    return CreateZipMultipleFiles(zipPath, files);
}

int CZlibHelper::CreateZipMultipleFiles(const CString& zipPath, const std::vector<CString>& filesToAdd)
{
    void* writer = mz_zip_writer_create();
    if (writer == nullptr)
    {
        //printf("Failed to create zip writer\n");
        return 1;
    }

    CT2A zipPathA(zipPath);
    const char* zipPathStr = (LPCSTR)zipPathA;

    if (mz_zip_writer_open_file(writer, zipPathStr, 0, 0) != MZ_OK)
    {
        //printf("Failed to open zip file for writing: %s\n", zipPathStr);
        mz_zip_writer_delete(&writer);
        return 1;
    }

    int totalFiles = (int)filesToAdd.size();
    int currentFile = 0;

    for (const auto& filePath : filesToAdd)
    {
        currentFile++;
        CString fileName = filePath.Mid(filePath.ReverseFind(L'\\') + 1);

        //NotifyProgress(currentFile, totalFiles, fileName);

        if (AddFileToZip(writer, filePath, fileName) != 0)
        {
            //printf("Failed to add file to zip: %ws\n", filePath.GetString());
            mz_zip_writer_close(writer);
            mz_zip_writer_delete(&writer);
            return 1;
        }
    }

    mz_zip_writer_close(writer);
    mz_zip_writer_delete(&writer);

    //printf("ZIP created successfully: %s\n", zipPathStr);
    return 0;
}

int CZlibHelper::CreateZipFromFolder(const CString& zipPath, const CString& folderPath)
{
    void* writer = mz_zip_writer_create();
    if (writer == nullptr)
    {
        //printf("Failed to create zip writer\n");
        return 1;
    }

    CT2A zipPathA(zipPath);
    const char* zipPathStr = (LPCSTR)zipPathA;

    if (mz_zip_writer_open_file(writer, zipPathStr, 0, 0) != MZ_OK)
    {
        //printf("Failed to open zip file for writing: %s\n", zipPathStr);
        mz_zip_writer_delete(&writer);
        return 1;
    }

    int result = AddFolderToZip(writer, folderPath, folderPath);

    mz_zip_writer_close(writer);
    mz_zip_writer_delete(&writer);

    if (result == 0)
    {
        //printf("ZIP created successfully from folder: %s\n", zipPathStr);
    }

    return result;
}

int CZlibHelper::ExtractZip(const CString& zipPath, const CString& extractFolder)
{
    void* reader = mz_zip_reader_create();
    if (reader == NULL)
    {
        //printf("Failed to create zip reader\n");
        return 1;
    }

    CT2A zipPathA(zipPath);
    const char* zipPathStr = (LPCSTR)zipPathA;

    int32_t openResult = mz_zip_reader_open_file(reader, zipPathStr);
    if (openResult != MZ_OK)
    {
        //printf("Failed to open zip file: %s, Error code: %d\n", zipPathStr, openResult);
        mz_zip_reader_delete(&reader);
        return 1;
    }

    if (!CreateDirectoriesRecursive(extractFolder))
    {
        //printf("Failed to create extraction folder: %ws\n", extractFolder.GetString());
        mz_zip_reader_close(reader);
        mz_zip_reader_delete(&reader);
        return 1;
    }

    // 전체 파일 수 계산
    int totalFiles = 0;
    int32_t result = mz_zip_reader_goto_first_entry(reader);
    while (result == MZ_OK) 
    {
        totalFiles++;
        result = mz_zip_reader_goto_next_entry(reader);
    }

    // 다시 첫 번째 엔트리로 이동
    result = mz_zip_reader_goto_first_entry(reader);
    if (result != MZ_OK)
    {
        //printf("Failed to go to the first entry\n");
        mz_zip_reader_close(reader);
        mz_zip_reader_delete(&reader);
        return 1;
    }

    int currentFile = 0;

    do {
        currentFile++;

        mz_zip_file* file_info = nullptr;
        if (mz_zip_reader_entry_get_info(reader, &file_info) != MZ_OK)
        {
            //printf("Failed to get entry info\n");
            result = mz_zip_reader_goto_next_entry(reader);
            continue;
        }

        if (file_info->filename == NULL)
        {
            result = mz_zip_reader_goto_next_entry(reader);
            continue;
        }

        CString fileNameInZipW = CA2W(file_info->filename);
        fileNameInZipW.Replace(L'/', L'\\');

        //NotifyProgress(currentFile, totalFiles, fileNameInZipW);

        CString extractPath = extractFolder;
        if (extractPath.GetLength() > 0 && extractPath.GetAt(extractPath.GetLength() - 1) != L'\\') {
            extractPath += L"\\";
        }
        extractPath += fileNameInZipW;

        bool isDirectory = (strlen(file_info->filename) > 0 &&
            (file_info->filename[strlen(file_info->filename) - 1] == '/' ||
                file_info->filename[strlen(file_info->filename) - 1] == '\\')) ||
            file_info->uncompressed_size == 0;

        if (isDirectory)
        {
            CreateDirectoriesRecursive(extractPath);
            result = mz_zip_reader_goto_next_entry(reader);
            continue;
        }

        // 상위 디렉토리 생성
        int pos = extractPath.ReverseFind(L'\\');
        if (pos >= 0)
        {
            CString dirPath = extractPath.Left(pos);
            CreateDirectoriesRecursive(dirPath);
        }

        // 파일 추출
        HANDLE hFile = CreateFile(extractPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            //printf("Failed to create output file: %ws\n", extractPath.GetString());
            result = mz_zip_reader_goto_next_entry(reader);
            continue;
        }

        if (mz_zip_reader_entry_open(reader) != MZ_OK)
        {
            //printf("Failed to open entry for reading: %ws\n", fileNameInZipW.GetString());
            CloseHandle(hFile);
            result = mz_zip_reader_goto_next_entry(reader);
            continue;
        }

        const int bufSize = 8192;
        char buffer[bufSize];
        DWORD totalBytesWritten = 0;

        while (true)
        {
            int32_t bytesRead = mz_zip_reader_entry_read(reader, buffer, bufSize);
            if (bytesRead < 0)
            {
                //printf("Failed to read entry data\n");
                break;
            }
            if (bytesRead == 0) break;

            DWORD written = 0;
            if (!WriteFile(hFile, buffer, bytesRead, &written, NULL) || written != (DWORD)bytesRead)
            {
                //printf("Failed to write output file\n");
                break;
            }
            totalBytesWritten += written;
        }

        mz_zip_reader_entry_close(reader);
        CloseHandle(hFile);

        result = mz_zip_reader_goto_next_entry(reader);

    } while (result == MZ_OK);

    mz_zip_reader_close(reader);
    mz_zip_reader_delete(&reader);

    //printf("Extraction completed successfully\n");
    return 0;
}

int CZlibHelper::GetZipFileList(const CString& zipPath, std::vector<CString>& fileList)
{
    fileList.clear();

    void* reader = mz_zip_reader_create();
    if (reader == NULL) return 1;

    CT2A zipPathA(zipPath);
    if (mz_zip_reader_open_file(reader, (LPCSTR)zipPathA) != MZ_OK)
    {
        mz_zip_reader_delete(&reader);
        return 1;
    }

    int32_t result = mz_zip_reader_goto_first_entry(reader);
    while (result == MZ_OK)
    {
        mz_zip_file* file_info = nullptr;
        if (mz_zip_reader_entry_get_info(reader, &file_info) == MZ_OK && file_info->filename != NULL)
        {
            fileList.push_back(CString(CA2W(file_info->filename)));
        }
        result = mz_zip_reader_goto_next_entry(reader);
    }

    mz_zip_reader_close(reader);
    mz_zip_reader_delete(&reader);
    return 0;
}

int CZlibHelper::ExtractSpecificFile(const CString& zipPath, const CString& fileNameInZip, const CString& outputPath)
{
    void* reader = mz_zip_reader_create();
    if (reader == NULL) return 1;

    CT2A zipPathA(zipPath);
    if (mz_zip_reader_open_file(reader, (LPCSTR)zipPathA) != MZ_OK)
    {
        mz_zip_reader_delete(&reader);
        return 1;
    }

    CT2A fileNameA(fileNameInZip);
    int32_t result = mz_zip_reader_locate_entry(reader, (LPCSTR)fileNameA, 0);
    if (result != MZ_OK)
    {
        //printf("File not found in zip: %ws\n", fileNameInZip.GetString());
        mz_zip_reader_close(reader);
        mz_zip_reader_delete(&reader);
        return 1;
    }

    // 출력 디렉토리 생성
    int pos = outputPath.ReverseFind(L'\\');
    if (pos >= 0)
    {
        CString dirPath = outputPath.Left(pos);
        CreateDirectoriesRecursive(dirPath);
    }

    HANDLE hFile = CreateFile(outputPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        mz_zip_reader_close(reader);
        mz_zip_reader_delete(&reader);
        return 1;
    }

    if (mz_zip_reader_entry_open(reader) != MZ_OK)
    {
        CloseHandle(hFile);
        mz_zip_reader_close(reader);
        mz_zip_reader_delete(&reader);
        return 1;
    }

    const int bufSize = 8192;
    char buffer[bufSize];

    while (true)
    {
        int32_t bytesRead = mz_zip_reader_entry_read(reader, buffer, bufSize);
        if (bytesRead <= 0) break;

        DWORD written = 0;
        WriteFile(hFile, buffer, bytesRead, &written, NULL);
    }

    mz_zip_reader_entry_close(reader);
    CloseHandle(hFile);
    mz_zip_reader_close(reader);
    mz_zip_reader_delete(&reader);

    return 0;
}

BOOL CZlibHelper::CreateDirectoriesRecursive(LPCTSTR lpszPath)
{
    if (lpszPath == NULL || *lpszPath == _T('\0')) return FALSE;

    LPTSTR pszPathCopy = _tcsdup(lpszPath);
    if (pszPathCopy == NULL) return FALSE;

    LPTSTR p = pszPathCopy;

    if (_istalpha(*p) && *(p + 1) == _T(':')) p += 2;
    while (*p == _T('\\') || *p == _T('/')) p++;

    while (*p != _T('\0'))
    {
        while (*p != _T('\\') && *p != _T('/') && *p != _T('\0')) p++;

        TCHAR c = *p;
        *p = _T('\0');

        if (!CreateDirectory(pszPathCopy, NULL))
        {
            DWORD dwError = GetLastError();
            if (dwError != ERROR_ALREADY_EXISTS)
            {
                free(pszPathCopy);
                return FALSE;
            }
        }

        *p = c;
        if (c != _T('\0')) p++;
    }

    free(pszPathCopy);
    return TRUE;
}

int CZlibHelper::AddFileToZip(void* writer, const CString& filePath, const CString& fileNameInZip)
{
    HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) return 1;

    DWORD fileSize = GetFileSize(hFile, nullptr);
    if (fileSize == INVALID_FILE_SIZE)
    {
        CloseHandle(hFile);
        return 1;
    }

    char* buffer = new char[fileSize];
    DWORD bytesRead = 0;
    if (!ReadFile(hFile, buffer, fileSize, &bytesRead, nullptr) || bytesRead != fileSize)
    {
        delete[] buffer;
        CloseHandle(hFile);
        return 1;
    }
    CloseHandle(hFile);

    CT2A fileNameInZipA(fileNameInZip);
    mz_zip_file file_info;
    memset(&file_info, 0, sizeof(file_info));
    file_info.filename = (LPCSTR)fileNameInZipA;
    file_info.compression_method = MZ_COMPRESS_METHOD_DEFLATE;

    int result = (mz_zip_writer_add_buffer(writer, buffer, fileSize, &file_info) == MZ_OK) ? 0 : 1;
    delete[] buffer;
    return result;
}

int CZlibHelper::AddFolderToZip(void* writer, const CString& folderPath, const CString& basePath)
{
    std::vector<CString> files;
    GetFilesInFolder(folderPath, files, TRUE);

    int totalFiles = (int)files.size();
    int currentFile = 0;

    for (const auto& filePath : files)
    {
        currentFile++;
        CString relativePath = GetRelativePath(filePath, basePath);
        relativePath.Replace(L'\\', L'/'); // ZIP 내부에서는 / 사용

        //NotifyProgress(currentFile, totalFiles, relativePath);

        if (AddFileToZip(writer, filePath, relativePath) != 0)
        {
            return 1;
        }
    }

    return 0;
}

void CZlibHelper::GetFilesInFolder(const CString& folderPath, std::vector<CString>& files, BOOL recursive)
{
    CString searchPath = folderPath;
    if (searchPath.GetLength() > 0 && searchPath.GetAt(searchPath.GetLength() - 1) != L'\\') {
        searchPath += L"\\";
    }
    searchPath += L"*.*";

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0) {
            continue;
        }

        CString filePath = folderPath;
        if (filePath.GetLength() > 0 && filePath.GetAt(filePath.GetLength() - 1) != L'\\') {
            filePath += L"\\";
        }
        filePath += findData.cFileName;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (recursive) {
                GetFilesInFolder(filePath, files, recursive);
            }
        }
        else {
            files.push_back(filePath);
        }
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
}

CString CZlibHelper::GetRelativePath(const CString& fullPath, const CString& basePath)
{
    CString relativePath = fullPath;
    if (relativePath.Find(basePath) == 0) {
        relativePath = relativePath.Mid(basePath.GetLength());
        if (relativePath.GetLength() > 0 && relativePath.GetAt(0) == L'\\') {
            relativePath = relativePath.Mid(1);
        }
    }
    return relativePath;
}
