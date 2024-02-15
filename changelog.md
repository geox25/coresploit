# v1.2 Stable

---
# Features

- Added theme selector in options
- Added `modern`, `retro`, and `gray` themes
- Added option to blacklist services in `config.hpp` via adding `#BLACKLIST <service>` to `SECURITY_FLAGS`
- `active` command now shows all running services and whether they are normal or system

# Backend Changes

### Added
- Added `security.svc` from `service_security` in `svc-security.cpp`
- Added difference between normal routines and system routines (system routines like `security.svc` cannot be stopped for obvious security reasons)
- Added separate `system/security` directories in `svc/` for services that are system related (such as `security.svc`)
- Added separate `system_services` variable for system services
- Added `log_system` log stack for system services
- Added `monitor_system_futures` called by `monitor_futures` (does not have its own thread to save resources)
- Added text wrap of logs by adding a `\n` every 150 characters
- Added some compiler flags including `set(CMAKE_CXX_FLAGS_RELEASE "-O3")` for optimization
- Added `DEFAULT_THEME` to `config.hpp`
### Changed

- Moved `futures` from `boot.cpp` to `svc.cpp`
- `std::format` is now being used rather than messy concatenation
- Renamed log stack variables from `util_log_stack` to more concise `log_util`
- Changed comments that were too long
- Minor refactoring of guards in `run` command code
- Removed unused `INJECT` button which is no longer necessary
- Removed unused font variable
- Removed unused window size function call
- Removed unused code in `TextEditCallbackStub`
- Cleaned up comments
### Fixed

- Fixed race condition causing segfault when a routine is erased from futures and monitor_futures tries to erase it and update iterator (one iteration is skipped)
- Fixed inaccurate window title of program version
- Fixed inaccurate log message in `monitor_futures`
- Fixed bug where host system was displayed as win32 instead of win64
- Fixed `monitor_futures` not marked as `[[noreturn]]`