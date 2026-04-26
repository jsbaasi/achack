cmake_minimum_required(VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(IMGUI_SRCS
        "external/imgui/imgui.cpp"
        "external/imgui/imgui_demo.cpp"
        "external/imgui/imgui_draw.cpp"
        "external/imgui/imgui_impl_dx11.cpp"
        "external/imgui/imgui_impl_win32.cpp"
        "external/imgui/imgui_tables.cpp"
        "external/imgui/imgui_widgets.cpp"
        #debugging stuff
        "external/imgui/imgui.natvis"
        "external/imgui/imgui.natstepfilter"
        #std::string
        "external/imgui/imgui_stdlib.cpp"
)

project(achack)

# entry point
add_executable(${PROJECT_NAME}
        "src/main.cpp"
        "src/gamememory.cpp"
        "src/thread_memory_read.cpp"
        "src/thread_esp_data.cpp"
        "src/pointer.cpp"
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
        <windows.h>
        "external/imgui/imgui.h"
        "external/imgui/imconfig.h"
        "external/imgui/imgui_impl_dx11.h"
        "external/imgui/imgui_impl_win32.h"
        "external/glm/glm.hpp"
)

target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE
        "include/"
        "external/"
        "external/imgui"
)

target_link_directories(${PROJECT_NAME} PRIVATE
        "lib/"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        d3d11
        dxgi
        d3dcompiler
        dwmapi
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
)