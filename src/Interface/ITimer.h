#pragma once
#include "../Common/EngineCommon.h"

MIDL_INTERFACE("7083b2d0-aff6-41a4-a355-ed71adaea9fb")
ITimer:public IUnknown
{
	virtual double GetGameTime() = 0;

	virtual void SetGameTime(double value) = 0;

	virtual double GetDeltaTime() = 0;

	virtual int GetTimeStopLevel() = 0;

	virtual void SetTimeStopLevel(int value) = 0;

	virtual int GetTimeRunningLevel() = 0;

	virtual void SetTimeRunningLevel(int value) = 0;

	virtual double GetTimeSpeed() = 0;

	virtual void SetTimeSpeed(double value) = 0;

	virtual int GetUpdateCount() = 0;

	virtual void SetUpdateCount(int count) = 0;

	virtual int GetPresentCount() = 0;

	virtual void SetPresentCount(int count) = 0;
};