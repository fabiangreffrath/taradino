
# Taradino

This is a port of Apogee's 3D action title Rise of the Triad, originally
released in 1994. This port duplicates the functionality of the original
game on modern operating systems, including Linux, Windows, and Mac OS X.

Portability fixes include support for 64-bit and big-endian systems.

This port makes use of the Simple Direct Media Layer, or SDL, and an additional
library named SDL_mixer.

## Known Issues

- Demos go out of sync.
- No netplay support.

## Building

Building Taradino requires GCC, CMake, the development packages for SDL2 and SDL2_Mixer.
After installing, you can run the following commands to build:

Registered:

```sh
cmake -Bcmake-build-registered -DCMAKE_BUILD_TYPE=Release -S.
cmake --build cmake-build-registered
```

Shareware:

```sh
cmake -Bcmake-build-shareware -DCMAKE_BUILD_TYPE=Release -S. -DTARADINO_SHAREWARE=ON -DTARADINO_SUFFIX=shareware
cmake --build cmake-build-shareware
```

## License

GNU GPL v2

Copyright (C) 1994-1995 Apogee Software, Ltd.
Copyright (C) 2002-2015 Steven Fuller, Ryan C. Gordon, John Hall, Dan Olson
Copyright (C) 2006-2025 Fabian Greffrath
Copyright (C) 2023-2025 erysdren

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
