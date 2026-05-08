# Changelog

All notable changes to the SynaptiX Data Logger project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Initial repository structure with Make + CMake build systems
- Comprehensive documentation (README, ARCHITECTURE, BUILD, CONTRIBUTING)
- Gitignore for STM32 embedded projects
- CMakeLists.txt with toolchain support for STM32F7
- Doxygen configuration for API documentation

### Changed
- N/A

### Deprecated
- N/A

### Removed
- N/A

### Fixed
- N/A

### Security
- N/A

---

## [1.0.0] - 2026-05-07

### Added
- Initial release of SynaptiX Data Logger firmware
- STM32F7 platform support (Cortex-M7)
- Ethernet connectivity with LwIP stack
- Multiple communication protocols:
  - HTTP server (port 80)
  - Modbus TCP (port 502) and Modbus RTU (USB)
  - MQTT client with ThingsBoard integration
  - SNTP time synchronization
- Zigbee coordinator support
- Device drivers:
  - SX-IO (40 devices max via Zigbee)
  - SX-420mA (20 devices max, 4-20mA analog)
  - SX-Meter (1 device)
  - SX-THS (1 device, temperature/humidity)
- FATFS filesystem on external SPI Flash (100KB reserved)
- Remote shell via TCP (port 8000) and serial
- JSON-based runtime configuration
- OTA firmware update support
- Settings persistence in flash
- Button interrupt handling
- LED indicators
- Modbus memory map:
  - Discrete Inputs: 80 coils (0-79)
  - Coils: 80 coils (0-79)
  - Input Registers: 100 registers (0-99)
  - Holding Registers: 100 registers (0-99)
- Default MAC address: 00:80:E1:00:01:02
- MQTT configuration:
  - Host: 192.168.110.79
  - Client ID: synaptix-demo01
  - Publish interval: 5000ms

### Changed
- N/A

### Deprecated
- N/A

### Removed
- N/A

### Fixed
- N/A

### Security
- N/A

---

## Versioning Legend

- **Major** (`X.0.0`): Incompatible API changes, major architecture shifts
- **Minor** (`1.X.0`): New features in a backwards-compatible manner
- **Patch** (`1.0.X`): Backwards-compatible bug fixes

---

## How to Add to This Changelog

When making changes, add them to the `[Unreleased]` section:

```markdown
### Added
- New feature description

### Changed
- Modified behavior description

### Fixed
- Bug fix description with issue number (if applicable)
```

Then when releasing:

1. Update version in `[Unreleased]` header
2. Create new `[Unreleased]` section at top
3. Commit with message: `chore: release v1.1.0`

---

**Note:** This is a template. Actual entries will be filled as the project evolves.
