# 1.4: The Polishing Update
This update focuses on making sure everything is clean and prepared for the next update which will add some major features. :tada:

---
# Features

- New `uptime` command to show how long coresploit has been online *(format `D:H:M:S`)*
- Updated `help` command to finally be accurate
- Verbose and theme options are now loaded from the config when coresploit is started and saved to the config when coresploit is stopped

# Backend Changes

### Added

- `VERBOSE` from `config.hpp` is now used internally instead of the old verbose variable

### Changed

- Old verbose variable in `boot.cpp` has been removed in favor of config changes

### Fixed

- Fixed linker issues with the executable trying to find standard libraries (static linked now)
- Fixed executable not being able to find `rsc/fonts/`
- Fixed warning on win64 from `exec` being defined but not used in `prg-ustat.cpp`

---

**Full Changelog**: https://github.com/geox25/PXL-private-codebase/blob/master/changelog.md
