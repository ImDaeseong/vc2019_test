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

    // �ý��� ���� �ð� ��������
    GetSystemTimeAsFileTime(&ftSystem);
    ULARGE_INTEGER ulSystem;
    ulSystem.LowPart = ftSystem.dwLowDateTime;
    ulSystem.HighPart = ftSystem.dwHighDateTime;

    // �ý��� ���� �ð�(�и���) ��������
    ULARGE_INTEGER ulUptime;
    ulUptime.QuadPart = GetTickCount64();

    // ���� �ð� ���
    ulSystem.QuadPart -= ulUptime.QuadPart * 10000; // 100������ ������ ��ȯ

    // FILETIME���� ��ȯ
    FILETIME ftBoot;
    ftBoot.dwLowDateTime = ulSystem.LowPart;
    ftBoot.dwHighDateTime = ulSystem.HighPart;

    // UTC�� ���� �ð�(�ѱ� �ð�)���� ��ȯ
    FileTimeToLocalFileTime(&ftBoot, &ftLocal);

    // SYSTEMTIME���� ��ȯ
    FileTimeToSystemTime(&ftLocal, &stLocal);

    // CString���� ������
    bootTimeStr.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
        stLocal.wYear,
        stLocal.wMonth,
        stLocal.wDay,
        stLocal.wHour,
        stLocal.wMinute,
        stLocal.wSecond);

    return bootTimeStr;
}