#include "pch.h"
#include "CheckServiceManager.h"

LPCTSTR lpstrServiceName = _T("DaeSeongService");
LPCTSTR lpstrServiceDisplay = _T("DaeSeong Service Desc");

BOOL CheckServiceManager::IsInstallService()
{
	SC_HANDLE hScm = NULL;
	SC_HANDLE hSrv = NULL;

	hScm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hScm == NULL)
		return FALSE;

	hSrv = OpenService(hScm, lpstrServiceName, SERVICE_QUERY_STATUS);
	if (hSrv == NULL)
	{
		CloseServiceHandle(hScm);
		return FALSE;
	}

	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);

	return TRUE;
}

BOOL CheckServiceManager::IsStopService()
{
	BOOL bStop = FALSE;
	SC_HANDLE hScm = NULL;
	SC_HANDLE hSrv = NULL;
	SERVICE_STATUS status;

	hScm = OpenSCManagerW(0, 0, SC_MANAGER_CONNECT);
	if (hScm == NULL)
		return FALSE;

	hSrv = OpenServiceW(hScm, lpstrServiceName, SERVICE_QUERY_STATUS);
	if (hSrv == NULL) {

		CloseServiceHandle(hScm);
		return FALSE;
	}

	if (QueryServiceStatus(hSrv, &status) == TRUE) {

		if (status.dwCurrentState == SERVICE_STOPPED) {

			bStop = TRUE;
		}
	}
	else {

		OutputDebugString(_T("QueryServiceStatus failed"));
	}

	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);

	return bStop;
}

BOOL CheckServiceManager::IsRunService()
{
	BOOL bRun = FALSE;
	SC_HANDLE hScm = NULL;
	SC_HANDLE hSrv = NULL;
	SERVICE_STATUS status;

	hScm = OpenSCManagerW(0, 0, SC_MANAGER_CONNECT);
	if (hScm == NULL)
		return FALSE;

	hSrv = OpenServiceW(hScm, lpstrServiceName, SERVICE_QUERY_STATUS);
	if (hSrv == NULL) {

		CloseServiceHandle(hScm);
		return FALSE;
	}

	if (QueryServiceStatus(hSrv, &status) == TRUE) {

		if (status.dwCurrentState == SERVICE_RUNNING) {

			bRun = TRUE;
		}
	}
	else {

		OutputDebugString(_T("QueryServiceStatus failed"));
	}

	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);

	return bRun;
}

BOOL CheckServiceManager::InstallIService(CString strPath)
{
	SC_HANDLE hScm = NULL;
	SC_HANDLE hSrv = NULL;

	hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
	if (hScm == NULL)
		return FALSE;

	hSrv = CreateService(hScm, lpstrServiceName, lpstrServiceDisplay,
		SERVICE_ALL_ACCESS,	SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START, //서비스 자동 실행
		//SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_DEMAND_START, //서비스 수동 실행
		SERVICE_ERROR_IGNORE, strPath, NULL, NULL, NULL, NULL, NULL);

	if (hSrv == NULL)
	{
		CloseServiceHandle(hScm);
		return FALSE;
	}

	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);

	return TRUE;
}

BOOL CheckServiceManager::UninstallService()
{
	SC_HANDLE hScm = NULL;
	SC_HANDLE hSrv = NULL;
	SERVICE_STATUS status;

	hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (hScm == NULL)
		return FALSE;

	hSrv = OpenService(hScm, lpstrServiceName, DELETE | SERVICE_STOP | SERVICE_QUERY_STATUS);
	if (hSrv == NULL)
	{
		CloseServiceHandle(hScm);
		return FALSE;
	}

	if (QueryServiceStatus(hSrv, &status) == FALSE) {

		CloseServiceHandle(hSrv);
		CloseServiceHandle(hScm);
		return FALSE;
	}

	if (status.dwCurrentState != SERVICE_STOPPED) {

		if (ControlService(hSrv, SERVICE_CONTROL_STOP, &status) == FALSE) {

			CloseServiceHandle(hSrv);
			CloseServiceHandle(hScm);
			return FALSE;
		}
	}

	DeleteService(hSrv);

	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);

	return TRUE;
}

BOOL CheckServiceManager::StartService()
{
	BOOL bStart = TRUE;
	SC_HANDLE hScm = NULL;
	SC_HANDLE hSrv = NULL;

	hScm = OpenSCManagerW(0, 0, SC_MANAGER_CONNECT);
	if (hScm == NULL)
		return FALSE;

	hSrv = OpenServiceW(hScm, lpstrServiceName, SERVICE_START);
	if (hSrv == NULL) {

		CloseServiceHandle(hScm);
		return FALSE;
	}

	if (StartServiceA(hSrv, 0, NULL) == FALSE) {

		bStart = FALSE;
		OutputDebugString(_T("StartService failed"));
	}

	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);

	return bStart;
}

BOOL CheckServiceManager::StopService()
{
	SC_HANDLE hScm = NULL;
	SC_HANDLE hSrv = NULL;
	SERVICE_STATUS status;

	hScm = OpenSCManagerW(0, 0, SC_MANAGER_CONNECT);
	if (hScm == NULL)
		return FALSE;

	hSrv = OpenServiceW(hScm, lpstrServiceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
	if (hSrv == NULL) {

		CloseServiceHandle(hScm);
		return FALSE;
	}

	if (QueryServiceStatus(hSrv, &status) == FALSE) {

		CloseServiceHandle(hSrv);
		CloseServiceHandle(hScm);
		return FALSE;
	}

	if (status.dwCurrentState != SERVICE_STOPPED) {

		if (ControlService(hSrv, SERVICE_CONTROL_STOP, &status) == FALSE) {

			CloseServiceHandle(hSrv);
			CloseServiceHandle(hScm);
			return FALSE;
		}
	}

	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);

	return TRUE;
}

DWORD CheckServiceManager::GetServiceStatus()
{
	SC_HANDLE hScm = NULL;
	SC_HANDLE hSrv = NULL;
	SERVICE_STATUS status;

	hScm = OpenSCManagerW(0, 0, SC_MANAGER_CONNECT);
	if (hScm == NULL)
		return FALSE;

	hSrv = OpenServiceW(hScm, lpstrServiceName, SERVICE_QUERY_STATUS);
	if (hSrv == NULL) {

		CloseServiceHandle(hScm);
		return FALSE;
	}

	if (QueryServiceStatus(hSrv, &status) == FALSE) {

		CloseServiceHandle(hSrv);
		CloseServiceHandle(hScm);
		return FALSE;
	}

	DWORD dwStatus = status.dwCurrentState;
	return dwStatus;
}