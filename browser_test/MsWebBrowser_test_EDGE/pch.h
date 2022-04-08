#ifndef PCH_H
#define PCH_H

#include "framework.h"

#include <comdef.h>
#include <mshtml.h>
#include <atlbase.h>
#include <sddl.h>
#include <Tlhelp32.h>
#include <fstream>

#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#include "CGdiPlusBitmap.h"
#include "TextStatic.h"
#include "PngStatic.h"
#include "Common_Util.h"
#include "EdgeWebBrowser.h"

static constexpr UINT MSG_NAVIGATE = WM_APP + 123;
static constexpr UINT MSG_RUN_ASYNC_CALLBACK = WM_APP + 124;

#define WM_WEB_MESSAGERECEIVE     WM_USER + 1001

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }

#define SAFE_DELETEDC( hdc ) if( hdc ){ ::DeleteDC( hdc ); hdc = NULL; }
#define SAFE_DELETEOBJECT( hobj ) if( hobj ){ ::DeleteObject( hobj ); hobj = NULL; }

#endif //PCH_H
