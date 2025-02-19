#include "pch.h"
#include "Common_Util.h"

BOOL Common_Util::GetToastEnabled()
{
	HKEY hKey = NULL;
	DWORD dwData = 0;
	DWORD dwSize = sizeof(DWORD);
	BOOL bResult = FALSE;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\PushNotifications"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, TEXT("ToastEnabled"), NULL, NULL, (LPBYTE)&dwData, &dwSize) == ERROR_SUCCESS)
		{
			bResult = (dwData == 1);
		}
		RegCloseKey(hKey); 
	}

	return bResult;
}

void Common_Util::OpenWindowsSettingURI(int nType)
{
	switch (nType)
	{
	case 1: //���ȭ��
		::ShellExecute(NULL, _T("open"), _T("ms-settings:lockscreen"), NULL, NULL, SW_SHOW);
		break;

	case 2: //���콺
		::ShellExecute(NULL, _T("open"), _T("ms-settings:mousetouchpad"), NULL, NULL, SW_SHOW);
		break;

	case 3: //Windows ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:"), NULL, NULL, SW_SHOW);
		break;

	case 4: //����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:about"), NULL, NULL, SW_SHOW);
		break;

	case 5: //��� ���� ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:display-advanced"), NULL, NULL, SW_SHOW);
		break;

	case 6: //Ŭ������
		::ShellExecute(NULL, _T("open"), _T("ms-settings:clipboard"), NULL, NULL, SW_SHOW);
		break;

	case 7: //���÷���
		::ShellExecute(NULL, _T("open"), _T("ms-settings:display"), NULL, NULL, SW_SHOW);
		break;

	case 8: //�� �������� ����Ǵ� ��ġ ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:savelocations"), NULL, NULL, SW_SHOW);
		break;

	case 9: //���÷���
		::ShellExecute(NULL, _T("open"), _T("ms-settings:screenrotation"), NULL, NULL, SW_SHOW);
		break;

	case 10: //���÷��̸� �����ϴ� ��
		::ShellExecute(NULL, _T("open"), _T("ms-settings:quietmomentspresentation"), NULL, NULL, SW_SHOW);
		break;

	case 11: //�ش� �ð� ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:quietmomentsscheduled"), NULL, NULL, SW_SHOW);
		break;

	case 12: //���� ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:quiethours"), NULL, NULL, SW_SHOW);
		break;

	case 13: //�׷��� ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:display-advancedgraphics"), NULL, NULL, SW_SHOW);
		break;

	case 14: //��Ƽ�½�ŷ
		::ShellExecute(NULL, _T("open"), _T("ms-settings:multitasking"), NULL, NULL, SW_SHOW);
		break;

	case 15: //�߰� ��� ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:nightlight"), NULL, NULL, SW_SHOW);
		break;

	case 16: //�� PC�� ȭ�� ǥ��
		::ShellExecute(NULL, _T("open"), _T("ms-settings:project"), NULL, NULL, SW_SHOW);
		break;

	case 17: //���� ȯ��
		::ShellExecute(NULL, _T("open"), _T("ms-settings:crossdevice"), NULL, NULL, SW_SHOW);
		break;

	case 18: //�˸� �� �۾�
		::ShellExecute(NULL, _T("open"), _T("ms-settings:notifications"), NULL, NULL, SW_SHOW);
		break;

	case 19: //���� ����ũ��
		::ShellExecute(NULL, _T("open"), _T("ms-settings:remotedesktop"), NULL, NULL, SW_SHOW);
		break;

	case 20: //���� �� ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:powersleep"), NULL, NULL, SW_SHOW);
		break;

	case 21: //�����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:storagesense"), NULL, NULL, SW_SHOW);
		break;

	case 22: //����� ���� ���� �Ǵ� ���� ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:storagepolicies"), NULL, NULL, SW_SHOW);
		break;

	case 23: //��¥ �� �ð�
		::ShellExecute(NULL, _T("open"), _T("ms-settings:dateandtime"), NULL, NULL, SW_SHOW);
		break;

	case 24: //����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:regionformatting"), NULL, NULL, SW_SHOW);
		break;

	case 25: //���
		::ShellExecute(NULL, _T("open"), _T("ms-settings:keyboard"), NULL, NULL, SW_SHOW);
		break;

	case 26: //����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:speech"), NULL, NULL, SW_SHOW);
		break;

	case 27: //��ǰ ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:activation"), NULL, NULL, SW_SHOW);
		break;

	case 28: //���� ���
		::ShellExecute(NULL, _T("open"), _T("ms-settings:backup"), NULL, NULL, SW_SHOW);
		break;

	case 29: //���� ����ȭ
		::ShellExecute(NULL, _T("open"), _T("ms-settings:delivery-optimization"), NULL, NULL, SW_SHOW);
		break;

	case 30: //�� ��ġ ã��
		::ShellExecute(NULL, _T("open"), _T("ms-settings:findmydevice"), NULL, NULL, SW_SHOW);
		break;

	case 31: //�����ڿ�
		::ShellExecute(NULL, _T("open"), _T("ms-settings:developers"), NULL, NULL, SW_SHOW);
		break;

	case 32: //����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:recovery"), NULL, NULL, SW_SHOW);
		break;

	case 33: //���� �ذ�
		::ShellExecute(NULL, _T("open"), _T("ms-settings:troubleshoot"), NULL, NULL, SW_SHOW);
		break;

	case 34: //Windows ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsdefender"), NULL, NULL, SW_SHOW);
		break;

	case 35: //Windows ������ ���α׷�
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsinsider"), NULL, NULL, SW_SHOW);
		break;

	case 36: //Windows ������Ʈ
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsupdate"), NULL, NULL, SW_SHOW);
		break;

	case 37: //��� �ð� ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsupdate-activehours"), NULL, NULL, SW_SHOW);
		break;

	case 38: //��� �ɼ�
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsupdate-options"), NULL, NULL, SW_SHOW);
		break;

	case 39: //������ ������Ʈ ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsupdate-optionalupdates"), NULL, NULL, SW_SHOW);
		break;

	case 40: //������Ʈ ��� ����
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsupdate-history"), NULL, NULL, SW_SHOW);
		break;
	}
}