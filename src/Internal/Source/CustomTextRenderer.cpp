#include "../Include/CustomTextRenderer.h"

CustomTextRenderer::CustomTextRenderer(
    ID2D1Factory* pD2DFactory, ID2D1RenderTarget* pRenderTarget,
    ID2D1Brush* pTextBodyBrush, ID2D1Brush* pTextOutlineBrush,
    float strokeWidth) :m_cRef(0), outlineEnabled(true), bodyEnabled(true), m_pD2DFactory(pD2DFactory), m_pRenderTarget(pRenderTarget),
    m_pTextBodyBrush(pTextBodyBrush), m_pTextOutlineBrush(pTextOutlineBrush), mStrokeWidth(strokeWidth) {}

HRESULT CustomTextRenderer::DrawGlyphRun(
    void* clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY,
    DWRITE_MEASURING_MODE measuringMode, DWRITE_GLYPH_RUN const* glyphRun,
    DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription, IUnknown* clientDrawingEffect)
{
    HRESULT hr = S_OK;

    ComPtr<ID2D1PathGeometry> pPathGeometry;
    hr = m_pD2DFactory->CreatePathGeometry(&pPathGeometry);
    ComPtr<ID2D1GeometrySink> pSink;
    hr = pPathGeometry->Open(&pSink);

    hr = glyphRun->fontFace->GetGlyphRunOutline(
        glyphRun->fontEmSize,
        glyphRun->glyphIndices,
        glyphRun->glyphAdvances,
        glyphRun->glyphOffsets,
        glyphRun->glyphCount,
        glyphRun->isSideways,
        glyphRun->bidiLevel,
        pSink.Get()
    );
    hr = pSink->Close();
    D2D1_RECT_F bound;
    pPathGeometry->GetBounds(Matrix3x2F::Identity(), &bound);

    D2D1_MATRIX_3X2_F matrix;
    matrix = Matrix3x2F::Translation({ baselineOriginX,baselineOriginY });
    matrix = matrix * Matrix3x2F::Translation({ rect.left,rect.top });
    matrix = matrix * Matrix3x2F::Scale({ scale.x,scale.y });
    matrix = matrix * Matrix3x2F::Rotation((double)rotation * 180 / M_PI);
    matrix = matrix * Matrix3x2F::Translation({ location.x ,location.y });
    ComPtr<ID2D1TransformedGeometry> pTransformedGeometry;
    hr = m_pD2DFactory->CreateTransformedGeometry(pPathGeometry.Get(), matrix, &pTransformedGeometry);

    if (outlineEnabled)
        m_pRenderTarget->DrawGeometry(pTransformedGeometry.Get(), m_pTextOutlineBrush.Get(), mStrokeWidth);
    if (bodyEnabled)
        m_pRenderTarget->FillGeometry(pTransformedGeometry.Get(), m_pTextBodyBrush.Get());

    return hr;
}

IFACEMETHODIMP CustomTextRenderer::DrawUnderline(
    void* clientDrawingContext,
    FLOAT baselineOriginX,
    FLOAT baselineOriginY,
    DWRITE_UNDERLINE const* underline,
    IUnknown* clientDrawingEffect
)
{
    return E_NOTIMPL;
}

IFACEMETHODIMP CustomTextRenderer::DrawStrikethrough(
    void* clientDrawingContext,
    FLOAT baselineOriginX,
    FLOAT baselineOriginY,
    DWRITE_STRIKETHROUGH const* strikethrough,
    IUnknown* clientDrawingEffect
)
{
    return E_NOTIMPL;
}

IFACEMETHODIMP CustomTextRenderer::DrawInlineObject(
    void* clientDrawingContext,
    FLOAT originX,
    FLOAT originY,
    IDWriteInlineObject* inlineObject,
    BOOL isSideways,
    BOOL isRightToLeft,
    IUnknown* clientDrawingEffect
)
{
    return E_NOTIMPL;
}

IFACEMETHODIMP_(unsigned long) CustomTextRenderer::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(unsigned long) CustomTextRenderer::Release()
{
    unsigned long newCount = InterlockedDecrement(&m_cRef);
    if (newCount == 0)
    {
        delete this;
        return 0;
    }

    return newCount;
}

IFACEMETHODIMP CustomTextRenderer::IsPixelSnappingDisabled(
    void* clientDrawingContext,
    BOOL* isDisabled
)
{
    *isDisabled = FALSE;
    return S_OK;
}

IFACEMETHODIMP CustomTextRenderer::GetCurrentTransform(
    void* clientDrawingContext,
    DWRITE_MATRIX* transform
)
{
    m_pRenderTarget->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
    return S_OK;
}

IFACEMETHODIMP CustomTextRenderer::GetPixelsPerDip(
    void* clientDrawingContext,
    FLOAT* pixelsPerDip
)
{
    float x, yUnused;

    m_pRenderTarget->GetDpi(&x, &yUnused);
    *pixelsPerDip = x / 96.0f;

    return S_OK;
}

IFACEMETHODIMP CustomTextRenderer::QueryInterface(
    REFIID riid, void FAR* FAR* ppvObject
)
{
    if (__uuidof(IDWriteTextRenderer) == riid)
    {
        *ppvObject = this;
    }
    else if (__uuidof(IDWritePixelSnapping) == riid)
    {
        *ppvObject = this;
    }
    else if (__uuidof(IUnknown) == riid)
    {
        *ppvObject = this;
    }
    else
    {
        *ppvObject = nullptr;
        return E_FAIL;
    }

    this->AddRef();

    return S_OK;
}
