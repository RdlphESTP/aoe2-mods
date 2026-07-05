# AoE2: The Age of Kings — Mods

A collection of mods for the original **Age of Empires II: The Age of Kings** (1999).

## Requirements

This project targets the **original Age of Kings** release (not *The Conquerors*).
You need a working install of the original game first (checkout tips under).

> [!IMPORTANT] 
> **Looking for The Conquerors?**
> You're probably looking for [UserPatch](https://userpatch.aiscripters.net/), the community patch for *AoE2: The Conquerors*.
> These mods were built for *The Age of Kings* only and haven't been tested on *The Conquerors*.

> [!TIP] 
> **Game won't launch on a modern PC?**
> If a fresh install doesn't start, try reinstalling from the original CD, then copy the [cnc-ddraw files](https://github.com/FunkyFr3sh/cnc-ddraw/releases/latest/download/cnc-ddraw.zip) in the game installation folder.

> [!TIP]
> Some copies require the CD to remain inserted. A no-CD patched executable is available on [Age of Kings Heaven](https://aok.heavengames.com/blacksmith/showfile.php?fileid=13742&f=2&st=0) if needed.

## Installation

1. Download the latest release from the [Releases page](https://github.com/RdlphESTP/aoe2-mods/releases/latest).
2. Copy `launcher.exe` into your Age of Empires II installation folder.
3. Copy the mod `.dll` file(s) into a `mods/` subfolder (create it if it doesn't exist).
4. Launch the game using `launcher.exe`, **not** the original `empires2.exe`.

> [!WARNING]
> Keep the original `empires2.exe` untouched. The launcher loads mods without modifying it.

## Available Mods

> [!NOTE] 
> **Drag Scroll** (`dragscroll.dll`)
> Adds middle-mouse drag scrolling to the game.
>
> **Controls**: Hold the middle mouse button and drag to scroll the map.

## Technical Details

For anyone interested in the reverse engineering side:

- **Target version**: Age of Empires II: The Age of Kings (1.0c)
- **Camera pointer**: `[[[empires2.exe+26FA60]+4C]+4]`
- **Camera coordinates (isometric grid)**: `+0x168`, `+0x16C`

## Building from source

Example using the MSVC compiler (`cl`):

```
cl /LD dragscroll.cpp user32.lib
cl launcher.cpp user32.lib
```

Any C++ compiler targeting **32-bit (x86)** should work, as long as the resulting
DLL matches the game's architecture.

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.

This project does not include or redistribute any part of the original game.
Age of Empires II: The Age of Kings is © Microsoft / Forgotten Empires.