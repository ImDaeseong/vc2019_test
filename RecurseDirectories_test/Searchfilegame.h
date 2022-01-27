#pragma once

class Searchfilegame
{
public:
	Searchfilegame();
	virtual ~Searchfilegame();

public:
	static CString GetFilePath(CString strFilename);
	static void getDriveList(CStringArray& drivelist);
	static void RecurseDirectories(CStringArray& folderList, CStringArray& fileList, CString strDirectory, int nDepth);
};

