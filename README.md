# Scandium (SC)
## Building the project

The project now builds with CMake.  The old Makefiles are kept in place for
reference only (`SC-ScriptCore/Makefile`, `SCEditor/Makefile`).

### Prerequisites

- CMake 3.22 or newer
- A C++20 compiler (clang++ on macOS/Linux, cl on Windows)
- [.NET 6.0 SDK](https://dotnet.microsoft.com/download/dotnet/6.0) (for the C# script core)
- Vendor dependencies prebuilt in `vendor/build/`:
  ```shell
  cd vendor && cmake -S . -B build && cmake --build build
  ```

### Building

Configure and build everything (SC engine, C# script core, SCEditor):

```shell
./Build.sh
```

Or manually:

```shell
cmake -S . -B build
cmake --build build --parallel
```

### Build options

Pass options to `cmake -S . -B build -D<option>=<value>`:

| Option | Values | Default | Description |
|--------|--------|---------|-------------|
| `SC_BUILD_ENGINE` | ON / OFF | ON | Build the SC C++ engine library |
| `SC_BUILD_SCRIPT_CORE` | ON / OFF | ON | Build the C# script core via dotnet |
| `SC_BUILD_EDITOR` | ON / OFF | ON | Build the SCEditor application |
| `SC_BUILD_CONFIGURATION` | Debug / Release | Debug | Build configuration |
| `SC_RENDERER` | Metal / OpenGL | Metal | Renderer backend |

Example — Release build, editor only:

```shell
cmake -S . -B build \
    -DSC_BUILD_ENGINE=OFF \
    -DSC_BUILD_SCRIPT_CORE=OFF \
    -DSC_BUILD_CONFIGURATION=Release \
    -DSC_RENDERER=Metal
cmake --build build --parallel
```

### Incremental builds

CMake tracks file timestamps, so `cmake --build build` only recompiles or
re-copies what changed.  The C# script core is rebuilt via `dotnet build`
only when `SCEngine.csproj` or any `.cs` source is newer than the built DLL.

### Running

You can run the project from the debugger by selecting the target to run
and pressing F5, or launch `build/SCEditor` directly.

## Features
SC is not fully complete but has a lot of features:
- Batch Renderer
- Physics System based on box2d
- Custom shaders
- A FileSystem and Resource management system
- ApplicationAddons allows to create a overlay or add engine features (*Not stable*)
- Simple rendering setup which has build time graphics api selection *(e.g OpenGL, Metal, Vulkan, DirectX, etc)*
- Editor Application (*Not stable and **Experimental***)
## Optimizations
- Minimal use of heap memory
- Batch Renderer (*It Reduces the draw calls*)