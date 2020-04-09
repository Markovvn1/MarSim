#pragma once

class SysMsg
{
	friend class ICore;

protected:
	virtual void onStart();
	virtual void onStop();

public:
	virtual ~SysMsg();
};
