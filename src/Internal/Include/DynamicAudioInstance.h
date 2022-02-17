#pragma once
#include "../../Interface/IDynamicAudioInstance.h"

struct IAudio;

class DynamicAudioInstance :public IDynamicAudioInstance
{
public:

    DynamicAudioInstance(const DynamicAudioDesc& desc, AudioEngine* audioEngine, unique_ptr<uint8_t[]>& buffer, unique_ptr<WAVEFORMATEX>& waveFormat);

    void Play() override;

    void Stop(bool immediate) override;

    void Pause() override;

    void Resume() override;

    void SetVolume(float volume) override;

    void SetPitch(float pitch) override;

    void SetPan(float pan) override;

    void Apply3D(const X3DAUDIO_LISTENER& listener, const X3DAUDIO_EMITTER& emitter, bool rhcoords) override;

    SoundState GetState() override;

    unsigned int GetChannelCount() const override;

    HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

    ULONG _stdcall AddRef() override;

    ULONG _stdcall Release() override;

private:

    ULONG mRef;

    DynamicAudioDesc desc;

    int loopRemain;

    unique_ptr<uint8_t> soundBuf;

    unique_ptr<WAVEFORMATEX> wfx;

    unique_ptr<DynamicSoundEffectInstance> soundIns;

    bool isLooping;

    UINT playPtrInSample;
};