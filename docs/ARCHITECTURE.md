# Kiến Trúc Hệ Thống - SynaptiX Data Logger

## Tổng Quan

SynaptiX Data Logger được thiết kế theo kiến trúc module-based, với sự tách biệt rõ ràng giữa các layer:

```
┌─────────────────────────────────────────────────────────────────┐
│                    Application Layer                           │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌──────────┐           │
│  │   MQTT  │ │  HTTP   │ │ Modbus │ │  Shell   │           │
│  │  Client │ │ Server  │ │ Slave   │ │  CLI     │           │
│  └────┬────┘ └────┬────┘ └────┬────┘ └────┬─────┘           │
│       │           │           │           │                  │
├───────┴───────────┴───────────┴───────────┴──────────────────┤
│                  Device Drivers Layer                         │
│  ┌──────┐ ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐ ┌──────┐          │
│  │420mA │ │THS  │ │IO   │ │Meter│ │RF   │ │Button│          │
│  └──────┘ └─────┘ └─────┘ └─────┘ └─────┘ └──────┘          │
├──────────────────────────────────────────────────────────────┤
│                  Communication Protocols                     │
│         Zigbee    Modbus    TCP/UDP    SNTP    USB           │
├──────────────────────────────────────────────────────────────┤
│                  Services Layer                              │
│  ┌──────────┐ ┌──────────┐ ┌─────────┐ ┌──────────┐       │
│  │   FATFS  │ │ LittleFS │ │  Logger │ │   Flash  │       │
│  └──────────┘ └──────────┘ └─────────┘ └──────────┘       │
│  ┌──────────┐ ┌──────────┐ ┌─────────┐ ┌──────────┐       │
│  │   cJSON  │ │  cQueue  │ │  Modbus │ │  LinkList│       │
│  └──────────┘ └──────────┘ └─────────┘ └──────────┘       │
├──────────────────────────────────────────────────────────────┤
│                  Board Abstraction Layer                     │
│           Flash Drivers    UART    SPI    I2C    GPIO         │
├──────────────────────────────────────────────────────────────┤
│                  HAL / CMSIS / RTOS (FreeRTOS)              │
└──────────────────────────────────────────────────────────────┘
```

## Các Module Chính

### 1. Application Layer (`apps/`)

Module cấp cao xử lý business logic và tích hợp các service.

#### `app.c` - Main Application
- **Vai trò:** Entry point, vòng lặp chính, task scheduler
- **Initialization:** Khởi tạo tất cả subsystems
- **Main Loop:** Polling các sensors, xử lý queue messages

#### `app_config.h` - Configuration Hub
- Feature flags (enable/disable modules)
- Device limits (max số lượng cảm biến)
- Network ports
- MQTT settings
- **Lưu ý:** Build-time configuration, cần rebuild để thay đổi

#### Protocol Modules

| Module | File | Mô Tả |
|--------|------|-------|
| MQTT | `mqtt/mqtt_app.c` | Client kết nối ThingsBoard, publish telemetry mỗi 5s |
| HTTP Server | `http_server/http_server.c` | Web interface trên port 80 |
| Modbus Slave | `modbus_slave/` | RTU qua USB, TCP qua Ethernet |
| SNTP | `sntp/sntp_app.c` | Time sync từ NTP server |
| OTA | `ota/ota.c` | Firmware update qua HTTP/MQTT |

#### Management Modules

| Module | File | Mô Tả |
|--------|------|-------|
| Shell | `shell/` | CLI qua serial và TCP (port 8000) |
| Settings | `settings/app_settings.c` | Persist cấu hình vào flash |
| Button | `button/button.c` | Button interrupt handling |

#### Device Drivers

| Driver | File | Giao Thức | Số Lượng |
|--------|------|-----------|----------|
| SX-420mA | `sx_420mA/` | Zigbee | 20 |
| SX-485-IO | `sx_485_io/` | Zigbee | 40 |
| SX-Metter | `sx_metter/` | Zigbee | 1 |
| SX-THS | `sx_ths/` | Zigbee | 1 |
| RF App | `rf_app/` | Zigbee coordinator |

### 2. Services Layer (`services/`)

Các thư viện utility độc lập, có thể reuse.

#### Filesystem
- **FATFS:** `fatfs/` - Full FAT implementation cho SPI Flash external
- **LittleFS:** `littlefs/` - Alternative wear-leveling filesystem

#### Data Structures
- **cqueue:** Circular buffer implementation
- **linklist:** Doubly-linked list

#### Protocols
- **Modbus:** Full stack (master/slave, RTU/TCP) trong `modbus/`
- **Zigbee:** `zigbee/` - Coordinator/driver
- **Socket Client/Server:** `socket_client/`, `socket_server/`
- **TCP/UDP:** `tcp_*`, `udp_*` - Low-level sockets

#### Serialization
- **cJSON:** JSON parsing và generation

#### Logging
- **logger:** `logger.c` - Logging system với different levels

#### Hardware Abstraction
- **mmb_serial:** Multi-port serial driver
- **flash:** `flash.c` - SPI/NOR flash operations

### 3. Board Layer (`board/`)

Hardware-specific implementations.

- `board.c/.h` - Board initialization (GPIO, clocks, pins)
- `spif/spif.c` - SPI Flash driver (từ SPI peripheral)
- `qspif/w25q_mem.c` - QSPI Flash driver (cho STM32F7 QUADSPI)

### 4. Configuration

#### Build-time (`app_config.h`)
```c
#define ZIGBEE_APP_ENABLE 1
#define MODBUS_APP_ENABLE 1
#define NUM_SX485IO_DEV 40
#define MQTT_PUSHLISH_TIME 5000
#define SHELL_TCP_PORT 8000
// ...
```

#### Runtime (`config_json/config.json`)
```json
{
    "devices": [
        {"id": 1, "type": "sx-io", "connection": "zigbee"},
        {"id": 2, "type": "sx-420mA", "connection": "zigbee"}
    ]
}
```

## Luồng Dữ Liệu

### Thu Thập Dữ Liệu Cảm Biến

```
Sensor (4-20mA/RS485/Zigbee)
    ↓
Device Driver (sx_420mA.c, sx_485_io.c)
    ↓
Queue/Buffer (cqueue)
    ↓
Application Logic (app.c - data processing)
    ↓
Logger (logger.c - write to file)
    ↓
FATFS/LittleFS → SPI Flash
    ↓
[Optional] MQTT/HTTP publish
```

### Command & Control

```
Remote Client
    ↓
HTTP/Modbus/Shell
    ↓
Command Handler
    ↓
Device Driver / Settings
```

## Thiết Kế & Patterns

### 1. Module Independence
Mỗi module trong `apps/` và `services/` được thiết kế để độc lập, với rõ ràng:
- Input: Hàm public API trong `.h` file
- State: Static variables trong `.c` file
- Dependencies: Explicit `#include`

### 2. Configuration-Driven
Hầu hết behavior được điều khiển bởi macros trong `app_config.h`:
- Enable/disable features để giảm code size
- Device counts để allocate arrays đúng kích thước

### 3. Event-Driven (với RTOS)
Nếu FreeRTOS được enable:
- Mỗi protocol (MQTT, HTTP, Modbus) chạy trong separate task
- Queue-based communication giữa tasks
- Mutex/semaphore cho shared resources

### 4. Fault Tolerance
- Return codes trong tất cả API functions
- Error logging qua `synaptix_error.h`
- Watchdog có thể được implement

## Memory Layout

### Flash (Typical STM32F746)
```
0x08000000 - Startup + Vector Table
0x08004000 - Text/Code segment
0x080XXXXX - Read-only data (configs, strings)
0x080YYYYY - Loadable segments (initialized data)
```

### RAM
```
0x20000000 - Stack (grows xuống)
0x2000XXXX - Heap (nếu malloc dùng)
0x2000YYYY - BSS (zero-initialized)
0x2000ZZZZ - Data (initialized)
0x2000AAAA - DMA buffers
```

**Storage trên External Flash:** 100KB reserved cho data logging qua FATFS.

## Performance Considerations

| Component | Priority | Notes |
|-----------|----------|-------|
| MQTT Publish | Medium | 5s interval, non-blocking |
| HTTP Server | Low | Background task, on-demand |
| Sensor Polling | High | Real-time, deterministic timing |
| File I/O | Low | Async với buffer |
| Shell Commands | Low | Interactive, blocking OK |

## Extensibility

Để thêm sensor/device mới:

1. Tạo module trong `apps/` hoặc `services/`
2. Add files vào `Synaptix.mk` (APPFILES hoặc SERVICESFILES)
3. Định nghĩa device type constant
4. Implement API: `init()`, `read()`, `write()`, `config()`
5. Register với device manager (trong `app.c`)

Để thêm protocol mới:

1. Tạo service trong `services/`
2. Implement socket/transport layer
3. Add task (nếu cần) trong `app.c`
4. Cấu hình ports trong `app_config.h`

## Security Considerations

- **Authentication:** MQTT username/password trong config (xem `app_config.h`)
- **Network Access:** Shell port (8000) nên đặt trong internal network
- **Firmware:** OTA cần verify signature (chưa implement)
- **Input Validation:** Validate tất cả JSON config và shell commands

## Testing Strategy

Xem thêm trong `docs/CONTRIBUTING.md`.

### Unit Tests
- Mỗi service có thể test isolation với mock HAL

### Integration Tests
- Test end-to-end: sensor → file → MQTT
- Sử dụng test harness với simulated sensors

### Hardware-in-the-Loop
- Run tests trên real hardware với test jig

## References

- STM32F7 Reference Manual (RM0385)
- STM32F7xx HAL Driver Documentation
- FreeRTOS API Reference
- LwIP Developer's Guide
- Modbus Application Protocol Specification

---

*Bản tài liệu này cung cấp overview. Xem các file khác trong `docs/` để biết thêm chi tiết.*
