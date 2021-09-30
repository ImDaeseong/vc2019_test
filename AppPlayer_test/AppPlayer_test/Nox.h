#pragma once
class Nox
{
public:
	Nox();
	virtual ~Nox();

public:
	CString getNoxPath();
	CString getNoxVersion();
	BOOL DefaultRunNox();
	BOOL RunNox(CString strPackage);
};