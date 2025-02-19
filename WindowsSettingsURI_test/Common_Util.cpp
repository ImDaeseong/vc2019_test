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
	case 1: //잠금화면
		::ShellExecute(NULL, _T("open"), _T("ms-settings:lockscreen"), NULL, NULL, SW_SHOW);
		break;

	case 2: //마우스
		::ShellExecute(NULL, _T("open"), _T("ms-settings:mousetouchpad"), NULL, NULL, SW_SHOW);
		break;

	case 3: //Windows 설정
		::ShellExecute(NULL, _T("open"), _T("ms-settings:"), NULL, NULL, SW_SHOW);
		break;

	case 4: //정보
		::ShellExecute(NULL, _T("open"), _T("ms-settings:about"), NULL, NULL, SW_SHOW);
		break;

	case 5: //고급 배율 설정
		::ShellExecute(NULL, _T("open"), _T("ms-settings:display-advanced"), NULL, NULL, SW_SHOW);
		break;

	case 6: //클립보드
		::ShellExecute(NULL, _T("open"), _T("ms-settings:clipboard"), NULL, NULL, SW_SHOW);
		break;

	case 7: //디스플레이
		::ShellExecute(NULL, _T("open"), _T("ms-settings:display"), NULL, NULL, SW_SHOW);
		break;

	case 8: //새 콘텐츠가 저장되는 위치 변경
		::ShellExecute(NULL, _T("open"), _T("ms-settings:savelocations"), NULL, NULL, SW_SHOW);
		break;

	case 9: //디스플레이
		::ShellExecute(NULL, _T("open"), _T("ms-settings:screenrotation"), NULL, NULL, SW_SHOW);
		break;

	case 10: //디스플레이를 복제하는 중
		::ShellExecute(NULL, _T("open"), _T("ms-settings:quietmomentspresentation"), NULL, NULL, SW_SHOW);
		break;

	case 11: //해당 시간 동안
		::ShellExecute(NULL, _T("open"), _T("ms-settings:quietmomentsscheduled"), NULL, NULL, SW_SHOW);
		break;

	case 12: //집중 지원
		::ShellExecute(NULL, _T("open"), _T("ms-settings:quiethours"), NULL, NULL, SW_SHOW);
		break;

	case 13: //그래픽 설정
		::ShellExecute(NULL, _T("open"), _T("ms-settings:display-advancedgraphics"), NULL, NULL, SW_SHOW);
		break;

	case 14: //멀티태스킹
		::ShellExecute(NULL, _T("open"), _T("ms-settings:multitasking"), NULL, NULL, SW_SHOW);
		break;

	case 15: //야간 모드 설정
		::ShellExecute(NULL, _T("open"), _T("ms-settings:nightlight"), NULL, NULL, SW_SHOW);
		break;

	case 16: //이 PC에 화면 표시
		::ShellExecute(NULL, _T("open"), _T("ms-settings:project"), NULL, NULL, SW_SHOW);
		break;

	case 17: //공유 환경
		::ShellExecute(NULL, _T("open"), _T("ms-settings:crossdevice"), NULL, NULL, SW_SHOW);
		break;

	case 18: //알림 및 작업
		::ShellExecute(NULL, _T("open"), _T("ms-settings:notifications"), NULL, NULL, SW_SHOW);
		break;

	case 19: //원격 데스크톱
		::ShellExecute(NULL, _T("open"), _T("ms-settings:remotedesktop"), NULL, NULL, SW_SHOW);
		break;

	case 20: //전원 및 절전
		::ShellExecute(NULL, _T("open"), _T("ms-settings:powersleep"), NULL, NULL, SW_SHOW);
		break;

	case 21: //저장소
		::ShellExecute(NULL, _T("open"), _T("ms-settings:storagesense"), NULL, NULL, SW_SHOW);
		break;

	case 22: //저장소 센스 구성 또는 지금 실행
		::ShellExecute(NULL, _T("open"), _T("ms-settings:storagepolicies"), NULL, NULL, SW_SHOW);
		break;

	case 23: //날짜 및 시간
		::ShellExecute(NULL, _T("open"), _T("ms-settings:dateandtime"), NULL, NULL, SW_SHOW);
		break;

	case 24: //지역
		::ShellExecute(NULL, _T("open"), _T("ms-settings:regionformatting"), NULL, NULL, SW_SHOW);
		break;

	case 25: //언어
		::ShellExecute(NULL, _T("open"), _T("ms-settings:keyboard"), NULL, NULL, SW_SHOW);
		break;

	case 26: //음성
		::ShellExecute(NULL, _T("open"), _T("ms-settings:speech"), NULL, NULL, SW_SHOW);
		break;

	case 27: //정품 인증
		::ShellExecute(NULL, _T("open"), _T("ms-settings:activation"), NULL, NULL, SW_SHOW);
		break;

	case 28: //파일 백업
		::ShellExecute(NULL, _T("open"), _T("ms-settings:backup"), NULL, NULL, SW_SHOW);
		break;

	case 29: //전송 최적화
		::ShellExecute(NULL, _T("open"), _T("ms-settings:delivery-optimization"), NULL, NULL, SW_SHOW);
		break;

	case 30: //내 장치 찾기
		::ShellExecute(NULL, _T("open"), _T("ms-settings:findmydevice"), NULL, NULL, SW_SHOW);
		break;

	case 31: //개발자용
		::ShellExecute(NULL, _T("open"), _T("ms-settings:developers"), NULL, NULL, SW_SHOW);
		break;

	case 32: //복구
		::ShellExecute(NULL, _T("open"), _T("ms-settings:recovery"), NULL, NULL, SW_SHOW);
		break;

	case 33: //문제 해결
		::ShellExecute(NULL, _T("open"), _T("ms-settings:troubleshoot"), NULL, NULL, SW_SHOW);
		break;

	case 34: //Windows 보안
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsdefender"), NULL, NULL, SW_SHOW);
		break;

	case 35: //Windows 참가자 프로그램
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsinsider"), NULL, NULL, SW_SHOW);
		break;

	case 36: //Windows 업데이트
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsupdate"), NULL, NULL, SW_SHOW);
		break;

	case 37: //사용 시간 변경
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsupdate-activehours"), NULL, NULL, SW_SHOW);
		break;

	case 38: //고급 옵션
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsupdate-options"), NULL, NULL, SW_SHOW);
		break;

	case 39: //선택적 업데이트 보기
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsupdate-optionalupdates"), NULL, NULL, SW_SHOW);
		break;

	case 40: //업데이트 기록 보기
		::ShellExecute(NULL, _T("open"), _T("ms-settings:windowsupdate-history"), NULL, NULL, SW_SHOW);
		break;
	}
}