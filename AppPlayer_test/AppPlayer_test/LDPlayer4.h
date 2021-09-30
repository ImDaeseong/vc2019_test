#pragma once
class LDPlayer4
{
public:
	LDPlayer4();
	virtual ~LDPlayer4();

public:
	CString getLDPlayer4Path();
	CString getLDPlayer4Version();
	BOOL DefaultRunLDPlayer4();
	BOOL RunLDPlayer4(CString strPackage);
};

