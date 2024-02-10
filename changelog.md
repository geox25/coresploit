- Added `security.svc` from `service_security` in `svc-security.cpp`
- Added difference between normal routines and system routines (system routines like `security.svc` cannot be stopped for obvious security reasons)
- Added separate `system/security` directories in `svc/` for services that are system related (such as `security.svc`)
- Added separate `system_services` variable for system services
- Added `log_system` log stack for system services
- Added `monitor_system_futures` called by `monitor_futures` (does not have its own thread to save resources)
- Added text wrap of logs by adding a `\n` every 150 characters

---
- Moved `futures` from `boot.cpp` to `svc.cpp`
- Renamed log stack variables from `util_log_stack` to more concise `log_util`
- Changed comments that were too long
- Minor refactoring of guards in `run` command code

---
- Fixed inaccurate window title of program version
- Fixed inaccurate log message in `monitor_futures`
