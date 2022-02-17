#include "../Include/Buffer.h"

Buffer::Buffer(void* _pData, UINT _size) :
    mRef(0), pData(_pData), size(_size) {}

Buffer::~Buffer()
{
    delete pData;
}

UINT Buffer::GetSize()
{
	return size;
}

void* Buffer::GetPointer()
{
	return pData;
}

HRESULT _stdcall Buffer::QueryInterface(const IID& iid, void** ppvObject)
{
    if (iid == IID_IBuffer)
    {
        *ppvObject = static_cast<IBuffer*>(this);
    }
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
    static_cast<IUnknown*>(*ppvObject)->AddRef();
    return S_OK;
}

ULONG _stdcall Buffer::AddRef()
{
    return InterlockedIncrement(&mRef);
}

ULONG _stdcall Buffer::Release()
{
    ULONG val = InterlockedDecrement(&mRef);
    if (val == 0)
    {
        delete this;
    }
    return val;
}