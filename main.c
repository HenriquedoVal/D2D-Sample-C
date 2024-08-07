#define UNICODE
#define _UNICODE
#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>

#include <Windows.h>

#include "d2d1_c.h"
#include "d2d1_helpers.c"

#pragma comment(lib, "user32")
#pragma comment(lib, "d2d1")


static void discard_graphic_resources(ID2D1HwndRenderTarget *target, ID2D1SolidColorBrush *brush)
{
    if (brush != NULL) {
        brush->lpVtbl->Release(brush);
        brush = NULL;
    }

    if (target != NULL) {
        target->lpVtbl->Release(target);
        target = NULL;
    }
}


LRESULT CALLBACK WindowProc(HWND window, unsigned msg, WPARAM wparam, LPARAM lparam)
{
    static ID2D1Factory *factory;
    static ID2D1HwndRenderTarget *target = NULL;
    static ID2D1SolidColorBrush *brush = NULL;

    HRESULT hr;

    switch (msg) {
        case WM_CREATE:
            hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
                                   &IID_ID2D1Factory,
                                   NULL,
                                   (void **)&factory);

            if (FAILED(hr))
                return -1;
            return 0;
            
        case WM_PAINT:
            if (target == NULL) {
                RECT rc;
                GetClientRect(window, &rc);
                D2D1_SIZE_U size_u = {(unsigned)rc.right, (unsigned)rc.bottom};

                D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_prop = {
                    .hwnd = window,
                    .pixelSize = size_u,
                    .presentOptions = D2D1_PRESENT_OPTIONS_NONE
                };

                D2D1_RENDER_TARGET_PROPERTIES prop = helper_RenderTargetProperties();

                hr = factory->lpVtbl->CreateHwndRenderTarget(
                    factory,
                    &prop,
                    &hwnd_prop,
                    &target
                );
                if (FAILED(hr))
                    return -1;
            }

            if (brush == NULL) {
                D2D1_COLOR_F yellow = { .r = 1.0f, .g = 1.0f, .a = 1.0f };
                D2D1_BRUSH_PROPERTIES brush_prop = helper_BrushProperties();
                hr = target->lpVtbl->CreateSolidColorBrush(target, &yellow, &brush_prop, &brush);
                if (FAILED(hr))
                    return -2;
            }

            PAINTSTRUCT ps;
            BeginPaint(window, &ps);
            target->lpVtbl->BeginDraw(target);

            D2D1_COLOR_F sky_blue = { .r = 0.529f, .g = 0.807f, .b = 0.921f, .a = 1.0f };
            target->lpVtbl->Clear(target, &sky_blue);

            D2D1_SIZE_F size_f;
            target->lpVtbl->GetSize(target, &size_f);
            const float x = size_f.width / 2;
            const float y = size_f.height / 2;
            const float radius = min(x, y);

            D2D1_POINT_2F center = { x, y };
            D2D1_ELLIPSE ellipse = { center, radius, radius };
            target->lpVtbl->FillEllipse(target, &ellipse, (ID2D1Brush *)brush);

            D2D1_TAG tag1, tag2;
            hr = target->lpVtbl->EndDraw(target, &tag1, &tag2);

            if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
                discard_graphic_resources(target, brush);
            
            EndPaint(window, &ps);

            return 0;

        case WM_SIZE:
            if (target == NULL)
                return 0;

            RECT rc;
            GetClientRect(window, &rc);
            D2D1_SIZE_U size_u = {(unsigned)rc.right, (unsigned)rc.bottom};
            target->lpVtbl->Resize(target, &size_u);

            int ret = InvalidateRect(window, NULL, FALSE);
            assert(ret);

            return 0;


        case WM_DESTROY:
            discard_graphic_resources(target, brush);
            if (factory != NULL)
                factory->lpVtbl->Release(factory);
            PostQuitMessage(0);

            return 0;
    }

    return DefWindowProc(window, msg, wparam, lparam);
}


int WINAPI wWinMain(HINSTANCE instance, HINSTANCE _prev_inst, PWSTR cmd_line, int cmd_show)
{
    const wchar_t class_name[] = L"D2D Sample Class";

    WNDCLASS wc = {
        .lpfnWndProc = WindowProc,
        .hInstance = instance,
        .lpszClassName = class_name
    };

    RegisterClass(&wc);

    HWND window = CreateWindowEx(
        0,
        class_name,
        L"Title",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        instance,
        NULL
    );

    if (window == NULL)
        return 1;

    ShowWindow(window, cmd_show);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
