#include "pch.h"
#include "tiemInfo.h"

CString tiemInfo::GetCurrentTime()
{
	CString strTime = _T("");

	COleDateTime oleNow = COleDateTime::GetCurrentTime();

	strTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"), 
		oleNow.GetYear(), oleNow.GetMonth(), oleNow.GetDay(),
		oleNow.GetHour(), oleNow.GetMinute(), oleNow.GetSecond());

	return strTime;
}