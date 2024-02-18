# Coresploit Private Repository

[![Stable - v1.2](https://img.shields.io/badge/Stable-v1.2-2ea44f?style=for-the-badge)](https://)
[![Nightly - v1.1](https://img.shields.io/badge/Nightly-v1.1-852ea4?style=for-the-badge)](https://)
[![License - Private/Proprietary](https://img.shields.io/badge/License-Private%2FProprietary-blue?style=for-the-badge)](https://)
[![view - Documentation](https://img.shields.io/badge/view-Documentation-blue?style=for-the-badge)](/docs/ "Go to project documentation")

Coresploit (previously Personal Exploitation Library) is a software-suite that contains different tools and programs for both exploitation
and specialized tasks that would become too messy if they were packaged separately. The goal for Coresploit is to have a secure, high performance,
and highly customizable environment with numerous integrated systems. Coresploit is intended to be ran on Windows, Mac, and Linux. However, it
has only been tested on Windows and Linux right now.

## Release Schedule
Precompiled binaries are available for Windows and Linux and are usually shipped with each stable release. During the beginning of a given month,
some nightly releases will be made containing experimental features and bug fixes. Towards the middle to end of a given month, there will be at least
one stable release that is much more polished and contains more features.

## Installation
Coresploit can be easily compiled from source (especially on CLion) however precompiled binaries are also available for each stable release. If you compile from
source, make sure OpenGL is installed and available on your system. 

## Roadmap
**[Go to Project Philosophy](philosophy.txt)**

Immediate Todo:
- (CLion) Right Click On File -> Analyze feature to improve code
- Shell file to run `tail -f` of a `master.log` file that uses `spdlog` or `loguru`
- Work on project philosophy & style guidelines
- Self-hosted gitea private server
- Setup Library Infrastructure
- LevelDB for library infrastructure and host os service threat detection
- Add ``win64-basic_keylog`` to library (Visual studio)
- Add ``secretsdump`` to library (Python Script)

Later Todo:
- Included Password Cracking Utils
- Injection Functionality (init.svc, inject.svc, etc)
- Window docking functionality with other windows (file browser, performance overview, system info)
- Encryption/Source Guarding -> Executable Binary

Eventually Todo:
- Encrypted notepad/files with cloud storage support
- Cross-platform keyloggers
- Cross-platform proccess monitoring
- Cross-platform spyware/authware detection
- Windows: Password detection/cracker
- Windows: Security Verification & Management
- Drivers to support specific services on more platforms

</div>

## Documentation

<div align="left">

[![view - Documentation](https://img.shields.io/badge/view-Documentation-blue?style=for-the-badge)](/docs/ "Go to project documentation")

</div>

## License

Released under [Private/Proprietary](/LICENSE) by [@geox25](https://github.com/geox25).
