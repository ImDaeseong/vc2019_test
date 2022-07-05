// dllmain.h : 모듈 클래스의 선언입니다.

class CSetupHelperModule : public ATL::CAtlDllModuleT< CSetupHelperModule >
{
public :
	DECLARE_LIBID(LIBID_SetupHelperLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SETUPHELPER, "{7230b8ee-ba81-489c-b355-4d78ca9ba529}")
};

extern class CSetupHelperModule _AtlModule;
