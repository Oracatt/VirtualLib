#pragma once
#include "../../Common/EngineCommon.h"

class CustomTextRenderer : public IDWriteTextRenderer
{
public:
	ULONG m_cRef;

	D2D1_RECT_F rect;
	Vector2 location;
	Vector2 scale;
	float rotation;
	bool outlineEnabled;
	bool bodyEnabled;
	ComPtr<ID2D1Factory> m_pD2DFactory;
	ComPtr<ID2D1RenderTarget> m_pRenderTarget;
	ComPtr<ID2D1Brush> m_pTextBodyBrush;
	ComPtr<ID2D1Brush> m_pTextOutlineBrush;
	float mStrokeWidth;

	CustomTextRenderer(
		ID2D1Factory* pD2DFactory, ID2D1RenderTarget* pRenderTarget,
		ID2D1Brush* pTextBodyBrush, ID2D1Brush* pTextOutlineBrush,
		float strokeWidth = 1.0f);

	STDMETHOD(DrawGlyphRun)(
		void* clientDrawingContext,
		FLOAT                              baselineOriginX,
		FLOAT                              baselineOriginY,
		DWRITE_MEASURING_MODE              measuringMode,
		DWRITE_GLYPH_RUN const* glyphRun,
		DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
		IUnknown* clientDrawingEffect
		);

	STDMETHOD(DrawUnderline)(
		void* clientDrawingContext,
		FLOAT                  baselineOriginX,
		FLOAT                  baselineOriginY,
		DWRITE_UNDERLINE const* underline,
		IUnknown* clientDrawingEffect
		);

	STDMETHOD(DrawStrikethrough)(
		void* clientDrawingContext,
		FLOAT                      baselineOriginX,
		FLOAT                      baselineOriginY,
		DWRITE_STRIKETHROUGH const* strikethrough,
		IUnknown* clientDrawingEffect
		);

	STDMETHOD(DrawInlineObject)(
		void* clientDrawingContext,
		FLOAT               originX,
		FLOAT               originY,
		IDWriteInlineObject* inlineObject,
		BOOL                isSideways,
		BOOL                isRightToLeft,
		IUnknown* clientDrawingEffect
		);

	STDMETHOD(IsPixelSnappingDisabled)(
		void* clientDrawingContext,
		BOOL* isDisabled
		);

	STDMETHOD(GetCurrentTransform)(
		void* clientDrawingContext,
		DWRITE_MATRIX* transform
		);

	STDMETHOD(GetPixelsPerDip)(
		void* clientDrawingContext,
		FLOAT* pixelsPerDip
		);

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void FAR* FAR* ppvObj);

	ULONG STDMETHODCALLTYPE AddRef();

	ULONG STDMETHODCALLTYPE Release();
};