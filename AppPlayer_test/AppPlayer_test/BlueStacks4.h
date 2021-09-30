#pragma once
class BlueStacks4
{
public:
	BlueStacks4();
	virtual ~BlueStacks4();

public:
	CString getBlueStacks4Path();
	CString getBlueStacks4Version();
	BOOL DefaultRunBlueStacks4();
	BOOL RunBlueStacks4(CString strPackage, CString strActivity);
};
