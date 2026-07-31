# Eagles Night EQ — VST3/AU Plugin

A working VST3/AU audio plugin featuring:
1. **Adjustable notch filter at 2.3kHz** — gain and Q are fully controllable
2. **"One of These Nights" EQ curve** — modeled after the warm, spacious 1975 Eagles sound

---

## The Sound Design

The EQ curve is based on the characteristic tonal profile of *One of These Nights* (Eagles, 1975):

| Band | Freq | Type | Default | Purpose |
|------|------|------|---------|---------|
| Low Shelf | 120 Hz | Shelf | +2.5 dB | Warm, full bass — that analog low-end weight |
| Low-Mid | 280 Hz | Peak | -2.8 dB | Clarity cut — removes mud, opens up the mix |
| Notch | 2.3 kHz | Peak | -6.0 dB | **User adjustable** — tame harshness or resonances |
| Presence | 4.5 kHz | Peak | +1.2 dB | Smooth upper-mid definition |
| High Shelf | 10 kHz | Shelf | +1.8 dB | Air and sparkle without harshness |

The result: warm lows, clear mids, smooth highs — that 1975 studio sound.

---

## Prerequisites

- [JUCE 7+](https://juce.com/download/) (GPL or commercial license)
- CMake 3.15+ **OR** Projucer (included with JUCE)
- A C++17 compiler (Visual Studio 2019+, Xcode, GCC/Clang)

---

## Build Instructions (CMake)

### 1. Get JUCE

```bash
cd EaglesVST
git clone https://github.com/juce-framework/JUCE.git
```

Or place your existing JUCE folder inside `EaglesVST/`.

### 2. Build

**macOS / Linux:**
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

**Windows (Visual Studio):**
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### 3. Locate the plugin

- **VST3**: `build/EaglesNightEQ_artefacts/Release/VST3/`
- **AU** (macOS): `build/EaglesNightEQ_artefacts/Release/AU/`
- **Standalone**: `build/EaglesNightEQ_artefacts/Release/Standalone/`

Copy the `.vst3` to your DAW's VST3 folder:
- **Windows**: `C:\Program Files\Common Files\VST3\`
- **macOS**: `~/Library/Audio/Plug-Ins/VST3/` (user) or `/Library/Audio/Plug-Ins/VST3/` (system)

---

## Build Instructions (Projucer)

1. Open Projucer (from your JUCE installation)
2. Create a new Audio Plugin project, or open the existing `.jucer` file if provided
3. Set **Plugin Formats** to VST3 and/or AU
4. Set **Plugin Characteristics**: `Plugin MIDI Input: No`, `Plugin MIDI Output: No`
5. Add the source files from `Source/`
6. Export to your IDE (Xcode / Visual Studio / Makefile)
7. Build in Release mode

---

## Controls

### 2.3kHz Notch Section
- **Gain**: -18 dB to +6 dB (default -6 dB). Negative = dip, positive = boost.
- **Q**: 0.5 to 10.0 (default 3.5). Higher = narrower notch.
- **Bypass**: Disables the notch while keeping the Eagles curve active.

### Eagles Curve Section
- **Low**: Low shelf gain at 120 Hz
- **Low-Mid**: Peaking cut at 280 Hz
- **Presence**: Peaking boost at 4.5 kHz
- **High**: High shelf gain at 10 kHz
- **Mix**: Blend between dry and processed signal (0–100%)

### Output Section
- **Gain**: Output trim (-24 dB to +24 dB)

---

## Using the Plugin

1. Insert on a track or bus in your DAW
2. Start with the **default settings** — they're tuned to the reference sound
3. Adjust the **2.3kHz notch** to tame any harshness in vocals, guitars, or mixes
4. Use the **Mix** knob to blend the Eagles curve subtly across a full mix
5. The **Standalone** build lets you process audio files or use as a live effect

---

## License

This source code is provided as-is for educational and personal use. JUCE is licensed separately under GPLv3 or commercial terms.
