# vendor/CMakeLists.txt
# Standalone build for the ./vendor repositories:
#   SHARED: box2d, imgui, ImGuizmo, yaml-cpp
#   STATIC: glfw
#
# Build this in its OWN build directory (e.g. ../vendor/build). Vendor is NOT
# part of the SC build tree anymore, so `make clean` on the SC project never
# removes vendor build artifacts:
#
#   cd ../vendor && cmake -S . -B build && cmake --build build
#   cmake -S SC -B build -DVENDOR_BUILD_DIR=../vendor/build
#
# Then SC links against the imported vendor targets below.

cmake_minimum_required(VERSION 3.22)
project(Vendor LANGUAGES C CXX)

# Suppress the per-config debug postfix (e.g. box2d -> box2dd, yaml-cpp ->
# yaml-cppd) so imported target locations stay valid for both Debug and
# Release builds of SC.
set(CMAKE_DEBUG_POSTFIX "")

set(VENDOR_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Static libs linked into shared libs need PIC (e.g. glfw static -> SC shared).
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Keep vendor artifacts together, outside the app's output tree.
set(VENDOR_LIB_DIR  "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(VENDOR_BIN_DIR  "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${VENDOR_LIB_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${VENDOR_LIB_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${VENDOR_BIN_DIR}")

# ------------------------------------------------------------------
# box2d — SHARED
# ------------------------------------------------------------------
# BUILD_SHARED_LIBS is set as a *normal* variable so it only shadows the cache
# inside the box2d add_subdirectory scope and is restored afterwards.
set(BUILD_SHARED_LIBS ON)
set(BOX2D_SAMPLES OFF)
set(BOX2D_BENCHMARKS OFF)
set(BOX2D_DOCS OFF)
set(BOX2D_UNIT_TESTS OFF)
set(BOX2D_PROFILE OFF)
set(BOX2D_VALIDATE OFF)
add_subdirectory("${VENDOR_SRC_DIR}/box2d" "${CMAKE_CURRENT_BINARY_DIR}/box2d")

set(BUILD_SHARED_LIBS OFF)

# ------------------------------------------------------------------
# glfw — STATIC
# ------------------------------------------------------------------
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_LIBRARY_TYPE STATIC)
add_subdirectory("${VENDOR_SRC_DIR}/glfw" "${CMAKE_CURRENT_BINARY_DIR}/glfw")

# ------------------------------------------------------------------
# yaml-cpp — SHARED
# ------------------------------------------------------------------
set(YAML_BUILD_SHARED_LIBS ON)
set(YAML_CPP_BUILD_TOOLS OFF)
set(YAML_CPP_BUILD_TESTS OFF)
set(YAML_CPP_INSTALL OFF)
set(YAML_CPP_FORMAT_SOURCE OFF)
add_subdirectory("${VENDOR_SRC_DIR}/yaml-cpp" "${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp")

# ------------------------------------------------------------------
# imgui — SHARED (vendor/imgui has no root CMakeLists.txt)
# Core four files only (no imgui_demo.cpp).
# ------------------------------------------------------------------
set(IMGUI_DIR "${VENDOR_SRC_DIR}/imgui")
add_library(imgui SHARED
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
)
target_include_directories(imgui PUBLIC "${IMGUI_DIR}")

# ------------------------------------------------------------------
# ImGuizmo — SHARED (vendor/ImGuizmo's root CMakeLists hardcodes STATIC)
# ------------------------------------------------------------------
set(IMGUIZMO_DIR "${VENDOR_SRC_DIR}/ImGuizmo")
add_library(imguizmo SHARED
    "${IMGUIZMO_DIR}/src/ImGuizmo.cpp"
    "${IMGUIZMO_DIR}/src/GraphEditor.cpp"
    "${IMGUIZMO_DIR}/src/ImCurveEdit.cpp"
    "${IMGUIZMO_DIR}/src/ImGradient.cpp"
    "${IMGUIZMO_DIR}/src/ImSequencer.cpp"
    "${IMGUIZMO_DIR}/src/ImVectorEditor.cpp"
)
target_include_directories(imguizmo PUBLIC
    "${IMGUIZMO_DIR}/src"
    "${IMGUI_DIR}"          # ImGuizmo includes <imgui.h>
)
target_link_libraries(imguizmo PUBLIC imgui)
