#include "../Include/Timer.h"
#include "../Include/Engine.h"

Timer::Timer(Engine* eng) :engine(eng) {}

double Timer::GetGameTime()
{
	return gameTime;
}

void Timer::SetGameTime(double value)
{
	gameTime = value;
}

double Timer::GetDeltaTime()
{
	return deltaTime;
}

int Timer::GetTimeStopLevel()
{
	return timeStopLevel;
}

void Timer::SetTimeStopLevel(int value)
{
	timeStopLevel = value;
}

int Timer::GetTimeRunningLevel()
{
	return timeRunningLevel;
}

void Timer::SetTimeRunningLevel(int value)
{
	timeRunningLevel = value;
}

double Timer::GetTimeSpeed()
{
	return timeSpeed;
}

void Timer::SetTimeSpeed(double value)
{
	timeSpeed = value;
}

int Timer::GetUpdateCount()
{
	return updateCount;
}

void Timer::SetUpdateCount(int count)
{
	updateCount = count;
}

int Timer::GetPresentCount()
{
	return renderCount;
}

void Timer::SetPresentCount(int count)
{
	renderCount = count;
}

void Timer::TimeUpdate()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	if (!inited)
	{
		inited = true;
		lastTime = currTime;
		QueryPerformanceFrequency((LARGE_INTEGER*)&cps);
	}
	double spc = 1.0 / (double)cps;
	deltaTime = (currTime - lastTime) * spc * timeSpeed;
	if (engine->mDesc.attributes.lockDeltaTime)
		deltaTime = 1.0 / engine->mDesc.videoDesc.refreshRate;
	if (timeStopLevel>timeRunningLevel)
		deltaTime = 0;
	gameTime += deltaTime;
	lastTime = currTime;
}