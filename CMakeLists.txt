cmake_minimum_required(VERSION 3.25)
project(LeftAtriumModel LANGUAGES C CXX CUDA)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(LEFT_ATRIUM_FETCH_GLFW "Download GLFW if not found on the system" ON)

find_package(OpenGL REQUIRED)

# Try to use an installed GLFW first, otherwise fetch it.
find_package(glfw3 CONFIG QUIET)
if(NOT TARGET glfw AND NOT TARGET glfw3::glfw)
    if(LEFT_ATRIUM_FETCH_GLFW)
        include(FetchContent)
        FetchContent_Declare(
            glfw
            URL https://github.com/glfw/glfw/archive/refs/tags/3.3.9.zip
        )
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(glfw)
    else()
        message(FATAL_ERROR "glfw3 not found and LEFT_ATRIUM_FETCH_GLFW is OFF")
    endif()
endif()

set(IMGUI_SOURCES
    Source/imgui/imgui.cpp
    Source/imgui/imgui_draw.cpp
    Source/imgui/imgui_tables.cpp
    Source/imgui/imgui_widgets.cpp
    Source/imgui/imgui_impl_glfw.cpp
    Source/imgui/imgui_impl_opengl3.cpp
)

add_executable(svt
    Source/SVT.cu
    Source/glad.c
    ${IMGUI_SOURCES}
)

set_target_properties(svt PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_ARCHITECTURES native
)

target_include_directories(svt PRIVATE
    Source
    Source/imgui
    include
    include/glad
)

set(_glfw_target "")
if(TARGET glfw3::glfw)
    set(_glfw_target glfw3::glfw)
elseif(TARGET glfw)
    set(_glfw_target glfw)
endif()

set(_opengl_targets OpenGL::GL)
if(TARGET OpenGL::GLU)
    list(APPEND _opengl_targets OpenGL::GLU)
endif()

target_link_libraries(svt PRIVATE
    ${_glfw_target}
    ${_opengl_targets}
    $<$<BOOL:${WIN32}>:glu32>
)

# Helper CUDA tool for generating node/muscle files
add_executable(make_nodes_and_muscles
    MakeNodesAndMuscleFiles/makeNodesAndMusclesFile.cu
)
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_ARCHITECTURES native
)

# Group common include dirs for the helper target if it ever needs them
# (currently only uses CUDA/CRT headers).
