
# Taradino

This is a port of Apogee's 3D action title Rise of the Triad, originally
released in 1994. This port duplicates the functionality of the original
game on modern operating systems, including Linux, Windows, and Mac OS X.

Portability fixes include support for 64-bit and big-endian systems.

This port makes use of the Simple Direct Media Layer, or SDL, and an additional
library named SDL_mixer.
Optional OPL3/AdLib music synthesis is supported by using libADLMIDI.

## Known Issues

- Demos go out of sync.
- No netplay support.

## Building

Building Taradino requires GCC, CMake, the development packages for SDL2 and SDL2_Mixer (and libADLMIDI, if you like).
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

## Legalese

Files: `*`  
Copyright:  
 © 1993-1994, James R. Dose  
 © 1993-1996, Id Software, Inc.  
 © 1993-2008, Raven Software  
 © 1994-1995, Apogee Software, Ltd.  
 © 2002-2015, Steven Fuller, Ryan C. Gordon, John Hall, Dan Olson  
 © 2005-2018, Simon Howard  
 © 2006-2025, Fabian Greffrath  
 © 2023-2025, erysdren  
License: [GPL-2.0+](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)

Files: `rott/vgatext.*`  
Copyright:  
 © 2024, erysdren  
License: [MIT](https://opensource.org/licenses/MIT)

Files: `rott/vgafont.h`  
Copyright:  
 © Joseph Gil  
License: public-domain
