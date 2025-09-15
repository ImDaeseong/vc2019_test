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
//minizip-ng.lib
//bcrypt.lib

CZlibHelper::CZlibHelper()
{
}

CZlibHelper::~CZlibHelper()
{
}

int CZlibHelper::CreateZipA(const CString& zipPath, const CString& fileToAdd)
{
    USES_CONVERSION;

    void* writer = mz_zip_writer_create();
    if (writer == nullptr)
    {
        //printf("Failed to create zip writer\n");
        return 1;
    }

    // Convert zip path to UTF-8
    CT2A zipPathUtf8(zipPath, CP_UTF8);
    if (mz_zip_writer_open_file(writer, (const char*)zipPathUtf8, 0, 0) != MZ_OK)
    {
        //printf("Failed to open zip file for writing: %s\n", zipPathStr);
        mz_zip_writer_delete(&writer);
        return 1;
    }

    CString fileName = fileToAdd.Mid(fileToAdd.ReverseFind(L'\\') + 1);

    int result = AddFileToZip(writer, fileToAdd, fileName);

    mz_zip_writer_close(writer);
    mz_zip_writer_delete(&writer);

    if (result == 0)
    {
        //printf("ZIP created successfully from folder: %s\n", zipPathStr);
    }

    return result;
}

int CZlibHelper::CreateZip(const CString& zipPath, const CString& fileToAdd)
{
    std::vector<CString> files;
    files.push_back(fileToAdd);
    return CreateZipMultipleFiles(zipPath, files);
}

int CZlibHelper::CreateZipMultipleFiles(const CString& zipPath, const std::vector<CString>& filesToAdd)
{
    USES_CONVERSION;

    void* writer = mz_zip_writer_create();
    if (writer == nullptr)
    {
        //printf("Failed to create zip writer\n");
        return 1;
    }

    // Convert zip path to UTF-8
    CT2A zipPathUtf8(zipPath, CP_UTF8);
    if (mz_zip_writer_open_file(writer, (const char*)zipPathUtf8, 0, 0) != MZ_OK)
    {
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
    USES_CONVERSION;

    void* writer = mz_zip_writer_create();
    if (writer == nullptr)
    {
        //printf("Failed to create zip writer\n");
        return 1;
    }

    // Convert zip path to UTF-8
    CT2A zipPathUtf8(zipPath, CP_UTF8);
    if (mz_zip_writer_open_file(writer, (const char*)zipPathUtf8, 0, 0) != MZ_OK)
    {
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
    USES_CONVERSION;

    void* reader = mz_zip_reader_create();
    if (reader == NULL)
    {
        //printf("Failed to create zip reader\n");
        return 1;
    }

    // Convert zip path to UTF-8
    CT2A zipPathUtf8(zipPath, CP_UTF8);
    int32_t openResult = mz_zip_reader_open_file(reader, (const char*)zipPathUtf8);
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
    USES_CONVERSION;

    fileList.clear();

    void* reader = mz_zip_reader_create();
    if (reader == NULL) return 1;

    // Convert zip path to UTF-8
    CT2A zipPathUtf8(zipPath, CP_UTF8);
    if (mz_zip_reader_open_file(reader, (const char*)zipPathUtf8) != MZ_OK)
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
            CString fileName;
            fileName = CString(CA2W(file_info->filename, CP_UTF8));
            if (fileName.GetLength() > 0)
            {
                fileList.push_back(fileName);
            }
        }
        result = mz_zip_reader_goto_next_entry(reader);
    }

    mz_zip_reader_close(reader);
    mz_zip_reader_delete(&reader);
    return 0;
}

int CZlibHelper::ExtractSpecificFile(const CString& zipPath, const CString& fileNameInZip, const CString& outputPath)
{
    USES_CONVERSION;

    void* reader = mz_zip_reader_create();
    if (reader == NULL) return 1;

    // Convert zip path to UTF-8
    CT2A zipPathUtf8(zipPath, CP_UTF8);
    if (mz_zip_reader_open_file(reader, (const char*)zipPathUtf8) != MZ_OK)
    {
        mz_zip_reader_delete(&reader);
        return 1;
    }

    CT2A fileNameUtf8(fileNameInZip, CP_UTF8);
    int32_t result = mz_zip_reader_locate_entry(reader, (const char*)fileNameUtf8, 0);
    if (result != MZ_OK)
    {
        //printf("File not found in zip: %ws\n", fileNameInZip.GetString());
        mz_zip_reader_close(reader);
        mz_zip_reader_delete(&reader);
        return 1;
    }

    // 출력 디렉토리 생성 - 혼합된 구분자 처리
    int pos1 = outputPath.ReverseFind(L'\\');
    int pos2 = outputPath.ReverseFind(L'/');
    int pos = max(pos1, pos2);  // 더 뒤에 있는 구분자 위치 사용
    if (pos >= 0)
    {
        CString dirPath = outputPath.Left(pos);
        CreateDirectoriesRecursive(dirPath);
    }
    
    //기존 파일이 읽기 전용이거나 사용 중일 수 있으므로 먼저 속성 변경 시도
    if (PathFileExists(outputPath))
    {
        SetFileAttributes(outputPath, FILE_ATTRIBUTE_NORMAL);
    }

    HANDLE hFile = CreateFile(outputPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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
    if (lpszPath == NULL || *lpszPath == _T('\0'))
        return FALSE;

    // 이미 존재하는지 확인
    if (PathFileExists(lpszPath))
        return TRUE;

    CString strPath(lpszPath);
    int nPos = strPath.ReverseFind(_T('\\'));
    if (nPos == -1)
        nPos = strPath.ReverseFind(_T('/'));

    if (nPos > 0)
    {
        // 부모 디렉토리 먼저 생성
        CString strParent = strPath.Left(nPos);
        if (!CreateDirectoriesRecursive(strParent))
            return FALSE;
    }

    // 현재 디렉토리 생성
    if (!CreateDirectory(lpszPath, NULL))
    {
        DWORD dwError = GetLastError();
        if (dwError != ERROR_ALREADY_EXISTS)
        {
            return FALSE;
        }
    }

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

    //파일 정보
    mz_zip_file file_info;
    memset(&file_info, 0, sizeof(file_info));

    //압축 파일 날짜 생성 - 그냥 압축시점 날짜로 설정
    const auto time = std::time(nullptr);
    file_info.creation_date = time;
    file_info.modified_date = time;

    CloseHandle(hFile);

    // UTF-8로 변환
    CT2A fileNameInZipUtf8(fileNameInZip, CP_UTF8);    
    file_info.filename = (const char*)fileNameInZipUtf8;
    file_info.compression_method = MZ_COMPRESS_METHOD_DEFLATE;
    file_info.flag |= MZ_ZIP_FLAG_UTF8; //UTF-8 플래그 설정

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
