# v1.3 Stable
This is the second stable release of Coresploit which focuses on improving code quality and bug fixes while still adding some small interesting features. :tada:

---
# Features

- Coresploit is now configured through `config.toml` which is much more readable than a header file

# Backend Changes

### Added

- Coresploit now uses the `toml11` library for `.TOML` file support

### Changed

- Heavily cleaned up `CMakeLists.txt` with better organization and sections
- `config.toml` is now used for configuration instead of directly modifying `config.hpp`
- Removed useless `blacklisted_services` variable in `svc-security.cpp`

### Fixed

- Fixed dangling references to tables of `config.toml` by copying instead of using a reference
- Fixed OS string comparison resulting in undefined behavior (replaced `==` with `strcmp`)
- Fixed unused parameter `args` for `service_ustat` by adding `[[maybe_unused]]`
- Fixed unused parameter `args` for `service_example` by adding `[[maybe_unused]]`
- Fixed unused parameter `args` for `service_security` by adding `[[maybe_unused]]`
- Fixed unused parameter `cmd` for `CLEAR` command by adding `[[maybe_unused]]`
- Fixed unused parameter `cmd` for `HELP` command by adding `[[maybe_unused]]`
- Fixed unused parameter `cmd` for `ACTIVE` command by adding `[[maybe_unused]]`
- Fixed unused parameter `data` for `TextEditCallbackStub` by adding `[[maybe_unused]]`
- Fixed console window not being centered in the OS window by setting next window position to `ImVec2(0, 0)`

---

**Full Changelog**: https://github.com/geox25/PXL-private-codebase/blob/master/changelog.md
