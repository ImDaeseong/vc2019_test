#pragma once
class Func
{
public:
	Func();
	virtual ~Func();

	static CString GetSystemFolderPath();
	static CString GetProgramFilesFolderPath();

};

