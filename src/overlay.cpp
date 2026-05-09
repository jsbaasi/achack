#include "imgui.h"
#include "pointer.h"
#include "common.h"
#include "glm/fwd.hpp"
#include "overlay.h"
#include <Windows.h>
#include "common.h"
#include "constants.h"
#include "IconsFontAwesome7.h"

#define IMVEC4(v) (v).x, (v).y, (v).z, (v).w

using namespace overlay;
void DebugMatrix(const char *label, const glm::mat4 &m) {
    ImGui::Text("%s:", label);
    // GLM is column-major; print as math rows (transposed display)
    for (int row = 0; row < 4; ++row) {
        ImGui::Text("[ %8.3f  %8.3f  %8.3f  %8.3f ]",
                    m[0][row], m[1][row], m[2][row], m[3][row]);
    }
    ImGui::Spacing();
}

void overlay::GetInput() {
    if (ImGui::Shortcut(constants::mod_menu_shortcut)) _gd._toggles._mod_menu = !_gd._toggles._mod_menu;
    if ((GetAsyncKeyState('1') & 1) && _gd._toggles._mod_menu) _gd._toggles._esp = !_gd._toggles._esp;
    if ((GetAsyncKeyState('2') & 1) && _gd._toggles._mod_menu) _gd._toggles._aimbot = !_gd._toggles._aimbot;
}

void overlay::Overlay() {
    if (_gd._toggles._mod_menu) {
        ModMenu();
    }
    if (_gd._toggles._esp) {
        Esp();
    }
}

int overlay::Setup() {
    if (int ec = init_dynamic_info("ac_client.exe", _da); ec) {
        MessageBoxA(NULL, ec == 1 ? "Failed to get pid" : "Failed to get base address", "Error", MB_ICONERROR);
        return 1;
    }
    init_game_info();
    if (_gd._entity_count == 0) {
        MessageBoxA(NULL, "Entity count is 0", "Error", MB_ICONERROR);
        return 1;
    }
    std::thread mem_read_thread{memory_read};
    std::thread esp_draw_thread{esp_data};
    mem_read_thread.detach();
    esp_draw_thread.detach();
    return 0;
}

void overlay::ModMenu() {
    ImGui::Begin("Mod Menu", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
    ImGui::Text("%lu", _gd._entity_count);
    ImGui::Text("x=%.2f,y=%.2f,z=%.2f", _gd._player_ent.player_x, _gd._player_ent.player_y, _gd._player_ent.player_z);
    ImGui::Text("<1>     %s ESP",    _gd._toggles._esp    ? ICON_FA_EYE        : ICON_FA_EYE_SLASH);
    ImGui::Text("<2>     %s Aimbot", _gd._toggles._aimbot ? ICON_FA_CROSSHAIRS : ICON_FA_BAN);
    ImGui::End();
}

void overlay::Esp() {
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    {
        std::scoped_lock lock(_ed._cuboid_mex);
        for (cuboid& coid : _ed._entity_cuboids) {
            coid.draw(draw_list, IM_COL32(0, 255, 249, 255), IM_COL32(255, 0, 0, 255), 1);
        }
    }
}