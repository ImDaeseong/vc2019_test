#include "pch.h"
#include "BootTimerManager.h"
#include <winbase.h>

BootTimerManager::BootTimerManager()
{
}

CString BootTimerManager::GetBootTime()
{
    FILETIME ftSystem, ftLocal;
    SYSTEMTIME stLocal;
    CString bootTimeStr;

    // 시스템 부팅 시간 가져오기
    GetSystemTimeAsFileTime(&ftSystem);
    ULARGE_INTEGER ulSystem;
    ulSystem.LowPart = ftSystem.dwLowDateTime;
    ulSystem.HighPart = ftSystem.dwHighDateTime;

    // 시스템 가동 시간(밀리초) 가져오기
    ULARGE_INTEGER ulUptime;
    ulUptime.QuadPart = GetTickCount64();

    // 부팅 시간 계산
    ulSystem.QuadPart -= ulUptime.QuadPart * 10000; // 100나노초 단위로 변환

    // FILETIME으로 변환
    FILETIME ftBoot;
    ftBoot.dwLowDateTime = ulSystem.LowPart;
    ftBoot.dwHighDateTime = ulSystem.HighPart;

    // UTC를 로컬 시간(한국 시간)으로 변환
    FileTimeToLocalFileTime(&ftBoot, &ftLocal);

    // SYSTEMTIME으로 변환
    FileTimeToSystemTime(&ftLocal, &stLocal);

    // CString으로 포맷팅
    bootTimeStr.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
        stLocal.wYear,
        stLocal.wMonth,
        stLocal.wDay,
        stLocal.wHour,
        stLocal.wMinute,
        stLocal.wSecond);

    return bootTimeStr;
}