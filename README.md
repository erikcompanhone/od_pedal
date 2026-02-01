# OD Pedal

A professional-quality VST3 guitar overdrive pedal plugin built with JUCE 8.x and CMake.

## Features

- **VST3** plugin format for modern DAWs (Ableton Live, Studio One, Reaper, etc.)
- **MIDI Automation** support via `AudioProcessorValueTreeState`
- **Real-time audio processing** with zero allocations in audio thread
- **Modular DSP design** for reuse in future pedal chain projects
- **Production-ready structure** with clean separation of concerns

## Build Requirements

- **Windows 10+** with MSVC 2019+ or Clang
- **CMake 3.22+**
- **Ninja** (for parallel builds)
- **JUCE 8.x** (included as git submodule)

## Building

### Clone with Submodules

```bash
git clone --recursive https://github.com/erikcompanhone/od_pedal.git
cd od_pedal
```

### Configure & Build

```bash
# Configure
cmake -S . -B build -G "Ninja Multi-Config"

# Build (Release)
cmake --build build --config Release --parallel

# VST3 plugin will be at:
# build/ODPedal.vst3/
```

## Design Principles

- **DSP Decoupling:** Core algorithm in `OverdriveDSP` has zero JUCE dependencies for easy reuse
- **Real-time Safe:** All memory allocations happen during `prepare()`, not in `process()`
- **Automation Ready:** All parameters integrated with APVTS for DAW automation support
- **Scalable:** Structure supports future expansion to multi-effect chain

## CI/CD

This project uses **GitHub Actions** to automatically build on every push/PR to `main`.

- **Trigger:** `push` or `pull_request` on `main` branch
- **Platform:** Windows (latest runner)
- **Output:** VST3 artifact available in workflow run

See [`.github/workflows/ci.yml`](.github/workflows/ci.yml) for details.

## License

MIT License — see [LICENSE](LICENSE) for details.
