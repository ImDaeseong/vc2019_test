#pragma once
class FeatureUsage
{
public:
	FeatureUsage();
	virtual ~FeatureUsage();

public:
	void InitLoad();
	void FindExe(CString strFileName);
	void FindExe(CStringArray& list);
	void FindExe(CString strFileName, CStringArray& list);
	void FindExe(CStringArray& strFileNamelist, CStringArray& list);

private:
	void getRecentDocs();
	void getRunMRU();
	void getAppBadgeUpdated();
	void getAppLaunch();
	void getAppSwitched();
	void getShowJumpView();
	CString GetFileName(CString strFilename);
	CString FindPathString(CString strPath);
	CString GetSystem32Path();
	CString GetProgramFilesPath();
	CString GetProgramFilesx86Path();

	CStringArray aryList;
};

