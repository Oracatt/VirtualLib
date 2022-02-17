#pragma once
#include "../../Interface/IAudioPlayer.h"
#include "../../DXMaster/Include/Audio.h"

class AudioPlayer :public IAudioPlayer
{
public:

	AudioPlayer(class Engine* engine);

	void AudioUpdate();

	HRESULT InitAudioPlayer();

	HRESULT CreateAudio(LPCSTR filename, IAudio** ppAudio) override;

	HRESULT CreateAudio(IBuffer* pBuffer, IAudio** ppAudio) override;

	HRESULT CreateDynamicAudioInstance(LPCSTR filename, const DynamicAudioDesc& desc, IDynamicAudioInstance** ppAudioInstance) override;

	HRESULT CreateDynamicAudioInstance(IBuffer* pBuffer, const DynamicAudioDesc& desc, IDynamicAudioInstance** ppAudioInstance) override;

private:

	class Engine* engine;

	unique_ptr<AudioEngine> audioEngine;

};