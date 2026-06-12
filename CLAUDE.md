# CLAUDE.md

## Project Overview

C++ library for controlling RGB LED display matrices on Raspberry Pi GPIO. Supports Pi 4 (BCM2711) and Pi 5 (RP1) — the SoC is detected at runtime and the appropriate GPIO backend is selected. Includes Python bindings (Cython), C bindings, and C# bindings.

This is the `jamesawesome` fork. The default `main` branch is based on kingdo9's `pi5_support` branch (upstream PR [hzeller#1886](https://github.com/hzeller/rpi-rgb-led-matrix/pull/1886), maintainer-approved) with one local patch: 42 anonymous `PIO` parameters in `lib/rp1/rp1_pio_vendor/piolib/pio_rp1.c` were given a name so the file builds under bullseye GCC 10 (Debian 11). The pre-RP1 codebase is preserved on the `pi4_legacy` branch as a rollback point. Once #1886 merges into `hzeller/master`, retire this fork.

## Hardware Backends

Selected at runtime:

- **BCM2711 (Pi 4 / Pi 3 / earlier)**: classic GPIO path in `lib/gpio.cc` + `lib/framebuffer.cc`.
- **RP1 (Pi 5)**: `lib/rp1/rp1_rio_backend.cc` (RIO mode, default, faster but more CPU) or `lib/rp1/rp1_pio_backend.cc` (PIO mode, low CPU). Vendored piolib under `lib/rp1/rp1_pio_vendor/`. The CLI accepts `--led-rp1-pio=0|1` to pick the backend (default 0 = RIO; 1 forces PIO). The old `--led-rp1-rio` flag and `RuntimeOptions::rp1_rio` field were renamed to `rp1_pio` upstream (June 2026) — the default backend flipped from PIO to RIO at the same time. For chain ≥ 2 with flicker, raise `--led-slowdown-gpio` from 2 to 3+ (range now 0..60).

## Build

```bash
# Make (legacy, simplest)
make                       # build C++ library and all examples
make -C lib                # just the library
make -C examples-api-use   # just examples
make clean

# CMake (added alongside the Pi 5 work)
cmake -B build && cmake --build build
```

### Python bindings

```bash
pip install -e .    # Python 3.9+, Cython, scikit-build-core
```

The old `setup.py` was replaced by `pyproject.toml` + `scikit-build-core`. Cython sources live in `bindings/python/rgbmatrix/*.pyx`.

## CI

GitHub Actions (`.github/workflows/c-cpp.yml`) runs on every push/PR (Ubuntu 24.04). Tests against `cython3-legacy` and `cython3`. The CI step is just `make` — there is no automated test suite.

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
- `lib/rp1/` — Pi 5 RP1 backends (PIO + RIO) and vendored piolib
- `include/` — Public C++ headers (`led-matrix.h`, `canvas.h`, `graphics.h`)
- `bindings/python/` — Python bindings (Cython `.pyx` files)
- `bindings/c#/` — C# bindings (restructured vs. the legacy fork)
- `examples-api-use/` — C++ example programs
- `utils/` — Utility programs (image viewer, video viewer, text scroller)
- `fonts/` — BDF font files
- `adapter/` — Hardware adapter PCB designs
- `RT-kernel/` — Real-time kernel artifacts and setup guide (Trixie)

## Architecture Notes

- Canvas abstraction: `Canvas` (interface) → `RGBMatrix`, `FrameCanvas`
- Private implementation via inner `Impl` classes
- GPIO access requires root on Raspberry Pi (or `--privileged` in Docker)
- No exceptions in library code (`-fno-exceptions`)
- Runtime SoC/backend selection happens early in `lib/led-matrix.cc` — extend that file for new variants.

## When Updating This File

Keep notes brief — this is a fork and divergence is a liability. If you change something a downstream consumer cares about (build flags, runtime CLI, public headers), note it here so the next person doesn't have to re-discover it from `git log`.
