#include "../Include/Audio.h"
#include "../Include/AudioInstance.h"

Audio::Audio(unique_ptr<SoundEffect>& sound, unique_ptr<WAVEFORMATEX>& waveFormat) :mRef(0)
{
    soundBuf.reset(sound.release());
    wfx.reset(waveFormat.release());
}

void Audio::Play()
{
    soundBuf->Play();
}

void Audio::Play(float volume, float pitch, float pan)
{
    soundBuf->Play(volume, pitch, pan);
}

IAudioInstance* Audio::CreateInstance(IAudioInstance** ppInstance, SOUND_EFFECT_INSTANCE_FLAGS flags)
{
    if (ppInstance)
    {
        AudioInstance* audio = new AudioInstance(soundBuf->CreateInstance(flags), static_cast<IAudio*>(this));
        audio->AddRef();
        *ppInstance = static_cast<IAudioInstance*>(audio);
        return *ppInstance;
    }
    return 0;
}

HRESULT _stdcall Audio::QueryInterface(const IID& iid, void** ppvObject)
{
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
    static_cast<IUnknown*>(*ppvObject)->AddRef();
    return S_OK;
}

ULONG _stdcall Audio::AddRef()
{
    return InterlockedIncrement(&mRef);
}

ULONG _stdcall Audio::Release()
{
    ULONG val = InterlockedDecrement(&mRef);
    if (val == 0)
    {
        delete this;
    }
    return val;
}