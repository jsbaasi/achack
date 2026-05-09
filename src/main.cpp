#include "imgui.h"
#include "overlay.h"
#include <thread>
#include "renderer.h"
#include <Windows.h>
#include "common.h"
#include "imgui_impl_win32.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
DWORD WINAPI HookThread(LPVOID);
ImGuiKey ImGui_ImplWin32_KeyEventToImGuiKey(WPARAM wParam, LPARAM lParam);

int main(int, char **) {
    if (!FindWindowA("SDL_app", "AssaultCube")) {
        MessageBoxA(NULL, "Please run game first", "Error", MB_ICONERROR);
        return 0;
    }

    ImGui_ImplWin32_EnableDpiAwareness();
    const float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{0, 0}, MONITOR_DEFAULTTOPRIMARY));

    WNDCLASSEXW wc = {
        sizeof(wc), CS_VREDRAW | CS_HREDRAW, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr,
        nullptr, L"External", nullptr
    };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TRANSPARENT,
                                  wc.lpszClassName, L"Transparent_External", WS_POPUP, 100, 100,
                                  (int) (1280 * main_scale), (int) (800 * main_scale), nullptr, nullptr, wc.hInstance,
                                  nullptr);

    if (renderer::Setup(hwnd, wc, main_scale)) return 1;
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_ALPHA);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);



    if (overlay::Setup()) return 1;

    auto game_hwnd = FindWindowA("SDL_app", "AssaultCube");
    bool done = false;
    while (!done) {
        WINDOWINFO info{};
        BOOL res = GetWindowInfo(game_hwnd, &info);
        MoveWindow(hwnd, info.rcClient.left, info.rcClient.top, info.rcClient.right - info.rcClient.left,
                   info.rcClient.bottom - info.rcClient.top, true);

        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;
        if (renderer::BeginFrame()) continue;


        overlay::GetInput();
        overlay::Overlay();

        renderer::EndFrame();
    }

    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    renderer::Cleanup();

    return 0;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED)
                return 0;
            g_ResizeWidth = (UINT) LOWORD(lParam); // Queue resize
            g_ResizeHeight = (UINT) HIWORD(lParam);
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}