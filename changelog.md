# v1.3 Stable
This is the second stable release of Coresploit which focuses on improving code quality and bug fixes while still adding some small interesting features. :tada:

---
# Features



# Backend Changes

### Added

- Coresploit now uses the `toml11` library for `.TOML` file support

### Changed

- Heavily cleaned up `CMakeLists.txt` with better organization and sections
- `config.toml` is now used for configuration instead of directly modifying `config.hpp`
- Removed useless `blacklisted_services` variable in `svc-security.cpp`

### Fixed

- Console window not being centered in the OS window

---

**Full Changelog**: https://github.com/geox25/PXL-private-codebase/blob/master/changelog.md
