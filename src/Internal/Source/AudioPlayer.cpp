#include "../Include/AudioPlayer.h"
#include "../Include/Engine.h"
#include "../Include/CWaveFile.h"
#include "../Include/Audio.h"
#include "../Include/DynamicAudioInstance.h"
#include "../../Extend/ResourceLoader.h"
#include <io.h>

AudioPlayer::AudioPlayer(Engine* eng)
{
	engine = eng;
}

void AudioPlayer::AudioUpdate()
{
	if (!audioEngine->Update())
	{
		if (audioEngine->IsCriticalError())
		{
			char msg[512];
			sprintf(msg, "No audio device is active");
			engine->ReportFatalError(msg, FE_AUDIOUPDATE);
		}
	}
}

HRESULT AudioPlayer::InitAudioPlayer()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif
	audioEngine = std::make_unique<AudioEngine>(eflags);
	if (!audioEngine)
		return ERR_INITAUDIOPLAYER;
	return SUCCEED_OPERATE;
}

HRESULT AudioPlayer::CreateAudio(LPCSTR filename, IAudio** ppAudio)
{
	if (_access(filename, 0) == -1)
	{
		ComPtr<IBuffer> pBuffer;
		if (FAILED(engine->CreateBuffer(filename, &pBuffer)))
		{
			*ppAudio = 0;
			char msg[512];
			sprintf(msg, "未能加载音频文件：%hs", filename);
			engine->ReportFatalError(msg, FE_FILENOTFOUND);
			return ERR_ACCESS;
		}
		return CreateAudio(pBuffer.Get(), ppAudio);
	}
	CWaveFile waveFile;
	unique_ptr<WAVEFORMATEX> wfx(new WAVEFORMATEX);
	if (FAILED(waveFile.Open(const_cast<LPSTR>(filename), 0, 1)))
	{
		char msg[512];
		sprintf(msg, "未能加载音频文件：%hs", filename);
		engine->ReportFatalError(msg, FE_CANNOTOPENFILE);
		return ERR_UNEXPECTED;
	}
	memcpy(wfx.get(), waveFile.GetFormat(), sizeof(WAVEFORMATEX));
	unique_ptr<uint8_t[]> buffer(new uint8_t[waveFile.GetSize() + sizeof(WAVEFORMATEX)]);
	memcpy(buffer.get(), wfx.get(), sizeof(WAVEFORMATEX));
	DWORD sizeRead;
	if (FAILED(waveFile.Read(buffer.get() + sizeof(WAVEFORMATEX), waveFile.GetSize(), &sizeRead)))
	{
		char msg[512];
		sprintf(msg, "未能加载音频文件：%hs", filename);
		engine->ReportFatalError(msg, FE_CANNOTOPENFILE);
		return ERR_UNEXPECTED;
	}
	unique_ptr<SoundEffect> sound = make_unique<SoundEffect>(audioEngine.get(), buffer, wfx.get(), (buffer.get()) + sizeof(WAVEFORMATEX), (size_t)waveFile.GetSize());
	waveFile.Close();
	*ppAudio = new Audio(sound, wfx);
	(*ppAudio)->AddRef();
	return SUCCEED_OPERATE;
}

HRESULT AudioPlayer::CreateAudio(IBuffer* pBuffer, IAudio** ppAudio)
{
	CWaveFile waveFile;
	unique_ptr<WAVEFORMATEX> wfx(new WAVEFORMATEX);
	if (FAILED(waveFile.OpenFromMemory((BYTE*)pBuffer->GetPointer(), pBuffer->GetSize(), 0, 1)))
	{
		char msg[512];
		sprintf(msg, "未能加载音频文件：%p(size=%d)", pBuffer->GetPointer(), pBuffer->GetSize());
		engine->ReportFatalError(msg, FE_CANNOTOPENFILE);
		return ERR_UNEXPECTED;
	}
	memcpy(wfx.get(), waveFile.GetFormat(), sizeof(WAVEFORMATEX));
	unique_ptr<uint8_t[]> buffer(new uint8_t[waveFile.GetSize() + sizeof(WAVEFORMATEX)]);
	memcpy(buffer.get(), wfx.get(), sizeof(WAVEFORMATEX));
	DWORD sizeRead;
	if (FAILED(waveFile.Read(buffer.get() + sizeof(WAVEFORMATEX), waveFile.GetSize(), &sizeRead)))
	{
		char msg[512];
		sprintf(msg, "未能加载音频文件：%p(size=%d)", pBuffer->GetPointer(), pBuffer->GetSize());
		engine->ReportFatalError(msg, FE_CANNOTOPENFILE);
		return ERR_UNEXPECTED;
	}
	unique_ptr<SoundEffect> sound = make_unique<SoundEffect>(audioEngine.get(), buffer, wfx.get(), (buffer.get()) + sizeof(WAVEFORMATEX), (size_t)waveFile.GetSize());
	waveFile.Close();
	*ppAudio = new Audio(sound, wfx);
	(*ppAudio)->AddRef();
	return SUCCEED_OPERATE;
}

HRESULT AudioPlayer::CreateDynamicAudioInstance(LPCSTR filename, const DynamicAudioDesc& desc, IDynamicAudioInstance** ppAudioInstance)
{
	if (_access(filename, 0) == -1)
	{
		ComPtr<IBuffer> pBuffer;
		if (FAILED(engine->CreateBuffer(filename, &pBuffer)))
		{
			*ppAudioInstance = 0;
			char msg[512];
			sprintf(msg, "未能加载音频文件：%hs", filename);
			engine->ReportFatalError(msg, FE_FILENOTFOUND);
			return ERR_ACCESS;
		}
		return CreateDynamicAudioInstance(pBuffer.Get(), desc, ppAudioInstance);
	}
	CWaveFile waveFile;
	unique_ptr<WAVEFORMATEX> wfx(new WAVEFORMATEX);
	if (FAILED(waveFile.Open(const_cast<LPSTR>(filename), 0, 1)))
	{
		char msg[512];
		sprintf(msg, "未能加载音频文件：%hs", filename);
		engine->ReportFatalError(msg, FE_CANNOTOPENFILE);
		return ERR_UNEXPECTED;
	}
	memcpy(wfx.get(), waveFile.GetFormat(), sizeof(WAVEFORMATEX));
	unique_ptr<uint8_t[]> buffer(new uint8_t[waveFile.GetSize() + sizeof(WAVEFORMATEX)]);
	DWORD sizeRead;
	if (FAILED(waveFile.Read(buffer.get(), waveFile.GetSize(), &sizeRead)))
	{
		char msg[512];
		sprintf(msg, "未能加载音频文件：%hs", filename);
		engine->ReportFatalError(msg, FE_CANNOTOPENFILE);
		return ERR_UNEXPECTED;
	}
	waveFile.Close();
	*ppAudioInstance = new DynamicAudioInstance(desc, audioEngine.get(), buffer, wfx);
	(*ppAudioInstance)->AddRef();
	return SUCCEED_OPERATE;
}

HRESULT AudioPlayer::CreateDynamicAudioInstance(IBuffer* pBuffer, const DynamicAudioDesc& desc, IDynamicAudioInstance** ppAudioInstance)
{
	CWaveFile waveFile;
	unique_ptr<WAVEFORMATEX> wfx(new WAVEFORMATEX);
	if (FAILED(waveFile.OpenFromMemory((BYTE*)pBuffer->GetPointer(), pBuffer->GetSize(), 0, 1)))
	{
		char msg[512];
		sprintf(msg, "未能加载音频文件：%p(size=%d)", pBuffer->GetPointer(), pBuffer->GetSize());
		engine->ReportFatalError(msg, FE_CANNOTOPENFILE);
		return ERR_UNEXPECTED;
	}
	memcpy(wfx.get(), waveFile.GetFormat(), sizeof(WAVEFORMATEX));
	unique_ptr<uint8_t[]> buffer(new uint8_t[waveFile.GetSize() + sizeof(WAVEFORMATEX)]);
	DWORD sizeRead;
	if (FAILED(waveFile.Read(buffer.get(), waveFile.GetSize(), &sizeRead)))
	{
		char msg[512];
		sprintf(msg, "未能加载音频文件：%p(size=%d)", pBuffer->GetPointer(), pBuffer->GetSize());
		engine->ReportFatalError(msg, FE_CANNOTOPENFILE);
		return ERR_UNEXPECTED;
	}
	waveFile.Close();
	*ppAudioInstance = new DynamicAudioInstance(desc, audioEngine.get(), buffer, wfx);
	(*ppAudioInstance)->AddRef();
	return SUCCEED_OPERATE;
}