#pragma once
#include "../Common/EngineCommon.h"
#include "../DXMaster/Include/Audio.h"

MIDL_INTERFACE("f917a3e0-43c3-4767-b44b-c8df3ede981d")
IAudioInstance:public IUnknown
{
    virtual void Play(bool loop = false) = 0;

    virtual void Stop(bool immediate = true) = 0;
    
    virtual void Pause() = 0;
    
    virtual void Resume() = 0;

    virtual void SetVolume(float volume) = 0;
    
    virtual void SetPitch(float pitch) = 0;
    
    virtual void SetPan(float pan) = 0;

    virtual void Apply3D(const X3DAUDIO_LISTENER& listener, const X3DAUDIO_EMITTER& emitter, bool rhcoords = true) = 0;

    virtual bool IsLooped() const = 0;

    virtual SoundState GetState() = 0;

    virtual unsigned int GetChannelCount() const = 0;

    virtual IVoiceNotify* GetVoiceNotify() const = 0;
};