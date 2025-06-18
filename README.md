# ChatESP

<p align="center">
  <img src="chatESP-github-logo.png" width="160" alt="ChatESP logo">
</p>

ChatESP turns an ESP32 board with a small OLED display into a pocket-sized weather station and ChatGPT interface. Forecasts come from Open-Meteo, and ChatGPT replies appear directly on the screen.

---

## Features

- Weather screen showing current, min and max temperatures with a simple icon
- Progress bar indicates time until the next update
- ChatGPT responses typed out on the display
- `IMAGE:` prompts draw 32×32 monochrome bitmaps
- Supports boards with built-in OLEDs or external SSD1306 modules

## Hardware

- **ESP32 board** (tested with Wemos Lolin32)
- **0.96" SSD1306 OLED display**
  - Built-in screens – SDA on GPIO&nbsp;5, SCL on GPIO&nbsp;4  
    ![Lolin32 board](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/07/Lolin32-OLED.jpg?w=750&quality=100&strip=all&ssl=1)
  - External displays – connect VIN to 3.3&nbsp;V, GND to GND, SDA to GPIO&nbsp;21 and SCL to GPIO&nbsp;22  
    ![OLED wiring](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266_oled_display_wiring.png?quality=100&strip=all&ssl=1)

## Getting started

### Arduino IDE 2
1. [Install Arduino IDE 2](https://www.arduino.cc/en/software) and any USB drivers your board requires.
2. In **Preferences** add `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` to *Additional Board Manager URLs*.
3. In **Boards Manager** install **ESP32 by Espressif Systems**.
4. Via the *Library Manager* install **Adafruit SSD1306**, **Adafruit GFX** and **ArduinoJson**.

### Project setup
1. Clone this repository and open `ESP32_CHAT/ESP32_CHAT.ino`.
2. Copy `ESP32_CHAT/secrets.example.h` to `ESP32_CHAT/secrets.h`.
3. Edit `secrets.h` with your Wi‑Fi details, OpenAI key and location:
   ```c++
   #define WIFI_SSID "YourWiFiSSID"
   #define WIFI_PASS "YourWiFiPassword"
   #define OPENAI_API_KEY "sk-..."
   #define PLACE_NAME "City"
   #define PLACE_LAT  "12.34"
   #define PLACE_LNG  "56.78"
   ```
4. Select the correct board and port and upload the sketch.

## Usage

- The weather page refreshes roughly every 30&nbsp;seconds.
- Open the serial monitor at 115200&nbsp;baud to send prompts.  
  *Tip:* start a prompt with `IMAGE:` to draw a 32×32 bitmap.

---

Released under the MIT License.
