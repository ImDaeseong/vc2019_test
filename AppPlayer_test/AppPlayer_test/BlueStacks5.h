#pragma once
class BlueStacks5
{
public:
	BlueStacks5();
	virtual ~BlueStacks5();

public:
	CString getBlueStacks5Path();
	CString getBlueStacks5Version();
	BOOL DefaultRunBlueStacks5();
	BOOL RunBlueStacks5(CString strPackage, CString strActivity);
};