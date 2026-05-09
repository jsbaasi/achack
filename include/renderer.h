#ifndef ACHACK_RENDERER_H
#define ACHACK_RENDERER_H

#include <Windows.h>

inline UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

namespace renderer {
    int Setup(const HWND& hwnd, const WNDCLASSEXW& wc, const float& main_scale);
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
    int BeginFrame();
    void EndFrame();
    void Cleanup();
}

#endif //ACHACK_RENDERER_H