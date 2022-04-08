#include "pch.h"
#include "Common_Util.h"

Bitmap* Common_Util::Load(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
{
	Gdiplus::Bitmap* m_pBitmap = NULL;

	HRSRC hResource = ::FindResource(hInst, pName, pType);
	if (!hResource)
		return NULL;

	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)
		return NULL;

	const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
	if (!pResourceData)
		return NULL;

	HGLOBAL m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
	if (m_hBuffer)
	{
		void* pBuffer = ::GlobalLock(m_hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			::GlobalUnlock(pBuffer);

			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
			{
				m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
				pStream->Release();
				if (m_pBitmap)
				{
					if (m_pBitmap->GetLastStatus() == Gdiplus::Ok)
						return m_pBitmap;

					delete m_pBitmap;
					m_pBitmap = NULL;
				}
			}
		}
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}
	return NULL;
}

Bitmap* Common_Util::ResizeBitmap(Bitmap* pInImg, int nResizeWidth, int nResizeHeight)
{
	int nImgWidth = pInImg->GetWidth();
	int nIngHeight = pInImg->GetHeight();
	float fHor = (float)nResizeWidth / nImgWidth;
	float fVor = (float)nResizeHeight / nIngHeight;

	nResizeWidth = (int)(nImgWidth * fHor);
	nResizeHeight = (int)(nIngHeight * fVor);
	Bitmap* pCopyImg = new Bitmap(nResizeWidth, nResizeHeight, PixelFormat32bppARGB);
	Graphics graphic(pCopyImg);
	graphic.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	graphic.DrawImage(pInImg, Gdiplus::Rect(0, 0, nResizeWidth, nResizeHeight), 0, 0, nImgWidth, nIngHeight, Gdiplus::UnitPixel);
	graphic.Flush();
	return pCopyImg;
}

void Common_Util::SetBrowserEmulation(CString strFileName)
{
	LPTSTR lpControlRegKey = _T("SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION");

	int nValue = 11000;

	CRegKey	regKey;

	if (!IsWin7OsVersion())
	{
		if (ERROR_SUCCESS != regKey.Open(HKEY_LOCAL_MACHINE, lpControlRegKey))
		{
			regKey.Create(HKEY_LOCAL_MACHINE, lpControlRegKey);
		}
	}
	else
	{
		CString strSubKey;
		CString strSID = GetCurrentUserSID();
		strSubKey = strSID + _T("\\") + lpControlRegKey;
		if (ERROR_SUCCESS != regKey.Open(HKEY_USERS, strSubKey))
		{
			regKey.Create(HKEY_USERS, strSubKey);
		}
	}

	regKey.SetValue((DWORD)nValue, strFileName);

	regKey.Close();
}

BOOL Common_Util::IsWin7OsVersion()
{
	BOOL bResult = FALSE;

	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi);
	if (!bOsVersionInfoEx)
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO*)&osvi))
			return bResult;
	}

	switch (osvi.dwPlatformId)
	{

		// Test for the Windows NT product family.
	case VER_PLATFORM_WIN32_NT:

		// Window7
		if (osvi.dwMajorVersion >= 6 && osvi.dwMinorVersion >= 1)
			bResult = TRUE;
		break;

		// Test for the Windows 95 product family.
	case VER_PLATFORM_WIN32_WINDOWS:
		break;

	case VER_PLATFORM_WIN32s:
		break;
	}

	return bResult;
}

CString Common_Util::GetCurrentUserSID()
{
	CString strUserSID;

	strUserSID.Empty();

	DWORD dwRval = 0;

	HANDLE hTProcess;
	DWORD dwSize;

	// Process 핸들로 토큰을 얻는다. (windows7이므로, 될수 있으면 권한은 낮게... QUERY 권한만으로 접근)
	if (OpenProcessToken(GetExplorerProcessHandle(), TOKEN_QUERY, &hTProcess) == FALSE)
	{
		return strUserSID;
	}

	// NULL 값으로 토큰 정보를 요청하여,  토큰 정보가 저장될 버퍼의 크기를 받아온다.
	GetTokenInformation(hTProcess, TokenUser, NULL, 0, &dwSize);

	TOKEN_USER* sidUser = (TOKEN_USER*)malloc(dwSize);
	if (GetTokenInformation(hTProcess, TokenUser, sidUser, dwSize, &dwSize) == 0)
	{
		free(sidUser);
		CloseHandle(hTProcess);

		return strUserSID;
	}

	// SID는 문자열이 아니고 구조체형이다.
	// 문자형 SID로 변경한다.
	LPTSTR pszSid;
	ConvertSidToStringSid(sidUser[0].User.Sid, &pszSid);
	free(sidUser);
	CloseHandle(hTProcess);

	strUserSID.Format(_T("%s\\"), pszSid);
	LocalFree(pszSid);


	return strUserSID;
}

HANDLE Common_Util::GetExplorerProcessHandle()
{
	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	Process32First(snapshot, &proc);

	CString strFileName = proc.szExeFile;

	while (TRUE == Process32Next(snapshot, &proc))
	{
		strFileName = proc.szExeFile;

		strFileName.MakeLower();

		if (strFileName == "explorer.exe")
		{
			HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, proc.th32ProcessID);

			::CloseHandle(snapshot);

			return hProcess;
		}
	}

	::CloseHandle(snapshot);

	return NULL;
}
