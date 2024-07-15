# Marika

An Elden Ring mod launcher

## Purpose

Allows all users to play seamless coop on steam servers, even those on a steam emulator

## How it works

Lauches Elden Ring with a steam emulator then pipes all network requests through steam allowing the user to play on steam servers (counter for the seamless coop dev removing lan).

### setting seamless password

if you dont want to maually set the password for seamless coop, edit the string at these lines before you compile and the button to set the password will work https://github.com/Tacotakedown/Marika/blob/2609cdde4103a7f3f05520b220a5b7082f9f0abf/src/main.cpp#L273

### Steam users

Steam users are still required to launch with the launcher even though it is using the official steam servers.

### File Structure For non-Steam users

non steam users can manually select the folder that contains the `eldenring.exe` file by turning off the `Steam Install` checkbox, we will call this folder `${ROOT}`. Mod engine 2 must be installed to `${ROOT}` and the SeamlessCoop folder from ERSC should be located at `${ROOT}/SeamlessCoop`. The ERSC launcher cannot be used in this configuration, instead load the `SeamlessCoop\\ersc.dll` in the `external_dlls` section of your  `config_eldenring.toml` (which should be in `${ROOT}` if you installed mod engine 2 correctly

## Requirements

1. Elden Ring files >= v1.12 (small vesion mismatch seems to work fine, ive tested 1.12.0 with 1.12.3 with no issues)
2. DLC files (all users must have if you wanna play DLC)
3. [C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x64.exe)

## Building

```bash
    git clone --recursive https://github.com/Tacotakedown/Marika.git
    cd Marika
    mkdir build
    cd build
    cmake ..
    cmake --build --config Release .
    # release is built to build/bin/release/Marika.exe
```
