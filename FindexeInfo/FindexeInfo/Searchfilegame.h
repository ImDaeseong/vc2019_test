#ifndef _SEARCHFILE_GAME_H
#define _SEARCHFILE_GAME_H

namespace Searchfilegame
{
	int getProcesscpu();
	CString GetFilePath(CString strFilename);
	void getDriveList(CStringArray& drivelist);
	void RecurseDirectories(CStringArray& folderList, CStringArray& fileList, CString strDirectory, int nDepth);
}

#endif

