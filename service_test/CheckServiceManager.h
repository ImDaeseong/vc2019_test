#pragma once

#include <WinSvc.h>

class CheckServiceManager
{
public:
	BOOL IsInstallService();
	BOOL IsStopService();
	BOOL IsRunService();
	BOOL InstallIService(CString strPath);
	BOOL UninstallService();		
	BOOL StartService();
	BOOL StopService();
	DWORD GetServiceStatus();
};

