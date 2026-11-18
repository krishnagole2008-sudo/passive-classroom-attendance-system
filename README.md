# Passive Classroom Attendance Monitoring System

An ESP32 and Bluetooth Low Energy (BLE) based system designed to detect registered BLE-enabled devices and provide visual and audio alerts for classroom presence monitoring.

## Project Overview

Traditional classroom attendance often requires manual intervention and consumes valuable classroom time.

This project explores a passive attendance monitoring approach using an ESP32 microcontroller and Bluetooth Low Energy technology.

The system scans for registered BLE devices, checks their advertised service data and received signal strength (RSSI), and displays detection information on an OLED screen.

## Key Features

- BLE-based registered device detection
- Service-data identification using `ATTEND001`
- RSSI-based signal filtering
- Real-time OLED status display
- LED indication when a registered device is detected
- Buzzer alert with cooldown control
- Automatic presence timeout
- Serial Monitor output for debugging

## System Working Principle

The system follows this sequence:

1. **BLE Scanning:** ESP32 continuously scans for nearby BLE advertisements.
2. **Device Identification:** The system checks whether the received service data matches `ATTEND001`.
3. **RSSI Filtering:** The detected signal is checked against the configured threshold of -80 dBm.
4. **Presence Validation:** The last successful detection is used to maintain a temporary presence status.
5. **Alert Generation:** The LED and buzzer provide alerts when a qualifying device is detected.
6. **OLED Display:** The display shows device detection, identifier, RSSI, and status information.

## Hardware Requirements

| Component | Purpose |
|---|---|
| ESP32 Development Board | Main controller and BLE scanning |
| 0.96-inch OLED Display (SSD1306) | Displaying system status |
| LED | Visual detection indication |
| Active Buzzer | Audio alert |
| Resistor | LED current limiting |
| BC547 transistor | Buzzer driver |
| Breadboard and jumper wires | Circuit connections |

## Pin Configuration

| Component | ESP32 Pin |
|---|---|
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| LED | GPIO 23 |
| Buzzer control | GPIO 18 |

**OLED Configuration:** 128 × 64 pixels, I²C address `0x3C`.

## Software Requirements

- Visual Studio Code
- PlatformIO IDE extension
- ESP32 Arduino framework
- USB cable for programming

### Libraries Used

- `Adafruit SSD1306`
- `Adafruit GFX Library`
- ESP32 BLE libraries
- Arduino framework libraries

## Project Configuration

The project uses PlatformIO with the following configuration:

- Platform: Espressif 32
- Board: ESP32 Dev Module
- Framework: Arduino
- Serial Monitor Baud Rate: 115200

## Installation and Setup

1. Install Visual Studio Code.
2. Install the PlatformIO IDE extension.
3. Clone or download this repository.
4. Open the project folder in VS Code.
5. Connect your ESP32 development board.
6. Check the upload port in `platformio.ini` and change it if necessary.
7. Build and upload the firmware using PlatformIO.
8. Open the Serial Monitor at 115200 baud.

### Git Clone

```bash
git clone https://github.com/krishnagole2008-sudo/passive-classroom-attendance-system.git
```

## Firmware Parameters

| Parameter | Configured Value |
|---|---|
| Registered BLE ID | ATTEND001 |
| RSSI Threshold | -80 dBm |
| Presence Timeout | 5000 ms |
| LED Duration | 1000 ms |
| Buzzer Duration | 150 ms |
| Alert Cooldown | 3000 ms |
| BLE Scan Duration | 1 second |

These values can be modified in `src/main.cpp`.

## Project Structure

```text
ESP32_LED_TEST/
│
├── .vscode/
├── include/
├── lib/
├── src/
│   └── main.cpp
├── test/
├── .gitignore
├── platformio.ini
└── README.md
```

## Current Limitations

- The current firmware identifies devices using a predefined BLE service-data value.
- RSSI is an approximate signal-strength measurement and cannot provide exact distance.
- The system does not currently maintain a permanent attendance database.
- Detection depends on the registered device advertising the expected BLE service data.
- The current implementation is a prototype and requires further testing in different classroom environments.

## Future Scope

- Multiple registered device identification
- Database integration for attendance records
- Web-based attendance dashboard
- Cloud data synchronization
- Improved detection reliability
- Attendance reports and analytics
- Multi-classroom deployment

## Project Demonstration

Project prototype photographs and demonstration video will be added here.

## License

This project is intended for academic and educational purposes.
