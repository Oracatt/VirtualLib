#pragma once
#include "../Common/EngineCommon.h"
#include "IDynamicAudioInstance.h"
#include "IAudio.h"
#include "IBuffer.h"

MIDL_INTERFACE("472fe509-e04f-4f4c-86fa-cfeed45e74d3")
IAudioPlayer:public IUnknown
{
	virtual HRESULT CreateAudio(LPCSTR filename, IAudio * *ppAudio) = 0;

	virtual HRESULT CreateAudio(IBuffer* pBuffer, IAudio** ppAudio) = 0;

	virtual HRESULT CreateDynamicAudioInstance(LPCSTR filename, const DynamicAudioDesc& desc, IDynamicAudioInstance** ppAudioInstance) = 0;

	virtual HRESULT CreateDynamicAudioInstance(IBuffer* pBuffer, const DynamicAudioDesc& desc, IDynamicAudioInstance** ppAudioInstance) = 0;
};