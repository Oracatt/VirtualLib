#pragma once
#include "../../Interface/IAudio.h"
#include "../../DXMaster/Include/Audio.h"

class Audio :public IAudio
{
public:

	Audio(unique_ptr<SoundEffect>& sound, unique_ptr<WAVEFORMATEX>& waveFormat);

	void Play() override;

	void Play(float volume, float pitch, float pan) override;

	IAudioInstance* CreateInstance(IAudioInstance** ppInstance, SOUND_EFFECT_INSTANCE_FLAGS flags) override;

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

private:

	ULONG mRef;

	unique_ptr<SoundEffect> soundBuf;

	unique_ptr<WAVEFORMATEX> wfx;

};