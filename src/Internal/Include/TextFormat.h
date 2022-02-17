#pragma once
#include "../../Interface/ITextFormat.h"
#include "../../Interface/IBuffer.h"

class TextFormat :public ITextFormat
{
public:

	TextFormat();

	~TextFormat();

	HRESULT Initialize(IDWriteFactory* pFactory, LPCSTR lpFaceName, float size, LPCSTR filename = 0);

	HRESULT Initialize(IDWriteFactory* pFactory, LPCSTR lpFaceName, float size,
		DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, LPCSTR filename = 0);

	HRESULT Initialize(IDWriteFactory* _pFactory, LPCSTR lpFaceName, float size, IBuffer* pBuffer);

	HRESULT Initialize(IDWriteFactory* pFactory, LPCSTR lpFaceName, float size,
		DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, LPCSTR localeName, IBuffer* pBuffer);

	HRESULT _stdcall QueryInterface(const IID& iid, void** ppvObject) override;

	ULONG _stdcall AddRef() override;

	ULONG _stdcall Release() override;

	UINT GetAlignment() override;

	void SetAlignment(UINT alignment) override;

	IDWriteTextFormat* GetDWriteTextFormat() override;

private:

	ULONG mRef;

	UINT align = ALIGNMENT_TYPE_LEFT | ALIGNMENT_TYPE_TOP;

	ComPtr<IDWriteTextFormat> pFormat;

	ComPtr<IDWriteInMemoryFontFileLoader> pInMemoryFontFileLoader;

	ComPtr<IDWriteFactory> pFactory1;

	void ResetMemory(bool _register);
};