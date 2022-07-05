// dllmain.h : 모듈 클래스의 선언입니다.

class CUrlFileDownModule : public ATL::CAtlDllModuleT< CUrlFileDownModule >
{
public :
	DECLARE_LIBID(LIBID_UrlFileDownLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_URLFILEDOWN, "{30483ccf-847c-417d-a76c-ca897e60bf0e}")
};

extern class CUrlFileDownModule _AtlModule;
