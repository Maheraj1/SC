# Scandium (SC) — top-level CMake build
#
# Replaces the old Makefile-based build system.  Builds the C++ engine,
# the C# script core, and the C++ editor from a single configure/build step.
#
# Usage:
#   cmake -S . -B build
#   cmake --build build --parallel
#
# Options:
#   SC_BUILD_ENGINE      Build the SC C++ shared library            (ON)
#   SC_BUILD_SCRIPT_CORE Build the C# script core via dotnet         (ON)
#   SC_BUILD_EDITOR      Build the SCEditor C++ application         (ON)
#   SC_BUILD_CONFIGURATION Debug | Release                          (Debug)
#   SC_RENDERER          Metal | OpenGL                             (Metal)

cmake_minimum_required(VERSION 3.22)
project(Scandium VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------
# Vendor include directories (propagate to SC subdirectory)
# Vendor binaries must be prebuilt in vendor/build/ (or present as
# prebuilt .dylib/.a in SCEditor/bin/ and ext/lib/).
# ------------------------------------------------------------------
set(_VENDOR_SRC "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
    CACHE PATH "Vendor source directory")
set(VENDOR_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/build"
    CACHE PATH "Directory where vendor dependencies were built")
include_directories(
    ${_VENDOR_SRC}/box2d/include
    ${_VENDOR_SRC}/box2d/extern/glad/include
    ${_VENDOR_SRC}/glfw/include
    ${_VENDOR_SRC}/glm
    ${_VENDOR_SRC}/imgui
    ${_VENDOR_SRC}/imgui/backends
    ${_VENDOR_SRC}/ImGuizmo/src
    ${_VENDOR_SRC}/stb
    ${_VENDOR_SRC}/yaml-cpp/include
    ${_VENDOR_SRC}/yaml-cpp/src
)

link_directories(${VENDOR_BUILD_DIR}/lib)

# ------------------------------------------------------------------
# Build toggles
# ------------------------------------------------------------------
option(SC_BUILD_ENGINE     "Build the SC C++ engine library"   ON)
option(SC_BUILD_SCRIPT_CORE "Build the C# script core via dotnet" ON)
option(SC_BUILD_EDITOR     "Build the SCEditor application"     ON)

set(SC_BUILD_CONFIGURATION "Debug" CACHE STRING "Build configuration: Debug or Release")
set_property(CACHE SC_BUILD_CONFIGURATION PROPERTY STRINGS "Debug" "Release")

# ------------------------------------------------------------------
# Sub-projects
# ------------------------------------------------------------------

if(SC_BUILD_ENGINE)
    add_subdirectory(SC)
endif()

if(SC_BUILD_SCRIPT_CORE)
    add_subdirectory(SC-ScriptCore)
endif()

if(SC_BUILD_EDITOR)
    add_subdirectory(SCEditor)
endif()

# ------------------------------------------------------------------
# Convenience targets
# ------------------------------------------------------------------

# Build everything (engine -> script-core -> editor)
if(SC_BUILD_ENGINE AND SC_BUILD_SCRIPT_CORE AND SC_BUILD_EDITOR)
    add_custom_target(all-components
        DEPENDS SC sc-engine-copy SCEditor
    )
elseif(SC_BUILD_ENGINE AND SC_BUILD_SCRIPT_CORE)
    add_custom_target(all-components
        DEPENDS SC sc-engine-copy
    )
elseif(SC_BUILD_ENGINE AND SC_BUILD_EDITOR)
    add_custom_target(all-components
        DEPENDS SC SCEditor
    )
elseif(SC_BUILD_SCRIPT_CORE AND SC_BUILD_EDITOR)
    add_custom_target(all-components
        DEPENDS sc-engine-copy SCEditor
    )
elseif(SC_BUILD_ENGINE)
    add_custom_target(all-components DEPENDS SC)
elseif(SC_BUILD_SCRIPT_CORE)
    add_custom_target(all-components DEPENDS sc-engine-copy)
elseif(SC_BUILD_EDITOR)
    add_custom_target(all-components DEPENDS SCEditor)
else()
    add_custom_target(all-components)
endif()
