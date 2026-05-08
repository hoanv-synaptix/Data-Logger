# Build Guide - SynaptiX Data Logger

## Nội Dung

- [Yêu Cầu Hệ Thống](#yêu-cầu-hệ-thống)
- [Toolchain Setup](#toolchain-setup)
- [Build với Make (Original)](#build-với-make-original)
- [Build với CMake (Alternative)](#build-với-cmake-alternative)
- [Configuration](#configuration)
- [Flashing Firmware](#flashing-firmware)
- [Debugging](#debugging)
- [Troubleshooting](#troubleshooting)

---

## Yêu Cầu Hệ Thống

### Bắt Buộc

- **Compiler:** `arm-none-eabi-gcc` (GNU Arm Embedded Toolchain) 10.x hoặc mới hơn
  - Download: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
- **Make:** GNU Make 4.x
- **CMake:** 3.20+ (chỉ cho CMake build)

### Tùy Chọn

- **STM32CubeF7:** STM32Cube firmware package (cung cấp HAL, CMSIS, startup files)
  - Download: https://www.st.com/en/embedded-software/stm32cubef7.html
- **OpenOCD:** On-chip debugging và flashing
  - https://openocd.org/
- **STM32CubeProgrammer:** Official ST flashing tool
  - https://www.st.com/en/development-tools/stm32cubeprogrammer.html
- **IDE:** STM32CubeIDE, Keil µVision, IAR Embedded Workbench

## Toolchain Setup

### Windows

1. **Install ARM GCC:**
   - Download installer từ ARM website
   - Thêm vào PATH: `C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2021.10\bin`

2. **Install Make:**
   - Từ MSYS2: `pacman -S make`
   - Hoặc từ Chocolatey: `choco install make`

3. **Install CMake:**
   - Download từ cmake.org
   - Hoặc: `choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System'`

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi make cmake
```

### macOS

```bash
brew install arm-none-eabi-gcc make cmake
```

## Build với Make (Original)

Đây là build system gốc của dự án, sử dụng `Synaptix.mk`.

### Build Commands

```bash
cd Data_Logger/SynaptiX

# Clean build
make clean

# Build debug (default)
make

# Build release (optimized)
make CONFIG=Release

# Just compile, no link
make compile

# Show build size
make size
```

### Make Targets

| Target | Mô Tả |
|--------|-------|
| `all` (default) | Build firmware .elf |
| `clean` | Remove all build artifacts |
| `re` | Clean + build |
| `size` | Print firmware size |
| `hex` | Generate .hex file |
| `bin` | Generate .bin file |
| `flash` | Flash to device (nếu OpenOCD configured) |
| `debug` | Build với debug symbols |

### Configuration qua `app_config.h`

`app_config.h` là single source of truth cho build configuration. Ví dụ:

```c
#define ZIGBEE_APP_ENABLE 1        // Enable Zigbee support
#define MODBUS_APP_ENABLE 1        // Enable Modbus
#define NUM_SX485IO_DEV 40         // Max 485 devices
#define NUM_SX420mA_DEV 20         // Max 4-20mA devices
#define MQTT_PUSHLISH_TIME 5000    // Publish interval 5s
```

**Lưu ý:** Thay đổi `app_config.h` yêu cầu rebuild toàn bộ.

## Build với CMake (Alternative)

CMake cung cấp IDE integration tốt hơn và cross-platform build files.

### Setup Toolchain File

Tạo `toolchain-arm.cmake` trong project root:

```cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
```

### Build Commands

```bash
cd Data-Logger

# Create build directory
mkdir -p build
cd build

# Configure
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm.cmake \
      -DMCU_MODEL=STM32F746 \
      -DSTARTUP_FILE=../Data_Logger/SynaptiX/startup_stm32f746xx.s \
      -DLINKER_SCRIPT=../Data_Logger/SynaptiX/STM32F746xx_FLASH.ld ..

# Build
cmake --build . --parallel $(nproc)

# Or use make directly
make -j$(nproc)

# Check size
${CMAKE_SIZE} SynaptiX-DataLogger.elf
```

### IDE Integration

#### VS Code
```json
// .vscode/settings.json
{
    "cmake.configureSettings": {
        "CMAKE_TOOLCHAIN_FILE": "${workspaceFolder}/toolchain-arm.cmake",
        "MCU_MODEL": "STM32F746"
    }
}
```

#### CLion
- File → Settings → Build, Execution, Deployment → CMake
- Toolchain: `CMAKE_TOOLCHAIN_FILE=...`
- Profile: `CMAKE_BUILD_TYPE=Debug`

## Configuration

### Build-Time Configuration

Edit `Data_Logger/SynaptiX/apps/app_config.h`:

```c
// Enable/disable features
#define ZIGBEE_APP_ENABLE 1
#define MODBUS_APP_ENABLE 1
#define BUTTON_APP_ENABLE 1
#define USB_DEVICE_FS_ENABLE 1
#define FATFS_ENABLE 1

// Device counts
#define NUM_SX485IO_DEV 40
#define NUM_SX420mA_DEV 20
#define NUM_SXMETTER_DEV 1
#define NUM_SXTHS_DEV 1

// Network ports
#define SHELL_TCP_PORT 8000
#define MODBUS_TCP_PORT 502
#define HTTP_SERVER_PORT 80

// MQTT Configuration
#define MQTT_HOST "192.168.110.79"
#define MQTT_CLIENT_ID "synaptix-demo01"
#define MQTT_PUSHLISH_TIME 5000  // ms

// Storage
#define STORAGE_SIZE (100 * 1024)  // 100KB on SPI Flash

// MAC Address
#define ETH_MAC_ADDR0 0x00
#define ETH_MAC_ADDR1 0x80
#define ETH_MAC_ADDR2 0xE1
#define ETH_MAC_ADDR3 0x00
#define ETH_MAC_ADDR4 0x01
#define ETH_MAC_ADDR5 0x02
```

### Runtime Configuration

Edit `Data_Logger/SynaptiX/config_json/config.json`:

```json
{
    "devices": [
        {"id": 1, "type": "sx-io", "connection": "zigbee"},
        {"id": 2, "type": "sx-420mA", "connection": "zigbee"},
        {"id": 3, "type": "sx-ths", "connection": "zigbee"}
    ]
}
```

**Lưu ý:** Config này được load bởi `file_io.c` tại runtime.

## Flashing Firmware

### Phương Pháp 1: OpenOCD

```bash
# Connect via ST-Link
openocd -f interface/stlink-v2.cfg -f target/stm32f7x.cfg

# Trong terminal khác:
arm-none-eabi-gdb build/SynaptiX-DataLogger.elf
(gdb) target remote localhost:3333
(gdb) monitor reset halt
(gdb) load
(gdb) monitor reset run
(gdb) detach
(gdb) quit
```

Hoặc dùng `flash` target của make:

```bash
cd Data_Logger/SynaptiX
make flash
```

### Phương Pháp 2: STM32CubeProgrammer

```bash
STM32_Programmer_CLI \
    -c port=SWD \
    -w build/SynaptiX-DataLogger.bin 0x08000000 \
    -v \
    -g
```

### Phương Pháp 3: STM32CubeIDE

1. Import project: File → Import → C/C++ → Existing Code
2. Right-click project → Debug As → STM32 Cortex-M C/C++ Application

## Debugging

### GDB với OpenOCD

```bash
# Start OpenOCD in one terminal
openocd -f interface/stlink-v2.cfg -f target/stm32f7x.cfg

# GDB in another terminal
arm-none-eabi-gdb build/SynaptiX-DataLogger.elf

(gdb) target remote localhost:3333
(gdb) monitor reset halt
(gdb) load
(gdb) break app_main
(gdb) continue
(gdb) info registers
(gdb) bt
(gdb) info threads
```

### Common GDB Commands

| Command | Mô Tả |
|---------|-------|
| `break <file>:<line>` | Set breakpoint |
| `break <function>` | Set function breakpoint |
| `continue` / `c` | Resume execution |
| `step` / `s` | Step into |
| `next` / `n` | Step over |
| `finish` | Step out |
| `print <var>` | Print variable |
| `info locals` | Show local variables |
| `info threads` | List all threads |
| `thread <id>` | Switch thread |

### Debug với IDE

**STM32CubeIDE:**
- Automatic GDB server setup
- Graphical register view
- Peripheral registers view
- Real-time expressions

**Keil µVision:**
- uVision debugger
- System Viewer
- Event Recorder

**IAR EWARM:**
- C-SPY debugger
- Power debugger

## Troubleshooting

### Build Errors

#### "Startup file not found"
```cmake
# Trong build command, chỉ định STARTUP_FILE:
-DSTARTUP_FILE=../path/to/startup_stm32f746xx.s
```

Startup file thường nằm trong:
- STM32CubeF7/Projects/STM32746G-Discovery/...
- STM32CubeF7/Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/

#### "Linker script not found"
Tương tự, chỉ định `-DLINKER_SCRIPT=path/to/STM32F746xx_FLASH.ld`.

Linker script trong:
- STM32CubeF7/Projects/STM32746G-Discovery/...
- STM32CubeF7/Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/gcc/

#### "undefined reference to `HAL_*`"
Cần link HAL library:
```cmake
target_link_libraries(${PROJECT_NAME}
    stm32f7xx_hal
    stm32f7xx_hal_cortex
    stm32f7xx_hal_rcc
    # ... other HAL modules
)
```

Hoặc trong Makefile, cần set `USE_HAL=1` và include HAL paths.

### Flash Failures

#### "Error: device not found"
- Kiểm tra kết nối ST-Link/J-Link
- Driver installed?
- Target powered?
- Reset button pressed?

#### "Verify failed"
- Firmware quá lớn cho flash? Check map file
- Wrong linker script/flash address?
- Flash protected? Unlock với STM32CubeProgrammer

### Runtime Issues

#### HardFault
Kiểm tra:
1. Stack overflow? Tăng stack size trong FreeRTOS config
2. Null pointer? Enable `MONITOR_HEAP_RTOS` và check heap
3. Watchdog timeout? Disable temporarily để debug

#### Ethernet không hoạt động
- Kiểm tra PHY reset pin
- Kiểm tra clock config (HAL_RCC_ETH_CLK_ENABLE)
- Kiểm tra MAC address config

#### MQTT không connect
- Kiểm tra network connectivity (ping broker)
- Kiểm tra MQTT_HOST config
- Kiểm tra credentials

## Optimization Tips

### Code Size

1. **Remove unused features** trong `app_config.h`:
   ```c
   #define MQTT_APP_ENABLE 0  // Nếu không dùng MQTT
   ```

2. **Link-time optimization (LTO):**
   ```make
   CFLAGS += -flto
   ```

3. **Remove debug symbols sau khi done:**
   ```bash
   arm-none-eabi-strip --strip-debug build/firmware.elf
   ```

### Performance

1. **Optimization level:** `-O2` hoặc `-Os` (size) trong release
2. **Cache:** Enable instruction/data cache trong SystemInit()
3. **RTOS tick:** Tăng `configTICK_RATE_HZ` nếu cần resolution tốt hơn

## Continuous Integration

Ví dụ `.github/workflows/build.yml`:

```yaml
name: Build Firmware

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install ARM GCC
        run: sudo apt-get install gcc-arm-none-eabi make cmake
      - name: Build with Make
        run: |
          cd Data_Logger/SynaptiX
          make clean
          make
      - name: Upload artifact
        uses: actions/upload-artifact@v3
        with:
          name: firmware
          path: Data_Logger/SynaptiX/*.elf
```

---

**Cần trợ giúp?** Xem [ARCHITECTURE.md](ARCHITECTURE.md) cho kiến trúc chi tiết hoặc [CONTRIBUTING.md](CONTRIBUTING.md) cho guidelines.
