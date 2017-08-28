#include "app/outline_text_render.h"

void DrawOutlinedTextLayout(ID2D1RenderTarget* d2d_target,
    IDWriteTextLayout* text_layout, ID2D1Brush* fill_brush,
    ID2D1Brush* stroke_brush, float stroke_width,
    const D2D1_POINT_2F& origin) {

    ComPtr<OutlinedTextRenderer> textRenderer = OutlinedTextRenderer::Create(
        d2d_target, fill_brush, stroke_brush, stroke_width);
    text_layout->Draw(nullptr, textRenderer.Get(), origin.x, origin.y);
}

OutlinedTextRenderer::OutlinedTextRenderer(ID2D1RenderTarget* d2d_target,
    ID2D1Brush* fill_brush, ID2D1Brush* stroke_brush, float stroke_width)
    : ref_count_(0),
    d2d_target_(d2d_target),
    fill_brush_(fill_brush),
    stroke_brush_(stroke_brush),
    stroke_width_(stroke_width) {
}

OutlinedTextRenderer::~OutlinedTextRenderer() {
}

ComPtr<OutlinedTextRenderer> OutlinedTextRenderer::Create(
    ID2D1RenderTarget* d2d_target, ID2D1Brush* fill_brush,
    ID2D1Brush* stroke_brush, float stroke_width) {

    ComPtr<OutlinedTextRenderer> p = new OutlinedTextRenderer(d2d_target, fill_brush, stroke_brush,
        stroke_width);
    return p;
}

HRESULT OutlinedTextRenderer::QueryInterface(REFIID riid, void** ppvObject) {

    if (ppvObject == nullptr) {
        return E_POINTER;
    }

    if (__uuidof(IDWriteTextRenderer) == riid) {
        *ppvObject = this;
    }
    else if (__uuidof(IDWritePixelSnapping) == riid) {
        *ppvObject = this;
    }
    else if (__uuidof(IUnknown) == riid) {
        *ppvObject = this;
    }
    else {
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

ULONG OutlinedTextRenderer::AddRef() {
    return InterlockedIncrement(&ref_count_);
}

ULONG OutlinedTextRenderer::Release() {

    ULONG c = InterlockedDecrement(&ref_count_);
    if (c == 0) {
        delete this;
    }
    return c;
}

HRESULT OutlinedTextRenderer::IsPixelSnappingDisabled(
    void* client_drawing_context, BOOL* is_disabled) {

    *is_disabled = TRUE;
    return S_OK;
}

HRESULT OutlinedTextRenderer::GetCurrentTransform(
    void* client_drawing_context, DWRITE_MATRIX* transform) {

    d2d_target_->GetTransform((D2D1_MATRIX_3X2_F*)transform);
    return S_OK;
}

HRESULT OutlinedTextRenderer::GetPixelsPerDip(
    void* client_drawing_context, FLOAT* pixels_per_dip) {

    float x, y;
    d2d_target_->GetDpi(&x, &y);
    *pixels_per_dip = x / 96.0f;
    return S_OK;
}

HRESULT OutlinedTextRenderer::DrawGlyphRun(void* client_drawing_context,
    FLOAT baseline_origin_x, FLOAT baseline_origin_y,
    DWRITE_MEASURING_MODE measuring_mode,
    const DWRITE_GLYPH_RUN* glyph_run,
    const DWRITE_GLYPH_RUN_DESCRIPTION* glyph_run_description,
    IUnknown* client_drawing_effect)
{
    HRESULT hr = S_OK;

    // XXX: Specifying any client drawing effect causes this to not draw.
    // This is used to implement the revealing text object.
    if (client_drawing_effect == nullptr)
    {
        ComPtr<ID2D1Factory> d2dFactory;
        d2d_target_->GetFactory(&d2dFactory);

        ComPtr<ID2D1PathGeometry> geom;
        hr = d2dFactory->CreatePathGeometry(&geom);
        if (FAILED(hr)) {
            return hr;
        }

        ComPtr<ID2D1GeometrySink> sink;
        hr = geom->Open(&sink);
        if (FAILED(hr)) {
            return hr;
        }

        hr = glyph_run->fontFace->GetGlyphRunOutline(
            glyph_run->fontEmSize,
            glyph_run->glyphIndices,
            glyph_run->glyphAdvances,
            glyph_run->glyphOffsets,
            glyph_run->glyphCount,
            glyph_run->isSideways,
            glyph_run->bidiLevel % 2,
            sink.Get());
        if (FAILED(hr)) {
            return hr;
        }

        hr = sink->Close();
        if (FAILED(hr)) {
            return hr;
        }

        D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Translation(
            baseline_origin_x, baseline_origin_y);

        ComPtr<ID2D1TransformedGeometry> transGeom;
        hr = d2dFactory->CreateTransformedGeometry(geom.Get(), &matrix, &transGeom);
        if (FAILED(hr)) {
            return hr;
        }

        if (stroke_brush_)
        {
            d2d_target_->DrawGeometry(transGeom.Get(), stroke_brush_.Get(), stroke_width_);
        }

        if (fill_brush_)
        {
            d2d_target_->FillGeometry(transGeom.Get(), fill_brush_.Get());
        }
    }

    return hr;
}

HRESULT OutlinedTextRenderer::DrawUnderline(void* client_drawing_context,
    FLOAT baseline_origin_x, FLOAT baseline_origin_y,
    const DWRITE_UNDERLINE* underline,
    IUnknown* client_drawing_effect)
{
    return E_NOTIMPL;
}

HRESULT OutlinedTextRenderer::DrawStrikethrough(void* client_drawing_context,
    FLOAT baseline_origin_x, FLOAT baseline_origin_y,
    const DWRITE_STRIKETHROUGH* underline,
    IUnknown* client_drawing_effect)
{
    return E_NOTIMPL;
}

HRESULT OutlinedTextRenderer::DrawInlineObject(void* client_drawing_context,
    FLOAT origin_x, FLOAT origin_y, IDWriteInlineObject* inline_object,
    BOOL is_sideways, BOOL is_right_toLeft, IUnknown* client_drawing_effect)
{
    return E_NOTIMPL;
}