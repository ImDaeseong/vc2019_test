#pragma once
class FeatureUsage
{
public:
	FeatureUsage();
	virtual ~FeatureUsage();

public:
	void InitLoad();
	void clear();
	void FindExe(CString strFileName);
	void FindExe(CString strFileName, CStringArray& list);

private:
	void getRunMRU();
	void getAppLaunch();
	void getAppSwitched();
	void getShowJumpView();
	CString GetFileName(CString strFilename);

	CStringArray aryList;
};
