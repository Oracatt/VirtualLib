#include "../Include/TextFormat.h"
#include "../Include/FontLoader.h"
#include "../Include/MiscFunc.h"

TextFormat::TextFormat() :mRef(0) {}

TextFormat::~TextFormat()
{
    ResetMemory(false);
}

HRESULT TextFormat::Initialize(IDWriteFactory* pFactory, LPCSTR lpFaceName, float size, LPCSTR filename)
{
    pFactory1 = pFactory;

    if (!filename)
    {
        HRESULT hr = pFactory->CreateTextFormat(c2w(lpFaceName).c_str(), 0,
            DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"zh-cn", &pFormat);
        if (SUCCEEDED(hr))
        {
            SetAlignment((ALIGNMENT_TYPE)align);
            return SUCCEED_OPERATE;
        }
        return ERR_ACCESS;
    }
    ComPtr<IDWriteFontCollection> fCollection;
    MFFontContext fContext(pFactory);
    std::vector<std::wstring> filePaths;
    filePaths.emplace_back(c2w(filename));
    HRESULT hr = fContext.CreateFontCollection(filePaths, &fCollection);
    hr = pFactory->CreateTextFormat(c2w(lpFaceName).c_str(), fCollection.Get(),
        DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"zh-cn", &pFormat);
    if (SUCCEEDED(hr))
    {
        SetAlignment((ALIGNMENT_TYPE)align);
        return SUCCEED_OPERATE;
    }
    return ERR_ACCESS;
}

HRESULT TextFormat::Initialize(IDWriteFactory* pFactory, LPCSTR lpFaceName, float size,
    DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, LPCSTR filename)
{
    pFactory1 = pFactory;

    if (!filename)
    {
        HRESULT hr = pFactory->CreateTextFormat(c2w(lpFaceName).c_str(), 0,
            weight, style, stretch, size, c2w(localeName).c_str(), &pFormat);
        if (SUCCEEDED(hr))
        {
            SetAlignment((ALIGNMENT_TYPE)align);
            return SUCCEED_OPERATE;
        }
        return ERR_ACCESS;
    }
    ComPtr<IDWriteFontCollection> fCollection;
    MFFontContext fContext(pFactory);
    std::vector<std::wstring> filePaths;
    filePaths.emplace_back(c2w(filename));
    HRESULT hr = fContext.CreateFontCollection(filePaths, &fCollection);
    hr = pFactory->CreateTextFormat(c2w(lpFaceName).c_str(), fCollection.Get(),
        weight, style, stretch, size, c2w(localeName).c_str(), &pFormat);
    if (SUCCEEDED(hr))
    {
        SetAlignment((ALIGNMENT_TYPE)align);
        return SUCCEED_OPERATE;
    }
    return ERR_ACCESS;
}

HRESULT TextFormat::Initialize(IDWriteFactory* _pFactory, LPCSTR lpFaceName, float size, IBuffer* pBuffer)
{
#define CR(a) if(FAILED(a)){ResetMemory(false); return ERR_UNEXPECTED;}
    pFactory1 = _pFactory;

    HRESULT hr;
    ComPtr<IDWriteFactory5> pFactory;
    ComPtr<IDWriteFontFile> fontFile;
    ComPtr<IDWriteFontSetBuilder1> pFontSetBuilder;
    ComPtr<IDWriteFontSet> pFontSet;
    ComPtr<IDWriteFontCollection1> pCollection;

    ResetMemory(true);

    _pFactory->QueryInterface(__uuidof(IDWriteFactory5), &pFactory);
    hr = pInMemoryFontFileLoader->CreateInMemoryFontFileReference(_pFactory, pBuffer->GetPointer(), pBuffer->GetSize(), pBuffer, &fontFile); CR(hr);
    hr = pFactory->CreateFontSetBuilder(&pFontSetBuilder); CR(hr);
    hr = pFontSetBuilder->AddFontFile(fontFile.Get()); CR(hr);
    hr = pFontSetBuilder->CreateFontSet(&pFontSet); CR(hr);
    hr = pFactory->CreateFontCollectionFromFontSet(pFontSet.Get(), &pCollection); CR(hr);
    hr = pFactory->CreateTextFormat(c2w(lpFaceName).c_str(), pCollection.Get(),
        DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"zh-cn", &pFormat); CR(hr);

    if (SUCCEEDED(hr))
        SetAlignment((ALIGNMENT_TYPE)align);

    return SUCCEED_OPERATE;
#undef CR;
}

HRESULT TextFormat::Initialize(IDWriteFactory* _pFactory, LPCSTR lpFaceName, float size,
    DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, IBuffer* pBuffer)
{
#define CR(a) if(FAILED(a)){ResetMemory(false); return ERR_UNEXPECTED;}
    pFactory1 = _pFactory;

    HRESULT hr;
    ComPtr<IDWriteFactory5> pFactory;
    ComPtr<IDWriteInMemoryFontFileLoader> pInMemoryFontFileLoader;
    ComPtr<IDWriteFontFile> fontFile;
    ComPtr<IDWriteFontSetBuilder1> pFontSetBuilder;
    ComPtr<IDWriteFontSet> pFontSet;
    ComPtr<IDWriteFontCollection1> pCollection;

    ResetMemory(true);

    _pFactory->QueryInterface(__uuidof(IDWriteFactory5), &pFactory);
    hr = pInMemoryFontFileLoader->CreateInMemoryFontFileReference(_pFactory, pBuffer->GetPointer(), pBuffer->GetSize(), pBuffer, &fontFile); CR(hr);
    hr = pFactory->CreateFontSetBuilder(&pFontSetBuilder); CR(hr);
    hr = pFontSetBuilder->AddFontFile(fontFile.Get()); CR(hr);
    hr = pFontSetBuilder->CreateFontSet(&pFontSet); CR(hr);
    hr = pFactory->CreateFontCollectionFromFontSet(pFontSet.Get(), &pCollection); CR(hr);
    hr = pFactory->CreateTextFormat(c2w(lpFaceName).c_str(), pCollection.Get(),
        weight, style, stretch, size, c2w(localeName).c_str(), &pFormat); CR(hr);

    if (SUCCEEDED(hr))
        SetAlignment((ALIGNMENT_TYPE)align);

    return SUCCEED_OPERATE;
#undef CR;
}

HRESULT _stdcall TextFormat::QueryInterface(const IID& iid, void** ppvObject)
{
    if (iid == IID_ITextFormat)
    {
        *ppvObject = static_cast<ITextFormat*>(this);
    }
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
    static_cast<IUnknown*>(*ppvObject)->AddRef();
    return S_OK;
}

ULONG _stdcall TextFormat::AddRef()
{
    return InterlockedIncrement(&mRef);
}

ULONG _stdcall TextFormat::Release()
{
    ULONG val = InterlockedDecrement(&mRef);
    if (val == 0)
    {
        delete this;
    }
    return val;
}

UINT TextFormat::GetAlignment()
{
    return align;
}

void TextFormat::SetAlignment(UINT alignment)
{
    align = alignment;
    if (align & ALIGNMENT_TYPE_LEFT)
        pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    else if (align & ALIGNMENT_TYPE_RIGHT)
        pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
    else
        pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    if (align & ALIGNMENT_TYPE_TOP)
        pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    else if (align & ALIGNMENT_TYPE_BOTTOM)
        pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
    else
        pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

IDWriteTextFormat* TextFormat::GetDWriteTextFormat()
{
    return pFormat.Get();
}

void TextFormat::ResetMemory(bool _register)
{
    if (pInMemoryFontFileLoader)
    {
        pFactory1->UnregisterFontFileLoader(pInMemoryFontFileLoader.Get());
        pInMemoryFontFileLoader.Reset();
    }
    if (_register)
    {
        ComPtr<IDWriteFactory5> pFactory;
        pFactory1->QueryInterface(__uuidof(IDWriteFactory5), &pFactory);
        pFactory->CreateInMemoryFontFileLoader(&pInMemoryFontFileLoader);
        pFactory->RegisterFontFileLoader(pInMemoryFontFileLoader.Get());
    }
}