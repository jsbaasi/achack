#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Windows.h>
#include "imgui.h"

namespace constants {
	inline constexpr float z_near{0.15};
	inline constexpr float z_far{875};
	inline constexpr float gap_between_top{0.5};
	inline constexpr float gap_between_bottom{4};
	inline constexpr float gap_between_right{1};
	inline constexpr float gap_between_front{1};
	// offset from base
	inline constexpr DWORD code_cave_offset{0xD9329};
	inline constexpr DWORD yaw_setting_offset{0xBF922};
	inline constexpr DWORD pitch_setting_offset{0xBF957};
    inline constexpr ImGuiKeyChord mod_menu_shortcut = ImGuiMod_Ctrl | ImGuiKey_M;
}

#endif