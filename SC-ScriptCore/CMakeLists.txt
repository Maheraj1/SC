# SC-ScriptCore CMakeLists.txt
# Builds the C# script engine (SCEngine.csproj) via the .NET SDK and copies
# the resulting SCEngine.dll / SCEngine.pdb into the editor's Libs folder.
#
# dotnet is required; CMake will fatally abort if it is not found.

cmake_minimum_required(VERSION 3.22)
project(SC-ScriptCore LANGUAGES NONE)

# ------------------------------------------------------------------
# Locate the .NET SDK (required)
# ------------------------------------------------------------------
find_program(DOTNET_EXECUTABLE NAMES dotnet)
if(NOT DOTNET_EXECUTABLE)
    message(FATAL_ERROR
        "dotnet SDK not found. Install the .NET 6.0 SDK to build SC-ScriptCore."
    )
endif()
message(STATUS "Found dotnet: ${DOTNET_EXECUTABLE}")

# ------------------------------------------------------------------
# Options
# ------------------------------------------------------------------
set(SC_BUILD_CONFIGURATION "Debug" CACHE STRING "Build configuration: Debug or Release")
set_property(CACHE SC_BUILD_CONFIGURATION PROPERTY STRINGS "Debug" "Release")

set(SC_TARGET_FRAMEWORK "net6.0" CACHE STRING "Target framework for the script core")

# Output directory for the copied dll/pdb (SCEditor/bin/Libs/)
set(SC_SCRIPTCORE_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../SCEditor/bin/Libs"
    CACHE PATH "Directory where SCEngine.dll and SCEngine.pdb are copied")

# ------------------------------------------------------------------
# Source discovery (for incremental dependency tracking)
# ------------------------------------------------------------------
set(SCENGINE_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/SCEngine/SCEngine.csproj")
set(SCENGINE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SCEngine")

file(GLOB_RECURSE SCENGINE_CSHARP_SOURCES "${SCENGINE_SOURCE_DIR}/**/*.cs")

# ------------------------------------------------------------------
# Build output locations (matches `dotnet build -c <cfg> -f net6.0`)
# ------------------------------------------------------------------
set(SCENGINE_BUILD_DIR "${SCENGINE_SOURCE_DIR}/bin/${SC_BUILD_CONFIGURATION}/${SC_TARGET_FRAMEWORK}")
set(SCENGINE_DLL "${SCENGINE_BUILD_DIR}/SCEngine.dll")
set(SCENGINE_PDB "${SCENGINE_BUILD_DIR}/SCEngine.pdb")

# ------------------------------------------------------------------
# Custom command: `dotnet build`
# Outputs are tracked so CMake skips rebuilding when nothing changed.
# ------------------------------------------------------------------
add_custom_command(
    OUTPUT "${SCENGINE_DLL}" "${SCENGINE_PDB}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SCENGINE_BUILD_DIR}"
    COMMAND ${DOTNET_EXECUTABLE} build "${SCENGINE_PROJECT}"
            -c "${SC_BUILD_CONFIGURATION}"
            -f "${SC_TARGET_FRAMEWORK}"
    DEPENDS "${SCENGINE_PROJECT}" "${SCENGINE_CSHARP_SOURCES}"
    WORKING_DIRECTORY "${SCENGINE_SOURCE_DIR}"
    COMMENT "Building SC-ScriptCore (dotnet build ${SC_BUILD_CONFIGURATION})"
    VERBATIM
)

add_custom_target(sc-engine
    DEPENDS "${SCENGINE_DLL}" "${SCENGINE_PDB}"
)

# ------------------------------------------------------------------
# Custom command: copy dll + pdb into SCEditor/bin/Libs/
# ------------------------------------------------------------------
add_custom_command(
    OUTPUT "${SC_SCRIPTCORE_OUTPUT_DIR}/SCEngine.dll"
            "${SC_SCRIPTCORE_OUTPUT_DIR}/SCEngine.pdb"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SC_SCRIPTCORE_OUTPUT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy "${SCENGINE_DLL}"  "${SC_SCRIPTCORE_OUTPUT_DIR}/SCEngine.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${SCENGINE_PDB}"  "${SC_SCRIPTCORE_OUTPUT_DIR}/SCEngine.pdb"
    DEPENDS "${SCENGINE_DLL}" "${SCENGINE_PDB}"
    COMMENT "Copying SCEngine.dll and SCEngine.pdb to SCEditor/bin/Libs/"
    VERBATIM
)

add_custom_target(sc-engine-copy
    DEPENDS "${SC_SCRIPTCORE_OUTPUT_DIR}/SCEngine.dll"
            "${SC_SCRIPTCORE_OUTPUT_DIR}/SCEngine.pdb"
)
add_dependencies(sc-engine-copy sc-engine)
