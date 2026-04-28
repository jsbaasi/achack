//
// Created by jujha on 27/04/2026.
//

#ifndef ACHACK_LOGGER_H
#define ACHACK_LOGGER_H

#include <cstdio>
#include <cstring>
#include <imgui.h>
#include <glm/glm.hpp>

// --- type handlers ---

inline void log_value(const char* name, float v)         { ImGui::Text("%-20s %.4f", name, v); }
inline void log_value(const char* name, int v)           { ImGui::Text("%-20s %d", name, v); }
inline void log_value(const char* name, bool v)          { ImGui::Text("%-20s %s", name, v ? "true" : "false"); }
inline void log_value(const char* name, const char* v)   { ImGui::Text("%-20s %s", name, v); }

inline void log_value(const char* name, glm::vec2 v)
{ ImGui::Text("%-20s (%.3f, %.3f)", name, v.x, v.y); }

inline void log_value(const char* name, glm::vec3 v)
{ ImGui::Text("%-20s (%.3f, %.3f, %.3f)", name, v.x, v.y, v.z); }

inline void log_value(const char* name, glm::vec4 v)
{ ImGui::Text("%-20s (%.3f, %.3f, %.3f, %.3f)", name, v.x, v.y, v.z, v.w); }

inline void log_value(const char* name, glm::mat4 v) {
    ImGui::Text("%s", name);
    for (int row = 0; row < 4; row++)
        ImGui::Text("  [ %.3f  %.3f  %.3f  %.3f ]", v[0][row], v[1][row], v[2][row], v[3][row]);
}

// --- core ---

template<typename... Args>
inline void log_window(const char* title, Args&&... args)
{
    ImGui::Begin(title);
    (args(), ...);  // each arg is a lambda that calls log_value
    ImGui::End();
}

// --- macros ---

// LOG_VAR(title, var1, var2, ...) — window title + any number of variables
#define LOG_VAR(title, ...) log_window(title, __VA_ARGS__)

// shortcut: auto-title from file+line
#define LOG_HERE(...) log_window( \
[]{ \
static char buf[128]; \
snprintf(buf, sizeof(buf), "%s:%d", __FILE__, __LINE__); \
return buf; \
}(), __VA_ARGS__)

// wrap a variable into a lambda for log_window
#define V(var) [&]{ log_value(#var, var); }
#endif //ACHACK_LOGGER_H
