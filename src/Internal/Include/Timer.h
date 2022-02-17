#pragma once
#include "../../Interface/ITimer.h"
#include <windows.h>

class Timer:public ITimer
{
public:

	Timer(class Engine*);

	double GetGameTime() override;

	void SetGameTime(double value) override;

	double GetDeltaTime() override;

	int GetTimeStopLevel() override;

	void SetTimeStopLevel(int value) override;

	int GetTimeRunningLevel() override;

	void SetTimeRunningLevel(int value) override;

	double GetTimeSpeed() override;

	void SetTimeSpeed(double value) override;

	int GetUpdateCount() override;

	void SetUpdateCount(int count) override;

	int GetPresentCount() override;

	void SetPresentCount(int count) override;

	void TimeUpdate();

private:

	bool inited = false;

	INT64 cps;

	INT64 currTime = 0;
	
	INT64 lastTime = 0;

	class Engine* engine = 0;

	double gameTime = 0;

	double deltaTime = 0;

	int timeStopLevel = 0; //大于timeRunningLevel时间停止，大于updateLevel时停止update和lateupdate,最高10

	int timeRunningLevel = 0; //小于timeStopLevel时时间停止

	double timeSpeed = 1; //时间流逝速度

	int updateCount = 1; //每帧更新次数

	int renderCount = 1; //每帧渲染次数

};