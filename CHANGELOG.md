# Changelog

All notable changes to sget are documented here.

## [2.0.1] - 2026-09-06

### Changed

- Removed a few unoptimized lines for cleaner, faster code.

## [2.0.0] - 2026-09-05

### Added

- GitHub Actions packaging workflows that build, verify, and publish installable `.deb` and `.rpm` packages.
- Parallel download engine with a terminal-aware animated progress UI.
- Resume (`-C`), insecure TLS (`-k`), custom User-Agent (`-A`), output directory/name (`-d` / `-o`), parallelism (`-P`).
- CPack-driven DEB/RPM generation with auto-detected runtime dependencies.

### Changed

- Modularized the codebase: each struct now lives in its own include-guarded header.
- Unified versioning (`2.0.0`) across the binary, packages, and vcpkg manifest.