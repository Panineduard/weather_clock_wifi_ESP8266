# HTU21D Temperature & Humidity Sensor - Setup Guide

## Overview

The HTU21D is a low-cost, high-precision digital temperature and humidity sensor. This guide explains how to add it to your Weather Clock project to monitor **indoor home temperature and humidity**.

## 📋 What You'll Get

With HTU21D sensor added:

*   ✅ **Accurate indoor/home temperature** (±0.3°C accuracy)
*   ✅ **Indoor humidity readings** (±2% RH accuracy)
*   ✅ **Calculated dew point**
*   ✅ **Comfort level** indicator for your home
*   ✅ **New display mode** showing indoor climate data

## 🛒 Hardware Requirements

### HTU21D Sensor Module

*   HTU21D breakout board (GY-21 or similar)
*   Operating voltage: 3.3V
*   I2C interface (default address: 0x40)

## 🔌 Wiring Connections

### Important: I2C Shared Bus

The HTU21D uses I2C communication, which allows **multiple devices on the same bus**. This means the HTU21D shares the same SDA and SCL pins with your OLED display.

### Connection Diagram

```
ESP8266 (NodeMCU)          SSD1306 OLED         HTU21D Sensor
┌─────────────┐           ┌──────────┐         ┌──────────┐
│             │           │          │         │          │
│    3.3V   ──┼───────────┤  VCC     ├─────────┤  VCC     │
│     GND   ──┼───────────┤  GND     ├─────────┤  GND     │
│  GPIO0/D3 ──┼───────────┤  SDA     ├─────────┤  SDA     │
│  GPIO2/D4 ──┼───────────┤  SCL     ├─────────┤  SCL     │
│             │           │          │         │          │
└─────────────┘           └──────────┘         └──────────┘
```

### Pin Connections Table

| HTU21D Pin | ESP8266 Pin | OLED Pin | Description |
| --- | --- | --- | --- |
| VCC / VIN | 3.3V | VCC | Power (3.3V) |
| GND | GND | GND | Ground |
| SDA | GPIO0 (D3) | SDA | I2C Data (shared) |
| SCL | GPIO2 (D4) | SCL | I2C Clock (shared) |

### Wiring Notes

**Shared I2C Bus**

*   Both OLED and HTU21D connect to the **same** SDA and SCL pins
*   No conflict - I2C allows multiple devices
*   Each device has a unique address:
    *   OLED: 0x3C (or 0x3D)
    *   HTU21D: 0x40

**Power Supply**

*   Use **3.3V** not 5V
*   Some modules are 5V tolerant, but 3.3V is safer
*   ESP8266 provides adequate current for both devices

**Wire Length**

*   Keep wires as short as possible (\< 20cm ideal)
*   Use same length wires for both devices if possible
*   Twisted pair for SDA/SCL reduces interference

## 🔧 Physical Mounting

### Recommended Placement

**DO:**

*   ✅ Mount sensor away from heat sources
*   ✅ Allow air circulation around sensor
*   ✅ Keep away from direct sunlight
*   ✅ Position away from ESP8266 chip (it generates heat)
*   ✅ Use standoffs or spacers for airflow

**DON'T:**

*   ❌ Mount directly against ESP8266 board
*   ❌ Cover sensor in enclosure without ventilation
*   ❌ Place near voltage regulator or WiFi chip
*   ❌ Expose to moisture (unless using sealed version)

### Example Mounting

```
┌─────────────────────────────────┐
│                                 │
│  [HTU21D Sensor]                │  ← Away from heat sources
│                                 │
│                                 │
│         [ESP8266]               │
│                                 │
│         [OLED Display]          │
│                                 │
└─────────────────────────────────┘

Side spacing: 2-3cm minimum between sensor and ESP8266
```

## 💻 Software Setup

### Required Library

Install the **Adafruit HTU21DF Library**:

1.  Open Arduino IDE
2.  Go to **Sketch > Include Library > Manage Libraries...**
3.  Search for: **Adafruit HTU21DF**
4.  Install library by **Adafruit**
5.  Also install dependencies when prompted:
    *   Adafruit Unified Sensor

### Add to Your Project

1.  Copy `HTU21DService.h` to your project folder
2.  That's it! The sensor initializes automatically

### How It Works

The code automatically:

*   ✅ Detects if HTU21D is connected
*   ✅ Falls back gracefully if sensor not found
*   ✅ Adds 4th display mode if sensor available
*   ✅ Uses local temperature instead of API data
*   ✅ Updates readings every 5 seconds

**Successful Connection:**

```
Sensor Init...
HTU21D sensor found and initialized
=== HTU21D Sensor Readings ===
Temperature: 23.5°C (74.3°F)
Humidity: 45.2%
Dew Point: 11.3°C
Heat Index: 23.7°C
Comfort Level: Comfortable
==============================
```

**Sensor Not Found:**

```
Sensor Init...
HTU21D sensor not found!
Check wiring:
  VCC -> 3.3V
  GND -> GND
  SDA -> GPIO0 (D3)
  SCL -> GPIO2 (D4)
Continuing without local sensor...
```

## 📐 Advanced Features

### Temperature Calibration

If readings are consistently off, add calibration offset:

Edit `HTU21DService.h`:

```cpp
float getTemperature() const {
    return m_temperature + 0.5;  // Add offset (adjust as needed)
}
```

### Change Update Interval

Default: 5 seconds

In `weather_clock.ino`:

```cpp
const unsigned long sensorInterval = 10000;  // 10 seconds
```

Or set dynamically:

```cpp
localSensor.setReadInterval(10000);  // 10 seconds
```

## 📊 Sensor Specifications

| Parameter | Specification |
| --- | --- |
| Temperature Range | \-40°C to +125°C |
| Temperature Accuracy | ±0.3°C (typical) |
| Humidity Range | 0% to 100% RH |
| Humidity Accuracy | ±2% RH (typical) |
| Response Time | 5-30 seconds |
| I2C Address | 0x40 (fixed) |
| Supply Voltage | 3.3V |
| Supply Current | 500µA (measuring) |
| Interface | I2C (TWI) |

## 💡 Home Monitoring Ideas

With the HTU21D sensor measuring your **indoor climate**, you can:

## 📝 Quick Reference

### Ideal Indoor Home Climate

| Room Type | Temperature | Humidity | Notes |
| --- | --- | --- | --- |
| **Living Room** | 20-22°C | 40-60% | General comfort |
| **Bedroom** | 16-19°C | 30-50% | Better sleep quality |
| **Bathroom** | 20-23°C | 50-70% | Higher OK when showering |
| **Kitchen** | 18-20°C | 40-60% | Can spike during cooking |
| **Home Office** | 20-22°C | 40-60% | Productivity optimal |
| **Baby's Room** | 18-20°C | 30-50% | Consistent is key |

### Home Humidity Guidelines

| Humidity Level | Indoor Condition | What to Do |
| --- | --- | --- |
| **\< 30%** | Too dry | Use humidifier, water plants |
| **30-40%** | Low-normal | Acceptable, monitor |
| **40-60%** | **Ideal** | Perfect comfort zone |
| **60-70%** | High | Improve ventilation |
| **\> 70%** | Too humid | Risk of mold - use dehumidifier |

### Comfort Levels (Indoor)

| Condition | Temperature | Humidity | Comfort |
| --- | --- | --- | --- |
| Cold | \< 15°C | Any | Too cold - increase heat |
| Cool | 15-20°C | 30-60% | Cool but tolerable |
| Comfortable | 20-24°C | 30-60% | Ideal home comfort |
| Warm | 24-27°C | 30-60% | Warm - consider cooling |
| Hot | 27-32°C | Any | Too hot - needs AC |
| Too Humid | Any | \> 70% | Uncomfortable - needs dehumidifier |
| Too Dry | Any | \< 30% | Dry air - needs humidifier |

### Heat Index Guide (Indoor Feel)

*   **\< 27°C**: No discomfort
*   **27-32°C**: Caution - possible fatigue (check AC/fans)
*   **32-41°C**: Extreme caution - heat exhaustion possible
*   **\> 41°C**: Danger - immediate cooling needed

---

**Ready to add the sensor?** Follow the wiring diagram, install the library, and upload the code!

```
┌──────────────────────┐
│  Home Climate        │
│                      │
│       23.5°C         │  ← Indoor temp
│                      │
│  Humidity: 45%       │  ← Indoor humidity
│  Feel: Comfortable   │  ← Home comfort
│                      │
│       14:35:42       │  ← Time
└──────────────────────┘
```