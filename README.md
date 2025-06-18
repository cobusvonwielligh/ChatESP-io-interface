# ChatESP

![ChatESP logo](chatESP-github-logo.png)

ChatESP transforms an ESP32 equipped with a small OLED display into a compact weather station and ChatGPT client. The device presents the current forecast and allows you to exchange short messages with ChatGPT directly on the screen.

## Features

- Current temperature and daily min/max values pulled from the Open‑Meteo API
- Simple weather icons
- ChatGPT responses printed on the display with a typing effect
- `IMAGE:` prompts return a 32×32 monochrome bitmap
- Works with boards that have a built‑in OLED as well as external 128×64 I²C modules

## Hardware requirements

- ESP32 development board
- 0.96" SSD1306 OLED display
  - **Built‑in displays** (for example Wemos Lolin32): SDA on GPIO&nbsp;5, SCL on GPIO&nbsp;4  
    ![Wemos Lolin32](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/07/Lolin32-OLED.jpg?w=750&quality=100&strip=all&ssl=1)
  - **External displays**: connect Vin to 3.3&nbsp;V, GND to GND, SDA to GPIO&nbsp;21 and SCL to GPIO&nbsp;22 by default  
    ![External display wiring](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266_oled_display_wiring.png?quality=100&strip=all&ssl=1)

## Arduino IDE 2 setup

1. [Download Arduino IDE&nbsp;2](https://www.arduino.cc/en/software) and install any USB driver required by your board (CP210x or CH340).
2. Open **File → Preferences** and add the following URL to *Additional Board Manager URLs*:
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. Open **Tools → Board → Boards Manager** (or click the **Boards Manager** icon), search for **esp32** and install *ESP32 by Espressif Systems*.
4. Install the libraries **Adafruit SSD1306**, **Adafruit GFX** and **ArduinoJson** via **Sketch → Include Library → Manage Libraries**.

## Project setup

1. Clone this repository and open `ESP32_CHAT/ESP32_CHAT.ino` in the Arduino IDE.
2. Copy `ESP32_CHAT/secrets.example.h` to `ESP32_CHAT/secrets.h`.
3. Edit `secrets.h` and fill in:
   ```c++
   #define WIFI_SSID "YourWiFiSSID"
   #define WIFI_PASS "YourWiFiPassword"
   #define OPENAI_API_KEY "sk-..."
   #define PLACE_NAME "City name"
   #define PLACE_LAT  "12.34"
   #define PLACE_LNG  "56.78"
   ```
4. Select your ESP32 board and the correct COM port, then compile and upload.

## Usage

- On boot the device shows the weather and updates about every 30 seconds.
- Open the serial monitor (115200 baud) to send ChatGPT prompts.
- Prefix a prompt with `IMAGE:` to request a small bitmap.

## License

Released under the MIT License.
