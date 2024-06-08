# ESP32 Bicycle Speedometer

This project is an ESP32-based bicycle speedometer that uses a Hall effect sensor and multiple magnets to calculate the speed of tire rotation. The calculated speed is displayed on an ST7565-based LCD screen. The project is implemented using ESP-IDF, and no external libraries were used for the LCD interface, which was written from scratch.

## Table of Contents

- [Overview](#overview)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Setup](#setup)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Overview

This project measures the speed of a bicycle by detecting the rotation of its tires. A Hall effect sensor detects the passing of magnets mounted on the bicycle wheel, and the ESP32 microcontroller calculates the speed based on the time between detections. The speed is then displayed on an ST7565 LCD screen.

## Hardware Requirements

- ESP32 development board
- Hall effect sensor (41F)
- Magnets (to be mounted on the bicycle wheel)
- ST7565 LCD screen (GMG12864-06D)
- MCP1700-3302 voltage regulator
- Push button to switch between km/h and m/s

## Software Requirements

- ESP-IDF (Espressif IoT Development Framework)

## Setup

### 1. Install ESP-IDF

Follow the instructions to set up the ESP-IDF environment:
- [ESP-IDF Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)

### 2. Clone the Repository

```bash
git clone https://github.com/thierryE9/st7565test
```

### 3. Build and Flash

Build the project and flash it to your ESP32:

```bash
idf.py build
idf.py flash
```

### 4. Monitor the Output

After flashing, you can monitor the output using:

```bash
idf.py monitor
```

## Usage

1. Mount the magnets on the bicycle wheel at equal distances.
2. Position the Hall effect sensor so that it detects the passing magnets as the wheel rotates.
3. Power on the ESP32.
4. The LCD screen will display the current speed based on the tire rotations.
5. Press the button to switch between km/h and m/s.