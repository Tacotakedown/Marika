# Elden ring mod launcher GUI

## Purpose

Allows Steam users to play seamless coop with crack users without continuously needing to swap files around

## How it works

The launcher will automatically write the proper files to the game directory and launch with the proper mod configuration. If the user opts for persistent launching, the user will be able to launch
seamless coop from the `modengine2_launcher.exe` until they disable it in the launcher. On the other hand, the launcher will revert back to the steam version upon closing the game meaning that every
time the user would like to play seamless, they have to launch from this launcher.

## Requirements

1. Steam version of Elden Ring (thats the only reason to use this)
2. [C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x64.exe)

## Building

```bash
    git clone --recursive https://github.com/Tacotakedown/EldedRingModLauncher.git
    cd EldedRingModLauncher
    mkdir build
    cd build
    cmake ..
    cmake --build --config Release .
    # release is built to build/bin/release/EldenRingModLauncher.exe
```