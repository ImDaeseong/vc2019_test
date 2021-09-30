#include "pch.h"
#include "framework.h"
#include "steamapps.h"
#include "steamappsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CsteamappsDlg::CsteamappsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STEAMAPPS_DIALOG, pParent)
{	
}

void CsteamappsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CsteamappsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CsteamappsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CsteamappsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CsteamappsDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CsteamappsDlg::OnBnClickedButton4)
END_MESSAGE_MAP()

BOOL CsteamappsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDlgItemText(IDC_EDIT_ID, _T("���̵� �Է��ϼ���"));
	SetDlgItemText(IDC_EDIT_PASSWORD, _T("��й�ȣ�� �Է��ϼ���"));

	return TRUE;  
}

void CsteamappsDlg::OnPaint()
{
	CPaintDC dc(this);
}

//���� �α���
void CsteamappsDlg::OnBnClickedButton1()
{
	//���� ��ġ ��� Ȯ��
	RegInfo reg;
	CString strPath = reg.IsExistSteamInfo();
	if (strPath.IsEmpty())
		return;


	//���� ���� ���� ���
	CString strAppExe = strPath + _T("\\steam.exe");


	//���̵�
	CString strID = _T("");
	GetDlgItemText(IDC_EDIT_ID, strID);


	//��й�ȣ
	CString strPassword = _T("");
	GetDlgItemText(IDC_EDIT_PASSWORD, strPassword);


	//�α��� ������ ���� ���
	if (strID == "" || strPassword == "")
	{
		::ShellExecute(NULL, _T("open"), strAppExe, NULL, NULL, SW_SHOW);
		return;
	}

	//���� �������� �α����ϱ�
	CString strParams;
	strParams.Format(_T("-login %s %s"), strID, strPassword);
	::ShellExecute(NULL, _T("open"), strAppExe, strParams, NULL, SW_SHOW);
}

//���� �α׾ƿ�
void CsteamappsDlg::OnBnClickedButton2()
{
	//���� ��ġ ��� Ȯ��
	RegInfo reg;
	CString strPath = reg.IsExistSteamInfo();
	if (strPath.IsEmpty())
		return;

	CString strAppExe;
	strAppExe.Format(_T("%s\\steam.exe"), strPath);
	::ShellExecute(NULL, _T("open"), strAppExe, _T("-shutdown"), NULL, SW_SHOW);
}

//���� ���� ��ġ ���
void CsteamappsDlg::OnBnClickedButton3()
{
	m_AppInfos.clear();

	//���� ��ġ ��� Ȯ��
	RegInfo reg;
	CString strPath = reg.IsExistSteamInfo();
	if (strPath.IsEmpty())
		return;

	//���� ��ġ ���� ��� Ȯ��
	CString strSteamAppsPath = strPath + _T("\\steamapps");
	CString strWildcard = strSteamAppsPath;
	strWildcard += _T("\\*.acf");

	//���� ���� ã��
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
			continue;

		CString strFullPath = finder.GetFilePath();
		CStdioFile stdFile(strFullPath, CFile::modeRead | CFile::typeText);
		CString strLine, strAppid, strAppname;

		while (stdFile.ReadString(strLine))
		{
			if (strLine.Find(_T("appid")) > 0)
			{
				strAppid = strLine;
				strAppid.Replace(_T("appid"), _T(""));
				strAppid.Replace(_T("\""), _T(""));
				strAppid.Replace(_T("\r"), _T(""));
				strAppid.Replace(_T("\n"), _T(""));
				strAppid.Replace(_T("\t"), _T(""));
				strAppid.Trim();
			}
			else if (strLine.Find(_T("name")) > 0)
			{
				strAppname = strLine;
				strAppname.Replace(_T("name"), _T(""));
				strAppname.Replace(_T("\""), _T(""));
				strAppname.Replace(_T("\r"), _T(""));
				strAppname.Replace(_T("\n"), _T(""));
				strAppname.Replace(_T("\t"), _T(""));
				strAppname.Trim();
			}

			if (!strAppid.IsEmpty() && !strAppname.IsEmpty())
			{
				AppInfo AppInfos;
				AppInfos.strAppid = strAppid;
				AppInfos.strAppname = strAppname;
				m_AppInfos.push_back(AppInfos);
				strAppid = _T("");
				strAppname = _T("");
				break;
			}

		}
		stdFile.Close();

	}
	finder.Close();
}

//���� ���� ����
void CsteamappsDlg::OnBnClickedButton4()
{
	CString strGamename = _T("FortressV2");
	CString strAppID = _T("");

	for (int i = 0; i < m_AppInfos.size(); i++)
	{
		if (m_AppInfos[i].strAppname == strGamename)
		{
			strAppID = m_AppInfos[i].strAppid;
			break;
		}
	}

	if (!strAppID.IsEmpty())
	{
		CString strParam;
		strParam.Format(_T("steam://rungameid/%s"), strAppID);
		::ShellExecute(NULL, _T("open"), strParam, NULL, NULL, SW_SHOW);
	}	
}
