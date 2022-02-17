#include "../../Internal/Include/AudioInstance.h"
#include "../../Interface/IAudio.h"

AudioInstance::AudioInstance(unique_ptr<SoundEffectInstance>& sound, IAudio* _pAudio) :mRef(0), pAudio(_pAudio)
{
	soundIns.reset(sound.release());
}

void AudioInstance::Play(bool loop)
{
	soundIns->Play(loop);
}

void AudioInstance::Stop(bool immediate)
{
	soundIns->Stop(immediate);
}

void AudioInstance::Pause()
{
	soundIns->Pause();
}

void AudioInstance::Resume()
{
	soundIns->Resume();
}

void AudioInstance::SetVolume(float volume)
{
	soundIns->SetVolume(volume);
}

void AudioInstance::SetPitch(float pitch) 
{
	soundIns->SetPitch(pitch);
}

void AudioInstance::SetPan(float pan)
{
	soundIns->SetPan(pan);
}

void AudioInstance::Apply3D(const X3DAUDIO_LISTENER& listener, const X3DAUDIO_EMITTER& emitter, bool rhcoords)
{
	soundIns->Apply3D(listener, emitter, rhcoords);
}

bool AudioInstance::IsLooped() const
{
	return soundIns->IsLooped();
}

SoundState AudioInstance::GetState()
{
	return soundIns->GetState();
}

unsigned int AudioInstance::GetChannelCount() const
{
	return soundIns->GetChannelCount();
}

IVoiceNotify* AudioInstance::GetVoiceNotify() const
{
	return soundIns->GetVoiceNotify();
}

HRESULT _stdcall AudioInstance::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == IID_IAudioInstance)
	{
		*ppvObject = static_cast<IAudioInstance*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	static_cast<IUnknown*>(*ppvObject)->AddRef();
	return S_OK;
}

ULONG _stdcall AudioInstance::AddRef()
{
	return InterlockedIncrement(&mRef);
}

ULONG _stdcall AudioInstance::Release()
{
	ULONG val = InterlockedDecrement(&mRef);
	if (val == 0)
	{
		delete this;
	}
	return val;
}