# Eagles Night EQ

## What Changed (Bulletproof Version)
- Replaced `ProcessorChain` template with plain separate filters (no template issues)
- Removed ALL custom font code (no JUCE 7 Font API issues)
- Added file verification steps to workflow so we can see what's actually on disk
- Added error log capture if build fails

## To Build
Files must be at repo ROOT (not inside a subfolder):
```
CMakeLists.txt
Source/PluginProcessor.cpp
Source/PluginProcessor.h
Source/PluginEditor.cpp
Source/PluginEditor.h
.github/workflows/build.yml
```

Push to GitHub and check the Actions tab.
