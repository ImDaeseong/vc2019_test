#include "pch.h"
#include "Common.h"

Common::Common()
{
}

Common::~Common()
{
}

CString Common::GetFilePath(CString strFilename)
{
	int nPos = strFilename.ReverseFind('\\');
	if (nPos > 0)
		return strFilename.Left(nPos);

	return strFilename;
}