#pragma once
#include "../../Interface/IBuffer.h"

class Buffer :public IBuffer
{
public:

	Buffer(void* pData, UINT size);

	~Buffer();

	UINT GetSize() override;

	void* GetPointer() override;

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

private:

	ULONG mRef;

	void* pData;

	UINT size;
};