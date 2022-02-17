#pragma once
#include "../../Interface/IAudioInstance.h"

struct IAudio;

class AudioInstance :public IAudioInstance
{
public:

    AudioInstance(unique_ptr<SoundEffectInstance>& sound, IAudio* pAudio);

    void Play(bool loop) override;

    void Stop(bool immediate) override;

    void Pause() override;

    void Resume() override;

    void SetVolume(float volume) override;

    void SetPitch(float pitch) override;

    void SetPan(float pan) override;

    void Apply3D(const X3DAUDIO_LISTENER& listener, const X3DAUDIO_EMITTER& emitter, bool rhcoords) override;

    bool IsLooped() const override;

    SoundState GetState() override;

    unsigned int GetChannelCount() const override;

    IVoiceNotify* GetVoiceNotify() const override;

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

private:

	ULONG mRef;

	unique_ptr<SoundEffectInstance> soundIns;

    ComPtr<IAudio> pAudio;
};