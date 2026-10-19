cmake_minimum_required(VERSION 3.22)

# Force toolchain per platform before project() detects the compiler.
if(APPLE OR UNIX)
    set(CMAKE_C_COMPILER   "clang"  CACHE FILEPATH "C compiler"   FORCE)
    set(CMAKE_CXX_COMPILER "clang++" CACHE FILEPATH "C++ compiler" FORCE)
elseif(WIN32)
    set(CMAKE_C_COMPILER   "cl"     CACHE FILEPATH "C compiler"   FORCE)
    set(CMAKE_CXX_COMPILER "cl"     CACHE FILEPATH "C++ compiler" FORCE)
endif()

project(SC VERSION 1.0.0 LANGUAGES C CXX OBJC OBJCXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build vendor dependencies as CMake targets (shared: box2d/imgui/ImGuizmo/yaml-cpp; static: glfw).
include(${CMAKE_CURRENT_LIST_DIR}/../vendor/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SC_RENDERER_METAL "Use Metal renderer" ON)
option(SC_RENDERER_OPENGL "Use OpenGL renderer" OFF)
option(SC_BUILD_EDITOR "Build with editor support" ON)
option(SC_DEBUG "Enable debug build" ON)

set(SC_RENDERER "Metal" CACHE STRING "Renderer backend: Metal or OpenGL")
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

file(GLOB_RECURSE SC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.m
)

if(APPLE)
    if(SC_RENDERER STREQUAL "OpenGL")
        add_definitions(-DSC_RENDERER_OpenGL)
        find_library(OPENGL_LIB OpenGL)
        list(APPEND SC_FRAMEWORKS ${OPENGL_LIB})
    else()
        add_definitions(-DSC_RENDERER_Metal)
        add_definitions(-DGLFW_EXPOSE_NATIVE_COCOA)
        find_library(METAL_LIB Metal)
        find_library(QUARTZCORE_LIB QuartzCore)
        find_library(FOUNDATION_LIB Foundation)
        list(APPEND SC_FRAMEWORKS ${METAL_LIB} ${QUARTZCORE_LIB} ${FOUNDATION_LIB})
    endif()
    find_library(IOKIT_LIB IOKit)
    find_library(COREVIDEO_LIB CoreVideo)
    find_library(COCOA_LIB Cocoa)
    list(APPEND SC_FRAMEWORKS ${IOKIT_LIB} ${COREVIDEO_LIB} ${COCOA_LIB})
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/../ext/include/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/../ext/include/stbi
    ${CMAKE_CURRENT_SOURCE_DIR}/../ext/include/box2d
)

set(MONO_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(${MONO_INCLUDE})

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../ext/lib/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/../ext/lib/stbi
    ${CMAKE_CURRENT_SOURCE_DIR}/../ext/lib/ImLog
    ${CMAKE_CURRENT_SOURCE_DIR}/../ext/lib/imgui-glfw-opengl
    /opt/homebrew/lib
)

add_library(SC SHARED ${SC_SOURCES})
set_target_properties(SC PROPERTIES
    LIBRARY_OUTPUT_NAME "SC"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(SC PRIVATE
    glfw
    yaml-cpp
    glad
    stbi
    box2d
    imgui
    imguizmo
    monosgen-2.0
    ${SC_FRAMEWORKS}
)

if(WIN32)
    target_link_libraries(SC PRIVATE opengl32 user32 gdi32 shell32 kernel32)
endif()

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

add_custom_command(TARGET SC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ../SandBox/lib/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ../SandBox/bin/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ../SCEditor/bin/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ../SCEditor/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SC> ../SCEditor/libs/${CMAKE_SHARED_LIBRARY_PREFIX}SC${CMAKE_SHARED_LIBRARY_SUFFIX}
)
