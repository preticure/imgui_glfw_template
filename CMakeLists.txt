cmake_minimum_required(VERSION 3.23)
project("Dear ImGui")

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.5 REQUIRED)

add_executable(app_framework)

target_sources(app_framework
    PRIVATE
        src/main.cpp
        src/glad/gl.c
        src/imgui/imgui.cpp
        src/imgui/imgui_demo.cpp
        src/imgui/imgui_draw.cpp
        src/imgui/imgui_tables.cpp
        src/imgui/imgui_widgets.cpp
        src/imgui/backends/imgui_impl_glfw.cpp
        src/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(app_framework
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui/backends
)

target_link_libraries(app_framework
    PRIVATE
        glfw
)