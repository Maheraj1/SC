# SCEditor CMakeLists.txt
# Builds the SCEditor C++ application.
#
# Links against:
#   - libSC            (built by ../SC/CMakeLists.dll,  found in ../SC/bin/)
#   - glfw, box2d, yaml-cpp, imgui, imguizmo   (vendor/build/lib/ or SCEditor/bin/)
#
# Incremental: only changed .cpp files are recompiled; unchanged targets
# are skipped by CMake's normal up-to-date checks.

cmake_minimum_required(VERSION 3.22)
project(SCEditor LANGUAGES C CXX OBJC OBJCXX)

# ------------------------------------------------------------------
# C++ standard
# ------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------
# Options (mirror the old Makefile flags)
# ------------------------------------------------------------------
option(SC_DEBUG "Enable debug build" ON)

set(SC_RENDERER "OpenGL" CACHE STRING "Renderer backend: Metal or OpenGL")
set_property(CACHE SC_RENDERER PROPERTY STRINGS "Metal" "OpenGL")

# ------------------------------------------------------------------
# Paths
# ------------------------------------------------------------------
set(SC_SOURCE_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/../SC")
set(SC_BIN_DIR      "${SC_SOURCE_DIR}/bin")
set(VENDOR_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/build"
    CACHE PATH "Directory where vendor dependencies were built")
set(VENDOR_SRC_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/../vendor")
set(EDITOR_BIN_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# ------------------------------------------------------------------
# Helper: find a library in vendor build dir or SCEditor/bin/
# ------------------------------------------------------------------
macro(_sc_editor_find_library VAR)
    set(_NAMES ${ARGN})
    find_library(${VAR}
        NAMES ${_NAMES}
        HINTS
            "${VENDOR_BUILD_DIR}/lib"
            "${EDITOR_BIN_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}"
        NO_DEFAULT_PATH
    )
    if(NOT ${VAR})
        message(FATAL_ERROR "Could not find library. "
                            "Build vendor dependencies first (vendor/build) "
                            "or ensure prebuilt .dylib/.a files exist in SCEditor/bin/.")
    endif()
    message(STATUS "Found ${VAR}: ${${VAR}}")
endmacro()

# ------------------------------------------------------------------
# Imported targets
# ------------------------------------------------------------------

# SC shared library: reuse the target created by ../SC/CMakeLists.txt if it
# was built as part of this tree; otherwise import the prebuilt .dylib.
if(NOT TARGET SC)
    add_library(SC SHARED IMPORTED)
    set_target_properties(SC PROPERTIES
        IMPORTED_LOCATION "${SC_BIN_DIR}/libSC.dylib"
        INTERFACE_INCLUDE_DIRECTORIES "${SC_SOURCE_DIR}/include"
    )
endif()

# Vendor dependencies
_sc_editor_find_library(GLFW_LIBRARY     glfw3 glfw)
_sc_editor_find_library(BOX2D_LIBRARY    box2dd box2d)
_sc_editor_find_library(YAML_CPP_LIBRARY yaml-cpp)
_sc_editor_find_library(IMGUIZMO_LIBRARY imguizmo)

# Build imgui from source using vendor/imgui (v1.93) core and backends.
# Prebuilt .dylib files in SCEditor/bin/ are x86_64-only and
# won't link on arm64.
set(IMGUI_DIR "${VENDOR_SRC_DIR}/imgui")
set(IMGUI_BACKEND_DIR "${VENDOR_SRC_DIR}/imgui/backends")

set(IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp"
)

if(SC_RENDERER STREQUAL "Metal")
    list(APPEND IMGUI_SOURCES
        "${IMGUI_BACKEND_DIR}/imgui_impl_metal.mm"
    )
elseif(SC_RENDERER STREQUAL "OpenGL")
    list(APPEND IMGUI_SOURCES
        "${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp"
    )
endif()

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    "${IMGUI_DIR}"
    "${IMGUI_BACKEND_DIR}"
    "${VENDOR_SRC_DIR}/glfw/include"
    "${VENDOR_SRC_DIR}/glad/include"
)
set_target_properties(imgui PROPERTIES
    LINKER_LANGUAGE CXX
)

# For the Metal backend .mm file
if(SC_RENDERER STREQUAL "Metal" AND APPLE)
    set_source_files_properties(
        "${IMGUI_BACKEND_DIR}/imgui_impl_metal.mm"
        PROPERTIES COMPILE_FLAGS "-ObjC++ -fobjc-weak -fobjc-arc"
    )
endif()

# ------------------------------------------------------------------
# Include directories
# ------------------------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SC_SOURCE_DIR}/include
    ${VENDOR_SRC_DIR}/glm
    ${VENDOR_SRC_DIR}/glfw/include
    ${VENDOR_SRC_DIR}/imgui
    ${VENDOR_SRC_DIR}/imgui/backends
    ${VENDOR_SRC_DIR}/ImGuizmo/src
    ${VENDOR_SRC_DIR}/stb
    ${VENDOR_SRC_DIR}/box2d/include
    ${VENDOR_SRC_DIR}/yaml-cpp/include
    ${VENDOR_SRC_DIR}/yaml-cpp/src
)

# ------------------------------------------------------------------
# Compile definitions (match the old Makefile)
# ------------------------------------------------------------------
add_definitions(
    -DSC_RENDERER_${SC_RENDERER}
    -DGLFW_INCLUDE_NONE
    -DSC_WINDOW_API_GLFW
    -DSC_USE__asm__
    -DSC_EDITOR_IMPL
)

if(SC_DEBUG)
    add_definitions(-DSC_DEBUG)
endif()

# ------------------------------------------------------------------
# Compile options
# ------------------------------------------------------------------
add_compile_options(
    -Wall
    -Wno-undefined-var-template
    -Wno-deprecated-volatile
    -fPIC
)

if(SC_DEBUG)
    add_compile_options(-g3)
endif()

# ------------------------------------------------------------------
# Source files
# ------------------------------------------------------------------
file(GLOB EDITOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm
)

# ------------------------------------------------------------------
# Executable target
# ------------------------------------------------------------------
add_executable(SCEditor ${EDITOR_SOURCES})

set_target_properties(SCEditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${EDITOR_BIN_DIR}"
)

target_link_libraries(SCEditor PRIVATE
    SC
    ${GLFW_LIBRARY}
    ${BOX2D_LIBRARY}
    ${YAML_CPP_LIBRARY}
    imgui
    ${IMGUIZMO_LIBRARY}
)

# ------------------------------------------------------------------
# macOS frameworks
# ------------------------------------------------------------------
if(APPLE)
    find_library(METAL_LIB Metal)
    find_library(QUARTZCORE_LIB QuartzCore)
    find_library(FOUNDATION_LIB Foundation)
    find_library(IOKIT_LIB IOKit)
    find_library(COREVIDEO_LIB CoreVideo)
    find_library(COCOA_LIB Cocoa)

    if(SC_RENDERER STREQUAL "Metal")
        target_link_libraries(SCEditor PRIVATE
            ${METAL_LIB}
            ${QUARTZCORE_LIB}
            ${FOUNDATION_LIB}
            ${IOKIT_LIB}
            ${COREVIDEO_LIB}
            ${COCOA_LIB}
        )
    elseif(SC_RENDERER STREQUAL "OpenGL")
        find_library(OPENGL_LIB OpenGL)
        target_link_libraries(SCEditor PRIVATE
            ${OPENGL_LIB}
            ${QUARTZCORE_LIB}
            ${FOUNDATION_LIB}
            ${IOKIT_LIB}
            ${COREVIDEO_LIB}
            ${COCOA_LIB}
        )
    endif()
endif()
