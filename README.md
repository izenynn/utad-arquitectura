# U-Tad Architecture Engine

## Info

2D "Super Pang"-Inspired game created with my own 2D Game Engine made with C++
for the "Architecture" subject of the master's degree in video game programming
at [U-Tad](https://u-tad.com/en/).

- [Info](#info)
- [Build](#build)

## Build

### Dependencies:

- [CMake](https://cmake.org/download/) (>= 3.18).
- [Ninja](https://ninja-build.org/).

### 1. Configure with `Ninja`:

```bash
cmake -B build -G "Ninja Multi-Config" .
```

Alternatively, use a [Visual Studio generator](https://cmake.org/cmake/help/v3.18/manual/cmake-generators.7.html#visual-studio-generators)
to create a solution:

```bash
# 64-bit
cmake -B build -G "Visual Studio 17 2022" -A x64 .
# 32-bit
cmake -B build -G "Visual Studio 17 2022" -A Win32 .
```

### 2. Build:

```bash
cmake --build build --config Release
```

The [Ninja Multi-Config](https://cmake.org/cmake/help/v3.18/generator/Ninja%20Multi-Config.html)
generator lets you use other build types ([`CMAKE_BUILD_TYPE`](https://cmake.org/cmake/help/v3.18/variable/CMAKE_BUILD_TYPE.html))
like "`Debug`" or "`RelWithDebInfo`", apart from "`Release`".

The resulting binaries are in `build/<build-type>/`.

### Windows notes

To build and use `CMake` you can open a developer powershell, or load the
Visual Studio command-line tools by running [`vcvarsall.bat`](https://learn.microsoft.com/en-us/cpp/build/building-on-the-command-line#developer_command_file_locations) (locations vary) in your shell:
```
# 32-bit
path\to\vcvarsall\vcvarsall.bat x86
# 64-bit
path\to\vcvarsall\vcvarsall.bat amd64
```

### Linux notes

If you are using Linux I'm sure you know how to install CMake and Ninja in
your distro.

### MacOS notes

1. Install [Homebrew](https://brew.sh/), which automatically installs the XCode command line tools.

2. Install [CMake](https://formulae.brew.sh/formula/cmake) and [Ninja](https://formulae.brew.sh/formula/ninja) using Homebrew:

```
brew install cmake
brew install ninja
```

3. Configure/build using the generic instructions above.

##

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/powered-by-coffee.svg)](https://forthebadge.com)
