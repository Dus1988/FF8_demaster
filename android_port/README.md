# Android port scaffolding (FF8 Demaster)

This directory provides a first implementation pass for Android-targeted demastering support, focused on reusable and platform-agnostic groundwork.

## Implemented pieces

- `archive_parity.*`
  - Verifies whether candidate `main.zzz` / `other.zzz` files follow the same structural assumptions used by the current demaster parser.
  - Produces explicit issues for malformed headers, entry metadata, and out-of-range offsets.

- `filesystem_android.*`
  - Replaces Windows path assumptions (`DEMASTER_EXP\\...`) with Android storage layout resolution.
  - Produces canonical paths for app root, mod root, export root, and hash output root.

- `address_map.*`
  - Adds per-build/per-ABI address table support through a simple CSV loader.
  - Enables lookup by `(build, abi, symbol)` to prepare ARM/ARM64 hook mapping.

- `runtime_target.*`
  - Models Android runtime targeting constraints (renderer backend, ABI, injection strategy, build fingerprint).
  - Validates mandatory fields before attempting runtime hooks.

- `runtime_probe_cli.cpp`
  - Minimal validation binary to exercise archive parity checks, address-map loading, and storage/runtime validation in one place.

## Build (host-side validation)

```bash
cmake -S /home/runner/work/FF8_demaster/FF8_demaster/android_port \
      -B /tmp/ff8_android_build
cmake --build /tmp/ff8_android_build
```

## Address map CSV format

One record per line:

```text
build,abi,symbol,address
```

Example:

```text
1.0.2,arm64-v8a,SET_RESOLUTION,0x12345678
```

## Next implementation steps

- Add Android-native loader/injection integration (`.so` startup path).
- Implement ARM64 hook backends that consume loaded address maps.
- Add GLES/EGL (and optional Vulkan) hook shims matching game renderer usage.
- Connect this module with shared texture replacement and hash-loading core logic.
