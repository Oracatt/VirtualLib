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

	int timeStopLevel = 0; //����timeRunningLevelʱ��ֹͣ������updateLevelʱֹͣupdate��lateupdate,���10

	int timeRunningLevel = 0; //С��timeStopLevelʱʱ��ֹͣ

	double timeSpeed = 1; //ʱ�������ٶ�

	int updateCount = 1; //ÿ֡���´���

	int renderCount = 1; //ÿ֡��Ⱦ����

};