#include "app/app_d2d.h"

#include <windows.h>

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

int g_win_w = 888+16;
int g_win_h = 506+38;

APPD2D::APPD2D()
    : hwnd_(nullptr) {
}
APPD2D::~APPD2D() { 
}

HRESULT APPD2D::Initialize() {

    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = APPD2D::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(LONG_PTR);
    wcex.hInstance = HINST_THISCOMPONENT;
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDI_APPLICATION);
    wcex.lpszClassName = L"D2DDemoApp";
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    hwnd_ = CreateWindow(
        L"D2DDemoApp",
        L"Direct2D_Demo_App",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        g_win_w, g_win_h,
        nullptr, nullptr,
        HINST_THISCOMPONENT,
        this );

    if (hwnd_){
        ShowWindow(hwnd_, SW_SHOWNORMAL);
        UpdateWindow(hwnd_);
        return S_OK;
    }
    return E_FAIL;
}

LRESULT CALLBACK APPD2D::WndProc(
        HWND hwnd,
        UINT message,
        WPARAM w_param,
        LPARAM l_param ){

    APPD2D* app_d2d = nullptr;
    if (WM_NCCREATE == message) {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)l_param;
        app_d2d = (APPD2D *)pcs->lpCreateParams;
        SetWindowLongPtrW( hwnd,
            GWLP_USERDATA,
            PtrToUlong(app_d2d));
    }
    else {
        app_d2d = reinterpret_cast<APPD2D *>(static_cast<LONG_PTR>(
            GetWindowLongPtrW( hwnd,
            GWLP_USERDATA)));
    }

    if (!app_d2d)
        return DefWindowProc(hwnd, message, w_param, l_param);

    switch (message) {
    case WM_SIZE: {
        if (app_d2d->render_thread_.get()){
            UINT width = LOWORD(l_param);
            UINT height = HIWORD(l_param);
            app_d2d->render_thread_->message_loop_proxy()->PostTask(FROM_HERE,
                base::Bind(&RenderThread::OnResize,
                base::Unretained(app_d2d->render_thread_.get()), width, height));
        }
        break;
    }
    case WM_DESTROY: {
        app_d2d->render_thread_->message_loop_proxy()->PostTask(FROM_HERE,
            base::Bind(&RenderThread::StopRender,
            base::Unretained(app_d2d->render_thread_.get())));
        app_d2d->render_thread_->Stop();
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hwnd, message, w_param, l_param);
    }
    return 0;
}

void APPD2D::Start() {

    base::Thread::Options options;
    options.message_loop_type = base::MessageLoop::TYPE_UI;
    render_thread_.reset(
        new RenderThread("RenderThread", hwnd_) );
    if (!render_thread_->StartWithOptions(options)){
        render_thread_->Stop();
        NOTREACHED();
    }

    render_thread_->message_loop_proxy()->PostTask(FROM_HERE,
        base::Bind(&RenderThread::StartRender,
        base::Unretained(render_thread_.get())));

}