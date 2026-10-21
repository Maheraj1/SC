cmake_minimum_required(VERSION 3.22)

project(SC VERSION 1.0.0 LANGUAGES C CXX OBJC OBJCXX)

# Toolchain is forced by the top-level CMakeLists.txt (clang on Apple/Unix).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Vendor dependencies (box2d, glfw, yaml-cpp, imgui, ImGuizmo) are built
# in-tree by the top-level CMakeLists.txt via add_subdirectory(vendor), so the
# real targets box2d/glfw/yaml-cpp/imgui/imguizmo are available here.

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate glfw from vendor build directory
find_library(GLFW_LIBRARY NAMES glfw3 glfw HINTS "${VENDOR_BUILD_DIR}/lib" NO_DEFAULT_PATH)
if(NOT GLFW_LIBRARY)
    message(FATAL_ERROR "Could not find glfw library. Build vendor dependencies first (vendor/build).")
endif()
message(STATUS "Found glfw: ${GLFW_LIBRARY}")

option(SC_RENDERER_METAL "Use Metal renderer" OFF)
option(SC_RENDERER_OPENGL "Use OpenGL renderer" ON)
option(SC_BUILD_EDITOR "Build with editor support" ON)
option(SC_DEBUG "Enable debug build" ON)

set(SC_RENDERER "OpenGL" CACHE STRING "Renderer backend: Metal or OpenGL")
set_property(CACHE SC_RENDERER PROPERTY STRINGS "Metal" "OpenGL")

add_compile_options(-Wall -Wno-undefined-var-template -Wno-deprecated-volatile -fPIC)

if(SC_DEBUG)
    add_compile_options(-g3)
    add_definitions(-DSC_DEBUG)
endif()

add_definitions(
    -DSC_RENDERER_${SC_RENDERER}
    -DGLFW_INCLUDE_NONE
    -DSC_WINDOW_API_GLFW
    -DSC_USE__asm__
    -DSC_EDITOR_IMPL
    -DSC_SHARED
)

# Collect all source files
file(GLOB_RECURSE SC_ALL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.m
)

# Filter out renderer backend sources (added conditionally below)
list(FILTER SC_ALL_SOURCES EXCLUDE REGEX "src/(Window|Renderer|Input)/(Metal|OpenGL)/")

# Add the selected renderer backend
if(SC_RENDERER STREQUAL "Metal")
    file(GLOB SC_BACKEND_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Metal/*.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Metal/*.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Metal/*.m
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Input/Metal/*.mm
    )
elseif(SC_RENDERER STREQUAL "OpenGL")
    file(GLOB SC_BACKEND_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/OpenGL/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/OpenGL/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Input/OpenGL/*.cpp
    )
else()
    message(FATAL_ERROR "Invalid SC_RENDERER '${SC_RENDERER}'. Must be 'Metal' or 'OpenGL'.")
endif()

set(SC_SOURCES ${SC_ALL_SOURCES} ${SC_BACKEND_SOURCES})

if(APPLE)
    find_library(METAL_LIB Metal)
    find_library(QUARTZCORE_LIB QuartzCore)
    find_library(FOUNDATION_LIB Foundation)
    find_library(IOKIT_LIB IOKit)
    find_library(COREVIDEO_LIB CoreVideo)
    find_library(COCOA_LIB Cocoa)
    list(APPEND SC_FRAMEWORKS ${METAL_LIB} ${QUARTZCORE_LIB} ${FOUNDATION_LIB} ${IOKIT_LIB} ${COREVIDEO_LIB} ${COCOA_LIB})

    if(SC_RENDERER STREQUAL "OpenGL")
        add_definitions(-DSC_RENDERER_OpenGL)
        find_library(OPENGL_LIB OpenGL)
        list(APPEND SC_FRAMEWORKS ${OPENGL_LIB})
    else()
        add_definitions(-DSC_RENDERER_Metal)
        add_definitions(-DGLFW_EXPOSE_NATIVE_COCOA)
    endif()
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glad/include
)

set(MONO_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(${MONO_INCLUDE})

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    /opt/homebrew/lib
)

add_library(SC SHARED ${SC_SOURCES})
set_target_properties(SC PROPERTIES
    LIBRARY_OUTPUT_NAME "SC"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(SC PRIVATE
    ${GLFW_LIBRARY}
    yaml-cpp
    box2d
    imgui
    imguizmo
    monosgen-2.0
    ${SC_FRAMEWORKS}
)

if(WIN32)
    target_link_libraries(SC PRIVATE opengl32 user32 gdi32 shell32 kernel32)
endif()

if(SC_RENDERER STREQUAL "Metal")
    set_source_files_properties(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Metal/MetalWindow.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Metal/MetalFrameBuffer.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Metal/MetalRenderer.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Metal/MetalShader.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Metal/MetalTexture.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Input/Metal/MetalInput.mm
        PROPERTIES COMPILE_FLAGS "-ObjC++ -fobjc-weak -fobjc-arc"
    )

    set_source_files_properties(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Metal/MetalTexture.m
        PROPERTIES COMPILE_FLAGS "-ObjC"
    )
endif()

add_custom_command(TARGET SC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ${CMAKE_CURRENT_SOURCE_DIR}/../SandBox/lib/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ${CMAKE_CURRENT_SOURCE_DIR}/../SandBox/bin/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ${CMAKE_CURRENT_SOURCE_DIR}/../SCEditor/bin/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ${CMAKE_CURRENT_SOURCE_DIR}/../SCEditor/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ${CMAKE_CURRENT_SOURCE_DIR}/../SCEditor/libs/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
)
