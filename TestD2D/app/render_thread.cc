#include "app/render_thread.h"
#include "app/outline_text_render.h"

#include <Mmsystem.h>    

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "winmm.lib")

#define USE_TRANS  0

template<class Interface>
inline void SafeRelease(
    Interface **ppInterfaceToRelease
    ) {
    if (*ppInterfaceToRelease != NULL) {
        (*ppInterfaceToRelease)->Release(); 
        (*ppInterfaceToRelease) = NULL;
    }
}

FLOAT g_danmaku_speed = 0;
int g_msec_per_screen = 8000;
float g_offset = 0.0f;
base::Time g_last_time;

float GetFPS() {

    static float  fps = 0;
    static int    frameCount = 0;
    static float  currentTime = 0.0f;
    static float  lastTime = 0.0f;

    frameCount++;
    currentTime = timeGetTime()*0.001f;

    if (currentTime - lastTime > 1.0f) {
        fps = (float)frameCount / (currentTime - lastTime);
        lastTime = currentTime;
        frameCount = 0;
    }
    return fps;
}


RenderThread::RenderThread(const char* thread_name, HWND hwnd_to_render)
    : base::Thread(thread_name)
    , hwnd_to_render_(hwnd_to_render)
    , render_timer_interval_(33)
    , d2d_factory_(nullptr)
    , render_target_(nullptr)
    , stroke_brush_(nullptr)
    , fill_brush_(nullptr)
    , write_factory_(nullptr)
    , text_fps_format_(nullptr)
    , text_layout_(nullptr)
    , path_geometry_(nullptr)
    , geometry_sink_(nullptr)
{

}

RenderThread::~RenderThread() {

}


void RenderThread::Init(){

    CleanUp();

    if (FAILED(CreateDeviceIndependentResources())){
        DCHECK(TRUE);
    }

//   FLOAT dpiX, dpiY;
//   d2d_factory_->GetDesktopDpi(&dpiX, &dpiY);
//   UINT w = static_cast<UINT>(ceil(g_win_w * dpiX / 96.f));
//   UINT h = static_cast<UINT>(ceil(g_win_h * dpiY / 96.f));
//   SetWindowPos(hwnd_to_render_, NULL, 240, 180, w, h, SWP_NOSIZE);

    RECT rc;
    GetClientRect(hwnd_to_render_, &rc);

    DWRITE_TEXT_METRICS text_metrics;
    text_layout_->GetMetrics(&text_metrics);

    g_danmaku_speed = (text_metrics.width + (rc.right - rc.left)*1.0f) / g_msec_per_screen;
}

void RenderThread::CleanUp(){

    SafeRelease(&d2d_factory_);
    SafeRelease(&render_target_);
    SafeRelease(&stroke_brush_);
    SafeRelease(&fill_brush_);
    SafeRelease(&write_factory_);
    SafeRelease(&text_fps_format_);
    SafeRelease(&text_layout_);
    SafeRelease(&path_geometry_);
    SafeRelease(&geometry_sink_);
}

HRESULT RenderThread::CreateDeviceIndependentResources() {

    HRESULT hr = S_OK;
    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory_)))
        return E_FAIL;

    if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(write_factory_),
        reinterpret_cast<IUnknown**>(&write_factory_))))
        return E_FAIL;

    SafeRelease(&path_geometry_);
    SafeRelease(&geometry_sink_);

    if (FAILED(d2d_factory_->CreatePathGeometry(&path_geometry_))){
        return E_FAIL;
    }
    if (FAILED(path_geometry_->Open(&geometry_sink_))){
        return E_FAIL;
    }
    geometry_sink_->Close();

    static const WCHAR font_name[] = L"΢���ź�";
    static const FLOAT font_size = 30.0f;
    hr = write_factory_->CreateTextFormat(
        font_name,
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        font_size,
        L"zh-cn",    //local
        &text_fps_format_);
    if (SUCCEEDED(hr)){
        text_fps_format_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        text_fps_format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        text_fps_format_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    }

    static const WCHAR txt_contents[] = L"...test text...";
    static const UINT string_length = ARRAYSIZE(txt_contents)-1;
    hr = write_factory_->CreateTextLayout(
        txt_contents,
        string_length,
        text_fps_format_,
        400,    //max width
        1000,   //max height
        &text_layout_);
    DWRITE_TEXT_RANGE text_range = { 0, string_length };
    //text_layout_->SetUnderline(TRUE, text_range);

    return hr;
}

HRESULT RenderThread::CreateDeviceResources() {

    HRESULT hr = S_OK;
    if (!render_target_) {

        RECT rc;
        GetClientRect(hwnd_to_render_, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
            );

        hr = d2d_factory_->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd_to_render_, size),
            &render_target_
            );

        if (SUCCEEDED(hr)) {
            hr = render_target_->CreateSolidColorBrush(
                //D2D1::ColorF(D2D1::ColorF::LightSlateGray),
                D2D1::ColorF(D2D1::ColorF::White),
                &stroke_brush_ );
        }
        if (SUCCEEDED(hr)) {
            hr = render_target_->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
                &fill_brush_ );
        }

    }
    return hr;
}

void RenderThread::DiscardDeviceResources() {

    SafeRelease(&render_target_);
    SafeRelease(&stroke_brush_);
    SafeRelease(&fill_brush_);
}

void RenderThread::StartRender() {

    if (!render_timer_.get()) {
        render_timer_.reset(new base::RepeatingTimer<RenderThread>());
    }
    else{
        render_timer_->Stop();
        render_timer_.reset(nullptr);
    }

    if (!render_timer_->IsRunning()) {
        render_timer_->Start(FROM_HERE,
            base::TimeDelta::FromMilliseconds(render_timer_interval_),
            this, &RenderThread::OnRender);
    }
}

void RenderThread::StopRender() {

    if (render_timer_.get()) {
        render_timer_->Stop();
        render_timer_.reset(nullptr);
    }
}

void RenderThread::OnResize(UINT width, UINT height) {

    if (render_target_) {
        render_target_->Resize(D2D1::SizeU(width, height));
    }
    DWRITE_TEXT_METRICS text_metrics;
    text_layout_->GetMetrics(&text_metrics);
    g_danmaku_speed = (text_metrics.width + width*1.0f) / g_msec_per_screen;
}

void RenderThread::OnRender() {

    HRESULT hr = CreateDeviceResources();

    if (SUCCEEDED(hr) && !(render_target_->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED)) {

#if USE_TRANS  
        auto calculate_transform = [this](D2D1_MATRIX_3X2_F *transform){
            //if (g_last_time.is_null()) {
            //    g_last_time = base::Time::Now();
            //}
            //base::TimeDelta time_delta = base::Time::Now() - g_last_time;
            //float t = 2 * (time_delta.InMilliseconds() % 8000) / 8000.0f;
            //if (t > 1.0f){
            //    t = 2 - t;
            //}

            //D2D1_SIZE_F rt_size = render_target_->GetSize();
            //float translation_offset = (t - 0.5f) * 100;// rt_size.width;
            //LOG(INFO) << "-----------------------------------" << translation_offset;
            //*transform = D2D1::Matrix3x2F::Translation(translation_offset + rt_size.width, 100);

            D2D1_SIZE_F rt_size = render_target_->GetSize();
            DWRITE_TEXT_METRICS text_metrics;
            text_layout_->GetMetrics(&text_metrics);

            if (g_last_time.is_null()) {
                g_offset = 0.0f;
            }
            else{
                base::TimeDelta time_delta = base::Time::Now() - g_last_time;
                int delta_msec = time_delta.InMilliseconds();
                g_offset += g_danmaku_speed*delta_msec;
            }
            g_last_time = base::Time::Now();
            if (g_offset - text_metrics.width - rt_size.width > 0) {
                g_offset = 0;
            }
            *transform = D2D1::Matrix3x2F::Translation(rt_size.width-g_offset, 100);
        };
        D2D1_MATRIX_3X2_F transform;
        calculate_transform(&transform);
#endif

        render_target_->BeginDraw();

#if USE_TRANS  
        render_target_->SetTransform(transform);
#else
        render_target_->SetTransform(D2D1::Matrix3x2F::Identity());
#endif

        render_target_->Clear(D2D1::ColorF(D2D1::ColorF::Black));

#if USE_TRANS  
        DWRITE_TEXT_METRICS text_metrics;
        text_layout_->GetMetrics(&text_metrics);
        render_target_->DrawTextLayout(
            D2D1::Point2F(-text_metrics.width / 2.0f, -text_metrics.height / 2.0f),
            text_layout_,
            stroke_brush_,
            D2D1_DRAW_TEXT_OPTIONS_NO_SNAP );

        //fps
        static WCHAR fps_str[50] = { 0 };
        memset(fps_str, 0, 50 * sizeof(WCHAR));
        swprintf(fps_str, L"FPS:%0.3f\n", GetFPS());
        SetWindowText(hwnd_to_render_, fps_str);
        LOG(INFO) << fps_str;
#else
        D2D1_SIZE_F rt_size = render_target_->GetSize();

        DWRITE_TEXT_METRICS text_metrics;
        text_layout_->GetMetrics(&text_metrics);


        if ( g_last_time.is_null() ) {
            g_offset = 0.0f;
        }
        else{
            base::TimeDelta time_delta = base::Time::Now() - g_last_time;
            int64 delta_msec = time_delta.InMilliseconds();
            float delta_offset = g_danmaku_speed*delta_msec;
            g_offset += delta_offset;
        }
        g_last_time = base::Time::Now();
        if (g_offset - text_metrics.width - rt_size.width > 0) {
            g_offset = 0;
        }

//       static const WCHAR danmaku_str[] = L"...test text...";
//       render_target_->DrawText(
//           danmaku_str,
//           static_cast<UINT>(wcsnlen(danmaku_str, ARRAYSIZE(danmaku_str))),
//           text_fps_format_,
//           //D2D1::RectF(rt_size.width - g_offset, 0, g_offset + text_metrics.width, text_metrics.height),
//           D2D1::RectF(rt_size.width - g_offset, 0, rt_size.width, text_metrics.height),
//           stroke_brush_);
        
//       render_target_->DrawTextLayout(
//           D2D1::Point2F(rt_size.width - g_offset, 0),
//           text_layout_,
//           stroke_brush_,
//           D2D1_DRAW_TEXT_OPTIONS_NO_SNAP );

        DrawOutlinedTextLayout(render_target_,
            text_layout_, fill_brush_,
            stroke_brush_, 3,
            D2D1::Point2F(rt_size.width - g_offset, 0));

        //fps
        static WCHAR fps_str[50] = { 0 };
        memset(fps_str, 0, 50 * sizeof(WCHAR));
        swprintf(fps_str, L"FPS:%0.3f, POS: %0.3f\n", GetFPS(), rt_size.width - g_offset);
        SetWindowText(hwnd_to_render_, fps_str);
        LOG(INFO) << fps_str;
#endif

        hr = render_target_->EndDraw();
    }

    if (hr == D2DERR_RECREATE_TARGET) {
        hr = S_OK;
        DiscardDeviceResources();
    }

    InvalidateRect(hwnd_to_render_, nullptr, FALSE);
    //return hr;
}