#pragma once

namespace Common_Util
{
	Bitmap* Load(LPCTSTR pName, LPCTSTR pType, HMODULE hInst);
	Bitmap* ResizeBitmap(Bitmap* pInImg, int nResizeWidth, int nResizeHeight);
};

