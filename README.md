# OD Pedal

A professional-quality VST3 guitar overdrive pedal plugin built with JUCE 8.x and CMake.

## Features

- **VST3** plugin format for modern DAWs (Ableton Live, Studio One, Reaper, etc.)
- **MIDI Automation** support via `AudioProcessorValueTreeState`
- **Real-time audio processing** with zero allocations in audio thread
- **Modular DSP design** for reuse in future pedal chain projects
- **Production-ready structure** with clean separation of concerns

## Design Principles

- **DSP Decoupling:** Core algorithm in `OverdriveDSP` has zero JUCE dependencies for easy reuse
- **Real-time Safe:** All memory allocations happen during `prepare()`, not in `process()`
- **Automation Ready:** All parameters integrated with APVTS for DAW automation support
- **Scalable:** Structure supports future expansion to multi-effect chain

## Build Requirements

- **Visual Studio 2026** (with C++ workload) or compatible MSVC compiler
- **CMake 3.22+**
- **JUCE 8.x** (included as git submodule)

**Note:** Build script uses Visual Studio 18 2026 generator. If you have a different VS version, update the `-G` flag in `build.ps1` accordingly (e.g., `"Visual Studio 17 2022"` for VS 2022).

## Building

### Clone with Submodules

```bash
git clone --recursive https://github.com/erikcompanhone/od_pedal.git
cd od_pedal
```

### Build

Run the build script:

```powershell
.\build.ps1
```

Or with a specific configuration:

```powershell
.\build.ps1 -Configuration Debug
.\build.ps1 -Configuration Release
```

The VST3 plugin will be in `build/ODPedal_artefacts/{Configuration}/VST3/ODPedal.vst3`

## IntelliSense Configuration

VS Code may show IntelliSense errors about missing `BinaryData.h` members (e.g., `knob_png`, `bypass_up_png`, etc.) even though the project builds successfully. This is because the binary data header is generated during the CMake build process.

**The errors do not affect compilation or functionality** — they're purely IntelliSense squiggles.

### Fix IntelliSense Errors

A `.vscode/c_cpp_properties.json` file is included in the repository that configures IntelliSense to find the generated header:

```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/src",
                "${workspaceFolder}/src/plugin",
                "${workspaceFolder}/src/dsp",
                "${workspaceFolder}/build/src/juce_binarydata_ODPedalBinaryData/JuceLibraryCode",
                "${workspaceFolder}/third_party/JUCE/modules",
                "${workspaceFolder}/build/ODPedal_artefacts/JuceLibraryCode"
            ],
            ...
        }
    ]
}
```

**If you still see errors after building:**
1. Reload VS Code (`Ctrl+Shift+P` → "Developer: Reload Window")
2. The IntelliSense database should refresh and errors will disappear

This project uses **GitHub Actions** to automatically build on every push/PR to `main`.

- **Trigger:** `push` or `pull_request` on `main` branch
- **Platform:** Windows (latest runner)
- **Output:** VST3 artifact available in workflow run

See [`.github/workflows/ci.yml`](.github/workflows/ci.yml) for details.

## License

MIT License — see [LICENSE](LICENSE) for details.
