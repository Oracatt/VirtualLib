#pragma once
#include "../Common/EngineCommon.h"

MIDL_INTERFACE("e2391c20-4c1c-4936-8c4c-4e50e95ddaa5")
IDelegate:public IUnknown
{
	virtual void Invoke() = 0;
};

template <class _Fx, class... _Args>
class _TDelegate :public IDelegate
{
public:

    _TDelegate(_Fx&& func, _Args... args) :mRef(0), pf(forward<_Fx>(func)), pfArgs(args...) {}

    void Invoke() override
    {
        _invoke(generate_sequence<sizeof...(_Args)>{});
    }

private:

    ULONG mRef;

    _Fx pf;

    tuple<_Args...> pfArgs;

    template <int... I>
    struct get_sequence {};

    template <int N, int... I>
    struct generate_sequence : generate_sequence<N - 1, N - 1, I...> {};

    template <int... I>
    struct generate_sequence<0, I...> : get_sequence<I...> {};

    template <int... I>
    void _invoke(get_sequence<I...>)
    {
        pf(get<I>(pfArgs)...);
    }

    HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override
    {
        if (iid == IID_IDelegate)
        {
            *ppvObject = static_cast<IDelegate*>(this);
        }
        else
        {
            *ppvObject = NULL;
            return E_NOINTERFACE;
        }
        static_cast<IUnknown*>(*ppvObject)->AddRef();
        return S_OK;
    }

    ULONG _stdcall AddRef() override
    {
        return InterlockedIncrement(&mRef);
    }

    ULONG _stdcall Release() override
    {
        ULONG val = InterlockedDecrement(&mRef);
        if (val == 0)
        {
            delete this;
        }
        return val;
    }
};

template <class _Fx, class... _Args>
IDelegate* Delegate(_Fx&& func, const _Args&... args)
{
    return new _TDelegate<_Fx, _Args...>(forward<_Fx>(func), args...);
}

template <class _Fx, class... _Args>
IDelegate* Delegate(_Binder<_Unforced, _Fx>&& func, const _Args&... args)
{
    return new _TDelegate<_Fx, _Args...>(forward<_Fx>(func), args...);
}