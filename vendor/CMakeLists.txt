# vendor/CMakeLists.txt
# Builds the ./vendor repositories as CMake targets:
#   SHARED: box2d, imgui, ImGuizmo, yaml-cpp
#   STATIC: glfw
#
# Include this from your top-level CMakeLists.txt:
#
#   include(${CMAKE_CURRENT_LIST_DIR}/vendor/CMakeLists.txt)
#
# The top-level project must require CMake >= 3.22 (box2d's requirement).

set(VENDOR_DIR "${CMAKE_CURRENT_LIST_DIR}")

# ------------------------------------------------------------------
# Global settings
# ------------------------------------------------------------------
# Static libs linked into shared libs need PIC (e.g. glfw static -> SC shared).
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Keep vendor artifacts together so they don't pollute the app's output tree.
set(VENDOR_LIB_DIR  "${CMAKE_CURRENT_BINARY_DIR}/vendor/lib")
set(VENDOR_BIN_DIR  "${CMAKE_CURRENT_BINARY_DIR}/vendor/bin")
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
add_subdirectory("${VENDOR_DIR}/box2d" "${VENDOR_BIN_DIR}/box2d")

set(BUILD_SHARED_LIBS OFF)

# ------------------------------------------------------------------
# glfw — STATIC
# ------------------------------------------------------------------
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_LIBRARY_TYPE STATIC)
add_subdirectory("${VENDOR_DIR}/glfw" "${VENDOR_BIN_DIR}/glfw")

# ------------------------------------------------------------------
# yaml-cpp — SHARED
# ------------------------------------------------------------------
set(YAML_BUILD_SHARED_LIBS ON)
set(YAML_CPP_BUILD_TOOLS OFF)
set(YAML_CPP_BUILD_TESTS OFF)
set(YAML_CPP_INSTALL OFF)
set(YAML_CPP_FORMAT_SOURCE OFF)
add_subdirectory("${VENDOR_DIR}/yaml-cpp" "${VENDOR_BIN_DIR}/yaml-cpp")

# ------------------------------------------------------------------
# imgui — SHARED (vendor/imgui has no root CMakeLists.txt)
# Core four files only (no imgui_demo.cpp).
# ------------------------------------------------------------------
set(IMGUI_DIR "${VENDOR_DIR}/imgui")
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
set(IMGUIZMO_DIR "${VENDOR_DIR}/ImGuizmo")
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

# ------------------------------------------------------------------
# Convenience aliases (guarded because box2d and yaml-cpp create theirs already)
# ------------------------------------------------------------------
if(NOT TARGET box2d::box2d)
    add_library(box2d::box2d ALIAS box2d)
endif()
if(NOT TARGET glfw::glfw)
    add_library(glfw::glfw ALIAS glfw)
endif()
if(NOT TARGET yaml-cpp::yaml-cpp)
    add_library(yaml-cpp::yaml-cpp ALIAS yaml-cpp)
endif()
if(NOT TARGET imgui::imgui)
    add_library(imgui::imgui ALIAS imgui)
endif()
if(NOT TARGET imguizmo::imguizmo)
    add_library(imguizmo::imguizmo ALIAS imguizmo)
endif()

# ------------------------------------------------------------------
# Restore output dirs so the app's own targets keep default locations
# ------------------------------------------------------------------
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "")
