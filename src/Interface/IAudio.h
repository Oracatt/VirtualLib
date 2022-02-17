#pragma once
#include "../Common/EngineCommon.h"
#include "../DXMaster/Include/Audio.h"
#include "IAudioInstance.h"

MIDL_INTERFACE("b3a1d2d1-e883-43fd-89cb-8c97f02c2da0")
IAudio:public IUnknown
{
	virtual void Play() = 0;

	virtual void Play(float volume, float pitch, float pan) = 0;

	virtual IAudioInstance* CreateInstance(IAudioInstance** ppInstance, SOUND_EFFECT_INSTANCE_FLAGS flags = SoundEffectInstance_Default) = 0;
};