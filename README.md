# ESP32 Autonomous IoT Car

This project is an ESP32-based autonomous car platform with web-based control. It allows you to steer, throttle, and manage the car remotely via a WiFi access point and a modern web interface.

## Features
- WiFi Access Point mode (no external router needed)
- Web interface for real-time control
- Steering and throttle control
- EEPROM-based position memory
- Configurable features via `#define` macros
- Responsive UI for mobile and desktop

## Hardware Requirements
- ESP32 Dev Board
- Motor driver (for throttle and steering)
- Stepper or servo for steering
- Power supply for motors and ESP32
- Optional: Brakes, additional sensors

## Folder Structure
```
Autonomos_Car_Esp/
├── app.h              # Main application header (pins, HTML, config)
├── Autonomos_Car_Esp.ino  # Main Arduino sketch
├── conf.h             # Configuration constants
├── README.md          # Project documentation
```

## Getting Started
1. **Clone the repository**
   ```sh
   git clone <repo-url>
   ```
2. **Open in Arduino IDE or PlatformIO**
3. **Install required libraries:**
   - WiFi
   - WebServer
   - EEPROM
4. **Configure your hardware pins in `app.h` if needed.**
5. **Upload to your ESP32 board.**

## Usage
- Power on the ESP32 car.
- Connect your phone or PC to the WiFi network:
  - SSID: `Wifi Car`
  - Password: `12345678`
- Open a browser and go to `192.168.4.1`
- Use the web interface to control the car:
  - Arrow buttons: Move/steer
  - Sliders: Adjust speed and steering speed
  - Home buttons: Auto/manual steering reset

## Customization
- Edit `app.h` and `conf.h` to change pins, WiFi credentials, or enable/disable features.
- The web interface HTML is inside `app.h` as a raw string literal.
- Use `#define` macros to include/exclude features at compile time.

## License
MIT License

## Credits
- Developed by CUERT-2024-2025 Team
- Based on ESP32 Arduino framework
