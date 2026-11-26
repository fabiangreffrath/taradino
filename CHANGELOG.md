# Changelog

## 2026.11.26

Changes since the 20251222 release.

### Added

- A single executable now handles both the shareware and the registered
  version of the game. The version is detected at runtime: `darkwar.wad` is
  preferred, `huntbgin.wad` is used as a fallback. A shareware session is
  marked in the version string.

### Changed

- The game now renders at 640x400 (aspect ratio corrected) instead of 640x480,
  and the user interface is scaled accordingly, including the status bar,
  messages, LED progress bars and the crosshair. The `resolution` parameter
  accepts `320x200` and `640x400` now.
- Use the DOSBox OPL3 emulator with two chips and the DMXOPL bank for music
  synthesis with libADLMIDI.
- The quit sound now blends into the end screen.
- New application icon in PNG format, replacing the old SVG and XPM icons.
- Removed the `TARADINO_SHAREWARE` and `TARADINO_SUFFIX` CMake options, as
  they are no longer needed. The `version.h` header is no longer generated.
- Removed all private header files and cleaned up the shareware-specific
  code paths.
- Desktop file and manual page are now plain files instead of templates and
  are installed from the source tree.
- The code is now compiled with `-Wundef`.
- Removed cppcheck from the CI.

### Fixed

- Fix a crash when toggling mouselook in the extended options menu while no
  game is running.
- Fix joystick buttons for joysticks whose instance ID differs from their
  device index, e.g. after re-plugging a gamepad. (@WindyCityJess)
- Avoid type-punned reads of lump names and packet CRCs, which violated
  strict aliasing rules. (@WindyCityJess)
- Include `<limits.h>` in `adlmusic.c`.
- Fix some `unused-but-set-variable` compiler warnings.
