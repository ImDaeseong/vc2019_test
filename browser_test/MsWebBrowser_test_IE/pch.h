#ifndef PCH_H
#define PCH_H

#include "framework.h"

#include <comdef.h>
#include <mshtml.h>
#include <atlbase.h>
#include <sddl.h>
#include <Tlhelp32.h>

#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#include "CGdiPlusBitmap.h"
#include "TextStatic.h"
#include "PngStatic.h"
#include "Common_Util.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }

#define SAFE_DELETEDC( hdc ) if( hdc ){ ::DeleteDC( hdc ); hdc = NULL; }
#define SAFE_DELETEOBJECT( hobj ) if( hobj ){ ::DeleteObject( hobj ); hobj = NULL; }

#endif //PCH_H
