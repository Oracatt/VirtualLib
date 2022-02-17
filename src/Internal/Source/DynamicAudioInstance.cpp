#include "../../Internal/Include/DynamicAudioInstance.h"
#include "../../Interface/IAudio.h"

DynamicAudioInstance::DynamicAudioInstance(const DynamicAudioDesc& _desc, AudioEngine* audioEngine, unique_ptr<uint8_t[]>& buffer, unique_ptr<WAVEFORMATEX>& waveFormat) :
	mRef(0), desc(_desc), loopRemain(desc.loopCount >= 0 ? desc.loopCount : -1), soundBuf(buffer.release()), wfx(waveFormat.release()), soundIns(make_unique<DynamicSoundEffectInstance>(audioEngine,
		[&](DynamicSoundEffectInstance* ins)->void _cdecl
{
	DWORD samplesToSubmit = wfx->nSamplesPerSec * 16;
	if (isLooping)
	{
		if (loopRemain > 0 || loopRemain == -1)
		{
			if (playPtrInSample + samplesToSubmit >= desc.loopEnd)
			{
				ins->SubmitBuffer(soundBuf.get() + (uint64_t)playPtrInSample * (wfx->wBitsPerSample / 8), (uint64_t)(desc.loopEnd - playPtrInSample) * (wfx->wBitsPerSample / 8));
				playPtrInSample = desc.loopBegin;
				if (loopRemain > 0)
					loopRemain--;
			}
			else
			{
				ins->SubmitBuffer(soundBuf.get() + (uint64_t)playPtrInSample * (wfx->wBitsPerSample / 8), (uint64_t)(samplesToSubmit) * (wfx->wBitsPerSample / 8));
				playPtrInSample += samplesToSubmit;
			}
		}
		else
		{
			ins->Stop(false);
		}
	}
	else
	{
		if (playPtrInSample + samplesToSubmit >= desc.playEnd)
		{
			ins->SubmitBuffer(soundBuf.get() + (uint64_t)playPtrInSample * (wfx->wBitsPerSample / 8), (uint64_t)(desc.playEnd - playPtrInSample) * (wfx->wBitsPerSample / 8));
			playPtrInSample = desc.loopBegin;
			isLooping = true;
		}
		else
		{
			ins->SubmitBuffer(soundBuf.get() + (uint64_t)playPtrInSample * (wfx->wBitsPerSample / 8), (uint64_t)(samplesToSubmit) * (wfx->wBitsPerSample / 8));
			playPtrInSample += samplesToSubmit;
		}
	}
},
wfx->nSamplesPerSec, wfx->nChannels, wfx->wBitsPerSample)), isLooping(false), playPtrInSample(desc.playBegin) {}

void DynamicAudioInstance::Play()
{
	soundIns->Play();
}

void DynamicAudioInstance::Stop(bool immediate)
{
	isLooping = false;
	loopRemain = desc.loopCount >= 0 ? desc.loopCount : -1;
	playPtrInSample = desc.playBegin;
	soundIns->Stop(immediate);
}

void DynamicAudioInstance::Pause()
{
	soundIns->Pause();
}

void DynamicAudioInstance::Resume()
{
	soundIns->Resume();
}

void DynamicAudioInstance::SetVolume(float volume)
{
	soundIns->SetVolume(volume);
}

void DynamicAudioInstance::SetPitch(float pitch)
{
	soundIns->SetPitch(pitch);
}

void DynamicAudioInstance::SetPan(float pan)
{
	soundIns->SetPan(pan);
}

void DynamicAudioInstance::Apply3D(const X3DAUDIO_LISTENER& listener, const X3DAUDIO_EMITTER& emitter, bool rhcoords)
{
	soundIns->Apply3D(listener, emitter, rhcoords);
}

SoundState DynamicAudioInstance::GetState()
{
	return soundIns->GetState();
}

unsigned int DynamicAudioInstance::GetChannelCount() const
{
	return soundIns->GetChannelCount();
}

HRESULT _stdcall DynamicAudioInstance::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == IID_IDynamicAudioInstance)
	{
		*ppvObject = static_cast<IDynamicAudioInstance*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	static_cast<IUnknown*>(*ppvObject)->AddRef();
	return S_OK;
}

ULONG _stdcall DynamicAudioInstance::AddRef()
{
	return InterlockedIncrement(&mRef);
}

ULONG _stdcall DynamicAudioInstance::Release()
{
	ULONG val = InterlockedDecrement(&mRef);
	if (val == 0)
	{
		delete this;
	}
	return val;
}