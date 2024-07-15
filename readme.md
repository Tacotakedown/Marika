# Marika

An Elden Ring mod launcher

## Purpose

Allows all users to play seamless coop on steam servers, even those on a steam emulator

## How it works

Lauches Elden Ring with a steam emulator then pipes all network requests through steam allowing the user to play on steam servers (counter for the seamless coop dev removing lan).

## Requirements

1. Elden ring >= v1.12 
2. [C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x64.exe)

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
