#ifndef TESTD2D_APP_OUTLINE_TEXT_RENDER_H
#define TESTD2D_APP_OUTLINE_TEXT_RENDER_H

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

void DrawOutlinedTextLayout(ID2D1RenderTarget* d2d_target,
    IDWriteTextLayout* text_layout, ID2D1Brush* fill_brush,
    ID2D1Brush* stroke_brush, float stroke_width,
    const D2D1_POINT_2F& origin);

class OutlinedTextRenderer : public IDWriteTextRenderer {

public:

	static ComPtr<OutlinedTextRenderer> Create(
		ID2D1RenderTarget* d2d_target, ID2D1Brush* fill_brush,
		ID2D1Brush* stroke_brush, float stroke_width);

	// IUnknown implementation
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IDWritePixelSnapping implementation
	STDMETHOD(IsPixelSnappingDisabled)(void* client_drawing_context,
		BOOL* is_disabled);
	STDMETHOD(GetCurrentTransform)(void* client_drawing_context,
		DWRITE_MATRIX* transform);
	STDMETHOD(GetPixelsPerDip)(void* client_drawing_context,
		FLOAT* pixels_per_dip);

	// IDWriteTextRenderer implementation
	STDMETHOD(DrawGlyphRun)(void* client_drawing_context,
		FLOAT baseline_origin_x, FLOAT baseline_origin_y,
		DWRITE_MEASURING_MODE measuring_mode,
		const DWRITE_GLYPH_RUN* glyph_run,
		const DWRITE_GLYPH_RUN_DESCRIPTION* glyph_run_description,
		IUnknown* client_drawing_effect);
	STDMETHOD(DrawUnderline)(void* client_drawing_context,
		FLOAT baseline_origin_x, FLOAT baseline_origin_y,
		const DWRITE_UNDERLINE* underline,
		IUnknown* client_drawing_effect);
	STDMETHOD(DrawStrikethrough)(void* client_drawing_context,
		FLOAT baseline_origin_x, FLOAT baseline_origin_y,
		const DWRITE_STRIKETHROUGH* strikethrough,
		IUnknown* client_drawing_effect);
	STDMETHOD(DrawInlineObject)(void* client_drawing_context,
		FLOAT origin_x, FLOAT origin_y,
		IDWriteInlineObject* inline_object,
		BOOL is_sideways, BOOL is_right_toLeft,
		IUnknown* client_drawing_effect);

private:
	OutlinedTextRenderer(
		ID2D1RenderTarget* d2d_target, ID2D1Brush* fill_brush,
		ID2D1Brush* stroke_brush, float stroke_width);
	~OutlinedTextRenderer();

	ULONG ref_count_;
	ComPtr<ID2D1RenderTarget> d2d_target_;
	ComPtr<ID2D1Brush> fill_brush_;
	ComPtr<ID2D1Brush> stroke_brush_;
	float stroke_width_;
};


#endif //TESTD2D_APP_OUTLINE_TEXT_RENDER_H

