#ifndef TESTD2D_APP_RENDER_THREAD_H
#define TESTD2D_APP_RENDER_THREAD_H

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include "base/threading/thread.h"
#include "base/timer/timer.h"

class RenderThread
    : public base::Thread{

public:
    RenderThread(const char* thread_name, HWND hwnd_to_render);
    virtual ~RenderThread();

    //override from Thread
    virtual void Init() override;
    virtual void CleanUp() override;

    void StartRender();
    void StopRender();
    void OnResize(UINT width, UINT height);
    void OnRender();

private:
    HRESULT CreateDeviceIndependentResources();
    HRESULT CreateDeviceResources();
    void DiscardDeviceResources();

private:
    HWND hwnd_to_render_;
    int render_timer_interval_;
    scoped_ptr<base::RepeatingTimer<RenderThread>> render_timer_;

    ID2D1Factory* d2d_factory_;
    ID2D1HwndRenderTarget* render_target_;
    ID2D1SolidColorBrush* stroke_brush_;
    ID2D1SolidColorBrush* fill_brush_;
    IDWriteFactory* write_factory_;
    IDWriteTextFormat* text_fps_format_;
    IDWriteTextLayout* text_layout_;
    ID2D1PathGeometry* path_geometry_;
    ID2D1GeometrySink* geometry_sink_;

    DISALLOW_COPY_AND_ASSIGN(RenderThread);
};

#endif //TESTD2D_APP_RENDER_THREAD_H