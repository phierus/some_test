#ifndef TESTD2D_APP_APP_D2D_H
#define TESTD2D_APP_APP_D2D_H

#include "app/render_thread.h"

#include "base/macros.h"

class APPD2D{

public:
    APPD2D();
    virtual ~APPD2D();

    HRESULT Initialize();
    void Start();

private:
    static LRESULT CALLBACK WndProc(
        HWND hwnd,
        UINT message,
        WPARAM w_param,
        LPARAM l_param );

    void Render();
    HRESULT OnRender();
    void OnResize(UINT width, UINT height);


    void StartRenderTimer(int interval_millisec);
    void StopRenderTimer();

private:
    HWND hwnd_;
    scoped_ptr<RenderThread> render_thread_;

    DISALLOW_COPY_AND_ASSIGN(APPD2D);
};
#endif //TestD2D_app_app_d2d_h