
# Tarradino

This is a port of Apogee's 3D action title Rise of the Triad, originally
released in 1994. This port duplicates the functionality of the original
game on modern operating systems, including Linux, Windows, and Mac OS X.

Portability fixes include support for 64-bit and big-endian systems.

This port makes use of the Simple Direct Media Layer, or SDL, and an additional
library named SDL_mixer.

## Known Issues

- Demos go out of sync.
- No netplay support.
- Won't build with MSVC.
- Some crashes in the sound mixer.

## Building

### Linux

Debian:
```
sudo apt-get install gcc build-essential cmake libsdl2-dev libsdl2-mixer-dev
mkdir cmake-build && cd cmake-build
cmake -DCMAKE_BUILD_TYPE=Release ..
sudo make
```

Arch:
```
sudo pacman -S gcc cmake sdl2 sdl2_mixer
mkdir cmake-build && cd cmake-build
cmake -DCMAKE_BUILD_TYPE=Release ..
sudo make
```

### Windows

MSYS2 UCRT64:
```
pacman -S mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-SDL2  mingw-w64-ucrt-x86_64-SDL2_mixer
mkdir cmake-build && cd cmake-build
cmake -DCMAKE_BUILD_TYPE=Release ..
sudo make
```

### Haiku

```
pkgman install gcc cmake libsdl2_devel sdl2_mixer_devel
mkdir cmake-build && cd cmake-build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## License

GNU GPL v2

Copyright (C) 1994-1995 Apogee Software, Ltd.

Copyright (C) 2002-2015 Steven Fuller, Ryan C. Gordon, John Hall, Dan Olson

Copyright (C) 2024 Fabian Greffrath

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
