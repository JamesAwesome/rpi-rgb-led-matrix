# CLAUDE.md

## Project Overview

C++ library for controlling RGB LED display matrices on Raspberry Pi GPIO. Includes Python bindings (Cython), C bindings, and C# bindings.

## Build

```bash
# Build C++ library and all examples
make

# Build just the library
make -C lib

# Build just examples
make -C examples-api-use

# Clean
make clean
```

### Python bindings

```bash
pip install -e .    # Development install (requires Python 3.9+, Cython, scikit-build-core)
```

## CI

GitHub Actions runs on every push/PR (Ubuntu 24.04). Tests against `cython3-legacy` and `cython3`. The CI step is just `make` — there is no automated test suite.

## Code Style

- **C++ indentation**: 2 spaces (see `.editorconfig`)
- **Makefiles**: tabs for indentation
- **Namespace**: `rgb_matrix` for public API
- **Naming**: CamelCase for classes (`RGBMatrix`, `FrameCanvas`), UPPERCASE for constants/macros
- **C++ standard**: C++11 (Make), C++17 (CMake/Python bindings)
- **Headers**: files in `include/` use `// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-` marker
- **GPL 2.0+** license headers on source files

## Key Directories

- `lib/` — Core C++ library implementation
- `include/` — Public C++ headers (`led-matrix.h`, `canvas.h`, `graphics.h`)
- `bindings/python/` — Python bindings (Cython `.pyx` files)
- `examples-api-use/` — C++ example programs
- `utils/` — Utility programs (image viewer, video viewer)
- `fonts/` — BDF font files
- `adapter/` — Hardware adapter PCB designs

## Architecture Notes

- Canvas abstraction: `Canvas` (interface) → `RGBMatrix`, `FrameCanvas`
- Private implementation via inner `Impl` classes
- GPIO access requires root on Raspberry Pi
- No exceptions in library code (`-fno-exceptions`)
