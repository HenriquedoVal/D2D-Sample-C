#include "d2d1_c.h"


D2D1_MATRIX_3X2_F helper_IdentityMatrix(void)
{
    D2D1_MATRIX_3X2_F identity = {0};

    identity._11 = 1.f;
    identity._12 = 0.f;
    identity._21 = 0.f;
    identity._22 = 1.f;
    identity._31 = 0.f;
    identity._32 = 0.f;

    return identity;
}


// BrushProperties(
//     _In_ FLOAT opacity = 1.0,
//     _In_ CONST D2D1_MATRIX_3X2_F &transform = D2D1::IdentityMatrix()
//     )
D2D1_BRUSH_PROPERTIES helper_BrushProperties(void)
{
    return (D2D1_BRUSH_PROPERTIES) {
        .opacity = 1.0f,
        .transform = helper_IdentityMatrix()
    };
}


// PixelFormat(
//     _In_ DXGI_FORMAT dxgiFormat = DXGI_FORMAT_UNKNOWN,
//     _In_ D2D1_ALPHA_MODE alphaMode = D2D1_ALPHA_MODE_UNKNOWN
//     )
D2D1_PIXEL_FORMAT helper_PixelFormat(void)
{
    return (D2D1_PIXEL_FORMAT) {
        .format = DXGI_FORMAT_UNKNOWN,
        .alphaMode = D2D1_ALPHA_MODE_UNKNOWN
    };
}

// RenderTargetProperties(
//     D2D1_RENDER_TARGET_TYPE type =  D2D1_RENDER_TARGET_TYPE_DEFAULT,
//     _In_ CONST D2D1_PIXEL_FORMAT &pixelFormat = D2D1::PixelFormat(),
//     FLOAT dpiX = 0.0,
//     FLOAT dpiY = 0.0,
//     D2D1_RENDER_TARGET_USAGE usage = D2D1_RENDER_TARGET_USAGE_NONE,
//     D2D1_FEATURE_LEVEL  minLevel = D2D1_FEATURE_LEVEL_DEFAULT
//     )
D2D1_RENDER_TARGET_PROPERTIES helper_RenderTargetProperties(void)
{
    return (D2D1_RENDER_TARGET_PROPERTIES) {
        .type = 0,
        .pixelFormat = helper_PixelFormat(),
        .dpiX = 0.0,
        .dpiY = 0.0,
        .usage = D2D1_RENDER_TARGET_USAGE_NONE,
        .minLevel = D2D1_FEATURE_LEVEL_DEFAULT
    };
}
