# Eagles Night EQ — VST3/AU Plugin

## Quick Start for GitHub Actions Build

1. **Download the source** from the release or clone this repo
2. **Push to GitHub** — the `.github/workflows/build.yml` file triggers automatic builds
3. Go to **Actions** tab → wait ~5-10 minutes → download your `.vst3` artifact

## Manual Build

```bash
git clone https://github.com/juce-framework/JUCE.git
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

## Features
- Adjustable notch at 2.3kHz (gain + Q)
- Eagles "One of These Nights" curve: 120Hz shelf, 280Hz cut, 4.5kHz presence, 10kHz air
- Dry/wet mix + output gain
