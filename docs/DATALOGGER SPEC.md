\# Datalogger Architecture Specification



\---



\## 1. Overview



Datalogger là thiết bị thu thập và giám sát dữ liệu môi trường không khí, được thiết kế cho các ứng dụng quan trắc công nghiệp và đô thị thông minh.



Thiết bị sử dụng vi xử lý Arm® Cortex®-M7 hiệu năng cao, đảm bảo khả năng xử lý nhanh, ổn định và đáp ứng tốt các tác vụ thời gian thực.



\---



\## 2. Hardware Architecture



\### 2.1 Processing

\- Arm® Cortex®-M7 STM32F750N8H6 DK

\### 2.2 Network Interfaces

\- 1 x Ethernet 10/100 Mbps

\- 1 x LTE

\- 1 x WiFi (Optional)

\- 1 x Zigbee hoặc LoRa (Optional)



\### 2.3 Peripheral Interfaces

\- 1 x USB

\- 1 x SD Card

\- 2 x RS485

\- 1 x RS232



\### 2.4 IO System

\- 8 x Digital Input

\- 8 x Digital Output

\- 8 x Analog Input



\### 2.5 Display \& Storage

\- LCD cảm ứng 5 inch

\- RAM: up to 128 Mb

\- Flash: up to 256 Mb



\### 2.6 Power

\- 12\~24 VDC



\---



\## 3. Software Architecture



\### 3.1 OS Layer

\- FreeRTOS



\### 3.2 Middleware Layer

\- LWIP

\- MbedTLS

\- TinyUSB

\- nanomodbus

\- FATFS

\- TouchGFX

\- LittleFS



\### 3.3 Application Layer

\- MQTT / MQTTS

\- HTTP / HTTPS Client

\- HTTP Server

\- SNTP

\- TCP / UDP

\- Modbus TCP/IP

\- LoRaWAN Gateway



\### 3.4 Service Layer

\- Modem WiFi

\- Modem BLE

\- Modem LTE

\- Modem LoRa

\- USB CDC / MSC / ECM

\- Storage

\- Logger

\- RS485

\- Command Line

\- AT Parser

\- Netif



\---



\## 4. System Functions



\### 4.1 Connectivity

\- MQTT / MQTTS

\- HTTP / HTTPS

\- FTP / FTPS



\### 4.2 Data Processing

\- Caculator AQM

\- Save data



\### 4.3 UI

\- Control and Display with LCD



\### 4.4 Firmware

\- FOTA (Secure Boot, Rollback Firmware, Update firmware over MQTT/HTTP)



\### 4.5 Communication

\- Connect Zigbee sensor (or LoRa sensor)

\- Modbus RTU over serial master

\- Modbus RTU over TCP server



\---



\## 5. Network Architecture



\### 5.1 Ethernet



\#### Stack Flow

App

↓

LWIP

↓

Ethernet Interface

↓

MAC

↓

RMII Bus

↓

Ethernet Module



\---



\### 5.2 LTE (USB Modem)



\#### Ports

\- Port 0: PPPOS

\- Port 1: AT Commands



\#### Data Flow

App

↓

LWIP

↓

PPPOS

↓

AT Port 0

↓

USB CDC Host

↓

LTE Module



\#### Control Flow

App / AT Process

↓

AT Port 1

↓

USB CDC Host

↓

LTE Module



\---



\### 5.3 WiFi (AT-based Module)



\#### Interface

\- SPI

\- UART

\- Device RDY

\- CHIP EN



\#### Flow

App

↓

W6x APIs

↓

W6x Core

↓

AT Driver

↓

SPI Interface

↓

WiFi Module (ST67W611M1)



\---



\## 6. Network Priority \& Failover



\### 6.1 Priority

1\. Ethernet (High)

2\. WiFi (Normal)

3\. LTE (Low)



\### 6.2 Failover Logic



\[Ethernet Connected]

→ Lost → Switch to WiFi



\[WiFi Connected]

→ Lost → Switch to LTE



\[LTE Connected]

→ Lost → Retry Ethernet



\---



\## 7. Netif Architecture



\### 7.1 Interfaces

\- netif\_eth

\- netif\_wifi

\- netif\_lte



\### 7.2 Responsibilities

\- Link up/down

\- IP assignment

\- Packet routing

\- Failover switching



\### 7.3 APIs

\- netif\_init()

\- netif\_set\_active(type)

\- netif\_get\_active()

\- netif\_is\_connected(type)



\### 7.4 LWIP Integration

\- netif\_add()

\- netif\_set\_default()



\---



\## 8. Thread Architecture



\### 8.1 Core Threads

\- App Thread

\- LWIP Thread

\- Netif Thread



\### 8.2 Ethernet

\- Ethernet IF Thread



\### 8.3 WiFi

\- SPI Sync Thread

\- AT Process Thread



\### 8.4 LTE

\- PPPOS Thread

\- AT Process Thread



\### 8.5 Rules

\- LWIP chạy trong một thread duy nhất

\- Tất cả network input đi qua LWIP thread

\- Application không truy cập trực tiếp driver



\---



\## 9. Data Flow



Sensor

↓

MCU

↓

Application

↓

Protocol Layer (MQTT/HTTP)

↓

LWIP

↓

Netif

↓

Driver

↓

Physical Interface

↓

Network



\---

