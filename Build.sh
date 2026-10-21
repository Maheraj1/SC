#!/bin/sh
# Build the Scandium project using CMake.
#
# Replaces the old Makefile-based build.  Configures and builds all enabled
# components (SC engine, C# script core, SCEditor) in one step.

set -e

BUILD_DIR="build"

echo "Configuring CMake..."
cmake -S . -B "${BUILD_DIR}"

echo "Building all components..."
cmake --build "${BUILD_DIR}" --parallel

echo "Build complete."
