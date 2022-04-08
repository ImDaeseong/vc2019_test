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
