# ChatESP

<p align="center">
  <img src="chatESP-github-logo.png" width="160" alt="ChatESP logo">
</p>

ChatESP transforms an ESP32 development board equipped with a small OLED display into a compact weather station and ChatGPT client. It retrieves real-time weather data and shows ChatGPT replies directly on the screen.

## Features

- Current temperature and daily min/max from the Open‑Meteo API
- Simple sun/rain icons with a progress bar for updates
- Typewriter effect for ChatGPT responses
- `IMAGE:` prefix draws a 32×32 monochrome bitmap
- Works with built-in or external SSD1306 displays

### (!) Future features planned (!):

- 🏠 Integration with [Home Assistant](https://www.home-assistant.io/) for IoT control
- 🎤 Microphone input for voice prompts
- 📷 Camera input for snapshot prompts (e.g. clothing suggestions)
- 🖥️ Larger display support for calendar or full weather report
- 🔊 Small speaker for alerts
- 💡 Reactive LEDs for notifications
- 🪞 Smart mirror with two-way glass to display ChatESP replies, weather, and alerts

## Hardware

### Built-in OLED (e.g. Wemos Lolin32)

![Wemos Lolin32](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/07/Lolin32-OLED.jpg?w=750&quality=100&strip=all&ssl=1)

- SDA on GPIO 5
- SCL on GPIO 4

### External OLED wiring

![External display wiring](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266_oled_display_wiring.png?quality=100&strip=all&ssl=1)

- Vin → 3.3&nbsp;V
- GND → GND
- SDA → GPIO 21
- SCL → GPIO 22

If your board uses different pins, edit the `Wire.begin()` call in `ESP32_CHAT.ino` accordingly.

## Arduino IDE 2

1. [Download Arduino IDE&nbsp;2](https://www.arduino.cc/en/software) and install any required drivers.
2. Open **File → Preferences** and add:
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   to *Additional Board Manager URLs*.
3. In **Tools → Board → Boards Manager**, install **ESP32 by Espressif Systems**.
4. Install the **Adafruit SSD1306**, **Adafruit GFX** and **ArduinoJson** libraries via the library manager.

## Project setup

1. Clone this repository and open `ESP32_CHAT/ESP32_CHAT.ino`.
2. Copy `ESP32_CHAT/secrets.example.h` to `ESP32_CHAT/secrets.h`.
3. Edit `secrets.h` and set your Wi‑Fi credentials, OpenAI API key and location:
   ```c++
   #define WIFI_SSID "YourWiFiSSID"
   #define WIFI_PASS "YourWiFiPassword"
   #define OPENAI_API_KEY "sk-..."
   #define PLACE_NAME "City"
   #define PLACE_LAT  "12.34"
   #define PLACE_LNG  "56.78"
   ```
4. Select your board and COM port, then compile and upload.

## Usage

- The device displays weather information and refreshes roughly every 30&nbsp;s.
- Open the Serial Monitor at 115200&nbsp;baud to send prompts.
- Start a prompt with `IMAGE:` to get a small bitmap.

## License

MIT
